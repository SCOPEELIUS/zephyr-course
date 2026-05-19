#ifndef OUR_DRIVER_H
#define OUR_DRIVER_H

#include <zephyr/device.h>

int our_driver_set_delay(const struct device *dev, int delay);

#endif 