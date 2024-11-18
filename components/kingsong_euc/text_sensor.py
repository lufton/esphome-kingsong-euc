import re

import esphome.codegen as cg
from esphome.components import text_sensor
import esphome.config_validation as cv
from esphome.const import CONF_ID, CONF_MODEL, CONF_NAME, ENTITY_CATEGORY_DIAGNOSTIC
from esphome.core import CORE, ID

from . import (
    CONF_BMS_COUNT,
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

CONF_BMS_FIRMWARE = "bms_firmware"
CONF_BMS_MANUFACTURE_DATE = "bms_manufacture_date"
CONF_BMS_SERIAL = "bms_serial"
CONF_CHARGING_STATUS = "charging_status"
CONF_ERROR_DESCRIPTION = "error_description"
CONF_SERIAL = "serial"
ICON_ALERT = "mdi:alert"
ICON_BATTERY_CHARGING = "mdi:battery-charging"
ICON_CALENDAR = "mdi:calendar"
ICON_NUMERIC = "mdi:numeric"

TEXT_SENSOR_TYPES = {
    CONF_BMS_FIRMWARE: text_sensor.text_sensor_schema(
        KingSongEUCTextSensor,
        entity_category=ENTITY_CATEGORY_DIAGNOSTIC,
    ),
    CONF_BMS_MANUFACTURE_DATE: text_sensor.text_sensor_schema(
        KingSongEUCTextSensor,
        icon=ICON_CALENDAR,
        entity_category=ENTITY_CATEGORY_DIAGNOSTIC,
    ),
    CONF_BMS_SERIAL: text_sensor.text_sensor_schema(
        KingSongEUCTextSensor,
        icon=ICON_NUMERIC,
        entity_category=ENTITY_CATEGORY_DIAGNOSTIC,
    ),
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
    CONF_SERIAL: text_sensor.text_sensor_schema(
        KingSongEUCTextSensor,
        icon=ICON_NUMERIC,
        entity_category=ENTITY_CATEGORY_DIAGNOSTIC,
    ),
}

CONFIG_SCHEMA = KINGSONG_EUC_COMPONENT_CONFIG_SCHEMA.extend(
    {
        cv.Optional(text_sensor_type): schema.extend(report_interval_schema()).extend(
            cv.polling_component_schema("never")
        )
        for text_sensor_type, schema in TEXT_SENSOR_TYPES.items()
    }
)


async def register_text_sensor(
    kingsong_euc_hub, id, text_sensor_type, text_sensor_name, conf
):
    text_sens = cg.new_Pvariable(
        id,
        getattr(KingSongEUCTextSensorTypeEnum, text_sensor_type.upper()),
        text_sensor_name,
        conf.get(CONF_REPORT_INTERVAL),
    )
    await text_sensor.register_text_sensor(text_sens, conf)
    await cg.register_component(text_sens, conf)
    cg.add(getattr(kingsong_euc_hub, f"set_{text_sensor_name}_text_sensor")(text_sens))


async def register_bms_text_sensor(kingsong_euc_hub, bms_count, text_sensor_type, conf):
    name = conf[CONF_NAME]
    for bms in range(bms_count):
        id_ = f"bms_{bms + 1}_{text_sensor_type.replace('bms_', '')}"
        CORE.component_ids.add(id_)
        if re.search("bms", name, re.IGNORECASE):
            conf[CONF_NAME] = re.sub("bms", f"BMS {bms + 1}", name, flags=re.IGNORECASE)
        else:
            conf[CONF_NAME] = f"BMS {bms + 1} {name}"
        await register_text_sensor(
            kingsong_euc_hub,
            ID(id_, True, KingSongEUCTextSensor),
            text_sensor_type,
            id_,
            conf,
        )


async def to_code(config):
    kingsong_euc_id = config[CONF_KINGSONG_EUC_ID]
    kingsong_euc_hub = await cg.get_variable(kingsong_euc_id)
    bms_count = config[CONF_BMS_COUNT]
    cg.add_define("KINGSONG_EUC_BMS_COUNT", bms_count)

    for text_sensor_type, _ in TEXT_SENSOR_TYPES.items():
        if conf := config.get(text_sensor_type):
            if text_sensor_type.startswith("bms_"):
                await register_bms_text_sensor(
                    kingsong_euc_hub, bms_count, text_sensor_type, conf
                )
            else:
                await register_text_sensor(
                    kingsong_euc_hub,
                    conf[CONF_ID],
                    text_sensor_type,
                    text_sensor_type,
                    conf,
                )
