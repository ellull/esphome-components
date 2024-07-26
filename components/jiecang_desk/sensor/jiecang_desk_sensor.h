#pragma once

#include "esphome/core/component.h"
#include "esphome/components/sensor/sensor.h"

namespace esphome {
namespace jiecang_desk {

class JiecangDeskHeightSensor : public Component, public sensor::Sensor {
 public:
   void dump_config() override;
   
 protected:
};

}  // namespace jiecang_desk
}  // namespace esphome