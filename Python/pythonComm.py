import re
import sys
import numpy as numpy
import serial
import socket

class myPySerial:
	def __init__(self, port):
		port = str(port)
		self.ser = serial.Serial('/dev/' + port, 9600, timeout = 1)
		if(self.ser.isOpen() == False):
			print("Serial failed to open")
		else:
			print("Serial has opened")

	def openSerial(self):
		if (self.ser.isOpen() == False):
			self.ser.open()
			if(self.ser.isOpen): print("Successful opening of Serial Port /dev/" + port)

	def readSerial(self):
		while self.ser.isOpen():
			out = ''
			while self.ser.inWaiting() > 0:
				out += self.ser.read(1).decode('utf-8')
				if out != '':
					print("Received string: {:s}".format(out))

	def checkString(self):
		if out == "1 + 2":
			pat = re.compile("+");
			nums = pat.split(out);
			num1 = int(nums[0]);
			num2 = int(nums[1]);
			if(isinstance(num1, int) and isinstance(num2,int)):
				print("Returning {:d} + {:d} = {:d}".format(num1, num2, num1+num2))
				return num1 + num2

	def main(argv):
		ser = myPySerial(ttyACM0)
		myPySerial.readSerial()
		num = myPySerial.checkString()
		print("The number we receieved was {:d}".format(num))

if __name__ == '__main__':
	myPySerial.main(sys.argv)
