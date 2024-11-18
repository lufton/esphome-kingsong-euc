import esphome.codegen as cg
from esphome.components import ble_client
import esphome.config_validation as cv
from esphome.const import CONF_ID

CODEOWNERS = ["@lufton"]
DEPENDENCIES = ["ble_client"]
AUTO_LOAD = [
    "binary_sensor",
    "button",
    "lock",
    "number",
    "select",
    "sensor",
    "switch",
    "text_sensor",
]
CELL_COUNT_OPTIONS = [16, 20, 30]
CONF_BMS_COUNT = "bms_count"
CONF_CELL_COUNT = "cell_count"
CONF_KINGSONG_EUC_ID = "kingsong_euc_id"
CONF_REPORT_INTERVAL = "report_interval"

kingsong_euc_ns = cg.esphome_ns.namespace("kingsong_euc")
KingSongEUC = kingsong_euc_ns.class_(
    "KingSongEUC", ble_client.BLEClientNode, cg.Component
)

CONFIG_SCHEMA = cv.COMPONENT_SCHEMA.extend(
    {
        cv.GenerateID(): cv.declare_id(KingSongEUC),
    }
).extend(ble_client.BLE_CLIENT_SCHEMA)

KINGSONG_EUC_COMPONENT_CONFIG_SCHEMA = cv.Schema(
    {
        cv.GenerateID(CONF_KINGSONG_EUC_ID): cv.use_id(KingSongEUC),
        cv.Optional(CONF_BMS_COUNT, default=1): cv.int_range(1, 2),
        cv.Optional(CONF_CELL_COUNT, default=16): cv.one_of(*CELL_COUNT_OPTIONS),
    }
)


def report_interval_schema(default_report_interval="10s"):
    return cv.Schema(
        {
            cv.Optional(
                CONF_REPORT_INTERVAL, default=default_report_interval
            ): cv.update_interval,
        }
    )


async def to_code(config):
    var = cg.new_Pvariable(config[CONF_ID])
    await cg.register_component(var, config)
    await ble_client.register_ble_node(var, config)
