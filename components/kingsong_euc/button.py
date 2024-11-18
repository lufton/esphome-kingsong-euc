import esphome.codegen as cg
from esphome.components import button
import esphome.config_validation as cv
from esphome.const import CONF_ID, ICON_POWER

from . import (
    CONF_KINGSONG_EUC_ID,
    KINGSONG_EUC_COMPONENT_CONFIG_SCHEMA,
    kingsong_euc_ns,
)

KingSongEUCButton = kingsong_euc_ns.class_(
    "KingSongEUCButton", button.Button, cg.PollingComponent
)
KingSongEUCButtonTypeEnum = kingsong_euc_ns.enum("KingSongEUCButtonType", True)

CONF_BEEP = "beep"
CONF_HORN = "horn"
CONF_POWER_OFF = "power_off"
ICON_AIR_HORN = "mdi:air-horn"

BUTTON_TYPES = {
    CONF_BEEP: button.button_schema(
        KingSongEUCButton,
        icon=ICON_AIR_HORN,
    ),
    CONF_HORN: button.button_schema(
        KingSongEUCButton,
        icon=ICON_AIR_HORN,
    ),
    CONF_POWER_OFF: button.button_schema(
        KingSongEUCButton,
        icon=ICON_POWER,
    ),
}

CONFIG_SCHEMA = KINGSONG_EUC_COMPONENT_CONFIG_SCHEMA.extend(
    {
        cv.Optional(button_type): schema.extend(cv.polling_component_schema("never"))
        for button_type, schema in BUTTON_TYPES.items()
    }
)


async def to_code(config):
    kingsong_euc_id = config[CONF_KINGSONG_EUC_ID]
    kingsong_euc_hub = await cg.get_variable(kingsong_euc_id)

    for button_type, _ in BUTTON_TYPES.items():
        if conf := config.get(button_type):
            but = cg.new_Pvariable(
                conf[CONF_ID],
                getattr(KingSongEUCButtonTypeEnum, button_type.upper()),
                button_type,
            )
            await button.register_button(but, conf)
            await cg.register_component(but, conf)
            cg.add(getattr(kingsong_euc_hub, f"set_{button_type}_button")(but))
