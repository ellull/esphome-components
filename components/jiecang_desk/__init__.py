import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import uart
from esphome.const import CONF_ID

CODEOWNERS = ["@ellull"]

CONF_JIECANG_DESK_ID = "jiecang_desk_id"

jiecang_desk_ns = cg.esphome_ns.namespace("jiecang_desk")

JiecangDeskComponent = jiecang_desk_ns.class_("JiecangDeskComponent", cg.PollingComponent, uart.UARTDevice)

JIECANG_DESK_COMPONENT_SCHEMA = cv.Schema(
    {
        cv.GenerateID(CONF_JIECANG_DESK_ID): cv.use_id(JiecangDeskComponent),
    }
)

CONFIG_SCHEMA = cv.All(
    cv.Schema(
        {
            cv.GenerateID(): cv.declare_id(JiecangDeskComponent),
        }
    )
    .extend(uart.UART_DEVICE_SCHEMA)
    .extend(cv.polling_component_schema("1min"))
)
FINAL_VALIDATE_SCHEMA = uart.final_validate_device_schema(
    "jiecang_desk",
    baud_rate=9600,
    require_tx=True,
    require_rx=True,
    data_bits=8,
    parity=None,
    stop_bits=1,
)

async def to_code(config):
    var = cg.new_Pvariable(config[CONF_ID])
    await cg.register_component(var, config)
    await uart.register_uart_device(var, config)
