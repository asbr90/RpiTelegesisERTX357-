/**
 * @author	Artjom Siebert
 * @date	01.01.2015
 *
 */

#include "PhilipsHUE.h"

char* constructDataForLevel(char* frame, char* seqNumber,
		char* commandIdentifier, char* level, char* transitionTime) {
	char* data;
	if ((data = malloc(
			strlen(frame) + strlen(seqNumber) + strlen(commandIdentifier)
					+ strlen(level) + strlen(transitionTime) + 1)) != NULL) {
		data[0] = '\0';   // ensures the memory is an empty string
		strcat(data, frame);
		strcat(data, seqNumber);
		strcat(data, commandIdentifier);
		strcat(data, level);
		strcat(data, transitionTime);
	} else {
		printf("malloc failed\n");
	}

	return data;
}

char* constructDataForONOFF(char* frame, char* seqNumber,
		char* commandIdentifier) {
	char* data;

	if ((data = malloc(
			strlen(frame) + strlen(seqNumber) + strlen(commandIdentifier) + 1))
			!= NULL) {
		data[0] = '\0';   // ensures the memory is an empty string
		strcat(data, frame);
		strcat(data, seqNumber);
		strcat(data, commandIdentifier);
	} else {
		printf("malloc failed\n");
	}
	return data;
}

char* constructDataForCOLOR(char* frame, char* seqNumber,
		char* commandIdentifier, char* hue, char* direction,
		char* transitionTime) {
	char* data;
	printf("construct data\n");
	if ((data = malloc(
			strlen(frame) + strlen(seqNumber) + strlen(commandIdentifier)
					+ strlen(direction) + strlen(transitionTime) + strlen(hue)
					+ 1)) != NULL) {
		data[0] = '\0';   // ensures the memory is an empty string
		strcat(data, frame);
		strcat(data, seqNumber);
		strcat(data, commandIdentifier);
		strcat(data, hue);
		strcat(data, direction);
		strcat(data, transitionTime);
	} else {
		printf("malloc failed\n");
	}
	return data;
}

void changeColor(char* nodeid, char* endpoint, char* color) {
	char* data = constructDataForCOLOR("01", "00", "00", color, "00", "0500");
	char* payload;

	printf("change state\n");
	if ((payload = malloc(
			strlen(nodeid) + strlen(endpoint) + strlen(COLORCLUSTER)
					+ strlen(data) + 1)) != NULL) {
		payload[0] = '\0';   // ensures the memory is an empty string
		strcat(payload, nodeid);
		strcat(payload, ",");
		strcat(payload, endpoint);
		strcat(payload, ",");
		strcat(payload, COLORCLUSTER);
		strcat(payload, ",");
		strcat(payload, data);

	} else {
		printf("malloc failed\n");
	}
	SendRAWZCLMessagetoTarget(payload);   //"717E,0B,0006,010002");
}

void changeONOFFState(char* nodeid, char* endpoint, char* state) {
	char* data = constructDataForONOFF("01", "00", state);
	char* payload;

	printf("change state\n");
	if ((payload = malloc(
			strlen(nodeid) + strlen(endpoint) + strlen(ONOFCLUSTER)
					+ strlen(data) + 1)) != NULL) {
		payload[0] = '\0';   // ensures the memory is an empty string
		strcat(payload, nodeid);
		strcat(payload, ",");
		strcat(payload, endpoint);
		strcat(payload, ",");
		strcat(payload, ONOFCLUSTER);
		strcat(payload, ",");
		strcat(payload, data);

	} else {
		printf("malloc failed\n");
	}
	SendRAWZCLMessagetoTarget(payload);
}

void moveToLevel(char* nodeid, char* endpoint, char* level) {
	char* data = constructDataForLevel("01", "00", "00", level, "0500");
	char* payload;

	printf("change state\n");
	if ((payload = malloc(
			strlen(nodeid) + strlen(endpoint) + strlen(LEVELCLUSTER)
					+ strlen(data) + 1)) != NULL) {
		payload[0] = '\0';   // ensures the memory is an empty string
		strcat(payload, nodeid);
		strcat(payload, ",");
		strcat(payload, endpoint);
		strcat(payload, ",");
		strcat(payload, LEVELCLUSTER);
		strcat(payload, ",");
		strcat(payload, data);

	} else {
		printf("malloc failed\n");
	}
	SendRAWZCLMessagetoTarget(payload);
}
