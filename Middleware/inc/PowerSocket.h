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

/**
 * @brief	contains a list of all power sockets in the network.
 */
struct Socket_list{
	//TODO	List structure to saving the power socket information
	//TODO	split the power socket by company or by other issues?
};

char* setJoinPermitPermanent(void);

/**
  *	@brief	The functionality of this function is to open the network for permit-join.
  *			Otherwise it is not possible to join into the PAN. The AT command called +PJOIN-Permit joining.
  *	@return	OK if already is fine
  *			ERROR<errorcode> if the command is undeliverable, prompt, NACK will show
  *
  */
char* openNetworkForTime(void);


/**
 * @brief Open network forever. This function should be use for test cases or similar issues. If you like to open the network
 * 		  use the @see openNetworkForTime(void) .
 */
void openNetworkForEver(void);


#endif /*POWERSOCKET_H_*/ 
