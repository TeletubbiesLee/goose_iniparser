

/*-------------------------------------------------------------------------*/
/**
   @file    goose.c
   @author  Lilei
   @brief   对goose的ini文件进行解析，生成相应的结构体
*/
/*--------------------------------------------------------------------------*/

/*------------------------------Include-------------------------------------*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "iniparser.h"
#include "goose.h"

/*--------------------------------------------------------------------------*/

/*----------------------------STATIC FUNCTION-------------------------------*/
static void GooseMessageInit(GooseTxMessage* gooseTxMessage, GooseRxMessage* gooseRxMessage);
static void GocbTxStructInit(GooseTxMessage* gooseTxMessage);
static void GocbRxStructInit(GooseRxMessage* gooseRxMessage);
static void FcdaStructInit(GooseTxMessage* gooseMassage, uint32_t num);
static void GoInputStructInit(GooseRxMessage* gooseMassage);
static void DictStrTrim(uint8_t* srcStr, uint8_t* destStr);
static uint32_t CharToInt(uint8_t* str);
static void AddrToInt(uint8_t* str, uint8_t* ret);
static uint32_t HexToInt(uint8_t* str, uint8_t size);

/*--------------------------------------------------------------------------*/

/*---------------------------------MAIN-------------------------------------*/

int main(int argc, char* argv[])
{
    dictionary* ini;
    uint8_t* iniName;
    GooseTxMessage gooseTxMessage;
    GooseRxMessage gooseRxMessage;

    GooseMessageInit(&gooseTxMessage, &gooseRxMessage);

    if (argc < 2)
    {
        return 1;
    }
    else
    {
        iniName = argv[1];
    }

    ini = iniparser_load(iniName);
    // iniparser_dump(ini, stdout);
    IniToStruct(ini, &gooseTxMessage, &gooseRxMessage);
    iniparser_freedict(ini);

    PrintGooseTxRxMessage(&gooseTxMessage, &gooseRxMessage);

    return 0;
}

/*------------------------------MAIN END------------------------------------*/

/*
 * @brief 将ini文件转换后的字典数据类型转为相应的结构体数据
 * @param dict 已经解析好的字典结构体指针
 * @param gooseTxMessage Goose发送结构体指针
 * @param gooseRxMessage Goose接收结构体指针
 * 
 */
