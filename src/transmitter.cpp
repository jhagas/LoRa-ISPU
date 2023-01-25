#include "Arduino.h"
#include "LoRa_E32.h"
#include <SoftwareSerial.h>

#define PIN_CO A0
#define PIN_NO2 A2

SoftwareSerial mySerial(3, 4);		 // TX, RX
LoRa_E32 e32ttl(&mySerial, 2, 6, 5); // AUX, M0, M1

void printParameters(struct Configuration configuration);
void printModuleInformation(struct ModuleInformation moduleInformation);

void setup()
{
	Serial.begin(9600);
	delay(100);

	e32ttl.begin();
	e32ttl.setMode(MODE_1_WAKE_UP);

	ResponseStructContainer c;
	c = e32ttl.getConfiguration();
	Configuration configuration = *(Configuration *)c.data;
	configuration.ADDL = 0x01;
	configuration.ADDH = 0x00;
	configuration.CHAN = 0x04;

	configuration.OPTION.fec = FEC_1_ON;
	configuration.OPTION.ioDriveMode = IO_D_MODE_PUSH_PULLS_PULL_UPS;
	configuration.OPTION.transmissionPower = POWER_20;
	configuration.OPTION.wirelessWakeupTime = WAKE_UP_2000;

	configuration.SPED.airDataRate = AIR_DATA_RATE_000_03;
	configuration.SPED.uartBaudRate = UART_BPS_9600;
	configuration.SPED.uartParity = MODE_00_8N1;

	configuration.OPTION.fixedTransmission = FT_FIXED_TRANSMISSION;
	e32ttl.setConfiguration(configuration, WRITE_CFG_PWR_DWN_SAVE);
	printParameters(configuration);
	c.close();
}

void loop()
{
	delay(10000);

	struct Data
	{
		float co = map((float)analogRead(PIN_CO), 0, 1023, 1, 1000);
		float no2 = map((float)analogRead(PIN_NO2), 0, 1023, 0.05, 10);
	} data;

	Serial.println("Broadcast message to channel 19");
	ResponseStatus rs = e32ttl.sendBroadcastFixedMessage(0x19, &data, sizeof(Data));
	Serial.println(rs.getResponseDescription());
}

void printParameters(struct Configuration configuration)
{
	Serial.println("----------------------------------------");

	Serial.print(F("HEAD : "));
	Serial.print(configuration.HEAD, BIN);
	Serial.print(" ");
	Serial.print(configuration.HEAD, DEC);
	Serial.print(" ");
	Serial.println(configuration.HEAD, HEX);
	Serial.println(F(" "));
	Serial.print(F("AddH : "));
	Serial.println(configuration.ADDH, BIN);
	Serial.print(F("AddL : "));
	Serial.println(configuration.ADDL, BIN);
	Serial.print(F("Chan : "));
	Serial.print(configuration.CHAN, DEC);
	Serial.print(" -> ");
	Serial.println(configuration.getChannelDescription());
	Serial.println(F(" "));
	Serial.print(F("SpeedParityBit     : "));
	Serial.print(configuration.SPED.uartParity, BIN);
	Serial.print(" -> ");
	Serial.println(configuration.SPED.getUARTParityDescription());
	Serial.print(F("SpeedUARTDatte  : "));
	Serial.print(configuration.SPED.uartBaudRate, BIN);
	Serial.print(" -> ");
	Serial.println(configuration.SPED.getUARTBaudRate());
	Serial.print(F("SpeedAirDataRate   : "));
	Serial.print(configuration.SPED.airDataRate, BIN);
	Serial.print(" -> ");
	Serial.println(configuration.SPED.getAirDataRate());

	Serial.print(F("OptionTrans        : "));
	Serial.print(configuration.OPTION.fixedTransmission, BIN);
	Serial.print(" -> ");
	Serial.println(configuration.OPTION.getFixedTransmissionDescription());
	Serial.print(F("OptionPullup       : "));
	Serial.print(configuration.OPTION.ioDriveMode, BIN);
	Serial.print(" -> ");
	Serial.println(configuration.OPTION.getIODroveModeDescription());
	Serial.print(F("OptionWakeup       : "));
	Serial.print(configuration.OPTION.wirelessWakeupTime, BIN);
	Serial.print(" -> ");
	Serial.println(configuration.OPTION.getWirelessWakeUPTimeDescription());
	Serial.print(F("OptionFEC          : "));
	Serial.print(configuration.OPTION.fec, BIN);
	Serial.print(" -> ");
	Serial.println(configuration.OPTION.getFECDescription());
	Serial.print(F("OptionPower        : "));
	Serial.print(configuration.OPTION.transmissionPower, BIN);
	Serial.print(" -> ");
	Serial.println(configuration.OPTION.getTransmissionPowerDescription());

	Serial.println("----------------------------------------");
}
void printModuleInformation(struct ModuleInformation moduleInformation)
{
	Serial.println("----------------------------------------");
	Serial.print(F("HEAD BIN: "));
	Serial.print(moduleInformation.HEAD, BIN);
	Serial.print(" ");
	Serial.print(moduleInformation.HEAD, DEC);
	Serial.print(" ");
	Serial.println(moduleInformation.HEAD, HEX);

	Serial.print(F("Freq.: "));
	Serial.println(moduleInformation.frequency, HEX);
	Serial.print(F("Version  : "));
	Serial.println(moduleInformation.version, HEX);
	Serial.print(F("Features : "));
	Serial.println(moduleInformation.features, HEX);
	Serial.println("----------------------------------------");
}