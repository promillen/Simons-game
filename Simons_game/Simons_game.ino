//Ide til spil. Implementer mulighed for at vælge tidsinterval mellem LED blink

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

const int redLedButton = D4;
const int yellowLedButton = D0;
const int greenLedButton = 9;
const int blueLedButton = 10;

const int selectorButton = D3;

/*
const int buzzer = x;
*/

//Other variables
int gamemode = 0;
int difficulty = -1;
int skillLevel[] = {8, 14, 20, 31};
int levels[31];
int buttonValue[] = {1, 2, 3, 4};
int currentLength = 0;
boolean gameIsComplete = false;

//Buttons
boolean buttonStateInputButton;
boolean lastButtonStateInputButton = true; //Starts high
boolean inputButtonStillBeingHeld = false;

boolean buttonStateRedInput;
boolean buttonStateYellowInput;
boolean buttonStateGreenInput;
boolean buttonStateBlueInput;
boolean lastButtonStateRedInput = true;
boolean lastButtonStateYellowInput = true;
boolean lastButtonStateGreenInput = true;
boolean lastButtonStateBlueInput = true;

void setup() {
  pinMode(selectorButton, INPUT_PULLUP);
  pinMode(redLedButton, INPUT);
  pinMode(yellowLedButton, INPUT);
  pinMode(greenLedButton, INPUT);
  pinMode(blueLedButton, INPUT);
  
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
  if(!gameIsComplete) {
    playGame();
  } else {
    restartGame(); 
  }
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
    
    if(lastButtonStateInputButton == false && millis() >= (holdingButton + 1000)){
      inputButtonStillBeingHeld = true;
      break;
    }
    buttonStateInputButton = digitalRead(selectorButton);
    if(lastButtonStateInputButton != buttonStateInputButton) {
      lastButtonStateInputButton = buttonStateInputButton;
      if(lastButtonStateInputButton == false) {
        holdingButton = millis();
      }
      if(lastButtonStateInputButton == true) {
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
  
  while(inputButtonStillBeingHeld == true && buttonStateInputButton == false) {
    buttonStateInputButton = digitalRead(selectorButton);
    delay(20);
  }

  inputButtonStillBeingHeld = false;

  unsigned long holdingButton = millis();

  while(true) {
    delay(20);
    if(lastButtonStateInputButton == false && millis() >= (holdingButton + 1000)){ //might not work
      difficulty = difficulty % 4;
      break;
    }
    buttonStateInputButton = digitalRead(selectorButton);

    if(lastButtonStateInputButton != buttonStateInputButton) {
      lastButtonStateInputButton = buttonStateInputButton;
      if(lastButtonStateInputButton == false) {
        holdingButton = millis();
      }
      if(lastButtonStateInputButton == true) {
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
  Serial.print("Called with: ");
  Serial.println(x);

  Serial.print("skillLevel[x] = ");
  Serial.println(skillLevel[x]);
  
  for(int i = 0; i<skillLevel[x]; i++) {
    levels[i] = random(1,5);
  }
  Serial.println();
  for(int i = 0; i<31; i++) {
    Serial.print(levels[i]);
    Serial.print(", ");
  }
  Serial.println();
}

void restartGame() {
  boolean restartOrSetup = false;

  while(inputButtonStillBeingHeld == true && buttonStateInputButton == false) {
    buttonStateInputButton = digitalRead(selectorButton);
    delay(20);
  }

  inputButtonStillBeingHeld = false;

  unsigned long holdingButton = millis();

  while(true) {
    delay(20);
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Now do?");
    lcd.setCursor(0,1);
    if(restartOrSetup) {
      lcd.print("restart");
      delay(50);
    } else {
      lcd.print("setup");
      delay(50);
    }
    
    if(lastButtonStateInputButton == false && millis() >= (holdingButton + 1000)){ //might not work
      if(restartOrSetup) {
        ESP.restart();
      } else {
        lcd.clear();
        lcd.setCursor(0,0);
        lcd.print("Not implemented");
        lcd.setCursor(0,1);
        lcd.print("yet");
        delay(1000);
        ESP.restart();
      }
      break;
    }
    buttonStateInputButton = digitalRead(selectorButton);

    if(lastButtonStateInputButton != buttonStateInputButton) {
      lastButtonStateInputButton = buttonStateInputButton;
      if(lastButtonStateInputButton == false) {
        holdingButton = millis();
      }
      if(lastButtonStateInputButton == true) {
        restartOrSetup = !restartOrSetup;
      }
    }
  }
}

void playGame() {
  currentLength = 0;
  Serial.println("Play game called");
  Serial.print("Choosen difficulty: ");
  Serial.println(skillLevel[difficulty]);
  
  for(int i = 0; i < skillLevel[difficulty]; i++) {
    Serial.print("currentLength = ");
    Serial.println(currentLength);
    delay(1000);
    for(int h = 0; h <= currentLength; h++) {
      Serial.print("Calling LED switch with: ");
      Serial.println(levels[h]);
      delay(100);

      switch(levels[h]) {
        case 1:
          lcd.clear();
          lcd.setCursor(0,0);
          lcd.print("Red");
          digitalWrite(redLed, HIGH);
          delay(250);
          digitalWrite(redLed, LOW);
          delay(250);
          Serial.println("Lighting on red LED");
          break;
        case 2:
          lcd.clear();
          lcd.setCursor(0,0);
          lcd.print("Yellow");
          digitalWrite(yellowLed, HIGH);
          delay(250);
          digitalWrite(yellowLed, LOW);
          delay(250);
          Serial.println("Lighting on yellow LED");
          break;
        case 3:
          lcd.clear();
          lcd.setCursor(0,0);
          lcd.print("Green");
          digitalWrite(greenLed, HIGH);
          delay(250);
          digitalWrite(greenLed, LOW);
          delay(250);
          Serial.println("Lighting on green LED");
          break;
        case 4:
          lcd.clear();
          lcd.setCursor(0,0);
          lcd.print("Blue");
          digitalWrite(blueLed, HIGH);
          delay(250);
          digitalWrite(blueLed, LOW);
          delay(250);
          Serial.println("Lighting on blue LED");
          break;
        default:
          Serial.println("done");
          lcd.clear();
          lcd.setCursor(0,0);
          lcd.print("Done");
          delay(1000);
      }
    }  
    Serial.println("Increasing currentLenght by 1. Going for next color");
    Serial.println("Increasing before check for easibility");
    Serial.println();
    currentLength++;
    Serial.println("Going into guess logic");

    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Press the colors,");
    lcd.setCursor(0,1);
    lcd.print("in correct order");

    delay(1000);

    Serial.print("Number of guesses: ");
    Serial.println(currentLength);
    Serial.println();

    Serial.print("Making array guesses with size: ");
    Serial.println(skillLevel[difficulty]);

    int guesses[skillLevel[difficulty]];
    for(int w = 0; w < skillLevel[difficulty]; w++) {
      guesses[w] = 0;
    }
    
    
    for(int w = 0; w < currentLength; w++) {
      Serial.print("Input ");
      Serial.print(w + 1);
      Serial.println();

      guesses[w] = gettingInputFromColorButtons();

      Serial.print("Guesses array: ");
      for(int w = 0; w < currentLength; w++) {
        Serial.print(guesses[w]);
        Serial.print(", ");
      }

      Serial.println();
      Serial.println();
      delay(200); //Shold probably be deleted in final version
    }

    Serial.println("Comparing guess to actual sequence");
    for(int t = 0; t < currentLength; t++) {
      if(guesses[t] == levels[t]) {
        Serial.print("Guess ");
        Serial.print(t +1);
        Serial.println(" correct");
      } else {
        Serial.print("Guess ");
        Serial.print(t +1);
        Serial.println(" wasn't correct");
        restartGame();
      }
    }
  }

  Serial.println("Game is complete");
  gameIsComplete = true;
}

int gettingInputFromColorButtons() {
  while(true) {
    delay(20);
    buttonStateRedInput = digitalRead(redLedButton);
    buttonStateYellowInput = digitalRead(yellowLedButton);
    buttonStateGreenInput = digitalRead(greenLedButton);
    buttonStateBlueInput = digitalRead(blueLedButton);
  
    if(lastButtonStateRedInput != buttonStateRedInput) {
      lastButtonStateRedInput = buttonStateRedInput;
  
      lastButtonStateRedInput = buttonStateRedInput;
    
      if(lastButtonStateRedInput == false) {
        Serial.println("Red input button pushed");
      }
      if(lastButtonStateRedInput == true) {
        return 1;
      }
  
    } else if(lastButtonStateYellowInput != buttonStateYellowInput) {
      lastButtonStateYellowInput = buttonStateYellowInput;
      
      if(lastButtonStateYellowInput == false) {
       Serial.println("Yellow input button pushed");
      }
      if(lastButtonStateYellowInput == true) {
        return 2;
      }
  
    } else if(lastButtonStateGreenInput != buttonStateGreenInput) {
      lastButtonStateGreenInput = buttonStateGreenInput;
      
      if(lastButtonStateGreenInput == false) {
       Serial.println("Green input button pushed");
      }
      if(lastButtonStateGreenInput == true) {
        return 3;
      }
  
    } else if(lastButtonStateBlueInput != buttonStateBlueInput) {
      lastButtonStateBlueInput = buttonStateBlueInput;
      if(lastButtonStateBlueInput == false) {
       Serial.println("Blue input button pushed");
      }
      if(lastButtonStateBlueInput == true) {
        return 4;
      }
    }
  }
}

/*

int readColorButtons() {
  Serial.println("readColorButtons method called");

  while(true) {
    
  }
  
  while(buttonStateRedInput && buttonStateYellowInput && buttonStateGreenInput && buttonStateBlueInput) {
    buttonStateRedInput = digitalRead(redLedButton);
    buttonStateYellowInput = digitalRead(yellowLedButton);
    buttonStateGreenInput = digitalRead(greenLedButton);
    buttonStateBlueInput = digitalRead(blueLedButton);
    delay(20);
  }
  delay(100);

  if(!buttonStateRedInput) {
    Serial.print("Returning 1 - ");
    Serial.println("Red button pushed");
    return 1; 
  } else if(!buttonStateYellowInput) {
    Serial.println("Returning 2");
    Serial.println("Yellow button pushed");
    return 2;
  } else if(!buttonStateGreenInput) {
    Serial.println("Returning 3");
    Serial.println("Green button pushed");
    return 3;
  } else if(!buttonStateBlueInput) {
    Serial.println("Returning 4");
    Serial.println("Blue button pushed");
    return 4;
  }
}
*/

/*

      for(int g; g < currentLength; g++) {
        int currentButtonPressed = 0;
        
        while(digitalRead(redLedButton) && digitalRead(yellowLedButton) && digitalRead(greenLedButton) && digitalRead(blueLedButton)) {
          delay(5);
        }
        if(digitalRead(redLedButton)) {
          currentButtonPressed = 1;
        } else if(digitalRead(yellowLedButton)) {
          currentButtonPressed = 2;
        } else if(digitalRead(greenLedButton)) {
          currentButtonPressed = 3;
        } else if(digitalRead(blueLedButton)) {
          currentButtonPressed = 4;
        }

        if(currentButtonPressed != levels[g]) {
          Serial.println("Wrong button pressed");
          return;
        }
      }
      currentLength++;
  }
}
*/
