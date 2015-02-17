/*
 * RF_Transmit library v1.0.0 (20150217) made by Richard Spaargaren
 * See RF_Transmit.h for more details.
 *
 * License: GPLv3. See license.txt
 */

#include "RF_Transmit.h"

//Setup of the internal ARRAYS for global acces within this routine
int _Bytes[50];
int _start[2];
int _one[4];
int _zero[4];
int _close[2];

//Constructor of the routine
RF_Transmit::RF_Transmit(byte pin, byte repeats) {
  _pin = pin; // The pin to which the 433 Mhz transmittor is connected
  _repeats = repeats; // The number of repeats the signal is submitted
  pinMode(_pin, OUTPUT); //Init the pin to which the transmittor is connected
} 

//The recepy for the SendPIR1 signal as defined in Rfcontroljs
void RF_Transmit::sendPIR1(int _unit,int _id,bool _state) {
  int PIR_LENGTH[4]={358,1095,11244};
  int PIR_START[2] = { 0, 0 }; // NOT USED FOR PIR
  int PIR_ONE[4]={0,1,0,1};
  int PIR_ZERO[4]={0,1,1,0};
  int PIR_CLOSE[2]={0,2};

  // Creating the bit string
  _itob(_unit,5,0); // 5 bits for the unit Max value 31
  _itob(_id,5,5); // 5 bits for the id Max value 31
  _Bytes[10] = 0; // 1 bit not used
  if (_state) { _Bytes[11] = 1; } // 1 bit for the state, this value is not used in RFcontroljs
	 else{ _Bytes[11] = 0; }
  
// Create global value for the puls arrays - if the 1 and 0 are 4 bits longs then the last value is true
  _init(PIR_LENGTH, PIR_START , PIR_ONE, PIR_ZERO, PIR_CLOSE, true); 
  // Sending the actual data via a sub-routine
  // The first argument is the length of the bit array + 1
  // The second argument is true when the 1 and 0 are 4 bits long
  // The thrid argument is true when a start puls is transmitted
_senddata(12, true, false);
}
// Switch protocol as used for KAKU
void RF_Transmit::sendSwitch1(int _unit, int _id, bool _all, bool _state) {
	 int PIR_LENGTH[4] = { 270, 1300, 2700, 10400 };
	 int PIR_START[2] = { 0, 2 };
	 int PIR_ONE[4] = { 0, 1, 0, 0 };
	 int PIR_ZERO[4] = { 0, 0, 0, 1 };
	 int PIR_CLOSE[2] = { 0, 3 };

	 _itob(_id, 26, 0); //25 bits for the id
	 if (_all) { _Bytes[26] = 1; } // boolean for the all status
	 else{ _Bytes[26] = 0; } 
	 if (_state) { _Bytes[27] = 1; } //boolean for the state status
	 else{ _Bytes[27] = 0; }
	 _itob(_unit, 4, 28); // 4 bits for the unit Max value is 15

	 _init(PIR_LENGTH, PIR_START, PIR_ONE, PIR_ZERO, PIR_CLOSE, true);
	 _senddata(32, true, true);
 }
// Contact protocal as used for KAKU - smilar to switch1
void RF_Transmit::sendContact1(int _unit, int _id, bool _all, bool _contact) {
	 int PIR_LENGTH[4] = { 270, 1300, 2700, 10400 };
	 int PIR_START[2] = { 0, 2 };
	 int PIR_ONE[4] = { 0, 1, 0, 0 };
	 int PIR_ZERO[4] = { 0, 0, 0, 1 };
	 int PIR_CLOSE[2] = { 0, 3 };

	 _itob(_id, 26, 0); //25 bits for the id
	 if (_all) { _Bytes[26] = 1; } //boolean for the all status
	 else{ _Bytes[26] = 0; }
	 if (_contact) { _Bytes[27] = 1; } //boolean for the contact
	 else{ _Bytes[27] = 0; }
	 _itob(_unit, 4, 28); // 4 bits for the unit max value 15
	 
	 _init(PIR_LENGTH, PIR_START, PIR_ONE, PIR_ZERO, PIR_CLOSE, true);
	 _senddata(32, true, true);
 }
