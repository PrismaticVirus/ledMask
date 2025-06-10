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
  float threshold = 1.2;  // how many times louder is the sound than the noise floor

  // 0 centering the singnal
  baseline = alpha * raw + (1 - alpha) * baseline;
  float acSignal = abs(raw - baseline);

  // Track rolling noise floor
  noiseFloor = beta * acSignal + (1 - beta) * noiseFloor;

  // Actually supressing the background noise
  float output = (acSignal > threshold * noiseFloor) ? acSignal : 0.0;

  return output;
}
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
      mask.setPixelColor(leds - 1 - i, 0);  // Off
    }
  }
  mask.show();
}

void loop() {
  int lightLevel = analogRead(LIGHT_SENSOR);
  int micLevel = micOutput(MIC_PIN);

  Serial.print("Light: "); Serial.print(lightLevel);
  Serial.print(" | Mic: "); Serial.println(micLevel);

  if (lightLevel <= 5){
    lightEffect(micLevel, NUM_LEDS);
  } else {
    mask.fill(mask.Color(0, 0, 0));
    mask.show();
  }
}
