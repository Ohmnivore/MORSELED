from flask import Flask, request
import json

import morse_driver


app = Flask(__name__)


@app.route('/')
def hello_world():
    return app.send_static_file('index.html')


@app.route('/send', methods=['POST'])
def send_text():
    return 'Received: ' + request.json['text']
