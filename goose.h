
/*-------------------------------------------------------------------------*/
/**
   @file    goose.h
   @author  Lilei
   @brief   对goose的ini文件进行解析，生成相应的结构体
*/
/*--------------------------------------------------------------------------*/

#ifndef _GOOSE_H_
#define _GOOSE_H_

#include<stdint.h>


#define MAXSIZE 40
#define TYPESIZE 8
#define APPIDSIZE 16


/*---------------------------------------------------------------------------
                                New struct types
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
  @brief    struct goose control block rx
 */
typedef struct TagGooseControlBlockRx
{
    uint8_t addr[8];
    uint32_t appid;
    uint8_t gocbRef[MAXSIZE];
    uint8_t appID[APPIDSIZE];
    uint8_t datSet[MAXSIZE];
    uint32_t confRev;
    uint32_t numDatSetEntriess;

}GooseControlBlockRx;

/**
  @brief    struct goose control block tx
 */
typedef struct TagGooseControlBlockTx
{
    uint8_t gocbRef[MAXSIZE];
    uint8_t appID[APPIDSIZE];
    uint8_t datSet[MAXSIZE];
    uint32_t confRev;
    uint32_t numDatSetEntriess;
    uint8_t addr[8];
    uint32_t priority;
    uint32_t vid;
    uint32_t appid;
    uint32_t minTime;
    uint32_t maxTime;
    FuncConDatAttr* fcda;

}GooseControlBlockTx;

/**
  @brief    struct goose Tx Message
 */
typedef struct TagGooseTxMessage
{
    uint32_t numGoCb;
    GooseControlBlockTx* gocd;

}GooseTxMessage;

/**
  @brief    struct goose Rx Message
 */
typedef struct TagGooseRxMessage
{
    uint32_t numGoCb;
    uint32_t numInput;
    GooseControlBlockRx* gocd;
    GooseInput* input;

}GooseRxMessage;



/*---------------------------------------------------------------------------
                                Function
 ---------------------------------------------------------------------------*/
void IniToStruct(const dictionary* dict, GooseTxMessage* gooseTxMessage, GooseRxMessage* gooseRxMessage);
void FreeGooseMessageMem(GooseTxMessage* gooseTxMessage, GooseRxMessage* gooseRxMessage);
void PrintGooseTxRxMessage(GooseTxMessage* gooseTxMessage, GooseRxMessage* gooseRxMessage);



#endif