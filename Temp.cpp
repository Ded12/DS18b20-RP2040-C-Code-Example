#include <cstdio>
#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include "one_wire.h"
#include "hardware/timer.h"
#include "hardware/uart.h"

#define TEMP_SENSE_GPIO_PIN 17
#define EXIT_GPIO_PIN 22
#define UART_ID uart0
#define BAUD_RATE 9600
float Value;
char data[6] ;
void Gpio_initall(){
	uart_init(UART_ID, BAUD_RATE);
    gpio_set_function(0, GPIO_FUNC_UART);
    gpio_set_function(1, GPIO_FUNC_UART);
}
int main() {
	stdio_init_all();
	Gpio_initall();
	One_wire one_wire(TEMP_SENSE_GPIO_PIN);
	one_wire.init();
	gpio_init(EXIT_GPIO_PIN);
	gpio_set_dir(EXIT_GPIO_PIN, GPIO_IN);
	gpio_pull_up(EXIT_GPIO_PIN);
	sleep_ms(1);
	while (1) {
		int count = one_wire.find_and_count_devices_on_bus();
		rom_address_t null_address{};

		one_wire.convert_temperature(null_address, true, true);
                sleep_ms(750);
		for (int i = 0; i < count; i++) {
			auto address = One_wire::get_address(i);
			Value =  one_wire.temperature(address);
			sprintf(data, "%3.1f", Value);
			printf("\r\n", data);
			uart_puts(UART_ID, "-");
			uart_puts(UART_ID, data);
			// uart_puts(UART_ID, "-");
			// uart_puts(UART_ID, "30.2");
			uart_puts(UART_ID, "-");
			uart_puts(UART_ID, "32.2");
			
		}

	   		
	}
	return 0;
}
