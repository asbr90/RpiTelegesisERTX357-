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
#define ONOFF_SWITCH	"0x0000"
#define LEVELCONTROL_SWITCH	"0x0001"
#define ONOFF_OUTPUT	"0x0002"
#define	LEVELCONTROLLABLE_OUTPUT	"0x0003"
#define	RANGE_EXTENDER	"0x0008"
#define	MAINS_POWER_OUTLET	"0x0009"
#define	DIMMABLE_LIGHT	"0x0101"


/**
 * @brief	contains a list of all power sockets in the network.
 */
struct Socket_list {
	char* ManufacturerName;
	char* DeviceID;
	char* InputCluster;
	char* OutputCluster;
	char* Endpoint;

	//TODO	List structure to saving the power socket information
	//TODO	split the power socket by company or by other issues?
};

int getSocketInNetwork(void);
void connectSocketToNetwork(void);	//PJOIN command

#endif /*POWERSOCKET_H_*/
