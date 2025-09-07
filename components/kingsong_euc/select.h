#pragma once

#include "esphome/core/component.h"
#include "esphome/core/log.h"
#include "esphome/components/select/select.h"
#include "base_entity.h"
#include "const.h"

namespace esphome {
namespace kingsong_euc {

#define REGISTER_SELECT(name) \
 protected: \
  KingSongEUCSelect *name##_select_{nullptr}; \
\
 public: \
  void set_##name##_select(KingSongEUCSelect *select) { \
    this->selects_.push_back(select); \
    this->name##_select_ = select; \
    select->set_parent(this); \
  }

enum class KingSongEUCSelectType {
  MAGIC_LIGHT_MODE,
  MAIN_LIGHT_MODE,
  RIDE_MODE,
  SPECTRUM_LIGHT_MODE,
  VOICE_LANGUAGE,
};

class KingSongEUCSelect : public select::Select, public KingSongEUCBaseEntity {
 public:
  KingSongEUCSelect(KingSongEUCSelectType select_type, std::string name, uint32_t report_interval)
      : KingSongEUCBaseEntity(name, report_interval) {
    this->select_type_ = select_type;
  }

  void dump_config() { LOG_SELECT("  ", this->type_.c_str(), this); }

  bool has_state() override {
    return this->has_state_ && this->last_updated_ > 0 && KingSongEUCBaseEntity::has_state();
  }

  void publish_state(const std::string &state) {
    const std::string prev_state = this->state;
    this->state = state;
    this->has_state_ = true;
    this->just_updated();
    if (state != prev_state)
      this->report_state();
  }

  void report_state() override {
    select::Select::publish_state(this->state);
    this->just_reported();
  }

  void request_state() override {
    switch (this->select_type_) {
      case KingSongEUCSelectType::MAGIC_LIGHT_MODE:
        this->get_parent()->get_magic_light_mode();
        break;
      case KingSongEUCSelectType::SPECTRUM_LIGHT_MODE:
        this->get_parent()->get_spectrum_light_mode();
        break;
      case KingSongEUCSelectType::VOICE_LANGUAGE:
        this->get_parent()->get_voice_language();
        break;
      case KingSongEUCSelectType::MAIN_LIGHT_MODE:
      case KingSongEUCSelectType::RIDE_MODE:
        break;
    }
  }

 protected:
  KingSongEUCSelectType select_type_;

  void control(const std::string &value) override {
    if (!this->is_connected())
      return;
    auto index = this->index_of(value);
    if (!index.has_value())
      return;
    switch (this->select_type_) {
      case KingSongEUCSelectType::MAGIC_LIGHT_MODE:
        return this->get_parent()->set_magic_light_mode(index.value());
      case KingSongEUCSelectType::MAIN_LIGHT_MODE:
        return this->get_parent()->set_main_light_mode(index.value() + 18);
      case KingSongEUCSelectType::RIDE_MODE:
        return this->get_parent()->set_ride_mode(index.value());
      case KingSongEUCSelectType::SPECTRUM_LIGHT_MODE:
        return this->get_parent()->set_spectrum_light_mode(index.value());
      case KingSongEUCSelectType::VOICE_LANGUAGE:
        return this->get_parent()->set_voice_language(index.value());
    }
  }
};

}  // namespace kingsong_euc
}  // namespace esphome
