/**
 *	@author	Artjom Siebert
 *	@date	21.12.2014
 *
 *	@brief	Include specific commands and functions for the telegesis ETRX357 ZigBee module.
 *			Each command must be preceded by the "AT" or "at" prefix. To terminate a command enter
 *			<CR>. Any data not following this pattern is either not accepted by the module or will cause an
 *			error message in response. Every command must be terminated with a <CR>, they cannot be
 *			concatenated.
 */

#ifndef ETRX357_H_
#define ETRX357_H_

#include "serialDriver.h"
#include "LUT.h"

/*ZigBee types*/

/*AT Commands*/
/*Module control and configuratio*/
#define ATI				"ATI"			/**<Display Product Identification Information*/
#define ATZ 			"ATZ"			/**<Software Reset*/
#define ATF 			"AT&F" 			/**<Restore Factory Defaults*/
#define ATBLOAD 		"AT+BLOAD" 		/**<Enter The Bootloader Menu*/
#define ATClone			"AT+CLONE" 		/**<Clone Local Node To Remote Node (ETRX2 Series Only)*/
#define ATPASSTHROUGH 	"AT+PASSTHROUGH"/**<Pass new Firmware Image To Remote Node (ETRX3 Series Only)*/
#define ATRECOVER 		"AT+Recover"	/**<Recover From A Failed Clone Attempt*/
#define ATS    			"ATS"			/**<S-Register Access "S-R" */
#define ATREMS			"ATREMS"		/**<Remote S-Register Access*/
#define ATSALL			"ATSALL"		/**<Remote S-Register Access*/
#define ATTOKDUMP		"AT+TOKDUMP" 	/**<Display All S-Registers*/

/*Network control and configuration*/
#define ATESCAN 		"AT+ESCAN"		/**<Scan The Energy Of All Channels */
#define ATPANSCAN 		"AT+PANSCAN"	/**<Scan For Active Pans*/
#define ATEN 			"AT+EN"			/**<Establish Personal Area Network*/
#define ATJN 			"AT+JN"			/**<Join Network*/
#define ATJPAN 			"AT+JPAN"		/**<Join Specific Pan*/
#define ATSJN			"AT+SJN"		/**< Silent Join*/
#define ATDASSL			"AT+DASSL"		/**< Disassociate Local Device From Pan*/
#define ATDASSR 		"AT+DASSR "		/**<Disassociate Remote Node From PAN (ZDO)*/
#define ATN 			"AT+N"			/**< Display Network Information*/
#define ATNTABLE		"AT+NTABLE"		/**< Display Neighbour Table (ZDO)*/
#define ATRTABLE		"AT+RTABLE"		/**< Display Routing Table (ZDO)*/
#define ATDREQ			"AT+IDREQ"		/**< Request Node’s Network address (ZDO)*/
#define ATEUIREQ		"AT+EUIREQ"		/**<Request Node’s EUI (ZDO)*/
#define ATNODEDESC		"AT+NODEDESC"	/**<Request Node’s Descriptor (ZDO)*/
#define ATPOWERDESC 	"AT+POWERDESC"	/**<Request Node’s Power Descriptor (ZDO)*/
#define ATACTEPDESC		"AT+ACTEPDESC"	/**< Request Node’s Active Endpoint List (ZDO)*/
#define ATSIMPLEDESC	"AT+SIMPLEDESC"	/**< Request Endpoint’s Simple Descriptor (ZDO)*/
#define ATMATCHREQ		"AT+MATCHREQ"	/**< Find Nodes which Match a Specific Descriptor (ZDO)*/
#define ATANNCE 		"AT+ANNCE"		/**< Announce Local Device in the Network (ZDO)*/
#define ATSR  			"AT+SR"			/**<Set Source Route To Remote Device*/
#define ATFNDSR			"AT+FNDSR"		/**< Find The Source Route To A Remote Device*/
#define ATPOLL 			"AT+POLL"		/**< Poll For Data From Parent*/
#define ATREJOIN 		"AT+REJOIN"		/**<Rejoin The Network*/
#define ATSN 			"AT+SN"			/**< Scan Network*/
#define ATKEYUPD		"AT+KEYUPD"		/**<Update the Network Key (ZDO)*/
#define ATBECOMETC 		"AT+BECOMETC"	/**<Make Local Device the Trust Centre*/
#define ATBECOMENM		"AT+BECOMENM"	/**<Make the local device Network Manager*/
#define ATCCHANGE		"AT+CCHANGE"	/**<Change the network’s channel*/

