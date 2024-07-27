#pragma once

#include "esphome/core/component.h"
#include "esphome/components/sensor/sensor.h"
#include "../jiecang_desk.h"

namespace esphome {
namespace jiecang_desk {

class JiecangDeskHeightSensor : public sensor::Sensor, public Component, public JiecangDeskHeightListener {
 public:
   void dump_config() override;
   void update_height(const int height);
   
 protected:
};

}  // namespace jiecang_desk
}  // namespace esphome