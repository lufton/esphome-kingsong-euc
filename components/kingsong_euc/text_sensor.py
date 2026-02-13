import re

import esphome.codegen as cg
from esphome.components import text_sensor
import esphome.config_validation as cv
from esphome.const import CONF_ID, CONF_MODEL, CONF_NAME, ENTITY_CATEGORY_DIAGNOSTIC
from esphome.core import CORE, ID

from . import (
    CONF_KINGSONG_EUC_ID,
    CONF_REPORT_INTERVAL,
    KINGSONG_EUC_COMPONENT_CONFIG_SCHEMA,
    kingsong_euc_ns,
    report_interval_schema,
)

KingSongEUCTextSensor = kingsong_euc_ns.class_(
    "KingSongEUCTextSensor", text_sensor.TextSensor, cg.PollingComponent
)
KingSongEUCTextSensorTypeEnum = kingsong_euc_ns.enum("KingSongEUCTextSensorType", True)

CONF_BMS_FIRMWARE = "bms_{}_firmware"
CONF_BMS_MANUFACTURE_DATE = "bms_{}_manufacture_date"
CONF_BMS_SERIAL_NUMBER = "bms_{}_serial_number"
CONF_CHARGING_STATUS = "charging_status"
CONF_ERROR_DESCRIPTION = "error_description"
CONF_SERIAL_NUMBER = "serial_number"
ICON_ALERT = "mdi:alert"
ICON_BATTERY_CHARGING = "mdi:battery-charging"
ICON_CALENDAR = "mdi:calendar"
ICON_NUMERIC = "mdi:numeric"

TEXT_SENSOR_TYPES = {
    CONF_CHARGING_STATUS: text_sensor.text_sensor_schema(
        KingSongEUCTextSensor,
        icon=ICON_BATTERY_CHARGING,
        entity_category=ENTITY_CATEGORY_DIAGNOSTIC,
    ),
    CONF_ERROR_DESCRIPTION: text_sensor.text_sensor_schema(
        KingSongEUCTextSensor,
        icon=ICON_ALERT,
        entity_category=ENTITY_CATEGORY_DIAGNOSTIC,
    ),
    CONF_MODEL: text_sensor.text_sensor_schema(
        KingSongEUCTextSensor,
        icon=ICON_NUMERIC,
        entity_category=ENTITY_CATEGORY_DIAGNOSTIC,
    ),
    CONF_SERIAL_NUMBER: text_sensor.text_sensor_schema(
        KingSongEUCTextSensor,
        icon=ICON_NUMERIC,
        entity_category=ENTITY_CATEGORY_DIAGNOSTIC,
    ),
}

BMS_TEXT_SENSOR_TYPES = {
        CONF_BMS_FIRMWARE: text_sensor.text_sensor_schema(
        KingSongEUCTextSensor,
        entity_category=ENTITY_CATEGORY_DIAGNOSTIC,
    ),
    CONF_BMS_MANUFACTURE_DATE: text_sensor.text_sensor_schema(
        KingSongEUCTextSensor,
        icon=ICON_CALENDAR,
        entity_category=ENTITY_CATEGORY_DIAGNOSTIC,
    ),
    CONF_BMS_SERIAL_NUMBER: text_sensor.text_sensor_schema(
        KingSongEUCTextSensor,
        icon=ICON_NUMERIC,
        entity_category=ENTITY_CATEGORY_DIAGNOSTIC,
    ),
}

DEFAULT_TEXT_SENSOR_SCHEMA = (
    report_interval_schema()
    .extend(cv.polling_component_schema("never"))
)

CONFIG_SCHEMA = (
    KINGSONG_EUC_COMPONENT_CONFIG_SCHEMA
    .extend(
        {
            cv.Optional(text_sensor_type): schema.extend(DEFAULT_TEXT_SENSOR_SCHEMA)
            for text_sensor_type, schema in TEXT_SENSOR_TYPES.items()
        }
    )
    .extend(
        {
            cv.Optional(text_sensor_type.format(bms + 1)): schema.extend(DEFAULT_TEXT_SENSOR_SCHEMA)
            for bms in range(2) for text_sensor_type, schema in BMS_TEXT_SENSOR_TYPES.items()
        }
    )
)


async def register_text_sensor(kingsong_euc_hub, id, text_sensor_type, conf):
    text_sens = cg.new_Pvariable(
        id,
        getattr(KingSongEUCTextSensorTypeEnum, text_sensor_type.upper()),
        text_sensor_type,
        conf.get(CONF_REPORT_INTERVAL),
    )
    await text_sensor.register_text_sensor(text_sens, conf)
    await cg.register_component(text_sens, conf)
    cg.add(getattr(kingsong_euc_hub, f"set_{text_sensor_type}_text_sensor")(text_sens))


async def to_code(config):
    kingsong_euc_id = config[CONF_KINGSONG_EUC_ID]
    kingsong_euc_hub = await cg.get_variable(kingsong_euc_id)

    for text_sensor_type, _ in TEXT_SENSOR_TYPES.items():
        if conf := config.get(text_sensor_type):
            await register_text_sensor(kingsong_euc_hub, conf[CONF_ID], text_sensor_type, conf)

    for bms in range(2):
        for bms_text_sensor_type, _ in BMS_TEXT_SENSOR_TYPES.items():
            text_sensor_type = bms_text_sensor_type.format(bms + 1)
            if conf := config.get(text_sensor_type):
                await register_text_sensor(kingsong_euc_hub, conf[CONF_ID], text_sensor_type, conf)
