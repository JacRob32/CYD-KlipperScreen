#include <Arduino.h>
#include <lvgl.h>
#include "config.h"
#include "hardware/display_driver.h"
#include "ui/ui_manager.h"

// Global instances
DisplayDriver displayDriver;
UIManager* g_uiManager = nullptr;

// LVGL tick increment timer
static uint32_t lastTick = 0;

void setup() {
    // Initialize serial for debugging
    Serial.begin(115200);
    delay(1000);
    
    Serial.println(F("\n============================="));
    Serial.println(F("CYD KlipperScreen Starting"));
    Serial.println(F("============================="));
    Serial.printf("Free heap: %d bytes\n", ESP.getFreeHeap());
    
    // Initialize display driver
    if (!displayDriver.init()) {
        Serial.println(F("[ERROR] Display driver initialization failed!"));
        while (1) delay(1000);
    }
    
    // Initialize UI manager
    g_uiManager = new UIManager();
    if (!g_uiManager->init()) {
        Serial.println(F("[ERROR] UI Manager initialization failed!"));
        while (1) delay(1000);
    }
    
    Serial.println(F("[SETUP] System ready"));
    Serial.printf("Free heap after setup: %d bytes\n", ESP.getFreeHeap());
}

void loop() {
    // Increment LVGL tick
    uint32_t currentTick = millis();
    if (currentTick - lastTick >= LVGL_TICK_PERIOD) {
        lv_tick_inc(currentTick - lastTick);
        lastTick = currentTick;
    }
    
    // Run LVGL tasks
    displayDriver.taskHandler();
    
    // Update UI manager
    if (g_uiManager) {
        g_uiManager->update();
    }
}
