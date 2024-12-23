#include <stdio.h>
#include <inttypes.h>
#include "sdkconfig.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_chip_info.h"
#include "esp_flash.h"
#include "driver/gpio.h"
#define NUMPINS 30

// Parameter structure for task
typedef struct {
  int pin;
  int highLength;
  int lowLength;
} TaskParams;

// Turns on and off a pin at a specified interval
void task(void * params) {
  TaskParams* p = (TaskParams*) params;
  //gpio_reset_pin(pin);
  gpio_set_direction(p->pin, GPIO_MODE_OUTPUT);
  for(;;){ // infinite loop
    gpio_set_level(p->pin, 1);
    vTaskDelay(p->highLength / portTICK_PERIOD_MS);
    gpio_set_level(p->pin, 0);
    vTaskDelay(p->lowLength / portTICK_PERIOD_MS);
    fflush(stdout);
    }
}
// Stores the task handles for the timers
TaskHandle_t taskHandles[NUMPINS] = {NULL};

void vTaskDelete( TaskHandle_t xTask );

// Takes in a pin number and destroys the timer associated with that pin
void destroyPinTimer(int pin) {
    printf("\ndestroying task\n");
    if (taskHandles[pin] != NULL) {vTaskDelete(taskHandles[pin]);}
}

// Takes in a pin number and two interval lengths and creates a timer
void createPinTimer(int pin, int highLength, int lowLength) {
  destroyPinTimer(pin);
  TaskParams* params = (TaskParams *) malloc(sizeof(TaskParams));
  params->pin = pin;
  params->highLength = highLength;
  params->lowLength = lowLength;
  //void (*t) (void*) = task;
  xTaskCreate(
    task,// Function that should be called
    "timer",   // Name of the task (for debugging)
    1000,            // Stack size (bytes)
    params,            // Parameter to pass
    1,              // Task priority
    &taskHandles[pin]            // Task handle
  );
}


void app_main(void)
{
  int pin1 = GPIO_NUM_25;
  int pin2 = GPIO_NUM_27;
  createPinTimer(pin1, 50, 950);
  vTaskDelay(2500 / portTICK_PERIOD_MS);
  createPinTimer(pin1, 50, 450);

}