#include "zephyr/device.h"
#include <zephyr/drivers/pwm.h>
#include <zephyr/drivers/sensor.h>
#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>

LOG_MODULE_REGISTER(main, LOG_LEVEL_INF);

const struct device *my_driver_dev = DEVICE_DT_GET(DT_ALIAS(custom_driver));

int main(void) {
  if (!IS_ENABLED(CONFIG_MY_LED_SUBSYSTEM)) {
    return 0;
  }

  if (!device_is_ready(my_driver_dev)) {
    LOG_ERR("Custom device not found or not ready");
	return -ENODEV;
  } else {
    LOG_INF("Custom device found. Executing driver test...");
  }

  while (1) {
    struct sensor_value val;

    sensor_sample_fetch(my_driver_dev);
    LOG_INF("Called sample_fetch -> LED should be ON");
    k_msleep(2000);

    int ret = sensor_channel_get(my_driver_dev, SENSOR_CHAN_ALL, &val);
    LOG_INF("Called channel_get (returned %d) -> LED should be OFF", ret);
    k_msleep(2000);
  }
  return 0;
}