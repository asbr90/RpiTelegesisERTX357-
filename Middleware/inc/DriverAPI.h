/**
 * @author	Artjom Siebert
 * @date	21.02.2015
 *
 * @brief	This header contains the api for the driver which communicate with End devices.
 */

#ifndef _DRIVER_API_H_
#define _DRIVER_API_H_

#include "HUE.h"
#include "PowerSocket.h"

#define CHANGE_SOCKET_STATE_TO	"SocketState"	/**<Change the socket state to declared value. */
#define GET_SOCKET_INFO	"SocketInformation"	/**<Get the information of the current socket*/
#define GET_HUE_INFO	"HueInformation"	/**<Get the information of the current hue*/
#define	GET_LIST_OF_DEVICES	"DeviceList"	/**<Get all connected devices in the network*/
#define CHANGE_HUE_COLOR_TO	"HueColor"		/**<Cahnge the color of hue to declared value*/
#define CHANGE_HUE_LEVEL_TO	"HueLevel"		/**<Change the level of hue to declared value*/
#define CHANGE_HUE_STATE_TO	"HueState"		/**<Change the hue state to declared value*/
#define OPEN_NETWORK	"OpenNetwork"		/**<Open the network for 60s. In this time new devices can connect to network*/
#define ADD_DEVICE_TO_GROUP	"AddToGroup"	/**<Add a device to Group*/

/**
 * @brief	Function that distinguish the api command.
 * @param[in]	The parameter should be one of the API commands
 * @return	Return the api command. If the value is nor defined, it returns NULL.
 */
char* distinguishInterface(char*);
char* changeSocketStateTo(char*, char*, char*, char*);
#endif
