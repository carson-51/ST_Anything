//******************************************************************************************
//  File: EX_Evap_Cooler.h
//  Authors: Dan G Ogorchock
//
//  Summary:  EX_Evap_Cooler is a class which implements the "Valve" device capability, where output1 opens a valve, and
//            output2 closes a valve.  It features optional automatic-turn-off time delay times for for both outputs.
//
//			  It inherits from the st::Executor class
//
//			  Create an instance of this class in your sketch's global variable section
//			  For Example:  st::EX_Evap_Cooler executor1(F("valve1"), PIN_RELAY1, PIN_RELAY2, LOW, true, 1000, 1000);
//
//			  st::EX_Evap_Cooler() constructor requires the following arguments
//				- String &name - REQUIRED - the name of the object - must match the Groovy ST_Anything DeviceType tile name
//				- byte pinOutput1 - REQUIRED - the Arduino Pin to be used as a digital output
//				- byte pinOutput2 - REQUIRED - the Arduino Pin to be used as a digital output
//				- bool startingState - REQUIRED - the value desired for the initial state of the switch.  LOW = "closed", HIGH = "open"
//				- bool invertLogic - REQUIRED - determines whether the Arduino Digital Output should use inverted logic (e.g. active high versus active low relays)
//				- long Output1Time - REQUIRED - the number of milliseconds to keep the output1 on, DEFAULTS to 1000 milliseconds, 0 = will stay on
//				- long Output2Time - REQUIRED - the number of milliseconds to keep the output2 on, DEFAULTS to 1000 milliseconds, 0 = will stay on
//
//  Change History:
//
//    Date        Who            What
//    ----        ---            ----
//    2019-10-30  Dan Ogorchock  Original Creation
//
//
//******************************************************************************************

#ifndef ST_EX_Evap_Cooler_H
#define ST_EX_Evap_Cooler_H

#include "Executor.h"

enum Evap_Cooler_States { off, fan, cool_lo, cool_hi};

namespace st
{
	class EX_Evap_Cooler : public Executor  //inherits from parent Executor Class
	{
		private:
			
			//following are for the digital output
			bool m_bInvertLogic;	        //determines whether the Arduino Digital Output should use inverted logic
			byte m_nPinPump;
			byte m_nPinFanLo;
			byte m_nPinFanHi1;
			byte m_nPinFanHi2;
			Evap_Cooler_States m_nCurrentCoolState;

			void setState();
			
		public:
			//constructor - called in your sketch's global variable declaration section
			EX_Evap_Cooler(const __FlashStringHelper *name, byte PinPump, byte PinFanLo, byte PinFanHi1, byte PinFanHi2, bool invertLogic = false);
			
			//destructor
			virtual ~EX_Evap_Cooler();
			
			//initialization function
			virtual void init();

			//SmartThings Shield data handler (receives command to turn "on" or "off" the switch (digital output)
			virtual void beSmart(const String &str);

			//called periodically by Everything class to ensure ST Cloud is kept consistent with the state of the contact sensor
			virtual void refresh();

			//gets
			//virtual bool getPumpPin() const { return m_nPinPump; }
			//virtual bool getFanLoPin() const { return m_nPinFanLo; }
			//virtual bool getFanHiPin1() const { return m_nPinFanHi1; }
			//virtual bool getFanHiPin2() const { return m_nPinFanHi2; }
			
			//sets
			//virtual void setOutputPin(byte pin);
	};
}


#endif