/*Messaging*/
#define ATATABLE 		"AT+ATABLE"		/**<Display Address Table*/
#define ATASET 			"AT+ASET"		/**<Set Address Table Entry*/
#define ATMTABLE		"AT+MTABLE"		/**<Display Multicast Table*/
#define ATMSET			"AT+MSET"		/**<Set Multicast Table Entry*/
#define ATBCAST 		"AT+BCAST"		/**<Transmit A Broadcast*/
#define ATBCASTB		"AT+BCASTB" 	/**<Transmit A Broadcast Of Binary Data*/
#define ATUCAST 		"AT+UCAST"		/**<Transmit A Unicast*/
#define ATUCASTB		"AT+UCASTB"		/**< Transmit A Unicast Of Binary Data*/
#define ATSCAST  		"AT+SCAST "		/**<Transmit Data To The Sink*/
#define ATSCASTB		"AT+SCASTB" 	/**<Transmit Binary Data To The Sink*/
#define ATSSINK 		"AT+SSINK"		/**<Search For A Sink*/
#define ATMCAST 		"AT+MCAST"		/**<Transmit A Multicast*/
#define ATMCASTB 		"AT+MCASTB"		/**<Transmit A Multicast Of Binary Data*/
#define ATDMODE 		"AT+DMODE"		/**<Enter Data Mode (Serial Link Mode)*/
#define plusplusplus	"+++"			/**<Leave Data Mode*/
#define ATIDENT 		"AT+IDENT"		/**<Play A Tune On Remote Devboard*/
#define ATRDATAB 		"AT+RDATAB"		/**<Send Binary Raw Data*/
#define ATBinding 		"AT+Binding"	/**<Management (ETRX3 only)*/
#define ATLBTABLE 		"AT+LBTABLE"	/**<Display Local Binding Table*/
#define ATBSET 			"AT+BSET"		/**<Set Local Binding Table Entry*/
#define ATBCLR 			"AT+BCLR"		/**<Clear Local Binding Table Entry*/
#define ATBTABLE 		"AT+BTABLE "	/**<Display Binding Table*/
#define ATBIND			"AT+BIND"		/**<Create Binding on Remote Device*/
#define ATUNBIND		"AT+UNBIND"		/**<Delete Binding on Remote Device*/

/*Error codes*/
static char* error[] = { "00", "01", "02", "04", "05", "06", "07", "08", "09",
		"0A", "0B", "0C", "0E", "0F", "10", "12", "18", "19", "1A", "1B", "1C",
		"20", "25", "27", "28", "2C", "2D", "33", "34", "35", "39", "6C", "70",
		"72", "74", "80", "81", "82", "83", "84", "85", "86", "87", "88", "89",
		"91", "93", "94", "96", "98", "A1", "AB", "AC", "AD", "AE", "AF", "C5",
		"C7", "C8" };

