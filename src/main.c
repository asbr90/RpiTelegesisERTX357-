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
		bzero(buffer, sBuffer);
		printf("Waiting for request..\n");
		n = read(newsockfd, buffer, sBuffer);

		if (n < 0) {
			perror("ERROR reading from socket");
			newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr, &clilen);

		} else {
			printf("Server received message from client: %s\n", buffer);
			api = distinguishInterface(buffer);
			printf("API Command received:\n%s\n", buffer);
			if (api != NULL) {
				if (strstr(api, UPDATE_DEVICE_LIST) && api != NULL) {
					printf("Send data to client:\n%s\n", api);
					n = write(newsockfd, api, strlen(api) + 8);
					printf("Write to client %d\n", n);
				} else {
					n = write(newsockfd, "ACK", 4);
				}
				if (n < 0)
					perror("ERROR writing to socket");
			} else {
				perror("NO valide API command found\n");
			}
		}
	}
	close(newsockfd);
	close(sockfd);

	return 0;
}

