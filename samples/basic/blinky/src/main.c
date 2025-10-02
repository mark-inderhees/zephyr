/*
 * Copyright (c) 2016 Intel Corporation
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <stdio.h>
#include <zephyr/kernel.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/logging/log.h>
#include <hello_driver.h>

LOG_MODULE_REGISTER(main, LOG_LEVEL_INF);

#define HELLO_DRIVER_NODE DT_NODELABEL(hello_driver)


/* 1000 msec = 1 sec */
#define SLEEP_TIME_MS   1000

/* The devicetree node identifier for the "led0" alias. */
#define LED0_NODE DT_ALIAS(led0)

/*
 * A build error on this line means your board is unsupported.
 * See the sample documentation for information on how to fix this.
 */
static const struct gpio_dt_spec led = GPIO_DT_SPEC_GET(LED0_NODE, gpios);

int main(void)
{
	int ret;
	bool led_state = true;

	const struct device *hello_driver_dev = DEVICE_DT_GET(HELLO_DRIVER_NODE);

	if (!device_is_ready(hello_driver_dev)) {
		LOG_ERR("Hello driver is not ready.");
		return 0;
	}

	LOG_INF("Hello from the app, retrieved from device: %p", hello_driver_dev);

	ret = hello_driver_print_message(hello_driver_dev);
	if (ret != 0) {
		LOG_ERR("Failed to print message from hello driver: %d", ret);
	}

	if (!gpio_is_ready_dt(&led)) {
		return 0;
	}

	ret = gpio_pin_configure_dt(&led, GPIO_OUTPUT_ACTIVE);
	if (ret < 0) {
		return 0;
	}

	while (1) {
		ret = gpio_pin_toggle_dt(&led);
		if (ret < 0) {
			return 0;
		}

		led_state = !led_state;
		printf("LED state: %s\n", led_state ? "ON" : "OFF");
		k_msleep(SLEEP_TIME_MS);
	}
	return 0;
}
