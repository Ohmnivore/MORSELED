// Credit to Wikipedia for morse code timings and alphabet:
// https://en.wikipedia.org/wiki/Morse_code


// Morse code timings (in milliseconds)
typedef unsigned int morse_timing;
const morse_timing MORSE_DOT = 100;                   // Base unit, modify to affect the overall output speed. Component of a letter's pattern.
const morse_timing MORSE_DASH = MORSE_DOT * 3;        // Component of a letter's pattern.
const morse_timing MORSE_SILENCE = MORSE_DOT;         // One silence between every dot and dash in a letter
const morse_timing MORSE_LETTER_SEP = MORSE_DASH;     // Between every letter
const morse_timing MORSE_WORD_SEP = MORSE_DOT * 7;    // Between every word


///////////////////////////////////////////////////////////////////////////////
// Morse code alphabet, generated using hardware/alphabet_gen/alphabet_gen.py

// Contains pattern length, then pattern
// Can probably obtain the pattern length at compile-time with some effort
const int MORSE_LETTER_A[] = { 2, MORSE_DOT, MORSE_DASH };
const int MORSE_LETTER_B[] = { 4, MORSE_DASH, MORSE_DOT, MORSE_DOT, MORSE_DOT };
const int MORSE_LETTER_C[] = { 4, MORSE_DASH, MORSE_DOT, MORSE_DASH, MORSE_DOT };
const int MORSE_LETTER_D[] = { 3, MORSE_DASH, MORSE_DOT, MORSE_DOT };
const int MORSE_LETTER_E[] = { 1, MORSE_DOT };
const int MORSE_LETTER_F[] = { 4, MORSE_DOT, MORSE_DOT, MORSE_DASH, MORSE_DOT };
const int MORSE_LETTER_G[] = { 3, MORSE_DASH, MORSE_DASH, MORSE_DOT };
const int MORSE_LETTER_H[] = { 4, MORSE_DOT, MORSE_DOT, MORSE_DOT, MORSE_DOT };
const int MORSE_LETTER_I[] = { 2, MORSE_DOT, MORSE_DOT };
const int MORSE_LETTER_J[] = { 4, MORSE_DOT, MORSE_DASH, MORSE_DASH, MORSE_DASH };
const int MORSE_LETTER_K[] = { 3, MORSE_DASH, MORSE_DOT, MORSE_DASH };
const int MORSE_LETTER_L[] = { 4, MORSE_DOT, MORSE_DASH, MORSE_DOT, MORSE_DOT };
const int MORSE_LETTER_M[] = { 2, MORSE_DASH, MORSE_DASH };
const int MORSE_LETTER_N[] = { 2, MORSE_DASH, MORSE_DOT };
const int MORSE_LETTER_O[] = { 3, MORSE_DASH, MORSE_DASH, MORSE_DASH };
const int MORSE_LETTER_P[] = { 4, MORSE_DOT, MORSE_DASH, MORSE_DASH, MORSE_DOT };
const int MORSE_LETTER_Q[] = { 4, MORSE_DASH, MORSE_DASH, MORSE_DOT, MORSE_DASH };
const int MORSE_LETTER_R[] = { 3, MORSE_DOT, MORSE_DASH, MORSE_DOT };
const int MORSE_LETTER_S[] = { 3, MORSE_DOT, MORSE_DOT, MORSE_DOT };
const int MORSE_LETTER_T[] = { 1, MORSE_DASH };
const int MORSE_LETTER_U[] = { 3, MORSE_DOT, MORSE_DOT, MORSE_DASH };
const int MORSE_LETTER_V[] = { 4, MORSE_DOT, MORSE_DOT, MORSE_DOT, MORSE_DASH };
const int MORSE_LETTER_W[] = { 3, MORSE_DOT, MORSE_DASH, MORSE_DASH };
const int MORSE_LETTER_X[] = { 4, MORSE_DASH, MORSE_DOT, MORSE_DOT, MORSE_DASH };
const int MORSE_LETTER_Y[] = { 4, MORSE_DASH, MORSE_DOT, MORSE_DASH, MORSE_DASH };
const int MORSE_LETTER_Z[] = { 4, MORSE_DASH, MORSE_DASH, MORSE_DOT, MORSE_DOT };
const int MORSE_LETTER_0[] = { 5, MORSE_DASH, MORSE_DASH, MORSE_DASH, MORSE_DASH, MORSE_DASH };
const int MORSE_LETTER_1[] = { 5, MORSE_DOT, MORSE_DASH, MORSE_DASH, MORSE_DASH, MORSE_DASH };
const int MORSE_LETTER_2[] = { 5, MORSE_DOT, MORSE_DOT, MORSE_DASH, MORSE_DASH, MORSE_DASH };
const int MORSE_LETTER_3[] = { 5, MORSE_DOT, MORSE_DOT, MORSE_DOT, MORSE_DASH, MORSE_DASH };
const int MORSE_LETTER_4[] = { 5, MORSE_DOT, MORSE_DOT, MORSE_DOT, MORSE_DOT, MORSE_DASH };
const int MORSE_LETTER_5[] = { 5, MORSE_DOT, MORSE_DOT, MORSE_DOT, MORSE_DOT, MORSE_DOT };
const int MORSE_LETTER_6[] = { 5, MORSE_DASH, MORSE_DOT, MORSE_DOT, MORSE_DOT, MORSE_DOT };
const int MORSE_LETTER_7[] = { 5, MORSE_DASH, MORSE_DASH, MORSE_DOT, MORSE_DOT, MORSE_DOT };
const int MORSE_LETTER_8[] = { 5, MORSE_DASH, MORSE_DASH, MORSE_DASH, MORSE_DOT, MORSE_DOT };
const int MORSE_LETTER_9[] = { 5, MORSE_DASH, MORSE_DASH, MORSE_DASH, MORSE_DASH, MORSE_DOT };

