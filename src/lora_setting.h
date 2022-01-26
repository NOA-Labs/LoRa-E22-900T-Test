#pragma once

#include "LoRa_E22.h"

#define LORA_SEND_BUFF_SIZE                         64
#define LORA_RECEIVE_BUFFER_SIZE                    32

#define LORA_SERIAL_TX_PIN              26
#define LORA_SERIAL_RX_PIN              27
#define LORA_AUX_PIN                    25
#define LORA_M0_PIN                     12
#define LORA_M1_PIN                     14

/**
 * @brief  
 * 
 */

class appLora{
public:
    appLora();

    void setAddress(int addr){address = addr;}
    int  getAddress(){return address;}
    void setInterval(int val){sendInterval = val;}
    int  getInterval(){return sendInterval;}
    void setTransmissionPower(byte db) {transmissionPower = db;}
    byte getTransmissionPower(){return transmissionPower;}
    void setEnableRSSI(bool enable){enableRSSI = enable;}
    bool getEnableRSSI(){return enableRSSI;}
    void setMode(uint8_t m){mode = m;}
    uint8_t getMode(){return mode;}
    void setSendChannel(byte chan){sendChannel = chan;}
    byte getSendChannel(){return sendChannel;}
    void setSubPacketSize(uint8_t size){subPacketSize = size;}
    uint8_t getSubPacketSize(){return subPacketSize;}
    void setNetID(byte id){netId = id;}
    byte getNetID(){return netId;}
    void setAirDataRate(uint8_t rate){airDataRate = rate;}
    void setRssiValue(int val) {rssiVal = val;}
    int  getRssiValue(){return rssiVal;} 
    void fillDataToSendBuffer(const byte *d, int length);
    int  available(){return LoRa->available();}
    void setLoraParam();
    void sendData();
    int  sendDataOnce();
    size_t readBytes(char *buff, size_t size){LoRa->readBytes(buff, size);}
    void cleanUARTBuffer(){LoRa->cleanUARTBuffer();}
    ResponseStructContainer receiveDataRssi(uint8_t len){return LoRa->receiveMessageRSSI(len);}
    int  getLoraAuxPinSta(){return LoRa->getAuxPinSta();}
    void setLoraAuxPinAsInterruptMode(void (*cb)(void)){LoRa->setAuxPinAsInterruptMode(LORA_AUX_PIN, cb, RISING);}
    void setLoraAuxPinAsInputMode(){LoRa->setAuxPinAsInputMode(LORA_AUX_PIN);}
private:
    LoRa_E22 *LoRa;
    int  address = 1;
    int sendInterval = 80 * 1000;                  //ms default: 80ms.
    int rssiVal = -1;
    byte *sendBuff;
    int   sendLength = 0;

    UART_BPS_RATE baudRate = UART_BPS_RATE_115200;
    byte transmissionPower = POWER_22;
    bool enableRSSI = true;
    uint8_t mode = MODE_0_NORMAL;     
    uint8_t subPacketSize = SPS_032_11;                
    byte sendChannel = 80;                   //0 ~ 80.
    byte netId = 1;
    uint8_t airDataRate = AIR_DATA_RATE_110_384;

    struct Configuration configuration;
};
