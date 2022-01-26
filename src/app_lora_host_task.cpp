#include "system_cfg.h"

#if (SYS_LORA_IS_HOST_MODE)

#include "app_lora_task.h"
#include "esp32-hal-timer.h"

#define LORA_SEND_TASK_STACK_SIZE                   2048
#define LORA_SEND_TASK_PRIORITY                     6
#define LORA_REV_TASK_STACK_SIZE                    2048
#define LORA_REV_TASK_PRIORITY                      6
#if (LORA_E22_TEST_MODE == 0)
hw_timer_t      *hw_timer_lora_interval = NULL;
#endif
TaskHandle_t    lora_send_task_handle_t = NULL;
TaskHandle_t    lora_rev_task_handle_t  = NULL;
SemaphoreHandle_t xSemaphoreLoraSend    = NULL;
SemaphoreHandle_t xSemaphoreLoraRev     = NULL;
SemaphoreHandle_t xMutexSetSendData     = NULL;
class appLora *appLoRa;
uint8_t pin_sta = LOW;
uint8_t lora_sta = LORA_STA_IDLE;

#if (LORA_E22_TEST_MODE == 0)
void IRAM_ATTR lora_interval_timer_cb_func(void)
{
    static BaseType_t xHigherPriorityTaskWoken = pdFALSE;
    xSemaphoreGiveFromISR(xSemaphoreLoraSend, &xHigherPriorityTaskWoken);
    if( xHigherPriorityTaskWoken != pdFALSE )
    {
        portYIELD_FROM_ISR();
    }
}
#endif

void IRAM_ATTR aux_pin_interrupt_cb_func(void)
{
    static BaseType_t xHigherPriorityTaskWoken = pdFALSE;
    switch(lora_sta){
        case LORA_STA_SEND:
            lora_sta = LORA_STA_RECIEVE;
            // DBG_PRINTLN("lora send finish.");
        break;
        case LORA_STA_RECIEVE:
            // DBG_PRINTLN("lora recieve finish.");
            xSemaphoreGiveFromISR(xSemaphoreLoraRev, &xHigherPriorityTaskWoken);
            if( xHigherPriorityTaskWoken != pdFALSE )
            {
                portYIELD_FROM_ISR();
            }
        break;
        default: break;
    }
}
/**
 *****************************************************************************************************************
 * 
 ***************************************************************************************************************** 
 */
#if (LORA_E22_TEST_MODE)
const char *p[101] = {
    "00000000000000000000",
    "00000000000000000001",
    "00000000000000000002",
    "00000000000000000003",
    "00000000000000000004",
    "00000000000000000005",
    "00000000000000000006",
    "00000000000000000007",
    "00000000000000000008",
    "00000000000000000009",
    "00000000000000000010",
    "00000000000000000011",
    "00000000000000000012",
    "00000000000000000013",
    "00000000000000000014",
    "00000000000000000015",
    "00000000000000000016",
    "00000000000000000017",
    "00000000000000000018",
    "00000000000000000019",
    "00000000000000000020",
    "00000000000000000021",
    "00000000000000000022",
    "00000000000000000023",
    "00000000000000000024",
    "00000000000000000025",
    "00000000000000000026",
    "00000000000000000027",
    "00000000000000000028",
    "00000000000000000029",
    "00000000000000000030",
    "00000000000000000031",
    "00000000000000000032",
    "00000000000000000033",
    "00000000000000000034",
    "00000000000000000035",
    "00000000000000000036",
    "00000000000000000037",
    "00000000000000000038",
    "00000000000000000039",
    "00000000000000000040",
    "00000000000000000041",
    "00000000000000000042",
    "00000000000000000043",
    "00000000000000000044",
    "00000000000000000045",
    "00000000000000000046",
    "00000000000000000047",
    "00000000000000000048",
    "00000000000000000049",
    "00000000000000000050",
    "00000000000000000051",
    "00000000000000000052",
    "00000000000000000053",
    "00000000000000000054",
    "00000000000000000055",
    "00000000000000000056",
    "00000000000000000057",
    "00000000000000000058",
    "00000000000000000059",
    "00000000000000000060",
    "00000000000000000061",
    "00000000000000000062",
    "00000000000000000063",
    "00000000000000000064",
    "00000000000000000065",
    "00000000000000000066",
    "00000000000000000067",
    "00000000000000000068",
    "00000000000000000069",
    "00000000000000000070",
    "00000000000000000071",
    "00000000000000000072",
    "00000000000000000073",
    "00000000000000000074",
    "00000000000000000075",
    "00000000000000000076",
    "00000000000000000077",
    "00000000000000000078",
    "00000000000000000079",
    "00000000000000000080",
    "00000000000000000081",
    "00000000000000000082",
    "00000000000000000083",
    "00000000000000000084",
    "00000000000000000085",
    "00000000000000000086",
    "00000000000000000087",
    "00000000000000000088",
    "00000000000000000089",
    "00000000000000000090",
    "00000000000000000091",
    "00000000000000000092",
    "00000000000000000093",
    "00000000000000000094",
    "00000000000000000095",
    "00000000000000000096",
    "00000000000000000097",
    "00000000000000000098",
    "00000000000000000099",
    "00000000000000000100"
};
#endif
static void app_lora_send_task_entry(void *param)
{
    DBG_PRINTLN("app lora task entry");
    uint32_t send_count = 0;

     xSemaphoreLoraSend = xSemaphoreCreateBinary();
    if(xSemaphoreLoraSend == NULL){
        while(1);
    }

    xMutexSetSendData = xSemaphoreCreateMutex();
    if(xMutexSetSendData == NULL){
        while(1);
    }
    // xSemaphoreGive(xMutexSetSendData);

    pinMode(17, OUTPUT);
	digitalWrite(17, HIGH);

    appLoRa = new appLora();
    appLoRa->setMode(0);
    appLoRa->setLoraParam();
#if (LORA_E22_TEST_MODE == 0)  
    int interval = appLoRa->getInterval();

    hw_timer_lora_interval = timerBegin(2, 80, true);
    if(hw_timer_lora_interval != NULL){
        DBG_PRINTLN("start timer");
        timerAttachInterrupt(hw_timer_lora_interval, lora_interval_timer_cb_func, true);
        timerAlarmWrite(hw_timer_lora_interval, interval, true);
        timerAlarmEnable(hw_timer_lora_interval);
        // timerStop(hw_timer_lora_interval);
    }
#endif
    xSemaphoreGive(xSemaphoreLoraSend);
    // timerStart(hw_timer_lora_interval);
    while(1){
        #if (LORA_E22_TEST_MODE)
        xSemaphoreTake(xSemaphoreLoraSend, 1000);
        #else
        xSemaphoreTake(xSemaphoreLoraSend, portMAX_DELAY);
        #endif
        digitalWrite(17, pin_sta);
        pin_sta = (pin_sta == LOW) ? HIGH : LOW;
        #if (LORA_E22_TEST_MODE)
            app_lora_set_send_data((const char *)p[send_count], strlen(p[send_count]));
            DBG_PRINTF("send data: %s\r\n", p[send_count]);
            send_count ++;
            if(send_count == 100)send_count = 0;
        #endif
        xSemaphoreTake(xMutexSetSendData, 10);
        appLoRa->setLoraAuxPinAsInputMode();
        // appLoRa->sendDataOnce();
        appLoRa->sendData();
        delay(5);
        lora_sta = LORA_STA_SEND;
        appLoRa->setLoraAuxPinAsInterruptMode(aux_pin_interrupt_cb_func);
        xSemaphoreGive(xMutexSetSendData);
    }
}

