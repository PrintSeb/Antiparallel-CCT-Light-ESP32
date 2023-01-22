


void IRAM_ATTR detect_touch() {
  Serial.println("TOUCH DETECTED!!!");
  touch=true;
  touch_end = false; 
  previousMillis_controller = millis();
  detachInterrupt(digitalPinToInterrupt(interruptor_pin));
  attachInterrupt(digitalPinToInterrupt(flow_pin), touch_loss, RISING);
}

void IRAM_ATTR touch_loss() {
  Serial.println("TOUCH LOSS!!!");
  touch = false;
  touch_end = true; 
  detachInterrupt(digitalPinToInterrupt(flow_pin));
  attachInterrupt(digitalPinToInterrupt(interruptor_pin), detect_touch, CHANGE);  
}

void controller_setup(){


  pinMode(interruptor_pin, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(interruptor_pin), detect_touch, CHANGE);
  pinMode(flow_pin, INPUT_PULLUP);
 
}

