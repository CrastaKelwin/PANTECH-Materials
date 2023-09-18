#ifndef __CAN_SPISW_H
#define __CAN_SPISW_H


void CANSPIInitialize(char SJW,
											char BRP,
											char PHSEG1,
											char PHSEG2,
											char PROPSEG,
											char CAN_CONFIG_FLAGS);
void CANSPISetOperationMode(char mode, char WAIT);
char CANSPIGetOperationMode(void);
void CANSPISetBaudRate(	char SJW,
												char BRP,
												char PHSEG1,
												char PHSEG2,
												char PROPSEG,
												char CAN_CONFIG_FLAGS);
void CANSPISetMask(char CAN_MASK, unsigned long val, char CAN_CONFIG_FLAGS);
void CANSPISetFilter(char CAN_FILTER, unsigned long val, char CAN_CONFIG_FLAGS);
char CANSPIRead(	unsigned long *id,
									char *data,
									char *data_len,
									char *CAN_RX_MSG_FLAGS);
char CANSPIWrite( unsigned long id, char *data, char data_len, char CAN_TX_MSG_FLAGS);


#endif
