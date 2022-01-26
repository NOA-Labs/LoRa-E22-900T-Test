#include <Arduino.h>
#include "app_lora_task.h"
#include "joystick_task.h"
#include "system_cfg.h"

const char *s = "\r\n################################################\r\n";

void setup() {
    // put your setup code here, to run once:
    Serial.begin(115200);
    Serial.print(s);
    Serial.printf("# system version: %s #", (const char *)FW_BUILD_VERSION);
    Serial.print(s);
    app_lora_task_create();
    // app_joystick_task_create();
    // enableLoopWDT();
}

void loop() {
  // put your main code here, to run repeatedly:
    delay(100);
}