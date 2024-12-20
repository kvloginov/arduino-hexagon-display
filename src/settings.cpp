#include "settings.h"

#include <GyverNTP.h>
#include <LittleFS.h>
#include <Looper.h>
#include <SettingsAsync.h>
#include <WiFiConnector.h>

#include "config.h"
#include "palettes.h"
#include "redraw.h"

GyverDBFile db(&LittleFS, "/data.db");
static SettingsAsync sett(PROJECT_NAME " v" PROJECT_VER, &db);
bool backModeFirstLoad = true;

static void update(sets::Updater &u)
{
    // Looper.getTimer("redraw")->restart(100);
}

static void build(sets::Builder &b)
{
    // {
    //     sets::Group g(b, "Часы");

    //     b.Select(kk::clock_style, "Шрифт", "Нет;Тип 1;Тип 2;Тип 3");
    //     b.Color(kk::clock_color, "Цвет");
    // }
    {
        sets::Group g(b, "Фон");

        if (b.Select(kk::back_mode, "Фон", "Нет;Градиент;Перлин")) {
            if (!backModeFirstLoad) {
                b.reload();
            }
            backModeFirstLoad = false;
        }
        if (db[kk::back_mode].toInt())
        {
            b.Select(kk::back_pal, "Палитра", getPaletteList());
            b.Slider(kk::back_bright, "Яркость", 0, 255);
            b.Slider(kk::back_speed, "Скорость");
            b.Slider(kk::back_scale, "Масштаб");
            b.Slider(kk::back_angle, "Угол", -180, 180);
        }
    }
    {
        sets::Group g(b, "Яркость");
        if (b.Switch(kk::auto_bright, "Автояркость"))
            b.reload();
        b.Label("adc_val"_h, "Сигнал с датчика");

        if (db[kk::auto_bright])
        {
            b.Slider(kk::bright_min, "Мин.", 0, 255);
            b.Slider(kk::bright_max, "Макс.", 0, 255);
        }
        else
        {
            b.Slider(kk::bright, "Яркость", 0, 255);
        }
    }
    // {
    //     sets::Group g(b, "Ночной режим");

    //     if (b.Switch(kk::night_mode, "Включен"))
    //         b.reload();

    //     if (db[kk::night_mode])
    //     {
    //         b.Color(kk::night_color, "Цвет");
    //         b.Slider(kk::night_trsh, "Порог", 0, 1023);
    //     }
    // }
    // {
    //     sets::Group g(b, "Время");

    //     b.Input(kk::ntp_gmt, "Часовой пояс");
    //     b.Input(kk::ntp_host, "NTP сервер");
    //     b.LED("synced"_h, "Синхронизирован", NTP.synced());
    //     b.Label("local_time"_h, "Локальное время", NTP.timeToString());
    // }
    {
        sets::Group g(b, "WiFi");

        b.Switch(kk::show_ip, "Показывать IP");
        b.Input(kk::wifi_ssid, "SSID");
        b.Pass(kk::wifi_pass, "Pass", "");

        if (b.Button("wifi_save"_h, "Подключить"))
        {
            Looper.pushEvent("wifi_connect");
        }
    }

    // if (b.build.isAction())
    // {
    //     switch (b.build.id)
    //     {
    //     case kk::ntp_gmt:
    //         NTP.setGMT(b.build.value);
    //         break;
    //     case kk::ntp_host:
    //         NTP.setHost(b.build.value);
    //         break;
    //     }
    // }

    // Looper.getTimer("redraw")->restart(100);
    // if (b.Button("restart"_h, "restart")) ESP.restart();
}

LP_LISTENER_("wifi_connect", []()
             {
    db.update();
    WiFiConnector.connect(db[kk::wifi_ssid], db[kk::wifi_pass]); });

LP_TICKER([]()
          {
    if (Looper.thisSetup()) {
        Serial.println("Start setup");

        LittleFS.begin();
        db.begin();
        // db.reset();

        db.init(kk::wifi_ssid, "TP-Link_9CFF");
        db.init(kk::wifi_pass, "CXZasdEWQ12#");
        db.init(kk::show_ip, true);

        db.init(kk::ntp_host, "pool.ntp.org");
        db.init(kk::ntp_gmt, 3);

        db.init(kk::bright, 100);
        db.init(kk::auto_bright, false);
        db.init(kk::bright_min, 10);
        db.init(kk::bright_max, 255);
        db.init(kk::adc_min, 0);
        db.init(kk::adc_max, 1023);

        db.init(kk::night_mode, false);
        db.init(kk::night_color, 0xff0000);
        db.init(kk::night_trsh, 50);

        db.init(kk::clock_style, 1);
        db.init(kk::clock_color, 0xffffff);

        db.init(kk::back_mode, 1);
        db.init(kk::back_pal, 0);
        db.init(kk::back_bright, 60);
        db.init(kk::back_speed, 50);
        db.init(kk::back_scale, 50);
        db.init(kk::back_angle, 0);
        
        auto isCreatedNew = WiFiConnector.connect(db[kk::wifi_ssid].str(), db[kk::wifi_pass].str());
        Serial.printf("used existed ssid?: %d\n", isCreatedNew);
        Serial.printf("ssid: %s\n", db[kk::wifi_ssid].str());
        sett.begin();
        sett.onBuild(build);
        sett.onUpdate(update);

        NTP.setHost(db[kk::ntp_host]);
        NTP.setGMT(db[kk::ntp_gmt]);
    }
    WiFiConnector.tick();
    sett.tick();
    NTP.tick(); 
    });