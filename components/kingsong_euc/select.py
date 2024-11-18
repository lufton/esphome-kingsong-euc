import esphome.codegen as cg
from esphome.components import select
import esphome.config_validation as cv
from esphome.const import CONF_ID, ENTITY_CATEGORY_CONFIG

from . import (
    CONF_KINGSONG_EUC_ID,
    CONF_REPORT_INTERVAL,
    KINGSONG_EUC_COMPONENT_CONFIG_SCHEMA,
    kingsong_euc_ns,
    report_interval_schema,
)

KingSongEUCSelect = kingsong_euc_ns.class_(
    "KingSongEUCSelect", select.Select, cg.PollingComponent
)
KingSongEUCSelectTypeEnum = kingsong_euc_ns.enum("KingSongEUCSelectType", True)

CONF_MAGIC_LIGHT_MODE = "magic_light_mode"
CONF_MAIN_LIGHT_MODE = "main_light_mode"
CONF_RIDE_MODE = "ride_mode"
CONF_SPECTRUM_LIGHT_MODE = "spectrum_light_mode"
CONF_VOICE_LANGUAGE = "voice_language"
ICON_ACCOUNT_VOICE = "mdi:account-voice"
ICON_ALARM_LIGHT = "mdi:alarm-light"
ICON_BIKE_FAST = "mdi:bike-fast"
ICON_CAR_LIGHT_HIGH = "mdi:car-light-high"
ICON_GOOGLE_TRANSLATE = "mdi:google-translate"
ICON_TELEVISION_AMBIENT_LIGHT = "mdi:television-ambient-light"

SELECT_TYPES = {
    CONF_MAGIC_LIGHT_MODE: select.select_schema(
        KingSongEUCSelect,
        icon=ICON_ALARM_LIGHT,
    ),
    CONF_MAIN_LIGHT_MODE: select.select_schema(
        KingSongEUCSelect,
        icon=ICON_CAR_LIGHT_HIGH,
    ),
    CONF_RIDE_MODE: select.select_schema(
        KingSongEUCSelect,
        icon=ICON_BIKE_FAST,
        entity_category=ENTITY_CATEGORY_CONFIG,
    ),
    CONF_SPECTRUM_LIGHT_MODE: select.select_schema(
        KingSongEUCSelect,
        icon=ICON_TELEVISION_AMBIENT_LIGHT,
    ),
    CONF_VOICE_LANGUAGE: select.select_schema(
        KingSongEUCSelect,
        icon=ICON_GOOGLE_TRANSLATE,
    ),
}

SELECT_OPTIONS = {
    CONF_MAGIC_LIGHT_MODE: ["Annular 1", "Annular 2", "Annular 3", "Annular 4"],
    CONF_MAIN_LIGHT_MODE: ["On", "Off", "Auto"],
    CONF_RIDE_MODE: ["Hard", "Medium", "Soft"],
    CONF_SPECTRUM_LIGHT_MODE: ["Automatic", "Beating", "Flashing", "Alternate"],
    CONF_VOICE_LANGUAGE: ["English", "Chinese"],
}

CONFIG_SCHEMA = KINGSONG_EUC_COMPONENT_CONFIG_SCHEMA.extend(
    {
        cv.Optional(select_type): schema.extend(report_interval_schema()).extend(
            cv.polling_component_schema("never")
        )
        for select_type, schema in SELECT_TYPES.items()
    }
)


async def to_code(config):
    kingsong_euc_id = config[CONF_KINGSONG_EUC_ID]
    kingsong_euc_hub = await cg.get_variable(kingsong_euc_id)

    for select_type, _ in SELECT_TYPES.items():
        if (conf := config.get(select_type)) and (
            options := SELECT_OPTIONS.get(select_type)
        ):
            sel = cg.new_Pvariable(
                conf[CONF_ID],
                getattr(KingSongEUCSelectTypeEnum, select_type.upper()),
                select_type,
                conf.get(CONF_REPORT_INTERVAL),
            )
            await select.register_select(sel, conf, options=options)
            await cg.register_component(sel, conf)
            cg.add(getattr(kingsong_euc_hub, f"set_{select_type}_select")(sel))
