#include <LiquidCrystal_I2C.h> // I2C address: 0x27

// set the LCD number of columns and rows
int lcdColumns = 16;
int lcdRows = 2;

// set LCD address, number of columns and rows
// if you don't know your display address, run an I2C scanner sketch
LiquidCrystal_I2C lcd(0x27, lcdColumns, lcdRows);

//Pin constants
const int redLed = x;
const int greenLed = x;
const int blueLed = x;
const int yellowLed = x;

const int redLedButton = x;
const int greenLedButton = x;
const int blueLedButton = x;
const int yellowLedButton = x;

const int selectorButton = x;

const int buzzer = x;

//Other variables
int skillLevel[] = [8, 14, 20, 31];

void setup() {
  // put your setup code here, to run once:

}

void loop() {
  // put your main code here, to run repeatedly:

}
