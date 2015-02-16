/**
 * @author	Artjom Siebert
 * @date	01.01.2015
 *
 */

#include "PhilipsHUE.h"

/**
 * @deprecated
 */
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
/**
 * @deprecated
 */
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
/**
 * @deprecated
 */
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

void changeColor(char* nodeid, char* endpoint, char* color, char* sendmode) {
	char* payload;
	if ((payload = malloc(
			strlen(nodeid) + strlen(endpoint) + strlen(sendmode) + strlen(color)
					+ 16 + 1)) != NULL) {
		payload[0] = '\0';   // ensures the memory is an empty string
		strcat(payload, nodeid);
		strcat(payload, ",");
		strcat(payload, endpoint);
		strcat(payload, ",");
		strcat(payload, sendmode);
		strcat(payload, ",");
		strcat(payload, color);
		strcat(payload, ",");
		strcat(payload, "00");
		strcat(payload, ",");
		strcat(payload, "0005");

		ColourControlMovetoHue(payload);
	}
}

void changeONOFFState(char* nodeid, char* endpoint, char* state, char* sendmode) {
	char* data;
	char* payload;

	//send to group node id
	if ((payload = malloc(
			strlen(nodeid) + strlen(endpoint) + strlen(sendmode) + strlen(state)
					+ 1)) != NULL) {
		payload[0] = '\0';   // ensures the memory is an empty string
		strcat(payload, nodeid);
		strcat(payload, ",");
		strcat(payload, endpoint);
		strcat(payload, ",");
		strcat(payload, sendmode);
		strcat(payload, ",");
		strcat(payload, state);

		SwtichingTargetDevices(payload);
	} else {

		printf("malloc failed\n");
	}

}

void moveToLevel(char* nodeid, char* endpoint, char* levelValue, char* sendmode) {
	char* payload;

	if ((payload = malloc(
			strlen(nodeid) + strlen(endpoint) + strlen(sendmode)
					+ strlen(levelValue) + 128  + 1)) != NULL) {

		strcat(payload, nodeid);
		strcat(payload, ",");
		strcat(payload, endpoint);
		strcat(payload, ",");
		strcat(payload, sendmode);
		strcat(payload, ",");
		strcat(payload, "0");	// It means the command is implemented as Move to Level command
		strcat(payload, ",");
		strcat(payload, levelValue);
		strcat(payload, ",");
		strcat(payload, "0025");

		LevelControlCluster(payload);
	}
	else {
		printf("malloc failed\n");
	}
}
