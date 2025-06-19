#include<../src/task/FanRelayTask.h>
int flagTask = false;
void initTask()
{
    xTaskCreate(fanRelayTask, "fanRelayTask", 2048, NULL, 1, NULL);
}
void fanRelayTask(void *pvParameters)
{
    while (true)
    {
        if(flagTask){
            relayState = "ACTIVATE";
            fanState = "UNACTIVATE";
            flagTask = false;
        }else{
            relayState = "UNACTIVATE";
            fanState = "ACTIVATE";
            flagTask = true;
        }
        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
}