void IniToStruct(const dictionary* dict, GooseTxMessage* gooseTxMessage, GooseRxMessage* gooseRxMessage)
{
    uint32_t i = 0, numGocb = 0, numFcda = 0, numInput = 0;
    uint8_t flag = 0;
    uint8_t strTrim[20] = {0};

    if(dict == NULL)
    {
        return ;
    }
       
    for(i=0; i<dict->size; i++)
    {
        if(dict->key[i] == NULL)
        {
            continue;
        }
        if(dict->val[i] != NULL)                /* 有值 */
        {
            if(flag == 1)                       /* TX */
            {
                if(strncmp(dict->key[i], "GOOSE Tx", strlen("GOOSE Tx")) == 0)      /* 识别发送结构体的相关元素 */
                {
                    DictStrTrim(dict->key[i], strTrim);
                    if(strcmp(strTrim, "numGoCb") == 0)
                    {
                        gooseTxMessage->numGoCb = CharToInt(dict->val[i]);
                        GocbTxStructInit(gooseTxMessage);
                    }
                    else
                    {
                        printf("[%s] is nonrecognition!\r\n", dict->key[i]);        /* 不识别的字符串 */
                        continue;
                    }
                }
                else if(strncmp(dict->key[i], "GoCB", strlen("GoCB")) == 0)         /* 识别发送结构体的GoCB相关元素 */
                {
                    DictStrTrim(dict->key[i], strTrim);
                    if(strcmp(strTrim, "GoCBRef") == 0)
                    {
                        strncpy(gooseTxMessage->gocd[numGocb-1].gocbRef, dict->val[i], strlen(dict->val[i])+1);
                    }
                    else if(strcmp(strTrim, "AppID") == 0)
                    {
                        strncpy(gooseTxMessage->gocd[numGocb-1].appID, dict->val[i], strlen(dict->val[i])+1);
                    }
                    else if(strcmp(strTrim, "DatSet") == 0)
                    {
                        strncpy(gooseTxMessage->gocd[numGocb-1].datSet, dict->val[i], strlen(dict->val[i])+1);
                    }
                    else if(strcmp(strTrim, "ConfRev") == 0)
                    {
                        gooseTxMessage->gocd[numGocb-1].confRev = CharToInt(dict->val[i]);
                    }
                    else if(strcmp(strTrim, "numDatSetEntries") == 0)
                    {
                        gooseTxMessage->gocd[numGocb-1].numDatSetEntriess = CharToInt(dict->val[i]);
                        FcdaStructInit(gooseTxMessage, numGocb-1);
                    }
                    else
                    {
                        printf("[%s] is nonrecognition!\r\n", dict->key[i]);        /* 不识别的字符串 */
                        continue;
                    }
                }
                else if(strncmp(dict->key[i], "DstAddr", strlen("DstAddr")) == 0)   /* 识别DstAddr下的属性元素 */
                {
                    DictStrTrim(dict->key[i], strTrim);
                    if(strcmp(strTrim, "Addr") == 0)
                    {
                        AddrToInt(dict->val[i], gooseTxMessage->gocd[numGocb-1].addr);
                    }
                    else if(strcmp(strTrim, "Priority") == 0)
                    {
                        gooseTxMessage->gocd[numGocb-1].priority = CharToInt(dict->val[i]);
                    }
                    else if(strcmp(strTrim, "VID") == 0)
                    {
                        gooseTxMessage->gocd[numGocb-1].vid = CharToInt(dict->val[i]);
                    }
                    else if(strcmp(strTrim, "Appid") == 0)
                    {
                        gooseTxMessage->gocd[numGocb-1].appid = HexToInt(dict->val[i], 4);
                    }
                    else if(strcmp(strTrim, "MinTime") == 0)
                    {
                        gooseTxMessage->gocd[numGocb-1].minTime = CharToInt(dict->val[i]);
                    }
                    else if(strcmp(strTrim, "MaxTime") == 0)
                    {
                        gooseTxMessage->gocd[numGocb-1].maxTime = CharToInt(dict->val[i]);
                    }
                    else
                    {
                        printf("[%s] is nonrecognition!\r\n", dict->key[i]);        /* 不识别的字符串 */
                        continue;
                    }
                }
                else if(strncmp(dict->key[i], "FCDA", strlen("FCDA")) == 0)         /* 识别FCDA结构体的相关元素 */
                {
                    DictStrTrim(dict->key[i], strTrim);
                    if(strcmp(strTrim, "Ref") == 0)
                    {
                        strncpy(gooseTxMessage->gocd[numGocb-1].fcda[numFcda-1].ref, dict->val[i], strlen(dict->val[i])+1);
                    }
                    else if(strcmp(strTrim, "Type") == 0)
                    {
                        strncpy(gooseTxMessage->gocd[numGocb-1].fcda[numFcda-1].type, dict->val[i], strlen(dict->val[i])+1);
                    }
                    else if(strcmp(strTrim, "InVarName") == 0)
                    {
                        strncpy(gooseTxMessage->gocd[numGocb-1].fcda[numFcda-1].inVarName, dict->val[i], strlen(dict->val[i])+1);
                    }
                    else if(strcmp(strTrim, "ACT") == 0)
                    {
                        gooseTxMessage->gocd[numGocb-1].fcda[numFcda-1].act = CharToInt(dict->val[i]);
                    }
                    else
                    {
                        printf("[%s] is nonrecognition!\r\n", dict->key[i]);        /* 不识别的字符串 */
                        continue;
                    }
                }
                else
                {
                    printf("[%s] is nonrecognition!\r\n", dict->key[i]);        /* 不识别的字符串 */
                    continue;
                }

            }
            else if(flag == 2)                  /* RX */
            {
                if(strncmp(dict->key[i], "GOOSE Rx", strlen("GOOSE Rx")) == 0)
                {
                    DictStrTrim(dict->key[i], strTrim);
                    if(strcmp(strTrim, "numGoCb") == 0)
                    {
                        gooseRxMessage->numGoCb = CharToInt(dict->val[i]);
                        GocbRxStructInit(gooseRxMessage);
                    }
                    else if(strcmp(strTrim, "numInput") == 0)
                    {
                        gooseRxMessage->numInput = CharToInt(dict->val[i]);
                        GoInputStructInit(gooseRxMessage);
                    }
                    else
                    {
                        printf("[%s] is nonrecognition!\r\n", dict->key[i]);        /* 不识别的字符串 */
                        continue;
                    }
                }
                else if(strncmp(dict->key[i], "GoCB", strlen("GoCB")) == 0)         /* 识别接收的GoCB结构体的相关元素 */
                {
                    DictStrTrim(dict->key[i], strTrim);
                    if(strcmp(strTrim, "Addr") == 0)
                    {
                        AddrToInt(dict->val[i], gooseRxMessage->gocd[numGocb-1].addr);
                    }
                    else if(strcmp(strTrim, "Appid") == 0)
                    {
                        gooseRxMessage->gocd[numGocb-1].appid = HexToInt(dict->val[i], 4);
                    }
                    else if(strcmp(strTrim, "GoCBRef") == 0)
                    {
                        strncpy(gooseRxMessage->gocd[numGocb-1].gocbRef, dict->val[i], strlen(dict->val[i])+1);
                    }
                    else if(strcmp(strTrim, "AppID") == 0)
                    {
                        strncpy(gooseRxMessage->gocd[numGocb-1].appID, dict->val[i], strlen(dict->val[i])+1);
                    }
                    else if(strcmp(strTrim, "DatSet") == 0)
                    {
                        strncpy(gooseRxMessage->gocd[numGocb-1].datSet, dict->val[i], strlen(dict->val[i])+1);
                    }
                    else if(strcmp(strTrim, "ConfRev") == 0)
                    {
                        gooseRxMessage->gocd[numGocb-1].confRev = CharToInt(dict->val[i]);
                    }
                    else if(strcmp(strTrim, "numDatSetEntries") == 0)
                    {
                        gooseRxMessage->gocd[numGocb-1].numDatSetEntriess = CharToInt(dict->val[i]);
                    }
                    else
                    {
                        printf("[%s] is nonrecognition!\r\n", dict->key[i]);        /* 不识别的字符串 */
                        continue;
                    }
                }
                else if(strncmp(dict->key[i], "INPUT", strlen("INPUT")) == 0)       /* 识别INPUT结构体相关的元素 */
                {
                    DictStrTrim(dict->key[i], strTrim);
                    if(strcmp(strTrim, "GoCbIndex") == 0)
                    {
                        gooseRxMessage->input[numInput-1].gocbIndex = CharToInt(dict->val[i]);
                    }
                    else if(strcmp(strTrim, "GoCbEntryIndex") == 0)
                    {
                        gooseRxMessage->input[numInput-1].gocbEntryIntex = CharToInt(dict->val[i]);
                    }
                    else if(strcmp(strTrim, "Ref") == 0)
                    {
                        strncpy(gooseRxMessage->input[numInput-1].ref, dict->val[i], strlen(dict->val[i])+1);
                    }
                    else if(strcmp(strTrim, "Type") == 0)
                    {
                        strncpy(gooseRxMessage->input[numInput-1].type, dict->val[i], strlen(dict->val[i])+1);
                    }
                    else if(strcmp(strTrim, "OutVarName") == 0)
                    {
                        strncpy(gooseRxMessage->input[numInput-1].outVarName, dict->val[i], strlen(dict->val[i])+1);
                    }
                    else
                    {
                        printf("[%s] is nonrecognition!\r\n", dict->key[i]);        /* 不识别的字符串 */
                        continue;
                    }
                }
                else
                {
                    printf("[%s] is nonrecognition!\r\n", dict->key[i]);        /* 不识别的字符串 */
                    continue;
                }

            }
            else
            {
                continue;
            }
        }
        else                                    /* 没有值 */
        {
            if(strcmp(dict->key[i], "GOOSE Tx") == 0)
            {
                flag = 1;
                numGocb = 0;
                numFcda = 0;
                numInput = 0;
            }
            else if(strcmp(dict->key[i], "GOOSE Rx") == 0)
            {
                flag = 2;
                numGocb = 0;
                numFcda = 0;
                numInput = 0;
            }
            else if(strncmp(dict->key[i], "GoCB", 4) == 0)
            {
                numGocb++;
                numFcda = 0;
                numInput = 0;
            }
            else if(strncmp(dict->key[i], "FCDA", 4) == 0)
            {
                numFcda++;
            }
            else if(strncmp(dict->key[i], "INPUT", 5) == 0)
            {
                numInput++;
            }
            else if(strcmp(dict->key[i], "SMV Rx") == 0)
            {
                flag = 0;
                numGocb = 0;
                numFcda = 0;
                numInput = 0;
                break;
            }
            else
            {
                continue;
            }
        }
    }

    return;
}

