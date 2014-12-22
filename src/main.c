/*
 * main.c
 *
 *  Created on: 05.12.2014
 *      Author: Artjom Siebert
 */

#include <wiringPi.h>
#include <stdio.h>
#include "ATParser.h"
#include "ETRX357.h"

int main() {
	telegesis_t telegesis;
	InitializeSerialPort("/dev/ttyUSB0");
	ProductIdentificationInformation(&telegesis);
	
	return 0;
}
