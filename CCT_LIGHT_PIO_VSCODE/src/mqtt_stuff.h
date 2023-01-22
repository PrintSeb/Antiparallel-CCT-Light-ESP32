
void reconnect()
{
int fails = 0;
 const char *mqttname = lightName.c_str();
  while (!mqtt_client.connected())
  {
    if (mqtt_client.connect(mqttname))
    {
          Serial.print("MQTT Connected");
          mqtt_client.subscribe(String(on_topic).c_str());
          mqtt_client.subscribe(String(bri_topic).c_str());
          mqtt_client.subscribe(String(bri_level_topic).c_str());
          mqtt_client.subscribe(String(transitiontime_topic).c_str());
          mqtt_client.subscribe(String(command_topic).c_str());
          mqtt_client.subscribe(String(ct_topic).c_str());
          mqtt_client.subscribe(String(cct_topic).c_str());
    }
    else
    {
      delay(1000);
      fails++;
      Serial.println("MQTT -> try again");
    }
    if (fails > 10)
    {
      break;
    }
  }
}

void mqtt_connector()
{
  if (!mqtt_client.connected())
  {
    reconnect();
  }
  else
  {
    mqtt_client.loop();

  }
}

void mqtt_publish()
{
  if (millis() > previousMillis_mqtt + 1000){
    block_mqtt_publish = false;
  }
  if(!block_mqtt_publish){
  if (touch_end)
  {
    unsigned long now = millis();
    if (now <= previousMillis_controller + 500)
    {
      mqtt_client.publish(mqtt_topic, String("0").c_str(), true);
    }
    else if (now > previousMillis_controller + 500)
    {
      mqtt_client.publish(mqtt_topic, String("1").c_str(), true);
    }
    touch_end = false;
  }


  if(light.transitiontime != lightvalitdation.transitiontime || light.level != lightvalitdation.level || 
  light.lightState != lightvalitdation.lightState || light.ct != lightvalitdation.ct || light.bri != lightvalitdation.bri){

  DynamicJsonDocument root(1024);

  int kelvin = map(light.ct, 153, 500, 6500, 2000);

    root["on"] = light.lightState;
    root["bri"] = light.bri;
    root["level"] = light.level;
    root["ct"] = kelvin;
    root["cct"] = light.ct;
    root["transitiontime"] = light.transitiontime;

    String output;
    serializeJson(root, output);
    
    mqtt_client.publish(String(command_topic).c_str(), String(output).c_str(), true);
    mqtt_client.publish(String(bri_topic).c_str(), String(light.bri).c_str(), true);
    mqtt_client.publish(String(bri_level_topic).c_str(), String(light.level).c_str(), true);
    mqtt_client.publish(String(ct_topic).c_str(), String(kelvin).c_str(), true);
    mqtt_client.publish(String(cct_topic).c_str(), String(light.ct).c_str(), true);
    mqtt_client.publish(String(transitiontime_topic).c_str(), String(light.transitiontime).c_str(), true);
    if(light.lightState){mqtt_client.publish(String(on_topic).c_str(),"true");}
    else{mqtt_client.publish(String(on_topic).c_str(),"false");}  

    lightvalitdation.lightState = light.lightState;
    lightvalitdation.ct = light.ct;
    lightvalitdation.bri = light.bri;
    lightvalitdation.level = light.level;
    lightvalitdation.transitiontime = light.transitiontime;

      }
}

}




void callback(char *topic, byte *payload, int length)
{

  String message;
  for (int i = 0; i < length; i++)
  {
    char c = (char)payload[i];
    message += c;
  }


if (strcmp(topic, String(on_topic).c_str()) == 0)
  {
    if ((char)payload[0] == '0' || message == "false")
    {
      light.lightState=false;
    }
    else if ((char)payload[0] == '1' || message == "true")
    {
      light.lightState=true;
    }
  }
   if (strcmp(topic, String(bri_topic).c_str()) == 0)
  {
    light.bri=message.toInt();
  }
     if (strcmp(topic, String(bri_level_topic).c_str()) == 0)
  {
    light.level=message.toInt();
    light.bri = map(message.toInt(), 0, 100, 0, 255);
  }
     if (strcmp(topic, String(transitiontime_topic).c_str()) == 0)
  {
    light.transitiontime = message.toInt();
  }
     if (strcmp(topic, String(ct_topic).c_str()) == 0)
  {
    int kelvin = map(message.toInt(), 6500, 2000, 153, 500);
    light.ct = kelvin;
  }     
  if (strcmp(topic, String(cct_topic).c_str()) == 0)
  {
    light.ct = message.toInt();
  }
  

     if (strcmp(topic, String(command_topic).c_str()) == 0)
  {
   DynamicJsonDocument root(1024);
deserializeJson(root, payload,length);

if (root.containsKey("ct")) {
      int kelvin = root["ct"];
      kelvin = map(message.toInt(), 6500, 2000, 153, 500);
      light.ct = kelvin;
      }
            if (root.containsKey("cct")) {
        light.ct = root["cct"];
      }

      if (root.containsKey("on")) {
        if (root["on"]) {
          light.lightState = true;
        } else {
          light.lightState = false;
        }
      }
      if (root.containsKey("bri")) {
        light.bri = root["bri"];
      }
      if (root.containsKey("bri_inc")) {
        light.bri += (int) root["bri_inc"];
        if (light.bri >= 255) light.bri = 255;
        else if (light.bri < 1) light.bri = 1;
      }
      if (root.containsKey("transitiontime")) {
        light.transitiontime = root["transitiontime"];
      }    
  
  }

        if (light.transitiontime < 0) {light.transitiontime = 0;}
        if (light.bri >= 255) {light.bri = 255;}
        else if (light.bri < 0) {light.bri = 0;}
        if (light.ct < 153){light.ct = 153;}
        else if (light.ct > 500){light.ct = 500;}
        block_mqtt_publish = true;
        previousMillis_mqtt = millis();
        processLightdata();
}