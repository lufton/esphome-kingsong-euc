#pragma once

#include "esphome/core/component.h"
#include "esphome/core/log.h"
#include "esphome/components/number/number.h"
#include "base_entity.h"
#include "const.h"

namespace esphome {
namespace kingsong_euc {

#define REGISTER_NUMBER(name) \
 protected: \
  KingSongEUCNumber *name##_number_{nullptr}; \
\
 public: \
  void set_##name##_number(KingSongEUCNumber *number) { \
    this->numbers_.push_back(number); \
    this->name##_number_ = number; \
    number->set_parent(this); \
  }

enum class KingSongEUCNumberType {
  ALARM_1,
  ALARM_2,
  ALARM_3,
  COMMAND,
  STANDBY_DELAY,
  TILT_BACK,
};

class KingSongEUCNumber : public number::Number, public KingSongEUCBaseEntity {
 public:
  KingSongEUCNumber(KingSongEUCNumberType number_type, std::string name, uint32_t report_interval)
      : KingSongEUCBaseEntity(name, report_interval) {
    this->number_type_ = number_type;
  }

  void dump_config() { LOG_NUMBER("  ", this->type_.c_str(), this); }

  bool has_state() override {
    return this->has_state_ && this->last_updated_ > 0 && KingSongEUCBaseEntity::has_state();
  }

  void publish_state(float state) {
    float prev_state = this->state;
    this->state = state;
    this->has_state_ = true;
    this->just_updated();
    if (state != prev_state)
      this->report_state();
  }

  void report_state() override {
    number::Number::publish_state(this->state);
    this->just_reported();
  }

  void request_state() override {
    switch (this->number_type_) {
      case KingSongEUCNumberType::ALARM_1:
      case KingSongEUCNumberType::ALARM_2:
      case KingSongEUCNumberType::ALARM_3:
      case KingSongEUCNumberType::TILT_BACK:
        this->get_parent()->get_alarms();
        break;
      case KingSongEUCNumberType::STANDBY_DELAY:
        this->get_parent()->get_standby_delay();
        break;
      case KingSongEUCNumberType::COMMAND:
        break;
    }
  }

 protected:
  KingSongEUCNumberType number_type_;

  void control(float value) {
    if (!this->is_connected())
      return;
    if (this->number_type_ == KingSongEUCNumberType::COMMAND) {
        uint8_t command = static_cast<uint8_t>(std::round(value));
        this->get_parent()->send_command(value);
        ESP_LOGD(TAG, "Sending command: 0x%2X (%d)", command, command);
        return;
    }
    this->publish_state(value);
    switch (this->number_type_) {
      case KingSongEUCNumberType::ALARM_1:
        this->get_parent()->set_alarm_1(value);
        this->last_updated_ = 0;
        break;
      case KingSongEUCNumberType::ALARM_2:
        this->get_parent()->set_alarm_2(value);
        this->last_updated_ = 0;
        break;
      case KingSongEUCNumberType::ALARM_3:
        this->get_parent()->set_alarm_3(value);
        this->last_updated_ = 0;
        break;
      case KingSongEUCNumberType::STANDBY_DELAY:
        this->get_parent()->set_standby_delay(value);
        this->last_updated_ = 0;
        break;
      case KingSongEUCNumberType::TILT_BACK:
        this->get_parent()->set_tilt_back(value);
        this->last_updated_ = 0;
        break;
    }
  }
};

}  // namespace kingsong_euc
}  // namespace esphome
