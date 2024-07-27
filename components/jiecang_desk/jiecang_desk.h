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
 public:
  void set_height_sensor(JiecangDeskHeightSensor *height_sensor) { height_sensor_ = height_sensor; }

 protected:
  JiecangDeskHeightSensor *height_sensor_{nullptr};
#endif

 private:
   int read_packet_(uint8_t *buffer, const int len);
   void write_packet_(const uint8_t *buffer, const int len);

   uint8_t checksum_(const uint8_t *buffer, const int len);

   void process_response_(const uint8_t response, const int params_len, const uint8_t *params);
   void send_command_(const uint8_t command, const int params_len, const uint8_t *params);
   void send_command_(const uint8_t command);
};

}  // namespace jiecang_desk
}  // namespace esphome