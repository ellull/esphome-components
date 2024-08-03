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

CONF_HEIGHT = "height"
CONF_MAX_LIMIT = "max_limit"
CONF_MIN_LIMIT = "min_limit"

JiecangDeskHeightSensor = jiecang_desk_ns.class_("JiecangDeskHeightSensor", sensor.Sensor, cg.Component)
JiecangDeskMaxLimitSensor = jiecang_desk_ns.class_("JiecangDeskMaxLimitSensor", sensor.Sensor, cg.Component)
JiecangDeskMinLimitSensor = jiecang_desk_ns.class_("JiecangDeskMinLimitSensor", sensor.Sensor, cg.Component)

CONFIG_SCHEMA = (
    JIECANG_DESK_COMPONENT_SCHEMA
    .extend(
        {
            cv.Optional(CONF_HEIGHT): sensor.sensor_schema(
                JiecangDeskHeightSensor,
                icon=ICON_ARROW_EXPAND_VERTICAL,
                accuracy_decimals=1,
                device_class=DEVICE_CLASS_DISTANCE,
                state_class=STATE_CLASS_MEASUREMENT,
            ),
            cv.Optional(CONF_MAX_LIMIT): sensor.sensor_schema(
                JiecangDeskMaxLimitSensor,
                icon="mdi:arrow-collapse-up",
                accuracy_decimals=1,
                device_class=DEVICE_CLASS_DISTANCE,
                state_class=STATE_CLASS_MEASUREMENT,
            ),
            cv.Optional(CONF_MIN_LIMIT): sensor.sensor_schema(
                JiecangDeskMinLimitSensor,
                icon="mdi:arrow-collapse-down",
                accuracy_decimals=1,
                device_class=DEVICE_CLASS_DISTANCE,
                state_class=STATE_CLASS_MEASUREMENT,
            ),
        }
    )
    .extend(cv.COMPONENT_SCHEMA)
)

async def to_code(config):
    parent = await cg.get_variable(config[CONF_JIECANG_DESK_ID])

    if CONF_HEIGHT in config:
        var = await sensor.new_sensor(config[CONF_HEIGHT])
        await cg.register_component(var, config)
        cg.add(parent.add_listener(var))

    if CONF_MAX_LIMIT in config:
        var = await sensor.new_sensor(config[CONF_MAX_LIMIT])
        await cg.register_component(var, config)
        cg.add(parent.add_listener(var))

    if CONF_MIN_LIMIT in config:
        var = await sensor.new_sensor(config[CONF_MIN_LIMIT])
        await cg.register_component(var, config)
        cg.add(parent.add_listener(var))
