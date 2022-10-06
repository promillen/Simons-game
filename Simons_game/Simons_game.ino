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



const int buzzer = x;
*/
const int selectorButton = D3;


//Other variables
int gamemode = 0;
int difficulty = 0;
int skillLevel[] = {8, 14, 20, 31};
int levels[31];
boolean buttonState;
boolean lastButtonState = true; //Starts high
boolean buttonStillBeingHeld = false;

void setup() {
  pinMode(selectorButton, INPUT_PULLUP);
  Serial.begin(9600);
  lcd.init();
  lcd.backlight();
  lcd.setCursor(0,0);
  lcd.print("Initializing");
  
  //Initialize levels array
  for(int i = 0; i<31; i++) {
    levels[i] = -1;
  }

  lcd.clear();
  
  chooseGamemode();

  if(gamemode % 3 == 0) {
    setupSingleplayer();
  } else {
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("This gamemode");
    lcd.setCursor(0,1);
    lcd.print("isn't completed");
    delay(2000);
    ESP.restart();
  }
}

void loop() {
  // put your main code here, to run repeatedly:
  lcd.clear();
  Serial.print(".");
  delay(20);
}

void chooseGamemode() {
  Serial.println("Choose gamemode called");
  lcd.setCursor(0,0);
  lcd.print("Choose gamemode: ");
  lcd.setCursor(0,1);
  lcd.print("Single player");
  
  unsigned long holdingButton = millis();
  
  while(true) {
    delay(20);
    
    if(lastButtonState == false && millis() >= (holdingButton + 1000)){
      Serial.println("Button was hold for more than 1 seconds");
      buttonStillBeingHeld = true;
      break;
    }
    buttonState = digitalRead(selectorButton);
    if(lastButtonState != buttonState) {
      lastButtonState = buttonState;
      if(lastButtonState == false) {
        Serial.println("Button pressed");
        holdingButton = millis();
      }
      if(lastButtonState == true) {
        Serial.println("Button released");
        gamemode++;
        if(gamemode % 3 == 0) {
          lcd.setCursor(0,1);
          lcd.print("Single player");
        }
        
        if(gamemode % 3 == 1) {
          lcd.setCursor(0,1);
          lcd.print("Multiplayer 1");
        }
        
        if(gamemode % 3 == 2) {
          lcd.setCursor(0,1);
          lcd.print("Multiplayer 2");
        }
      }
    }
  }
}




void setupSingleplayer() {
  Serial.println("setupSinglePlayer called");
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Choose level:");
  lcd.setCursor(0,1);
  lcd.print("Easy");

  while(buttonStillBeingHeld == true && buttonState == false) {
    buttonState = digitalRead(selectorButton);
    delay(20);
  }

  buttonStillBeingHeld = false;

  unsigned long holdingButton = millis();

  while(true) {
    delay(20);
    if(lastButtonState == false && millis() >= (holdingButton + 1000)){ //might not work
      Serial.println("Button was hold for more than 1 seconds");
      break;
    }
    buttonState = digitalRead(selectorButton);

    if(lastButtonState != buttonState) {
      lastButtonState = buttonState;
      if(lastButtonState == false) {
        Serial.println("Button pressed");
        holdingButton = millis();
        gamemode++;
      }
      if(lastButtonState == true) {
        Serial.println("Button released");
        difficulty++;
        if(gamemode % 4 == 0) {
          lcd.clear();
          lcd.setCursor(0,0);
          lcd.print("Choose level:");
          lcd.setCursor(0,1);
          lcd.print("Easy");
        }
        
        if(gamemode % 4 == 1) {
          lcd.clear();
          lcd.setCursor(0,0);
          lcd.print("Choose level:");
          lcd.setCursor(0,1);
          lcd.print("Normal");
        }
        
        if(gamemode % 4 == 2) {
          lcd.clear();
          lcd.setCursor(0,0);
          lcd.print("Choose level:");
          lcd.setCursor(0,1);
          lcd.print("Hard");
        }

        if(gamemode % 4 == 3) {
          lcd.clear();
          lcd.setCursor(0,0);
          lcd.print("Choose level:");
          lcd.setCursor(0,1);
          lcd.print("Extreme");
        }
      }
    }
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
