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

static void update(sets::Updater &u)
{
    // Looper.getTimer("redraw")->restart(100);
}

static void build(sets::Builder &b)
{
    {
        sets::Group g(b, "Общие");
        b.Slider(kk::bright, "Яркость", 0, 255);
    }
    {
        sets::Group g(b, "Часы");
        if (b.Switch(kk::clock_enabled, "Включить часы?"))
        {
            b.reload();
        }
        if (db[kk::clock_enabled])
        {
            b.Switch(kk::clock_face_enabled, "Включить циферблат?");
            b.Slider(kk::clock_rotation, "Поворот часов, град.", 0, 359);
            b.Color(kk::clock_min_color, "Цвет минутной стрелки");
            b.Color(kk::clock_hour_color, "Цвет часовой стрелки");

            b.Paragraph("---");
            b.Input(kk::ntp_gmt, "Часовой пояс");
            b.Input(kk::ntp_host, "NTP сервер");
            b.LED("synced"_h, "Синхронизирован", NTP.synced());
            b.Label("local_time"_h, "Локальное время", NTP.timeToString());
        }
        // b.Color(kk::clock_color, "Цвет");
    }
    {
        sets::Group g(b, "Фон");
        if (b.Select(kk::back_mode, "Фон", "Нет;Градиент;Перлин"))
        {
            b.reload();
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
        sets::Group g(b, "WiFi");

        b.Switch(kk::show_ip, "Показывать IP");
        b.Input(kk::wifi_ssid, "SSID");
        b.Pass(kk::wifi_pass, "Pass", "");

        if (b.Button("wifi_save"_h, "Подключить"))
        {
            Looper.pushEvent("wifi_connect");
        }
    }

    if (b.build.isAction())
    {
        switch (b.build.id)
        {
        case kk::ntp_gmt:
            NTP.setGMT(b.build.value);
            break;
        case kk::ntp_host:
            NTP.setHost(b.build.value);
            break;
        }
    }

    Looper.getTimer("redraw")->restart(100);
    if (b.Button("restart"_h, "restart"))
        ESP.restart();
}

LP_LISTENER_("wifi_connect", []()
             {
    db.update();
    WiFiConnector.connect(db[kk::wifi_ssid], db[kk::wifi_pass]); 
    });

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
        db.init(kk::ntp_gmt, 5);

        
        db.init(kk::bright, 100);

        db.init(kk::clock_enabled, true);
        db.init(kk::clock_rotation, 331);
        db.init(kk::clock_face_enabled, true);
        // db.init(kk::clock_style, 1);
        db.init(kk::clock_min_color, 0xffffff);
        db.init(kk::clock_hour_color, 0xff00ff);

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
    NTP.tick(); });