/*
 * @brief Goose信息结构体的初始化
 * @param gooseTxMessage Goose发送结构体指针
 * @param gooseRxMessage Goose接收结构体指针
 * 
 */
static void GooseMessageInit(GooseTxMessage* gooseTxMessage, GooseRxMessage* gooseRxMessage)
{

    gooseTxMessage->numGoCb = 0;
    gooseRxMessage->numGoCb = 0;

    gooseRxMessage->numInput = 0;

    gooseTxMessage->gocd = NULL;
    gooseRxMessage->gocd = NULL;

    gooseRxMessage->input = NULL;

}

/*
 * @brief Goose发送信息结构体的Goose控制块结构体的初始化
 * @param gooseTxMessage Goose发送结构体指针
 * 
 */
static void GocbTxStructInit(GooseTxMessage* gooseTxMessage)
{
    uint32_t i = 0;
   
    gooseTxMessage->gocd  = (GooseControlBlockTx*)calloc(sizeof(GooseControlBlockTx), gooseTxMessage->numGoCb);
   
    if (!gooseTxMessage->gocd)
    {
        printf("DEBUG:%s():%d:gooseTxMessage->gocd Malloc Fail!\n", __FUNCTION__, __LINE__);
        return;
    }

    for(i=0; i<gooseTxMessage->numGoCb; i++)
    {
         gooseTxMessage->gocd[i].fcda == NULL;
    }

}

