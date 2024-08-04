import sys
import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import number
from esphome.const import (
    DEVICE_CLASS_DISTANCE,
)
from .. import (
    JIECANG_DESK_COMPONENT_SCHEMA,
    CONF_JIECANG_DESK_ID,
    jiecang_desk_ns,
)

DEPENDENCIES = ["jiecang_desk"]

CONF_REBOOT_ON_LIMITS_CHANGE = "reboot_on_limits_change"

JiecangDeskNumber = jiecang_desk_ns.class_("JiecangDeskNumber", number.Number, cg.Component)

CONFIG_SCHEMA = (
    JIECANG_DESK_COMPONENT_SCHEMA
    .extend(
        number.number_schema(
            JiecangDeskNumber,
            icon="mdi:desk",
            device_class=DEVICE_CLASS_DISTANCE,
        )
    )
    .extend(
        {
            cv.Optional(CONF_REBOOT_ON_LIMITS_CHANGE, default=False): cv.boolean,
        }
    )
    .extend(cv.COMPONENT_SCHEMA)
)

async def to_code(config):
    parent = await cg.get_variable(config[CONF_JIECANG_DESK_ID])
    var = await number.new_number(config, min_value=float('nan'), max_value=float('nan'), step=0.1)
    await cg.register_component(var, config)

    cg.add(var.set_parent(parent))
    cg.add(var.set_reboot_on_limits_change(config[CONF_REBOOT_ON_LIMITS_CHANGE]))
    cg.add(parent.add_listener(var))
