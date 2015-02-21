/**
 *	@author	Artjom Siebert
 *	@date	21.12.2014
 */

#include "ETRX357.h"
#include <malloc.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>

struct nlist *errorEntry;

char *concatCommand(char* command, char* payload) {
	char * cmd;
	if ((cmd = malloc(strlen(":") + strlen(command) + strlen(payload) + 1))
			!= NULL) {
		cmd[0] = '\0';   // ensures the memory is an empty string
		strcat(cmd, command);
		strcat(cmd, ":");
		strcat(cmd, payload);
	} else {
		printf("malloc failed\n");
	}
	return cmd;
}

/* insert new entry in the linked list (insert right)*/
void listPANInsert(nodes **list, int channel, char* PID, char* EPID,
		int stackProfile, int joinPermission) {

	nodes* new_node = (nodes*) malloc(sizeof(struct list_pans));
	new_node->channel = channel;

	new_node->PID = PID;
	new_node->EPID = EPID;
	new_node->stackProfile = stackProfile;
	new_node->joinPermission = joinPermission;
	new_node->next = *list;

	*list = new_node;
}

/*Prompt output*/
void promptRequest(char* cmd) {
	printf("\nRequest: \n<<<<<<<<<<<\n%s\n>>>>>>>>>>>\n", cmd);
}
/*Prompt Input*/
void promptResponse(char* msg) {
	printf("\nResponse: \n<<<<<<<<<<<\n%s\n>>>>>>>>>>>\n", msg);
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
	strtok(error, "\n");
	strtok(NULL, ":");
	error_code = strtok(NULL, "\n");
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

		return (char*) "OK";
	} else {
		return GetErrorCodeMessage(GetErrorCodeNumber(response));
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

		zigbee->channel = atoi(strtok(NULL, ","));
		zigbee->PID = strtok(NULL, ",");
		zigbee->EPID = strtok(NULL, "\n");

		return (char*) OK;
	} else {

		return GetErrorCodeMessage(GetErrorCodeNumber(response));
	}
}

char* ScanForActivePAN(nodes **list) {
	char response[255], rp2[255];
	int channel, stackProfile, joinPermission;
	char* PID, *EPID;

	serialTransmit(ATPANSCAN);
	promptRequest(ATPANSCAN);
	sprintf(response, "%s", serialReceive());
	promptResponse(response);
	delay(4000);// this is the max scanning time. Hint: could be change to interrupt handling?!

	if (IsError(response) == NULL) {
		sprintf(rp2, "%s", serialReceive());
		promptResponse(rp2);

		strtok(rp2, "\n");
		strtok(NULL, ":");

		channel = atoi(strtok(NULL, ","));
		PID = strtok(NULL, ",");
		EPID = strtok(NULL, ",");
		stackProfile = atoi(strtok(NULL, ","));
		joinPermission = atoi(strtok(NULL, "\n"));

		listPANInsert(list, channel, PID, EPID, stackProfile, joinPermission);
		//printf("Channel: %d \n",(*(*list)).channel);
		//printf("PID: %s\n",(*(*list)).PID);

		return (char*) OK;
	} else {
		return GetErrorCodeMessage(GetErrorCodeNumber(response));
	}
}

char* JoinNetwork(zigbee_t* zigbee) {
	char response[255];

	serialTransmit(ATJN);
	promptRequest(ATJN);
	sprintf(response, "%s", serialReceive());
	promptResponse(response);

	delay(4000);// this is the max scanning time. Hint: could be change to interrupt handling?!

	if (IsError(response) == NULL) {
		//promptResponse(rp2);
		//!IMPORTANT!! This part should be tested and the received value 
		// set to the zigbee struct

		printf("Response Join Network: %s\n", strtok(response, ":"));
		printf("%s\n", strtok(NULL, ","));
		printf("%s\n", strtok(NULL, ","));
		printf("%s\n", strtok(NULL, "\n"));

		return (char*) OK;
	} else {
		return GetErrorCodeMessage(GetErrorCodeNumber(response));
	}
}

char* DisplayNetworkInformation(void) {
	char response[255];

	serialTransmit(ATN);
	promptRequest(ATN);
	sprintf(response, "%s", serialReceive());
	promptResponse(response);

	strtok(response, "\n");

	if (strstr(strtok(NULL, "\n"), "NoPAN"))
		return "NoPAN";
	else {
		printf("%s\n", strtok(NULL, "\n"));
		return OK;
	}
}

