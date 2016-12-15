#pragma once
#include <string>
#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <termios.h>
#include <unistd.h>
#include <fcntl.h>
using namespace std;

class myCSerial
{
public:
	myCSerial();
	myCSerial(string, int);
	~myCSerial(void);
	void setupSerial(int);
	void makeBlocking();
	void makeUnBlocking();
	string Read(char*, int);
	void printHelp();
private:
	string blockingRead(char*, int);
	string nonBlockingRead(char*, int);
	static const char* portName;
	int fd;
	struct termios portOptions;
	bool blocking;
protected:
};

myCSerial::myCSerial(void)
{
	printHelp();
}

myCSerial::myCSerial(string port = "/dev/ttyACM0", int BAUD_RATE = B9600)
{
	portName = port.c_str();
	fd = open(portName, O_RDWR | O_NOCTTY);
	if (fd <0) {perror(portName); exit(-1); }
	tcgetattr(fd, &portOptions);
	cfsetispeed(&portOptions, BAUD_RATE);
	cfsetospeed(&portOptions, BAUD_RATE);
	tcsetattr(fd, TCSANOW, &portOptions);
	tcflush(fd, TCIOFLUSH);
	blocking = true;
}

void myCSerial::setupSerial(int BAUD_RATE = B9600)
{
	portOptions.c_cflag = BAUD_RATE | CRTSCTS | CS8 | CLOCAL | CREAD;
	portOptions.c_cflag &= ~PARENB;
	portOptions.c_cflag &= ~CSTOPB;
	portOptions.c_cflag &= ~CSIZE;
	portOptions.c_oflag = 0;
	portOptions.c_cc[VINTR]    = 0;     /* Ctrl-c */ 
    portOptions.c_cc[VQUIT]    = 0;     /* Ctrl-\ */
    portOptions.c_cc[VERASE]   = 0;     /* del */
    portOptions.c_cc[VKILL]    = 0;     /* @ */
    portOptions.c_cc[VEOF]     = 4;     /* Ctrl-d */
    portOptions.c_cc[VSWTC]    = 0;     /* '\0' */
    portOptions.c_cc[VSTART]   = 0;     /* Ctrl-q */ 
    portOptions.c_cc[VSTOP]    = 0;     /* Ctrl-s */
    portOptions.c_cc[VSUSP]    = 0;     /* Ctrl-z */
    portOptions.c_cc[VEOL]     = 0;     /* '\0' */
    portOptions.c_cc[VREPRINT] = 0;     /* Ctrl-r */
    portOptions.c_cc[VDISCARD] = 0;     /* Ctrl-u */
    portOptions.c_cc[VWERASE]  = 0;     /* Ctrl-w */
    portOptions.c_cc[VLNEXT]   = 0;     /* Ctrl-v */
    portOptions.c_cc[VEOL2]    = 0;     /* '\0' */

    portOptions.c_cc[VTIME]    = 0;     /* inter-character timer unused */
    portOptions.c_cc[VMIN]     = 1;     /* blocking read until 1 character arrives */


	tcflush(fd, TCIOFLUSH);
 	tcsetattr(fd, TCSANOW, &portOptions);

}

myCSerial::~myCSerial(void)
{

}

void myCSerial::makeUnBlocking()
{
	int flags = fcntl(fd,F_GETFL, 0);
	fcntl(fd, F_SETFL, flags | FNDELAY);
}
void myCSerial::makeBlocking()
{
	int flags = fcntl(fd,F_GETFL, 0);
	fcntl(fd, F_SETFL, O_RDWR | O_NOCTTY);
}
string myCSerial::blockingRead(char buf[64], int res)
{
	string tempString;
	/*fd_set read_fds, write_fds, except_fds;
	FD_ZERO(&read_fds);
	FD_ZERO(&write_fds);
	FD_ZERO(&except_fds);
	FD_SET(fd, &read_fds);

	// Set timeout to 1.0 secondse
	struct timeval timeout;
	timeout.tv_sec = 1;
	timeout.tv_usec = 0;

	// Wait for input to become ready or until the time out; the first parameter is
	// 1 more than the largest file descriptor in any of the sets
	if (select(fd + 1, &read_fds, &write_fds, &except_fds, &timeout) == 1)
	{
	    res = read(fd,buf,sizeof(buf));
		buf[res] = 0;
	}
	else
	{
	    return tempString;
	}*/

	res = read(fd,buf,sizeof(buf));
	buf[res] = 0;
	if(res < 0)
		cout << "Nothing to be read" << endl;
	else
	{
	tempString = string(buf);
	}
	return tempString;
}

string myCSerial::nonBlockingRead(char buf[64], int res)
{
	string tempString;
	fd_set read_fds, write_fds, except_fds;
	FD_ZERO(&read_fds);
	FD_ZERO(&write_fds);
	FD_ZERO(&except_fds);
	FD_SET(fd, &read_fds);

	// Set timeout to 1.0 secondse
	struct timeval timeout;
	timeout.tv_sec = 1;
	timeout.tv_usec = 0;

	// Wait for input to become ready or until the time out; the first parameter is
	// 1 more than the largest file descriptor in any of the sets
	if (select(fd + 1, &read_fds, &write_fds, &except_fds, &timeout) == 1)
	{
	    res = read(fd,buf,sizeof(buf));
		buf[res] = 0;
	}
	return tempString;
}

string myCSerial::Read(char buf[64], int res)
{
	return blocking == true ? blockingRead(&buf[64],res) : nonBlockingRead(&buf[64], res);
}
void myCSerial::printHelp()
{
	printf("---------------------------------------------------------------------------------\n\
		\rList of possible commands:\n\
		\rmyCSerial(string port, int BAUD_RATE)\n\
		This will automatically set the port to ttyACM0 and the BAUD_RATE to 9600 for easy\
		access to arduino. It can be changed to any port you want and BAUD_RATE.\n\
		\rsetupSerial(int BAUD_RATE)\n\
		Once again, the BAUD_RATE will be 9600 for ease. This will set up your serial\
		for blocking mode to initiate. 8 Bit, local read and write.\n\
		\rmakeUnBlocking()\n\
		This will take your current serial configuration and remove blocking read which\
		will make your program stop until it receives information from serial.\
		\rmakeBlocking()\n\
		This will take your current serial configuration and add blocking read which\
		will make your program stop until it receives information from serial.\n\
		\rread(char buf[64])\n\
		This will read the serial and return a string.");
}