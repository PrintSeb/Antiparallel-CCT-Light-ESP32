



void processLightdata() {

  
  step_time = (light.transitiontime)*10;
  ct_steptime = (light.transitiontime)*10;
  convert_bri();
  convert_ct();
  light.level = map(light.bri,0,255,0,100);
  
  if (light.lightState == false) {
    new_brightness = 0;
    br_update = true;
    update_ct = true;

  }
    Serial.print("brightness: ");     
    Serial.println(new_brightness);
    Serial.print("colortemp: ");     
    Serial.println(new_ct);
    Serial.print("light.transitiontime: ");     
    Serial.println((light.transitiontime)*10);


}

void convert_ct() {

//int warm_steps;
  //warm_steps = map(light.ct, 150, 500, 0, 255);
  //new_ct = map(light.ct, 0, 255, 0, 1023);
  new_ct = map(light.ct, 153, 500, 0, 1023);
  if (new_ct != old_ct) {
    if (new_ct > old_ct) {
      ct_steps = (ct_steptime * 10000) / (new_ct - old_ct);
    }
    else if (new_ct < old_ct) {
      ct_steps = (ct_steptime * 10000) / (old_ct - new_ct);
    }
    update_ct = true;
  }
}



void convert_bri() {

  new_brightness = map(light.bri, 0, 255, 80, 1023);
  if(new_brightness < 80){new_brightness=80;}
  if (new_brightness != old_brightness) {
    if (new_brightness > old_brightness) {
      bri_steps = (step_time * 10000) / (new_brightness - old_brightness);
    }
    else if (new_brightness < old_brightness) {
      bri_steps = (step_time * 10000) / (old_brightness - new_brightness);
    }
    br_update = true;
  }
}




void lightEngine()
{

  if (br_update == true)
  {
    brightness_update = old_brightness;

    unsigned long currentMicros = micros();
    if (currentMicros - previousMicros >= bri_steps) {
      previousMicros = currentMicros;

      if (brightness_update > new_brightness)
      {
        brightness_update --;
        old_brightness = brightness_update;
        ledcWrite(bri_ledChannel, brightness_update);
        //Serial.println(brightness_update);
      }
      else if (brightness_update < new_brightness)
      {
        brightness_update ++;
        old_brightness = brightness_update;
        ledcWrite(bri_ledChannel, brightness_update);
      }
    }
    else if (brightness_update == new_brightness)
    {
      br_update = false;
      old_brightness = new_brightness;
      saveState();
    }

  }

  
  if (update_ct == true)
  {
    ct_update = old_ct;

    unsigned long currentMicros_ct = micros();
    if (currentMicros_ct - previousMicros_ct >= ct_steps) {
      previousMicros_ct = currentMicros_ct;

      if (ct_update > new_ct)
      {
        ct_update --;
        old_ct = ct_update;
       ledcWrite(cct_ledChannel, ct_update);
        //Serial.println(ct_update);
      }
      else if (ct_update < new_ct)
      {
        ct_update ++;
        old_ct = ct_update;
        ledcWrite(cct_ledChannel, ct_update);
      }
    }
    else if (ct_update == new_ct)
    {
      update_ct = false;
      old_ct = new_ct;
      saveState();
    }
  }

}





void apply_scene(uint8_t new_scene) {
  if ( new_scene == 1) {
    light.bri = 254; light.ct = 346; convert_ct();
  } else if ( new_scene == 2) {
    light.bri = 254; light.ct = 233; convert_ct();
  }  else if ( new_scene == 3) {
    light.bri = 254; light.ct = 156; convert_ct();
  }  else if ( new_scene == 4) {
    light.bri = 77; light.ct = 367; convert_ct();
  }  else if ( new_scene == 5) {
    light.bri = 254; light.ct = 447; convert_ct();
  }  else {
    light.bri = 144; light.ct = 447; convert_ct();
  }
}
