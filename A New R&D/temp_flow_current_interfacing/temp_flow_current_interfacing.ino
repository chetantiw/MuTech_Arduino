                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                              

#include "EmonLib.h"                  
EnergyMonitor emon1; 

#include <FlowMeter.h>  
FlowMeter *Meter;
const unsigned long period = 1000;

void MeterISR()
   {
    Meter->count();
   }
                 

float Vin=5;     // [V]        
float Rt=10000;    // Resistor t [ohm]
float R0=10000;    // value of rct in T0 [ohm]
float Tzero=298.15;   // use T0 in Kelvin [K]
float Vout=0.0;    // Vout in A0 
float Rout=0.0;    // Rout in A0
// use the datasheet to get this data.
float Tone=273.15;      // [K] in datasheet 0º C
float Ttwo=373.15;      // [K] in datasheet 100° C
float RT1=35563;   // [ohms]  resistence in T1
float RT2=549;    // [ohms]   resistence in T2
float beta=0.0;    // initial parameters [K]
float Rinf=0.0;    // initial parameters [ohm]   
float TempK=0.0;   // variable output
float TempC=0.0;   // variable output
#define temp_probe 33





void setup() {

   Serial.begin(9600);
   pinMode(33, INPUT);
   pinMode(22, OUTPUT);
   delay(200);
   Serial.println("temp_flow_current demo....");
 
   beta=(log(RT1/RT2))/((1/Tone)-(1/Ttwo));
   Rinf=R0*exp(-beta/Tzero);
   emon1.current(32, 350); 
   Meter = new FlowMeter(digitalPinToInterrupt(26), UncalibratedSensor, MeterISR, RISING); 
  
}

void loop()
{
    Vout=Vin*((float)(analogRead(temp_probe))/4096.0); // calc for ntc
    Rout=(Rt*Vout/(Vin-Vout));
    TempK=(beta/log(Rout/Rinf)); // calc for temperature
    TempC=TempK-273.15;
    double Irms = emon1.calcIrms(1480);
    Serial.println("Temperature: ");
    Serial.println(TempC);
    Serial.println("C  ");
    delay(500);
    Serial.println("Current: ");
    Serial.println(Irms);
    Serial.println("MA");
    delay(500);
    delay(period);
    Meter->tick(period);
    Serial.println("Currently " + String(Meter->getCurrentFlowrate()) + " l/min, " + String(Meter->getTotalVolume())+ " l total.");

    
}
