#include "jiecang_desk_number.h"

namespace esphome {
namespace jiecang_desk {

static const char *const TAG = "jiecang_desk.number";

void JiecangDeskNumber::dump_config() {
  LOG_NUMBER("", "Jiecang Desk Number", this);
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

void JiecangDeskNumber::control(const float value) {
  this->parent_->move_to(round(value * 10));
}

void JiecangDeskNumber::on_height_update(const optional<int> height) {
  float new_height = height.has_value() ? *height * 0.1f : NAN;
  this->publish_state(new_height);
}

void JiecangDeskNumber::on_limits_update(const std::tuple<optional<int>, optional<int>> limits) {
    optional<int> max_limit = std::get<0>(limits);
    float new_max = max_limit.has_value() ? *max_limit * 0.1f : NAN;
    ESP_LOGD(TAG, "Setting max value to %.1f", new_max);
    this->traits.set_max_value(new_max);

    optional<int> min_limit = std::get<1>(limits);
    float new_min = min_limit.has_value() ? *min_limit * 0.1f : NAN;
    ESP_LOGD(TAG, "Setting min value to %.1f", new_min);
    this->traits.set_min_value(new_min);
}

}  // namespace jiecang_desk
}  // namespace esphome