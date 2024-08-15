#pragma once

#include "esphome/core/component.h"
#include "esphome/components/binary_sensor/binary_sensor.h"

namespace esphome {
namespace problem {

class ProblemBinarySensor : public binary_sensor::BinarySensor, public Component {
 public:
  void loop() override;

  void setup() override;
  void dump_config() override;

  float get_setup_priority() const override { return setup_priority::DATA; }
};

}  // namespace problem
}  // namespace esphome