/*
 * @brief Goose接收信息结构体的Goose控制块结构体的初始化
 * @param gooseRxMessage Goose接收结构体指针
 * 
 */
static void GocbRxStructInit(GooseRxMessage* gooseRxMessage)
{
    gooseRxMessage->gocd  = (GooseControlBlockRx*)calloc(sizeof(GooseControlBlockRx), gooseRxMessage->numGoCb);
   
    if (!gooseRxMessage->gocd)
    {
        printf("DEBUG:%s():%d:gooseRxMessage->gocd Malloc Fail!\n", __FUNCTION__, __LINE__);
        return;
    }

}

/*
 * @brief Goose发送信息结构体的FCDA(功能约束数据属性)结构体的初始化
 * @param gooseTxMessage Goose发送结构体指针
 * @param num FCDA结构体对应的GoCB的编号
 * 
 */
static void FcdaStructInit(GooseTxMessage* gooseTxMessage, uint32_t num)
{
    
    gooseTxMessage->gocd[num].fcda = (FuncConDatAttr*)calloc(sizeof(FuncConDatAttr), gooseTxMessage->gocd[num].numDatSetEntriess);

    if (!gooseTxMessage->gocd[num].fcda)
    {
        printf("DEBUG:%s():%d:gooseTxMessage->gocd[%d].fcda Malloc Fail!\n", __FUNCTION__, __LINE__, num);
        return;
    }

}

/*
 * @brief Goose接收信息结构体的INPUT结构体的初始化
 * @param gooseRxMessage Goose接收结构体指针
 * 
 */
static void GoInputStructInit(GooseRxMessage* gooseRxMessage)
{

    gooseRxMessage->input = (GooseInput*)calloc(sizeof(GooseInput), gooseRxMessage->numInput);
    
    if (!gooseRxMessage->input)
    {
        printf("DEBUG:%s():%d:gooseRxMessage->input Malloc Fail!\n", __FUNCTION__, __LINE__);
        return;
    }

    
}

/*
 * @brief 提取已经解析的字典中键名最后一个冒号后边的字符串
 * @param srcStr 字典中的键名的字符串
 * @param destStr 最终提取出的字符串
 * 
 */
static void DictStrTrim(uint8_t* srcStr, uint8_t* destStr)
{
    uint8_t* pStr = srcStr;
    uint8_t* pTemp = pStr;
    uint8_t i = 0;
    
    memset(destStr, 0, 20);
    for( ; *pStr!='\0'; pStr++)
    {
        if(*pStr == ':')
        {
            pTemp = pStr + 1;
        }
    }
    for(; *pTemp!='\0'; pTemp++)
    {
        destStr[i] = *pTemp;
        i++;
    }
    destStr[i] = '\0';
}

