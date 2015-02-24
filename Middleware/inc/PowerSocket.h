/**
 *	@author	Artjom Siebert
 *	@date	22.01.2015
 *
 *	@breif	This module contains the functionality for power Socket from netvox. That connect and switch the power socket.
 *			Z809 and Z800 are a smart plug that allows off site remote control. Users can monitor the load current, voltage and power
 *			and kilo-watt-hour consumption of the attached appliance and determine if the device is overloaded.
 */

#ifndef	POWERSOCKET_H_
#define POWERSOCKET_H_
#include "ETRX357.h"

//Device Ids
#define ONOFF_SWITCH	"0000"
#define LEVELCONTROL_SWITCH	"0001"
#define ONOFF_OUTPUT	"0002"
#define	LEVELCONTROLLABLE_OUTPUT	"0003"
#define	RANGE_EXTENDER	"0008"
#define	MAINS_POWER_OUTLET	"0009"
#define	DIMMABLE_LIGHT	"0101"


/**
 * @brief	contains a list of all power sockets in the network.
 */
typedef struct Socket_list {
	char* NodeID;
	char* ManufacturerName;
	char* ep;
	char* DeviceID;
	char* InputCluster;
	char* OutputCluster;
	struct Socket_list *next;
} sockets;
extern sockets *powerSocket;
int getSocketInNetwork(void);
char* connectSocketToNetwork(void);	//PJOIN command
void appendSocket(sockets **lst, char* nodeid, char* endpoint,
		char* ManufacturerName, char* DeviceID, char* InputCluster,
		char* OutputCluste);

#endif /*POWERSOCKET_H_*/
