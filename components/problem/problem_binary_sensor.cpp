#include "problem_binary_sensor.h"
#include "esphome/core/application.h"
#include "esphome/core/log.h"

namespace esphome {
namespace problem {

static const char *const TAG = "problem";

static const uint32_t STATE_MASK = STATUS_LED_WARNING | STATUS_LED_ERROR;

void ProblemBinarySensor::loop() {
  this->publish_state(App.get_app_state() & STATE_MASK);
}

void ProblemBinarySensor::setup() { this->publish_initial_state(false); }

void ProblemBinarySensor::dump_config() { LOG_BINARY_SENSOR("", "Problem Binary Sensor", this); }

}  // namespace problem
}  // namespace esphome
