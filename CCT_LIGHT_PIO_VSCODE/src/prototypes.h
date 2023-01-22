void saveState();
void restoreState();
void saveConfig();
void restoreConfig();
void apply_scene(uint8_t new_scene);
void lightEngine();
void convert_bri();
void convert_ct();
void processLightdata();
void controller_setup();
void ext_ctrl();
void switch_brigtness();
void IRAM_ATTR touch_loss();
void IRAM_ATTR detect_touch();
void mqtt_connector();
void reconnect();
void mqtt_publish();
void serverMainconfig();




#include <FS.h>
#include <LITTLEFS.h> 
#include <WiFi.h>
#include <ESPmDNS.h>
#include <WiFiUdp.h>
#include <WebServer.h>
//#include <WiFiManager.h>
#include <ArduinoJson.h>

#include <ArduinoOTA.h>
#include <PubSubClient.h>


#include "core_configs.h"
#include "light_engine.h"
#include "flash_configs.h"
#include "server_stuff.h"
#include "external_controller.h"
#include "mqtt_stuff.h"


