boolean effect_running[NUM_LEDS/EFFECT_SPREAD];
unsigned long effect_start[NUM_LEDS/EFFECT_SPREAD];
unsigned int effect_length[NUM_LEDS/EFFECT_SPREAD];
byte effect_inital_value[NUM_LEDS/EFFECT_SPREAD];

/**
 * run the current step in all running effects, update the strip
 */
void playEffects() {
  for(int i = 0; i < NUM_LEDS; i+=EFFECT_SPREAD) {
    bool effectRunning = effect_running[i/EFFECT_SPREAD];

    for(int j = 0; j < EFFECT_SPREAD; j++) {
      if (effectRunning) {
        setLedEffectColor(i+j);
      } else {
        leds.setPixelColor(i+j, NO_COLOR);
      }
    }
  }
  
  leds.show();
}

/**
 * Tell an individual led to start its effect
 * @param {int} led - the ACTUAL led
 */
void startEffect(int led) {
  int start_led = led - (led % (EFFECT_SPREAD));
  startLedEffect(start_led);
}

/**
 * Set the start params for an leds effect
 * @param {int} led - the START led
 */
void startLedEffect(int led) {
  int effect_index = led/EFFECT_SPREAD;
  
  effect_running[effect_index] = true;
  effect_start[effect_index] = millis();

  // the duration of the effect
  effect_length[effect_index] = EFFECT_DUR;

  // the initial value (that will taper off) for the effect
  // in this case, brightness, 0-255
  effect_inital_value[effect_index] = 255;
}

/**
 * run the current step in the given effect
 * @param {int} led - the ACTUAL led
 */
void setLedEffectColor(int led) {
  unsigned long clock = millis();
  int brightness = getEffectValue(clock, led);
  int effect_index = led/EFFECT_SPREAD;

  if (brightness == 0) {
    // end the effect
    effect_running[effect_index] = false;
  } else {
    // keep running the effect
    lightLedWithBrightness(led, brightness);
  }
}

/**
 * given the clock and led, determine what the current effect value should be
 * @param {unsigned long} clock
 * @param {int} led - the ACTUAL led
 */
int getEffectValue(unsigned long clock, int led) {
  int effect_index = led/EFFECT_SPREAD;
  int effect_step = clock - effect_start[effect_index];

  if (effect_step > effect_length[effect_index]) {
    // the effect has ended
    return 0;
  }

  // the effect has not ended
  float step_by = effect_inital_value[effect_index] / (float) effect_length[effect_index];
  return effect_inital_value[effect_index] - (effect_step * step_by);
}

/**
 * Set an led to its proper color given its position and brightness
 * @param {int} led - the ACTUAL led
 * @param {int} brightness
 * @return uint32_t
 */
void lightLedWithBrightness(int led, int brightness) {
  uint32_t color = getLedColor(led, brightness);
  leds.setPixelColor(led, color);
}

/**
 * get an led color given its position and brightness
 * @param {int} led - the ACTUAL led
 * @param {int} brightness
 * @return uint32_t
 */
uint32_t getLedColor(int led, int brightness) {
  int color_index = led/EFFECT_SPREAD;
  byte red = ledColors[color_index][0];
  byte green = ledColors[color_index][1];
  byte blue = ledColors[color_index][2];
  
  //scale each color by a factor of the brightness
  // brightness of 255 = a full start value
  // brightness of 127 = half start value
  return leds.Color(
      ((red)   ? brightness * (float(red)   / 255) : 0),
      ((green) ? brightness * (float(green) / 255) : 0),
      ((blue)  ? brightness * (float(blue)  / 255) : 0)
  );
}

