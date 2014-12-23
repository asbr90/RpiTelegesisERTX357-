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
	telegesis_t productInfo;
	zigbee_t zigbee;
	int error_code;

	InitializeSerialPort("/dev/ttyUSB0");
	ProductIdentificationInformation(&productInfo);

	/*printf("%s\n",productInfo.deviceName);
	printf("%s\n",productInfo.firmwareRevision);
	printf("%s\n",productInfo.EUID);
*/
	if((error_code = EstablishPAN(&zigbee))!=OK)
		printf("Error code: %s\n",GetErrorCodeMessage(error_code));

	//printf("%s\n",getErrorCodeMessage(getErrorCodeNumber("1A")));
	return 0;
}