/*
 * @brief 将字符串转为int类型的数字
 * @param str 将要转换的字符串
 * @return 转换完成的数
 * 
 */
static uint32_t CharToInt(uint8_t* str)
{
    uint32_t num = 0;
    uint8_t i = 0;
    uint8_t pStr[10] = {0};
    for(i=0; str[i]!='\0'; i++)
    {
        pStr[i] = str[i];
    }
     pStr[i] = '\0';
    for(i=0; pStr[i]!='\0'; i++)
    {
        num *= 10;
        num += (pStr[i] - '0');
    }

    return num;
}

/*
 * @brief 将字符串中的16进制地址提取出来，转换为int类型的数
 * @param str 将要转换的字符串
 * @param  ret 转换完成的数组,作为输出参数
 * 
 * @example "01-0C-CD-01-00-11"   ->   {0x01, 0x0C, 0xCD, 0x01, 0x00, 0x11}
 */
static void AddrToInt(uint8_t* str, uint8_t* ret)
{
    uint32_t i = 0;
    uint8_t* pStr = str;
    uint32_t num = 0;
    uint8_t hexStr[5] = {0};           /* 拆分出单个的十六进制值字符串 */

    for(; *pStr!='\0' && num<strlen(str); pStr++)
    {
        hexStr[i] = *pStr;
        i++;
        if(i == 2)
        {
            ret[num] = HexToInt(hexStr, 2);
            num++;
        }
        if(*pStr == '-')
        {
            i = 0;
            continue;
        }
    }

}

/*
 * @brief 将16进制的字符串转为int类型的数字
 * @param str 将要转换的字符串
 * @param size 将要转换的字符串的大小
 * @return 转换完成的数
 * 
 * @example "CD"   ->   0xCD
 */
static uint32_t HexToInt(uint8_t* str, uint8_t size)
{
    uint8_t i = 0;
    uint32_t retVal = 0;            /* 返回值 */
    uint8_t pStr[5] = {0};
    strncpy(pStr, str, size);
    for(i=0; i<size; i++)
    {
        retVal *= 16;
        if((pStr[i] >= 'A') && (pStr[i] <= 'F'))
        {
            retVal += (pStr[i] - 'A' + 10);
        }
        else if((pStr[i] >= 'a') && (pStr[i] <= 'f'))
        {
            retVal += (pStr[i] - 'a' + 10);
        }
        else if((pStr[i] >= '0') && (pStr[i] <= '9'))
        {
            retVal += (pStr[i] - '0');
        }
        else
        {
            return 0;
        }
    }

    return retVal;
}

/*
 * @brief 将goose发送和接收的结构体中的所有空间释放，可以只输入一个参数，另一个参数为NULL
 * @param gooseTxMessage 发送结构体指针
 * @param gooseRxMessage 接收结构体指针
 * 
 */
void FreeGooseMessageMem(GooseTxMessage* gooseTxMessage, GooseRxMessage* gooseRxMessage)
{
    uint32_t i = 0;
    /* 释放发送结构体中的空间 */
    if(gooseTxMessage != NULL)
    {
        for(i=0; i<gooseTxMessage->numGoCb; i++)                /* 释放FCDA空间 */
        {
            free(gooseTxMessage->gocd[i].fcda);
            gooseTxMessage->gocd[i].fcda = NULL;
        }
        free(gooseTxMessage->gocd);
        gooseTxMessage->gocd = NULL;
    }
    /* 释放接收结构体中的空间 */
    if(gooseRxMessage != NULL)
    {
        free(gooseRxMessage->gocd);
        gooseRxMessage->gocd = NULL;

        free(gooseRxMessage->input);
        gooseRxMessage->input = NULL;
    }

}

/*
 * @brief 将goose发送和接收的结构体中的所有元素打印出来，可以只输入一个参数，另一个参数为NULL
 * @param gooseTxMessage 发送结构体指针
 * @param gooseRxMessage 接收结构体指针
 * 
 */
