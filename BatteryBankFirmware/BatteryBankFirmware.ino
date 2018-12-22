#include <avr/interrupt.h>
#include <avr/sleep.h>

int vBatMeasurePin = A0;                           //pin for measuring the voltage of the battery
int currentSetPin = 6;                             //pin for setting the charging current
int wakeUpOutputtoUSBPin = 2;                      //interupt pin to enable USB output -> mapped to a button on the side
int wakeUpChargePin = 3;                           //interupt pin to begin charging the battery banks battery -> comes from plugging in USB Power Delivery
int batLevel = 0;
volatile boolean chargeStatus;                              
volatile boolean outputStatus;
float currentLevel; 
float vBat;

void setup() {
pinMode(currentSetPin, OUTPUT);
pinMode(vBatMeasurePin, INPUT);
pinMode(wakeUpOutputtoUSBPin, INPUT);
pinMode(wakeUpChargePin, INPUT);
Serial.begin(115200);
Serial.println("Initialization done");

}

void loop() {
if(chargeStatus == true){
  checkBattandCharge();
}
if(outputStatus == true){
  usbCharge();
}
  Serial.println("Going to Sleep");
delay(100);                                         //Give serial com. time to communicate
sleepNow();

} 

void checkBattandCharge(){
  vBat = analogRead(vBatMeasurePin) * .0049;        //converts the ADCs reading of the battery line to a human readable voltage
  while(vBat < 3.65){
    getBatLvl();
    if(batLevel = 3){
      //set charge current to a tenth
    }
    if(batLevel = 2){
        //set charge current to max
    }
    if(batLevel = 1){
      //set charge current to minimum
    }
  }
  chargeStatus = false;                             //ensures that the battery won't start charging and outputing after a repeated interrupt
  Serial.println("Charging Complete");
}

void usbCharge(){                           //output 5V on USB
 int i;
 Serial.println("Charging Phone");
 while(i > 0){
  indicateBatLvl();
  //close some switch to allow 5v output
 }  
 //open 5V switch
 outputStatus = false;
}

void sleepNow()                             // function to put arduino to sleep
{
    
    set_sleep_mode(SLEEP_MODE_PWR_DOWN);    // sleep mode is set here
    sleep_enable();                         // enables the sleep bit in the mcucr register
                                            // so sleep is possible. just a safety pin                            
    attachInterrupt(digitalPinToInterrupt(wakeUpOutputtoUSBPin), wakeUpOutput, RISING); // use interrupt 0 (pin 2) and run function
    attachInterrupt(digitalPinToInterrupt(wakeUpChargePin), wakeUpCharge, RISING);                            // wakeUpCharge when pin 2 gets LOW
    sleep_mode();                           // here the device is actually put to sleep!!
                                 
    sleep_disable();                        // first thing after waking from sleep: disable it
    detachInterrupt(0);                     // disables interrupt 0 on pin 2 so the
    detachInterrupt(1);                     // wakeUpNow code will not be executed
}

void wakeUpCharge(){
  detachInterrupt(0);                       //detach so that the battery bank doesn't charge and output
  chargeStatus = true;
}

void wakeUpOutput(){
  detachInterrupt(1);                       //detach so that the battery bank doesn't charge and output
  outputStatus = true;
}

int getBatLvl(){                                   //returns the batteries energy level
  vBat = analogRead(vBatMeasurePin) * .0049;        //converts the ADCs reading of the battery line to a human readable voltage
  if(vBat >= 3.600){
    batLevel = 3;
  }
  if(vBat < 3.6 && vBat > 2.5){
    batLevel = 2;
  }
  if(vBat < 2.5){
    batLevel = 1;
  }
  return batLevel;
}
void indicateBatLvl(){           //Lights up the fuel guage and set charge current
  
  int led25 = 10;                //Sets the pin number for fuel guage
  int led50 = 11; 
  int led75 = 9; 
  int led100 = 8;
  pinMode(led25, OUTPUT);
  pinMode(led50, OUTPUT);
  pinMode(led75, OUTPUT);
  pinMode(led100, OUTPUT);

  getBatLvl();
  if(batLevel = 3){
    digitalWrite(led100, HIGH);
    digitalWrite(led75, HIGH);
    digitalWrite(led50, HIGH);
    digitalWrite(led25, HIGH);
  }
  if(batLevel = 2){
    digitalWrite(led75, HIGH);
    digitalWrite(led50, HIGH);
    digitalWrite(led25, HIGH);
  }
  if(batLevel = 1){
    digitalWrite(led25, HIGH);
  }
   delay(2000);                                     //leaves the indicator on for 2 seconds
   digitalWrite(led100, LOW);
   digitalWrite(led75, LOW);
   digitalWrite(led50, LOW);
   digitalWrite(led25, LOW);
   Serial.println(vBat);                            
}




