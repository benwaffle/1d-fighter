#include <Arduino.h>
#include <FastLED.h>

#define N_LEDS 16
#define INITIAL_LENGTH 4
#define INITIAL_P1 2
#define INITIAL_P2 8

#define EMPTY 0
#define P1 1
#define P2 2

int board[N_LEDS];
CRGB leds[N_LEDS];

typedef struct {
    int pos;
    int len;
} player;

player players[2] = {
    { .pos = 2, .len = 5 },
    { .pos = 8, .len = 5 },
};

void flush() {
    for (int i = 0; i < N_LEDS; ++i)
        leds[i] = CRGB::Black;

    for (int j = players[0].pos; j < players[0].pos + players[0].len; ++j)
        leds[j % N_LEDS] = CRGB(1,0,0);
    for (int j = players[1].pos; j < players[1].pos + players[1].len; ++j)
        leds[j % N_LEDS] = CRGB(0,0,1);

    FastLED.show();
}

void moveLeft(int playerNum) {
    player& p = players[playerNum];
    if (p.pos == 0) { // wrap
        p.pos = N_LEDS-1;
    } else {
        p.pos--;
    }
}

void moveRight(int playerNum) {
    player& p = players[playerNum];
    if (p.pos + p.len == N_LEDS) { // wrap
        p.pos = 0;
    } else {
        p.pos++;
    }
}

void setup() {
    Serial.begin(9600);

    FastLED.addLeds<NEOPIXEL, 7>(leds, N_LEDS);

    for (int i = 0; i < N_LEDS; ++i)
        board[i] = EMPTY;

    for (int i = 0; i < 2; ++i)
        for (int j = players[i].pos; j < players[i].pos + players[i].len; ++j)
            board[j] = i;

    flush();
}

void loop() {
    moveLeft(1);
    flush();
    delay(500);
}