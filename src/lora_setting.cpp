#include "lora_setting.h"

appLora::appLora()
{
///////////////////////////////////////////////////////////////////////////        
    LoRa = new LoRa_E22(LORA_SERIAL_TX_PIN, 
                        LORA_SERIAL_RX_PIN, 
                        &Serial2, 
                        LORA_AUX_PIN, 
                        LORA_M0_PIN, 
                        LORA_M1_PIN, 
                        UART_BPS_RATE_115200
                        );                           
    LoRa->begin();
    LoRa->setMode(mode); 

    configuration.ADDL = 0x00;
    configuration.ADDH = 0x00;
    configuration.NETID = 0x00;

    configuration.CHAN = 23;

    configuration.SPED.airDataRate = airDataRate;
    configuration.SPED.uartBaudRate = UART_BPS_115200;
    configuration.SPED.uartParity   = MODE_00_8N1;

    configuration.OPTION.subPacketSetting = subPacketSize;
    configuration.OPTION.RSSIAmbientNoise = RSSI_AMBIENT_NOISE_DISABLED;
    configuration.OPTION.transmissionPower = transmissionPower;

    configuration.TRANSMISSION_MODE.enableRSSI = enableRSSI;
    configuration.TRANSMISSION_MODE.fixedTransmission = FT_TRANSPARENT_TRANSMISSION;
    configuration.TRANSMISSION_MODE.enableRepeater = REPEATER_DISABLED;
    configuration.TRANSMISSION_MODE.enableLBT = LBT_DISABLED;
    configuration.TRANSMISSION_MODE.WORTransceiverControl = WOR_RECEIVER;
    configuration.TRANSMISSION_MODE.WORPeriod = WOR_2000_011;

    sendBuff = new byte[LORA_SEND_BUFF_SIZE];

    DEBUG_PRINTF("lora param: \r\n\
    address = %d\r\n\
    enableRSSI = %d\r\n\
    mode = %d\r\n\
    transmissionPower = %d\r\n\
    sendChannel = %d\r\n\
    netId = %d\r\n\
    airDataRate = %d\r\n\
    subPacketSize = %d\r\n\
    baudRate = %d\r\n\
    sendBuffAddr = %d\r\n",
                    address,
                    enableRSSI,
                    mode,
                    transmissionPower,
                    sendChannel,
                    netId,
                    airDataRate,
                    subPacketSize,
                    baudRate,
                    sendBuff);  
}

void appLora::fillDataToSendBuffer(const byte *d, int length)
{
    if(length > LORA_SEND_BUFF_SIZE){
        length = LORA_SEND_BUFF_SIZE;
    }
    memcpy(sendBuff, d, length);
    sendLength = length;
}
/**
 * @brief 
 * E22带掉电保存功能，设置成功后，下次上电时，直接按照设置的参数运行；
 */
void appLora::setLoraParam()
{
    configuration.ADDH = (address >> 8) & 0xff;
    configuration.ADDL = address & 0xff;
    configuration.CHAN = sendChannel;
    configuration.NETID = netId;
    configuration.SPED.airDataRate = airDataRate;
    configuration.OPTION.transmissionPower = transmissionPower;
    configuration.OPTION.subPacketSetting = subPacketSize;
    configuration.TRANSMISSION_MODE.enableRSSI = enableRSSI;

    LoRa->updateBaudRate(UART_BPS_RATE_9600);
    ResponseStatus sta = LoRa->setConfiguration(configuration, WRITE_CFG_PWR_DWN_SAVE);

    String strSta = sta.getResponseDescription();
    DEBUG_PRINTLN(strSta.c_str());

    LoRa->updateBaudRate(baudRate);
}

void appLora::sendData()
{
    DEBUG_PRINTLN("lora send data.");
    if(sendLength <= 0)return;
    LoRa->sendMessage((const byte *)sendBuff, sendLength);
}

int appLora::sendDataOnce()
{
    DEBUG_PRINTLN("lora send data.");
    if(sendLength <= 0)return -1;
    LoRa->sendMessage((const byte *)sendBuff, sendLength);
    sendLength = 0;
    return 0;
}
