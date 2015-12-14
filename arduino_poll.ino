#define SERIAL_NUMBER "s0"

#define SENSOR_PIN1 A0
#define SENSOR_PIN2 A2
#define LEFT_BREAK_BUTTON_PIN 5
#define RIGHT_BREAK_BUTTON_PIN 6
#define LEFT_GEAR_BUTTON_PIN 7
#define RIGHT_GEAR_BUTTON_PIN 8
#define LEFT_TURN_BUTTON_PIN 9
#define RIGHT_TURN_BUTTON_PIN 10
#define THRESHOLD 500
#define BUTTONS_ACTIVE LOW

#define N_BUTTONS 6

boolean last = false; // low - false, high - true
unsigned int counter = 0;

int buttons[] = {LEFT_BREAK_BUTTON_PIN, RIGHT_BREAK_BUTTON_PIN, LEFT_GEAR_BUTTON_PIN, RIGHT_GEAR_BUTTON_PIN, LEFT_TURN_BUTTON_PIN, RIGHT_TURN_BUTTON_PIN};
bool button_states[] = {false, false, false, false, false, false};

bool seq[4][2] = {{false, false}, {true, false}, {true, true}, {false, true}}; // low = false, high = true
int iSeq = 0; // Used to keep track of which values to expect

String msg;

void setup() {
  for (int i = 0; i < N_BUTTONS; i++) {
    pinMode(buttons[i], INPUT);
    digitalWrite(buttons[i], HIGH);
  }
  Serial.begin(9600);
  msg = String();
}

void loop() {
  // Buttons
  for (int i = 0; i < N_BUTTONS; i++) {
    if (digitalRead(buttons[i]) == BUTTONS_ACTIVE) {
      button_states[i] = true;
    }
  }
  
  // IR sensors
  bool val1 = analogRead(SENSOR_PIN1) > THRESHOLD;
  bool val2 = analogRead(SENSOR_PIN2) > THRESHOLD;
  if (seq[iSeq][0] == val1 && seq[iSeq][1] == val2) {
    ++counter;
    iSeq = (iSeq + 1) % 4;
  }
  
  // Serial com
  if (Serial.available() >= 1) {
    int rcvd = Serial.read();
    msg = "";
    if (rcvd == 's') {
      msg += "serial=";
      msg += SERIAL_NUMBER;
    }
    else {
      for (int i = 0; i < N_BUTTONS; i++) {
        msg += button_states[i];
        button_states[i] = false;
      }
      msg += counter;
    }
    Serial.println(msg);
    // Just clear the buffer
    while (Serial.read() != -1);
    // Reset counter
    counter = 0;
  }
  
//  delay(2);
}
