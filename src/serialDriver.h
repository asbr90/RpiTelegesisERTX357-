/*
 * Driver_uart.h
 *
 *  Created on: 05.12.2014
 *      Author: artjom
 */

#ifndef SERIAL_DRIVER_H_
#define SERIAL_DRIVER_H_

/**
 * inlcudes
 */
#include <wiringPi.h>
#include <wiringSerial.h>
#include <stdio.h>
#include <termios.h>		//Used for UART
#include <unistd.h>			//Used for UART
#include <fcntl.h>			//Used for UART
/**
 * definitions
 */
#define false	-1
#define true	0
#define CRLF	"\r\n"

/**
 * function prototypes
 */
int serialOpenPort(char* , int);
void serialClosePort(void);
void serialTransmit(char *message);

/**
 * @brief 	The wiringSerial library is intended to provide simplified control – suitable for most applications,
 * 			if you need advanced control – e.g.  parity control, modem control lines (via a USB adapter,
 * 			there are none on the Pi’s on-board UART!) and so on, then you need to do some of this the “old fashioned” way.
 *
 * @param	csize 		Character size mask. values are 5, 6, 7, or 8.
 * @param	cstopbit	Set two stop bits, rather than one.
 * @param	paritybit 	Enable parity generation on output and parity checking for input.
 * @param	paddodd		If set, then parity for input and output is odd; otherwise even parity is used.
 */
void serialSetSettings(speed_t baudconst, short csize, short cstopbit, short paritybit,
		short paddodd);
void serialCurrentSettings(void);
long getBaudrate(long baud);
void getData(void);
char* serialReceive(void);
#endif /* DRIVER_UART_H_ */
