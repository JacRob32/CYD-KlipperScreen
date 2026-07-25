# CYD KlipperScreen

A production-ready KlipperScreen interface for **ESP32-2432S028R (Cheap Yellow Display)** using **LVGL v8.3** and **PlatformIO**.

## 📋 Project Overview

This project translates the KlipperScreen Python UI into a modular C++ component optimized for ESP32 hardware constraints. It features:

- ✅ **Multi-screen navigation system** with history management
- ✅ **LVGL v8.3** with Grid and Flex layouts
- ✅ **Memory-efficient** screen lifecycle management
- ✅ **Modular architecture** - add screens incrementally
- ✅ **ESP32 CYD optimized** (240x320 ILI9341 + XPT2046 touch)

## 📁 Project Structure

```
./
├── platformio.ini                  # PlatformIO build configuration
├── README.md                       # This file
└── src/
    ├── main.cpp                    # Entry point
    ├── config.h                    # Pin definitions, colors, constants
    ├── lv_conf.h                   # LVGL v8 configuration
    ├── hardware/
    │   ├── display_driver.h        # Display driver header
    │   └── display_driver.cpp      # TFT_eSPI + LVGL integration
    └── ui/
        ├── ui_manager.h            # Screen navigation manager
        ├── ui_manager.cpp          # Navigation implementation
        └── screens/
            ├── screen_base.h       # Base screen class (shared utilities)
            ├── screen_base.cpp     # Helper methods for LVGL
            ├── screen_home.h       # Home screen header
            └── screen_home.cpp     # Home screen implementation
```

## 🚀 Quick Start

### 1. Install PlatformIO

```bash
# Install PlatformIO Core
pip install platformio

# Or use VS Code with PlatformIO extension
```

### 2. Build & Upload

```bash
# Build
pio run

# Upload to ESP32
pio run --target upload

# Monitor serial output
pio device monitor
```

### 3. First Boot

The display will show:
- **Statusbar** with connection status and time
- **Temperature display** (bed + extruder)
- **4 menu buttons**: Extrude, Move, Temp, Settings

## 🎯 Adding New Screens

### Step 1: Update `config.h`

Add your screen ID to the `ScreenID` enum:

```cpp
enum ScreenID {
    SCREEN_HOME = 0,
    SCREEN_EXTRUDE,
    SCREEN_MOVE,
    SCREEN_TEMPERATURE,
    SCREEN_SETTINGS,
    SCREEN_YOURNEW,  // ← Add here
    SCREEN_MAX
};
```

### Step 2: Create Screen Files

Create `src/ui/screens/screen_yournew.h`:

```cpp
#ifndef SCREEN_YOURNEW_H
#define SCREEN_YOURNEW_H

#include "screen_base.h"

class ScreenYourNew : public ScreenBase {
public:
    ScreenYourNew();
    void load() override;
    
private:
    void createLayout();
    // Add UI elements here
    
    static void onButtonClicked(lv_event_t* e);
};

#endif // SCREEN_YOURNEW_H
```

Create `src/ui/screens/screen_yournew.cpp`:

```cpp
#include "ui/screens/screen_yournew.h"
#include "ui/ui_manager.h"

extern UIManager* g_uiManager;

ScreenYourNew::ScreenYourNew() {
    setScreenName("Your New Screen");
}

void ScreenYourNew::load() {
    screen = lv_obj_create(nullptr);
    lv_obj_set_size(screen, DISPLAY_WIDTH, DISPLAY_HEIGHT);
    lv_obj_set_style_bg_color(screen, lv_color_hex(COLOR_BG_DARK), LV_PART_MAIN);
    
    createLayout();
}

void ScreenYourNew::createLayout() {
    // Your LVGL layout code here
    // Example:
    lv_obj_t* btn = createButton(screen, "Click Me", 150, 50, 45, 120);
    applyButtonStyle(btn, lv_color_hex(COLOR_PRIMARY));
    lv_obj_add_event_cb(btn, onButtonClicked, LV_EVENT_CLICKED, NULL);
}

void ScreenYourNew::onButtonClicked(lv_event_t* e) {
    Serial.println(F("[ScreenYourNew] Button clicked!"));
}
```

### Step 3: Register Screen in UI Manager

Edit `ui/ui_manager.cpp`:

```cpp
#include "ui/screens/screen_yournew.h"  // ← Add include

ScreenBase* UIManager::createScreen(ScreenID screenID) {
    switch (screenID) {
        case SCREEN_HOME:
            return new ScreenHome();
        
        case SCREEN_YOURNEW:
            return new ScreenYourNew();  // ← Add case
        
        // ... other cases
        
        default:
            return nullptr;
    }
}
```

