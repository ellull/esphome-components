#include "esphome/core/component.h"
#include "esphome/components/sensor/sensor.h"
#include "jiecang_desk_sensor.h"

namespace esphome {
namespace jiecang_desk {

static const char *const TAG = "jiecang_desk";

void JiecangDeskHeightSensor::dump_config() { LOG_SENSOR("  ", "height_sensor", this); }

}  // namespace jiecang_desk
}  // namespace esphome