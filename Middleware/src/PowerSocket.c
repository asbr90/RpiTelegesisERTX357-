/**
 *	@author	Artjom Siebert
 *	@date	22.01.2015
 *
 */

#include "PowerSocket.h"

void changeONOFFStateOfSocket(char* nodeid, char* endpoint, char* state,
		char* sendmode) {
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
