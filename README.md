# 1d-fighter

## Description

This is a 1-dimensional fighting game inspried by [this youtube video](https://www.youtube.com/watch?v=HrhVkw9Fgl4). You have two players, red and blue. Your player is also your health bar. When you get punched, you lose one LED. The last person standing wins.

## Code

The code is pretty simple, just reading button pins in  loop and updating the LEDs if necessary. The [FastLED](http://fastled.io/) library is used to talk to the LED strip. There are functions for punching, moving left or right, and flushing any updates to the LED strip.

## Construction

The game runs on an Arduino Uno. Instead of wiring a bunch of LEDs together, I am just using a WS2812 RGB LED strip. It only requires a single wire to control it. The top and bottom pieces are made of laser-cut acrylic (see [case.svg](case.svg)). The two acrylic pieces are separated with standoffs, which take 4-40 screws. The LED strip is attached using size 3 screws.

## Images

![](img/top.jpg)
![](img/side.jpg)