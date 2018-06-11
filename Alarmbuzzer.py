import RPi.GPIO as GPIO
import time
from datetime import datetime
from time import sleep

BUZ = 15
ON = 1
OFF = 0

GPIO.setmode(GPIO.BCM)
GPIO.setup(BUZ, GPIO.OUT)
ck_pwm = GPIO.PWM(BUZ, 10)

alram = “time”
alram =  raw_input("Input Time Number (ex. 0800) : ")  # 프로그램 실행시 알람시간을 설정

while True :
    currtime = datetime.today().strftime(“T”+"%H%M")  #현재시간을 시, 분 형식으로 대입 
    if(str(alram) == str(currtime)) :               # 설정시간과 현재시간이 같으면
        ck_pwm.start(50)                         # 부저가 울린다.
        time.sleep(10)                            # 10초간 울림
        ck_pwm.stop()                            # 부저 정지
        sleep(60)                                 # 60초 동안 비교 X
    sleep(1)                                     # 이후 시간 비교를 1초마다 함.
