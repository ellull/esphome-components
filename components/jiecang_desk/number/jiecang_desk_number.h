#pragma once

#include "esphome/core/component.h"
#include "esphome/components/number/number.h"
#include "../jiecang_desk.h"

namespace esphome {
namespace jiecang_desk {

class JiecangDeskNumber : public number::Number, public Component, public JiecangDeskHeightListener, public JiecangDeskLimitsListener {
 public:
  void dump_config() override;

  void set_parent(JiecangDeskComponent *parent) { this->parent_ = parent; }
   
 protected:
  void control(const float value) override;
  void update_state() override;

  JiecangDeskComponent *parent_{nullptr};

 private:
  int height_;
  optional<int> physical_max_;
  optional<int> physical_min_;
  optional<int> configured_max_;
  optional<int> configured_min_;

  optional<int> get_limit_max() { return this->configured_max_.has_value() ? this->configured_max_ : this->physical_max_; }
  optional<int> get_limit_min() { return this->configured_min_.has_value() ? this->configured_min_ : this->physical_min_; }
};

}  // namespace jiecang_desk
}  // namespace esphome