### Step 4: Add Navigation Trigger

In any existing screen, add navigation:

```cpp
void ScreenHome::onSomeButtonClicked(lv_event_t* e) {
    Serial.println(F("Navigating to new screen"));
    if (g_uiManager) {
        g_uiManager->navigateTo(SCREEN_YOURNEW);
    }
}
```

## 🎨 Styling Guide

### Color Scheme (KlipperScreen Z-Bolt Dark)

```cpp
#define COLOR_BG_DARK     0x1A1A1A  // Main background
#define COLOR_BG_MEDIUM   0x2D2D2D  // Containers
#define COLOR_BG_LIGHT    0x3D3D3D  // Buttons
#define COLOR_TEXT_LIGHT  0xE0E0E0  // Text

// Button colors
#define BTN_COLOR_1 0xD32F2F  // Red (cancel/stop)
#define BTN_COLOR_2 0x388E3C  // Green (go/start)
#define BTN_COLOR_3 0x1976D2  // Blue (info)
#define BTN_COLOR_4 0xF57C00  // Orange (warning)
```

### Using Grid Layouts

```cpp
// 3x3 grid with 10px gaps
lv_obj_set_layout(container, LV_LAYOUT_GRID);
static lv_coord_t col_dsc[] = {70, 70, 70, LV_GRID_TEMPLATE_LAST};
static lv_coord_t row_dsc[] = {50, 50, 50, LV_GRID_TEMPLATE_LAST};
lv_obj_set_grid_dsc_array(container, col_dsc, row_dsc);

// Position button in grid
lv_obj_t* btn = createButton(container, "Text", 70, 50);
lv_obj_set_grid_cell(btn, LV_GRID_ALIGN_CENTER, 1, 1, LV_GRID_ALIGN_CENTER, 1, 1);
```

## 🔧 Hardware Configuration

### ESP32-2432S028R Pins

```
TFT_CS   = 15
TFT_DC   = 2
TFT_MOSI = 13
TFT_MISO = 12
TFT_SCLK = 14
TFT_BL   = 21 (backlight)
TOUCH_CS = 33
TOUCH_IRQ = 36
```

### Display Settings

- **Resolution**: 240x320 (portrait)
- **Driver**: ILI9341
- **Touch**: XPT2046
- **SPI Speed**: 40MHz

## 🐛 Debugging

### Serial Monitor

```bash
pio device monitor --baud 115200
```

You'll see debug messages like:
```
[ScreenHome] Loading...
[ScreenHome] Loaded successfully
[UIManager] Navigating to screen: 1
[ScreenExtrude] Button clicked!
```

### Common Issues

| Issue | Solution |
|-------|----------|
| Display shows white screen | Check TFT_BL pin (21) and backlight ON setting |
| Touch not responding | Verify TOUCH_CS (33) and XPT2046 library installed |
| Out of memory | Reduce `LV_MEM_SIZE` in `lv_conf.h` or simplify screens |
| Flickering | Increase display buffer size in `display_driver.cpp` |

## 📝 Development Workflow

1. **Analyze** KlipperScreen Python panel (structure, buttons, layout)
2. **Design** C++ screen class with LVGL Grid/Flex layouts
3. **Implement** with placeholder callbacks (Serial debug messages)
4. **Test** on hardware, verify memory usage
5. **Iterate** on styling and layout scaling

### Memory Budget (ESP32)

```
Total Heap: ~200KB
LVGL Buffer: ~20KB (2x 10KB buffers)
LVGL Internal: ~48KB
Free for App: ~132KB
```

**Keep screens lightweight!** Avoid creating unnecessary objects.

## 🎯 Next Steps

When you share screenshots of KlipperScreen panels, I'll generate:
- Pixel-perfect LVGL grid layouts
- Color-matched button styles
- Properly scaled fonts and spacing
- Navigation callbacks with debug output

## 📄 License

This project is a translation of KlipperScreen's UI concepts into C++ for ESP32. 
Original KlipperScreen: https://github.com/KlipperScreen/KlipperScreen

## 🙏 Credits

- **KlipperScreen Team** - Original Python UI inspiration
- **LVGL** - Graphics library
- **TFT_eSPI** - Display driver by Bodmer
- **PlatformIO** - Build framework

---

**Ready to build your screens!** Share your first KlipperScreen screenshot and I'll generate the exact C++ implementation.
