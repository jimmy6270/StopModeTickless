#include "usart.h"
#include "gpio.h"
#include "uart_bsp.h"

int _write( int fd, const void *buf, size_t count )
{
    HAL_UART_Transmit(&hlpuart1, (uint8_t*)buf, count, HAL_MAX_DELAY);
    return count;
}