static void app_lora_rev_task_entry(void *param)
{
     int length = 0;
    ResponseStructContainer receiveDataRssi;
    char recieve_buff[32] = {0};
    int rssi = 0;
    int slave_rssi = 0;

    xSemaphoreLoraRev =  xSemaphoreCreateBinary();
    if(xSemaphoreLoraRev == NULL){
        while(1);
    }

    while(1){
        if( xSemaphoreTake( xSemaphoreLoraRev, ( TickType_t ) LORA_RECEIVE_WAIT_TIMEOUT ) != pdTRUE ){
            appLoRa->cleanUARTBuffer();
            continue;
        }
        
        length = appLoRa->available();
        receiveDataRssi = appLoRa->receiveDataRssi(length);
        rssi = -(256 - receiveDataRssi.rssi);
        strncpy(recieve_buff, (const char *)receiveDataRssi.data, length);
        slave_rssi = recieve_buff[length - 1];
        recieve_buff[length] = 0;
        receiveDataRssi.close();
        DBG_PRINTF("receive data: %s\r\n",recieve_buff);
        DBG_PRINTF("recieve rssi is: %d\r\n", rssi);
        DBG_PRINTF("slave recieve rssi is: %d\r\n", -(256 -slave_rssi));

        #if (LORA_E22_TEST_MODE)
            xSemaphoreGive(xSemaphoreLoraSend);
        #endif
    }
}

int app_lora_task_create()
{
    xTaskCreateUniversal(app_lora_send_task_entry,
                        "lora send task",
                        LORA_SEND_TASK_STACK_SIZE,
                        NULL,
                        LORA_SEND_TASK_PRIORITY,
                        &lora_send_task_handle_t,
                        CONFIG_ARDUINO_RUNNING_CORE);

    if(lora_send_task_handle_t == NULL)return -1;

    xTaskCreateUniversal(app_lora_rev_task_entry,
                        "lora rev task",
                        LORA_REV_TASK_STACK_SIZE,
                        NULL,
                        LORA_REV_TASK_PRIORITY,
                        &lora_rev_task_handle_t,
                        CONFIG_ARDUINO_RUNNING_CORE);

    if(lora_rev_task_handle_t == NULL)return -1;

    DBG_PRINTLN("<app_lora.cpp>app lora task create.");
    return 0;
}

int app_lora_set_send_data(const void *buff, int size)
{
    xSemaphoreTake(xMutexSetSendData, 10);
    appLoRa->fillDataToSendBuffer((const byte *)buff, size);
    xSemaphoreGive(xMutexSetSendData);

    return 0;
}

#endif