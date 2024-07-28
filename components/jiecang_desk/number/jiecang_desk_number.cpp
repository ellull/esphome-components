#include "jiecang_desk_number.h"

namespace esphome {
namespace jiecang_desk {

static const char *const TAG = "jiecang_desk.number";

void JiecangDeskNumber::dump_config() { LOG_NUMBER("", "Jiecang Desk Number", this); }

void JiecangDeskNumber::control(const float value) {
  this->parent_->move_to(value * 10);
}

void JiecangDeskNumber::update_state() {
  optional<int> max = this->get_limit_max();
  if (max.has_value())
    this->traits.set_max_value(*max * 0.1);

  optional<int> min = this->get_limit_min();
  if (min.has_value())
    this->traits.set_min_value(*min * 0.1);

  this->publish_state(this->height_ * 0.1);
}

}  // namespace jiecang_desk
}  // namespace esphome