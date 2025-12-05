#include "effects/effects.h"
#include "effects/led_control.h"
#include "effects/palettes.h"

#include <Arduino.h>
#include <FastLED.h>


void effectsRoutine() {
  static byte prevEff = 255;
  if (!effectsTimer.isReady()) return;

  if (dawnTimer.running() || postDawn.running()) {
    FastLED.setBrightness(255);
    byte thisColor = dawnTimer.getLength8();
    if (postDawn.running()) thisColor = 255;
    fill_solid(leds, MAX_LEDS, ColorFromPalette(HeatColors_p, thisColor, scaleFF(thisColor, dawn.bright), LINEARBLEND));
    drawClock(cfg.length / 2 - 4, 100, 0);
    FastLED.show();
    if (dawnTimer.isReady()) {
      dawnTimer.stop();
      postDawn.setInterval(dawn.post * 60000ul);
      postDawn.restart();
    }
    if (postDawn.isReady()) {
      postDawn.stop();
      FastLED.clear();
      FastLED.show();
    }
    return;
  }

  if (!cfg.state) return;
  int thisLength = getLength();
  byte thisScale = getScale();
  byte thisBright = getBright();

  // UDP communication logic simplified: no musicMode check, only briMode now affects this path.
  if (briMode()) {
    if (cfg.role) {
      static uint32_t tmr = 0;
      if ((millis() - tmr >= 1000) && (millis() - updateTimer >= 1000)) { // Simplified timing, no longer dependent on musicMode
        sendUDP(7, thisLength, thisScale, thisBright);
        tmr = millis();
      }
    } else {
      if (millis() - gotAdcTimer < 4000) {
        thisLength = udpLength;
        thisScale = udpScale;
        thisBright = udpBright;
      }
    }
  }

  if (turnoffTimer.running()) thisBright = scaleFF(thisBright, 255 - turnoffTimer.getLength8());
  else if (blinkTimer.runningStop()) thisBright = scaleFF(thisBright, blinkTimer.getLength8());
  if (turnoffTimer.isReady()) {
    turnoffTimer.stop();
    setPower(0);
    return;
  }
  FastLED.setBrightness(thisBright);

  if (prevEff != CUR_PRES.effect) {
    FastLED.clear();
    prevEff = CUR_PRES.effect;
    extern bool loading;
    loading = true;
  }
  yield();

  switch (CUR_PRES.effect) {
    case 1: // =================================== PERLIN NOISE ===================================
      if (cfg.deviceType > 1) {
        FOR_j(0, cfg.length) {
          FOR_i(0, cfg.width) {
            setPix(i, j, ColorFromPalette(paletteArr[CUR_PRES.palette - 1],
                                          scalePalette(inoise8(
                                              i * (thisScale / 5) - cfg.width * (thisScale / 5) / 2,
                                              j * (thisScale / 5) - cfg.length * (thisScale / 5) / 2,
                                              (now.weekMs >> 1) * CUR_PRES.speed / 255)),
                                          255, LINEARBLEND));
          }
        }
      } else {
        FOR_i(0, cfg.length) {
          leds[i] = ColorFromPalette(paletteArr[CUR_PRES.palette - 1],
                                     scalePalette(inoise8(i * (thisScale / 5) - cfg.length * (thisScale / 5) / 2,
                                                      (now.weekMs >> 1) * CUR_PRES.speed / 255)),
                                     255, LINEARBLEND);
        }
      }
      break;

    case 2: // ==================================== SOLID COLOR ====================================
      {
        fill_solid(leds, (size_t)cfg.length * cfg.width, CHSV(CUR_PRES.color, thisScale, 30));
        CRGB thisColor = CHSV(CUR_PRES.color, thisScale, thisBright);
        if (CUR_PRES.fromCenter) {
          fillStrip(cfg.length / 2, cfg.length / 2 + thisLength / 2, thisColor);
          fillStrip(cfg.length / 2 - thisLength / 2, cfg.length / 2, thisColor);
        } else {
          fillStrip(0, thisLength, thisColor);
        }
      }
      break;

    case 3: // ================================= COLOR FADE / CHANGE =================================
      {
        CRGB thisColor = ColorFromPalette(paletteArr[CUR_PRES.palette - 1], scalePalette((now.weekMs >> 5) * CUR_PRES.speed / 255), 10, LINEARBLEND);
        fill_solid(leds, (size_t)cfg.length * cfg.width, thisColor);
        thisColor = ColorFromPalette(paletteArr[CUR_PRES.palette - 1], scalePalette((now.weekMs >> 5) * CUR_PRES.speed / 255), thisBright, LINEARBLEND);
        if (CUR_PRES.fromCenter) {
          fillStrip(cfg.length / 2, cfg.length / 2 + thisLength / 2, thisColor);
          fillStrip(cfg.length / 2 - thisLength / 2, cfg.length / 2, thisColor);
        } else {
          fillStrip(0, thisLength, thisColor);
        }
      }
      break;

    case 4: // ================================== GRADIENT ==================================
      if (CUR_PRES.fromCenter) {
        FOR_i(cfg.length / 2, cfg.length) {
          byte bright = 255;
          // Removed: if (CUR_PRES.soundReact == GL_REACT_LEN) bright = (i < cfg.length / 2 + thisLength / 2) ? (thisBright) : (10);
          CRGB thisColor = ColorFromPalette(
                                paletteArr[CUR_PRES.palette - 1],
                                scalePalette((i * (thisScale * 1.9 + 25) / cfg.length) + ((now.weekMs >> 3) * (CUR_PRES.speed - 128) / 128)),
                                bright, LINEARBLEND);
          if (cfg.deviceType > 1) fillRow(i, thisColor);
          else leds[i] = thisColor;
        }
        if (cfg.deviceType > 1) FOR_i(0, cfg.length / 2) fillRow(i, leds[(cfg.length - i)*cfg.width - 1]);
        else FOR_i(0, cfg.length / 2) leds[i] = leds[cfg.length - i - 1];

      } else {
        FOR_i(0, cfg.length) {
          byte bright = 255;
          // Removed: if (CUR_PRES.soundReact == GL_REACT_LEN) bright = (i < thisLength) ? (thisBright) : (10);
          CRGB thisColor = ColorFromPalette(
                                paletteArr[CUR_PRES.palette - 1],
                                scalePalette((i * (thisScale * 1.9 + 25) / cfg.length) + ((now.weekMs >> 3) * (CUR_PRES.speed - 128) / 128)),
                                bright, LINEARBLEND);
          if (cfg.deviceType > 1) fillRow(i, thisColor);
          else leds[i] = thisColor;
        }
      }
      break;

    case 5: // =================================== PARTICLES ===================================
      FOR_i(0, (size_t)cfg.length * cfg.width) leds[i].fadeToBlackBy(70);
      {
        uint16_t rndVal = 0;
        byte amount = (thisScale >> 3) + 1;
        FOR_i(0, amount) {
          rndVal = rndVal * 2053 + 13849;
          int homeX = inoise16((uint32_t)i * 100000000ul + (now.weekMs << 3) * CUR_PRES.speed / 255);
          homeX = map(homeX, 15000, 50000, 0, cfg.length);
          int offsX = inoise8((uint32_t)i * 2500 + (now.weekMs >> 1) * CUR_PRES.speed / 255) - 128;
          offsX = cfg.length / 2 * offsX / 128;
          int thisX = homeX + offsX;

          if (cfg.deviceType > 1) {
            int homeY = inoise16((uint32_t)i * 100000000ul + 2000000000ul + (now.weekMs << 3) * CUR_PRES.speed / 255);
            homeY = map(homeY, 15000, 50000, 0, cfg.width);
            int offsY = inoise8((uint32_t)i * 2500 + 30000 + (now.weekMs >> 1) * CUR_PRES.speed / 255) - 128;
            offsY = cfg.length / 2 * offsY / 128;
            int thisY = homeY + offsY;
            setPix(thisX, thisY, CUR_PRES.fromPal ?
                   ColorFromPalette(paletteArr[CUR_PRES.palette - 1], scalePalette(i * 255 / amount), 255, LINEARBLEND) :
                   CHSV(CUR_PRES.color, 255, 255)
                  );
          } else {
            setLED(thisX, CUR_PRES.fromPal ?
                   ColorFromPalette(paletteArr[CUR_PRES.palette - 1], scalePalette(i * 255 / amount), 255, LINEARBLEND) :
                   CHSV(CUR_PRES.color, 255, 255)
                  );
          }
        }
      }
      break;

    case 6: // ==================================== FIRE (Old) ====================================
      if (cfg.deviceType > 1) {
        fireRoutine(CUR_PRES.speed / 2);
      } else {
        FastLED.clear();
        static byte heat[MAX_LEDS];
        CRGBPalette16 gPal;
        if (CUR_PRES.color < 5) gPal = HeatColors_p;
        else gPal = CRGBPalette16(CRGB::Black, CHSV(CUR_PRES.color, 255, 255), CRGB::White);
        if (CUR_PRES.fromCenter) thisLength /= 2;

        for (int i = 0; i < thisLength; i++) heat[i] = qsub8(heat[i], random8(0, ((((255 - thisScale) / 2 + 20) * 10) / thisLength) + 2));
        for (int k = thisLength - 1; k >= 2; k--) heat[k] = (heat[k - 1] + heat[k - 2] + heat[k - 2] ) / 3;
        if (random8() < 120 ) {
          int y = random8(7);
          heat[y] = qadd8(heat[y], random8(160, 255));
        }
        if (CUR_PRES.fromCenter) {
          for (int j = 0; j < thisLength; j++) leds[cfg.length / 2 + j] = ColorFromPalette(gPal, scale8(heat[j], 240));
          FOR_i(0, cfg.length / 2) leds[i] = leds[cfg.length - i - 1];
        } else {
          for (int j = 0; j < thisLength; j++) leds[j] = ColorFromPalette(gPal, scale8(heat[j], 240));
        }
      }
      break;

    case 7: // ==================================== FIRE 2020 ====================================
      FastLED.clear();
      if (cfg.deviceType > 1) {
        fire2020(CUR_PRES.scale, thisLength);
      } else {
        static byte heat[MAX_LEDS];
        if (CUR_PRES.fromCenter) thisLength /= 2;

        for (int i = 0; i < thisLength; i++) heat[i] = qsub8(heat[i], random8(0, ((((255 - thisScale) / 2 + 20) * 10) / thisLength) + 2));
        for (int k = thisLength - 1; k >= 2; k--) heat[k] = (heat[k - 1] + heat[k - 2] + heat[k - 2] ) / 3;
        if (random8() < 120 ) {
          int y = random8(7);
          heat[y] = qadd8(heat[y], random8(160, 255));
        }
        if (CUR_PRES.fromCenter) {
          for (int j = 0; j < thisLength; j++) leds[cfg.length / 2 + j] = ColorFromPalette(paletteArr[CUR_PRES.palette - 1], scale8(heat[j], 240));
          FOR_i(0, cfg.length / 2) leds[i] = leds[cfg.length - i - 1];
        } else {
          for (int j = 0; j < thisLength; j++) leds[j] = ColorFromPalette(paletteArr[CUR_PRES.palette - 1], scale8(heat[j], 240));
        }
      }
      break;

    case 8: // ================================== CONFETTI ==================================
      {
        byte amount = (thisScale >> 3) + 1;
        FOR_i(0, amount) {
          int x = random(0, (long)cfg.length * cfg.width);
          if (leds[x] == CRGB(0, 0, 0)) leds[x] = CUR_PRES.fromPal ?
                                          ColorFromPalette(paletteArr[CUR_PRES.palette - 1], scalePalette((byte)(i * 255 / amount)), 255, LINEARBLEND) :
                                          CHSV(CUR_PRES.color, 255, 255);
        }
        FOR_i(0, (size_t)cfg.length * cfg.width) {
          if (leds[i].r >= 10 || leds[i].g >= 10 || leds[i].b >= 10) leds[i].fadeToBlackBy(CUR_PRES.speed / 2 + 1);
          else leds[i] = 0;
        }
      }
      break;

    case 9: // =================================== TORNADO / SWIRL ===================================
      FastLED.clear();
      FOR_k(0, (thisScale >> 5) + 1) {
        FOR_i(0, cfg.length) {
          byte thisPos = inoise8((uint32_t)i * 10 + (now.weekMs >> 3) * CUR_PRES.speed / 255 + (uint32_t)k * 10000, (now.weekMs >> 1) * CUR_PRES.speed / 255);
          thisPos = map(thisPos, 50, 200, 0, cfg.width);
          byte scale = 4;
          FOR_j(0, scale) {
            CRGB color = ColorFromPalette(paletteArr[CUR_PRES.palette - 1], scalePalette((byte)(j * 255 / scale)), (255 - (byte)(j * 255 / (scale - 1))), LINEARBLEND);
            if (j == 0) {
              setPixOverlap(thisPos, i, color);
            } else {
              setPixOverlap(thisPos - j, i, color);
              setPixOverlap(thisPos + j, i, color);
            }
          }
        }
      }
      break;

    case 10: // =================================== CLOCK ===================================
      FastLED.clear();
      drawClock(mapFF(CUR_PRES.scale, 0, cfg.length - 7), (CUR_PRES.speed < 10) ? 0 : (255 - CUR_PRES.speed), CHSV(CUR_PRES.color, 255, 255));
      break;

    case 11: // ================================= WEATHER ==================================
      // No implementation provided for this case.
      break;
  }

  if (CUR_PRES.advMode == GL_ADV_CLOCK && CUR_PRES.effect != 9) drawClock(mapFF(CUR_PRES.scale, 0, cfg.length - 7), 100, 0);
  if (btnClicks > 0) fill_solid(leds, (size_t)btnClicks, CRGB::White);
  if (brTicks > 0) fill_solid(leds, (size_t)brTicks, CRGB::Cyan);
  yield();
  FastLED.show();
}

// Removed: bool musicMode() function

// BriMode remains, as it's not directly related to sound input.
bool briMode() {
  return (cfg.adcMode == GL_ADC_BRI); // Simplified, only ADC_BRI possible
}

byte getBright() {
  int maxBr = cfg.bright;
  byte fadeBr = 255;
  if (CUR_PRES.fadeBright) fadeBr = CUR_PRES.bright;

  extern class PhotSensor phot; // Placeholder

  if (briMode()) {
    maxBr = constrain(phot.getFil(), cfg.minLight, cfg.maxLight);
    if (cfg.minLight != cfg.maxLight)
      maxBr = map(maxBr, cfg.minLight, cfg.maxLight, cfg.minBright, cfg.maxBright);
  }
  // Removed: Sound reactivity for brightness (if (musicMode() && CUR_PRES.soundReact == GL_REACT_BRI))
  return scaleFF((byte)maxBr, fadeBr);
}

int getLength() {
  // Removed: Sound reactivity for length
  return cfg.length; // Always returns full length if no sound reactivity
}

byte getScale() {
  // Removed: Sound reactivity for scale
  return CUR_PRES.scale; // Always returns preset scale if no sound reactivity
}