// The protocol for a KAKU dimmer - Similar to switch1/contact1 now with additional dimlevel
void RF_Transmit::sendDimmer1(int _unit, int _id, bool _all, bool _state, int _dimlevel) {
	 int PIR_LENGTH[4] = { 270, 1300, 2700, 10400 };
	 int PIR_START[2] = { 0, 2 };
	 int PIR_ONE[4] = { 0, 1, 0, 0 };
	 int PIR_ZERO[4] = { 0, 0, 0, 1 };
	 int PIR_CLOSE[2] = { 0, 3 };

	 _itob(_id, 26, 0); //25 bits for the id
	 if (_all) { _Bytes[26] = 1; } //boolean for the all status
	 else{ _Bytes[26] = 0; }
	 if (_state) { _Bytes[27] = 1; } //boolean for the state status -> The N status 0000 is not implemented
	 else{ _Bytes[27] = 0; }
	 _itob(_unit, 4, 28); // 4 bits for the Unit max value 15
	 _itob(_dimlevel, 4, 32); // 4 bits for the Dim Level max value 15
	 
	 _init(PIR_LENGTH, PIR_START, PIR_ONE, PIR_ZERO, PIR_CLOSE, true);
	 _senddata(36, true, true);
 }
// The generic protocol as used in Homeduino / rfcontroljs
 void RF_Transmit::sendGeneric(int _unit, int _id, bool _posneg, int _num) {
	 int PIR_LENGTH[4] = { 671, 2049, 4346, 10208 };
	 int PIR_START[2] = { 0, 0 }; // NOT USED FOR GENERIC
	 int PIR_ONE[4] = { 0, 1, 0, 2 };
	 int PIR_ZERO[4] = { 0, 2, 0, 1 };
	 int PIR_CLOSE[2] = { 0, 3 };

	 _itob(_unit, 14, 0); //14 bits for the unit  
	 _itob(_id, 4, 14); // 4 bits for the id max value 15
	 if (_posneg) { _Bytes[18] = 1; } // if number is positiv the 1 else 0
	 else{ _Bytes[18] = 0; }
	 _itob(_num, 30, 19); // 30 bits for the actual value
	 

	 _init(PIR_LENGTH, PIR_START, PIR_ONE, PIR_ZERO, PIR_CLOSE, true);
	 _senddata(49, true, false);
 }

// Internal routine for the transmission of the byte array
 void RF_Transmit::_senddata(int bytesize,bool bit_puls, bool start_YN){
   for (int8_t i = 0; i <_repeats; i++) { //Repeat a number of times as defined
     if (start_YN) { _sendHighLow(_start[0], _start[1]); } // Submit a startstring if required
		 for (int8_t j = 0; j<bytesize; j++){ //sizeof(_Bytes) is given as an argument 
		   if (_Bytes[j] == 1) { // if bit is 1 the recepy one
				 _sendHighLow(_one[0], _one[1]);
				 if (bit_puls) { _sendHighLow(_one[2], _one[3]); } //if recepy is 4 pulses
			 }
			 else {
			   _sendHighLow(_zero[0], _zero[1]); // if bit is 0 the recepy zero
			   if (bit_puls) { _sendHighLow(_zero[2], _zero[3]); } // recepy is 4 pulses
			 }
		 }
		 _sendHighLow(_close[0], _close[1]); // Send the closing pulses
	 }
 }

// Internal routine for the actual transmisson of the data on the pin by pulling the pin HIGH and LOW based on
// the pulse length
void RF_Transmit::_sendHighLow(int Period_high, int Period_low) {
		digitalWrite(_pin, HIGH);
		delayMicroseconds(Period_high);
		digitalWrite(_pin, LOW);
		delayMicroseconds(Period_low);	
}

// Internal routine for creating the arrays for start,one,zero,close pulses
void RF_Transmit::_init(int pulses[], int start[], int one[], int zero[], int close[], bool bit_puls){
	_start[0] = pulses[start[0]];
	_start[1] = pulses[start[1]];
	_one[0] = pulses[one[0]];
	_one[1] = pulses[one[1]];
	_one[2] = 0;
	_one[3] = 0;
	_zero[0] = pulses[zero[0]];
	_zero[1] = pulses[zero[1]];
	_zero[2] = 0;
	_zero[3] = 0;
	if (bit_puls) {
		_one[2] = pulses[one[2]];
		_one[3] = pulses[one[3]];
		_zero[2] = pulses[zero[2]];
		_zero[3] = pulses[zero[3]];
		}
	_close[0] = pulses[close[0]];
	_close[1] = pulses[close[1]];
}

//Internal routine for transfering a number into bits
//Based on Yves Grange routine
void  RF_Transmit::_itob(unsigned long integer, int length, int start)
{
  int pos=0;  
 for (int i=0; i<length; i++){
   pos = i+start;
   if ((integer / _power2(length-1-i))==1){
     integer-=_power2(length-1-i);
     _Bytes[pos]=1;
   }
   else _Bytes[pos]=0;
 }
}

//Internal routine for transfering a number into bits
//Based on Yves Grange routine
unsigned long RF_Transmit::_power2(int power){    //gives 2 to the (power)
 unsigned long integer=1;          
 for (int i=0; i<power; i++){      
   integer*=2;
 }
 return integer;
}
