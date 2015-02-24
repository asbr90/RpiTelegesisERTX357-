/**
 *	@author	Artjom Siebert
 *	@date	22.01.2015
 *
 */

#include "PowerSocket.h"
sockets *powerSocket;

void appendSocket(sockets **lst, char* nodeid, char* endpoint,
		char* ManufacturerName, char* DeviceID, char* InputCluster,
		char* OutputCluster) {
	sockets *neuesElement;
	sockets *lst_iter = *lst;

	neuesElement = (struct Socket_list*) malloc(sizeof(*neuesElement)); // erzeuge ein neues Element
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
