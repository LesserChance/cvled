#include <Adafruit_NeoPixel.h>

/** CONSTANTS **/
const int NUM_LEDS = 220;    // The total number of leds
const int EFFECT_SPREAD = 20;// The number of leds before and after the target to apply the affect to
const int EFFECT_DUR = 100;  // The duration in milliseconds for the collor effect to run
const int CV_REPEAT_MIN = 4; // The minimum number of times we read a CV val before calling it changed
const uint32_t NO_COLOR = 0; // used to turn an led off

const int LED_PIN = 2;       // data pin for the LEDs
const int CV_PIN = A0;       // data pin for cv (tip)

/** GLOBALS **/
Adafruit_NeoPixel leds = Adafruit_NeoPixel(NUM_LEDS, LED_PIN, NEO_GRB + NEO_KHZ800);
uint32_t ledColors[NUM_LEDS/EFFECT_SPREAD][3];
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
  int val = getCvLed(analogRead(CV_PIN));
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
  int stepSize = 255 / (NUM_LEDS/EFFECT_SPREAD);
  
  //green to green-blue
  for(int i = 0; i < NUM_LEDS/EFFECT_SPREAD; i++) {
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
  return cv / (1023 / (NUM_LEDS - EFFECT_SPREAD));
}

