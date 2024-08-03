#pragma once

#include "esphome/core/component.h"
#include "esphome/components/sensor/sensor.h"
#include "../jiecang_desk.h"

namespace esphome {
namespace jiecang_desk {

class JiecangDeskMaxLimitSensor : public sensor::Sensor, public Component, public JiecangDeskListener {
 public:
   void dump_config() override;
   void on_limits_update(const std::tuple<optional<int>, optional<int>> limits) override;
};

class JiecangDeskMinLimitSensor : public sensor::Sensor, public Component, public JiecangDeskListener {
 public:
   void dump_config() override;
   void on_limits_update(const std::tuple<optional<int>, optional<int>> limits) override;
};

}  // namespace jiecang_desk
}  // namespace esphome