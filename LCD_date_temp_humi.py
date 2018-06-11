from Adafruit_CharLCD import Adafruit_CharLCD
import Adafruit_DHT
from time import sleep
from datetime import datetime
import time
from threading import Thread
import threading

turn = 4

sensor = Adafruit_DHT.DHT22
pin=18

humidity, temperature = Adafruit_DHT.read_retry(sensor, pin)


def thread_go():
    global humidity
    global temperature
    while True:
        humidity, temperature = Adafruit_DHT.read_retry(sensor, pin)
   print (datetime.today().strftime("CheckTime : %p %I:%M:%S"))
        print('Temp       {0:0.1f}C'.format(temperature))
        print('Humidity   {0:0.1f}%'.format(humidity))

        sleep(turn*3)

lcd = Adafruit_CharLCD(rs=22, en=11, d4=23, d5=10, d6=9, d7=25, cols=16, lines=2)

count = 0

#threading.Timer(12,thread_go).start()
t1 = Thread(target = thread_go)
t1.start()

while True:
    message = datetime.today().strftime("   %p %I:%M:%S\n")
    lcd.clear()
    
    if count >= 0 and count < turn:
        message = message + datetime.today().strftime("Date  %Y.%m.%d\n")
    elif count >= turn and count < (turn*2):    
        message = message + ('Temp       {0:0.1f}C'.format(temperature))
    elif count >= (turn*2) and count < (turn*3):
        message = message + ('Humidity   {0:0.1f}%'.format(humidity))
    lcd.message(message)
    
    count = count + 1
    if count == (turn*3) :
        count = 0
    sleep(1)
