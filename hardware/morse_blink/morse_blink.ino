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

typedef byte morse_pattern;

// Contains pattern length, then pattern
// Can probably obtain the pattern length at compile-time with some effort
const morse_pattern MORSE_LETTER_A = 0b00000101;
const morse_pattern MORSE_LETTER_B = 0b00011000;
const morse_pattern MORSE_LETTER_C = 0b00011010;
const morse_pattern MORSE_LETTER_D = 0b00001100;
const morse_pattern MORSE_LETTER_E = 0b00000010;
const morse_pattern MORSE_LETTER_F = 0b00010010;
const morse_pattern MORSE_LETTER_G = 0b00001110;
const morse_pattern MORSE_LETTER_H = 0b00010000;
const morse_pattern MORSE_LETTER_I = 0b00000100;
const morse_pattern MORSE_LETTER_J = 0b00010111;
const morse_pattern MORSE_LETTER_K = 0b00001101;
const morse_pattern MORSE_LETTER_L = 0b00010100;
const morse_pattern MORSE_LETTER_M = 0b00000111;
const morse_pattern MORSE_LETTER_N = 0b00000110;
const morse_pattern MORSE_LETTER_O = 0b00001111;
const morse_pattern MORSE_LETTER_P = 0b00010110;
const morse_pattern MORSE_LETTER_Q = 0b00011101;
const morse_pattern MORSE_LETTER_R = 0b00001010;
const morse_pattern MORSE_LETTER_S = 0b00001000;
const morse_pattern MORSE_LETTER_T = 0b00000011;
const morse_pattern MORSE_LETTER_U = 0b00001001;
const morse_pattern MORSE_LETTER_V = 0b00010001;
const morse_pattern MORSE_LETTER_W = 0b00001011;
const morse_pattern MORSE_LETTER_X = 0b00011001;
const morse_pattern MORSE_LETTER_Y = 0b00011011;
const morse_pattern MORSE_LETTER_Z = 0b00011100;
const morse_pattern MORSE_LETTER_0 = 0b00111111;
const morse_pattern MORSE_LETTER_1 = 0b00101111;
const morse_pattern MORSE_LETTER_2 = 0b00100111;
const morse_pattern MORSE_LETTER_3 = 0b00100011;
const morse_pattern MORSE_LETTER_4 = 0b00100001;
const morse_pattern MORSE_LETTER_5 = 0b00100000;
const morse_pattern MORSE_LETTER_6 = 0b00110000;
const morse_pattern MORSE_LETTER_7 = 0b00111000;
const morse_pattern MORSE_LETTER_8 = 0b00111100;
const morse_pattern MORSE_LETTER_9 = 0b00111110;

typedef byte morse_letter;

const morse_pattern MORSE_ALPHABET[] = {
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
const morse_letter MORSE_EXT_WHITESPACE = 36;
const morse_letter MORSE_EXT_END = 37;
const morse_letter MORSE_EXT_CANCEL = 38;

bool letterIsInAlphabet(morse_letter letter) {
  return letter >= 0 && letter < MORSE_EXT_WHITESPACE;
}

bool letterIsWhitespace(morse_letter letter) {
  return letter == MORSE_EXT_WHITESPACE;
}

bool letterIsEnd(morse_letter letter) {
  return letter == MORSE_EXT_END;
}

bool letterIsCancel(morse_letter letter) {
  return letter == MORSE_EXT_CANCEL;
}

///////////////////////////////////////////////////////////////////////////////


template <int CAPACITY, typename T>
struct CircularBuffer {

  void clear() {
    count = 0;
    head = 0;
    tail = 0;
  }

  T pop() {
    T cop = buff[tail];
    tail++;
    tail = tail % capacity;
    count--;
    
    return cop;
  }

  T peek() {
    return buff[tail];
  }

  void push(T e) {
    buff[head] = e;
    
    head++;
    head = head % capacity;
    count++;
  }

  int getCapacity() {
    return capacity;
  }

  int getCount() {
    return count;
  }

  int getFreeCount() {
    return capacity - count;
  }

  bool isEmpty() {
    return count == 0;
  }

private:

  int capacity = CAPACITY;
  int count = 0;
  T buff[CAPACITY];

  int head = 0;
  int tail = 0;
};


///////////////////////////////////////////////////////////////////////////////
// Main

// Config
const byte PIN_LED = 11;
const byte PIN_BUZZER = 12;
CircularBuffer<32, morse_letter> incoming;

void morseON() {
  digitalWrite(PIN_LED, HIGH);
  tone(PIN_BUZZER, 500);
}

void morseOFF() {
  digitalWrite(PIN_LED, LOW);
  noTone(PIN_BUZZER);
}

// For debugging
void serialTest() {
  //Serial.println("Hello!");

  if (Serial.available() > 0) {
    byte in = Serial.read();
    Serial.println(in);
  }
}

// For debugging
void populateTestText() {
  // Translates to SOS
  incoming.push(18);
  incoming.push(14);
  incoming.push(18);
  incoming.push(MORSE_EXT_END);
}

void setup() {
  pinMode(PIN_LED, OUTPUT);
  pinMode(PIN_BUZZER, OUTPUT);

  Serial.setTimeout(50);
  Serial.begin(9600);

//  populateTestText();
}

void loop() {
//  serialTest();
  
  if (Serial.available() > 0) {
    byte in = Serial.read();
    morse_letter converted = static_cast<morse_letter>(in);

    // Cancel everything and empty the buffer
    if (letterIsCancel(converted)) {
      morseOFF();
      incoming.clear();
      Serial.println("cleared");
    }
    else {
      incoming.push(in);
    }
  }
  
  if (incoming.getCount() >= 2) {
    const morse_letter letter = incoming.pop();

    // Whitespace (word separator)
    if (letterIsWhitespace(letter)) {
      morseOFF();
      delay(MORSE_WORD_SEP);
    }
    // Letter
    else if (letterIsInAlphabet(letter)) {
      morse_pattern patternScratchPad = MORSE_ALPHABET[letter];
//      Serial.println(patternScratchPad, BIN);

      // Remove padding and leading 1 from the pattern
      char paddingLength = 1;
      while ((patternScratchPad & (1 << 7)) == 0) {
        patternScratchPad <<= 1;
        paddingLength++;
      }
      patternScratchPad <<= 1;
      char patternLength = 8 - paddingLength;
//      Serial.println(patternScratchPad, BIN);
//      Serial.println(paddingLength);
//      Serial.println(patternLength);
//      Serial.println("");

      // For every bit in the pattern
      for (char componentIdx = 0; componentIdx < patternLength; ++componentIdx) {
        morseON();

        // Check if dot or dash
        morse_timing timeON = MORSE_DASH;
        if ((patternScratchPad & (1 << 7)) == 0) {
          timeON = MORSE_DOT;
        }
        patternScratchPad <<= 1;
        delay(timeON);

        // If we're not dealing with the last bit of the pattern
        if (componentIdx < patternLength - 1) {
          morseOFF();
          delay(MORSE_SILENCE);
        }
      }
    }
    // Invalid range, ignore
    else {
      
    }

    // If we transitioned from a letter to another letter
    const morse_letter nextChar = incoming.peek();
    if (letterIsInAlphabet(nextChar)) {
      morseOFF();
      delay(MORSE_LETTER_SEP);
    }

    morseOFF();
  }
}
///////////////////////////////////////////////////////////////////////////////
