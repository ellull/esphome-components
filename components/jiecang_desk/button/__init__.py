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
CONF_UP = "up"
CONF_DOWN = "down"
CONF_STOP = "stop"

JiecangDeskCommand = jiecang_desk_ns.enum("JiecangDeskCommand")

JiecangDeskCommandButton = jiecang_desk_ns.class_("JiecangDeskCommandButton", button.Button, cg.Component)

CONFIG_SCHEMA = (
    JIECANG_DESK_COMPONENT_SCHEMA
    .extend(
        {
            cv.Optional(CONF_POSITION_1): button.button_schema(
                JiecangDeskCommandButton,
                icon="mdi:numeric-1-circle",
            ),
            cv.Optional(CONF_POSITION_2): button.button_schema(
                JiecangDeskCommandButton,
                icon="mdi:numeric-2-circle",
            ),
            cv.Optional(CONF_POSITION_3): button.button_schema(
                JiecangDeskCommandButton,
                icon="mdi:numeric-3-circle",
            ),
            cv.Optional(CONF_POSITION_4): button.button_schema(
                JiecangDeskCommandButton,
                icon="mdi:numeric-4-circle",
            ),
            cv.Optional(CONF_UP): button.button_schema(
                JiecangDeskCommandButton,
                icon="mdi:arrow-up-circle",
            ),
            cv.Optional(CONF_DOWN): button.button_schema(
                JiecangDeskCommandButton,
                icon="mdi:arrow-down-circle",
            ),
            cv.Optional(CONF_STOP): button.button_schema(
                JiecangDeskCommandButton,
                icon="mdi:stop-circle",
            ),
        }
    )
    .extend(cv.COMPONENT_SCHEMA)
)

async def to_code(config):
    parent = await cg.get_variable(config[CONF_JIECANG_DESK_ID])

    if CONF_POSITION_1 in config:
        var = await button.new_button(config[CONF_POSITION_1])
        await cg.register_component(var, config)
        cg.add(var.set_parent(parent))
        cg.add(var.set_command(JiecangDeskCommand.POSITION_1))

    if CONF_POSITION_2 in config:
        var = await button.new_button(config[CONF_POSITION_2])
        await cg.register_component(var, config)
        cg.add(var.set_parent(parent))
        cg.add(var.set_command(JiecangDeskCommand.POSITION_2))

    if CONF_POSITION_3 in config:
        var = await button.new_button(config[CONF_POSITION_3])
        await cg.register_component(var, config)
        cg.add(var.set_parent(parent))
        cg.add(var.set_command(JiecangDeskCommand.POSITION_3))

    if CONF_POSITION_4 in config:
        var = await button.new_button(config[CONF_POSITION_4])
        await cg.register_component(var, config)
        cg.add(var.set_parent(parent))
        cg.add(var.set_command(JiecangDeskCommand.POSITION_4))

    if CONF_UP in config:
        var = await button.new_button(config[CONF_UP])
        await cg.register_component(var, config)
        cg.add(var.set_parent(parent))
        cg.add(var.set_command(JiecangDeskCommand.UP))

    if CONF_DOWN in config:
        var = await button.new_button(config[CONF_DOWN])
        await cg.register_component(var, config)
        cg.add(var.set_parent(parent))
        cg.add(var.set_command(JiecangDeskCommand.DOWN))

    if CONF_STOP in config:
        var = await button.new_button(config[CONF_STOP])
        await cg.register_component(var, config)
        cg.add(var.set_parent(parent))
        cg.add(var.set_command(JiecangDeskCommand.STOP))
