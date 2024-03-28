/*
 *  @file bq79616.h
 *
 *  @author Vince Toledo - Texas Instruments Inc.
 *  @date February 2020
 *  @version 1.2
 *  @note Built with CCS for Hercules Version: 8.1.0.00011
 *  @note Built for TMS570LS1224 (LAUNCH XL2)
 */

/*****************************************************************************
**
**  Copyright (c) 2011-2019 Texas Instruments
**
******************************************************************************/


#ifndef BQ79616_H_
#define BQ79616_H_

#include "datatypes.h"
#include <stdint.h>
#include "main.h"
//#include "hal_stdtypes.h"

//****************************************************
// ***Register defines, choose one of the following***
// ***based on your device silicon revision:       ***
//****************************************************
//#include "A0_reg.h"
#include "BQ_reg.h"

// User defines
#define TOTALBOARDS 1       //boards in stack
#define MAXBYTES (16*2)     //maximum number of bytes to be read from the devices (for array creation)
#define BAUDRATE 1000000    //device + uC baudrate

#define FRMWRT_SGL_R	0x00    //single device READ
#define FRMWRT_SGL_W	0x10    //single device WRITE
#define FRMWRT_STK_R	0x20    //stack READ
#define FRMWRT_STK_W	0x30    //stack WRITE
#define FRMWRT_ALL_R	0x40    //broadcast READ
#define FRMWRT_ALL_W	0x50    //broadcast WRITE
#define FRMWRT_REV_ALL_W 0x60   //broadcast WRITE reverse direction
                  
// Function Prototypes
void AutoAddress(UART_HandleTypeDef *huart);
BOOL GetFaultStat();

uint16_t CRC16(BYTE *pBuf, int nLen);

int WriteFrame(BYTE bID, uint16_t wAddr, BYTE * pData, BYTE bLen, BYTE bWriteType, UART_HandleTypeDef *huart);
int ReadReg(BYTE bID, uint16_t wAddr, BYTE * pData, BYTE bLen, uint32_t dwTimeOut,
        BYTE bWriteType, UART_HandleTypeDef *huart);

int WriteReg(BYTE bID, uint16_t wAddr, uint64_t dwData, BYTE bLEN, BYTE bWriteType, UART_HandleTypeDef *huart);
int ReadFrameReq(BYTE bID, uint16_t wAddr, BYTE bByteToReturn, BYTE bWriteType, UART_HandleTypeDef *huart);
int  WaitRespFrame(BYTE *pFrame, uint32_t bLen, uint32_t dwTimeOut);

void delayms(uint16_t ms);
void delayus(uint16_t us);

void ResetAllFaults(BYTE bID, BYTE bWriteType);
void MaskAllFaults(BYTE bID, BYTE bWriteType);

void RunCB();
void ReverseAddressing();

#endif /* BQ79606_H_ */
//EOF