const int* MORSE_ALPHABET[] = {
  MORSE_LETTER_A, MORSE_LETTER_B, MORSE_LETTER_C, MORSE_LETTER_D,
  MORSE_LETTER_E, MORSE_LETTER_F, MORSE_LETTER_G, MORSE_LETTER_H,
  MORSE_LETTER_I, MORSE_LETTER_J, MORSE_LETTER_K, MORSE_LETTER_L,
  MORSE_LETTER_M, MORSE_LETTER_N, MORSE_LETTER_O, MORSE_LETTER_P,
  MORSE_LETTER_Q, MORSE_LETTER_R, MORSE_LETTER_S, MORSE_LETTER_T,
  MORSE_LETTER_U, MORSE_LETTER_V, MORSE_LETTER_W, MORSE_LETTER_X,
  MORSE_LETTER_Y, MORSE_LETTER_Z,

  MORSE_LETTER_0, MORSE_LETTER_1, MORSE_LETTER_2, MORSE_LETTER_3,
  MORSE_LETTER_4, MORSE_LETTER_5, MORSE_LETTER_6, MORSE_LETTER_7,
  MORSE_LETTER_8, MORSE_LETTER_9
};
const int MORSE_WHITESPACE = 36;

bool letterIsInAlphabet(int letter) {
  return letter >= 0 && letter < MORSE_WHITESPACE;
}

bool letterIsWhitespace(int letter) {
  return letter == MORSE_WHITESPACE;
}

///////////////////////////////////////////////////////////////////////////////


//template <int CAPACITY, typename T>
//struct CircularBuffer {
//  int capacity = CAPACITY;
//  T buff[SIZE];
//
//  
//};


///////////////////////////////////////////////////////////////////////////////
// Main

// Config
const int PIN_LED = 11;
const int PIN_BUZZER = 12;
const int BUFFER_SIZE = 3;
int BUFFER[BUFFER_SIZE] = {18, 14, 18};
int curCharIdx = 0;

void morseON() {
  digitalWrite(PIN_LED, HIGH);
  tone(PIN_BUZZER, 500);
}

void morseOFF() {
  digitalWrite(PIN_LED, LOW);
  noTone(PIN_BUZZER);
}

void setup() {
  pinMode(PIN_LED, OUTPUT);
}

void loop() {
  if (curCharIdx < BUFFER_SIZE) {
    const int curChar = BUFFER[curCharIdx];

    // Whitespace
    if (letterIsWhitespace(curChar)) {
      morseOFF();
      delay(MORSE_WORD_SEP);
    }
    // Letter
    else if (letterIsInAlphabet(curChar)) {
      const int* pattern = MORSE_ALPHABET[curChar];
      int patternLength = pattern[0];

      for (int componentIdx = 0; componentIdx < patternLength; ++componentIdx) {
        morseON();
        int timeBlink = pattern[componentIdx + 1];
        delay(timeBlink);

        // If we transitioned from a component to another component
        if (componentIdx < patternLength - 1) {
          morseOFF();
          delay(MORSE_SILENCE);
        }
      }
    }
    // Invalid range, ignore
    else {
      
    }
    
    curCharIdx++;

    // If we transitioned from a letter to another letter
    const int nextChar = BUFFER[curCharIdx];
    if (letterIsInAlphabet(nextChar)) {
      morseOFF();
      delay(MORSE_LETTER_SEP);
    }

    morseOFF();
  }
}
///////////////////////////////////////////////////////////////////////////////
