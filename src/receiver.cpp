#include "Arduino.h"
#include "LoRa_E32.h"

LoRa_E32 e32ttl(3, 4, 2, 6, 5); // TX, RX, AUX, M0, M1

void setup()
{
    Serial.begin(9600);
    delay(100);

    e32ttl.begin();
    e32ttl.setMode(MODE_2_POWER_SAVING);

    e32ttl.resetModule();
    ResponseStructContainer c;
    c = e32ttl.getConfiguration();
    Configuration configuration = *(Configuration *)c.data;
    configuration.ADDL = BROADCAST_ADDRESS;
    configuration.ADDH = BROADCAST_ADDRESS;
    configuration.CHAN = 0x19;

    configuration.OPTION.fec = FEC_1_ON;
    configuration.OPTION.ioDriveMode = IO_D_MODE_PUSH_PULLS_PULL_UPS;
    configuration.OPTION.transmissionPower = POWER_20;
    configuration.OPTION.wirelessWakeupTime = WAKE_UP_250;

    configuration.SPED.airDataRate = AIR_DATA_RATE_000_03;
    configuration.SPED.uartBaudRate = UART_BPS_9600;
    configuration.SPED.uartParity = MODE_00_8N1;

    configuration.OPTION.fixedTransmission = FT_FIXED_TRANSMISSION;
    e32ttl.setConfiguration(configuration, WRITE_CFG_PWR_DWN_SAVE);
    c.close();
}

struct Data
{
    float co;
    float no2;
};

void loop()
{
    if (e32ttl.available() > 1)
    {
        ResponseStructContainer rsc = e32ttl.receiveMessage(sizeof(Data));
        struct Data data = *(Data *)rsc.data;

        Serial.print("CO : ");
        Serial.print(data.co);
        Serial.print(" ppm || ");
        Serial.print("NO2 : ");
        Serial.print(data.no2);
        Serial.println(" ppm");
        free(rsc.data);
    }
}