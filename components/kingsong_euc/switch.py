import esphome.codegen as cg
from esphome.components import switch
import esphome.config_validation as cv
from esphome.const import CONF_ID, ICON_FLASH

from . import (
    CONF_KINGSONG_EUC_ID,
    CONF_REPORT_INTERVAL,
    KINGSONG_EUC_COMPONENT_CONFIG_SCHEMA,
    kingsong_euc_ns,
    report_interval_schema,
)

KingSongEUCSwitch = kingsong_euc_ns.class_(
    "KingSongEUCSwitch", switch.Switch, cg.PollingComponent
)
KingSongEUCSwitchTypeEnum = kingsong_euc_ns.enum("KingSongEUCSwitchType", True)

CONF_CIRCLE_LIGHT = "circle_light"
CONF_LIFT_SENSOR = "lift_sensor"
CONF_MUSIC_BLUETOOTH = "music_bluetooth"
CONF_SPECTRUM_LIGHT = "spectrum_light"
CONF_STROBE = "strobe"
CONF_VOICE = "voice"
ICON_ACCOUNT_VOICE = "mdi:account-voice"
ICON_BIKE_FAST = "mdi:bike-fast"
ICON_SPEAKER_BLUETOOTH = "mdi:speaker-bluetooth"
ICON_TELEVISION_AMBIENT_LIGHT = "mdi:television-ambient-light"

SWITCH_TYPES = {
    CONF_CIRCLE_LIGHT: switch.switch_schema(
        KingSongEUCSwitch,
    ),
    CONF_LIFT_SENSOR: switch.switch_schema(
        KingSongEUCSwitch,
    ),
    CONF_MUSIC_BLUETOOTH: switch.switch_schema(
        KingSongEUCSwitch,
        icon=ICON_SPEAKER_BLUETOOTH,
    ),
    CONF_SPECTRUM_LIGHT: switch.switch_schema(
        KingSongEUCSwitch,
        icon=ICON_TELEVISION_AMBIENT_LIGHT,
    ),
    CONF_STROBE: switch.switch_schema(
        KingSongEUCSwitch,
        icon=ICON_FLASH,
    ),
    CONF_VOICE: switch.switch_schema(
        KingSongEUCSwitch,
        icon=ICON_ACCOUNT_VOICE,
    ),
}

CONFIG_SCHEMA = KINGSONG_EUC_COMPONENT_CONFIG_SCHEMA.extend(
    {
        cv.Optional(switch_type): schema.extend(report_interval_schema()).extend(
            cv.polling_component_schema("never")
        )
        for switch_type, schema in SWITCH_TYPES.items()
    }
)


async def to_code(config):
    kingsong_euc_id = config[CONF_KINGSONG_EUC_ID]
    kingsong_euc_hub = await cg.get_variable(kingsong_euc_id)

    for switch_type, _ in SWITCH_TYPES.items():
        if conf := config.get(switch_type):
            sw = cg.new_Pvariable(
                conf[CONF_ID],
                getattr(KingSongEUCSwitchTypeEnum, switch_type.upper()),
                switch_type,
                conf.get(CONF_REPORT_INTERVAL),
            )
            await switch.register_switch(sw, conf)
            await cg.register_component(sw, conf)
            cg.add(getattr(kingsong_euc_hub, f"set_{switch_type}_switch")(sw))
