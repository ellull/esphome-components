#include "jiecang_desk_button.h"

namespace esphome {
namespace jiecang_desk {

static const char *const TAG = "jiecang_desk.button";

void JiecangDeskCommandButton::dump_config() { LOG_BUTTON("", "Jiecang Desk Button", this); }

void JiecangDeskCommandButton::press_action() {
  this->parent_->send_command(this->command_);
}

}  // namespace jiecang_desk
}  // namespace esphome