#pragma once

#include "esphome/core/component.h"
#include "esphome/core/log.h"
#include "esphome/components/text_sensor/text_sensor.h"
#include "base_entity.h"
#include "const.h"

namespace esphome {
namespace kingsong_euc {

#define REGISTER_TEXT_SENSOR(name) \
 protected: \
  KingSongEUCTextSensor *name##_text_sensor_{nullptr}; \
\
 public: \
  void set_##name##_text_sensor(KingSongEUCTextSensor *text_sensor) { \
    this->text_sensors_.push_back(text_sensor); \
    this->name##_text_sensor_ = text_sensor; \
    text_sensor->set_parent(this); \
  }

enum class KingSongEUCTextSensorType {
  BMS_FIRMWARE,
  BMS_MANUFACTURE_DATE,
  BMS_SERIAL_NUMBER,
  CHARGING_STATUS,
  ERROR_DESCRIPTION,
  MODEL,
  SERIAL_NUMBER,
};

class KingSongEUCTextSensor : public text_sensor::TextSensor, public KingSongEUCBaseEntity {
 public:
  KingSongEUCTextSensor(KingSongEUCTextSensorType text_sensor_type, std::string name, uint32_t report_interval)
      : KingSongEUCBaseEntity(name, report_interval) {
    this->text_sensor_type_ = text_sensor_type;
  }

  void dump_config() { LOG_TEXT_SENSOR("  ", this->type_.c_str(), this); }

  bool has_state() override { return this->has_state_ && KingSongEUCBaseEntity::has_state(); }

  void publish_state(const std::string state) {
    const std::string prev_state = this->state;
    this->state = state;
    this->has_state_ = true;
    this->just_updated();
    if (state != prev_state)
      this->report_state();
  }

  void report_state() override {
    text_sensor::TextSensor::publish_state(this->state);
    this->just_reported();
  }

  void request_state() override {
    switch (this->text_sensor_type_) {
      case KingSongEUCTextSensorType::BMS_FIRMWARE:
        this->get_parent()->get_bms_1_firmware();
        this->get_parent()->get_bms_2_firmware();
        break;
      case KingSongEUCTextSensorType::BMS_MANUFACTURE_DATE:
        this->get_parent()->get_bms_1_manufacture_date();
        this->get_parent()->get_bms_2_manufacture_date();
        break;
      case KingSongEUCTextSensorType::BMS_SERIAL_NUMBER:
        this->get_parent()->get_bms_1_serial_number();
        this->get_parent()->get_bms_2_serial_number();
        break;
      case KingSongEUCTextSensorType::MODEL:
        this->get_parent()->get_model();
        break;
      case KingSongEUCTextSensorType::SERIAL_NUMBER:
        this->get_parent()->get_serial_number();
        break;
      case KingSongEUCTextSensorType::CHARGING_STATUS:
      case KingSongEUCTextSensorType::ERROR_DESCRIPTION:
        break;
    }
    this->just_updated();
  }

 protected:
  KingSongEUCTextSensorType text_sensor_type_;
};

}  // namespace kingsong_euc
}  // namespace esphome
