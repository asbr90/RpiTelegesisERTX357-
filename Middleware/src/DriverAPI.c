/**
 * @author	Artjom Siebert
 * @date	22.02.2015
 *
 */

#include "DriverAPI.h"

char* changeSocketStateTo(char* nodeid, char* endpoint, char* value,
		char* sendmode) {

	return (char*) changeONOFFStateOfSocket(nodeid, endpoint, value, sendmode);
}

char* distinguishInterface(char* command) {
	char *ptr = strtok(command, "/");
	// set the parameter for the most of commands. If comands does not need the
	// value of this will be NULL
	char* nodeid = strtok(NULL, "/");
	char* endpoint = strtok(NULL, "/");
	char* value = strtok(NULL, "/");
	char* sendmode = strtok(NULL, "/");

	if (strcmp(ptr, CHANGE_SOCKET_STATE_TO) == 0) {
		changeSocketStateTo(nodeid, endpoint, value, sendmode);
		return CHANGE_SOCKET_STATE_TO;
	}

	if (strcmp(ptr, CHANGE_HUE_STATE_TO) == 0) {
		changeONOFFState(nodeid, endpoint, value, sendmode);
		return CHANGE_HUE_STATE_TO;
	}

	if (strcmp(ptr, GET_SOCKET_INFO) == 0)
		return GET_SOCKET_INFO;

	if (strcmp(ptr, GET_HUE_INFO) == 0)
		return GET_HUE_INFO;

	if (strcmp(ptr, GET_LIST_OF_DEVICES) == 0)
		return GET_LIST_OF_DEVICES;

	if (strcmp(ptr, CHANGE_HUE_COLOR_TO) == 0) {
		changeColor(nodeid, endpoint, value, sendmode);
		return CHANGE_HUE_COLOR_TO;
	}

	if (strcmp(ptr, CHANGE_HUE_LEVEL_TO) == 0) {
		moveToLevel(nodeid, endpoint, value, sendmode);
		return CHANGE_HUE_LEVEL_TO;
	}

	if (strcmp(ptr, CHANGE_HUE_SATURATION_TO) == 0) {
		changeSaturation(nodeid, endpoint, value, sendmode);
		return CHANGE_HUE_SATURATION_TO;
	}

	if (strcmp(ptr, OPEN_NETWORK) == 0)
		return OPEN_NETWORK;

	if (strcmp(ptr, ADD_DEVICE_TO_GROUP) == 0)
		return ADD_DEVICE_TO_GROUP;

	if (strcmp(ptr, UPDATE_DEVICE_LIST) == 0) {
		const char* nTable = DisplayNeighbourTable("00,0000");
	/*	char* device = strtok(nTable, "\n");

		while(ptr != NULL || strstr(ptr,"No.") == NULL ) {
			printf("Abschnitt gefunden: %s\n", ptr);
		 	ptr = strtok(NULL, "\n");
		}
*/
		printf("nTable: \n%s\n",nTable);
		return UPDATE_DEVICE_LIST;
	}
	return NULL;
}

