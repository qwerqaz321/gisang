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

alram = ��time��
alram = input("Input Time Number (ex. 0800) : ") # ���α׷� ����� �˶��ð��� ����

while True :
    currtime = datetime.today().strftime("%H%M") #����ð��� ��, �� �������� ���� 
    if(str(alram) == str(currtime)) :               # �����ð��� ����ð��� ������
        ck_pwm.start(50)                         # ������ �︰��.
        time.sleep(10)                            # 10�ʰ� �︲
        ck_pwm.stop()                            # ���� ����
        sleep(60)                                 # 60�� ���� �� X
    sleep(1)                                     # ���� �ð� �񱳸� 1�ʸ��� ��.