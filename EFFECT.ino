boolean effect_running[NUM_LEDS];
unsigned long effect_start[NUM_LEDS];
unsigned int effect_length[NUM_LEDS];
byte effect_inital_value[NUM_LEDS];

/**
 * run the current step in all running effects, update the strip
 */
void playEffects() {
  for(int i = 0; i < NUM_LEDS; i++) {
    if (effect_running[i]) {
      setLedEffectColor(i);
    } else {
      leds.setPixelColor(i, NO_COLOR);
    }
  }
  
  leds.show();
}

/**
 * Tell an individual led to start its effect
 * @param {int} led
 */
void startEffect(int led) {
  for(int i = led - EFFECT_SPREAD; i <= led + EFFECT_SPREAD; i++) {
    if (i >= 0 && i < NUM_LEDS) {
      startLedEffect(i);
    }
  }
}

/**
 * Set the start params for an leds effect
 * @param {int} led
 */
void startLedEffect(int led) {
  effect_running[led] = true;
  effect_start[led] = millis();

  // the duration of the effect
  effect_length[led] = 600;

  // the initial value (that will taper off) for the effect
  // in this case, brightness, 0-255
  effect_inital_value[led] = 255;
}

/**
 * run the current step in the given effect
 * @param {int} led
 */
void setLedEffectColor(int led) {
  unsigned long clock = millis();
  int brightness = getEffectValue(clock, led);

  if (brightness == 0) {
    // end the effect
    effect_running[led] = false;
  } else {
    // keep running the effect
    lightLedWithBrightness(led, brightness);
  }
}

/**
 * given the clock and led, determine what the current effect value should be
 * @param {unsigned long} clock
 * @param {int} led
 */
int getEffectValue(unsigned long clock, int led) {
  int effect_step = clock - effect_start[led];

  if (effect_step > effect_length[led]) {
    // the effect has ended
    return 0;
  }

  // the effect has not ended
  float step_by = effect_inital_value[led] / (float) effect_length[led];
  return effect_inital_value[led] - (effect_step * step_by);
}

/**
 * Set an led to its proper color given its position and brightness
 * @param {int} led
 * @param {int} brightness
 * @return uint32_t
 */
void lightLedWithBrightness(int led, int brightness) {
  uint32_t color = getLedColor(led, brightness);
  leds.setPixelColor(led, color);
}

/**
 * get an led color given its position and brightness
 * @param {int} led
 * @param {int} brightness
 * @return uint32_t
 */
uint32_t getLedColor(int led, int brightness) {
  byte red = ledColors[led][0];
  byte green = ledColors[led][1];
  byte blue = ledColors[led][2];
  
  //scale each color by a factor of the brightness
  // brightness of 255 = a full start value
  // brightness of 127 = half start value
  return leds.Color(
      ((red)   ? brightness * (float(red)   / 255) : 0),
      ((green) ? brightness * (float(green) / 255) : 0),
      ((blue)  ? brightness * (float(blue)  / 255) : 0)
  );
}

