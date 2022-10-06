#include <LiquidCrystal_I2C.h> // I2C address: 0x27

// set the LCD number of columns and rows
int lcdColumns = 16;
int lcdRows = 2;

// set LCD address, number of columns and rows
// if you don't know your display address, run an I2C scanner sketch
LiquidCrystal_I2C lcd(0x27, lcdColumns, lcdRows);

//Pin constants
const int redLed = D5;
const int yellowLed = D6;
const int greenLed = D7;
const int blueLed = D8;

/*
const int redLedButton = x;
const int greenLedButton = x;
const int blueLedButton = x;
const int yellowLedButton = x;

const int buzzer = x;
*/

const int selectorButton = D3;

//Other variables
int gamemode = 0;
int difficulty = -1;
int skillLevel[] = {8, 14, 20, 31};
int levels[31];
boolean buttonState;
boolean lastButtonState = true; //Starts high
boolean buttonStillBeingHeld = false;

void setup() {
  pinMode(selectorButton, INPUT_PULLUP);
  pinMode(redLed, OUTPUT);
  pinMode(yellowLed, OUTPUT);
  pinMode(greenLed, OUTPUT);
  pinMode(blueLed, OUTPUT);

  Serial.begin(9600);

  lcd.init();
  lcd.backlight();
  
  //Initialize levels array
  for(int i = 0; i<31; i++) {
    levels[i] = -1;
  }
  
  chooseGamemode();

  if(gamemode % 3 == 0) {
    setupSingleplayer();
    generateLevels(difficulty);
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
  Serial.println("Loop called");
  // put your main code here, to run repeatedly:
  lcd.clear();
  runLevels();
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
      difficulty = difficulty % 4;
      Serial.println("Button was hold for more than 1 seconds");
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
        difficulty++;
        
        if(difficulty % 4 == 0) {
          lcd.clear();
          lcd.setCursor(0,0);
          lcd.print("Choose level:");
          lcd.setCursor(0,1);
          lcd.print("Easy");
        }
        
        if(difficulty % 4 == 1) {
          lcd.clear();
          lcd.setCursor(0,0);
          lcd.print("Choose level:");
          lcd.setCursor(0,1);
          lcd.print("Normal");
        }
        
        if(difficulty % 4 == 2) {
          lcd.clear();
          lcd.setCursor(0,0);
          lcd.print("Choose level:");
          lcd.setCursor(0,1);
          lcd.print("Hard");
        }

        if(difficulty % 4 == 3) {
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
  delay(500);
  Serial.print("Called with: ");
  Serial.println(x);

  delay(500);
  Serial.print("skillLevel[x] = ");
  Serial.println(skillLevel[x]);

  delay(500);
  //Serial.print();
  //Serial.println();
  
  for(int i = 0; i<skillLevel[x]; i++) {
    levels[i] = random(1,5);
  }
  Serial.println();
  for(int i = 0; i<31; i++) {
    Serial.print(levels[i]);
    Serial.print(", ");
  }
  Serial.println();
  delay(1000);
}

void runLevels() {
  Serial.print("Run levels called");
    for (int i = 0; i < 31; i++) {
    if (levels[i] == 1) {
      Serial.println("rød");
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("Red");
      
      digitalWrite(redLed, HIGH);
      delay(500);
      digitalWrite(redLed, LOW);
      delay(500);
    } else if (levels[i] == 2) {
      Serial.println("gul");
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("Yellow");
      
      digitalWrite(yellowLed, HIGH);
      delay(500);
      digitalWrite(yellowLed, LOW);
      delay(500);
    } else if (levels[i] == 3) {
      Serial.println("grøn");
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("Green");
      digitalWrite(greenLed, HIGH);
      delay(500);
      digitalWrite(greenLed, LOW);
      delay(500);
    } else if (levels[i] == 4) {
      Serial.println("blå");
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("Blue");
      digitalWrite(blueLed, HIGH);
      delay(500);
      digitalWrite(blueLed, LOW);
      delay(500);
    } else if (levels[i] == -1) {
      Serial.println("done");
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("Done");
      delay(1000);
      ESP.restart();
    }
  }
}
