#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/logging/log.h>
#include <zephyr/drivers/sensor.h>
#include <zephyr/shell/shell.h>

#include "our_driver.h"

LOG_MODULE_REGISTER(main, LOG_LEVEL_INF);

const struct device *my_driver_dev = DEVICE_DT_GET(DT_ALIAS(custom_driver));

static int cmd_sensor_fetch(const struct shell *sh, size_t argc, char **argv) {
    if (!device_is_ready(my_driver_dev)) {
        shell_error(sh, "Sensor device not ready");
        return -ENODEV;
    }

    int err = sensor_sample_fetch(my_driver_dev);
    if (err) {
        shell_error(sh, "Failed to fetch: %d", err);
    } else {
        shell_print(sh, "Fetch successful (LED ON)");
    }
    return err;
}

static int cmd_sensor_read(const struct shell *sh, size_t argc, char **argv) {
    if (!device_is_ready(my_driver_dev)) {
        shell_error(sh, "Sensor device not ready");
        return -ENODEV;
    }

    struct sensor_value val;
    int err = sensor_channel_get(my_driver_dev, SENSOR_CHAN_ALL, &val);
    if (err) {
        shell_error(sh, "Failed to read: %d", err);
    } else {
        shell_print(sh, "Read successful: val1=%d, val2=%d (LED OFF)", val.val1, val.val2);
    }
    return err;
}

static int cmd_sensor_info(const struct shell *sh, size_t argc, char **argv) {
    bool ready = device_is_ready(my_driver_dev);
    shell_print(sh, "Device Name: %s", my_driver_dev->name);
    shell_print(sh, "Ready State: %s", ready ? "true" : "false");
    return 0;
}

SHELL_STATIC_SUBCMD_SET_CREATE(sensor_subcmds,
    SHELL_CMD(fetch, NULL, "Call sensor_sample_fetch()", cmd_sensor_fetch),
    SHELL_CMD(info, NULL, "Print device name and ready state", cmd_sensor_info),
    SHELL_CMD(read, NULL, "Call sensor_channel_get() and print result", cmd_sensor_read),
    SHELL_SUBCMD_SET_END
);

SHELL_CMD_REGISTER(sensor, &sensor_subcmds, "Custom Sensor Driver Commands", NULL);


int main(void) {
    if (!device_is_ready(my_driver_dev)) {
        LOG_ERR("Custom device not found or not ready");
        return 0;
    }

    LOG_INF("System booted. Use the shell terminal to interact with the sensor driver.");
    
    while (1) {
        k_msleep(10000);
    }

    return 0;
}