#include "jiecang_desk_cover.h"

namespace esphome {
namespace jiecang_desk {

static const char *const TAG = "jiecang_desk.cover";

//void JiecangDeskHeightSensor::dump_config() { LOG_SENSOR("  ", "Jiecang Desk Cover", this); }

void JiecangDeskCover::setup() {
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
}

}  // namespace jiecang_desk
}  // namespace esphome