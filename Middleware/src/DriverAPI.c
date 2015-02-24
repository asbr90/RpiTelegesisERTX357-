/**
 * @author	Artjom Siebert
 * @date	22.02.2015
 *
 */

#include "DriverAPI.h"

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
			|| strcmp(deviceid, OCCUPANCYSENSOR) == 0
			|| strcmp(deviceid, DIMMABLE_LIGHT) == 0
			|| strcmp(deviceid, DimmablePluginUnit) == 0
			|| strcmp(deviceid, ColourLight) == 0
			|| strcmp(deviceid, ExtendedColourLight) == 0
			|| strcmp(deviceid, ExtendedColourLight) == 0)
		return 1;
	else
		return 0;
}

char* changeSocketStateTo(char* nodeid, char* endpoint, char* value,
		char* sendmode) {

	return (char*) changeONOFFStateOfSocket(nodeid, endpoint, value, sendmode);
}

char *trimwhitespace(char *str) {
	char *end;

// Trim leading space
	while (isspace(*str))
		str++;

	if (*str == 0)  // All spaces?
		return str;

// Trim trailing space
	end = str + strlen(str) - 1;
	while (end > str && isspace(*end))
		end--;

// Write new null terminator
	*(end + 1) = 0;

	return str;
}

char* parseID(void) {
	char* id = (char*) malloc(5 * sizeof(char));
	int i;
	char* ptr;

	for (i = 0; i < 4; i++) {
		if ((ptr = strtok(NULL, "|")) == NULL)
			return NULL;
	}

	id = trimwhitespace(ptr);
	return id;
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
		char* nTable = DisplayNeighbourTable("00,0000");
		char* device = strtok(nTable, "\n");
		char* payloadActiveEndpoint = (char*) malloc(10 * sizeof(char));
		char* responseActiveEndpoint;
		char* ptr, *currentptr;
		char* deviceIDList[100];
		char* deviceEndpointList[100];
		char* deviceID;

		int index = 0, i;

		while ((device != NULL) && (strstr(device, "No.") == NULL))
			device = strtok(NULL, "\n");

		while ((device = parseID()) != NULL) {
			strcat(payloadActiveEndpoint, device);
			strcat(payloadActiveEndpoint, ",");
			strcat(payloadActiveEndpoint, device);

			printf("payloadActiveEndpoint: %s\n", payloadActiveEndpoint);

			responseActiveEndpoint = RequestNodesActiveEndpoints(
					payloadActiveEndpoint);

			responseActiveEndpoint = strstr(responseActiveEndpoint,
					"ActEpDesc");

			printf("responseActiveEndpoint: %s\n", responseActiveEndpoint);
			deviceIDList[index] = device;
			deviceEndpointList[index] = responseActiveEndpoint;
			index++;
			payloadActiveEndpoint = NULL;
			payloadActiveEndpoint = (char*) malloc(20 * sizeof(char));
		}

		for (i = 0; i < index; i++) {
			ptr = strtok(deviceEndpointList[i], ":");
			ptr = strtok(NULL, ",");
			ptr = strtok(NULL, ",");
			ptr = strtok(NULL, "\n");	// last response of actepdesc
			deviceEndpointList[i] = ptr;
			deviceID = getDeviceID(deviceIDList[i], deviceEndpointList[i]);
			if (isDeviceSocket(deviceID)) {
				//set socket list
				appendSocket(&powerSocket, deviceIDList[i],
						deviceEndpointList[i],
						getManufacturerName(deviceIDList[i],
								deviceEndpointList[i]), deviceID,
						getInCluster(deviceIDList[i], deviceEndpointList[i]),
						"");
				printf("Device is a socket\n");
				printf(
						"Endpoint: %s\nDevice ID: %s\nInput Cluster: %s\nManufacturer Name: %s\n",
						powerSocket->ep, powerSocket->DeviceID,
						powerSocket->InputCluster,
						powerSocket->ManufacturerName);

			} else if (isDeviceHue(deviceID)) {
				appendHue(&huesList, deviceIDList[i], deviceEndpointList[i],
						getManufacturerName(deviceIDList[i],
								deviceEndpointList[i]), deviceID,
						getInCluster(deviceIDList[i], deviceEndpointList[i]),
						"");
				printf("Device is a Hue\n");
				printf(
						"Endpoint: %s\nDevice ID: %s\nInput Cluster: %s\nManufacturer Name: %s\n",
						huesList->ep, huesList->DeviceID,
						huesList->InputCluster, huesList->ManufacturerName);
			}
		}

		return UPDATE_DEVICE_LIST;
	}
	return NULL;
}

