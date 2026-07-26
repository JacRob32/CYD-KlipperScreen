#include "ui/ui_manager.h"
#include "screens/screen_home_v2.h"
#include "screens/screen_actions.h"
#include "screens/screen_extrude.h"
#include "screens/screen_temperature.h"
#include "screens/screen_print_status.h"
#include <Arduino.h>

UIManager::UIManager() 
    : currentScreen(nullptr)
    , currentScreenID(SCREEN_HOME)
    , historyIndex(-1) {
    
    // Initialize history arrays
    for (int i = 0; i < MAX_HISTORY; i++) {
        screenHistory[i] = nullptr;
        historyStack[i] = -1;
    }
}

bool UIManager::init() {
    Serial.println(F("[UIManager] Initializing..."));
    
    // Create and display initial screen
    navigateTo(SCREEN_HOME);
    
    Serial.println(F("[UIManager] Initialized successfully"));
    return true;
}

void UIManager::update() {
    if (currentScreen) {
        currentScreen->update();
    }
}

void UIManager::navigateTo(ScreenID screenID) {
    Serial.printf("[UIManager] Navigating to screen: %d\n", screenID);
    
    // Save current screen to history before navigation
    if (currentScreen && currentScreenID != screenID) {
        if (historyIndex < MAX_HISTORY - 1) {
            historyIndex++;
            screenHistory[historyIndex] = currentScreen;
            historyStack[historyIndex] = currentScreenID;
        } else {
            // History full, cleanup oldest
            cleanupScreen(screenHistory[0]);
            
            // Shift history down
            for (int i = 0; i < MAX_HISTORY - 1; i++) {
                screenHistory[i] = screenHistory[i + 1];
                historyStack[i] = historyStack[i + 1];
            }
            historyIndex = MAX_HISTORY - 2;
            screenHistory[historyIndex + 1] = currentScreen;
            historyStack[historyIndex + 1] = currentScreenID;
            historyIndex++;
        }
    }
    
    // Create new screen
    ScreenBase* newScreen = createScreen(screenID);
    
    if (!newScreen) {
        Serial.printf("[UIManager] ERROR: Failed to create screen %d\n", screenID);
        return;
    }
    
    // Cleanup current screen
    if (currentScreen) {
        currentScreen->onDeactivate();
        cleanupScreen(currentScreen);
    }
    
    // Activate new screen
    currentScreen = newScreen;
    currentScreenID = screenID;
    loadScreen(newScreen);
    currentScreen->onActivate();
    
    Serial.printf("[UIManager] Screen %d activated\n", screenID);
}

void UIManager::goBack() {
    if (historyIndex < 0) {
        Serial.println(F("[UIManager] No history to navigate back to"));
        return;
    }
    
    Serial.println(F("[UIManager] Navigating back"));
    
    // Cleanup current screen
    if (currentScreen) {
        currentScreen->onDeactivate();
        cleanupScreen(currentScreen);
    }
    
    // Restore previous screen from history
    currentScreen = screenHistory[historyIndex];
    currentScreenID = (ScreenID)historyStack[historyIndex];
    
    screenHistory[historyIndex] = nullptr;
    historyStack[historyIndex] = -1;
    historyIndex--;
    
    if (currentScreen) {
        loadScreen(currentScreen);
        currentScreen->onActivate();
        Serial.printf("[UIManager] Restored screen: %d\n", currentScreenID);
    }
}

ScreenID UIManager::getCurrentScreen() const {
    return currentScreenID;
}

ScreenBase* UIManager::createScreen(ScreenID screenID) {
    switch (screenID) {
        case SCREEN_HOME:
            return new ScreenHomeV2();
        
        case SCREEN_ACTIONS:
            return new ScreenActions();
        
        case SCREEN_EXTRUDE:
            return new ScreenExtrude();
        
        case SCREEN_TEMPERATURE:
            return new ScreenTemperature();
        
        case SCREEN_PRINT_STATUS:
            return new ScreenPrintStatus();
        
        case SCREEN_MOVE:
            Serial.println(F("[UIManager] WARNING: ScreenMove not yet implemented"));
            return nullptr;
        
        case SCREEN_SETTINGS:
            Serial.println(F("[UIManager] WARNING: ScreenSettings not yet implemented"));
            return nullptr;
        
        default:
            Serial.printf("[UIManager] ERROR: Unknown screen ID: %d\n", screenID);
            return nullptr;
    }
}

void UIManager::cleanupScreen(ScreenBase* oldScreen) {
    if (oldScreen) {
        // LVGL will handle cleanup of screen object and children
        // Just need to delete our wrapper
        delete oldScreen;
    }
}

void UIManager::loadScreen(ScreenBase* newScreen) {
    if (!newScreen) return;
    
    // Load the screen's LVGL objects
    newScreen->load();
    
    // Set as active screen
    lv_scr_load(newScreen->getScreen());
}
