#pragma once

#include "esphome/core/component.h"
#include "esphome/components/sensor/sensor.h"
#include "../jiecang_desk.h"

namespace esphome {
namespace jiecang_desk {

class JiecangDeskHeightSensor : public sensor::Sensor, public Component, public JiecangDeskListener {
 public:
   void dump_config() override;
   void on_height_update(const optional<int> height) override;
};

}  // namespace jiecang_desk
}  // namespace esphome