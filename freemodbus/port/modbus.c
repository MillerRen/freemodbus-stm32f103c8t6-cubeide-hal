#include "mb.h"
#include "mbport.h"


// 十路输入寄存器
#define REG_INPUT_SIZE  10
uint16_t REG_INPUT_BUF[REG_INPUT_SIZE];


// 十路保持寄存器
#define REG_HOLD_SIZE   10
uint16_t REG_HOLD_BUF[REG_HOLD_SIZE] = {
		0,
		0,
		0,
		0,
		0,
		0,
		0,
		0,
		0,
		0x19
};

// 十路线圈
#define REG_COILS_SIZE 10
uint8_t REG_COILS_BUF[REG_COILS_SIZE];


// 十路离散量
#define REG_DISC_SIZE  10
uint8_t REG_DISC_BUF[10];


/// CMD4
eMBErrorCode eMBRegInputCB( UCHAR * pucRegBuffer, USHORT usAddress, USHORT usNRegs )
{
    USHORT usRegIndex = usAddress - 1;

    // 非法检测
    if((usRegIndex + usNRegs) > REG_INPUT_SIZE)
    {
        return MB_ENOREG;
    }

    // 循环读取
    while( usNRegs > 0 )
    {
        *pucRegBuffer++ = ( unsigned char )( REG_INPUT_BUF[usRegIndex] >> 8 );
        *pucRegBuffer++ = ( unsigned char )( REG_INPUT_BUF[usRegIndex] & 0xFF );
        usRegIndex++;
        usNRegs--;
    }

    // 模拟输入寄存器被改变
    for(usRegIndex = 0; usRegIndex < REG_INPUT_SIZE; usRegIndex++)
    {
        REG_INPUT_BUF[usRegIndex]++;
    }

    return MB_ENOERR;
}

/// CMD6、3、16
eMBErrorCode eMBRegHoldingCB( UCHAR * pucRegBuffer, USHORT usAddress, USHORT usNRegs, eMBRegisterMode eMode )
{
    USHORT usRegIndex = usAddress - 1;

    // 非法检测
    if((usRegIndex + usNRegs) > REG_HOLD_SIZE)
    {
        return MB_ENOREG;
    }

        // 写寄存器
    if(eMode == MB_REG_WRITE)
    {
        while( usNRegs > 0 )
        {
                        REG_HOLD_BUF[usRegIndex] = (pucRegBuffer[0] << 8) | pucRegBuffer[1];
                        pucRegBuffer += 2;
            usRegIndex++;
            usNRegs--;
        }
    }

        // 读寄存器
    else
    {
        while( usNRegs > 0 )
        {
            *pucRegBuffer++ = ( unsigned char )( REG_HOLD_BUF[usRegIndex] >> 8 );
            *pucRegBuffer++ = ( unsigned char )( REG_HOLD_BUF[usRegIndex] & 0xFF );
            usRegIndex++;
            usNRegs--;
        }
    }

    return MB_ENOERR;
}

/// CMD1、5、15
eMBErrorCode eMBRegCoilsCB( UCHAR * pucRegBuffer, USHORT usAddress, USHORT usNCoils, eMBRegisterMode eMode )
{
    USHORT usRegIndex   = usAddress - 1;
    USHORT usCoilGroups = ((usNCoils - 1) / 8 + 1);
    UCHAR  ucStatus     = 0;
    UCHAR  ucBits       = 0;
    UCHAR  ucDisp       = 0;

    // 非法检测
    if((usRegIndex + usNCoils) > REG_COILS_SIZE)
    {
        return MB_ENOREG;
    }

    // 写线圈
    if(eMode == MB_REG_WRITE)
    {
        while(usCoilGroups--)
        {
            ucStatus = *pucRegBuffer++;
            ucBits   = 8;
            while((usNCoils--) != 0 && (ucBits--) != 0)
            {
                REG_COILS_BUF[usRegIndex++] = ucStatus & 0X01;
                ucStatus >>= 1;
            }
        }
    }

    // 读线圈
    else
    {
        while(usCoilGroups--)
        {
            ucDisp = 0;
            ucBits = 8;
            while((usNCoils--) != 0 && (ucBits--) != 0)
            {
                ucStatus |= (REG_COILS_BUF[usRegIndex++] << (ucDisp++));
            }
            *pucRegBuffer++ = ucStatus;
        }
    }
    return MB_ENOERR;
}


/// CMD4
eMBErrorCode eMBRegDiscreteCB( UCHAR * pucRegBuffer, USHORT usAddress, USHORT usNDiscrete )
{
    USHORT usRegIndex   = usAddress - 1;
    USHORT usCoilGroups = ((usNDiscrete - 1) / 8 + 1);
    UCHAR  ucStatus     = 0;
    UCHAR  ucBits       = 0;
    UCHAR  ucDisp       = 0;

    // 非法检测
    if((usRegIndex + usNDiscrete) > REG_DISC_SIZE)
    {
        return MB_ENOREG;
    }

        // 读离散输入
        while(usCoilGroups--)
        {
                ucDisp = 0;
                ucBits = 8;
                while((usNDiscrete--) != 0 && (ucBits--) != 0)
                {
                        if(REG_DISC_BUF[usRegIndex])
                        {
                                ucStatus |= (1 << ucDisp);
                        }
                        ucDisp++;
                }
                *pucRegBuffer++ = ucStatus;
        }

    // 模拟改变
    for(usRegIndex = 0; usRegIndex < REG_DISC_SIZE; usRegIndex++)
    {
        REG_DISC_BUF[usRegIndex] = !REG_DISC_BUF[usRegIndex];
    }

    return MB_ENOERR;
}
