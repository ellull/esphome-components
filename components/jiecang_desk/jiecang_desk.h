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

enum JiecangDeskCommand : uint8_t {
  UP = 0x01,
  DOWN = 0x02,
  POSITION_1 = 0x05,
  POSITION_2 = 0x06,
  POSITION_3 = 0x27,
  POSITION_4 = 0x28,
  STOP = 0x2B,
};

class JiecangDeskListener {
  public:
  
    virtual bool notify_all_height_updates() { return false; };
    virtual void on_height_update(const optional<int> height) {};
    virtual void on_limits_update(const std::tuple<optional<int>, optional<int>> limits) {};
};

class JiecangDeskComponent : public PollingComponent, public uart::UARTDevice {
 public:
  void dump_config() override;
  void setup() override;
  void update() override;
  void loop() override;

  void send_command(const JiecangDeskCommand command);
  void move_to(const int height);

  optional<int> get_height() { return this->height_; }
  std::tuple<optional<int>, optional<int>> get_limits() {
    return {
      this->configured_max_.has_value() ? this->configured_max_ : this->physical_max_,
      this->configured_min_.has_value() ? this->configured_min_ : this->physical_min_
    };
  }

  void add_listener(JiecangDeskListener *listener) { listeners_.push_back(listener); }

 private:
  optional<int> height_;
  optional<int> physical_max_;
  optional<int> physical_min_;
  optional<int> configured_max_;
  optional<int> configured_min_;
  std::vector<JiecangDeskListener *> listeners_;

  int read_packet_(uint8_t *buffer, const int len);
  void write_packet_(const uint8_t *buffer, const int len);
  uint8_t checksum_(const uint8_t *buffer, const int len);
  void process_response_(const uint8_t response, const int params_len, const uint8_t *params);
  void send_command_(const uint8_t command, const int params_len, const uint8_t *params);
  void send_command_(const uint8_t command);

  void set_height_(const optional<int> value);
  void set_physical_limits_(const int max, const int min);
  void set_configured_max_(const optional<int> value);
  void set_configured_min_(const optional<int> value);
  void notify_height_update_(const optional<int> value);
  void notify_limits_update_(const std::tuple<optional<int>, optional<int>> prev_limits);
};

}  // namespace jiecang_desk
}  // namespace esphome