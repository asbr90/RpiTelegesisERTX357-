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

char* ProductIdentificationInformation(telegesis_t *info) {
	char response[255];

	serialTransmit(ATI);
	sprintf(response, "%s", serialReceive());

	//interpret response message
	strtok(response, "\n");		// remove AT command
	info->deviceName = strtok(NULL, "\n");
	info->firmwareRevision = strtok(NULL, "\n");
	info->EUID = strtok(NULL, "\n");

	return (char*) OK;

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

char* DisplayNeighbourTable(char* payload) {
	char* response = (char*) malloc(512 * sizeof(char));
	char* cmd = concatCommand(ATNTABLE, payload);

	serialTransmit(cmd);
	promptRequest(cmd);
	delay(1000); // this is the max scanning time. Hint: could be change to interrupt handling?!
	sprintf(response, "%s", serialReceive());
	promptResponse(response);
	if (IsError(response) != NULL)
		printf("%s\n", GetErrorCodeMessage(GetErrorCodeNumber(response)));
	else {
		return (char*) response;
	}
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

char* RequestNodesActiveEndpoints(char* payload) {

	char* response = (char*) malloc(1024 * sizeof(char));
	char* cmd = concatCommand(ATACTEPDESC, payload);

	serialTransmit(cmd);
	promptRequest(cmd);
	delay(500); // this is the max scanning time. Hint: could be change to interrupt handling?!
	sprintf(response, "%s", serialReceive());
	promptResponse(response);

	if (IsError(response) == NULL)
		return response;
	else
		return GetErrorCodeMessage(GetErrorCodeNumber(response));
}

char* RequestEndpointSimpleDescriptor(char* payload) {

	char* response = (char*) malloc(512 * sizeof(char));
	char* cmd = concatCommand(ATSIMPLEDESC, payload);

	serialTransmit(cmd);
	promptRequest(cmd);
	delay(500); // this is the max scanning time. Hint: could be change to interrupt handling?!
	sprintf(response, "%s", serialReceive());
	promptResponse(response);

	if (IsError(response) == NULL)
		return response;
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

char* SwitchingTargetDevices(char* payload) {
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

char* ColourControlMoveToSaturation(char* payload) {
	char response[255];
	char* cmd = concatCommand(CCMVTOSAT, payload);

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

char* ColourControlMovetoHueSaturation(char* payload) {
	char response[255];
	char* cmd = concatCommand(CCMVTOHUS, payload);

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

char* LevelControlCluster(char* payload) {
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

void changeSaturation(char* nodeid, char* endpoint, char* sat, char* sendmode) {
	char* payload;
	if ((payload = malloc(
			(strlen(nodeid) + strlen(endpoint) + strlen(sendmode) + strlen(sat))
					* sizeof(char) + 12 + 1)) != NULL) {
		payload[0] = '\0';   // ensures the memory is an empty string
		strcat(payload, nodeid);
		strcat(payload, ",");
		strcat(payload, endpoint);
		strcat(payload, ",");
		strcat(payload, sendmode);
		strcat(payload, ",");
		strcat(payload, sat);
		strcat(payload, ",");
		strcat(payload, "0005");

		ColourControlMoveToSaturation(payload);
	}
}

void changeColor(char* nodeid, char* endpoint, char* color, char* sendmode) {
	char* payload;
	if ((payload = malloc(
			(strlen(nodeid) + strlen(endpoint) + strlen(sendmode)
					+ strlen(color)) * sizeof(char) + 12 + 1)) != NULL) {
		payload[0] = '\0';   // ensures the memory is an empty string
		strcat(payload, nodeid);
		strcat(payload, ",");
		strcat(payload, endpoint);
		strcat(payload, ",");
		strcat(payload, sendmode);
		strcat(payload, ",");
		strcat(payload, color);
		strcat(payload, ",");
		strcat(payload, "00");
		strcat(payload, ",");
		strcat(payload, "0005");

		ColourControlMovetoHue(payload);
	}
}

void changeONOFFState(char* nodeid, char* endpoint, char* state, char* sendmode) {
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

		SwitchingTargetDevices(payload);
	} else {

		printf("malloc failed\n");
	}

}

void moveToLevel(char* nodeid, char* endpoint, char* levelValue, char* sendmode) {
	char* payload;

	if ((payload = malloc(
			strlen(nodeid) + strlen(endpoint) + strlen(sendmode)
					+ strlen(levelValue) + 128 + 1)) != NULL) {

		strcat(payload, nodeid);
		strcat(payload, ",");
		strcat(payload, endpoint);
		strcat(payload, ",");
		strcat(payload, sendmode);
		strcat(payload, ",");
		strcat(payload, "0"); // It means the command is implemented as Move to Level command
		strcat(payload, ",");
		strcat(payload, levelValue);
		strcat(payload, ",");
		strcat(payload, "0025");

		LevelControlCluster(payload);
	} else {
		printf("malloc failed\n");
	}
}

char* getEndPoint(char* nodeid) {

	char response[512];
	char* payload;
	char* ep;
	payload = (char*) malloc(strlen(nodeid) * sizeof(char) + 2);

	strcat(payload, nodeid);
	strcat(payload, ",");
	strcat(payload, nodeid);

	char* cmd = concatCommand(ATACTEPDESC, payload);

	serialTransmit(cmd);
	delay(2000); // this is the max scanning time. Hint: could be change to interrupt handling?!
	sprintf(response, "%s", serialReceive());
	promptResponse(response);

	char *ptr = strtok(response, "\n");

	while (ptr != NULL) {
		if (strstr(ptr, "ActEpDesc")) {
			ep = ptr;
			ptr = NULL;
		}
		ptr = strtok(NULL, "\n");
	}

	ptr = strtok(ep, ":");
	ptr = strtok(NULL, ",");
	ptr = strtok(NULL, ",");
	ptr = strtok(NULL, ",");	// last response of actepdesc

	free(payload);
	return ptr;
}

char* getDeviceID(const char* nodeid, const char* endpoint) {
	char* payload;
	char* simpledesc, *deviceId;

	payload = (char*) malloc(
			(2 * strlen(nodeid) + strlen(endpoint)) * sizeof(char) + 4);

	strcat(payload, nodeid);
	strcat(payload, ",");
	strcat(payload, nodeid);
	strcat(payload, ",");
	strcat(payload, endpoint);

	simpledesc = RequestEndpointSimpleDescriptor(payload);

	char *ptr = strtok(simpledesc, "\n");

	while (ptr != NULL) {
		if (strstr(ptr, "DeviceID")) {
			deviceId = ptr;
			ptr = NULL;
		}
		ptr = strtok(NULL, "\n");
	}
	ptr = strtok(deviceId, ":");
	ptr = strtok(NULL, "\n");

	//free(payload);
	return ptr;
}

char* getInCluster(char* nodeid, char* endpoint) {
	char* payload;
	char* simpledesc, *incluster;

	payload = (char*) malloc(2 * strlen(nodeid) * sizeof(char*) + 6);

	strcat(payload, nodeid);
	strcat(payload, ",");
	strcat(payload, nodeid);
	strcat(payload, ",");
	strcat(payload, endpoint);

	simpledesc = RequestEndpointSimpleDescriptor(payload);
	char *ptr = strtok(simpledesc, "\n");

	while (ptr != NULL) {
		if (strstr(ptr, "InCluster")) {
			incluster = ptr;
			ptr = NULL;
		}
		ptr = strtok(NULL, "\n");
	}
	ptr = strtok(incluster, ":");
	ptr = strtok(NULL, "\n");

	//free(payload);
	return ptr;
}

char* getManufacturerName(char* nodeid, char* endpoint) {
	char* payload = (char*) malloc(128 * sizeof(char));
	char* simpledesc, *deviceId;
	char* response = (char*) malloc(512 * sizeof(char));
	char* restattr;

	strcat(payload, nodeid);
	strcat(payload, ",");
	strcat(payload, endpoint);
	strcat(payload, ",");
	strcat(payload, "0");
	strcat(payload, ",");
	strcat(payload, "0000");
	strcat(payload, ",");
	strcat(payload, "0004");

	printf(" %s\n", payload);
	char* cmd = concatCommand(READATR, payload);

	serialTransmit(cmd);
	delay(2000); // this is the max scanning time. Hint: could be change to interrupt handling?!
	sprintf(response, "%s", serialReceive());
	promptResponse(response);
	char *ptr = strtok(response, "\n");

	while (ptr != NULL) {
		if (strstr(ptr, "RESPATTR")) {
			restattr = ptr;
			ptr = NULL;
		}
		ptr = strtok(NULL, "\n");
	}

	ptr = strtok(restattr, ":");
	ptr = strtok(NULL, ",");
	ptr = strtok(NULL, ",");
	ptr = strtok(NULL, ",");
	ptr = strtok(NULL, ",");
	ptr = strtok(NULL, ",");
	ptr = strtok(NULL, ",");

	if (IsError(response) == NULL)
		return ptr;
	else
		return GetErrorCodeMessage(GetErrorCodeNumber(response));
}

char* PermitJoining(char* payload) {
	char* response =(char*) malloc(128*sizeof(char)) ;
	char* cmd = concatCommand(PJOIN, payload);

	serialTransmit(cmd);
	promptRequest(cmd);
	delay(250); // this is the max scanning time. Hint: could be change to interrupt handling?!
	sprintf(response, "%s", serialReceive());
	promptResponse(response);

	if (IsError(response) == NULL)
		return (char*) OK;
	else
		return GetErrorCodeMessage(GetErrorCodeNumber(response));
}

char* RemoveGroupMembershipOnTargetDevice(char* payload){
	char* response = (char*) malloc(128*sizeof(char));

	char* cmd = concatCommand(GPRMV, payload);

	serialTransmit(cmd);
	promptRequest(cmd);
	delay(250); // this is the max scanning time. Hint: could be change to interrupt handling?!
	sprintf(response, "%s", serialReceive());
	promptResponse(response);

	if (IsError(response) == NULL)
		return (char*) OK;
	else
		return GetErrorCodeMessage(GetErrorCodeNumber(response));
}
