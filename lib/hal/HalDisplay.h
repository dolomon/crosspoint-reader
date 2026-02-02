// HalDisplay.h - Hardware Abstraction Layer for E-Paper Display
// Version: 2.0.0 - Updated for SSD1683 (400x300)
// Board: XteinkX4 custom hardware

#pragma once
#include <Arduino.h>
#include <EInkDisplay.h>

class HalDisplay {
 public:
  // Constructor with pin configuration
  HalDisplay();

  // Destructor
  ~HalDisplay();

  // Refresh modes
  enum RefreshMode {
    FULL_REFRESH,  // Full refresh with complete waveform (approx 3s)
    HALF_REFRESH,  // Half refresh (approx 1.7s) - balanced quality and speed
    FAST_REFRESH   // Fast refresh using custom LUT (approx 0.5s)
  };

  // Initialize the display hardware and driver
  void begin();

  // Display dimensions (400x300 for SSD1683)
  static constexpr uint16_t DISPLAY_WIDTH = EInkDisplay::DISPLAY_WIDTH;         // 400
  static constexpr uint16_t DISPLAY_HEIGHT = EInkDisplay::DISPLAY_HEIGHT;       // 300
  static constexpr uint16_t DISPLAY_WIDTH_BYTES = (DISPLAY_WIDTH + 7) / 8;     // 50

  // Buffer size must match actual display buffer allocation
  // GfxRenderer uses chunked allocation for its internal copy buffer, but BUFFER_SIZE
  // must match the actual framebuffer size to prevent buffer overruns
  static constexpr size_t RAW_BUFFER_SIZE = DISPLAY_WIDTH_BYTES * DISPLAY_HEIGHT;  // 15000
  static constexpr size_t BUFFER_SIZE = RAW_BUFFER_SIZE;  // MUST match EInkDisplay allocation
  static constexpr size_t BW_BUFFER_CHUNK_SIZE = 8000;  // For GfxRenderer internal chunking only

  // Frame buffer operations
  void clearScreen(uint8_t color = 0xFF) const;
  void drawImage(const uint8_t* imageData, uint16_t x, uint16_t y, uint16_t w, uint16_t h,
                 bool fromProgmem = false) const;

  void displayBuffer(RefreshMode mode = RefreshMode::FAST_REFRESH);
  void refreshDisplay(RefreshMode mode = RefreshMode::FAST_REFRESH, bool turnOffScreen = false);

  // Power management
  void deepSleep();

  // Access to frame buffer
  uint8_t* getFrameBuffer() const;

  // Grayscale support
  void copyGrayscaleBuffers(const uint8_t* lsbBuffer, const uint8_t* msbBuffer);
  void copyGrayscaleLsbBuffers(const uint8_t* lsbBuffer);
  void copyGrayscaleMsbBuffers(const uint8_t* msbBuffer);
  void cleanupGrayscaleBuffers(const uint8_t* bwBuffer);

  void displayGrayBuffer();

 private:
  EInkDisplay einkDisplay;
};