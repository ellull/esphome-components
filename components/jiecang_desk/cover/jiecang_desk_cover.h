#pragma once

#include "esphome/core/component.h"
#include "esphome/components/cover/cover.h"
#include "../jiecang_desk.h"

namespace esphome {
namespace jiecang_desk {

class JiecangDeskCover : public cover::Cover, public Component, public JiecangDeskListener {
 public:
  void dump_config() override;
  void setup() override;
  cover::CoverTraits get_traits() override;
  
  void set_parent(JiecangDeskComponent *parent) { this->parent_ = parent; }
  bool notify_all_height_updates() override { return true; };
  void on_height_update(const optional<int> height) override;
  void on_limits_update(const std::tuple<optional<int>, optional<int>> limits) override;
   
 protected:
  JiecangDeskComponent *parent_{nullptr};

  void control(const cover::CoverCall &call) override;  

 private:
  void update_state(const optional<int> height, const std::tuple<optional<int>, optional<int>> limits);
};

}  // namespace jiecang_desk
}  // namespace esphome