#include "jiecang_desk_sensor.h"

namespace esphome {
namespace jiecang_desk {

static const char *const TAG = "jiecang_desk.sensor";

void JiecangDeskHeightSensor::dump_config() { LOG_SENSOR("", "Jiecang Desk Sensor", this); }

void JiecangDeskHeightSensor::update_state() {
  float new_state = (float)this->height_ * 0.1F;

  if (this->state != new_state)
    this->publish_state(new_state);
}

}  // namespace jiecang_desk
}  // namespace esphome