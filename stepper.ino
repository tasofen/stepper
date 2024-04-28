// Два средних поменяны местыми. Неправильная распайка
int8_t motorMode[] = {
  0b1000,
  0b1010,
  0b0010,
  0b0110,
  0b0100,
  0b0101,
  0b0001,
  0b1001,
};


#define BTN_SLEEP 200

#define SPEED_ITERATION 10
uint16_t sleep = 100;
uint32_t lastChangeSpeed = 0;
uint32_t lastReverseTimeChange = 0;

#define REVERS_ITERATION_TIME 200000
uint32_t lastChangeAutoReverse = 0;
bool autoReverse = false;
uint8_t reverse = 1;

int8_t currentIndex = 0;

void setup() {
  pinMode(8, OUTPUT);
  pinMode(9, OUTPUT);
  pinMode(10, OUTPUT);
  pinMode(11, OUTPUT);
  
  pinMode(13, OUTPUT);

  pinMode(4, INPUT);
  pinMode(5, INPUT);
  pinMode(6, INPUT);
  pinMode(7, INPUT);
  digitalWrite(4, HIGH);
  digitalWrite(5, HIGH);
  digitalWrite(6, HIGH);
  digitalWrite(7, HIGH);
  Serial.begin(9600);
}


void sleepAndInput() {
  uint32_t start = millis();

  if (autoReverse && millis() - lastReverseTimeChange > REVERS_ITERATION_TIME) {
    reverse = (reverse + 1) % 2;
    lastReverseTimeChange = millis();
  }

  do {
    if (digitalRead(4)==0 && millis()-lastChangeSpeed > BTN_SLEEP) {
      lastChangeSpeed = millis();
      sleep += SPEED_ITERATION;
    } else if (digitalRead(5)==0 && millis()-lastChangeSpeed > BTN_SLEEP) {
      lastChangeSpeed = millis();
      sleep -= SPEED_ITERATION;
    } else if (digitalRead(6)==0) {
      reverse = (reverse + 1) % 2;
      delay(500);
      while(digitalRead(6)==0);
      delay(500);
    } else if (digitalRead(7)==0 && millis()-lastChangeAutoReverse > 1000) {
        autoReverse = !autoReverse;
        lastChangeAutoReverse = millis();
        digitalWrite(13, (int)autoReverse);
    }    
  } while(millis() < start + sleep);
}


void loop() {
  if (reverse) {
    currentIndex++;
  } else {
    currentIndex--;
  }

  if (currentIndex < 0) {
    currentIndex = 7;
  } else if (currentIndex > 7) {
    currentIndex = 0;
  }

  PORTB = (PORTB & 0b11110000 | motorMode[currentIndex]);

  sleepAndInput();
}
