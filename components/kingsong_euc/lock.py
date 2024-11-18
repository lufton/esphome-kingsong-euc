import esphome.codegen as cg
from esphome.components import lock
import esphome.config_validation as cv
from esphome.const import CONF_ID

from . import (
    CONF_KINGSONG_EUC_ID,
    CONF_REPORT_INTERVAL,
    KINGSONG_EUC_COMPONENT_CONFIG_SCHEMA,
    kingsong_euc_ns,
    report_interval_schema,
)

KingSongEUCLock = kingsong_euc_ns.class_(
    "KingSongEUCLock", lock.Lock, cg.PollingComponent
)
KingSongEUCLockTypeEnum = kingsong_euc_ns.enum("KingSongEUCLockType", True)

CONF_LOCK = "lock"

LOCK_TYPES = {
    CONF_LOCK: lock.LOCK_SCHEMA.extend(
        {
            cv.GenerateID(): cv.declare_id(KingSongEUCLock),
        }
    ),
}

CONFIG_SCHEMA = KINGSONG_EUC_COMPONENT_CONFIG_SCHEMA.extend(
    {
        cv.Optional(lock_type): schema.extend(report_interval_schema()).extend(
            cv.polling_component_schema("never")
        )
        for lock_type, schema in LOCK_TYPES.items()
    }
)


async def to_code(config):
    kingsong_euc_id = config[CONF_KINGSONG_EUC_ID]
    kingsong_euc_hub = await cg.get_variable(kingsong_euc_id)

    for lock_type, _ in LOCK_TYPES.items():
        if conf := config.get(lock_type):
            loc = cg.new_Pvariable(
                conf[CONF_ID],
                getattr(KingSongEUCLockTypeEnum, lock_type.upper()),
                lock_type,
                conf.get(CONF_REPORT_INTERVAL),
            )
            await lock.register_lock(loc, conf)
            await cg.register_component(loc, conf)
            cg.add(getattr(kingsong_euc_hub, f"set_{lock_type}_lock")(loc))
