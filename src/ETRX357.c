/**
 *	@author	Artjom Siebert
 *	@date	21.12.2014
 */

#include "ETRX357.h"
#include <malloc.h>
#include <string.h>
#include <stdlib.h>

char* IsError(char* response) {
	return strstr(response, ERROR);
}

short InitializeSerialPort(char* COM) {
	if (wiringPiSetup() != -1) {
		printf("Initialization was successfull\n");
		serialOpenPort(COM, 19200);
		serialSetSettings(B19200, 8, 1, 0, 0); //connect to the appropriate COM port at
											   //19200bps, Data bits - 8, Parity - none, Stop bits - 1, Flow Control – none
		return true;
	} else {
		printf("wiringPi library could not set up\n");
		return false;
	}
}

int ProductIdentificationInformation(telegesis_t *info) {
	char response[255];

	serialTransmit(ATI);
	sprintf(response, "%s", serialReceive());

	//interpret response message
	strtok(response, "\n");		// remove AT command
	info->deviceName = strtok(NULL, "\n");
	info->firmwareRevision = strtok(NULL, "\n");
	info->EUID = strtok(NULL, "\n");

	return OK;

}

int GetErrorCodeNumber(char* error) {
	char* error_code;

	printf("%s\n",strtok(error, ":"));
	error_code = strtok(NULL, "\0");
	printf("%s\n",error_code);
	return (int) strtoul(error_code, NULL, 16);
}

char* GetErrorCodeMessage(int errorNumber) {
	return error_list[errorNumber];
}

int EstablishPAN(zigbee_t* zigbee) {
	char response[255];
	char* retValue;

	serialTransmit(ATEN);
	sprintf(response, "%s", serialReceive());

	if (IsError(response) == NULL) {
		strtok(response, ":");
		printf("%s\n", response);
		/*zigbee->channel = (unsigned short) atoi(strtok(NULL, ","));
		printf("%d\n", zigbee->channel);
		zigbee->PID = atoi(strtok(NULL, ","));
		zigbee->EPID = atol(strtok(NULL, "\n"));*/

		return OK;
	} else {
		//strtok(response, "\n");
		strtok(response, "\n");
		//printf("code: %d\n",GetErrorCodeNumber(strtok(NULL, "\n")));
		return GetErrorCodeNumber(strtok(NULL, "\n"));
	}
}
