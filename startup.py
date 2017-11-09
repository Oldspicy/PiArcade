import RPi.GPIO as GPIO
import os
import time

LedPin = 40
switch = 26
sound = 24
toggleflag = 1

GPIO.setmode(GPIO.BOARD)
GPIO.setup(LedPin, GPIO.OUT)
GPIO.setup(switch, GPIO.IN, pull_up_down=GPIO.PUD_DOWN)
GPIO.setup(sound, GPIO.IN, pull_up_down=GPIO.PUD_UP)
GPIO.output(LedPin, GPIO.HIGH)

print "LED On"
GPIO.output(LedPin, GPIO.HIGH)
os.system ('omxplayer -o both /home/pi/sounds/mp3/Startsound.mp3')

while True:
	input_state = GPIO.input(switch)
	if input_state == True:
		print "shutdown now"
		time.sleep(1.0)
		os.system ('omxplayer -o both /home/pi/sounds/mp3/Endsound.mp3')
		os.system ('shutdown now -h')
	sound_state = GPIO.input(sound)
#	if sound_state == True:
#		print "button"
#	else:
#		print "not button"
#	time.sleep(3.0)

	if sound_state == True and toggleflag == True:
		print "button in"
		toggleflag = not toggleflag
#		time.sleep(2.0)
		os.system ('sudo amixer cset numid=3 2')
#		time.sleep(2.0)
	elif sound_state == False and toggleflag == False:
		print "button out"
		toggleflag = not toggleflag
#		time.sleep(2.0)
		os.system ('sudo amixer cset numid=3 1')
#	else:
#		time.sleep(2.0)
#	time.sleep(1.0)
#	print "done" 


GPIO.cleanup()
