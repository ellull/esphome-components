#include <sstream>
#include <iostream>
#include <iomanip>
#include "esphome/core/log.h"
#include "jiecang_desk.h"

namespace esphome {
namespace jiecang_desk {

static const char *const TAG = "jiecang_desk";

static const uint8_t BYTE_SEND_ADRESS = 0xF1; 
static const uint8_t BYTE_RECV_ADDRESS = 0xF2;
static const uint8_t BYTE_EOM = 0x7E;

static const int MAX_PARAMS_LENGTH = 4;

static const int POS_COMMAND = 2;
static const int POS_PARAMS_LENGTH = 3;
static const int POS_PARAMS = 4;

static const uint8_t RESPONSE_HEIGHT = 0x01;

std::string uint8_to_hex_string(const uint8_t *v, const int s) {
  std::stringstream ss;

  ss << std::hex << std::setfill('0');

  for (int i = 0; i < s; i++) {
    ss << std::hex << std::setw(2) << static_cast<int>(v[i]);
  }

  return ss.str();
}

void JiecangDeskComponent::dump_config() {
  ESP_LOGCONFIG(TAG, "Jiecang Desk component:");
  this->check_uart_settings(9600, 1, esphome::uart::UART_CONFIG_PARITY_NONE, 8);
}

void JiecangDeskComponent::setup() {
  ESP_LOGCONFIG(TAG, "Setting up Jiecang Desk sensor...");
  if (!this->height_listeners_.empty())
    this->send_command(COMMAND_SETTINGS);
}

void JiecangDeskComponent::loop() {
  static uint8_t buffer[16];
  static int packet_len;

  while (this->available()) {
    packet_len = this->read_packet_(buffer, sizeof(buffer));
    if (packet_len > 0) {
      ESP_LOGD(TAG, "Processing response %s", uint8_to_hex_string(buffer, packet_len).c_str());
      this->process_response_(buffer[POS_COMMAND], buffer[POS_PARAMS_LENGTH], &buffer[POS_PARAMS]);
    }
  }
}

void JiecangDeskComponent::send_command(const uint8_t command, const int params_len, const uint8_t *params) {
  static u_int8_t buffer[8] = { BYTE_SEND_ADRESS, BYTE_SEND_ADRESS, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };

  buffer[POS_COMMAND] = command;
  buffer[POS_PARAMS_LENGTH] = params_len;
  for (int i = 0; i < params_len; i++) {
    buffer[POS_PARAMS + i] = params[i];
  }
  buffer[POS_PARAMS + params_len] = this->checksum_(&buffer[POS_COMMAND], params_len + 2);
  buffer[POS_PARAMS + params_len + 1] = BYTE_EOM;
  
  this->write_packet_(buffer, 6 + params_len);
}

void JiecangDeskComponent::send_command(const uint8_t command) {
  this->send_command(command, 0, {});
}

int JiecangDeskComponent::read_packet_(uint8_t *buffer, const int len) {
  static int pos = 0;
  static PacketState state = PacketState::RECV_ADDRESS;
  static int params_len = 0;

  auto reset_state = [&](const char* error) {
    if (error != nullptr) {
      ESP_LOGE(TAG, "Failed reading packet (%s): buffer = %s", error, uint8_to_hex_string(buffer, len).c_str());
    }

    pos = 0;
    state = PacketState::RECV_ADDRESS;
    params_len = 0;
  };

  // Read next byte and add it to the buffer.
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
    if (rx_data != BYTE_RECV_ADDRESS) {
      reset_state("wrong address byte");
      return -1;
    }

    if (pos == POS_COMMAND) {
      state = PacketState::RECV_COMMAND;
    }
    break;
  
  // Read the response
  case PacketState::RECV_COMMAND:
    state = PacketState::RECV_PARAMS_LENGTH;
    break;

  // Read the params length
  case PacketState::RECV_PARAMS_LENGTH:
    params_len = rx_data;
    if (params_len > MAX_PARAMS_LENGTH) {
      reset_state("invalid params length");
      return -1;
    }
    state = params_len > 0 ? PacketState::RECV_PARAMS : PacketState::REVC_CHECKSUM;
    break;

  // Read the params
  case PacketState::RECV_PARAMS:
    if (pos == POS_PARAMS + params_len) {
      state = PacketState::REVC_CHECKSUM;
    }
    break;
  
  // Read and validate the checksum
  case PacketState::REVC_CHECKSUM:
    if (this->checksum_(&buffer[POS_COMMAND], params_len + 2) != rx_data) {
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

void JiecangDeskComponent::write_packet_(const uint8_t *buffer, const int len) {
  ESP_LOGD(TAG, "Sending command %s", uint8_to_hex_string(buffer, len).c_str());
  this->write_array(buffer, len);
}

uint8_t JiecangDeskComponent::checksum_(const uint8_t *buffer, const int len) {
  uint8_t sum = 0;
  for (int i = 0; i < len; i++) {
    sum += buffer[i];
  }
  return sum;
}

void JiecangDeskComponent::process_response_(const uint8_t response, const int params_len, const uint8_t *params) {

  switch (response)
  {
  case RESPONSE_HEIGHT: {
    if (params_len != 3)  // Height response must have three params.
      return;

    int height = (params[0] << 8 | params[1]);
    if (this->prev_height_ == height)
      return;

    for (auto *listener : this->height_listeners_)
      listener->update_height(height);
    this->prev_height_ = height;
    break;
  }

  default:
    ESP_LOGD(TAG, "unknown response 0x%02X", response);
  }
}

}  // namespace jiecang_desk
}  // namespace esphome
