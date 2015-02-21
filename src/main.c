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
#include "PhilipsHUE.h"
#include "PowerSocket.h"

int main(int argc, char *argv[]) {
	int sockfd, newsockfd, portno;
	socklen_t clilen;
	char buffer[256];
	struct sockaddr_in serv_addr, cli_addr;

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

	DisplayNeighbourTable("00,0000");
	changeONOFFState("3A4F","01","1","0");

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

	if (newsockfd < 0)
		perror("ERROR on accept");


	while (1) {
		bzero(buffer, 256);
		n = read(newsockfd, buffer, 255);
		if (n < 0)
			perror("ERROR reading from socket");
		printf("Server received message from client: %s\n", buffer);
		n = write(newsockfd, "ACK", 4);

		if (n < 0)
			perror("ERROR writing to socket");
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

