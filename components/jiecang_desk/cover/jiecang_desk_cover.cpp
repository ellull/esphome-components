#include "jiecang_desk_cover.h"

namespace esphome {
namespace jiecang_desk {

static const char *const TAG = "jiecang_desk.cover";

void JiecangDeskCover::dump_config() {
  LOG_COVER("", "Jiecang Desk Cover", this);

  if (this->physical_max_.has_value())
    ESP_LOGCONFIG(TAG, "  Physical max: %d", *this->physical_max_);

  if (this->physical_min_.has_value())
    ESP_LOGCONFIG(TAG, "  Physical min: %d", *this->physical_min_);

  if (this->configured_max_.has_value())
    ESP_LOGCONFIG(TAG, "  Configured max: %d", *this->configured_max_);

  if (this->configured_min_.has_value())
    ESP_LOGCONFIG(TAG, "  Configured min: %d", *this->configured_min_);
}

void JiecangDeskCover::setup() {
  ESP_LOGCONFIG(TAG, "Initializing cover");
  this->position = nanf("");
  this->parent_->send_command(COMMAND_PHYSICAL_LIMITS);
  this->parent_->send_command(COMMAND_LIMITS);
}

cover::CoverTraits JiecangDeskCover::get_traits() {
  auto traits = cover::CoverTraits();
  traits.set_supports_stop(true);
  traits.set_supports_position(true);
  traits.set_supports_toggle(false);
  traits.set_is_assumed_state(false);
  return traits;
}

void JiecangDeskCover::control(const cover::CoverCall &call) {
  if (call.get_stop()) {
    this->parent_->send_command(COMMAND_STOP);
  } else if (call.get_position().has_value()) {
    float target_position = *call.get_position();
    if (this->position == target_position)
      return;

    optional<int> max = this->get_limit_max();
    optional<int> min = this->get_limit_min();
    if (!min.has_value() || !max.has_value()) {
      ESP_LOGW(TAG, "limits are not reported.");
      return;
    }

    int target_height = (target_position * (*max - *min)) + *min;

    uint8_t bytes[2] = { (uint8_t)(target_height >> 8), (uint8_t)(target_height & 0xFF) };
    this->parent_->send_command(COMMAND_SET_HEIGHT, sizeof(bytes), bytes);
  }
}

void JiecangDeskCover::update_state() {
  optional<int> max = this->get_limit_max();
  optional<int> min = this->get_limit_min();

  // check limits.
  if (!max.has_value() || !min.has_value()) {
    this->position = nanf("");
    this->publish_state(false);
    return;
  }
  
  float new_position = (float)(this->height_ - *min) / (*max - *min);

  if (this->position == new_position && this->current_operation == cover::COVER_OPERATION_IDLE)
    return;

  if (new_position > this->position)
    this->current_operation = cover::COVER_OPERATION_OPENING;
  else if (new_position == this->position)
    this->current_operation = cover::COVER_OPERATION_IDLE;
  else if (new_position < this->position)
    this->current_operation = cover::COVER_OPERATION_CLOSING;

  this->position = new_position;
  this->publish_state(false);
}

}  // namespace jiecang_desk
}  // namespace esphome