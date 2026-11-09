if((CONFIG_BOARD_CORNIX_LEFT OR CONFIG_BOARD_CORNIX_RIGHT OR CONFIG_BOARD_CORNIX_PH_LEFT)
   AND CONFIG_ZMK_BLE AND CONFIG_ZMK_SLEEP)
  target_sources(app PRIVATE src/cornix_sleep_disconnect.c)
endif()

# Host-facing left half: faster advertising right after it starts.
option(CORNIX_FAST_ADV "Advertise fast for 30 s after host advertising starts" ON)
if(CORNIX_FAST_ADV AND CONFIG_BOARD_CORNIX_LEFT AND CONFIG_ZMK_BLE)
  target_sources(app PRIVATE src/cornix_fast_adv.c)
  zephyr_ld_options(-Wl,--wrap=bt_le_adv_start -Wl,--wrap=bt_le_adv_stop)
endif()
