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

#define BASICCLUSTER	"0000"
#define ONOFCLUSTER		"0006"
#define COLORCLUSTER	"0300"
#define LEVELCLUSTER	"0008"

#define OFF 	"00"
#define ON		"01"
#define TOGGLE	"02"

#define OFF_GROUP	"0"
#define ON_GROUP	"1"

#define SEND_TO_GROUP	"1"
#define SEND_TO_TARGET	"0"

void changeONOFFState(char*, char*, char*, char*);
void changeColor(char*, char*, char*, char*);
void moveToLevel(char*, char*, char*, char*);

#endif /*end define PHILIPSHUE_H_*/
