# -*- coding: utf-8 -*-

import flask
import serial
import time


arduino = serial.Serial("/dev/ttyUSB1", 9600, timeout=1)
app = flask.Flask(__name__)


def send_request(cmd):
    global arduino
    if arduino.isOpen():
        arduino.write(bytes(cmd.encode()))
        while arduino.inWaiting() == 0:
            pass
        if arduino.inWaiting() > 0:
            answer = arduino.readline()
            arduino.flushInput()
            return answer


@app.after_request
def cors(response):
    header = response.headers
    header['Access-Control-Allow-Origin'] = '*'
    return response


@app.route('/led/<pin>/switch', methods=['POST'])
def switch_led(pin):
    send_request(f'switch {pin}')
    return '', 204


time.sleep(3)
app.run(host='0.0.0.0')
