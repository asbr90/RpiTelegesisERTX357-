/*
 * Driver_uart.c
 *
 *  Created on: 05.12.2014
 *      Author: artjom
 */

#include "serialDriver.h"
#include <errno.h>
#include <string.h>
#include <malloc.h>

int fileDescriptor = -1;

int serialOpenPort(char *COM, int baud) {

	if ((fileDescriptor = serialOpen(COM, baud)) == -1) {
		fprintf(stderr, "Unable to open serial device: %s\n", strerror(errno));
		return false;
	}

	printf("Open serial Port %s\n", COM);
	return true;
}

void serialClosePort(void) {
	serialClose(fileDescriptor);
}

char *append(const char *orig, char c)
{
    size_t sz = strlen(orig);
    char *str = malloc(sz + 2);
    strcpy(str, orig);
    str[sz] = c;
    str[sz + 1] = '\0';
    return str;
}

void serialTransmit(char *message) {
	serialPrintf(fileDescriptor, append(message, '\r'));
}


void serialSetSettings(speed_t baudconst, short csize, short cstopbit,
		short paritybit, short paddodd) {
	struct termios options;
	int transBit;

	tcgetattr(fileDescriptor, &options);   // Read current options
	options.c_cflag &= ~CSIZE;  // Mask out size
	switch (csize) {
	case 5:
		transBit = CS5;
		break;
	case 6:
		transBit = CS6;
		break;
	case 7:
		transBit = CS7;
		break;
	case 8:
		transBit = CS8;
		break;
	}

	options.c_cflag |= transBit;     // Or in 7-bits
	options.c_cflag |= paritybit;  // Enable Parity - even by default

	if (cstopbit == 2)
		options.c_cflag |= CSTOPB;
	else
		options.c_cflag &= ~CSTOPB;

	if (paddodd == 1)/* odd parity, else even */
		options.c_cflag |= PARODD;
	else
		options.c_cflag &= ~PARODD;

	if (paritybit == 1)/* parity enable */
		options.c_cflag |= PARENB;
	else
		/* parity disable */
		options.c_cflag &= ~PARENB;

	if (cfsetspeed(&options, baudconst) == true) {
		printf("Set baud speed successfully\n");
	} else {
		fprintf(stderr, "Unable to to set baud speed %s\n", strerror(errno));
	}
	tcflush(fileDescriptor, TCIFLUSH);
	tcsetattr(fileDescriptor, TCSANOW, &options); // Set new options. The change occurs immediately.

}

void serialCurrentSettings(void) {
	struct termios options;
	long inputSpeed, outputSpeed;
	int transBit;

	tcgetattr(fileDescriptor, &options);   // Read current options
	inputSpeed = cfgetispeed(&options);
	outputSpeed = cfgetospeed(&options);
	printf("----- Serial Settings -----\n");
	printf("Input Speed: %lu baud \nOutput Speed: %lu baud\n",
			getBaudrate(inputSpeed), getBaudrate(outputSpeed));

	switch (options.c_cflag & CSIZE) {
	case CS5:
		transBit = 5;
		break;
	case CS6:
		transBit = 6;
		break;
	case CS7:
		transBit = 7;
		break;
	case CS8:
		transBit = 8;
		break;
	}
	printf("Character bits: %d Bits\n", transBit);
	printf("Parity Enabled: %d \nParity Odd: %d\n", options.c_cflag & PARENB,
			options.c_cflag & PARODD); // Enable Parity - even by default)
	printf("Stop bits: %d\n", options.c_cflag & CSTOPB);
	printf("---------------------------\n");
}

long getBaudrate(long baud) {

	switch (baud) {
	case B0:
		return 0;
	case B50:
		return 50;
	case B75:
		return 75;
	case B110:
		return 110;
	case B134:
		return 134;
	case B150:
		return 150;
	case B200:
		return 200;
	case B300:
		return 300;
	case B600:
		return 600;
	case B1200:
		return 1200;
	case B1800:
		return 1800;
	case B2400:
		return 2400;
	case B4800:
		return 4800;
	case B9600:
		return 9600;
	case B19200:
		return 19200;
	case B38400:
		return 38400;
		/*case B7200:
		 return 7200;
		 case B14400:
		 return 14400;
		 case B28800:
		 return 28800;
		 */case B57600:
		return 57600;
		/*	case B76800:
		 return 76800;*/
	case B115200:
		return 115200;
	case B230400:
		return 230400;
	default:
		return -1;
	}
}

char* serialReceive(void){
	//----- CHECK FOR ANY RX BYTES -----
	// Read up to 255 characters from the port if they are there
	unsigned char rx_buffer[256];
	if (fileDescriptor != -1)
	{
		int rx_length = read(fileDescriptor, (void*)rx_buffer, 255);		//Filestream, buffer to store in, number of bytes to read (max)
		if (rx_length < 0)
		{
			//An error occured (will occur if there are no bytes)
			printf("An error occured\n");
		}
		else if (rx_length == 0)
		{
			//No data waiting
			printf("No data waiting\n");
		}
		else
		{
			//Bytes received
			rx_buffer[rx_length] = '\0';
			//printf("%i bytes read : %s\n", rx_length, rx_buffer);
			
		}
	}
	return &rx_buffer;
}
