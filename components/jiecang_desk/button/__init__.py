import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import button
from .. import (
    JIECANG_DESK_COMPONENT_SCHEMA,
    CONF_JIECANG_DESK_ID,
    jiecang_desk_ns,
)

DEPENDENCIES = ["jiecang_desk"]

CONF_POSITION_1 = "position_1"
CONF_POSITION_2 = "position_2"
CONF_POSITION_3 = "position_3"
CONF_POSITION_4 = "position_4"

JiecangDeskCommandButton = jiecang_desk_ns.class_("JiecangDeskCommandButton", button.Button, cg.Component)

CONFIG_SCHEMA = JIECANG_DESK_COMPONENT_SCHEMA.extend(
    {
        cv.Optional(CONF_POSITION_1): button.button_schema(
            JiecangDeskCommandButton,
            icon="mdi:numeric-1-box",
        ),
        cv.Optional(CONF_POSITION_2): button.button_schema(
            JiecangDeskCommandButton,
            icon="mdi:numeric-2-box",
        ),
        cv.Optional(CONF_POSITION_3): button.button_schema(
            JiecangDeskCommandButton,
            icon="mdi:numeric-3-box",
        ),
        cv.Optional(CONF_POSITION_4): button.button_schema(
            JiecangDeskCommandButton,
            icon="mdi:numeric-4-box",
        ),
    }
).extend(cv.COMPONENT_SCHEMA)

async def to_code(config):
    parent = await cg.get_variable(config[CONF_JIECANG_DESK_ID])

    if CONF_POSITION_1 in config:
        var = await button.new_button(config[CONF_POSITION_1])
        await cg.register_component(var, config)
        cg.add(var.set_parent(parent))
        cg.add(var.set_command(0x05))

    if CONF_POSITION_2 in config:
        var = await button.new_button(config[CONF_POSITION_2])
        await cg.register_component(var, config)
        cg.add(var.set_parent(parent))
        cg.add(var.set_command(0x06))

    if CONF_POSITION_3 in config:
        var = await button.new_button(config[CONF_POSITION_3])
        await cg.register_component(var, config)
        cg.add(var.set_parent(parent))
        cg.add(var.set_command(0x25))

    if CONF_POSITION_4 in config:
        var = await button.new_button(config[CONF_POSITION_4])
        await cg.register_component(var, config)
        cg.add(var.set_parent(parent))
        cg.add(var.set_command(0x26))
