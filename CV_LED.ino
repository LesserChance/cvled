#include <Adafruit_NeoPixel.h>

/** CONSTANTS **/
const int NUM_LEDS = 30;     // The total number of leds
const int EFFECT_SPREAD = 1; // The number of leds before and after the target to apply the affect to
const int CV_REPEAT_MIN = 4; // The minimum number of times we read a CV val before calling it changed
const uint32_t NO_COLOR = 0; // used to turn an led off

/** GLOBALS **/
Adafruit_NeoPixel leds = Adafruit_NeoPixel(NUM_LEDS, 2, NEO_GRB + NEO_KHZ800);
uint32_t ledColors[NUM_LEDS][3];
int lastVal = 0;
int valCount = 0;
int currentVal = 0;

/**
 * Setup the board
 */
void setup() {
  Serial.begin(9600);
  setupColors();
  leds.setBrightness(255);
  leds.begin();
}

/**
 * Main loop
 */
void loop() {
  int val = getCvLed(analogRead(A0));
  if (val != lastVal) {
    lastVal = val;
    valCount = 0;
  } else {
    valCount++;
  }

  if (valCount == CV_REPEAT_MIN && currentVal != val) {
    currentVal = val;
    startEffect(currentVal);
  }

  playEffects();
}

/**
 * Initialize the array of led colors
 */
void setupColors() {
  int stepSize = 255 / (NUM_LEDS);
  
  //green to green-blue
  for(int i = 0; i < NUM_LEDS; i++) {
    ledColors[i][0] = 0;
    ledColors[i][1] = 255;
    ledColors[i][2] = i * stepSize;
  }
}

/**
 * return an led for 0 to [max leds] given the CV value 
 * @param {int} cv
 */
int getCvLed(int cv) {
  return cv / (1023 / (NUM_LEDS - 1));
}

