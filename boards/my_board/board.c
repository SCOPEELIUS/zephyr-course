#include <zephyr/platform/hooks.h>
#include <zephyr/sys/printk.h>

void board_early_init_hook(void)
{
	printk("Board Initialized\n");
}