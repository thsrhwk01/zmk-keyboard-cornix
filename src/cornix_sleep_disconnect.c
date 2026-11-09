/*
 * Terminate BLE links gracefully before ZMK enters deep sleep.
 *
 * ZMK powers off without disconnecting, so hosts must detect the lost link via
 * supervision timeout. Some hosts (seen with a Realtek adapter on Windows 11)
 * never do and keep a stale "connected" link, which blocks reconnection after
 * the keyboard wakes up until Bluetooth is toggled on the host.
 *
 * SPDX-License-Identifier: MIT
 */

#include <zephyr/kernel.h>
#include <zephyr/bluetooth/conn.h>
#include <zephyr/bluetooth/hci.h>
#include <zephyr/logging/log.h>

#include <zmk/event_manager.h>
#include <zmk/events/activity_state_changed.h>

LOG_MODULE_DECLARE(zmk, CONFIG_ZMK_LOG_LEVEL);

/* Upper bound on how long sleep is delayed while links are torn down. */
#define SLEEP_DISCONNECT_TIMEOUT_MS 500

static K_SEM_DEFINE(disconnected_sem, 0, CONFIG_BT_MAX_CONN);
static atomic_t waiting;

static void on_disconnected(struct bt_conn *conn, uint8_t reason) {
    if (atomic_get(&waiting)) {
        k_sem_give(&disconnected_sem);
    }
}

BT_CONN_CB_DEFINE(cornix_sleep_conn_callbacks) = {
    .disconnected = on_disconnected,
};

static void disconnect_conn(struct bt_conn *conn, void *data) {
    struct bt_conn_info info;
    int *pending = data;

    if (bt_conn_get_info(conn, &info) < 0 || info.state != BT_CONN_STATE_CONNECTED) {
        return;
    }

    if (bt_conn_disconnect(conn, BT_HCI_ERR_REMOTE_USER_TERM_CONN) == 0) {
        (*pending)++;
    }
}

static int sleep_disconnect_listener(const zmk_event_t *eh) {
    const struct zmk_activity_state_changed *ev = as_zmk_activity_state_changed(eh);

    if (ev == NULL || ev->state != ZMK_ACTIVITY_SLEEP) {
        return ZMK_EV_EVENT_BUBBLE;
    }

    int pending = 0;

    k_sem_reset(&disconnected_sem);
    atomic_set(&waiting, 1);
    bt_conn_foreach(BT_CONN_TYPE_LE, disconnect_conn, &pending);

    /* Give the controller time to deliver LL_TERMINATE_IND before power off. */
    int64_t deadline = k_uptime_get() + SLEEP_DISCONNECT_TIMEOUT_MS;
    int done = 0;

    while (done < pending) {
        int64_t remaining = deadline - k_uptime_get();

        if (remaining <= 0 || k_sem_take(&disconnected_sem, K_MSEC(remaining)) != 0) {
            break;
        }
        done++;
    }

    atomic_set(&waiting, 0);
    LOG_INF("Disconnected %d/%d BLE link(s) before sleep", done, pending);

    return ZMK_EV_EVENT_BUBBLE;
}

ZMK_LISTENER(cornix_sleep_disconnect, sleep_disconnect_listener);
ZMK_SUBSCRIPTION(cornix_sleep_disconnect, zmk_activity_state_changed);
