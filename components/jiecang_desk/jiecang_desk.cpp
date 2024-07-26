#include <sstream>
#include <iostream>
#include <iomanip>
#include "esphome/core/log.h"
#include "jiecang_desk.h"

namespace esphome {
namespace jiecang_desk {

static const char *const TAG = "jiecang_desk";

static const uint8_t BYTE_RECV_HEADER = 0xF2;
static const uint8_t BYTE_EOM = 0x7E;

static const int MAX_PARAMS_LENGTH = 4;

static const int POS_COMMAND = 2;
static const int POS_PARAMS_LENGTH = 3;

static const uint8_t COMMAND_HEIGHT = 0x01;

std::string uint8_to_hex_string(const uint8_t *v, const int s) {
  std::stringstream ss;

  ss << std::hex << std::setfill('0');

  for (int i = 0; i < s; i++) {
    ss << std::hex << std::setw(2) << static_cast<int>(v[i]);
  }

  return ss.str();
}

void JiecangDeskComponent::setup() {
  ESP_LOGCONFIG(TAG, "Setting up Jiecang Desk sensor...");
}

void JiecangDeskComponent::loop() {
  if (!this->available())
    return;

  static uint8_t buffer[16];
  static int packet_len;
  while (this->available()) {
    packet_len = this->read_packet_(buffer, sizeof(buffer));
    if (packet_len >= 0)
      this->process_packet_(buffer, packet_len);
  }
}

int JiecangDeskComponent::read_packet_(uint8_t *buffer, const int len) {
  static int pos = 0;
  static PacketState state = PacketState::RECV_ADDRESS;
  static int param_length = 0;

  auto reset_state = [&](const char* error) {
#ifdef ESP_LOGE
    if (error != nullptr) {
      ESP_LOGE(TAG, "Failed reading packet (%s): buffer = %s", error, uint8_to_hex_string(buffer, len).c_str());
    }
#endif
    pos = 0;
    state = PacketState::RECV_ADDRESS;
    param_length = 0;
  };

  static uint8_t rx_data;
  if (!this->read_byte(&rx_data))
    return -1;

  if (pos == len) {
    reset_state("reached end of buffer");
  }
  buffer[pos++] = rx_data;

  switch (state)
  {
  // Read the header (0xF2,0xF2)
  case PacketState::RECV_ADDRESS:
    if (rx_data != BYTE_RECV_HEADER) {
      reset_state("wrong address byte");
      return -1;
    }

    if (pos == 2) {
      state = PacketState::RECV_COMMAND;
    }
    break;
  
  // Read the command
  case PacketState::RECV_COMMAND:
    state = PacketState::RECV_PARAMS_LENGTH;
    break;

  // Read the params length
  case PacketState::RECV_PARAMS_LENGTH:
    param_length = rx_data;
    if (param_length > MAX_PARAMS_LENGTH) {
      reset_state("esceeded maximum params length");
      return -1;
    }
    state = param_length > 0 ? PacketState::RECV_PARAMS : PacketState::REVC_CHECKSUM;
    break;

  // Read the params
  case PacketState::RECV_PARAMS:
    if (pos == 4 + param_length) {
      state = PacketState::REVC_CHECKSUM;
    }
    break;
  
  // Read and validate the checksum
  case PacketState::REVC_CHECKSUM:
    if (!this->validate_packet_(buffer, param_length)) {
      reset_state("invalid checksum");
      return -1;
    }
    state = PacketState::RECV_EOM;
    break;

  // Read the EOM (0x7E)
  case PacketState::RECV_EOM:
    int prev_pos = pos;
    reset_state(nullptr);
    return rx_data == BYTE_EOM ? prev_pos : -1;
    break;
  }

  return -1;
}

bool JiecangDeskComponent::validate_packet_(const uint8_t *buffer, const int param_length) {
  int sum = 0;
  for (size_t i = 2; i < 4 + param_length; i++) {
    sum += buffer[i];
  }
  return (sum & 0xFF) == buffer[4 + param_length];
}

void JiecangDeskComponent::process_packet_(const uint8_t *buffer, const int packet_length) {
  ESP_LOGD(TAG, "Processing packet %s", uint8_to_hex_string(buffer, packet_length).c_str());

  switch (buffer[POS_COMMAND])
  {
  case COMMAND_HEIGHT:
    if (this->height_sensor_ == nullptr)
      return;
        
    if (buffer[POS_PARAMS_LENGTH] != 3)  // Height command must have three params.
      return;

    float height = (float)(buffer[4] << 8 | buffer[5]);
    this->height_sensor_->publish_state(height / 10.0);
    break;
  }
}

void JiecangDeskComponent::dump_config() {
  ESP_LOGCONFIG(TAG, "Jiecang Desk component:");
  this->check_uart_settings(9600, 1, esphome::uart::UART_CONFIG_PARITY_NONE, 8);
}

}  // namespace jiecang_desk
}  // namespace esphome
