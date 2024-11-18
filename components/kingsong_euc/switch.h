#pragma once

#include "esphome/core/component.h"
#include "esphome/core/log.h"
#include "esphome/components/switch/switch.h"
#include "base_entity.h"
#include "const.h"

namespace esphome {
namespace kingsong_euc {

#define REGISTER_SWITCH(name) \
 protected: \
  KingSongEUCSwitch *name##_switch_{nullptr}; \
\
 public: \
  void set_##name##_switch(KingSongEUCSwitch *switch_) { \
    this->switches_.push_back(switch_); \
    this->name##_switch_ = switch_; \
    switch_->set_parent(this); \
  }

enum class KingSongEUCSwitchType {
  CIRCLE_LIGHT,
  LIFT_SENSOR,
  MUSIC_BLUETOOTH,
  SPECTRUM_LIGHT,
  STROBE,
  VOICE,
};

class KingSongEUCSwitch : public switch_::Switch, public KingSongEUCBaseEntity {
 public:
  KingSongEUCSwitch(KingSongEUCSwitchType switch_type, std::string name, uint32_t report_interval)
      : KingSongEUCBaseEntity(name, report_interval) {
    this->switch_type_ = switch_type;
  }

  void dump_config() { LOG_SWITCH("  ", this->type_.c_str(), this); }

  void publish_state(bool state) {
    bool prev_state = this->state;
    this->state = state;
    this->just_updated();
    if (state != prev_state)
      this->report_state();
  }

  void report_state() override {
    switch_::Switch::publish_state(this->state);
    this->just_reported();
  }

  void request_state() override {
    if (!this->is_connected())
      return;
    switch (this->switch_type_) {
      case KingSongEUCSwitchType::CIRCLE_LIGHT:
        this->get_parent()->get_circle_light();
        break;
      case KingSongEUCSwitchType::LIFT_SENSOR:
        this->get_parent()->get_lift_sensor();
        break;
      case KingSongEUCSwitchType::MUSIC_BLUETOOTH:
        this->get_parent()->get_music_bluetooth();
        break;
      case KingSongEUCSwitchType::SPECTRUM_LIGHT:
        this->get_parent()->get_spectrum_light();
        break;
      case KingSongEUCSwitchType::STROBE:
        this->get_parent()->get_strobe();
        break;
      case KingSongEUCSwitchType::VOICE:
        this->get_parent()->get_voice();
        break;
    }
    this->just_updated();
  }

 protected:
  KingSongEUCSwitchType switch_type_;

  void write_state(bool state) {
    if (!this->is_connected())
      return;
    switch (this->switch_type_) {
      case KingSongEUCSwitchType::CIRCLE_LIGHT:
        return this->get_parent()->set_circle_light(state);
      case KingSongEUCSwitchType::LIFT_SENSOR:
        return this->get_parent()->set_lift_sensor(state);
      case KingSongEUCSwitchType::MUSIC_BLUETOOTH:
        return this->get_parent()->set_music_bluetooth(state);
      case KingSongEUCSwitchType::SPECTRUM_LIGHT:
        return this->get_parent()->set_spectrum_light(state);
      case KingSongEUCSwitchType::STROBE:
        return this->get_parent()->set_strobe(state);
      case KingSongEUCSwitchType::VOICE:
        return this->get_parent()->set_voice(state);
    }
  }
};

}  // namespace kingsong_euc
}  // namespace esphome
