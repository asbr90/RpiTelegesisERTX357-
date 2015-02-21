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
/*
 * Lighting
On/Off Light 0x0100
Color Dimmable Light 0x0102
On/Off Light Switch 0x0103
Dimmer Switch 0x0104
Color Dimmer Switch 0x0105
Light Sensor 0x0106
Occupancy Sensor 0x0107
*/
/**
 * @brief	contains a list of all power sockets in the network.
 */
struct hue_list {
	//TODO	List structure to saving the power socket information
	//TODO	split the power socket by company or by other issues?
};

#endif /*end define PHILIPSHUE_H_*/
