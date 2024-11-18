import esphome.codegen as cg
from esphome.components import number
import esphome.config_validation as cv
from esphome.const import (
    CONF_COMMAND,
    CONF_ID,
    CONF_MAX_VALUE,
    CONF_MIN_VALUE,
    CONF_MODE,
    CONF_STEP,
    ENTITY_CATEGORY_CONFIG,
    ENTITY_CATEGORY_DIAGNOSTIC,
    UNIT_KILOMETER_PER_HOUR,
    UNIT_SECOND,
)

from . import (
    CONF_KINGSONG_EUC_ID,
    CONF_REPORT_INTERVAL,
    KINGSONG_EUC_COMPONENT_CONFIG_SCHEMA,
    kingsong_euc_ns,
    report_interval_schema,
)

KingSongEUCNumber = kingsong_euc_ns.class_(
    "KingSongEUCNumber", number.Number, cg.PollingComponent
)
KingSongEUCNumberTypeEnum = kingsong_euc_ns.enum("KingSongEUCNumberType", True)

CONF_ALARM_1 = "alarm_1"
CONF_ALARM_2 = "alarm_2"
CONF_ALARM_3 = "alarm_3"
CONF_STANDBY_DELAY = "standby_delay"
CONF_TILT_BACK = "tilt_back"
ICON_CAR_SPEED_LIMITER = "mdi:car-speed-limiter"
ICON_CODE_ARRAY = "mdi:code-array"
ICON_TIMER_STOP = "mdi:timer-stop"

NUMBER_TYPES = {
    CONF_ALARM_1: number.number_schema(
        KingSongEUCNumber,
        icon=ICON_CAR_SPEED_LIMITER,
        unit_of_measurement=UNIT_KILOMETER_PER_HOUR,
        entity_category=ENTITY_CATEGORY_CONFIG,
    ),
    CONF_ALARM_2: number.number_schema(
        KingSongEUCNumber,
        icon=ICON_CAR_SPEED_LIMITER,
        unit_of_measurement=UNIT_KILOMETER_PER_HOUR,
        entity_category=ENTITY_CATEGORY_CONFIG,
    ),
    CONF_ALARM_3: number.number_schema(
        KingSongEUCNumber,
        icon=ICON_CAR_SPEED_LIMITER,
        unit_of_measurement=UNIT_KILOMETER_PER_HOUR,
        entity_category=ENTITY_CATEGORY_CONFIG,
    ),
    CONF_COMMAND: number.number_schema(
        KingSongEUCNumber,
        icon=ICON_CODE_ARRAY,
        entity_category=ENTITY_CATEGORY_DIAGNOSTIC,
    ),
    CONF_STANDBY_DELAY: number.number_schema(
        KingSongEUCNumber,
        icon=ICON_TIMER_STOP,
        unit_of_measurement=UNIT_SECOND,
        entity_category=ENTITY_CATEGORY_CONFIG,
    ),
    CONF_TILT_BACK: number.number_schema(
        KingSongEUCNumber,
        icon=ICON_CAR_SPEED_LIMITER,
        unit_of_measurement=UNIT_KILOMETER_PER_HOUR,
        entity_category=ENTITY_CATEGORY_CONFIG,
    ),
}

NUMBER_OPTIONS = {
    CONF_ALARM_1: {CONF_MIN_VALUE: 0, CONF_MAX_VALUE: 70, CONF_STEP: 1},
    CONF_ALARM_2: {CONF_MIN_VALUE: 0, CONF_MAX_VALUE: 70, CONF_STEP: 1},
    CONF_ALARM_3: {CONF_MIN_VALUE: 0, CONF_MAX_VALUE: 70, CONF_STEP: 1},
    CONF_COMMAND: {CONF_MIN_VALUE: 0, CONF_MAX_VALUE: 255, CONF_STEP: 1},
    CONF_STANDBY_DELAY: {CONF_MIN_VALUE: 60, CONF_MAX_VALUE: 14400, CONF_STEP: 1},
    CONF_TILT_BACK: {CONF_MIN_VALUE: 0, CONF_MAX_VALUE: 70, CONF_STEP: 1},
}

CONFIG_SCHEMA = KINGSONG_EUC_COMPONENT_CONFIG_SCHEMA.extend(
    {
        cv.Optional(number_type): schema.extend(
            {
                cv.Optional(CONF_MODE, default="BOX"): cv.enum(
                    number.NUMBER_MODES, upper=True
                )
            }
        )
        .extend(report_interval_schema())
        .extend(cv.polling_component_schema("never"))
        for number_type, schema in NUMBER_TYPES.items()
    }
)


async def to_code(config):
    kingsong_euc_id = config[CONF_KINGSONG_EUC_ID]
    kingsong_euc_hub = await cg.get_variable(kingsong_euc_id)

    for number_type, _ in NUMBER_TYPES.items():
        if (conf := config.get(number_type)) and (
            params := NUMBER_OPTIONS.get(number_type)
        ):
            num = cg.new_Pvariable(
                conf[CONF_ID],
                getattr(KingSongEUCNumberTypeEnum, number_type.upper()),
                number_type,
                conf.get(CONF_REPORT_INTERVAL),
            )
            await number.register_number(num, conf, **params)
            await cg.register_component(num, conf)
            cg.add(getattr(kingsong_euc_hub, f"set_{number_type}_number")(num))
