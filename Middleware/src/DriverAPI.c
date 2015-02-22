/**
 * @author	Artjom Siebert
 * @date	22.02.2015
 *
 */

#include "DriverAPI.h"

char* changeSocketStateTo(char* nodeid, char* endpoint, char* value,
		char* sendmode) {

	return changeONOFFStateOfSocket(nodeid, endpoint, value, sendmode);
}

char* changeHueStateTo(char* nodeid, char* endpoint, char* value) {

}

char* distinguishInterface(char* command) {
	char *ptr = strtok(command, "/");

	if (strcmp(ptr, CHANGE_SOCKET_STATE_TO) == 0)
		return CHANGE_SOCKET_STATE_TO;

	if (strcmp(ptr, CHANGE_HUE_STATE_TO) == 0)
		return CHANGE_HUE_STATE_TO;

	if (strcmp(ptr, GET_SOCKET_INFO) == 0)
		return GET_SOCKET_INFO;

	if (strcmp(ptr, GET_HUE_INFO) == 0)
		return GET_HUE_INFO;

	if (strcmp(ptr, GET_LIST_OF_DEVICES) == 0)
		return GET_LIST_OF_DEVICES;

	if (strcmp(ptr, CHANGE_HUE_COLOR_TO) == 0)
		return CHANGE_HUE_COLOR_TO;

	if (strcmp(ptr, CHANGE_HUE_LEVEL_TO) == 0)
		return CHANGE_HUE_LEVEL_TO;

	if (strcmp(ptr, OPEN_NETWORK) == 0)
		return OPEN_NETWORK;

	if (strcmp(ptr, ADD_DEVICE_TO_GROUP) == 0)
		return ADD_DEVICE_TO_GROUP;

	return NULL;
}

