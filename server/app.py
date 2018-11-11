from flask import Flask, jsonify, request, send_from_directory
import json
import os
import signal

import morse_driver

app = Flask(__name__)
driver = None

@app.before_first_request
def setup_driver():
    global driver
    driver = morse_driver.MorseDriver()

    SERIAL_PORT_ENV = 'SERIAL_PORT'
    if SERIAL_PORT_ENV not in os.environ or len(os.environ[SERIAL_PORT_ENV]) == 0:
        print(SERIAL_PORT_ENV + ' environment variable is undefined. Shutting down...')
        os.kill(os.getpid(), signal.SIGTERM)
    serial_port = os.environ[SERIAL_PORT_ENV]

    print('Using serial port: ' + serial_port)
    driver.start(serial_port)


@app.route('/')
def index():
    return app.send_static_file('index.html')


@app.route('/static/<path:path>')
def serve_static(path):
    return send_from_directory('static', path)


@app.route('/send', methods=['POST'])
def send_text():
    recvd = request.json['text']
    print('text received: ' + recvd)

    text = driver.sanitize_text(recvd)

    if len(text) == 0:
        response = jsonify(success=False, reason='Sanitized text has length 0')
        response.status_code = 400
        return response
    else:
        driver.send_text(text)
        return jsonify(success=True)
