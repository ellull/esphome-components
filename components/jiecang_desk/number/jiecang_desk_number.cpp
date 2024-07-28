#include "jiecang_desk_number.h"

namespace esphome {
namespace jiecang_desk {

static const char *const TAG = "jiecang_desk.number";

void JiecangDeskNumber::dump_config() {
  LOG_NUMBER("", "Jiecang Desk Number", this);

  if (this->physical_max_.has_value())
    ESP_LOGCONFIG(TAG, "  Physical max: %d", *this->physical_max_);

  if (this->physical_min_.has_value())
    ESP_LOGCONFIG(TAG, "  Physical min: %d", *this->physical_min_);

  if (this->configured_max_.has_value())
    ESP_LOGCONFIG(TAG, "  Configured max: %d", *this->configured_max_);

  if (this->configured_min_.has_value())
    ESP_LOGCONFIG(TAG, "  Configured min: %d", *this->configured_min_);
}

void JiecangDeskNumber::control(const float value) {
  //this->parent_->move_to(value * 10);
}

void JiecangDeskNumber::update_state() {
  ESP_LOGD(TAG, "update_state");

  optional<int> max = this->get_limit_max();
  if (max.has_value())
    this->traits.set_max_value(*max * 0.1F);

  optional<int> min = this->get_limit_min();
  if (min.has_value())
    this->traits.set_min_value(*min * 0.1F);

  float new_state = this->height_ * 0.1F;
  ESP_LOGD(TAG, "curr state = %f, new state = %f", this->state, new_state);
  if (this->state != new_state)
    this->publish_state(new_state);
}

}  // namespace jiecang_desk
}  // namespace esphome