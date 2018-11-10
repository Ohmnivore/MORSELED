import serial
import struct
import threading
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
MORSE_EXT_POLL = MORSE_EXT_WHITESPACE + 3

class MorseDriver:

    def __init__(self):
        self.connection = None
        self._stop_event = None
        self._can_send = None
        self._read_thread = None
        self._free_slots = 0

    def start(self, port, baud_rate=9600, timeout=0):
        self.connection = serial.Serial(port, baud_rate, timeout=timeout)

        self._stop_event = threading.Event()
        self._can_send = threading.Condition()
        self._read_thread = threading.Thread(target=self._read_loop, args=())
        self._read_thread.start()
        self._free_slots = 0

        time.sleep(3.0) # Wait for the Arduino to reboot (usual behavior on serial connection)

    def stop(self):
        print('stop')
        self._stop_event.set()

    def _read_loop(self):
        while not self._stop_event.isSet():
            request = self.connection.read(1)

            if len(request) > 0:
                print('recvd: ' + repr(request))
                amount = ord(request)

                if amount > 0:
                    self._can_send.acquire()
                    print('free slots recvd: ' + str(amount))
                    self._free_slots = amount
                    self._can_send.notifyAll()
                    self._can_send.release()

        print('stopped reading')

    def _convert_text(self, text):
        # Morse code doesn't have letter case
        temp = text.lower()
        # Replace all whitespace sequences by a space (Morse code doesn't have any other whitespace)
        temp = ' '.join(temp.split())

        # Discard previous task
        data = [MORSE_EXT_CANCEL]

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
        cur_offset = 0

        while cur_offset < len(converted):
            self.connection.write([MORSE_EXT_POLL])
            print('sent poll')

            self._can_send.acquire()
            self._can_send.wait()
            amount = self._free_slots
            self._can_send.release()

            print('free slots: ' + str(amount))
            to_send = converted[cur_offset:cur_offset + amount]
            cur_offset += amount

            print('sent: ' + str(to_send))
            self.connection.write(to_send)

        print('done sending')

    def debug_read(self):
        while True:
            line = self.connection.readline()
            if len(line) > 0:
                print(line)
