#pragma once

#include "esphome/core/component.h"
#include "esphome/components/number/number.h"
#include "../jiecang_desk.h"

namespace esphome {
namespace jiecang_desk {

class JiecangDeskNumber : public number::Number, public Component, public JiecangDeskListener {
 public:
  void dump_config() override;

  void set_parent(JiecangDeskComponent *parent) { this->parent_ = parent; }
  void on_height_update(const optional<int> height) override;
  void on_limits_update(const std::tuple<optional<int>, optional<int>> limits) override;
  void set_reboot_on_limits_change(const bool value) { this->reboot_on_limits_change_ = value; }

 protected:
  void control(const float value) override;

  JiecangDeskComponent *parent_{nullptr};

 private:
  bool configured_{false};
  bool reboot_on_limits_change_{false};
};

}  // namespace jiecang_desk
}  // namespace esphome