/*Not tested*/
char* RejoinNetwork(char* b) {
	char response[255];
	char * command;

	if ((command = malloc(strlen(":") + strlen(ATREJOIN) + strlen(b) + 1))
			!= NULL) {
		command[0] = '\0';   // ensures the memory is an empty string
		strcat(command, ATREJOIN);
		strcat(command, ":");
		strcat(command, b);
	} else {
		printf("malloc failed!\n");
	}

	serialTransmit(command);
	promptRequest(command);
	sprintf(response, "%s", serialReceive());
	promptResponse(response);

	if (IsError(response) == NULL) {
		return (char*) OK;
	} else {
		return GetErrorCodeMessage(GetErrorCodeNumber(response));
	}
}

char* ChangeNetworkChannel(char* dB) {
	char response[255];
	char * command;

	if ((command = malloc(strlen(":") + strlen(ATCCHANGE) + strlen(dB) + 1))
			!= NULL) {
		command[0] = '\0';   // ensures the memory is an empty string
		strcat(command, ATCCHANGE);
		if (strcmp(dB, "00") != 0) {
			strcat(command, ":");
			strcat(command, dB);
		}
	} else {
		printf("malloc failed!\n");
	}

	serialTransmit(command);
	promptRequest(command);
	sprintf(response, "%s", serialReceive());
	promptResponse(response);

	if (IsError(response) == NULL) {
		return (char*) OK;
	} else {
		return GetErrorCodeMessage(GetErrorCodeNumber(response));
	}
}

void DisplayNeighbourTable(char* payload) {
	char response[512];
	char* cmd = concatCommand(ATNTABLE, payload);

	serialTransmit(cmd);
	promptRequest(cmd);
	delay(4000); // this is the max scanning time. Hint: could be change to interrupt handling?!

	sprintf(response, "%s", serialReceive());
	promptResponse(response);

	if (IsError(response) != NULL)
		printf("%s\n", GetErrorCodeMessage(GetErrorCodeNumber(response)));
}

char* SendRAWZCLMessagetoTarget(char* payload) {
	char response[255];
	char* cmd = concatCommand(ATRAWZCL, payload);

	serialTransmit(cmd);
	promptRequest(cmd);
	delay(2000); // this is the max scanning time. Hint: could be change to interrupt handling?!
	sprintf(response, "%s", serialReceive());
	promptResponse(response);

	if (IsError(response) == NULL)
		return (char*) OK;
	else
		return GetErrorCodeMessage(GetErrorCodeNumber(response));
}

char* AddGroupOnTargetDevice(char* payload) {
	char response[255];
	char* cmd = concatCommand(GPADD, payload);

	serialTransmit(cmd);
	promptRequest(cmd);
	delay(2000); // this is the max scanning time. Hint: could be change to interrupt handling?!
	sprintf(response, "%s", serialReceive());
	promptResponse(response);

	if (IsError(response) == NULL)
		return (char*) OK;
	else
		return GetErrorCodeMessage(GetErrorCodeNumber(response));
}

char* SwtichingTargetDevices(char* payload) {
	char response[255];
	char* cmd = concatCommand(RONOFF, payload);

	serialTransmit(cmd);
	promptRequest(cmd);
	delay(1000); // this is the max scanning time. Hint: could be change to interrupt handling?!
	sprintf(response, "%s", serialReceive());
	promptResponse(response);

	if (IsError(response) == NULL)
		return (char*) OK;
	else
		return GetErrorCodeMessage(GetErrorCodeNumber(response));
}

char* ColourControlMovetoHue(char* payload) {
	char response[255];
	char* cmd = concatCommand(CCMVTOHUE, payload);

	serialTransmit(cmd);
	promptRequest(cmd);
	delay(1000); // this is the max scanning time. Hint: could be change to interrupt handling?!
	sprintf(response, "%s", serialReceive());
	promptResponse(response);

	if (IsError(response) == NULL)
		return (char*) OK;
	else
		return GetErrorCodeMessage(GetErrorCodeNumber(response));
}

char* LevelControlCluster(char* payload){
	char response[255];
	char* cmd = concatCommand(LCMVTOLEV, payload);

	serialTransmit(cmd);
	promptRequest(cmd);
	delay(1000); // this is the max scanning time. Hint: could be change to interrupt handling?!
	sprintf(response, "%s", serialReceive());
	promptResponse(response);

	if (IsError(response) == NULL)
		return (char*) OK;
	else
		return GetErrorCodeMessage(GetErrorCodeNumber(response));
}
