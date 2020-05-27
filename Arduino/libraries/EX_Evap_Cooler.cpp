//******************************************************************************************
//  File: EX_Evap_Cooler.cpp
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

#include "EX_Evap_Cooler.h"

#include "Constants.h"
#include "Everything.h"

namespace st
{
//private
	void EX_Evap_Cooler::setState()
	{
		if (m_nCurrentCoolState == cool_hi) {
			digitalWrite(m_nPinPump, m_bInvertLogic ? LOW : HIGH);	//turn pump on
			digitalWrite(m_nPinFanLo, m_bInvertLogic ? HIGH : LOW);	//turn fan lo off
			digitalWrite(m_nPinFanHi1, m_bInvertLogic ? LOW : HIGH);//turn fan hi on
			digitalWrite(m_nPinFanHi2, m_bInvertLogic ? LOW : HIGH);//turn fan hi on
		}
		else if(m_nCurrentCoolState == cool_lo) {
			digitalWrite(m_nPinPump, m_bInvertLogic ? LOW : HIGH);
			digitalWrite(m_nPinFanLo, m_bInvertLogic ? LOW : HIGH);
			digitalWrite(m_nPinFanHi1, m_bInvertLogic ? HIGH : LOW);
			digitalWrite(m_nPinFanHi2, m_bInvertLogic ? HIGH : LOW);
		}
		else if(m_nCurrentCoolState == fan) {
			digitalWrite(m_nPinPump, m_bInvertLogic ? LOW : HIGH);
			digitalWrite(m_nPinFanLo, m_bInvertLogic ? HIGH : LOW);
			digitalWrite(m_nPinFanHi1, m_bInvertLogic ? HIGH : LOW);
			digitalWrite(m_nPinFanHi2, m_bInvertLogic ? HIGH : LOW);
		}
		else {			//off state : m_nCurrentCoolState == fan
			digitalWrite(m_nPinPump, m_bInvertLogic ? HIGH : LOW);
			digitalWrite(m_nPinFanLo, m_bInvertLogic ? HIGH : LOW);
			digitalWrite(m_nPinFanHi1, m_bInvertLogic ? HIGH : LOW);
			digitalWrite(m_nPinFanHi2, m_bInvertLogic ? HIGH : LOW);
		}
	}

//public
	//constructor
	EX_Evap_Cooler::EX_Evap_Cooler(const __FlashStringHelper *name,  byte PinPump, byte PinFanLo, byte PinFanHi1, byte PinFanHi2, bool invertLogic) :
		Executor(name),
		m_bInvertLogic(invertLogic),
		m_nPinPump(PinPump),
		m_nPinFanLo(PinFanLo),
		m_nPinFanHi1(PinFanHi1),
		m_nPinFanHi2(PinFanHi2)
		{
			//set pin mode
			pinMode(m_nPinPump, OUTPUT);
			pinMode(m_nPinFanLo, OUTPUT);
			pinMode(m_nPinFanHi1, OUTPUT);
			pinMode(m_nPinFanHi2, OUTPUT);
			//update the digital outputs
			m_nCurrentCoolState = off;
			setState();
		}
	
	//destructor
	EX_Evap_Cooler::~EX_Evap_Cooler()
	{
	}
	
	void EX_Evap_Cooler::init()
	{
		refresh();
	}

	void EX_Evap_Cooler::beSmart(const String &str)
	{
		String s = str.substring(str.indexOf(' ') + 1);

		if (debug) {
			Serial.print(F("EX_Evap_Cooler::beSmart s = "));
			Serial.println(s);
		}

		if (s == F("cool_hi")) {
			m_nCurrentCoolState=cool_hi;
		}
		else if(s == F("cool_lo")) {
			m_nCurrentCoolState=cool_lo;
		}
		else if(s == F("fan")) {
			m_nCurrentCoolState=fan;
		}
		else if(s == F("off")) {
			m_nCurrentCoolState = off;
		}

		setState();

		refresh();
		
	}

	//called periodically by Everything class to ensure ST Cloud is kept consistent with the state of the contact sensor
	void EX_Evap_Cooler::refresh()
	{
		//Queue the relay status update the ST Cloud
		if (m_nCurrentCoolState == cool_hi) {
			Everything::sendSmartString(getName() + F(" cool_hi"));
		}
		else if(m_nCurrentCoolState == cool_lo) {
			Everything::sendSmartString(getName() + F(" cool_lo"));
		}
		else if(m_nCurrentCoolState == fan) {
			Everything::sendSmartString(getName() + F(" fan"));
		}
		else if(m_nCurrentCoolState == off) {
		//else {
			Everything::sendSmartString(getName() + F(" off"));
		}
	}

}
