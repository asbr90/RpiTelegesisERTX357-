/**
 * @author	Artjom Siebert
 * @date	01.01.2015
 *
 * @brief	Provide the functionality of Philips hue bulb.
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
