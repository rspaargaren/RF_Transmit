/*
 * RF_Transmit library v1.0.0 (20150217) made by Richard Spaargaren
 * 
 * License: GPLv3. See license.txt
 */

#ifndef RF_Transmit_h
#define RF_Transmit_h

#include <Arduino.h>


class RF_Transmit {
	public:
		RF_Transmit(byte pin, byte repeats = 4); //Constructor pin and number of repeats is defined
		
		void sendPIR1(int _unit, int _id, bool _state);
		void sendGeneric(int _unit, int _id, bool _posneg, int _num);
		void sendDimmer1(int _unit, int _id, bool _all, bool _state, int _dimlevel);
		void sendSwitch1(int _unit, int _id, bool _all, bool _state);
		void sendContact1(int _unit, int _id, bool _all, bool _contact);

	protected:
		byte _pin;		// Transmitter output pin
		byte _repeats;		// Number over repetitions of one telegram
		int _Bytes[50];		// Global array for the dataset to be sent
		int _start[2];		// Global array for the start puls
		int _one[4];		// Global array for the one puls
		int _zero[4];		// Global array for the zero puls
		int _close[2];		// Global array for the close puls

		//Internal routine for the actual signal sending
		void _sendHighLow (int Period_high, int Period_low); 
		//Internal routine for the creating of the puls arrays		
		void _init(int pulses[], int start[], int one[], int zero[], int close[], bool bit_puls); 
		//Internal routine for transfering the arrays of bits into pulses
		void _senddata(int bytesize, bool bit_puls, bool start_YN);
		//Internal routine for transfering a number into bits
		//Based on Yves Grange routine
		void _itob(unsigned long integer, int length, int start);
		//Internal helping routing for transfering a number into bits
		//Based on Yves Grange routine
		unsigned long _power2(int power);
};
#endif
