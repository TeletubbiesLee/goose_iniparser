
/*-------------------------------------------------------------------------*/
/**
   @file    goose.h
   @author  Lilei
   @brief   
*/
/*--------------------------------------------------------------------------*/

#ifndef _GOOSE_H_
#define _GOOSE_H_

#include<stdint.h>


#define MAXSIZE 40
#define TYPESIZE 4
#define APPIDSIZE 16


/*---------------------------------------------------------------------------
                                New types
 ---------------------------------------------------------------------------*/

/**
  @brief    struct fcda
 */
typedef struct TagFuncConDatAttr
{
    uint8_t ref[MAXSIZE];
    uint8_t type[TYPESIZE];
    uint8_t inVarName[MAXSIZE];
    uint32_t act;
}FuncConDatAttr;


/**
  @brief    struct input
 */
typedef struct TagGooseInput
{
    uint32_t gocbIndex;
    uint32_t gocbEntryIntex;
    uint8_t ref[MAXSIZE];
    uint8_t type[TYPESIZE];
    uint8_t outVarName[MAXSIZE];
}GooseInput;


/**
  @brief    struct goose control block
 */
typedef struct TagGooseControlBlock
{
    uint8_t gocbRef[MAXSIZE];
    uint8_t appID[APPIDSIZE];
    uint8_t datSet[MAXSIZE];
    uint32_t confRev;
    uint32_t numDatSetEntriess;
    uint8_t addr[6];
    uint32_t priority;
    uint32_t vid;
    uint32_t appid;
    uint32_t minTime;
    uint32_t maxTime;
    union {
        FuncConDatAttr* fcda;          /* Goose Tx Message */
        GooseInput* input;             /* Goose Rx Message */
    }device;
}GooseControlBlock;


/**
  @brief    struct goose Tx or Rx Message
 */
typedef struct TagGooseTxRxMessage
{
    uint8_t flagTxRx;               /* 1:Tx;  2:Rx  0:error */
    uint32_t numGoCb;
    union {
        uint32_t numInput;
        uint32_t numOutput;
    }num;
    GooseControlBlock** gocd;
}GooseTxRxMessage;



/*---------------------------------------------------------------------------
                                Function
 ---------------------------------------------------------------------------*/

void IniToStruct(const dictionary* dict, GooseTxRxMessage* gooseTxMassage, GooseTxRxMessage* gooseRxMassage);
void GooseMessageInit(GooseTxRxMessage* gooseTxMassage, GooseTxRxMessage* gooseRxMassage);
void GocbStructInit(GooseTxRxMessage* gooseMassage);
void FcdaStructInit(GooseTxRxMessage* gooseMassage);
void GoInputStructInit(GooseTxRxMessage* gooseMassage);
void DictStrTrim(uint8_t* srcStr, uint8_t* destStr);
uint32_t CharToInt(uint8_t* str);
void AddrToInt(uint8_t* str, uint8_t* ret);
uint32_t HexToInt(uint8_t* str, uint8_t size);



#endif