/*Error codes as text*/
static char* error_list[] = { "Everything OK - Success",
		"Couldn’t poll Parent because of Timeout", "Unknown command",
		"Invalid S-Register", "Invalid parameter",
		"Recipient could not be reached", "Message was not acknowledged",
		"No sink known", "Address Table entry is in use and cannot be modified",
		"Message could not be sent", "Local node is not sink",
		"Too many characters",
		"Background Scan in Progress (Please wait and try again)",
		"Fatal error initialising the network", "Error bootloading",
		"Fatal error initialising the stack", "Node has run out of Buffers",
		"Trying to write read-only register",
		"Data Mode Refused by Remote Node", "Connection Lost in Data Mode",
		"Remote node is already in Data Mode", "Invalid password",
		"Cannot form network", "No network found",
		"Operation cannot be completed if node is part of a PAN",
		"Error leaving the PAN", "Error scanning for PANs",
		"No response from the remote bootloader",
		"Target did not respond during cloning",
		"Timeout occurred during xCASTB", "MAC Transmit Queue is Full",
		"Invalid Binding Index", "Invalid Operation",
		"More than 10 unicast messages were in flight at the same time",
		"Message too long", "ZDP Invalid Request Type", "ZDP Device not Found",
		"ZDP Invalid Endpoint", "ZDP Not Active", "ZDP Not Supported",
		"ZDP Timeout", "ZDP No Match", "ZDP Table Full", "ZDP No Entry",
		"ZDP No Descriptor", "Operation only possible if connected to a PAN",
		"Node is not part of a Network", "Cannot join network",
		"Mobile End Device Move to new Parent Failed",
		"Cannot join ZigBee 2006 Network as Router",
		"More than 8 broadcasts were sent within 8 seconds",
		"Trying to join, but no beacons could be heard",
		"Network key was sent in the clear when trying to join secured",
		"Did not receive Network Key", "No Link Key received",
		"Preconfigured Key Required", "NWK Already Present", "NWK Table Full",
		"NWK Unknown Device" };

struct nlist *errorEntry;

/*S-Registers*/

/*The following prompts can show up during the operation of the ETRXn modules. 
 * Telegesis response:
 */
#define OK			1
#define ERROR 		"ERROR"
#define ACK			"ACK"
#define NACK		"NACK"
#define LEFTPAN		"LeftPAN"
#define LOSTPAN		"LostPAN"

/**
 * ZigBee structure
 */
typedef struct struct_zigbee {
	short devicetype; /**<ZigBee device types */
	long EUI64; /**<16 hexadecimal characters. This is flashed on to the chip at manufacture and cannot be
	 changed by the user. This can be compared to the permanent MAC address of an IP-based
	 device*/
	short nwk_addr; /**<This is allocated to the device when it joins the
	 PAN and cannot be changed or preset, except that 0x0000 is always the coordinator. It is
	 analogous to a temporary IP address. Otherwise known as the Node ID.*/
	short channel; /**<*Joined channel in the network*/
	int PID; /**<Personal Area Network ID (PANID)*/
	long EPID; /**<Extended Personal Area Network ID (EPID)*/
	char stack_profile; /**<The Zigbee stack profile*/
} zigbee_t;

/**
 * @brief	Struct for the Product Identification Information.
 */
typedef struct struct_telegesis {
	char* deviceName;
	char* firmwareRevision;
	char* EUID;
} telegesis_t;

/**
 *	Functions
 */

/**
 *	@brief	Get the error code.
 *	@param	Prompt error code
 *	@return	Return the Error Code Value as hex .
 */
char* GetErrorCodeNumber(char*);

/**
 * 	@brief	Get the error message.
 * 	@param	You need the error number as string.
 * 	@return	Return the message of compared error value.
 */
char* GetErrorCodeMessage(char*);

short InitializeSerialPort(char*);

/**
 *	@brief	This function save the Product Identification Information.
 *	@param	telegesis_t	A pointer with type of telegesis_t. This will save the information here.
 *	@return	Always return OK.
 */
int ProductIdentificationInformation(telegesis_t*);

/**
 *	@brief	Establish Personal Area Network. The local node becomes a coordinator and
 *			performs an energy scan on all channels selected in S00.
 *	@param	struct_zigbee
 *	@return	OK
 *	@return	ERROR:<errorcode>
 */

char* EstablishPAN(zigbee_t*);

void bootload(void);

#endif
