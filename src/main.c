/*
 * main.c
 *
 *  Created on: 05.12.2014
 *      Author: Artjom Siebert
 */

#include <wiringPi.h>
#include <stdio.h>
#include <stdlib.h>
#include "ATParser.h"
#include "ETRX357.h"

int main() {
	telegesis_t productInfo;
	zigbee_t zigbee;
	char* error_code;
	node* list_pan = NULL;

	list_pan = malloc(sizeof(struct list_pans));
	list_pan->next = NULL;

	bootload();

	InitializeSerialPort("/dev/ttyUSB0");
	ProductIdentificationInformation(&productInfo);

	printf("%s\n", productInfo.deviceName);
	printf("%s\n", productInfo.firmwareRevision);
	printf("%s\n", productInfo.EUID);

	/*error_code = EstablishPAN(&zigbee);

	 if (error_code != (char*) OK)
	 printf("%s\n", GetErrorCodeMessage(error_code));
	 else {
	 printf("Channel: %d\n", zigbee.channel);
	 printf("PID: %s\n", zigbee.PID);
	 printf("EPID: %s\n", zigbee.EPID);
	 }*/

	//ScanForActivePAN(&list_pan);
	/*printf("%d\n", list_pan->channel);
	printf("%s\n", list_pan->PID);
	printf("%s\n", list_pan->EPID);
	printf("%d\n", list_pan->stackProfile);
	printf("%d\n", list_pan->joinPermission);
	*/

	//printf("%s\n", DisassociateLocalDeviceFromPAN(&zigbee));
	printf("%s\n", JoinNetwork(&zigbee));

	return 0;
}

