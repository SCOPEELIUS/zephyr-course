#define DT_DRV_COMPAT our_driver

#include "our_driver.h" 
#include <zephyr/device.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/drivers/sensor.h>
#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>


LOG_MODULE_REGISTER(our_driver, LOG_LEVEL_INF);

struct our_driver_data {
  int delay;
};

struct our_driver_config {
  struct gpio_dt_spec gpio_spec;
};

int our_driver_set_delay(const struct device *dev, int delay) {
  struct our_driver_data *data = dev->data;
  if (!data) {
    return -ENODEV;
  }

  data->delay = delay;
  LOG_INF("Custom API called: Delay updated to %d ms", data->delay);
  return 0;
}

static int sensor_channel_get_impl(const struct device *dev,
                                   enum sensor_channel channel,
                                   struct sensor_value *val) {
  const struct our_driver_config *config = dev->config;
  struct our_driver_data *data = dev->data;

  LOG_INF("Getting channel value for channel %d", channel);
  val->val1 = 0;
  val->val2 = 0;

  if (gpio_pin_set_dt(&config->gpio_spec, 0) < 0) {
    LOG_ERR("Failed to clear configured GPIO pin");
    return -EIO;
  }

  if (data->delay > 0) {
    k_msleep(data->delay);
  }

  return 0;
}

static int sensor_channel_fetch_impl(const struct device *dev,
                                     enum sensor_channel channel) {
  const struct our_driver_config *config = dev->config;
  struct our_driver_data *data = dev->data;

  LOG_INF("Fetching channel value for channel %d", channel);

  if (gpio_pin_set_dt(&config->gpio_spec, 1) < 0) {
    LOG_ERR("Failed to set configured GPIO pin");
    return -EIO;
  }

  if (data->delay > 0) {
    k_msleep(data->delay);
  }

  return 0;
}

static DEVICE_API(sensor, api_lecture_5) = {
    .channel_get = sensor_channel_get_impl,
    .sample_fetch = sensor_channel_fetch_impl,
};

static int init(const struct device *dev) {
  const struct our_driver_config *config = dev->config;
  LOG_INF("Initializing driver instance: %s", dev->name);

  if (!gpio_is_ready_dt(&config->gpio_spec)) {
    LOG_ERR("Target GPIO hardware device interface is not ready");
    return -ENODEV;
  }

  int ret = gpio_pin_configure_dt(&config->gpio_spec, GPIO_OUTPUT_INACTIVE);
  if (ret < 0) {
    LOG_ERR("Failed to configure GPIO pin output mode: %d", ret);
    return ret;
  }

  return 0;
}

#define OUR_DRIVER_INST(inst)                                                  \
  static const struct our_driver_config our_driver_config_##inst = {           \
      .gpio_spec = GPIO_DT_SPEC_GET(DT_DRV_INST(inst), gpios),                 \
  };                                                                           \
  static struct our_driver_data our_driver_data_##inst;                        \
                                                                               \
  DEVICE_DT_INST_DEFINE(inst, init, NULL, &our_driver_data_##inst,             \
                        &our_driver_config_##inst, POST_KERNEL, 80,            \
                        &api_lecture_5);

DT_INST_FOREACH_STATUS_OKAY(OUR_DRIVER_INST)