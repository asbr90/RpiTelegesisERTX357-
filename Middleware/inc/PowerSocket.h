/**
  *	@author	Artjom Siebert
  *	@date	22.01.2015
  *	
  *	@breif	This module contains the functionality for power Socket from netvox. That connect and switch the power socker.  
  *			Z809A is a smart plug that allows off site remote control. Users can monitor the load current, voltage and power 
  *			and kilo-watt-hour consumption of the attached appliance and determine if the device is overloaded. 
  */

#ifndef	POWERSOCKET_H_
#define POWERSOCKET_H_

  
char* setJoinPermitPermanent(void);

/**
  *	@param time is the time which define the time-slot for opened network
  */
char* openNetworkForTime(short); 

#endif /*POWERSOCKET_H_*/ 