int vBusMeasurePin = A0;
int currentSetPin = 1; 
float currentLevel;
float vBusVoltage;
void setup() {
pinMode(currentSetPin, OUTPUT);
pinMode(vBusMeasurePin, INPUT);
}

void loop() {
  vBusVoltage = analogRead(vBusMeasurePin) * .0049; //converts the ADCs reading of the TYPE C VBUS line to a human readable voltage
  while(vBusVoltage >= 19.5){                       //checks if the VBUS voltage is at 20V
    analogWrite(currentSetPin, 1/1024);
  }

} 
