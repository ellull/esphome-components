#pragma once

#include "esphome/core/component.h"
#include "esphome/components/cover/cover.h"
#include "../jiecang_desk.h"

namespace esphome {
namespace jiecang_desk {

class JiecangDeskCover : public cover::Cover, public Component, public JiecangDeskHeightListener, public JiecangDeskLimitsListener {
 public:
  void dump_config() override;
  void setup() override;
  cover::CoverTraits get_traits() override;

  void set_height(const int value) { this->height_ = value; this->update_state(); }
  void set_physical_limits(const int max, const int min) { this->physical_max_ = max; this->physical_min_ = min; this->update_state(); }
  void set_configured_max(const int value) { this->configured_max_ = value; this->update_state(); }
  void set_configured_min(const int value) { this->configured_min_ = value; this->update_state(); }
  void set_parent(JiecangDeskComponent *parent) { this->parent_ = parent; }
   
 protected:
  void control(const cover::CoverCall &call) override;
  void update_state();
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