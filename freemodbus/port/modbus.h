/*
 * modbus.h
 *
 *  Created on: Aug 16, 2022
 *      Author: ren12
 */

#ifndef PORT_MODBUS_H_
#define PORT_MODBUS_H_

#include "mb.h"
#include "mbport.h"

eMBErrorCode eMBRegInputCB( UCHAR * pucRegBuffer, USHORT usAddress, USHORT usNRegs );
eMBErrorCode eMBRegHoldingCB( UCHAR * pucRegBuffer, USHORT usAddress, USHORT usNRegs, eMBRegisterMode eMode );
eMBErrorCode eMBRegCoilsCB( UCHAR * pucRegBuffer, USHORT usAddress, USHORT usNCoils, eMBRegisterMode eMode );
eMBErrorCode eMBRegDiscreteCB( UCHAR * pucRegBuffer, USHORT usAddress, USHORT usNDiscrete );

#endif /* PORT_MODBUS_H_ */
