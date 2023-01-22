


void handleNotFound() {
  String message = "File Not Found\n\n";
  message += "URI: ";
  message += server.uri();
  message += "\nMethod: ";
  message += (server.method() == HTTP_GET) ? "GET" : "POST";
  message += "\nArguments: ";
  message += server.args();
  message += "\n";
  for (uint8_t i = 0; i < server.args(); i++) {
    message += " " + server.argName(i) + ": " + server.arg(i) + "\n";
  }
  server.send(404, "text/plain", message);
}

void serverMainconfig(){
  
  server.on("/state", HTTP_PUT, []() {
    DynamicJsonDocument root(1024);
    DeserializationError error = deserializeJson(root, server.arg("plain"));
    if (error) {
      server.send(404, "text/plain", "FAIL. " + server.arg("plain"));
    } else {



      if (root.containsKey("ct")) {
        light.ct = root["ct"];
      }
      if (root.containsKey("on")) {
        if (root["on"]) {
          light.lightState = true;
        } else {
          light.lightState = false;
        }
        if (startup == 0) {
          saveState();
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

      if (root.containsKey("alert") && root["alert"] == "select") {
        if (light.lightState) {
          //        light.currentColors[0] = 0; light.currentColors[1] = 0; light.currentColors[2] = 0; light.currentColors[3] = 0;
        } else {
          //          light.currentColors[3] = 126; light.currentColors[4] = 126;
        }
      }

      String output;
      serializeJson(root, output);
      server.send(200, "text/plain", output);
      if (light.transitiontime < 0) {
        light.transitiontime = 0;
      }
      processLightdata();
    }
  });

  server.on("/state", HTTP_GET, []() {
    DynamicJsonDocument root(1024);
    root["on"] = light.lightState;
    root["bri"] = light.bri;
    root["ct"] = light.ct;
    root["colormode"] = "ct";
    String output;
    serializeJson(root, output);
    server.send(200, "text/plain", output);
  });

  server.on("/detect", []() {
    char macString[32] = {0};
    sprintf(macString, "%02X:%02X:%02X:%02X:%02X:%02X", mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);
    DynamicJsonDocument root(1024);
    root["name"] = lightName;
    root["protocol"] = "native_single";
    root["modelid"] = "LTW001";
    root["type"] = "cct";
    root["mac"] = String(macString);
    root["version"] = 2.0;
    String output;
    serializeJson(root, output);
    server.send(200, "text/plain", output);
  });

  server.on("/config", []() {
    DynamicJsonDocument root(1024);
    root["name"] = lightName;
    root["scene"] = scene;
    root["startup"] = startup;
    root["brightstate"] = pins[1];
    root["colorstate"] = pins[0];
    root["dhcp"] = (int)useDhcp;
    root["addr"] = (String)address[0] + "." + (String)address[1] + "." + (String)address[2] + "." + (String)address[3];
    root["gw"] = (String)gateway[0] + "." + (String)gateway[1] + "." + (String)gateway[2] + "." + (String)gateway[3];
    root["sm"] = (String)submask[0] + "." + (String)submask[1] + "." + (String)submask[2] + "." + (String)submask[3];
    String output;
    serializeJson(root, output);
    server.send(200, "text/plain", output);
  });

  server.on("/", []() {
    if (server.hasArg("scene")) {
      lightName = server.arg("name");
      startup = server.arg("startup").toInt();
      scene = server.arg("scene").toInt();
      pins[1] = server.arg("brightstate").toInt();
      pins[0] = server.arg("colorstate").toInt();
      saveConfig();
    } else if (server.hasArg("dhcp")) {
      useDhcp = server.arg("dhcp").toInt();
      address.fromString(server.arg("addr"));
      gateway.fromString(server.arg("gw"));
      submask.fromString(server.arg("sm"));
      saveConfig();
    }

    const char * htmlContent = "<!DOCTYPE html><html> <head> <meta charset=\"UTF-8\"> <meta name=\"viewport\" content=\"width=device-width, initial-scale=1\"> <title>Hue Light</title> <link rel=\"stylesheet\" href=\"https://diyhue.org/cdn/bootstrap.min.css\"> <link rel=\"stylesheet\" href=\"https://diyhue.org/cdn/ion.rangeSlider.min.css\"/> <script src=\"https://diyhue.org/cdn/jquery-3.3.1.min.js\"></script> <script src=\"https://diyhue.org/cdn/bootstrap.min.js\"></script> <script src=\"https://diyhue.org/cdn/ion.rangeSlider.min.js\"></script> </head> <body> <nav class=\"navbar navbar-expand-lg navbar-light bg-light rounded\"> <button class=\"navbar-toggler\" type=\"button\" data-toggle=\"collapse\" data-target=\"#navbarToggler\" aria-controls=\"navbarToggler\" aria-expanded=\"false\" aria-label=\"Toggle navigation\"> <span class=\"navbar-toggler-icon\"></span> </button> <h2></h2> <div class=\"collapse navbar-collapse justify-content-md-center\" id=\"navbarToggler\"> <ul class=\"nav nav-pills\"> <li class=\"nav-item\"> <a class=\"nav-link active\" data-toggle=\"pill\" href=\"#home\">Home</a> </li><li class=\"nav-item\"> <a class=\"nav-link\" data-toggle=\"pill\" href=\"#menu1\">Settings</a> </li><li class=\"nav-item\"> <a class=\"nav-link\" data-toggle=\"pill\" href=\"#menu2\">Network</a> </li><li class=\"nav-item\"> <a class=\"nav-link\" data-toggle=\"pill\" href=\"#\" disabled> </a> </li><li class=\"nav-item\"> <a class=\"nav-link\" data-toggle=\"pill\" href=\"#\" disabled> </a> </li></ul> </div></nav> <div class=\"tab-content\"> <div class=\"tab-pane container active\" id=\"home\"> <br><br><form> <div class=\"form-group row\"> <label for=\"power\" class=\"col-sm-2 col-form-label\">Power</label> <div class=\"col-sm-10\"> <div id=\"power\" class=\"btn-group\" role=\"group\"> <button type=\"button\" class=\"btn btn-default border\" id=\"power-on\">On</button> <button type=\"button\" class=\"btn btn-default border\" id=\"power-off\">Off</button> </div></div></div><div class=\"form-group row\"> <label for=\"bri\" class=\"col-sm-2 col-form-label\">Brightness</label> <div class=\"col-sm-10\"> <input type=\"text\" id=\"bri\" class=\"js-range-slider\" name=\"bri\" value=\"\"/> </div></div><div class=\"form-group row\"> <label for=\"hue\" class=\"col-sm-2 col-form-label\">Color</label> <div class=\"col-sm-10\"> <canvas id=\"hue\" width=\"320px\" height=\"320px\" style=\"display:none;\"></canvas> </div></div><div class=\"form-group row\"> <label for=\"color\" class=\"col-sm-2 col-form-label\">Color Temp</label> <div class=\"col-sm-10\"> <canvas id=\"ct\" width=\"320px\" height=\"50px\" style=\"border:1px solid #d3d3d3;\"></canvas> </div></div></form> </div><div class=\"tab-pane container fade\" id=\"menu1\"> <br><form method=\"POST\" action=\"/\"> <div class=\"form-group row\"> <label for=\"name\" class=\"col-sm-2 col-form-label\">Light Name</label> <div class=\"col-sm-6\"> <input type=\"text\" class=\"form-control\" id=\"name\" name=\"name\"> </div></div><div class=\"form-group row\"> <label class=\"control-label col-sm-2\" for=\"startup\">Default Power:</label> <div class=\"col-sm-4\"> <select class=\"form-control\" name=\"startup\" id=\"startup\"> <option value=\"0\">Last State</option> <option value=\"1\">On</option> <option value=\"2\">Off</option> </select> </div></div><div class=\"form-group row\"> <label class=\"control-label col-sm-2\" for=\"scene\">Default Scene:</label> <div class=\"col-sm-4\"> <select class=\"form-control\" name=\"scene\" id=\"scene\"> < <option value=\"0\">Relax</option> <option value=\"1\">Read</option> <option value=\"2\">Concentrate</option> <option value=\"3\">Energize</option> <option value=\"4\">Bright</option> <option value=\"5\">Dimmed</option> </select> </div></div><div class=\"form-group row\"> <label for=\"colorstate\" class=\"col-sm-2 col-form-label\">CCW Pin</label> <div class=\"col-sm-3\"> <input type=\"number\" class=\"form-control\" id=\"colorstate\" name=\"colorstate\" placeholder=\"\"> </div></div><div class=\"form-group row\"> <label for=\"brightstate\" class=\"col-sm-2 col-form-label\">Brightness Pin</label> <div class=\"col-sm-3\"> <input type=\"number\" class=\"form-control\" id=\"brightstate\" name=\"brightstate\" placeholder=\"\"> </div></div><div class=\"form-group row\"> <label class=\"control-label col-sm-2\" </div></div><div class=\"form-group row\"> <div class=\"col-sm-10\"> <button type=\"submit\" class=\"btn btn-primary\">Save</button> </div></div></form> </div><div class=\"tab-pane container fade\" id=\"menu2\"> <br><form method=\"POST\" action=\"/\"> <div class=\"form-group row\"> <label class=\"control-label col-sm-2\" for=\"dhcp\">DHCP:</label> <div class=\"col-sm-3\"> <select class=\"form-control\" name=\"dhcp\" id=\"dhcp\"> <option value=\"1\">On</option> <option value=\"0\">Off</option> </select> </div></div><div class=\"form-group row\"> <label for=\"addr\" class=\"col-sm-2 col-form-label\">Ip</label> <div class=\"col-sm-4\"> <input type=\"text\" class=\"form-control\" id=\"addr\" name=\"addr\"> </div></div><div class=\"form-group row\"> <label for=\"sm\" class=\"col-sm-2 col-form-label\">Submask</label> <div class=\"col-sm-4\"> <input type=\"text\" class=\"form-control\" id=\"sm\" name=\"sm\"> </div></div><div class=\"form-group row\"> <label for=\"gw\" class=\"col-sm-2 col-form-label\">Gateway</label> <div class=\"col-sm-4\"> <input type=\"text\" class=\"form-control\" id=\"gw\" name=\"gw\"> </div></div><div class=\"form-group row\"> <div class=\"col-sm-10\"> <button type=\"submit\" class=\"btn btn-primary\">Save</button> </div></div></form> </div></div><script src=\"https://diyhue.org/cdn/color.min.js\"></script> </body></html>";
    server.send(200, "text/html", htmlContent);
    if (server.args()) {
      delay(100);
      ESP.restart();
    }

  });

  server.on("/reboot", []() {
    server.send(200, "text/html", "rebooting");
    saveConfig();
    delay(1000);
    ESP.restart();
  });

  server.on("/reset", []() {
    server.send(200, "text/html", "reset");
    delay(100);
    #ifdef USE_WIFIMANAGER
    WiFiManager wifiManager;
    wifiManager.resetSettings();
    #endif
    delay(1000);
    ESP.restart();
  });
  
  
  server.on("/", []() {
    if (server.hasArg("scene")) {
      lightName = server.arg("name");
      startup = server.arg("startup").toInt();
      scene = server.arg("scene").toInt();
      pins[1] = server.arg("brightstate").toInt();
      pins[0] = server.arg("colorstate").toInt();
      saveConfig();
    } else if (server.hasArg("dhcp")) {
      useDhcp = server.arg("dhcp").toInt();
      address.fromString(server.arg("addr"));
      gateway.fromString(server.arg("gw"));
      submask.fromString(server.arg("sm"));
      saveConfig();
    }

    const char * htmlContent = "<!DOCTYPE html><html> <head> <meta charset=\"UTF-8\"> <meta name=\"viewport\" content=\"width=device-width, initial-scale=1\"> <title>Hue Light</title> <link rel=\"stylesheet\" href=\"https://diyhue.org/cdn/bootstrap.min.css\"> <link rel=\"stylesheet\" href=\"https://diyhue.org/cdn/ion.rangeSlider.min.css\"/> <script src=\"https://diyhue.org/cdn/jquery-3.3.1.min.js\"></script> <script src=\"https://diyhue.org/cdn/bootstrap.min.js\"></script> <script src=\"https://diyhue.org/cdn/ion.rangeSlider.min.js\"></script> </head> <body> <nav class=\"navbar navbar-expand-lg navbar-light bg-light rounded\"> <button class=\"navbar-toggler\" type=\"button\" data-toggle=\"collapse\" data-target=\"#navbarToggler\" aria-controls=\"navbarToggler\" aria-expanded=\"false\" aria-label=\"Toggle navigation\"> <span class=\"navbar-toggler-icon\"></span> </button> <h2></h2> <div class=\"collapse navbar-collapse justify-content-md-center\" id=\"navbarToggler\"> <ul class=\"nav nav-pills\"> <li class=\"nav-item\"> <a class=\"nav-link active\" data-toggle=\"pill\" href=\"#home\">Home</a> </li><li class=\"nav-item\"> <a class=\"nav-link\" data-toggle=\"pill\" href=\"#menu1\">Settings</a> </li><li class=\"nav-item\"> <a class=\"nav-link\" data-toggle=\"pill\" href=\"#menu2\">Network</a> </li><li class=\"nav-item\"> <a class=\"nav-link\" data-toggle=\"pill\" href=\"#\" disabled> </a> </li><li class=\"nav-item\"> <a class=\"nav-link\" data-toggle=\"pill\" href=\"#\" disabled> </a> </li></ul> </div></nav> <div class=\"tab-content\"> <div class=\"tab-pane container active\" id=\"home\"> <br><br><form> <div class=\"form-group row\"> <label for=\"power\" class=\"col-sm-2 col-form-label\">Power</label> <div class=\"col-sm-10\"> <div id=\"power\" class=\"btn-group\" role=\"group\"> <button type=\"button\" class=\"btn btn-default border\" id=\"power-on\">On</button> <button type=\"button\" class=\"btn btn-default border\" id=\"power-off\">Off</button> </div></div></div><div class=\"form-group row\"> <label for=\"bri\" class=\"col-sm-2 col-form-label\">Brightness</label> <div class=\"col-sm-10\"> <input type=\"text\" id=\"bri\" class=\"js-range-slider\" name=\"bri\" value=\"\"/> </div></div><div class=\"form-group row\"> <label for=\"hue\" class=\"col-sm-2 col-form-label\">Color</label> <div class=\"col-sm-10\"> <canvas id=\"hue\" width=\"320px\" height=\"320px\" style=\"display:none;\"></canvas> </div></div><div class=\"form-group row\"> <label for=\"color\" class=\"col-sm-2 col-form-label\">Color Temp</label> <div class=\"col-sm-10\"> <canvas id=\"ct\" width=\"320px\" height=\"50px\" style=\"border:1px solid #d3d3d3;\"></canvas> </div></div></form> </div><div class=\"tab-pane container fade\" id=\"menu1\"> <br><form method=\"POST\" action=\"/\"> <div class=\"form-group row\"> <label for=\"name\" class=\"col-sm-2 col-form-label\">Light Name</label> <div class=\"col-sm-6\"> <input type=\"text\" class=\"form-control\" id=\"name\" name=\"name\"> </div></div><div class=\"form-group row\"> <label class=\"control-label col-sm-2\" for=\"startup\">Default Power:</label> <div class=\"col-sm-4\"> <select class=\"form-control\" name=\"startup\" id=\"startup\"> <option value=\"0\">Last State</option> <option value=\"1\">On</option> <option value=\"2\">Off</option> </select> </div></div><div class=\"form-group row\"> <label class=\"control-label col-sm-2\" for=\"scene\">Default Scene:</label> <div class=\"col-sm-4\"> <select class=\"form-control\" name=\"scene\" id=\"scene\"> < <option value=\"0\">Relax</option> <option value=\"1\">Read</option> <option value=\"2\">Concentrate</option> <option value=\"3\">Energize</option> <option value=\"4\">Bright</option> <option value=\"5\">Dimmed</option> </select> </div></div><div class=\"form-group row\"> <label for=\"colorstate\" class=\"col-sm-2 col-form-label\">CCW Pin</label> <div class=\"col-sm-3\"> <input type=\"number\" class=\"form-control\" id=\"colorstate\" name=\"colorstate\" placeholder=\"\"> </div></div><div class=\"form-group row\"> <label for=\"brightstate\" class=\"col-sm-2 col-form-label\">Brightness Pin</label> <div class=\"col-sm-3\"> <input type=\"number\" class=\"form-control\" id=\"brightstate\" name=\"brightstate\" placeholder=\"\"> </div></div><div class=\"form-group row\"> <label class=\"control-label col-sm-2\" </div></div><div class=\"form-group row\"> <div class=\"col-sm-10\"> <button type=\"submit\" class=\"btn btn-primary\">Save</button> </div></div></form> </div><div class=\"tab-pane container fade\" id=\"menu2\"> <br><form method=\"POST\" action=\"/\"> <div class=\"form-group row\"> <label class=\"control-label col-sm-2\" for=\"dhcp\">DHCP:</label> <div class=\"col-sm-3\"> <select class=\"form-control\" name=\"dhcp\" id=\"dhcp\"> <option value=\"1\">On</option> <option value=\"0\">Off</option> </select> </div></div><div class=\"form-group row\"> <label for=\"addr\" class=\"col-sm-2 col-form-label\">Ip</label> <div class=\"col-sm-4\"> <input type=\"text\" class=\"form-control\" id=\"addr\" name=\"addr\"> </div></div><div class=\"form-group row\"> <label for=\"sm\" class=\"col-sm-2 col-form-label\">Submask</label> <div class=\"col-sm-4\"> <input type=\"text\" class=\"form-control\" id=\"sm\" name=\"sm\"> </div></div><div class=\"form-group row\"> <label for=\"gw\" class=\"col-sm-2 col-form-label\">Gateway</label> <div class=\"col-sm-4\"> <input type=\"text\" class=\"form-control\" id=\"gw\" name=\"gw\"> </div></div><div class=\"form-group row\"> <div class=\"col-sm-10\"> <button type=\"submit\" class=\"btn btn-primary\">Save</button> </div></div></form> </div></div><script src=\"https://diyhue.org/cdn/color.min.js\"></script> </body></html>";
    server.send(200, "text/html", htmlContent);
    if (server.args()) {
      delay(100);
      ESP.restart();
    }

  });
  
}