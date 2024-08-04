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

static const uint8_t COMMAND_SETTINGS = 0x07;
static const uint8_t COMMAND_PHYSICAL_LIMITS = 0x0C;
static const uint8_t COMMAND_SET_HEIGHT = 0x1B;
static const uint8_t COMMAND_LIMITS = 0x20;

static const uint8_t RESPONSE_HEIGHT = 0x01;
static const uint8_t RESPONSE_PHYSICAL_LIMITS = 0x07;
static const uint8_t RESPONSE_SET_HEIGHT = 0x1B;
static const uint8_t RESPONSE_LIMITS = 0x20;
static const uint8_t RESPONSE_MAX_LIMIT = 0x21;
static const uint8_t RESPONSE_MIN_LIMIT = 0x22;
static const uint8_t RESPONSE_POSITION_1 = 0x25;
static const uint8_t RESPONSE_POSITION_2 = 0x26;
static const uint8_t RESPONSE_POSITION_3 = 0x27;
static const uint8_t RESPONSE_POSITION_4 = 0x28;

static const uint8_t MASK_MAX_LIMIT = 0x01;
static const uint8_t MASK_MIN_LIMIT = 0x10;

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
  LOG_UPDATE_INTERVAL(this);
  this->check_uart_settings(9600, 1, esphome::uart::UART_CONFIG_PARITY_NONE, 8);
}

void JiecangDeskComponent::setup() {
  this->status_set_warning("waiting for configuration");

  // Request settings and physical limits (which will trigger request of configured limits).
  this->send_command_(COMMAND_SETTINGS);
  this->update();
}

void JiecangDeskComponent::update() {
  this->send_command_(COMMAND_PHYSICAL_LIMITS);
}

void JiecangDeskComponent::loop() {
  static uint8_t buffer[16];
  static int packet_len;

  // Read and process incoming packets.
  while (this->available()) {
    packet_len = this->read_packet_(buffer, sizeof(buffer));
    if (packet_len > 0) {
      ESP_LOGD(TAG, "Processing packet %s", uint8_to_hex_string(buffer, packet_len).c_str());
      this->process_response_(buffer[POS_COMMAND], buffer[POS_PARAMS_LENGTH], &buffer[POS_PARAMS]);
    }
  }
}

void JiecangDeskComponent::send_command(const JiecangDeskCommand command) {
  this->send_command_(command);
};

void JiecangDeskComponent::move_to(const int height) {
    uint8_t bytes[2] = { (uint8_t)(height >> 8), (uint8_t)(height & 0xFF) };
    this->send_command_(COMMAND_SET_HEIGHT, sizeof(bytes), bytes);
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

  ESP_LOGV(TAG, "received byte 0x%02X", rx_data);

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
  ESP_LOGD(TAG, "Sending packet %s", uint8_to_hex_string(buffer, len).c_str());
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
  ESP_LOGD(TAG, "Processing response 0x%02X", response);

  switch (response)
  {
  case RESPONSE_HEIGHT: {
    if (params_len != 3)  // Height response must have three params (although third param is always ignored).
      return;

    this->set_height_(params[0] << 8 | params[1]);
    break;
  }

  case RESPONSE_PHYSICAL_LIMITS: {
    if (params_len != 4)  // Physical limits response must have four params.
      return;

    this->set_physical_limits_(params[0] << 8 | params[1], params[2] << 8 | params[3]);

    // Request configured limits
    this->send_command_(COMMAND_LIMITS);
    break;
  }

  case RESPONSE_LIMITS: {
    uint8_t limits_flags = params[0];

    if ((limits_flags & MASK_MAX_LIMIT) == 0) {
      this->set_configured_max_(nullopt);
    }

    if ((limits_flags & MASK_MIN_LIMIT) == 0) {
      this->set_configured_min_(nullopt);
    }
    break;
  }

  case RESPONSE_MAX_LIMIT: {
    if (params_len != 2)  // Physical limits response must have four params.
      return;

    this->set_configured_max_(params[0] << 8 | params[1]);
    break;
  }

  case RESPONSE_MIN_LIMIT: {
    if (params_len != 2)  // Physical limits response must have four params.
      return;

    this->set_configured_min_(params[0] << 8 | params[1]);
    break;
  }

  case RESPONSE_SET_HEIGHT:
  case RESPONSE_POSITION_1:
  case RESPONSE_POSITION_2:
  case RESPONSE_POSITION_3:
  case RESPONSE_POSITION_4:
    ESP_LOGD(TAG, "Ignored response 0x%02X", response);
    break;

  default:
    ESP_LOGW(TAG, "Unknown response 0x%02X", response);
  }
}

void JiecangDeskComponent::send_command_(const uint8_t command, const int params_len, const uint8_t *params) {
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

void JiecangDeskComponent::send_command_(const uint8_t command) {
  this->send_command_(command, 0, {});
}

void JiecangDeskComponent::set_height_(const optional<int> value) {
  auto prev_height = this->height_;

  if (prev_height != value) {
    ESP_LOGD(TAG, "Setting height to %d", *value);
    this->height_ = value;
  }

  this->notify_height_update_(prev_height);
}

void JiecangDeskComponent::set_physical_limits_(const int max, const int min) {
  auto prev_limits = this->get_limits();

  if (this->physical_max_ != max) {
    ESP_LOGD(TAG, "Setting physical max limit to %d", max);
    this->physical_max_ = max;
  }

  if (this->physical_min_ != min) {
    ESP_LOGD(TAG, "Setting physical min limit to %d", min);
    this->physical_min_ = min;
  }

  this->notify_limits_update_(prev_limits);
}

void JiecangDeskComponent::set_configured_max_(const optional<int> value) {
  auto prev_limits = this->get_limits();

  if (this->configured_max_ != value) {
    ESP_LOGD(TAG, "Setting configured max limit to %d", value);
    this->configured_max_ = value;
  }
  this->has_configured_max_ = value.has_value();

  this->notify_limits_update_(prev_limits);
}

void JiecangDeskComponent::set_configured_min_(const optional<int> value) {
  auto prev_limits = this->get_limits();

  if (this->configured_min_ != value) {
    ESP_LOGD(TAG, "Setting configured min limit to %d", value);
    this->configured_min_ = value;
  }
  this->has_configured_min_ = value.has_value();

  this->notify_limits_update_(prev_limits);
}

void JiecangDeskComponent::notify_height_update_(const optional<int> prev_height) {
  auto curr_height = this->height_;
  for (auto *listener : this->listeners_) {
    if (listener->notify_all_height_updates() || curr_height != prev_height) {
      listener->on_height_update(curr_height);
    }
  }

  if (this->is_configured()) {
    this->status_clear_warning();
  }
}

void JiecangDeskComponent::notify_limits_update_(const std::tuple<optional<int>, optional<int>> prev_limits) {
  auto curr_limits = this->get_limits();
  if (curr_limits != prev_limits) {
    for (auto *listener : this->listeners_) {
      listener->on_limits_update(curr_limits);
    }
  }

  if (this->is_configured()) {
    this->status_clear_warning();
  }
}

}  // namespace jiecang_desk
}  // namespace esphome
