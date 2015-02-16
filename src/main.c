/*
 * main.c
 *
 *  Created on: 05.12.2014
 *      Author: Artjom Siebert
 */

#include <wiringPi.h>
#include <stdio.h>
#include <stdlib.h>
#include "ETRX357.h"
#include "PhilipsHUE.h"

int main() {
	telegesis_t productInfo;
	zigbee_t zigbee;
	char* error_code;
	nodes* list_pan = NULL;

	list_pan = malloc(sizeof(struct list_pans));
	list_pan->next = NULL;

	bootload();

	InitializeSerialPort("/dev/ttyUSB0");
	ProductIdentificationInformation(&productInfo);

	printf("%s\n", productInfo.deviceName);
	printf("%s\n", productInfo.firmwareRevision);
	printf("%s\n", productInfo.EUID);

//	changeONOFFState("0001","0B",ON_GROUP,SEND_TO_GROUP);
	//changeColor("0001","0B","55",SEND_TO_GROUP);
	moveToLevel("0001","0B","0a",SEND_TO_GROUP);

	/*	error_code = EstablishPAN(&zigbee);

	 if (error_code != (char*) OK)
	 printf("%s\n", error_code);
	 else {
	 printf("Channel: %d\n", zigbee.channel);
	 printf("PID: %s\n", zigbee.PID);
	 printf("EPID: %s\n", zigbee.EPID);
	 }
	 */
	//ScanForActivePAN(&list_pan);
	//DisplayNeighbourTable("00,0000");
	//AddGroupOnTargetDevice("05A3,0B,0,0001,smartTower");
	//AddGroupOnTargetDevice("717E,0B,0,0001,smartTower");
	//SendRAWZCLMessagetoTarget("717E,0B,0006,010002");
	/*	printf("%d\n", list_pan->channel);
	 printf("%s\n", list_pan->PID);
	 printf("%s\n", list_pan->EPID);
	 printf("%d\n", list_pan->stackProfile);
	 printf("%d\n", list_pan->joinPermission);
	 */
	//printf("%s\n", DisassociateLocalDeviceFromPAN(&zigbee));
	//printf("%s\n", JoinNetwork(&zigbee));
	//printf("Display: %s \n", DisplayNetworkInformation());
	//printf("Change Channel: %s\n",ChangeNetworkChannel("1A"));
	return 0;
}

