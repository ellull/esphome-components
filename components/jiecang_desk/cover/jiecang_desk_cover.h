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
  void update_height(const int height);
  void update_physical_max(const int height) { this->physical_max_ = height; }
  void update_physical_min(const int height) { this->physical_min_ = height; }
  void update_configured_max(const int height) { this->configured_max_ = height; }
  void update_configured_min(const int height) { this->configured_min_ = height; }
  void set_parent(JiecangDeskComponent *parent) { this->parent_ = parent; }
   
 protected:
  void control(const cover::CoverCall &call) override;
  JiecangDeskComponent *parent_{nullptr};

 private:
  int physical_max_;
  int physical_min_;
  int configured_max_;
  int configured_min_;
};

}  // namespace jiecang_desk
}  // namespace esphome