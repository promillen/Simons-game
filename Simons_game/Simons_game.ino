#include <LiquidCrystal_I2C.h> // I2C address: 0x27

// set the LCD number of columns and rows
int lcdColumns = 16;
int lcdRows = 2;

// set LCD address, number of columns and rows
// if you don't know your display address, run an I2C scanner sketch
LiquidCrystal_I2C lcd(0x27, lcdColumns, lcdRows);

//Pin constants
/*const int redLed = x;
const int greenLed = x;
const int blueLed = x;
const int yellowLed = x;

const int redLedButton = x;
const int greenLedButton = x;
const int blueLedButton = x;
const int yellowLedButton = x;

const int selectorButton = x;

const int buzzer = x;
*/


//Other variables
int gamemode = 1;
int difficulty = 0;
int skillLevel[] = {8, 14, 20, 31};
int levels[31];

void setup() {
  Serial.begin(9600);
  
  //Initialize array
  for(int i = 0; i<sizeof(levels)/2; i++) {
    levels[i] = -1;
  }

  Serial.println("Choose gamemode");
  while(Serial.available() == 0) {
  }
  int gamemode = Serial.read();
  Serial.print("Gamemode ");
  
  Serial.print(gamemode);
  Serial.println(" choosen");
  
  if(gamemode == 49) { //ASCII character for 1
    setupGamemode1();
  } else if(gamemode == 2) {
    return;
  } else if(gamemode == 3) {
    return;
  } else {
    return;
  }
}

void loop() {
  // put your main code here, to run repeatedly:
}

void setupGamemode1() {
  Serial.println("Choose difficulty");
  delay(1000);
  serialFlush();

  
  while(Serial.available() == 0) {
  }
  int difficulty = Serial.read();

  if(difficulty == 49) {
    Serial.print("You have choosen difficulty: ");
    Serial.println(1);
    delay(1000);
    generateLevels(1);
    
  } else if(difficulty == 50) {
    Serial.print("You have choosen difficulty: ");
    Serial.println(difficulty);
    generateLevels(2);
    
  } else if(difficulty == 51) {
    Serial.print("You have choosen difficulty: ");
    Serial.println(difficulty);
    generateLevels(3);
    
  } else if(difficulty == 52) {
    Serial.print("You have choosen difficulty: ");
    Serial.println(difficulty);
    generateLevels(4);
  }
}

void generateLevels(int x) {
  for(int i = 0; i<skillLevel[x-1]; i++) {
    levels[i] = random(0,3);
    Serial.print(levels[i]);
  }
  Serial.println();
  for(int i = 0; i<31; i++) {
    Serial.print(levels[i]);
    Serial.print(", ");
  }
  delay(1000);
}

void serialFlush(){
  while(Serial.available() > 0) {
    char t = Serial.read();
  }
}
