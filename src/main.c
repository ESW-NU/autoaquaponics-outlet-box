#include <stdio.h>
#include <inttypes.h>
#include "sdkconfig.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_chip_info.h"
#include "esp_flash.h"
#include "driver/gpio.h"
#define NUMPINS 30

void task(void * params) {
  int* data = (int*) params;
  int pin = data[0];
  int hl = data[1];
  int ll = data[2];
  //gpio_reset_pin(pin);
  gpio_set_direction(pin, GPIO_MODE_OUTPUT);
  for(;;){ // infinite loop
    gpio_set_level(pin, 1);
    vTaskDelay(hl / portTICK_PERIOD_MS);
    gpio_set_level(pin, 0);
    vTaskDelay(ll / portTICK_PERIOD_MS);
    fflush(stdout);
    }
}
//TaskHandle_t taskHandles[NUMPINS];

void createPinTimer(int pin, int high_length, int low_length) {
  int params[3] = {pin, high_length, low_length};
  //TaskHandle_t xHandle = NULL;
  //taskHandles[pin] = xHandle;
  //void (*t) (void*) = task;
  xTaskCreate(
    task,// Function that should be called
    "timer",   // Name of the task (for debugging)
    1000,            // Stack size (bytes)
    params,            // Parameter to pass
    1,    
    NULL          // Task priority
    //&taskHandles[pin]            // Task handle
  );
}

void vTaskDelete( TaskHandle_t xTask );

void destroyPinTimer(int pin) {
    printf("\ndestroying task\n");
    //vTaskDelete(taskHandles[pin]);
}

void app_main(void)
{
  
  int pin1 = GPIO_NUM_26;
  int pin2 = GPIO_NUM_13;
  createPinTimer(pin1, 50, 950);
  vTaskDelay(5000 / portTICK_PERIOD_MS);
  createPinTimer(pin2, 50, 450);
  //vTaskDelay(10000 / portTICK_PERIOD_MS);
  //destroyPinTimer(pin1);
}