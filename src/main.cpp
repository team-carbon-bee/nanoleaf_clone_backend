#include <Arduino.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>

#include "tools/WiFiManager.h"
#include "AnimationFactory.h"
#include "Animator.h"
#include "ConfigurationProvider.h"
#include "ShapeHelper.h"
#include "PixelHelper.h"
#include "ledDriver/NeoPixelBusLedDriver.h"
#include "NetworkManager.h"
#include "tools/Logger.h"

TaskHandle_t xHandle = NULL;
void loop1(void* pvParameters);
void loop2(void* pvParameters);

void setup()
{
    Log.setup();
    Log.println("setup...");

    Configuration.setup();
    Configuration.loadFromFlash();

    Network.setup();

    GlobalAnimator.setup();

    Log.println("setup finished.");

    xTaskCreateUniversal(loop1, "loop1", 32768, NULL, 2, &xHandle, 1);
    xTaskCreateUniversal(loop2, "loop2", 32768, NULL, 3, &xHandle, 2);

    // Delete default Arduino task
    vTaskDelete(NULL);
}

void loop1(void* pvParameters)
{
    static unsigned long oldTick = 0;

    while (true)
    {
        const unsigned long currentTick = millis();

        if ((currentTick - oldTick) >= Configuration.parameters().speed)
        {
            GlobalAnimator.handle();
            oldTick = currentTick;
        }

        delay(5);
    }
}

void loop2(void* pvParameters)
{
    while (true)
    {
        Network.handle();

        delay(10);
    }
}

void loop() {}