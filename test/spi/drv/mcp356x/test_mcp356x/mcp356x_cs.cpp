#include "cy8c95xx.h"
#include "mcp356x.h"
#include "hw_platform.h"

#define _MCP356X_CS (14u)
 
static void cs_init(void)
{
#ifdef NEO_ENV
    static uint8_t cs_initialized = 0;
    if (!cs_initialized) {
        platform_gpio_init(_MCP356X_CS, PLATFORM_GPIO_OUT, PLATFORM_GPIO_PULL_DISABLED);
    }
    cs_initialized = 1;
#endif
}
 
void mcp356x_cs_select(int cs_pin)
{
#ifdef NEO_ENV
    cs_init();
    platform_gpio_set(_MCP356X_CS, false);
#endif
}
 
void mcp356x_cs_deselect(int cs_pin)
{
#ifdef NEO_ENV
    cs_init();
    platform_gpio_set(_MCP356X_CS, true);
#endif
}