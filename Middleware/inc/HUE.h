/**
 * @author	Artjom Siebert
 * @date	01.01.2015
 *
 * @brief	Provide the functionality of Philips hue bulb. With this library you can use the features of Philips hue.
 * 			It is possible that the scope of functions is not complete. Furthermore, this library use the ETRX357 zigbee modul
 * 			to interact with the bulb(s). More information about this modul and his firmware you will find in the @see ETRX357.h.
 */

#ifndef PHILIPSHUE_H_
#define PHILIPSHUE_H_

#include "ETRX357.h"

#define ONOFFLIGHT	"0100"
#define COLORDIMABLELIGHT	"0102"
#define ONOFFLIGHTSWITCH	"0103"
#define DIMMERSWITCH	"0104"
#define COLORDIMERSWITCH	"0105"
#define LIGHTSENSOR	"0106"
#define OCCUPANCYSENSOR	"0107"
#define	DIMMABLE_LIGHT	"0101"
#define DimmablePluginUnit "0110"
#define ColourLight "0200"
#define ExtendedColourLight "0210"
#define ColourTemperatureLight "0220"
/**
 * @brief	contains a list of all power sockets in the network.
 */
typedef struct hue_list {
	char* NodeID;
	char* ManufacturerName;
	char* ep;
	char* DeviceID;
	char* GroupID;
	char* InputCluster;
	char* OutputCluster;
	struct hue_list *next;
} hue;

extern hue* huesList;
void appendHuet(hue **lst, char* nodeid, char* endpoint,
		char* ManufacturerName, char* DeviceID, char* InputCluster,
		char* OutputCluste);
#endif /*end define PHILIPSHUE_H_*/
