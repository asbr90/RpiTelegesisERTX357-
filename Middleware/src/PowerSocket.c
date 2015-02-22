/**
 *	@author	Artjom Siebert
 *	@date	22.01.2015
 *
 */

#include "PowerSocket.h"

char* changeONOFFStateOfSocket(char* nodeid, char* endpoint, char* state,
		char* sendmode) {
	char* data;
	char* payload;

	//send to group node id
	if ((payload = malloc(
			strlen(nodeid) * sizeof(char) + strlen(endpoint) * sizeof(char)
					+ strlen(sendmode) * sizeof(char)
					+ strlen(state) * sizeof(char) + 4)) != NULL) {
		payload[0] = '\0';   // ensures the memory is an empty string
		strcat(payload, nodeid);
		strcat(payload, ",");
		strcat(payload, endpoint);
		strcat(payload, ",");
		strcat(payload, sendmode);
		strcat(payload, ",");
		strcat(payload, state);

		return SwitchingTargetDevices(payload);
	} else {

		printf("malloc failed\n");
		return "Error: Malloc failed";
	}

}
