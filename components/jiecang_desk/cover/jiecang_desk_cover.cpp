#include "jiecang_desk_cover.h"

namespace esphome {
namespace jiecang_desk {

static const char *const TAG = "jiecang_desk.cover";

void JiecangDeskCover::dump_config() {
  LOG_COVER("", "Jiecang Desk Cover", this);
  auto limits = this->parent_->get_limits();

  optional<int> max_limit = std::get<0>(limits);
  if (max_limit.has_value()) {
    ESP_LOGCONFIG(TAG, "  Max limit: %d", *max_limit);
  }

  optional<int> min_limit = std::get<1>(limits);
  if (min_limit.has_value()) {
    ESP_LOGCONFIG(TAG, "  Min limit: %d", *min_limit);
  }
}

void JiecangDeskCover::setup() { this->position = NAN; }

cover::CoverTraits JiecangDeskCover::get_traits() {
  auto traits = cover::CoverTraits();
  traits.set_supports_stop(true);
  traits.set_supports_position(true);
  return traits;
}

void JiecangDeskCover::control(const cover::CoverCall &call) {
  if (call.get_stop()) {
    this->parent_->send_command(JiecangDeskCommand::STOP);
  } else if (call.get_position().has_value()) {

    float target_position = *call.get_position();
    if (this->position == target_position) {
      return;
    }

    auto limits = this->parent_->get_limits();

    optional<int> max_limit = std::get<0>(limits);
    if (!max_limit.has_value()) {
      ESP_LOGW(TAG, "Cannot calculate height: max limit unavailable.");
      return;
    }

    optional<int> min_limit = std::get<1>(limits);
    if (!min_limit.has_value()) {
      ESP_LOGW(TAG, "Cannot calculate height: min limit unavailable.");
      return;
    }

    this->parent_->move_to(round((target_position * (*max_limit - *min_limit)) + *min_limit));
  }
}

void JiecangDeskCover::on_height_update(const optional<int> height) {
  this->update_state(height, this->parent_->get_limits());
}

void JiecangDeskCover::on_limits_update(const std::tuple<optional<int>, optional<int>> limits) {
  this->update_state(this->parent_->get_height(), limits);
}

void JiecangDeskCover::update_state(const optional<int> height, const std::tuple<optional<int>, optional<int>> limits) {
  optional<int> max_limit = std::get<0>(limits);
  optional<int> min_limit = std::get<1>(limits);

  if (!height.has_value() || !max_limit.has_value() || !min_limit.has_value()) {
    ESP_LOGW(TAG, "Cannot calculate position.");
    this->position = NAN;
    this->publish_state(false);
    return;
  }

  float new_position = (float)(*height - *min_limit) / (*max_limit - *min_limit);

  cover::CoverOperation new_operation;
  if (new_position > this->position) {
    new_operation = cover::COVER_OPERATION_OPENING;
  } else if (new_position == this->position) {
    new_operation = cover::COVER_OPERATION_IDLE;
  } else if (new_position < this->position) {
    new_operation = cover::COVER_OPERATION_CLOSING;
  }

  if (this->position == new_position && this->current_operation == new_operation) {
    return;
  }

  this->current_operation = new_operation;
  this->position = new_position;
  this->publish_state(false);
}

}  // namespace jiecang_desk
}  // namespace esphome