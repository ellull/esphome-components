import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import binary_sensor
from esphome.const import (
    DEVICE_CLASS_PROBLEM,
    ENTITY_CATEGORY_DIAGNOSTIC,
)

problem_ns = cg.esphome_ns.namespace("problem")
ProblemBinarySensor = problem_ns.class_(
    "ProblemBinarySensor", binary_sensor.BinarySensor, cg.Component
)

CONFIG_SCHEMA = binary_sensor.binary_sensor_schema(
    ProblemBinarySensor,
    device_class=DEVICE_CLASS_PROBLEM,
    entity_category=ENTITY_CATEGORY_DIAGNOSTIC,
).extend(cv.COMPONENT_SCHEMA)


async def to_code(config):
    var = await binary_sensor.new_binary_sensor(config)
    await cg.register_component(var, config)
