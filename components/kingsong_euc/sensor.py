import re

import esphome.codegen as cg
from esphome.components import sensor
import esphome.config_validation as cv
from esphome.const import (
    CONF_CURRENT,
    CONF_HYSTERESIS,
    CONF_ID,
    CONF_NAME,
    CONF_POWER,
    CONF_SPEED,
    CONF_VOLTAGE,
    DEVICE_CLASS_CURRENT,
    DEVICE_CLASS_DISTANCE,
    DEVICE_CLASS_DURATION,
    DEVICE_CLASS_POWER,
    DEVICE_CLASS_SPEED,
    DEVICE_CLASS_TEMPERATURE,
    DEVICE_CLASS_VOLTAGE,
    ENTITY_CATEGORY_DIAGNOSTIC,
    ICON_BATTERY,
    ICON_PULSE,
    ICON_THERMOMETER,
    ICON_TIMER,
    STATE_CLASS_MEASUREMENT,
    STATE_CLASS_TOTAL,
    STATE_CLASS_TOTAL_INCREASING,
    UNIT_AMPERE,
    UNIT_CELSIUS,
    UNIT_KILOMETER,
    UNIT_KILOMETER_PER_HOUR,
    UNIT_PERCENT,
    UNIT_SECOND,
    UNIT_VOLT,
    UNIT_WATT,
)
from esphome.core import CORE, ID

from . import (
    CONF_BMS_COUNT,
    CONF_CELL_COUNT,
    CONF_KINGSONG_EUC_ID,
    CONF_REPORT_INTERVAL,
    KINGSONG_EUC_COMPONENT_CONFIG_SCHEMA,
    kingsong_euc_ns,
    report_interval_schema,
)

KingSongEUCSensor = kingsong_euc_ns.class_(
    "KingSongEUCSensor", sensor.Sensor, cg.PollingComponent
)
KingSongEUCSensorTypeEnum = kingsong_euc_ns.enum("KingSongEUCSensorType", True)

CONF_BMS_CELL_VOLTAGE = "bms_cell_voltage"
CONF_BMS_CURRENT = "bms_current"
CONF_BMS_FACTORY_CAPACITY = "bms_factory_capacity"
CONF_BMS_FULL_CYCLES = "bms_full_cycles"
CONF_BMS_MOSFET_TEMPERATURE = "bms_mosfet_temperature"
CONF_BMS_REMAINING_CAPACITY = "bms_remaining_capacity"
CONF_BMS_SOC = "bms_soc"
CONF_BMS_TEMPERATURE_1 = "bms_temperature_1"
CONF_BMS_TEMPERATURE_2 = "bms_temperature_2"
CONF_BMS_TEMPERATURE_3 = "bms_temperature_3"
CONF_BMS_TEMPERATURE_4 = "bms_temperature_4"
CONF_BMS_TEMPERATURE_5 = "bms_temperature_5"
CONF_BMS_TEMPERATURE_6 = "bms_temperature_6"
CONF_BMS_VOLTAGE = "bms_voltage"
CONF_CPU_LOAD = "cpu_load"
CONF_ERROR_CODE = "error_code"
CONF_ODOMETER = "odometer"
CONF_MOSFET_TEMPERATURE = "mosfet_temperature"
CONF_MOTOR_TEMPERATURE = "motor_temperature"
CONF_PWM = "pwm"
CONF_RIDE_TIME = "ride_time"
CONF_SPEED_LIMIT = "speed_limit"
CONF_TRIP_DISTANCE = "trip_distance"
CONF_TRIP_MAX_SPEED = "trip_max_speed"
CONF_UPTIME = "uptime"
ICON_ALERT = "mdi:alert"
ICON_BATTERY_ARROW_DOWN = "mdi:battery-arrow-down"
ICON_BATTERY_ARROW_UP = "mdi:battery-arrow-up"
ICON_BATTERY_ARROW_UP_OUTLINE = "mdi:battery-arrow-up-outline"
ICON_BATTERY_CHECK = "mdi:battery-check"
ICON_BATTERY_SYNC = "mdi:battery-sync"
ICON_CAR_SPEED_LIMITER = "mdi:car-speed-limiter"
ICON_CURRENT_DC = "mdi:current-dc"
ICON_MAP_MARKER_DISTANCE = "mdi:map-marker-distance"
ICON_METER_ELECTRIC = "mdi:meter-electric"
ICON_SPEEDOMETER = "mdi:speedometer"
ICON_CHART_AREASPLINE = "mdi:chart-areaspline"
UNIT_MILLIAMPERE_HOURS = "mAh"

