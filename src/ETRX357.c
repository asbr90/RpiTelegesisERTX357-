/**
*	@author	Artjom Siebert
*	@date	21.12.2014	
*/

#include "ETRX357.h"

short InitializeSerialPort(char* COM){
	if (wiringPiSetup() != -1) {
		printf("Initialization was successfull\n");
		serialOpenPort(COM,19200);
		serialSetSettings(B19200, 8, 1, 0, 0);	//connect to the appropriate COM port at 
												//19200bps, Data bits - 8, Parity - none, Stop bits - 1, Flow Control – none 
		return true;
	} else {
		printf("wiringPi library could not set up\n");
		return false;
	}
}

int ProductIdentificationInformation(telegesis_t *telegesis){
	char response[255];

	serialTransmit(ATI);
	return OK;
}