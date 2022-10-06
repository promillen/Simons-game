//#include <LiquidCrystal_I2C.h> // I2C address: 0x27

// set the LCD number of columns and rows
/*int lcdColumns = 16;
int lcdRows = 2;*/

// set LCD address, number of columns and rows
// if you don't know your display address, run an I2C scanner sketch
//LiquidCrystal_I2C lcd(0x27, lcdColumns, lcdRows);

//Pin constants
const int redLed = D5;
const int yellowLed = D6;
const int greenLed = D7;
const int blueLed = D8;

/*const int redLedButton = x;
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
  Serial.begin(115200);
  pinMode(redLed, OUTPUT);
  pinMode(yellowLed, OUTPUT);
  pinMode(greenLed, OUTPUT);
  pinMode(blueLed, OUTPUT);
  
}

void loop() {
  // put your main code here, to run repeatedly:
  //Initialize array
  resetLevels();
  serialFlush();
  Serial.println("Choose gamemode");
  while(Serial.available() == 0) {
  }
  int gamemode = Serial.read();
  Serial.print("Gamemode ");
  
  Serial.print(gamemode);
  Serial.println(" chosen");
  delay(250);
  
  if(gamemode == 49) { //ASCII character for 1
    setupGamemode1();
  } else if(gamemode == 2) {
    return;
  } else if(gamemode == 3) {
    return;
  } else {
    return;
  }

  runLevels();
  
}

void setupGamemode1() {
  Serial.println("Choose difficulty");
  serialFlush();

  while(Serial.available() == 0) {
  }
  int difficulty = Serial.read();
  delay(250);
  if(difficulty == 49) {
    Serial.print("You have choosen difficulty: ");
    Serial.println(1);
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

void resetLevels() {
    for(int i = 0; i<31; i++) {
    if (levels[i] == -1) {
      break;
    } else {
    levels[i] = -1;
    }
  }
}

void generateLevels(int x) {
  for(int i = 0; i<skillLevel[x-1]; i++) {
    levels[i] = random(1,5);
    //Serial.print(levels[i]);
  }
  //Serial.println();
  for(int i = 0; i<31; i++) {
    Serial.print(levels[i]);
    Serial.print(", ");
  }
  Serial.println();
}

void runLevels() {
    for (int i = 0; i < 31; i++) {
    if (levels[i] == 1) {
      Serial.println("rød");
      digitalWrite(redLed, HIGH);
      delay(500);
      digitalWrite(redLed, LOW);
      delay(500);
    } else if (levels[i] == 2) {
      Serial.println("gul");
      digitalWrite(yellowLed, HIGH);
      delay(500);
      digitalWrite(yellowLed, LOW);
      delay(500);
    } else if (levels[i] == 3) {
      Serial.println("grøn");
      digitalWrite(greenLed, HIGH);
      delay(500);
      digitalWrite(greenLed, LOW);
      delay(500);
    } else if (levels[i] == 4) {
      Serial.println("blå");
      digitalWrite(blueLed, HIGH);
      delay(500);
      digitalWrite(blueLed, LOW);
      delay(500);
    } else if (levels[i] == -1) {
      Serial.println("done");
      break;
    }
  }
}

void serialFlush(){
  while(Serial.available() > 0) {
    char t = Serial.read();
  }
}
