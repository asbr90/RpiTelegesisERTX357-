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

/**
 * @brief	Scan Personal Area Network for ZigBee devices. The communication protocol will be ZigBee.
 * 			It is possible to find other devices as like as Philips Hue.
 *
 */
void scanNetwork(void);

#endif /*end define PHILIPSHUE_H_*/
