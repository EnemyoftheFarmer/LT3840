#include "lifepo4Batt.h"
#include "Arduino.h"

int lifepo4Batt::getBattLvl(vBatMeasurePin)
{
  //returns the batteries energy level
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
