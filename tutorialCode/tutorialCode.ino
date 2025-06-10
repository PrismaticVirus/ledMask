#include <Adafruit_NeoPixel.h>
#define MIC_PIN A9
#define LIGHT_SENSOR A7
#define LED_PIN 3
#define NUM_LEDS 6

Adafruit_NeoPixel mask(NUM_LEDS, LED_PIN, NEO_GRB + NEO_KHZ800);

void setup() {
  pinMode(MIC_PIN, INPUT);
  Serial.begin(9600);
  mask.begin();
  mask.show();
}
int micOutput(int pin) {
  int raw = analogRead(pin);
  int baseline = 512;   // Center of AC signal
  float noiseFloor;     // rolling average of background noise
  float alpha = 0.01;   // baseline smoothing factor
  float beta = 0.01;    // noise floor smoothing factor
  float threshold = 2;  // how many times louder is the sound than the noise floor

  // 0 centering the singnal
  baseline = alpha * raw + (1 - alpha) * baseline;
  float acSignal = abs(raw - baseline);

  // Track rolling noise floor
  noiseFloor = beta * acSignal + (1 - beta) * noiseFloor;

  // Actually supressing the background noise
  float output = (acSignal > threshold * noiseFloor) ? acSignal : 0.0;

  delay(10);  // 100Hz Sampling
  return map(output, 0, 512, 0, 255);
}

void lightEffect(int signal, int leds) {
  signal = constrain(signal, 0, 255);

  int ledsToLight = map(signal, 0, 255, 0, leds);

  for (int i = 0; i < NUM_LEDS; i++) {
    if (i < ledsToLight) {
      mask.setPixelColor(leds - 1 - i, mask.Color(255, 0, 255));
    } else {
      delay(20);
      mask.setPixelColor(leds - 1 - i, 0);  // Off
    }
  }
  mask.show();
}
void loop() {
  lightEffect(micOutput(MIC_PIN), NUM_LEDS);
}