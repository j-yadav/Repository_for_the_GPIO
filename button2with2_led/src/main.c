/*
 * Copyright (c) 2016 Open-RnD Sp. z o.o.
 * Copyright (c) 2020 Nordic Semiconductor ASA
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/sys/util.h>
#include <zephyr/sys/printk.h>
#include <inttypes.h>

#define SLEEP_TIME_MS	1

/*
 * Get button configuration from the devicetree sw0 alias. This is mandatory.
 */
#define SW0_NODE	DT_ALIAS(sw0)
#define SW1_NODE	DT_ALIAS(sw1)
#if !DT_NODE_HAS_STATUS(SW0_NODE, okay)
#error "Unsupported board: sw0 devicetree alias is not defined"
#endif
static const struct gpio_dt_spec button0 = GPIO_DT_SPEC_GET_OR(SW0_NODE, gpios,
							      {0});
static const struct gpio_dt_spec button1 = GPIO_DT_SPEC_GET_OR(SW1_NODE, gpios,
							      {0});
								  
static struct gpio_callback button_cb_data0;
/*
 * The led0 devicetree alias is optional. If present, we'll use it
 * to turn on the LED whenever the button is pressed.
 */
static struct gpio_dt_spec led0 = GPIO_DT_SPEC_GET_OR(DT_ALIAS(led0), gpios,
						     {0});
static struct gpio_dt_spec led1 = GPIO_DT_SPEC_GET_OR(DT_ALIAS(led1), gpios,
						     {0});

void button_pressed0(const struct device *dev, struct gpio_callback *cb,
		    uint32_t pins)
{
	printk("Button pressed at %" PRIu32 "\n", k_cycle_get_32());
}

void main(void)
{
	int ret;

	ret = gpio_pin_configure_dt(&button0, GPIO_INPUT);
	ret = gpio_pin_configure_dt(&button1, GPIO_INPUT);

	ret = gpio_pin_interrupt_configure_dt(&button0,
					      GPIO_INT_EDGE_TO_ACTIVE);
	ret = gpio_pin_interrupt_configure_dt(&button1,
					      GPIO_INT_EDGE_TO_ACTIVE);

	gpio_init_callback(&button_cb_data0, button_pressed0, BIT(button0.pin));
	gpio_add_callback(button0.port, &button_cb_data0);
	gpio_init_callback(&button_cb_data0, button_pressed0, BIT(button1.pin));
	gpio_add_callback(button1.port, &button_cb_data0);
	
		ret = gpio_pin_configure_dt(&led0, GPIO_OUTPUT);
		ret = gpio_pin_configure_dt(&led1, GPIO_OUTPUT);
		

	printk("Press the button\n");
	
		while (1) {
			/* If we have an LED, match its state to the button's. */
			int val = gpio_pin_get_dt(&button0);
			int val1 = gpio_pin_get_dt(&button1);

			if (val >= 0) {
				gpio_pin_set_dt(&led0, val);
			}
			if (val1 >= 0) {
				gpio_pin_set_dt(&led1, val1);
			}
			k_msleep(SLEEP_TIME_MS);
		}
	
}
