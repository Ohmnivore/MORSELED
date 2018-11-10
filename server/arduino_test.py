import time

import morse_driver


class Config:

    def __init__(self):
        self.OPT_PORT = None
        self.OPT_TEXT = None


def print_usage_and_exit():
    print("Usage: arduino_test.py send <COM port> <Text to send>")
    print("Usage: arduino_test.py test <COM port> <cancel>")
    exit(1)


def send(config):
    driver = morse_driver.MorseDriver()
    driver.start(config.OPT_PORT)

    driver.send_text(config.OPT_TEXT)
    driver.debug_read()


def test_cancel_feature(config):
    driver = morse_driver.MorseDriver()
    driver.start(config.OPT_PORT)

    driver.send_text('hello world')
    time.sleep(1.0)
    driver.send_text('sos')
    driver.debug_read()


def parse_args(args):
    config = Config()

    if len(args) >= 4:
        cmd = args[1]

        if cmd == 'send':
            config.OPT_PORT = args[2]
            config.OPT_TEXT = args[3]
            send(config)

        elif cmd == 'test':
            config.OPT_PORT = args[2]
            test_name = args[3]

            if test_name == 'cancel':
                test_cancel_feature(config)

            else:
                print_usage_and_exit()
        else:
            print_usage_and_exit()
    else:
        print_usage_and_exit()


if __name__ == "__main__":
    import sys
    parse_args(sys.argv)
