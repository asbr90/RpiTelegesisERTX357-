/*
 * main.c
 *
 *  Created on: 05.12.2014
 *      Author: Artjom Siebert
 */

#include <wiringPi.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

#include "ETRX357.h"
#include "DriverAPI.h"

int sBuffer = 512;

int isDeviceSocket(char* deviceid) {
	char* ptr = strtok(deviceid, "v");
	printf("%s\n", ptr);
	if (strcmp(ptr, ONOFF_SWITCH) == 0 || strcmp(ptr, LEVELCONTROL_SWITCH) == 0
			|| strcmp(ptr, ONOFF_OUTPUT) == 0
			|| strcmp(ptr, LEVELCONTROLLABLE_OUTPUT) == 0
			|| strcmp(ptr, RANGE_EXTENDER) == 0
			|| strcmp(ptr, MAINS_POWER_OUTLET) == 0
			|| strcmp(ptr, DIMMABLE_LIGHT) == 0)
		return 1;
	else
		return 0;
}

int isDeviceHue(char* deviceid) {

	if (strcmp(deviceid, ONOFFLIGHT) == 0
			|| strcmp(deviceid, COLORDIMABLELIGHT) == 0
			|| strcmp(deviceid, ONOFFLIGHTSWITCH) == 0
			|| strcmp(deviceid, DIMMERSWITCH) == 0
			|| strcmp(deviceid, COLORDIMERSWITCH) == 0
			|| strcmp(deviceid, LIGHTSENSOR) == 0
			|| strcmp(deviceid, OCCUPANCYSENSOR) == 0)
		return 1;
	else
		return 0;
}

int main(int argc, char *argv[]) {
	int sockfd, newsockfd, portno;
	socklen_t clilen;
	char buffer[sBuffer];
	char* api;
	struct sockaddr_in serv_addr, cli_addr;
	sockets *powerSocket = (sockets*) malloc(sizeof(struct Socket_list));
	hue *hues = (hue*) malloc(sizeof(struct hue_list));
	char* deviceID;

	telegesis_t productInfo;
	zigbee_t zigbee;
	char* error_code;
	nodes* list_pan = NULL;
	int n;

	list_pan = malloc(sizeof(struct list_pans));
	list_pan->next = NULL;

	bootload();

	InitializeSerialPort("/dev/ttyUSB0");
	ProductIdentificationInformation(&productInfo);

	printf("%s\n", productInfo.deviceName);
	printf("%s\n", productInfo.firmwareRevision);
	printf("%s\n", productInfo.EUID);

	//hier sollten dynamische Endpoints und nodeids stehen. So kann man
	// beim start schon Devices in die Strukturen schreiben
	deviceID = getDeviceID("3a4f", "01");
	printf("Device ID: %s\n", deviceID);
	if (isDeviceSocket(deviceID)) {
		//set socket list
		printf("Device is a socket\n");
		powerSocket->DeviceID = deviceID;
		powerSocket->InputCluster = getInCluster("3a4f", "01");
		powerSocket->ManufacturerName = getManufacturerName("3a4f", "01");
		powerSocket->ep = getEndPoint("3a4f");
		printf(
				"Endpoint: %s\nDevice ID: %s\nInput Cluster: %s\nManufacturer Name: %s\n",
				powerSocket->ep, powerSocket->DeviceID,
				powerSocket->InputCluster, powerSocket->ManufacturerName);

	} else if (isDeviceHue(deviceID)) {
		hues->DeviceID = deviceID;
	}

	if (argc < 2) {
		fprintf(stderr, "ERROR, no port provided\n");
		exit(1);
	}
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd < 0)
		perror("ERROR opening socket");

	bzero((char *) &serv_addr, sizeof(serv_addr));
	portno = atoi(argv[1]);
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = INADDR_ANY;
	serv_addr.sin_port = htons(portno);
	if (bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0)
		perror("ERROR on binding");
	listen(sockfd, 5);
	clilen = sizeof(cli_addr);

	newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr, &clilen);
	printf("Server connected");
	if (newsockfd < 0)
		perror("ERROR on accept");

	while (1) {
		bzero(buffer, sBuffer);
		printf("Waiting for request..");
		n = read(newsockfd, buffer, sBuffer);

		if (n < 0) {
			perror("ERROR reading from socket");
			n = write(newsockfd, "NACK", 5);

			if (n < 0)
				perror("ERROR writing to socket");
		} else {
			printf("Server received message from client: %s\n", buffer);
			api = distinguishInterface(buffer);
			printf("API Command received: %s\n", api);
			n = write(newsockfd, "ACK", 4);

			if (n < 0)
				perror("ERROR writing to socket");
		}
	}
	close(newsockfd);
	close(sockfd);

//	moveToLevel("0001","0B","0a",SEND_TO_GROUP);
//	changeONOFFState("0001","0B",ON_GROUP,SEND_TO_GROUP);
//	changeColor("0001","0B","55",SEND_TO_GROUP);
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

