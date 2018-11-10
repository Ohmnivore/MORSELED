import serial
import struct
import time

ASCII_A = ord('a')
ASCII_Z = ord('z')
ASCII_0 = ord('0')
ASCII_9 = ord('9')
ASCII_LETTERS_SIZE = ASCII_Z - ASCII_A + 1
ASCII_NUMBERS_SIZE = ASCII_9 - ASCII_0 + 1
MORSE_EXT_WHITESPACE = ASCII_LETTERS_SIZE + ASCII_NUMBERS_SIZE
MORSE_EXT_END = MORSE_EXT_WHITESPACE + 1
MORSE_EXT_CANCEL = MORSE_EXT_WHITESPACE + 2

class MorseDriver:

    def __init__(self):
        self.connection = None
        pass

    # Windows-only
    def start(self, port, baud_rate=9600, timeout=50):
        self.connection = serial.Serial(port, baud_rate, timeout=timeout)
        time.sleep(3.0) # Wait for the Arduino to reboot (usual behavior on serial connection)

    def _convert_text(self, text):
        # Morse code doesn't have letter case
        temp = text.lower()
        # Replace all whitespace sequences by a space (Morse code doesn't have any other whitespace)
        temp = ' '.join(temp.split())

        # Discard previous task
        # data = [MORSE_EXT_CANCEL] # Doesn't work as intended
        data = []

        for char in temp:
            ascii_char = ord(char)

            out = None

            if ascii_char >= ASCII_A and ascii_char <= ASCII_Z:
                out = ascii_char - ASCII_A
            elif ascii_char >= ASCII_0 and ascii_char <= ASCII_9:
                out = ASCII_LETTERS_SIZE + ascii_char - ASCII_0
            elif char == ' ':
                out = MORSE_EXT_WHITESPACE

            if out is not None:
                data.append(out)

        data.append(MORSE_EXT_END)

        return data

    def send_text(self, text):
        converted = self._convert_text(text)
        print('sent: ' + str(converted))
        self.connection.write(converted)

    def debug_read(self):
        while True:
            line = self.connection.readline()
            if len(line) > 0:
                print(line)
