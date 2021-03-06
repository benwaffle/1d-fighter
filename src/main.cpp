#include <Arduino.h>
#include <FastLED.h>

#define N_LEDS 24

#define EMPTY 0

#define P0_LEFT_BUT 8
#define P0_RIGHT_BUT 3
#define P0_PUNCH_BUT 10
#define P1_LEFT_BUT 11
#define P1_RIGHT_BUT 12
#define P1_PUNCH_BUT 13

CRGB leds[N_LEDS];

typedef struct {
    int pos;
    int len;
    unsigned long last_move;
    bool holding_punch;
} player;

player players[2] = {
    { .pos = 4, .len = 5, .last_move = 0, .holding_punch = false },
    { .pos = 15, .len = 5, .last_move = 0, .holding_punch = false },
};

void flush() {
    for (int i = 0; i < N_LEDS; ++i)
        leds[i] = CRGB::Black;

    for (int j = players[0].pos; j < players[0].pos + players[0].len; ++j)
        leds[j % N_LEDS] += CRGB(20,0,0);
    for (int j = players[1].pos; j < players[1].pos + players[1].len; ++j)
        leds[j % N_LEDS] += CRGB(0,0,20);

    FastLED.show();
}

void win(int winner) {
    CRGB color = (winner == 0) ? CRGB(1,0,0) : CRGB(0,0,1);

    for (int i = 0; i < 20; ++i) {
        for (int i = 0; i < N_LEDS; ++i)
            leds[i] = color;
        FastLED.show();
        delay(50);
        for (int i = 0; i < N_LEDS; ++i)
            leds[i] = CRGB::Black;
        FastLED.show();
        delay(50);
    }

    players[0] = { .pos = 4, .len = 5, .last_move = 0, .holding_punch = false };
    players[1] = { .pos = 15, .len = 5, .last_move = 0, .holding_punch = false };
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

void punch(int attacker) {
    int victim = (attacker == 0) ? 1 : 0;
    player& a = players[attacker];
    player& v = players[victim];

    if (((v.pos + v.len >= N_LEDS || v.pos <= a.pos) && a.pos < v.pos + v.len) // a start in v
     || ((a.pos + a.len >= N_LEDS || a.pos <= v.pos) && v.pos < a.pos + a.len) // v start in a
     || (v.pos + v.len) % N_LEDS == a.pos
     || (a.pos + a.len) % N_LEDS == v.pos)
    {
        v.len--;

        if (a.pos < v.pos) { // move v right
            int steps = (a.pos + a.len) - v.pos + 2;
            v.pos += steps;
            v.pos %= N_LEDS;
        } else { // move v left
            int steps = v.len - (a.pos - v.pos) + 2;
            v.pos -= steps;
            if (v.pos < 0)
                v.pos += N_LEDS;
        }
    }
}

void setup() {
    pinMode(P0_LEFT_BUT, INPUT_PULLUP);
    pinMode(P0_RIGHT_BUT, INPUT_PULLUP);
    pinMode(P0_PUNCH_BUT, INPUT_PULLUP);
    pinMode(P1_LEFT_BUT, INPUT_PULLUP);
    pinMode(P1_RIGHT_BUT, INPUT_PULLUP);
    pinMode(P1_PUNCH_BUT, INPUT_PULLUP);

    FastLED.addLeds<NEOPIXEL, 2>(leds, N_LEDS);

    flush();
}

void loop() {
    if (millis() - players[0].last_move > 300) {
        if (digitalRead(P0_LEFT_BUT) == LOW) {
            moveLeft(0);
            players[0].last_move = millis();
        }

        if (digitalRead(P0_RIGHT_BUT) == LOW) {
            moveRight(0);
            players[0].last_move = millis();
        }

        if (digitalRead(P0_PUNCH_BUT) == LOW) {
            if (!players[0].holding_punch) {
                punch(0);
                players[0].holding_punch = true;
                players[0].last_move = millis();
            }
        } else {
            players[0].holding_punch = false;
        }
    }

    if (millis() - players[1].last_move > 300) {
        if (digitalRead(P1_LEFT_BUT) == LOW) {
            moveLeft(1);
            players[1].last_move = millis();
        }

        if (digitalRead(P1_RIGHT_BUT) == LOW) {
            moveRight(1);
            players[1].last_move = millis();
        }

        if (digitalRead(P1_PUNCH_BUT) == LOW) {
            if (!players[1].holding_punch) {
                punch(1);
                players[1].holding_punch = true;
                players[1].last_move = millis();
            }
        } else {
            players[1].holding_punch = false;
        }
    }

    flush();

    if (players[0].len == 0) {
        win(1);
    } else if (players[1].len == 0) {
        win(0);
    }
}
