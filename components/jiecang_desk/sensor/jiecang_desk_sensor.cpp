#include "esphome/core/component.h"
#include "esphome/components/sensor/sensor.h"
#include "jiecang_desk_sensor.h"

namespace esphome {
namespace jiecang_desk {

static const char *const TAG = "jiecang_desk";

void JiecangDeskHeightSensor::dump_config() { LOG_SENSOR("  ", "height_sensor", this); }

void JiecangDeskHeightSensor::update_height(const int height) {
    static int prev_height = 0;

    if (height == prev_height)
        return;

    prev_height = height;
    this->publish_state((float)height * 0.1);
}

}  // namespace jiecang_desk
}  // namespace esphome