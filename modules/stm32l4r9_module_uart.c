#include "stm32l4r9_module_uart.h"
void debug_uart_init(void) {
  hwlist_require(&hw_head, &system_clock_init, NULL);
  uart_init(USART2, 9600);
}