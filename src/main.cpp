#include <Arduino.h>
#include <FastLED.h>

#define N_LEDS 16

#define EMPTY 0

#define P1_RIGHT_BUT 8
#define P1_LEFT_BUT 9
#define P1_PUNCH_BUT 10
#define P0_RIGHT_BUT 11
#define P0_LEFT_BUT 12
#define P0_PUNCH_BUT 13

CRGB leds[N_LEDS];

typedef struct {
    int pos;
    int len;
    unsigned long last_move;
} player;

player players[2] = {
    { .pos = 2, .len = 4, .last_move = 0 },
    { .pos = 8, .len = 4, .last_move = 0 },
};

void flush() {
    for (int i = 0; i < N_LEDS; ++i)
        leds[i] = CRGB::Black;

    for (int j = players[0].pos; j < players[0].pos + players[0].len; ++j)
        leds[j % N_LEDS] += CRGB(1,0,0);
    for (int j = players[1].pos; j < players[1].pos + players[1].len; ++j)
        leds[j % N_LEDS] += CRGB(0,0,1);

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
    if (p.pos == N_LEDS - 1) { // wrap
        p.pos = 0;
    } else {
        p.pos++;
    }
}

void setup() {
    pinMode(P0_LEFT_BUT, INPUT_PULLUP);
    pinMode(P0_RIGHT_BUT, INPUT_PULLUP);
    pinMode(P0_PUNCH_BUT, INPUT_PULLUP);
    pinMode(P1_LEFT_BUT, INPUT_PULLUP);
    pinMode(P1_RIGHT_BUT, INPUT_PULLUP);
    pinMode(P1_PUNCH_BUT, INPUT_PULLUP);

    Serial.begin(9600);

    FastLED.addLeds<NEOPIXEL, 7>(leds, N_LEDS);

    flush();
}

void loop() {
    if (millis() - players[0].last_move > 300) {
        if (digitalRead(P0_LEFT_BUT) == LOW) {
            moveLeft(0);
            Serial.println("P0 left");
            players[0].last_move = millis();
        }

        if (digitalRead(P0_RIGHT_BUT) == LOW) {
            moveRight(0);
            Serial.println("P0 right");
            players[0].last_move = millis();
        }
    }

    if (millis() - players[1].last_move > 300) {
        if (digitalRead(P1_LEFT_BUT) == LOW) {
            moveLeft(1);
            Serial.println("P1 left");
            players[1].last_move = millis();
        }

        if (digitalRead(P1_RIGHT_BUT) == LOW) {
            moveRight(1);
            Serial.println("P1 right");
            players[1].last_move = millis();
        }
    }

    flush();

    Serial.print("p0: at ");
    Serial.print(players[0].pos);
    Serial.print(" len ");
    Serial.print(players[0].len);
    Serial.print(", p1: at ");
    Serial.print(players[1].pos);
    Serial.print(" len ");
    Serial.print(players[1].len);
    Serial.println();
}