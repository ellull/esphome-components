#include "jiecang_desk_height_sensor.h"

namespace esphome {
namespace jiecang_desk {

static const char *const TAG = "jiecang_desk.sensor";

void JiecangDeskHeightSensor::dump_config() { LOG_SENSOR("", "Jiecang Desk Height Sensor", this); }

void JiecangDeskHeightSensor::on_height_update(const optional<int> height) {
  float new_height = height.has_value() ? *height * 0.1f : NAN;
  this->publish_state(new_height);
}

}  // namespace jiecang_desk
}  // namespace esphome