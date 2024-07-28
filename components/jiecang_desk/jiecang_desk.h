#pragma once

#include "esphome/core/component.h"
#include "esphome/components/uart/uart.h"

namespace esphome {
namespace jiecang_desk {

enum PacketState {
  RECV_ADDRESS,
  RECV_COMMAND,
  RECV_PARAMS_LENGTH,
  RECV_PARAMS,
  REVC_CHECKSUM,
  RECV_EOM,
};

static const uint8_t COMMAND_SETTINGS = 0x07;
static const uint8_t COMMAND_PHYSICAL_LIMITS = 0x0C;
static const uint8_t COMMAND_SET_HEIGHT = 0x1B;
static const uint8_t COMMAND_LIMITS = 0x20;
static const uint8_t COMMAND_STOP = 0x2B;

class JiecangDeskHeightListener {
 public:
  virtual void set_height(const int value) { this->height_ = value; this->update_state(); };

 protected:
  int height_;

  virtual void update_state() = 0;
};

class JiecangDeskLimitsListener {
 public:
  virtual void set_physical_limits(const int max, const int min) { this->physical_max_ = max; this->physical_min_ = min; this->update_state(); }
  virtual void set_configured_max(const int value) { this->configured_max_ = value; this->update_state(); }
  virtual void set_configured_min(const int value) { this->configured_min_ = value; this->update_state(); }

 protected:
  optional<int> physical_max_;
  optional<int> physical_min_;
  optional<int> configured_max_;
  optional<int> configured_min_;

  optional<int> get_limit_max() { return this->configured_max_.has_value() ? this->configured_max_ : this->physical_max_; }
  optional<int> get_limit_min() { return this->configured_min_.has_value() ? this->configured_min_ : this->physical_min_; }

  virtual void update_state() = 0;
};

class JiecangDeskComponent : public Component, public uart::UARTDevice {
 public:
  void dump_config() override;
  void setup() override;
  void loop() override;

  void add_height_listener(JiecangDeskHeightListener *listener) { height_listeners_.push_back(listener); }
  void add_limits_listener(JiecangDeskLimitsListener *listener) { limit_listeners_.push_back(listener); }
  void send_command(const uint8_t command);
  void move_to(const int height);

 private:
  int prev_height_;
  std::vector<JiecangDeskHeightListener *> height_listeners_;
  std::vector<JiecangDeskLimitsListener *> limit_listeners_;

  int read_packet_(uint8_t *buffer, const int len);
  void write_packet_(const uint8_t *buffer, const int len);
  uint8_t checksum_(const uint8_t *buffer, const int len);
  void process_response_(const uint8_t response, const int params_len, const uint8_t *params);
  void send_command_(const uint8_t command, const int params_len, const uint8_t *params);
};

}  // namespace jiecang_desk
}  // namespace esphome