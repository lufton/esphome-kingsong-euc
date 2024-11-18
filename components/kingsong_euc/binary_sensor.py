import esphome.codegen as cg
from esphome.components import binary_sensor
import esphome.config_validation as cv
from esphome.const import (
    CONF_ID,
    DEVICE_CLASS_BATTERY_CHARGING,
    DEVICE_CLASS_PROBLEM,
    DEVICE_CLASS_RUNNING,
    ICON_FAN,
    ICON_MAGNET,
)

from . import (
    CONF_KINGSONG_EUC_ID,
    CONF_REPORT_INTERVAL,
    KINGSONG_EUC_COMPONENT_CONFIG_SCHEMA,
    kingsong_euc_ns,
    report_interval_schema,
)

KingSongEUCBinarySensor = kingsong_euc_ns.class_(
    "KingSongEUCBinarySensor", binary_sensor.BinarySensor, cg.PollingComponent
)
KingSongEUCBinarySensorTypeEnum = kingsong_euc_ns.enum(
    "KingSongEUCBinarySensorType", True
)

CONF_CHARGING = "charging"
CONF_FAN = "fan"
CONF_GYROSCOPE_ERROR = "gyroscope_error"
CONF_HALL_SENSOR_ERROR = "hall_sensor_error"
CONF_PHASE_SHORT_CIRCUIT = "phase_short_circuit"
ICON_CAR_LIGHT_HIGH = "mdi:car-light-high"
ICON_ELECTRIC_SWITCH_CLOSED = "mdi:electric-switch-closed"
ICON_ROTATE_ORBIT = "mdi:rotate-orbit"

BINARY_SENSOR_TYPES = {
    CONF_CHARGING: binary_sensor.binary_sensor_schema(
        KingSongEUCBinarySensor,
        icon="mdi:battery-charging",
        device_class=DEVICE_CLASS_BATTERY_CHARGING,
    ),
    CONF_FAN: binary_sensor.binary_sensor_schema(
        KingSongEUCBinarySensor,
        icon=ICON_FAN,
        device_class=DEVICE_CLASS_RUNNING,
    ),
    CONF_GYROSCOPE_ERROR: binary_sensor.binary_sensor_schema(
        KingSongEUCBinarySensor,
        icon=ICON_ROTATE_ORBIT,
        device_class=DEVICE_CLASS_PROBLEM,
    ),
    CONF_HALL_SENSOR_ERROR: binary_sensor.binary_sensor_schema(
        KingSongEUCBinarySensor,
        icon=ICON_MAGNET,
        device_class=DEVICE_CLASS_PROBLEM,
    ),
    CONF_PHASE_SHORT_CIRCUIT: binary_sensor.binary_sensor_schema(
        KingSongEUCBinarySensor,
        icon=ICON_ELECTRIC_SWITCH_CLOSED,
        device_class=DEVICE_CLASS_PROBLEM,
    ),
}

CONFIG_SCHEMA = KINGSONG_EUC_COMPONENT_CONFIG_SCHEMA.extend(
    {
        cv.Optional(binary_sensor_type): schema.extend(report_interval_schema()).extend(
            cv.polling_component_schema("never")
        )
        for binary_sensor_type, schema in BINARY_SENSOR_TYPES.items()
    }
)


async def to_code(config):
    kingsong_euc_id = config[CONF_KINGSONG_EUC_ID]
    kingsong_euc_hub = await cg.get_variable(kingsong_euc_id)

    for binary_sensor_type, _ in BINARY_SENSOR_TYPES.items():
        if conf := config.get(binary_sensor_type):
            binary_sens = cg.new_Pvariable(
                conf[CONF_ID],
                getattr(KingSongEUCBinarySensorTypeEnum, binary_sensor_type.upper()),
                binary_sensor_type,
                conf.get(CONF_REPORT_INTERVAL),
            )
            await binary_sensor.register_binary_sensor(binary_sens, conf)
            await cg.register_component(binary_sens, conf)
            cg.add(
                getattr(kingsong_euc_hub, f"set_{binary_sensor_type}_binary_sensor")(
                    binary_sens
                )
            )
