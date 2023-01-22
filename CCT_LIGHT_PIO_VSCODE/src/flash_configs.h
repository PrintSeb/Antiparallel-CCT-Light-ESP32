

void saveState() {
  //LITTLEFS.remove("/state.json");
  File file = LITTLEFS.open("/state.json", FILE_WRITE);
  if (!file) {
    Serial.println(F("Failed to create file"));
    return;
  }
  DynamicJsonDocument json(1024);
  json["on"] = light.lightState;
  json["bri"] = light.bri;
  json["ct"] = light.ct;

  if (serializeJson(json, file) == 0) {
    Serial.println(F("Failed to write to staefile"));
  }

  // Close the file
  file.close();
     Serial.println("save state file");
}



void restoreState() {
  File file = LITTLEFS.open("/state.json");
  DynamicJsonDocument json(1024);
  if (!file) {
    saveState();
    return;
  }
  DeserializationError error = deserializeJson(json, file);
  if (!file) {
    Serial.println(F("Failed to read file"));
    return;
  }
   Serial.println("restore state file");
  if (error) {
    Serial.println("Failed to parse state file");
    saveState();
    Serial.println("New state file");
    return;
  }
  light.lightState = json["on"];
  light.bri = (uint8_t)json["bri"];
  light.ct = json["ct"];
    serializeJson(json, Serial);
    Serial.println("");
    file.close();
}



  void saveConfig() {
  File configFile = LITTLEFS.open("/config.json", FILE_WRITE);
  DynamicJsonDocument json(1024);
  json["name"] = lightName;
  json["startup"] = startup;
  json["firstboot"] = firstboot;
  json["scene"] = scene;
  json["bs"] = pins[1];
  json["cs"] = pins[0];
  json["dhcp"] = useDhcp;
  JsonArray addr = json.createNestedArray("addr");
  addr.add(address[0]);
  addr.add(address[1]);
  addr.add(address[2]);
  addr.add(address[3]);
  JsonArray gw = json.createNestedArray("gw");
  gw.add(gateway[0]);
  gw.add(gateway[1]);
  gw.add(gateway[2]);
  gw.add(gateway[3]);
  JsonArray mask = json.createNestedArray("mask");
  mask.add(submask[0]);
  mask.add(submask[1]);
  mask.add(submask[2]);
  mask.add(submask[3]);
  serializeJson(json, configFile);
  serializeJson(json, Serial);
  Serial.println("");
  configFile.close();
}



void restoreConfig() {
  
  File configFile = LITTLEFS.open("/config.json");
  if (!configFile) {
    saveConfig();
    return;
  }

  DynamicJsonDocument json(1024);
  deserializeJson(json, configFile.readString());

  lightName = json["name"].as<String>();
  startup = (uint8_t) json["startup"];
  firstboot =  (uint8_t)json["firstboot"];
  scene  = (uint8_t) json["scene"];
  pins[1] = (uint8_t) json["bs"];
  pins[0] = (uint8_t) json["cs"];
  useDhcp = json["dhcp"];
  address = {json["addr"][0], json["addr"][1], json["addr"][2], json["addr"][3]};
  submask = {json["mask"][0], json["mask"][1], json["mask"][2], json["mask"][3]};
  gateway = {json["gw"][0], json["gw"][1], json["gw"][2], json["gw"][3]};
  serializeJson(json, Serial);
  Serial.println("");
  configFile.close();
}


