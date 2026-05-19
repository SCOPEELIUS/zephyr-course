#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/logging/log.h>
#include <zephyr/drivers/sensor.h>
#include "our_driver.h"

LOG_MODULE_REGISTER(main, LOG_LEVEL_INF);

const struct device *my_driver_dev = DEVICE_DT_GET(DT_ALIAS(custom_driver));

int main(void) {
  if (!device_is_ready(my_driver_dev)) {
    LOG_ERR("Custom device not found or not ready");
    return 0;
  }

  struct sensor_value val;
  
  LOG_INF("Configuring driver delay via Custom API...");
  our_driver_set_delay(my_driver_dev, 5000); 

  LOG_INF("Entering continuous driver toggle loop...");

  while (1) {
    sensor_sample_fetch(my_driver_dev);
    LOG_INF("sample_fetch complete -> LED is ON");

    sensor_channel_get(my_driver_dev, SENSOR_CHAN_ALL, &val);
    LOG_INF("channel_get complete -> LED is OFF");
  }

  return 0;
}