#pragma once

#include <GyverDBFile.h>
#include <AutoOTA.h>

extern GyverDBFile db;

DB_KEYS(
    kk,
    wifi_ssid,
    wifi_pass,
    show_ip,

    ntp_gmt,
    ntp_host,

    bright,

    clock_enabled,
    clock_rotation,

    back_mode, // 0 none, 1 grad, 2 perlin
    back_pal,
    back_speed,
    back_angle,
    back_scale,
    back_bright);