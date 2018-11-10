This project contains a web application for sending text to an Arduino device which then outputs it in Morse code (in beeps and flashes).

# Implementation details
## Arduino
Practically the Arduino can be a dead-simple client with all logic done on the server. But it's fun if we give the Arduino more autonomy. So the Arduino accepts an ASCII-like character string and translates it into Morse code.

Some over-engineered moments:
1. The letter to Morse code mapping array is generated using a Python script where the Morse codes are defined in standard notation (. and -). I had previously tried using pre-processor macros and constexpr to do this but I didn't get anywhere.
2. A letter's Morse code is stored using 8 bits. 0 corresponds to dot and 1 to dash and there's a leading 1 to denote the beginning of the pattern. The leading 1 saves memory (avoids having to store pattern length - the Arduino is quite constrained in the memory department) at the cost of runtime performance. When searching for the leading 1 the first two bits can be skipped at the outset (pattern + leading 1 are 6 bits at most) but I didn't bother implementing this optimization.

### Data transfer
TODO

### Blocking main loop
Currently the main loop halts the entire program to control the output devices. Ideally it would plan out a timeline of actions at regular intervals instead. This timeline would be checked every loop iteration without perceptibly delaying the loop execution.

## Server
TODO

## Client
TODO
