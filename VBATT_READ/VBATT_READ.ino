#include <avr/interrupt.h>
#include <avr/sleep.h>

int vBatMeasurePin = A0;                           //pin for measuring the voltage of the battery
int currentSetPin = 6;                             //pin for setting the charging current
int wakeUpOutputtoUSBPin = 2;              //interupt pin to enable USB output -> mapped to a button on the side
int wakeUpChargePin = 3;                   //interupt pin to begin charging the battery banks battery
int interruptPin = 10;                     // LED to show the action of a interrupt
boolean chargeStatus;
boolean outputStatus;
float currentLevel; 
float vBat;
void setup() {
pinMode(currentSetPin, OUTPUT);
pinMode(vBatMeasurePin, INPUT);
pinMode(interruptPin, OUTPUT);
pinMode(wakeUpOutputtoUSBPin, INPUT);
pinMode(wakeUpChargePin, INPUT);
pinMode(13, OUTPUT);
Serial.begin(115200);
Serial.println("Initialization done");

}

void loop() {
if(chargeStatus == true){
  checkBattandSetChargeCurrent();
  Serial.println("Charging Complete");
}
if(outputStatus == true){
  usbCharge();
  outputStatus = false;
}
delay(1000);
digitalWrite(interruptPin, LOW);
delay(1000);
  Serial.println("Going to Sleep");
delay(100);
sleepNow();
  

} 

void checkBattandSetChargeCurrent(){
  vBat = analogRead(vBatMeasurePin) * .0049;        //converts the ADCs reading of the battery line to a human readable voltage
  if(vBat >= 3.600){                                //checks if the battery voltage is at 3.6V
    currentLevel = 0;
    analogWrite(currentSetPin, currentLevel);       //sets the output current of the charger to 0;
  }
  if(vBat < 3.600 && vBat > 2.500){
    currentLevel = 1/1024;                          //sets the output current of the charger to the maximum
    analogWrite(currentSetPin, currentLevel);
  }
  if(vBat < 2.500){
    currentLevel = 1/250;                           //set the output current to a quarter of the power
    analogWrite(currentSetPin, currentLevel);       
  }
  Serial.println(vBat);
  chargeStatus = false;
}

void usbCharge(){
 //open 5V usb OUTPUT
 Serial.println("Charging Phone");
}

void sleepNow()         // here we put the arduino to sleep
{
    
    set_sleep_mode(SLEEP_MODE_PWR_DOWN);   // sleep mode is set here
 
    sleep_enable();              // enables the sleep bit in the mcucr register
                                 // so sleep is possible. just a safety pin                            
    attachInterrupt(digitalPinToInterrupt(wakeUpOutputtoUSBPin), wakeUpOutput, LOW); // use interrupt 0 (pin 2) and run function
    attachInterrupt(digitalPinToInterrupt(wakeUpChargePin), wakeUpCharge, LOW);                            // wakeUpNow when pin 2 gets LOW
    sleep_mode();                // here the device is actually put to sleep!!
                                 //
    sleep_disable();             // first thing after waking from sleep:
                                 // disable sleep...
    detachInterrupt(0);          // disables interrupt 0 on pin 2 so the
    detachInterrupt(1);                             // wakeUpNow code will not be executed
}

void wakeUpCharge(){
  detachInterrupt(0);
  chargeStatus = true;
  digitalWrite(interruptPin, HIGH);
}

void wakeUpOutput(){
  detachInterrupt(1);
  outputStatus = true;
  digitalWrite(interruptPin, HIGH);
}

