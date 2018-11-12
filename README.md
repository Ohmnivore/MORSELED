This project contains a web application for sending text to an Arduino device which then outputs it in Morse code (in beeps and flashes).

![](screenshot.png)

# Libraries
* Arduino IDE `1.8.7`
* Python `3.6.1`
* Flask `1.0.2`
* PySerial `3.4`

# Running
* Open the `hardware/morse_blink/morse_blink.ino` Arduino script
* (Set LED and buzzer pins, 11 and 12 by default)
* Hook up a LED and a buzzer to your Arduino
* Upload the script to your Arduino
* In a terminal, `cd` to the `server` directory
* Run `host_dev.bat <serial port>` (Windows, serial port can be something like `COM5` - can check with the Arduino IDE) or `host_dev.sh <serial port>` (Linux) to launch the server
* Open `http://127.0.0.1:5000/` in a web browser (only tested with Firefox and Chrome on desktop)
* Write something in the text area and click the `SEND` button
* For `SOS` as sample input, the buzzer should chirp and the LED flash quickly three times, then slowly three times, then quickly three times
* Alternatively for testing without a server/GUI, use the `server/arduino_test.py` script

# Implementation details
## Arduino
Practically the Arduino could be a dead-simple client with all logic done on the server. But it's fun if we give the Arduino more autonomy. So the Arduino accepts an ASCII-like character string and translates it into Morse code.

Some over-engineered moments:
1. The Arduino constructs a "timeline" for the morse patterns to avoid any blocking `delay()` calls. This frees the main loop to do other things.
2. The letter to Morse code mapping array is generated using a Python script where the Morse codes are defined in standard notation (. and -). I had previously tried using pre-processor macros and constexpr to do this but I didn't get anywhere.
3. A letter's Morse code is stored using 8 bits. 0 corresponds to dot and 1 to dash and there's a leading 1 to denote the beginning of the pattern. The leading 1 saves memory (avoids having to store pattern length - the Arduino is quite constrained in the memory department) at the cost of runtime performance. When searching for the leading 1 the first two bits can be skipped at the outset (pattern + leading 1 are 6 bits at most) but I didn't bother implementing this optimization.
4. It's possible to tell the Arduino to stop the current task (clear buffers) to setup a new task.

### Data transfer
Data transfer is done using a polling mechanism (the driver asks Arduino how much space it has before sending data chunks). This avoids stressing the serial connection and avoids sending the Arduino more data than it can store in its circular buffer.

## Server
The server contains a Flask app and a driver which communicates with the Arduino.

The app serves static content and has a single endpoint for a POST request which contains text that the Arduino should output. The app performs a sanity check on the content.

The driver has a separate thread for receiving Arduino input (this will come in handy in the future when the Arduino needs to send more varied data). It also handles cancellation of tasks:
* If a web request comes in while an older one is still being processed then the older one will be cleanly and immediately cancelled

## Client
The client (in the spirit of a Cold War spy theme) is written in plain HTML/CSS/JS.

It has a cute title intro, client-side input filtering for the text panel, and a "send" button (using XMLHttpRequest).

# Possible improvements
* GUI indicator of which character the Arduino is outputting right now
* GUI to set the buzzer volume, LED intensity, Morse code output speed, turn buzzer or LED on/off
* Use a physical switch to also read Morse code from the Arduino
* Multi-user requests queuing + publicly visible task list
* Prevent possible buffer overflow in driver's send_text function (MORSE_EXT_CANCEL and MORSE_EXT_POLL are sent unconditionally)
* Make the GUI mobile-friendly layout + accessibility
