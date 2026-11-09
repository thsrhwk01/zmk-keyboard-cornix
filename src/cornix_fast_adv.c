/*
 * Advertise fast for a short window whenever host advertising starts.
 *
 * ZMK advertises to hosts at a fixed 100-150 ms interval. Windows scans for
 * known devices with a low duty cycle, so reconnecting after wake can take
 * several seconds. Following the common accessory practice, advertise at
 * 20-30 ms for the first 30 seconds, then fall back to ZMK's own parameters.
 *
 * ZMK hardcodes the interval in app/src/ble.c, so bt_le_adv_start() and
 * bt_le_adv_stop() are wrapped at link time (see CMakeLists.txt) instead of
 * forking ZMK.
 *
 * SPDX-License-Identifier: MIT
 */

#include <zephyr/kernel.h>
#include <zephyr/bluetooth/bluetooth.h>
#include <zephyr/bluetooth/conn.h>
#include <zephyr/logging/log.h>

LOG_MODULE_DECLARE(zmk, CONFIG_ZMK_LOG_LEVEL);

#define FAST_ADV_INT_MIN 0x0020 /* 20 ms */
#define FAST_ADV_INT_MAX 0x0030 /* 30 ms */
#define FAST_ADV_DURATION K_SECONDS(30)

int __real_bt_le_adv_start(const struct bt_le_adv_param *param, const struct bt_data *ad,
                           size_t ad_len, const struct bt_data *sd, size_t sd_len);
int __real_bt_le_adv_stop(void);

static struct bt_le_adv_param slow_param;
static const struct bt_data *saved_ad;
static size_t saved_ad_len;
static const struct bt_data *saved_sd;
static size_t saved_sd_len;
static atomic_t fast_active;

static void slowdown_work_handler(struct k_work *work) {
    if (!atomic_cas(&fast_active, 1, 0)) {
        return;
    }

    __real_bt_le_adv_stop();

    int err = __real_bt_le_adv_start(&slow_param, saved_ad, saved_ad_len, saved_sd, saved_sd_len);
    if (err) {
        LOG_ERR("Failed to restart advertising at normal interval (err %d)", err);
    }
}

static K_WORK_DELAYABLE_DEFINE(slowdown_work, slowdown_work_handler);

static void stop_fast_window(void) {
    atomic_set(&fast_active, 0);
    k_work_cancel_delayable(&slowdown_work);
}

int __wrap_bt_le_adv_start(const struct bt_le_adv_param *param, const struct bt_data *ad,
                           size_t ad_len, const struct bt_data *sd, size_t sd_len) {
    stop_fast_window();

    /* Only speed up undirected connectable advertising slower than the fast window. */
    if (param == NULL || param->peer != NULL || !(param->options & BT_LE_ADV_OPT_CONN) ||
        param->interval_min <= FAST_ADV_INT_MAX) {
        return __real_bt_le_adv_start(param, ad, ad_len, sd, sd_len);
    }

    struct bt_le_adv_param fast_param = *param;

    fast_param.interval_min = FAST_ADV_INT_MIN;
    fast_param.interval_max = FAST_ADV_INT_MAX;

    if (__real_bt_le_adv_start(&fast_param, ad, ad_len, sd, sd_len) != 0) {
        return __real_bt_le_adv_start(param, ad, ad_len, sd, sd_len);
    }

    slow_param = *param;
    saved_ad = ad;
    saved_ad_len = ad_len;
    saved_sd = sd;
    saved_sd_len = sd_len;
    atomic_set(&fast_active, 1);
    k_work_schedule(&slowdown_work, FAST_ADV_DURATION);

    return 0;
}

int __wrap_bt_le_adv_stop(void) {
    stop_fast_window();

    return __real_bt_le_adv_stop();
}

/* Connectable advertising stops by itself once a host connects. */
static void on_connected(struct bt_conn *conn, uint8_t err) {
    if (err == 0) {
        stop_fast_window();
    }
}

BT_CONN_CB_DEFINE(cornix_fast_adv_conn_callbacks) = {
    .connected = on_connected,
};
