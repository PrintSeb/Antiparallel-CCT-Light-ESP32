//#define USE_WIFIMANAGER

#define PWM_CHANNELS 2
#define CHANNEL_1 12
#define CHANNEL_2 17

#define bri_freq 20000
#define bri_resolution 10
#define cct_freq 500
#define cct_resolution 10
#define bri_ledChannel 2
#define cct_ledChannel 1

#define interruptor_pin 33
#define flow_pin 32

//#define SPIFFS LITTLEFS

#ifndef STASSID
#define STASSID "AirPort-Extreme"
#define STAPSK  "Airport1"
#endif

//#define esp_password "Jkmlj989sl!"

const char* ssid = STASSID;
const char* password = STAPSK;
String mqtt_prefix = "Wohnung/Kueche/Ulight/";

      String on_topic = mqtt_prefix + String("on");
      String bri_topic = mqtt_prefix + String("bri");
      String bri_level_topic = mqtt_prefix + String("level");
      String transitiontime_topic = mqtt_prefix + String("transitiontime");
      String command_topic = mqtt_prefix + String("command");
      String ct_topic = mqtt_prefix + String("ct");
      String cct_topic = mqtt_prefix + String("cct");


IPAddress address ( 192,  168,   111,  80); // choose an unique IP Adress
IPAddress gateway ( 192,  168,   111,   1); // Router IP
IPAddress submask (255, 255, 255,   0);
IPAddress mqtt_server (192,  168,   111,   9);
WiFiClient espClient; 

const int mqtt_port = 1883;

long lastReconnectAttempt = 0;
const char* mqtt_topic = "Wohnung/Kueche/TouchButton";
const char* mqtt_payload = "TouchButton";
PubSubClient mqtt_client(mqtt_server, mqtt_port, espClient);
bool block_mqtt_publish = true;


//for LED status
//#include <Ticker.h>
//Ticker ticker;


byte mac[6], packetBuffer[8];
bool inTransition, useDhcp = true, br_update = false, update_ct = false,
touch = false, touch_end = false, inverting_ctflow = false;

struct state {
  uint8_t colors[PWM_CHANNELS], bri = 100, level = map(bri,0,255,0,100);
  bool lightState;
  int ct = 200, transitiontime = 5;
  float stepLevel[PWM_CHANNELS], currentColors[PWM_CHANNELS];
};

state light;
state lightvalitdation;

int firstboot = 1;
int wificount = 0;
int mqtt_loop_intervall = 50
;

uint8_t scene = 0, startup = false, pins[] = {CHANNEL_1, CHANNEL_2}; //could white, warm white

String lightName = "Kuchl-light";


int old_brightness, new_brightness, bri_steps, step_time, brightness_update;
int old_ct, new_ct, ct_steps, ct_steptime, ct_update;
unsigned long previousMicros_ct;
unsigned long previousMicros;
unsigned long previousMillis_controller;
unsigned long previousMillis_ctupdate;
unsigned long previousMillis_mqtt;



WebServer server(80);
WiFiUDP Udp;
//HTTPUpdateServer httpUpdateServer;