SENSOR_TYPES = {
    CONF_BMS_CELL_VOLTAGE: sensor.sensor_schema(
        KingSongEUCSensor,
        unit_of_measurement=UNIT_VOLT,
        icon=ICON_BATTERY,
        accuracy_decimals=3,
        state_class=STATE_CLASS_MEASUREMENT,
        entity_category=ENTITY_CATEGORY_DIAGNOSTIC,
    ),
    CONF_BMS_CURRENT: sensor.sensor_schema(
        KingSongEUCSensor,
        unit_of_measurement=UNIT_AMPERE,
        icon=ICON_CURRENT_DC,
        accuracy_decimals=2,
        state_class=STATE_CLASS_MEASUREMENT,
        entity_category=ENTITY_CATEGORY_DIAGNOSTIC,
    ),
    CONF_BMS_FACTORY_CAPACITY: sensor.sensor_schema(
        KingSongEUCSensor,
        unit_of_measurement=UNIT_MILLIAMPERE_HOURS,
        icon=ICON_BATTERY_ARROW_UP,
        accuracy_decimals=0,
        state_class=STATE_CLASS_TOTAL,
        entity_category=ENTITY_CATEGORY_DIAGNOSTIC,
    ),
    CONF_BMS_FULL_CYCLES: sensor.sensor_schema(
        KingSongEUCSensor,
        icon=ICON_BATTERY_SYNC,
        accuracy_decimals=0,
        state_class=STATE_CLASS_TOTAL_INCREASING,
        entity_category=ENTITY_CATEGORY_DIAGNOSTIC,
    ),
    CONF_BMS_MOSFET_TEMPERATURE: sensor.sensor_schema(
        KingSongEUCSensor,
        unit_of_measurement=UNIT_CELSIUS,
        icon=ICON_THERMOMETER,
        accuracy_decimals=1,
        state_class=STATE_CLASS_MEASUREMENT,
        entity_category=ENTITY_CATEGORY_DIAGNOSTIC,
    ),
    CONF_BMS_REMAINING_CAPACITY: sensor.sensor_schema(
        KingSongEUCSensor,
        unit_of_measurement=UNIT_MILLIAMPERE_HOURS,
        icon=ICON_BATTERY_ARROW_UP_OUTLINE,
        accuracy_decimals=0,
        state_class=STATE_CLASS_MEASUREMENT,
        entity_category=ENTITY_CATEGORY_DIAGNOSTIC,
    ),
    CONF_BMS_SOC: sensor.sensor_schema(
        KingSongEUCSensor,
        unit_of_measurement=UNIT_PERCENT,
        icon=ICON_BATTERY,
        accuracy_decimals=2,
        state_class=STATE_CLASS_MEASUREMENT,
        entity_category=ENTITY_CATEGORY_DIAGNOSTIC,
    ),
    CONF_BMS_TEMPERATURE_1: sensor.sensor_schema(
        KingSongEUCSensor,
        unit_of_measurement=UNIT_CELSIUS,
        icon=ICON_THERMOMETER,
        accuracy_decimals=1,
        state_class=STATE_CLASS_MEASUREMENT,
        entity_category=ENTITY_CATEGORY_DIAGNOSTIC,
    ),
    CONF_BMS_TEMPERATURE_2: sensor.sensor_schema(
        KingSongEUCSensor,
        unit_of_measurement=UNIT_CELSIUS,
        icon=ICON_THERMOMETER,
        accuracy_decimals=1,
        state_class=STATE_CLASS_MEASUREMENT,
        entity_category=ENTITY_CATEGORY_DIAGNOSTIC,
    ),
    CONF_BMS_TEMPERATURE_3: sensor.sensor_schema(
        KingSongEUCSensor,
        unit_of_measurement=UNIT_CELSIUS,
        icon=ICON_THERMOMETER,
        accuracy_decimals=1,
        state_class=STATE_CLASS_MEASUREMENT,
        entity_category=ENTITY_CATEGORY_DIAGNOSTIC,
    ),
    CONF_BMS_TEMPERATURE_4: sensor.sensor_schema(
        KingSongEUCSensor,
        unit_of_measurement=UNIT_CELSIUS,
        icon=ICON_THERMOMETER,
        accuracy_decimals=1,
        state_class=STATE_CLASS_MEASUREMENT,
        entity_category=ENTITY_CATEGORY_DIAGNOSTIC,
    ),
    CONF_BMS_TEMPERATURE_5: sensor.sensor_schema(
        KingSongEUCSensor,
        unit_of_measurement=UNIT_CELSIUS,
        icon=ICON_THERMOMETER,
        accuracy_decimals=1,
        state_class=STATE_CLASS_MEASUREMENT,
        entity_category=ENTITY_CATEGORY_DIAGNOSTIC,
    ),
    CONF_BMS_TEMPERATURE_6: sensor.sensor_schema(
        KingSongEUCSensor,
        unit_of_measurement=UNIT_CELSIUS,
        icon=ICON_THERMOMETER,
        accuracy_decimals=1,
        state_class=STATE_CLASS_MEASUREMENT,
        entity_category=ENTITY_CATEGORY_DIAGNOSTIC,
    ),
    CONF_BMS_VOLTAGE: sensor.sensor_schema(
        KingSongEUCSensor,
        unit_of_measurement=UNIT_VOLT,
        icon=ICON_BATTERY,
        accuracy_decimals=2,
        state_class=STATE_CLASS_MEASUREMENT,
        entity_category=ENTITY_CATEGORY_DIAGNOSTIC,
    ),
    CONF_CPU_LOAD: sensor.sensor_schema(
        KingSongEUCSensor,
        unit_of_measurement=UNIT_PERCENT,
        icon=ICON_CHART_AREASPLINE,
        accuracy_decimals=0,
        state_class=STATE_CLASS_MEASUREMENT,
    ),
    CONF_ERROR_CODE: sensor.sensor_schema(
        KingSongEUCSensor,
        icon=ICON_ALERT,
        entity_category=ENTITY_CATEGORY_DIAGNOSTIC,
    ),
    CONF_CURRENT: sensor.sensor_schema(
        KingSongEUCSensor,
        unit_of_measurement=UNIT_AMPERE,
        icon=ICON_CURRENT_DC,
        accuracy_decimals=2,
        device_class=DEVICE_CLASS_CURRENT,
        state_class=STATE_CLASS_MEASUREMENT,
    ),
    CONF_MOSFET_TEMPERATURE: sensor.sensor_schema(
        KingSongEUCSensor,
        unit_of_measurement=UNIT_CELSIUS,
        icon=ICON_THERMOMETER,
        accuracy_decimals=0,
        device_class=DEVICE_CLASS_TEMPERATURE,
        state_class=STATE_CLASS_MEASUREMENT,
    ),
    CONF_MOTOR_TEMPERATURE: sensor.sensor_schema(
        KingSongEUCSensor,
        unit_of_measurement=UNIT_CELSIUS,
        icon=ICON_THERMOMETER,
        accuracy_decimals=0,
        device_class=DEVICE_CLASS_TEMPERATURE,
        state_class=STATE_CLASS_MEASUREMENT,
    ),
    CONF_ODOMETER: sensor.sensor_schema(
        KingSongEUCSensor,
        unit_of_measurement=UNIT_KILOMETER,
        icon=ICON_MAP_MARKER_DISTANCE,
        accuracy_decimals=3,
        device_class=DEVICE_CLASS_DISTANCE,
        state_class=STATE_CLASS_TOTAL_INCREASING,
    ),
    CONF_POWER: sensor.sensor_schema(
        KingSongEUCSensor,
        unit_of_measurement=UNIT_WATT,
        icon=ICON_METER_ELECTRIC,
        accuracy_decimals=2,
        device_class=DEVICE_CLASS_POWER,
        state_class=STATE_CLASS_MEASUREMENT,
    ),
    CONF_PWM: sensor.sensor_schema(
        KingSongEUCSensor,
        unit_of_measurement=UNIT_PERCENT,
        icon=ICON_PULSE,
        accuracy_decimals=0,
        state_class=STATE_CLASS_MEASUREMENT,
    ),
    CONF_RIDE_TIME: sensor.sensor_schema(
        KingSongEUCSensor,
        unit_of_measurement=UNIT_SECOND,
        icon=ICON_TIMER,
        accuracy_decimals=0,
        state_class=STATE_CLASS_TOTAL_INCREASING,
        device_class=DEVICE_CLASS_DURATION,
        entity_category=ENTITY_CATEGORY_DIAGNOSTIC,
    ),
    CONF_SPEED: sensor.sensor_schema(
        KingSongEUCSensor,
        unit_of_measurement=UNIT_KILOMETER_PER_HOUR,
        icon=ICON_SPEEDOMETER,
        accuracy_decimals=2,
        device_class=DEVICE_CLASS_SPEED,
        state_class=STATE_CLASS_MEASUREMENT,
    ),
    CONF_SPEED_LIMIT: sensor.sensor_schema(
        KingSongEUCSensor,
        unit_of_measurement=UNIT_KILOMETER_PER_HOUR,
        icon=ICON_CAR_SPEED_LIMITER,
        accuracy_decimals=2,
        device_class=DEVICE_CLASS_SPEED,
        state_class=STATE_CLASS_TOTAL,
    ),
    CONF_TRIP_DISTANCE: sensor.sensor_schema(
        KingSongEUCSensor,
        unit_of_measurement=UNIT_KILOMETER,
        icon=ICON_MAP_MARKER_DISTANCE,
        accuracy_decimals=3,
        device_class=DEVICE_CLASS_DISTANCE,
        state_class=STATE_CLASS_TOTAL_INCREASING,
    ),
    CONF_TRIP_MAX_SPEED: sensor.sensor_schema(
        KingSongEUCSensor,
        unit_of_measurement=UNIT_KILOMETER_PER_HOUR,
        icon=ICON_SPEEDOMETER,
        accuracy_decimals=2,
        device_class=DEVICE_CLASS_SPEED,
        state_class=STATE_CLASS_MEASUREMENT,
    ),
    CONF_UPTIME: sensor.sensor_schema(
        KingSongEUCSensor,
        unit_of_measurement=UNIT_SECOND,
        icon=ICON_TIMER,
        accuracy_decimals=0,
        state_class=STATE_CLASS_TOTAL_INCREASING,
        device_class=DEVICE_CLASS_DURATION,
        entity_category=ENTITY_CATEGORY_DIAGNOSTIC,
    ),
    CONF_VOLTAGE: sensor.sensor_schema(
        KingSongEUCSensor,
        unit_of_measurement=UNIT_VOLT,
        icon=ICON_BATTERY,
        accuracy_decimals=2,
        device_class=DEVICE_CLASS_VOLTAGE,
        state_class=STATE_CLASS_MEASUREMENT,
    ),
}