void PrintGooseTxRxMessage(GooseTxMessage* gooseTxMessage, GooseRxMessage* gooseRxMessage)
{
    uint32_t i = 0, j = 0, k = 0;
    /* 打印发送结构体中的元素 */
    if(gooseTxMessage != NULL)
    {
        printf("/*******************gooseTxMessage***********************/\r\n");
        printf("[gooseTxMessage]\r\n");
        printf("numGoCB : %d\r\n", gooseTxMessage->numGoCb);
        for(i=0; i<gooseTxMessage->numGoCb; i++)
        {
            printf("\r\n");
            printf("[GOCB%d]\r\n", i+1);
            printf("gocbRef : %s\r\n", gooseTxMessage->gocd[i].gocbRef);
            printf("appID : %s\r\n", gooseTxMessage->gocd[i].appID);
            printf("datSet : %s\r\n", gooseTxMessage->gocd[i].datSet);
            printf("confRev : %d\r\n", gooseTxMessage->gocd[i].confRev);
            printf("numDatSetEntriess : %d\r\n", gooseTxMessage->gocd[i].numDatSetEntriess);
            for(k=0; k<=6; k++)
            {
                if(k == 0)
                {
                    printf("addr :");
                }
                if(k == 6)
                {
                    printf("\r\n");
                    break;
                }
                printf(" 0x%x", gooseTxMessage->gocd[i].addr[k]);
            }
            printf("priority : %d\r\n", gooseTxMessage->gocd[i].priority);
            printf("vid : %d\r\n", gooseTxMessage->gocd[i].vid);
            printf("appid : 0x%x\r\n", gooseTxMessage->gocd[i].appid);
            printf("minTime : %d\r\n", gooseTxMessage->gocd[i].minTime);
            printf("maxTime : %d\r\n", gooseTxMessage->gocd[i].maxTime);
            for(j=0; j<gooseTxMessage->gocd[i].numDatSetEntriess; j++)
            {
                printf("\r\n");
                printf("[FCDA%d]\r\n", j+1);
                printf("ref : %s\r\n", gooseTxMessage->gocd[i].fcda->ref);
                printf("type : %s\r\n", gooseTxMessage->gocd[i].fcda->type);
                printf("inVarName : %s\r\n", gooseTxMessage->gocd[i].fcda->inVarName);
                printf("act : %d\r\n", gooseTxMessage->gocd[i].fcda->act);
            }
        }
        printf("\r\n");
    }

    /* 打印接收结构体中的元素 */
    if(gooseRxMessage != NULL)
    {
        printf("/*******************gooseRxMessage***********************/\r\n");
        printf("[gooseRxMessage]\r\n");
        printf("numGoCB : %d\r\n", gooseRxMessage->numGoCb);
        printf("numInput : %d\r\n", gooseRxMessage->numInput);
        for(i=0; i<gooseRxMessage->numGoCb; i++)
        {
            printf("\r\n");
            printf("[GOCB%d]\r\n", i+1);
            for(k=0; k<=6; k++)
            {
                if(k == 0)
                {
                    printf("addr :");
                }
                if(k == 6)
                {
                    printf("\r\n");
                    break;
                }
                printf(" 0x%x", gooseRxMessage->gocd[i].addr[k]);
            }
            printf("appid : 0x%x\r\n", gooseRxMessage->gocd[i].appid);
            printf("gocbRef : %s\r\n", gooseRxMessage->gocd[i].gocbRef);
            printf("appID : %s\r\n", gooseRxMessage->gocd[i].appID);
            printf("datSet : %s\r\n", gooseRxMessage->gocd[i].datSet);
            printf("confRev : %d\r\n", gooseRxMessage->gocd[i].confRev);
            printf("numDatSetEntriess : %d\r\n", gooseRxMessage->gocd[i].numDatSetEntriess);
        }
        for(j=0; j<gooseRxMessage->numInput; j++)
        {
            printf("\r\n");
            printf("[INPUT%d]\r\n", j+1);
            printf("gocbIndex : %d\r\n", gooseRxMessage->input[j].gocbIndex);
            printf("gocbEntryIntex : %d\r\n", gooseRxMessage->input[j].gocbEntryIntex);
            printf("ref : %s\r\n", gooseRxMessage->input[j].ref);
            printf("type : %s\r\n", gooseRxMessage->input[j].type);
            printf("outVarName : %s\r\n", gooseRxMessage->input[j].outVarName);
        }
        printf("\r\n");
    }
}



/*------------------------------FILE END---------------------------------------*/
