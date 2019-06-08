#ifndef Lifepo4Batt_h
#define Lifepo4Batt_h

#include "Arduino.h"

class Lifepo4Batt
{
    public:
        int getBattLvl();
        void indicateBatLvl();
        int checkBattandCharge(int vBatMeasurePin);
        int vBat;
    private:
        int pinFull;
        int pinHalf;
        int pinEmpty;
};








#endif
