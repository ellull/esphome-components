import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import sensor
from esphome.const import (
    DEVICE_CLASS_DISTANCE,
    STATE_CLASS_MEASUREMENT,
    ICON_ARROW_EXPAND_VERTICAL,
)
from .. import (
    JIECANG_DESK_COMPONENT_SCHEMA,
    CONF_JIECANG_DESK_ID,
    jiecang_desk_ns,
)

DEPENDENCIES = ["jiecang_desk"]

JiecangDeskHeightSensor = jiecang_desk_ns.class_("JiecangDeskHeightSensor", sensor.Sensor, cg.Component)

CONFIG_SCHEMA = (
    JIECANG_DESK_COMPONENT_SCHEMA
    .extend(
        sensor.sensor_schema(
            JiecangDeskHeightSensor,
            icon=ICON_ARROW_EXPAND_VERTICAL,
            accuracy_decimals=1,
            device_class=DEVICE_CLASS_DISTANCE,
            state_class=STATE_CLASS_MEASUREMENT,
        )
    )
    .extend(cv.COMPONENT_SCHEMA)
)

async def to_code(config):
    parent = await cg.get_variable(config[CONF_JIECANG_DESK_ID])
    var = await sensor.new_sensor(config)
    await cg.register_component(var, config)

    cg.add(parent.add_height_listener(var))
