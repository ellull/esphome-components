#include "jiecang_desk_limit_sensors.h"

namespace esphome {
namespace jiecang_desk {

static const char *const TAG = "jiecang_desk.sensor";

void JiecangDeskMaxLimitSensor::dump_config() { LOG_SENSOR("", "Jiecang Desk Max Limit Sensor", this); }

void JiecangDeskMaxLimitSensor::on_limits_update(const std::tuple<optional<int>, optional<int>> limits) {
  optional<int> limit = std::get<0>(limits);
  float new_limit = limit.has_value() ? *limit * 0.1f : NAN;
  this->publish_state(new_limit);
}

void JiecangDeskMinLimitSensor::dump_config() { LOG_SENSOR("", "Jiecang Desk Min Limit Sensor", this); }

void JiecangDeskMinLimitSensor::on_limits_update(const std::tuple<optional<int>, optional<int>> limits) {
  optional<int> limit = std::get<1>(limits);
  float new_limit = limit.has_value() ? *limit * 0.1f : NAN;
  this->publish_state(new_limit);
}

}  // namespace jiecang_desk
}  // namespace esphome