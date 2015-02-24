/**
 * @author	Artjom Siebert
 * @date	01.01.2015
 *
 */

#include "HUE.h"
hue* huesList;

void appendHue(hue **lst, char* nodeid, char* endpoint, char* ManufacturerName,
		char* DeviceID, char* InputCluster, char* OutputCluster) {
	hue *neuesElement;
	hue *lst_iter = *lst;

	neuesElement = (struct hue_list*) malloc(sizeof(*neuesElement)); // erzeuge ein neues Element
	neuesElement->NodeID = nodeid;
	neuesElement->ep = endpoint;
	neuesElement->ManufacturerName = ManufacturerName;
	neuesElement->DeviceID = DeviceID;
	neuesElement->InputCluster = InputCluster;
	neuesElement->OutputCluster = OutputCluster;
	neuesElement->next = NULL; // Wichtig für das Erkennen des Listenendes

	if (lst_iter != NULL) { // sind Elemente vorhanden
		while (lst_iter->next != NULL) // suche das letzte Element
			lst_iter = lst_iter->next;
		lst_iter->next = neuesElement; // Hänge das Element hinten an
	} else
		// wenn die liste leer ist, bin ich das erste Element
		*lst = neuesElement;
}

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
