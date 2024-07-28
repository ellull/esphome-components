#include "jiecang_desk_sensor.h"

namespace esphome {
namespace jiecang_desk {

static const char *const TAG = "jiecang_desk.sensor";

void JiecangDeskHeightSensor::dump_config() { LOG_SENSOR("", "Jiecang Desk Sensor", this); }

void JiecangDeskHeightSensor::set_height(const int height) {
    if (height == this->height_)
        return;

    this->height_ = height;
    this->publish_state((float)height * 0.1);
}

}  // namespace jiecang_desk
}  // namespace esphome