import serial
import time

ser = serial.Serial('/dev/ttyUSB0', 9600)
time.sleep(5)
print('Go Forward')
ser.write('1,0,80,80')
time.sleep(5)
print('Go backward')
ser.write('1,1,80,80')
time.sleep(5)
print('Stop')
ser.write('0')

#Get url
#import urllib2
#urllib2.urlopen("http://google.com").read()
