#include <zephyr/kernel.h>
#include <zephyr/drivers/pwm.h>
#include <zephyr/logging/log.h>

LOG_MODULE_REGISTER(main, LOG_LEVEL_INF);

static const struct pwm_dt_spec led_pwm = PWM_DT_SPEC_GET(DT_ALIAS(pwm_led0));

int main(void) {
	if (!IS_ENABLED(CONFIG_MY_LED_SUBSYSTEM)) {
		return 0;
	}

	if (!pwm_is_ready_dt(&led_pwm)) {
		LOG_ERR("PWM device on GPIO 5 not ready!");
		return 0;
	}

	const uint32_t brightness = CONFIG_LED_BRIGHTNESS;
	const uint32_t fade_ms = CONFIG_LED_FADE_MS;
	const uint32_t interval = CONFIG_BLINK_TIME_MS;

	LOG_INF("LED Subsystem Live: %d%% brightness, %dms fade, %dms interval", 
			brightness, fade_ms, interval);

	while (1) {
		if (fade_ms > 0) {
			for (int i = 0; i <= brightness; i++) {
				pwm_set_pulse_dt(&led_pwm, (led_pwm.period * i) / 100);
				k_msleep(fade_ms / brightness);
			}
		} else {
			pwm_set_pulse_dt(&led_pwm, (led_pwm.period * brightness) / 100);
		}

		k_msleep(interval);
		if (fade_ms > 0) {
			for (int i = brightness; i >= 0; i--) {
				pwm_set_pulse_dt(&led_pwm, (led_pwm.period * i) / 100);
				k_msleep(fade_ms / brightness);
			}
		} else {
			pwm_set_pulse_dt(&led_pwm, 0);
		}

		k_msleep(interval);
	}
	return 0;
}