CONFIG_SCHEMA = KINGSONG_EUC_COMPONENT_CONFIG_SCHEMA.extend(
    {
        cv.Optional(sensor_type): schema.extend(
            {
                cv.Optional(
                    CONF_HYSTERESIS, default=0xFFFFFFFF
                ): cv.positive_not_null_float,
            }
        )
        .extend(report_interval_schema())
        .extend(cv.polling_component_schema("never"))
        for sensor_type, schema in SENSOR_TYPES.items()
    }
)


async def register_sensor(kingsong_euc_hub, id, sensor_type, sensor_name, conf):
    sens = cg.new_Pvariable(
        id,
        getattr(KingSongEUCSensorTypeEnum, sensor_type.upper()),
        sensor_name,
        conf[CONF_REPORT_INTERVAL],
        conf[CONF_HYSTERESIS],
    )
    await sensor.register_sensor(sens, conf)
    await cg.register_component(sens, conf)
    cg.add(getattr(kingsong_euc_hub, f"set_{sensor_name}_sensor")(sens))


async def register_cell_voltage_sensors(kingsong_euc_hub, bms_count, cell_count, conf):
    name = conf[CONF_NAME]
    for bms in range(bms_count):
        for cell in range(cell_count):
            id_ = f"bms_{bms + 1}_cell_{cell + 1}_voltage"
            CORE.component_ids.add(id_)
            if re.search("bms", name, re.IGNORECASE):
                conf[CONF_NAME] = re.sub(
                    "bms", f"BMS {bms + 1}", name, flags=re.IGNORECASE
                )
            else:
                conf[CONF_NAME] = f"BMS {bms + 1} {name}"
            if re.search("cell", conf[CONF_NAME], re.IGNORECASE):
                conf[CONF_NAME] = re.sub(
                    "cell", f"cell {cell + 1}", conf[CONF_NAME], flags=re.IGNORECASE
                )
            else:
                conf[CONF_NAME] = f"{conf[CONF_NAME]} cell {cell + 1}"
            await register_sensor(
                kingsong_euc_hub,
                ID(id_, True, KingSongEUCSensor),
                CONF_BMS_CELL_VOLTAGE,
                f"bms_{bms + 1}_cell_{cell + 1}_voltage",
                conf,
            )


