import serial
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
        self._write_lock = None
        self._write_thread_id = 0

    def start(self, port, baud_rate=9600, timeout=0):
        self.connection = serial.Serial(port, baud_rate, timeout=timeout)

        self._stop_event = threading.Event()
        self._can_send = threading.Condition()
        self._read_thread = threading.Thread(target=self._read_loop, args=())
        self._read_thread.start()
        self._free_slots = 0
        self._write_lock = threading.Lock()
        self._write_thread_id = 0

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
        # Remove leading and trailing whitespace
        temp = temp.strip()

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

    def cancel_write(self):
        self._can_send.acquire()

        self._write_thread_id += 1
        thread_id = self._write_thread_id

        # Wake up write threads that are waiting for response
        # so that they can exit
        self._can_send.notifyAll()
        self._can_send.release()

        return thread_id

    def _check_write_thread_exit(self, write_thread_id):
        if write_thread_id is not self._write_thread_id:
            print('killed old serial write thread (id {0})'.format(write_thread_id))
            return True
        return False

    def send_text(self, text):
        thread_id = self.cancel_write()

        with self._write_lock:
            print('got write lock (thread id {0})'.format(thread_id))

            # Discard previous task
            self.connection.reset_output_buffer()
            self.connection.write([MORSE_EXT_CANCEL])

            converted = self._convert_text(text)
            cur_offset = 0

            while cur_offset < len(converted):
                self.connection.write([MORSE_EXT_POLL])
                print('sent poll (thread id {0})'.format(thread_id))

                self._can_send.acquire()
                self._can_send.wait()

                # See if newer thread asks to replace this one
                if self._check_write_thread_exit(thread_id):
                    self._can_send.release()
                    return

                amount = self._free_slots
                self._can_send.release()

                print('free slots: {0} (thread id {1})'.format(str(amount), thread_id))
                to_send = converted[cur_offset:cur_offset + amount]
                cur_offset += amount

                print('sent: {0} (thread id {1})'.format(str(to_send), thread_id))
                self.connection.write(to_send)

            print('done sending (thread id {0})'.format(thread_id))

    def debug_read(self):
        while True:
            line = self.connection.readline()
            if len(line) > 0:
                print(line)
