#pragma once

#include "esphome/core/component.h"
#include "esphome/components/button/button.h"
#include "../jiecang_desk.h"

namespace esphome {
namespace jiecang_desk {

class JiecangDeskCommandButton : public button::Button, public Component {
 public:
   void dump_config() override;

   void set_parent(JiecangDeskComponent *parent) { this->parent_ = parent; }
   void set_command(uint8_t command) { this->command_ = command; }
   
 protected:
  void press_action() override;
  
  JiecangDeskComponent *parent_{nullptr};
  uint8_t command_;
};

}  // namespace jiecang_desk
}  // namespace esphome