#include <Arduino.h>
#include <NeoPixelBus.h>

#define N_LEDS 16

NeoPixelBus<NeoGrbFeature, NeoWs2812Method> strip(N_LEDS, 7);

const uint8_t colorSaturation = 1;
RgbColor red(colorSaturation, 0, 0);
RgbColor green(0, colorSaturation, 0);
RgbColor blue(0, 0, colorSaturation);
RgbColor white(colorSaturation);
RgbColor black(0);

enum {
    EMPTY,
    P1,
    P2
} board[N_LEDS];

void setup() {
    strip.Begin();
    strip.ClearTo(red);
    strip.Show();
}

void loop() {
    strip.ClearTo(green);
    strip.Show();

    delay(500);

    strip.ClearTo(red);
    strip.Show();

    delay(500);

    strip.ClearTo(blue);
    strip.Show();

    delay(500);
}