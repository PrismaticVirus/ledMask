# Sound Reactive Led Mask Tutorial
<img src="/images/IMG_2536.jpg" alt="Finished Product" width="300"/>

This cyberpunk inspired mask cover's purpose is to bring more fun to masking

> Audience: This turotial assumes basic-intermediate experience with Arduino projects and C++, and some familarity with sewing

## Materials Needed:
**Parts Required:**
 - [ ] [AdaFruit Flora](https://www.adafruit.com/product/659)
 - [ ] [Sewable Neopixels (as many as you want but 4 is a good number)](https://www.adafruit.com/product/1260)
 - [ ] [Microphone](https://www.adafruit.com/product/1063)
 - [ ] [Light Sensor (Optional)](https://www.sparkfun.com/lilypad-light-sensor.html)
 - [ ] Battery with JST Connector
 - [ ] Any Cloth Face Mask
 
 **Materials:**
 - [ ] Conductive Thread
 - [ ] Regular Thread 
 - [ ] Sewing Needles 
 - [ ] Fabric (Optional)
 - [ ] Hot Glue Gun (Optional)


 ## Part 1: Planning and Design
 
**Create a mock up of how you want the mask to look**
- Decide how many neopixels you will be using 
- Decide the arrangement of the components on the mask
	- You can do this by: drawing a sketch, using CAD software, or physically taping the components to the mask 
	- **DON'T FORGET ABOUT THE BATTERY**
  <img src="/images/IMG_2468.jpg" alt="Mockup" width="300"/>

## Part 2: Assembling the Mask
**Start by wiring the microphone**
- Using conductive thread sew the pins on the microphone to the corresponding pins on the Flora 
- VCC to +3.3v; GND to Ground; OUT to an analog input pin 
- The analog input pins on the AdaFruit Flora are: 6, 9, 10, and 12
- It may be beneficial to hot glue the connections for the microphone 
- Secure the microphone to the mask using the top holes and regular thread (or just hot glue the whole thing down like I did)
<img src="/images/IMG_2476.jpg" alt="Mic" width="300"/>
**Wire the light sensor (Optional)**
- This is very similar to above but the light sensor is going to go on the inside of the mask because we are using it for wear detection 
- The light sensor must also be connected to an analog input pin
<img src="/images/IMG_2479.jpg" alt="Light Sensor" width="300"/>

**Time for the NeoPixels**
- I would recommend using a low resistance thread although it may be harder to work with
- Connect the first data in pin to data any unused input pin on the Flora (e.g. 3)
	- the data in pins on the rest of the neopixels will be connected to the data out of the previous one 
- Connect Power and ground the same way
	- note: power and ground can be connected to any power/ground pin on the board
### Testing the Neopixels:
The Neo pixels can be somewhat finicky so here is some code to make sure they all work correctly 

```cpp 
//this code was generated by an LLM (GPT 4o)
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
  ```
  - If this it working correctly all of the LEDS will alternate between red green blue and white.
  - If the connections arent solid the LEDS will not work as intenteded
    - Some of them may not be lit
    - They could all be red (See Q&A)

## Programming the Flora:

This step reads sound levels from a microphone and lights up NeoPixels based on the volume. Follow the breakdown to understand and reproduce the code.

### Breakdown of the Code
*The file can be found under tutorialCode*

#### 1. Include Required Library

```cpp
#include <Adafruit_NeoPixel.h>
```

Brings in support for controlling NeoPixels.

#### 2. Define Pins and Constants

```cpp
#define MIC_PIN A9
#define LIGHT_SENSOR A7
#define LED_PIN 3
#define NUM_LEDS 6
```

-   `MIC_PIN`: Where your microphone is connected.
    
-   `LIGHT_SENSOR`: (Unused in this version; available for future use).
    
-   `LED_PIN`: Connected to the data input of the NeoPixel strip.
    
-   `NUM_LEDS`: How many LEDs you’re using.
    

#### 3. Initialize the NeoPixel Strip

```
Adafruit_NeoPixel mask(NUM_LEDS, LED_PIN, NEO_GRB + NEO_KHZ800);
```

- Creates the `mask` object to control your LED strip.

#### 4. Setup Function

```cpp
void setup() {
  pinMode(MIC_PIN, INPUT);
  Serial.begin(9600);
  mask.begin();
  mask.show();
}
```

-   Prepares the mic pin for reading.
    
-   Starts serial communication for debugging.
    
-   Initializes the LED strip and turns all LEDs off.
    

#### 5. Process Microphone Signal

```cpp
int micOutput(int pin) {
  int raw = analogRead(pin);
  int baseline = 512;
  float noiseFloor;
  float alpha = 0.01;
  float beta = 0.01;
  float threshold = 2;

  baseline = alpha * raw + (1 - alpha) * baseline;
  float acSignal = abs(raw - baseline);

  noiseFloor = beta * acSignal + (1 - beta) * noiseFloor;
  float output = (acSignal > threshold * noiseFloor) ? acSignal : 0.0;

  delay(10);
  return map(output, 0, 512, 0, 255);
}
```

-   Reads the raw audio signal.
    
-   Removes background noise using a dynamic threshold.
    
-   Returns a value mapped to a brightness scale (0–255).
    

#### 6. Control NeoPixel Lighting

```cpp
void lightEffect(int signal, int leds) {
  signal = constrain(signal, 0, 255);
  int ledsToLight = map(signal, 0, 255, 0, leds);

  for (int i = 0; i < NUM_LEDS; i++) {
    if (i < ledsToLight) {
      mask.setPixelColor(leds - 1 - i, mask.Color(255, 0, 255));
    } else {
      delay(20);
      mask.setPixelColor(leds - 1 - i, 0);
    }
  }
  mask.show();
}
```

-   Lights up a number of LEDs proportional to sound intensity.
    
-   Creates a visual volume meter effect in magenta.
    

#### 7. Main Loop

```cpp
void loop() {
  lightEffect(micOutput(MIC_PIN), NUM_LEDS);
}
```

-   Continuously reads the mic signal.
    
-   Updates the LEDs in real-time based on audio input.

#### Using the light sensor for wear detection
```cpp
void loop() {
  if (analogRead(LIGHT_SENSOR) <= 5){
    lightEffect(micOutput(MIC_PIN), NUM_LEDS);
  }
  else {
    mask.fill(mask.color(0,0,0));
  }
}
```
-   Replace the main loop code with this

### Summary of What It Does:

-   It reads audio levels from a microphone.
    
-   Filters out background noise.
    
-   Lights up a strip of NeoPixels in response to sound volume, with a magenta glow.
    
-   Continuously updates in real time.

## The fun part:
**Time to actually wear your creation**
plug in the battery and put it on and test it out in the mirror 

 ## Enhancements & Ways to Expand 
 - Bluetooth Control
 - Motion Effects
 - Color Cycling

 ### Changing the colors of the leds:
 - Theres lots of different effects you can do with the leds, heres a version of the lightEffect function that displays the colors of the rainbow 

 ```cpp
 void lightEffect(int signal, int leds) {
  signal = constrain(signal, 0, 512);

  int ledsToLight = map(signal, 0, 500, 0, leds);
  ledsToLight = constrain(ledsToLight, 0, leds);

  // Define color sequence: red, orange, yellow, green, blue, purple
  uint32_t colors[] = {
    mask.Color(255, 0, 0),     // Red
    mask.Color(255, 165, 0),   // Orange
    mask.Color(255, 255, 0),   // Yellow
    mask.Color(0, 255, 0),     // Green
    mask.Color(0, 0, 255),     // Blue
    mask.Color(128, 0, 128)    // Purple
  };
  int numColors = sizeof(colors) / sizeof(colors[0]);

  for (int i = 0; i < NUM_LEDS; i++) {
    if (i < ledsToLight) {
      int colorIndex = i % numColors;
      mask.setPixelColor(leds - 1 - i, colors[colorIndex]);
    } else {
      delay(20);
      mask.setPixelColor(leds - 1 - i, 0);  // Off
    }
  }
  mask.show();
}
```

## Debugging and Q&A:

### Why are all of the LEDS red?
This means the leds are getting insufficent/unstable voltage; the eaisest fix is lowering the brightness
```cpp 
strip.setBrightness(50); // (0-255) set it to as high as you want/can without impacting the color
```
add this inside the setup function

### Why aren’t the LEDs lighting up?

Use a multimeter to check continuity in your conductive thread.

Make sure VCC and GND are connected properly.

Confirm that you're using the correct data pin in your code.

### Why do only some LEDs work?

One bad NeoPixel or broken connection in the chain can stop the rest from lighting.

Double-check Data OUT to Data IN connections between pixels.

### How do I make it respond more to quiet sounds?

Adjust the threshold value in the code.

Consider using a more sensitive microphone.

### Can I use different colors or effects?

Yes! Play with `mask.Color(R, G, B)` or try `fill_rainbow()` from the FastLED library (with extra coding).

### How do I test individual parts?

Write a simple NeoPixel blink script to test LEDs.
The test code provided above is esentially a more complex version of this
Use `Serial.println(analogRead(MIC_PIN));` to monitor mic signal in the Serial Monitor.

