#include <RF_Transmit.h>
RF_Transmit verstuur(2,4);

void setup() {

  // put your setup code here, to run once:
}

void loop() {
  // put your main code here, to run repeatedly:

verstuur.sendPIR1(31,31);
verstuur.sendGeneric(31,15,true,111);
verstuur.sendGeneric(31,15,false,111);
verstuur.sendDimmer1(12,12,true,true,10);
verstuur.sendSwitch1(8,8,true,true);
verstuur.sendContact1(5,5,true,true);
delay(6000); 
}
