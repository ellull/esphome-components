#pragma once

#include "esphome/core/component.h"
#include "esphome/components/uart/uart.h"
#ifdef USE_SENSOR
#include "sensor/jiecang_desk_sensor.h"
#endif

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

class JiecangDeskComponent : public Component, public uart::UARTDevice {
 public:
  void dump_config() override;

  void setup() override;

  void loop() override;

#ifdef USE_SENSOR
  void set_height_sensor(JiecangDeskHeightSensor *height_sensor) { height_sensor_ = height_sensor; }
#endif

 protected:
#ifdef USE_SENSOR
  JiecangDeskHeightSensor *height_sensor_{nullptr};
#endif

 private:
   int read_packet_(uint8_t *buffer, const int len);
   bool validate_packet_(const uint8_t *buffer, const int param_length);
   void process_packet_(const uint8_t *buffer, const int packet_length);
};

}  // namespace jiecang_desk
}  // namespace esphome