async def register_bms_sensor(kingsong_euc_hub, bms_count, sensor_type, conf):
    name = conf[CONF_NAME]
    for bms in range(bms_count):
        id_ = f"bms_{bms + 1}_{sensor_type.replace('bms_', '')}"
        CORE.component_ids.add(id_)
        if re.search("bms", name, re.IGNORECASE):
            conf[CONF_NAME] = re.sub("bms", f"BMS {bms + 1}", name, flags=re.IGNORECASE)
        else:
            conf[CONF_NAME] = f"BMS {bms + 1} {name}"
        await register_sensor(
            kingsong_euc_hub,
            ID(id_, True, KingSongEUCSensor),
            sensor_type,
            id_,
            conf,
        )


async def to_code(config):
    kingsong_euc_id = config[CONF_KINGSONG_EUC_ID]
    kingsong_euc_hub = await cg.get_variable(kingsong_euc_id)
    bms_count = config[CONF_BMS_COUNT]
    cell_count = config[CONF_CELL_COUNT]
    cg.add_define("KINGSONG_EUC_BMS_COUNT", bms_count)
    cg.add_define("KINGSONG_EUC_CELL_COUNT", cell_count)

    for sensor_type, _ in SENSOR_TYPES.items():
        if conf := config.get(sensor_type):
            if sensor_type == CONF_BMS_CELL_VOLTAGE:
                await register_cell_voltage_sensors(
                    kingsong_euc_hub, bms_count, cell_count, conf
                )
            elif sensor_type.startswith("bms_"):
                await register_bms_sensor(
                    kingsong_euc_hub, bms_count, sensor_type, conf
                )
            else:
                await register_sensor(
                    kingsong_euc_hub, conf[CONF_ID], sensor_type, sensor_type, conf
                )
