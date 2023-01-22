// GPIO 12 + 17 OUTPUT ... define in core_configs.h


#include <Arduino.h>
#include "prototypes.h"

void setup()
{

  // Serial.begin(115200);
  controller_setup();

  Serial.println("mounting FS...");
  LITTLEFS.begin();
  // LITTLEFS.format();

  if (!LITTLEFS.begin())
  {
    Serial.println("Failed to mount file system");
    LITTLEFS.format();
    saveConfig();
    return;
  }
  else
  {
    restoreConfig();
    if (firstboot == 1)
    {
      firstboot = 0;
      Serial.println("new config");
      saveConfig();
    }
    Serial.println("Config loaded");
  }

  String hostname_v = lightName;
  hostname_v.replace(" ", "-");
  const char *hostname = (hostname_v.c_str());

  ledcSetup(cct_ledChannel, cct_freq, cct_resolution);
  ledcAttachPin(pins[0], cct_ledChannel);
  ledcSetup(bri_ledChannel, bri_freq, bri_resolution);
  ledcAttachPin(pins[1], bri_ledChannel);

  /* for (uint8_t pin = 0; pin < PWM_CHANNELS; pin++) {
      pinMode(pins[pin], OUTPUT);
      analogWrite(pins[pin], 0);
    }*/

  if (startup == 1)
  {
    light.lightState = true;
  }
  if (startup == 0)
  {
    restoreState();
  }
  else
  {
    apply_scene(scene);
  }
  processLightdata();
  light.transitiontime = 10;
  if (light.lightState)
  {
    for (uint8_t i = 0; i < 200; i++)
    {
      lightEngine();
    }
  }

  if (!MDNS.begin(hostname))
  { // http://esp32.local
    Serial.println("Error setting up MDNS responder!");
    delay(1000);
  }

#ifdef USE_WIFIMANAGER

  WiFiManager wifiManager;

  if (!useDhcp)
  {
    // wifiManager.setSTAStaticIPConfig(address, gateway, submask);
  }
  // wifiManager.autoConnect();
  wifiManager.setTimeout(180);
  if (!wifiManager.autoConnect(hostname))
  {
    delay(3000);
    ESP.restart();
    delay(5000);
  }

#else
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  while (WiFi.waitForConnectResult() != WL_CONNECTED)
  {
    Serial.println("Connection Failed! Rebooting...");
    delay(5000);
    ESP.restart();
  }

#endif

  if (useDhcp)
  {
    address = WiFi.localIP();
    gateway = WiFi.gatewayIP();
    submask = WiFi.subnetMask();
  }

  if (!light.lightState)
  {
    // Show that we are connected
    ledcWrite(bri_ledChannel, 100);
    delay(500);
    ledcWrite(bri_ledChannel, 0);
  }

  WiFi.macAddress(mac);
  Serial.print("mac: ");
  Serial.println(WiFi.macAddress());

  WiFi.setHostname(hostname);

  // mqtt_client.connect(hostname);
  lastReconnectAttempt = 0;

  Udp.begin(2100);

  serverMainconfig();
  server.onNotFound(handleNotFound);
  mqtt_client.setCallback(callback);

  server.begin();
  ArduinoOTA.setHostname(hostname);
  ArduinoOTA.begin();
  Serial.print("Work hard");
}

void loop()
{
  ArduinoOTA.handle();
  server.handleClient();
  lightEngine();
  mqtt_connector();
  mqtt_publish();

  int wifi_retry = 0;
  while (WiFi.status() != WL_CONNECTED && wifi_retry < 5)
  {
    wifi_retry++;
    Serial.println("WiFi not connected. Try to reconnect");
    WiFi.disconnect();
    WiFi.mode(WIFI_OFF);
    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid, password);
    delay(100);
  }
  if (wifi_retry >= 5)
  {
    Serial.println("Reboot");
    ESP.restart();
  }
}
