#include <AnalogButtons.h>

//Ide til spil. Implementer mulighed for at vælge tidsinterval mellem LED blink
//Overvej at bruge dette library til knapper https://github.com/madleech/Button

#include <LiquidCrystal_I2C.h> // I2C address: 0x27

#define ANALOG_PIN A0

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

AnalogButtons analogButtons(ANALOG_PIN, INPUT);
Button b1 = Button(1024, &b1Click);
Button b2 = Button(996, &b2Click);
Button b3 = Button(896, &b3Click);
Button b4 = Button(744, &b4Click);

const int selectorButton = D3;

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

int colorButtonPressed = 0;

// A call back function that you pass into the constructor of AnalogButtons, see example
// below. Alternativly you could extend the Button class and re-define the methods pressed() 
// or held() which are called 
void b1Click() {  
  colorButtonPressed = 4;
  Serial.print("button blue clicked"); 
}
void b2Click() {  
  colorButtonPressed = 3;
  Serial.print("button green clicked"); 
}

void b3Click() {  
  colorButtonPressed = 2;
  Serial.print("button yellow clicked"); 
}

void b4Click() {  
  colorButtonPressed = 1;
  Serial.print("button red clicked"); 
}

void setup() {
  pinMode(selectorButton, INPUT_PULLUP);

  analogButtons.add(b1);
  analogButtons.add(b2);
  analogButtons.add(b3);
  analogButtons.add(b4); 
  
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
      colorButtonPressed = 0;

      Serial.print("Guesses array: ");
      for(int w = 0; w < currentLength; w++) {
        Serial.print(guesses[w]);
        Serial.print(", ");
      }
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
  Serial.println("Calling analog button check");
  while(colorButtonPressed == 0) {
    delay(5);
    analogButtons.check();
    if(colorButtonPressed != 0) {
      return colorButtonPressed;
    }
  }
}
