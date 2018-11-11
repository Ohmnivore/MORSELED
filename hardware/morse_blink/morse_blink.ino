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
const morse_letter MORSE_EXT_POLL = 39;

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

bool letterIsPoll(morse_letter letter) {
  return letter == MORSE_EXT_POLL;
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

  T peek() const {
    return buff[tail];
  }

  void push(T e) {
    buff[head] = e;
    
    head++;
    head = head % capacity;
    count++;
  }

  int getCapacity() const {
    return capacity;
  }

  int getCount() const {
    return count;
  }

  int getFreeCount() const {
    return capacity - count;
  }

  bool isEmpty() const {
    return count == 0;
  }

  bool isFull() const {
    return count == capacity;
  }

private:

  int capacity = CAPACITY;
  int count = 0;
  T buff[CAPACITY];

  int head = 0;
  int tail = 0;
};


typedef unsigned long timestamp;

// The "on" field could be removed by using the alternating property of Morse code
// (elements with an odd index in a sequence are "on", even are "off").
// It could be used to save memory at the cost of code complexity.
struct TimingRecord {

  TimingRecord() {
    
  }

  TimingRecord(timestamp stamp, bool on) {
    this->stamp = stamp;
    this->on = on;
  }

  void debugPrint() const {
    Serial.println(stamp);
    Serial.println(on);
    Serial.println("");
  }
  
  timestamp stamp;
  bool on;
};


///////////////////////////////////////////////////////////////////////////////
// Main

// Debug flags
#define DEBUG_INITIAL_POPULATE 0      // Pushes 'SOS' to incoming buffer on init
#define DEBUG_SERIAL_TEST_SEND 0      // Prints 'Hello!' every loop execution
#define DEBUG_SERIAL_TEST_LOOPBACK 0  // Prints back received input every loop execution
#define DEBUG_PLANNING 0              // Prints each timing record as soon it's computed (watch out - can add latencies of >100ms)

// Config
const byte PIN_LED = 11;
const byte PIN_BUZZER = 12;
const byte MAX_TIMING_RECORDS_PER_LETTER = 8;
const byte MIN_DATA_CHUNK_SIZE = 8;
CircularBuffer<16, morse_letter> incoming;
CircularBuffer<32, TimingRecord> planned;

bool notify = false;
timestamp scheduleHead;

void morseON() {
  digitalWrite(PIN_LED, HIGH);
  const int hertz = 500;
  tone(PIN_BUZZER, hertz);
}

void morseOFF() {
  digitalWrite(PIN_LED, LOW);
  noTone(PIN_BUZZER);
}

void schedule(bool on, unsigned long delay) {
  TimingRecord rec = TimingRecord(scheduleHead, on);
  planned.push(rec);
  scheduleHead += delay;

  #if DEBUG_PLANNING
  rec.debugPrint();
  #endif
}

// For debugging
void serialTest() {
  #if DEBUG_SERIAL_TEST_SEND
  Serial.println("Hello!");
  #endif

  #if DEBUG_SERIAL_TEST_LOOPBACK
  if (Serial.available() > 0) {
    byte in = Serial.read();
    Serial.println(in);
  }
  #endif
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

  Serial.setTimeout(1);
  Serial.begin(9600);

  #if DEBUG_INITIAL_POPULATE
  populateTestText();
  #endif
}

void loop() {
  #if DEBUG_SERIAL_TEST_SEND || DEBUG_SERIAL_TEST_LOOPBACK
  serialTest();
  #endif


  // Get current timestamp, adjust schedule if it's behind
  timestamp currentTime = millis();
  if (scheduleHead < currentTime)
    scheduleHead = currentTime;


  // Read schedule, change output status if necessary
  if (!planned.isEmpty()) {
    TimingRecord scheduled = planned.peek();

    if (scheduled.stamp <= currentTime) {
      if (scheduled.on) {
        morseON();
      }
      else {
        morseOFF();
      }
      
      planned.pop();
    }
  }


  // Plan schedule:
  // We need at least two letters in the buffer to distinguish
  // letter->letter vs word->word contexts
  // Also, MORSE_EXT_END exists to be used as a terminator for this reason
  while (incoming.getCount() >= 2 && planned.getFreeCount() >= MAX_TIMING_RECORDS_PER_LETTER) {
    const morse_letter letter = incoming.pop();

    // Whitespace (word separator)
    if (letterIsWhitespace(letter)) {
      schedule(false, MORSE_WORD_SEP);
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
        // Check if dot or dash
        morse_timing timeON = MORSE_DASH;
        if ((patternScratchPad & (1 << 7)) == 0) {
          timeON = MORSE_DOT;
        }
        patternScratchPad <<= 1;

        schedule(true, timeON);

        // If we're not dealing with the last bit of the pattern
        if (componentIdx < patternLength - 1) {
          schedule(false, MORSE_SILENCE);
        }
      }
    }
    // Invalid range, ignore
    else {
      
    }

    // If we transitioned from a letter to another letter
    const morse_letter nextChar = incoming.peek();
    if (letterIsInAlphabet(nextChar)) {
      schedule(false, MORSE_LETTER_SEP);
    }
    else if (letterIsEnd(nextChar)) {
      incoming.pop();

      // Treat as word separator
      schedule(false, MORSE_WORD_SEP);
    }
  }


  // Read input from driver
  if (Serial.available() > 0) {
    byte in = Serial.read();
    morse_letter converted = static_cast<morse_letter>(in);

    // Cancel everything and empty the buffer
    if (letterIsCancel(converted)) {
      morseOFF();
      incoming.clear();
      planned.clear();
      scheduleHead = 0; // Will be set to current time when needed
    }
    else if (letterIsPoll(converted)) {
      notify = true;
    }
    else {
      incoming.push(in);
    }
  }


  // Notify driver of free space in buffer
  if (notify && (incoming.getFreeCount() >= MIN_DATA_CHUNK_SIZE)) {
    Serial.write(incoming.getFreeCount());
    notify = false;
  }
}
///////////////////////////////////////////////////////////////////////////////
