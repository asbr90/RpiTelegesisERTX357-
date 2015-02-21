/**
 * @author	Artjom Siebert
 * @date	01.01.2015
 *
 */

#include "HUE.h"

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
