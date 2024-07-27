import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import cover
from esphome.const import CONF_ID
from .. import (
    JIECANG_DESK_COMPONENT_SCHEMA,
    CONF_JIECANG_DESK_ID,
    jiecang_desk_ns,
)

DEPENDENCIES = ["jiecang_desk"]

JiecangDeskCover = jiecang_desk_ns.class_("JiecangDeskCover", cover.Cover, cg.Component)

CONFIG_SCHEMA = (
    JIECANG_DESK_COMPONENT_SCHEMA
    .extend(cover.COVER_SCHEMA)
    .extend({cv.GenerateID(): cv.declare_id(JiecangDeskCover)})
    .extend(cv.COMPONENT_SCHEMA)
)

async def to_code(config):
    parent = await cg.get_variable(config[CONF_JIECANG_DESK_ID])
    var = cg.new_Pvariable(config[CONF_ID])
    await cover.register_cover(var, config)

    cg.add(var.set_parent(parent))
    cg.add(parent.add_height_listener(var))
