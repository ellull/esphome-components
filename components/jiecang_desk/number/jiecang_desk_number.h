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
};

}  // namespace jiecang_desk
}  // namespace esphome