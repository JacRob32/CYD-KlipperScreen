#ifndef UI_MANAGER_H
#define UI_MANAGER_H

#include <Arduino.h>
#include <lvgl.h>
#include "../config.h"
#include "screens/screen_base.h"

class UIManager {
public:
    UIManager();
    
    bool init();
    void update();
    
    void navigateTo(ScreenID screenID);
    void goBack();
    ScreenID getCurrentScreen() const;
    
private:
    ScreenBase* currentScreen;
    ScreenID currentScreenID;
    
    // Simple history stack
    static const int MAX_HISTORY = 8;
    ScreenBase* screenHistory[MAX_HISTORY];
    int historyStack[MAX_HISTORY];
    int historyIndex;
    
    // Screen factory
    ScreenBase* createScreen(ScreenID screenID);
    
    // Lifecycle management
    void cleanupScreen(ScreenBase* oldScreen);
    void loadScreen(ScreenBase* newScreen);
};

#endif // UI_MANAGER_H
