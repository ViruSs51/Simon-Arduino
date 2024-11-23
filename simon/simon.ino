#define maxLevel 30

const int YLED = 2; //Yellow led
const int BLED = 3; //Blue led 
const int GLED = 4; //Green led
const int RLED = 5; //Red led
const int BTN_BUFFER = 6; //Button for buffer
const int BUFFER = 7;

int* p_leds[] = {&RLED, &GLED, &BLED, &YLED}; //Pointers array this pin of leds
const int SIZE = sizeof(p_leds) / sizeof(p_leds[0]); //Size of pointers array

int tone_leds[SIZE] = {1, 2, 3, 4};
bool btn_buffer_active = true; //Status of buffer button
const int BTN_BUFFER_MAX_DELAY = 35; //Maximal delay for function for buffer button
int btn_buffer_delay = BTN_BUFFER_MAX_DELAY; //Delay for function for buffer button

int level = 1; //User level in game
bool win = false;
bool waitInput = false;
int* activated_leds = new int[maxLevel]; //Array of ordonations activated leds in levels


//Animation with leds for start game and restart
void startGame(bool btn_buffer_active, int tone_leds[], int buffer) {
  activateLed(RLED, 100, 0, btn_buffer_active, tone_leds, buffer);
  activateLed(GLED, 100, 0, btn_buffer_active, tone_leds, buffer);
  activateLed(BLED, 100, 0, btn_buffer_active, tone_leds, buffer);
  activateLed(YLED, 100, 0, btn_buffer_active, tone_leds, buffer);

  activateLed(BLED, 100, 0, btn_buffer_active, tone_leds, buffer);
  activateLed(GLED, 100, 0, btn_buffer_active, tone_leds, buffer);
  activateLed(RLED, 100, 100, btn_buffer_active, tone_leds, buffer);

  activateLed(GLED, 100, 0, btn_buffer_active, tone_leds, buffer);
  activateTLed(GLED, BLED, 2000, 0, btn_buffer_active, tone_leds, buffer);

  activateTLed(RLED, YLED, 300, 0, btn_buffer_active, tone_leds, buffer);
  activateTLed(GLED, BLED, 300, 0, btn_buffer_active, tone_leds, buffer);

  activateTLed(RLED, YLED, 500, 1000, btn_buffer_active, tone_leds, buffer);
}

//Initialize a functional for buffer button
void bufferButton(int BTN, bool* btn_status, int* btn_delay, int btn_max_delay) {
  if (digitalRead(BTN) && *btn_delay >= btn_max_delay) {
    if (*btn_status) *btn_status = false;
    else *btn_status = true;

    *btn_delay = 0;
  }

  *btn_delay++;
}

//Filling another array with 0
void fillArray(int* array, int size) {
  for (int i=0; i<size; i++) {
    array[i] = 0;
  }
}

//Active led for time1 in ms and deactive led for time2 in ms
void activateLed(int LED, int time1, int time2, bool btn_buffer_active, int tone_leds[], int buffer) {
  if (btn_buffer_active) tone(buffer, tone_leds[LED-2]);
  digitalWrite(LED, 1);
  delay(time1);

  digitalWrite(LED, 0);
  noTone(buffer);
  delay(time2);
}

//Active two led for time1 in ms and deactive two led for time2 in ms
void activateTLed(int LED1, int LED2, int time1, int time2, bool btn_buffer_active, int tone_leds[], int buffer) {
  if (btn_buffer_active) tone(buffer, tone_leds[LED1-2]);
  digitalWrite(LED1, 1);
  if (btn_buffer_active) tone(buffer, tone_leds[LED2-2]);
  digitalWrite(LED2, 1);
  delay(time1);

  digitalWrite(LED1, 0);
  digitalWrite(LED2, 0);
  noTone(buffer);
  delay(time2);
}

//Active a random led and return led index in pointers array
int randomActivateLed(int* leds[], int size, int time1, int time2, bool btn_buffer_active, int tone_leds[], int buffer) {
  int index_led = random(size);

  activateLed(*leds[index_led], time1, time2, btn_buffer_active, tone_leds, buffer);

  return index_led;
}

//Generate a new level considere did levels in a game
void generateLevel(int* leds[], int size, int* activated_leds, int level, bool *waitInput, bool btn_buffer_active, int tone_leds[], int buffer) {
  int time1 = 1000;
  int time2 = 100;

  for (int i=0; i<level; i++) {
    if (i >= level-1) {
      activated_leds[i] = randomActivateLed(leds, size, time1, time2, btn_buffer_active, tone_leds, buffer);
    } else {
      activateLed(*leds[activated_leds[i]], time1, time2, btn_buffer_active, tone_leds, buffer);
    }
  }

  *waitInput = true;
}

