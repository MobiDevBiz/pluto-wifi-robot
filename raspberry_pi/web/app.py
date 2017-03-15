from flask import Flask, render_template, request, Response
import serial, time
from functools import wraps

def check_auth(username, password):
    """This function is called to check if a username /
    password combination is valid.
    """
    return username == 'pluto' and password == 'pluto'

def authenticate():
    """Sends a 401 response that enables basic auth"""
    return Response(
    'Could not verify your access level for that URL.\n'
    'You have to login with proper credentials', 401,
    {'WWW-Authenticate': 'Basic realm="Login Required"'})

def requires_auth(f):
    @wraps(f)
    def decorated(*args, **kwargs):
        auth = request.authorization
        if not auth or not check_auth(auth.username, auth.password):
            return authenticate()
        return f(*args, **kwargs)
    return decorated

app = Flask(__name__)
state_msg = 'Hello, Telerobot is ready to operate, have fun!'
try:
    ser = serial.Serial('/dev/ttyUSB0', 9600)
except serial.serialutil.SerialException:
    state_msg = 'Telerobot is disconnected (please connect Arduino to Raspberry Pi)'

@app.route('/')
@requires_auth
def index():
    return render_template('index.html', state_msg=state_msg)

@app.route('/move/<direction>')
@requires_auth
def move(direction):
    if direction == 'forward':
        ser.write('1,0')
    if direction == 'backward':
        ser.write('1,1')
    if direction == 'left':
        ser.write('1,3')
    if direction == 'right':
        ser.write('1,2')
    return '{}'

@app.route('/set_speed/<speed>')
@requires_auth
def set_speed(speed):
    ser.write('2,' + speed)
    return '{}'

@app.route('/camera_hor/<pos>')
@requires_auth
def camera_hor(pos):
    ser.write('3,' + pos)
    return '{}'

@app.route('/camera_ver/<pos>')
@requires_auth
def camera_ver(pos):
    ser.write('4,' + pos)
    return '{}'

@app.route('/stop')
@requires_auth
def stop():
    ser.write('0')
    return '{}'

if __name__ == '__main__':
    app.run(debug=True, host='0.0.0.0')
