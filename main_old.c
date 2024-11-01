/*
* SPDX-FileCopyrightText: 2010-2022 Espressif Systems (Shanghai) CO LTD
*
* SPDX-License-Identifier: CC0-1.0
*/

#include <stdio.h>
#include <inttypes.h>
#include "sdkconfig.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_chip_info.h"
#include "esp_sleep.h"
#include "esp_console.h"
#include "esp_system.h"
#include "driver/gpio.h"

#define TASK_NAME_BUFFER_SIZE 20
#define NUM_INTERVALS 3

// maps task numbers to which pin they control
const gpio_num_t pin_lookup[] = {
	GPIO_NUM_26,
	GPIO_NUM_14,
	GPIO_NUM_33
};

struct IntervalTimerArgs {
	char name[TASK_NAME_BUFFER_SIZE];
	TickType_t on_time;
	TickType_t total_time;
	gpio_num_t pin_num;
};

void interval_timer_task(void *interval_timer_args) {
	struct IntervalTimerArgs *args = (struct IntervalTimerArgs *)interval_timer_args;
	printf("Hello world! I'm %s.\n", args->name);

	gpio_reset_pin(args->pin_num);
	gpio_set_direction(args->pin_num, GPIO_MODE_OUTPUT);

	TickType_t last_wake_time = xTaskGetTickCount();
	while (true) {
		printf("%s timer went off! Turning pin ON\n", args->name);
		gpio_set_level(args->pin_num, 1);
		vTaskDelayUntil(&last_wake_time, args->on_time);
		printf("%s timer went off! Turning pin OFF\n", args->name);
		gpio_set_level(args->pin_num, 0);
		vTaskDelayUntil(&last_wake_time, args->total_time - args->on_time);
	}
}

void app_main(void) {
	printf("Hello world!\n");
	struct IntervalTimerArgs *tasks = malloc(NUM_INTERVALS * sizeof(struct IntervalTimerArgs));
	if (tasks == NULL) {
		printf("couldn't allocate memory for task structs\n");
		exit(1);
	}
	for (int i = 0; i < NUM_INTERVALS; ++i) {
		sprintf(tasks[i].name, "Interval #%d", i);
		tasks[i].total_time = (i + 1) * 500 / portTICK_PERIOD_MS;
		tasks[i].on_time = 100 / portTICK_PERIOD_MS;
		tasks[i].pin_num = pin_lookup[i];
		if (xTaskCreate(&interval_timer_task, tasks[i].name, 2048, &tasks[i], 1, NULL)) {
			printf("created task #%d successfully!\n", i);
		} else {
			printf("task #%d could not be created\n", i);
		}
	}
	printf("Y'all should be set up now. Good night.\n");

	// set up the repl
	// esp_console_dev_uart_config_t hw_config = ESP_CONSOLE_DEV_UART_CONFIG_DEFAULT();
	// esp_console_repl_config_t repl_config = ESP_CONSOLE_REPL_CONFIG_DEFAULT();
	// esp_console_repl_t *repl = NULL;
	// esp_console_new_repl_uart(&hw_config, &repl_config, &repl);
}