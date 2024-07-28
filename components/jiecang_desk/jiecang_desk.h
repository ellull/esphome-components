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
  virtual void set_height(const int value) = 0;
};

class JiecangDeskLimitsListener {
 public:
  virtual void set_physical_limits(const int max, const int min) = 0;
  virtual void set_configured_max(const int value) = 0;
  virtual void set_configured_min(const int value) = 0;
};

class JiecangDeskComponent : public Component, public uart::UARTDevice {
 public:
  void dump_config() override;
  void setup() override;
  void loop() override;
  void add_height_listener(JiecangDeskHeightListener *listener) { height_listeners_.push_back(listener); }
  void add_limits_listener(JiecangDeskLimitsListener *listener) { limit_listeners_.push_back(listener); }
  void send_command(const uint8_t command, const int params_len, const uint8_t *params);
  void send_command(const uint8_t command);

 private:
  int prev_height_;
  std::vector<JiecangDeskHeightListener *> height_listeners_;
  std::vector<JiecangDeskLimitsListener *> limit_listeners_;

  int read_packet_(uint8_t *buffer, const int len);
  void write_packet_(const uint8_t *buffer, const int len);
  uint8_t checksum_(const uint8_t *buffer, const int len);
  void process_response_(const uint8_t response, const int params_len, const uint8_t *params);
};

}  // namespace jiecang_desk
}  // namespace esphome