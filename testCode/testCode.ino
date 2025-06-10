// This code is AI Generated
#include <Adafruit_NeoPixel.h>

#define PIN        3       // Data pin connected to NeoPixels
#define NUMPIXELS  6       // Adjust this to match your actual pixel count

Adafruit_NeoPixel pixels(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

// Define a set of test colors
uint32_t colors[] = {
  pixels.Color(255, 0, 0),    // Red
  pixels.Color(0, 255, 0),    // Green
  pixels.Color(0, 0, 255),    // Blue
  pixels.Color(255, 255, 255) // White
};

void setup() {
  pixels.begin();       // Initialize NeoPixel strip
  pixels.show();        // Ensure all pixels start off
}

void loop() {
  for (int offset = 0; offset < 4; offset++) {
    for (int i = 0; i < NUMPIXELS; i++) {
      int colorIndex = (i + offset) % 4;
      pixels.setPixelColor(i, colors[colorIndex]);
    }
    pixels.show();
    delay(500);         // Delay between color shifts
  }
  }