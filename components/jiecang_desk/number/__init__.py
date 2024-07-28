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

JiecangDeskNumber = jiecang_desk_ns.class_("JiecangDeskNumber", number.Number, cg.Component)

CONFIG_SCHEMA = (
    JIECANG_DESK_COMPONENT_SCHEMA
    .extend(
        number.number_schema(
            JiecangDeskNumber,
            device_class=DEVICE_CLASS_DISTANCE,
        )
    )
    .extend(cv.COMPONENT_SCHEMA)
)

async def to_code(config):
    parent = await cg.get_variable(config[CONF_JIECANG_DESK_ID])
    var = await number.new_number(config, min_value=0, max_value=0, step=0.1)
    await cg.register_component(var, config)

    cg.add(var.set_parent(parent))
    cg.add(parent.add_height_listener(var))
    cg.add(parent.add_limits_listener(var))
