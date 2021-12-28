# lsusb to check device name
# dmesg | grep "tty" to find port name

import flask
import serial
import time
import asyncio


arduino = serial.Serial("/dev/ttyUSB0", 9600, timeout=1)
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
            return str(answer, encoding='utf-8')


@app.after_request
def cors(response):
    header = response.headers
    header['Access-Control-Allow-Origin'] = '*'
    return response


@app.route('/led/<pin>/switch', methods=['POST'])
def switch_led(pin):
    send_request(f'switch {pin}')
    return '', 204


@app.route('/sensor/<sensor>', methods=['GET'])
def get_sensor(sensor):
    res = send_request(f'sensor {sensor}').split('|')
    if res[0] == '200':
        return res[1]
    else:
        return '', int(res[0])

                                                                                                                                                                                                                                                                                                                          time.sleep(3)
app.run(host='0.0.0.0')
