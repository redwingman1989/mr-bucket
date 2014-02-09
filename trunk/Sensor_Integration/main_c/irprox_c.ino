/* */
void getIRDistanace() {
  unsigned long average;
  int i;
  
  irRawValue[icount % FILTER] = analogRead(IR_1_PIN);
  for (i=0; i < FILTER; i++) {
    average += irRawValue[i];
  }
  average = average/FILTER;
  ir1Voltage = average * .0048828125; //raw value * 5V/1024
  if (ir1Voltage >= 1.3 && ir1Voltage < 2.8) {
    ir1Distance = (ir1Voltage-4.3)/(-0.15);
  }
  else if (ir1Voltage >= 0.9 && ir1Voltage < 1.3) {
    ir1Distance = (ir1Voltage-2.1)/(-0.04);
  }
  else if (ir1Voltage >= 0.75 && ir1Voltage < 0.9) {
    ir1Distance = (ir1Voltage-1.35)/(-0.015);
  }
  else if (ir1Voltage >= 0.6 && ir1Voltage < 0.75) {
    ir1Distance = (ir1Voltage-1.35)/(-0.015);
  }
  else if (ir1Voltage >= 0.5 && ir1Voltage < 0.6) {
    ir1Distance = (ir1Voltage-1.1)/(-0.01);
  }
  else if (ir1Voltage >= 0.45 && ir1Voltage < 0.5) {
    ir1Distance = (ir1Voltage-0.8)/(-0.005);
  }
  else if (ir1Voltage >= 4.2 && ir1Voltage < 0.45) {
    ir1Distance = (ir1Voltage-0.66)/(-0.003);
  }
  else
    ir1Distance = 0;
    
  ir1Distance = ir1Distance/2.54;
}
  
void printDistance() {
  //sprintf(outStr, "% in.", 20.0);//ir1Distance);
 // Serial.println(ir1Voltage); 
}