//Reset for begin game all parameters and show animation for game over
void gameOver(int* leds[], int size, bool* waitInput, bool* win, int* level, int* activated_leds, bool btn_buffer_active, int tone_leds[], int buffer) {
  int restart = false;

  *waitInput = false;
  *win = false;
  *level = 1;

  fillArray(activated_leds, maxLevel);

  for (int i=0; i<15; i++) {
    for (int j=0; j<size; j++) {
      if (btn_buffer_active) tone(buffer, -tone_leds[j]);
      digitalWrite(*leds[j], 1);
    }
    delay(100);

    noTone(buffer);
    for (int j=0; j<size; j++) {
      if (btn_buffer_active) tone(buffer, -tone_leds[j]);
      digitalWrite(*leds[j], 0);
    }
    delay(100);

    noTone(buffer);
  }

  while (not restart) {
    for (int i=0; i<size; i++) {
      if (digitalRead(*leds[i])) {
        if (btn_buffer_active) tone(buffer, tone_leds[i]);
        restart = true;
        delay(200);
        
        noTone(buffer);
      }
    }
  }

  startGame(btn_buffer_active, tone_leds, buffer);
}

//Update game parameters and show animation win for actual level
void actualLevelWin(int* leds[], int size, int* level, bool* waitInput, bool btn_buffer_active, int tone_leds[], int buffer) {
  *level += 1;
  *waitInput = false;

  for (int i=0; i<3; i++) {
    if (btn_buffer_active) tone(buffer, tone_leds[0]);
    digitalWrite(*leds[0], 1);
    if (btn_buffer_active) tone(buffer, tone_leds[3]);
    digitalWrite(*leds[3], 1);
    delay(400);

    digitalWrite(*leds[0], 0);
    digitalWrite(*leds[3], 0);
    noTone(buffer);
    delay(200);
  }
}

//This function wait a user input at four push buttons and verify if input is coincide with leds in ordonation
void waitingInput(int* leds[], int size, int* activated_leds, int* level, bool* waitInput, bool* win, bool btn_buffer_active, int tone_leds[], int buffer) {
  int pressButton = 0;

  for (int i=0; i<*level; i++) {
    int pressedLed = activated_leds[i];
    pressButton = 0;

    while (pressButton != 1) {
      for (int j=0; j<size; j++) {
        int press_button = digitalRead(*leds[j]);

        if (press_button && j == pressedLed) {
          if (btn_buffer_active) tone(buffer, tone_leds[j]);
          pressButton = 1;
        } else if (press_button && j != pressedLed) {
          if (btn_buffer_active) tone(buffer, -tone_leds[j]);
          pressButton = -1;
          break;
        }
      }

      if (pressButton == 1) {
        break;
      } else if (pressButton == -1) {
        break;
      }
    }

    if (pressButton == -1) {
      gameOver(leds, size, waitInput, win, level, activated_leds, btn_buffer_active, tone_leds, buffer);
    }
    delay(600);
    noTone(buffer);
  }

  if (pressButton == 1) {
    actualLevelWin(leds, size, level, waitInput, btn_buffer_active, tone_leds, buffer);
  }
}


void setup() {
  //Set a seed for a random generator
  int seed = analogRead(A0);
  randomSeed(seed);

  fillArray(activated_leds, maxLevel);

  Serial.begin(9600);
  pinMode(YLED, OUTPUT);
  pinMode(BLED, OUTPUT);
  pinMode(GLED, OUTPUT);
  pinMode(RLED, OUTPUT);
  pinMode(BTN_BUFFER, INPUT);
  pinMode(BUFFER, OUTPUT);

  startGame(btn_buffer_active, tone_leds, BUFFER);
}


void loop() {
  bufferButton(BTN_BUFFER, &btn_buffer_active, &btn_buffer_delay, BTN_BUFFER_MAX_DELAY);

  if (win) {
    bool restart = false;

    actualLevelWin(p_leds, SIZE, &level, &waitInput, btn_buffer_active, tone_leds, BUFFER);

    while (not restart) {
      for (int i=0; i<SIZE; i++) {
        if (digitalRead(*p_leds[i])) {
          if (btn_buffer_active) tone(BUFFER, tone_leds[i]);
          restart = true;
          delay(200);

          noTone(BUFFER);
        }
      }
    }

    waitInput = false;
    win = false;
    level = 1;

    startGame(btn_buffer_active, tone_leds, BUFFER);
  } else if (level <= maxLevel && not win && not waitInput) {
    generateLevel(p_leds, SIZE, activated_leds, level, &waitInput, btn_buffer_active, tone_leds, BUFFER);
  } else if (waitInput && not win) {
    waitingInput(p_leds, SIZE, activated_leds, &level, &waitInput, &win, btn_buffer_active, tone_leds, BUFFER);
  } else {
    startGame(btn_buffer_active, tone_leds, BUFFER);
  }

  if (level == maxLevel) win = true;
}