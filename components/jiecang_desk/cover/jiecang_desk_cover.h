#pragma once

#include "esphome/core/component.h"
#include "esphome/components/cover/cover.h"
//#include "../jiecang_desk.h"

namespace esphome {
namespace jiecang_desk {

class JiecangDeskCover : public cover::Cover, public Component {
 public:
  void setup() override;
  cover::CoverTraits get_traits() override;
  //void set_parent(JiecangDeskComponent *parent) { this->parent_ = parent; }
   
 protected:
  void control(const cover::CoverCall &call) override;
  //JiecangDeskComponent *parent_{nullptr};
};

}  // namespace jiecang_desk
}  // namespace esphome