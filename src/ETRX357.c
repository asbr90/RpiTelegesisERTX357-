/**
 *	@author	Artjom Siebert
 *	@date	21.12.2014
 */

#include "ETRX357.h"
#include <malloc.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

/* insert new entry in the linked list (insert right)*/
void listPANInsert(node **list, int channel, char* PID, char* EPID, int stackProfile, int joinPermission){

	node* new_node				= (node*) malloc(sizeof(struct list_pans));
	new_node->channel 			= channel;

	new_node->PID 				= PID;
	new_node->EPID 				= EPID;
	new_node->stackProfile 		= stackProfile;
	new_node->joinPermission 	= joinPermission;
	new_node->next				= *list;

	*list     					= new_node;

}

/*Prompt output*/
void promptRequest(char* cmd){
	printf("Request << \n\t%s\n>>\n",cmd);
}

void promptResponse(char* msg){
	printf("Response << \n\t%s\n>>\n",msg);
}

/*Trim whitespaces*/
char *strstrip(char *s) {
	size_t size;
	char *end;

	size = strlen(s);

	if (!size)
		return s;

	end = s + size - 1;
	while (end >= s && isspace(*end))
		end--;
	*(end + 1) = '\0';

	while (*s && isspace(*s))
		s++;

	return s;
}

char* IsError(char* response) {
	return strstr(response, ERROR);
}

short InitializeSerialPort(char* COM) {
	if (wiringPiSetup() != -1) {
		printf("Initialization was successfull\n");
		serialOpenPort(COM, 19200);
		serialSetSettings(B19200, 8, 1, 0, 0); //connect to the appropriate COM port at
											   //19200bps, Data bits - 8, Parity - none, Stop bits - 1, Flow Control – none
		return true;
	} else {
		printf("wiringPi library could not set up\n");
		return false;
	}
}

int ProductIdentificationInformation(telegesis_t *info) {
	char response[255];

	serialTransmit(ATI);
	sprintf(response, "%s", serialReceive());

	//interpret response message
	strtok(response, "\n");		// remove AT command
	info->deviceName = strtok(NULL, "\n");
	info->firmwareRevision = strtok(NULL, "\n");
	info->EUID = strtok(NULL, "\n");

	return OK;

}

char* GetErrorCodeNumber(char* error) {
	char* error_code;
	strtok(error, ":");
	error_code = strtok(NULL, "\0");
	return error_code;
}

void bootload(void) {
	//initialize error code lookup table
	int errorsize = sizeof(error) / sizeof(error[0]);
	int errortextsize = sizeof(error_list) / sizeof(error_list[0]);
	int index;

	if (errorsize == errortextsize) {
		printf("Initialize lookup table for error handling with %d entries\n",
				errorsize);

		for (index = 0; index < errorsize; index++) {
			install(error[index], error_list[index]);
		}
	}
}

char* GetErrorCodeMessage(char* errorNumber) {

	errorEntry = lookup(strstrip(errorNumber));

	if (errorEntry == NULL)
		printf("There is no entry for this error code\n");

	return errorEntry->defn;
}

char* DisassociateLocalDeviceFromPAN(zigbee_t* zigbee) {
	char response[255], rp2[255];

	serialTransmit(ATDASSL);
	promptRequest(ATDASSL);
	sprintf(response, "%s", serialReceive());
	promptResponse(response);

	if (IsError(response) == NULL) {
		serialReceive();
		sprintf(rp2, "%s", serialReceive());
		promptResponse(rp2);

		return (char*)"OK";
	} else {
		strtok(response, "\n");
		return GetErrorCodeNumber(strtok(NULL, "\n"));
	}
}

char* EstablishPAN(zigbee_t* zigbee) {
	char response[255], rp2[255];

	serialTransmit(ATEN);
	promptRequest(ATEN);
	sprintf(response, "%s", serialReceive());
	promptResponse(response);

	if (IsError(response) == NULL) {
		sprintf(rp2, "%s", serialReceive());
		promptResponse(rp2);

		strtok(rp2, "\n");
		strtok(NULL, "\n");
		strtok(NULL, ":");

		zigbee->channel =  atoi(strtok(NULL, ","));
		zigbee->PID = strtok(NULL, ",");
		zigbee->EPID = strtok(NULL, "\n");

		return (char*)OK;
	} else {
		strtok(response, "\n");
		return GetErrorCodeNumber(strtok(NULL, "\n"));
	}
}

char* ScanForActivePAN(node **list){
	char response[255], rp2[255];
	int channel, stackProfile, joinPermission;
	char* PID, *EPID;
	node *n;

	serialTransmit(ATPANSCAN);
	promptRequest(ATPANSCAN);
	sprintf(response, "%s", serialReceive());
	promptResponse(response);
	delay(4000);		// this is the max scanning time.Hint: could be change to interrupt handling?!

	if (IsError(response) == NULL) {
		sprintf(rp2, "%s", serialReceive());
		promptResponse(rp2);

		strtok(rp2, "\n");
		strtok(NULL, ":");

		channel = atoi(strtok(NULL, ","));
		PID = strtok(NULL, ",");
		EPID = strtok(NULL, ",");
		stackProfile =  atoi(strtok(NULL, ","));
		joinPermission = atoi(strtok(NULL, "\n"));

		listPANInsert(list, channel, PID, EPID, stackProfile, joinPermission);
		//printf("Channel: %d \n",(*(*list)).channel);
		//printf("PID: %s\n",(*(*list)).PID);

		return (char*)OK;
	} else {
		strtok(response, "\n");
		return GetErrorCodeNumber(strtok(NULL, "\n"));
	}

}
