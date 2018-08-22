#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "iniparser.h"
#include "goose.h"

int main(int argc, char* argv[])
{
    dictionary* ini;
    uint8_t* iniName;
    GooseTxRxMessage gooseTxMassage, gooseRxMassage;

    GooseMessageInit(&gooseTxMassage, &gooseRxMassage);

    if (argc < 2)
    {
        return 1;
    }
    else
    {
        iniName = argv[1];
    }

    ini = iniparser_load(iniName);
    IniToStruct(ini, &gooseTxMassage, &gooseRxMassage);
    iniparser_freedict(ini);

    printf("main: %d\r\n", gooseTxMassage.gocd[0].numDatSetEntriess);

    return 0;
}



/*--------------------------------------------------------------------------*/
void IniToStruct(const dictionary* dict, GooseTxRxMessage* gooseTxMassage, GooseTxRxMessage* gooseRxMassage)
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
                if(strncmp(dict->key[i], "GOOSE Tx", strlen("GOOSE Tx")) == 0)
                {
                    DictStrTrim(dict->key[i], strTrim);
                    if(strcmp(strTrim, "numGoCb") == 0)
                    {
                        gooseTxMassage->numGoCb = CharToInt(dict->val[i]);
                        GocbStructInit(gooseTxMassage);
                    }
                }
                else if(strncmp(dict->key[i], "GoCB", strlen("GoCB")) == 0)
                {
                    DictStrTrim(dict->key[i], strTrim);
                    if(strcmp(strTrim, "GoCBRef") == 0)
                    {
                        strncpy(gooseTxMassage->gocd[numGocb-1].gocbRef, dict->val[i], strlen(dict->val[i])+1);
                    }
                    else if(strcmp(strTrim, "AppID") == 0)
                    {
                        strncpy(gooseTxMassage->gocd[numGocb-1].appID, dict->val[i], strlen(dict->val[i])+1);
                    }
                    else if(strcmp(strTrim, "DatSet") == 0)
                    {
                        strncpy(gooseTxMassage->gocd[numGocb-1].datSet, dict->val[i], strlen(dict->val[i])+1);
                    }
                    else if(strcmp(strTrim, "ConfRev") == 0)
                    {
                        gooseTxMassage->gocd[numGocb-1].confRev = CharToInt(dict->val[i]);
                    }
                    else if(strcmp(strTrim, "numDatSetEntries") == 0)
                    {
                        gooseTxMassage->gocd[numGocb-1].numDatSetEntriess = CharToInt(dict->val[i]);
                        FcdaStructInit(gooseTxMassage);
                    }
                }
                else if(strncmp(dict->key[i], "DstAddr", strlen("DstAddr")) == 0)
                {
                    DictStrTrim(dict->key[i], strTrim);
                    if(strcmp(strTrim, "Addr") == 0)
                    {
                        AddrToInt(dict->val[i], gooseTxMassage->gocd[numGocb-1].addr);
                    }
                    else if(strcmp(strTrim, "Priority") == 0)
                    {
                        gooseTxMassage->gocd[numGocb-1].priority = CharToInt(dict->val[i]);
                    }
                    else if(strcmp(strTrim, "VID") == 0)
                    {
                        gooseTxMassage->gocd[numGocb-1].vid = CharToInt(dict->val[i]);
                    }
                    else if(strcmp(strTrim, "Appid") == 0)
                    {
                        gooseTxMassage->gocd[numGocb-1].appid = HexToInt(dict->val[i], 4);
                    }
                    else if(strcmp(strTrim, "MinTime") == 0)
                    {
                        gooseTxMassage->gocd[numGocb-1].minTime = CharToInt(dict->val[i]);
                    }
                    else if(strcmp(strTrim, "MaxTime") == 0)
                    {
                        gooseTxMassage->gocd[numGocb-1].maxTime = CharToInt(dict->val[i]);
                    }
                }
                else if(strncmp(dict->key[i], "FCDA", strlen("FCDA")) == 0)
                {
                    DictStrTrim(dict->key[i], strTrim);
                    if(strcmp(strTrim, "Ref") == 0)
                    {
                        strncpy(gooseTxMassage->gocd[numGocb-1].device.fcda[numFcda-1].ref, dict->val[i], strlen(dict->val[i])+1);
                    }
                    else if(strcmp(strTrim, "Type") == 0)
                    {
                        strncpy(gooseTxMassage->gocd[numGocb-1].device.fcda[numFcda-1].type, dict->val[i], strlen(dict->val[i])+1);
                    }
                    else if(strcmp(strTrim, "InVarName") == 0)
                    {
                        strncpy(gooseTxMassage->gocd[numGocb-1].device.fcda[numFcda-1].inVarName, dict->val[i], strlen(dict->val[i])+1);
                    }
                    else if(strcmp(strTrim, "ACT") == 0)
                    {
                        gooseTxMassage->gocd[numGocb-1].device.fcda[numFcda-1].act = CharToInt(dict->val[i]);
                    }
                }

            }
            else if(flag == 2)                  /* RX */
            {
                if(strncmp(dict->key[i], "GOOSE Rx", strlen("GOOSE Rx")) == 0)
                {
                    DictStrTrim(dict->key[i], strTrim);
                    if(strcmp(strTrim, "numGoCb") == 0)
                    {
                        gooseRxMassage->numGoCb = CharToInt(dict->val[i]);
                        GocbStructInit(gooseRxMassage);
                    }
                    else if(strcmp(strTrim, "numInput") == 0)
                    {
                        gooseRxMassage->num.numInput = CharToInt(dict->val[i]);
                    }
                }
                else if(strncmp(dict->key[i], "GoCB", strlen("GoCB")) == 0)
                {
                    DictStrTrim(dict->key[i], strTrim);
                    if(strcmp(dict->key[i], "Addr") == 0)
                    {
                        AddrToInt(dict->val[i], gooseRxMassage->gocd[numGocb-1].addr);
                    }
                    else if(strcmp(dict->key[i], "Appid") == 0)
                    {
                        gooseRxMassage->gocd[numGocb-1].appid = HexToInt(dict->val[i], 4);
                    }
                    else if(strcmp(dict->key[i], "GoCBRef") == 0)
                    {
                        strncpy(gooseRxMassage->gocd[numGocb-1].gocbRef, dict->val[i], strlen(dict->val[i])+1);
                    }
                    else if(strcmp(dict->key[i], "AppID") == 0)
                    {
                        strncpy(gooseRxMassage->gocd[numGocb-1].appID, dict->val[i], strlen(dict->val[i])+1);
                    }
                    else if(strcmp(dict->key[i], "DatSet") == 0)
                    {
                        strncpy(gooseRxMassage->gocd[numGocb-1].datSet, dict->val[i], strlen(dict->val[i])+1);
                    }
                    else if(strcmp(dict->key[i], "ConfRev") == 0)
                    {
                        gooseRxMassage->gocd[numGocb-1].confRev = CharToInt(dict->val[i]);
                    }
                    else if(strcmp(dict->key[i], "numDatSetEntries") == 0)
                    {
                        gooseRxMassage->gocd[numGocb-1].numDatSetEntriess = CharToInt(dict->val[i]);
                        GoInputStructInit(gooseRxMassage);
                    }
                }
                else if(strncmp(dict->key[i], "INPUT", strlen("INPUT")) == 0)
                {
                    DictStrTrim(dict->key[i], strTrim);
                    if(strcmp(dict->key[i], "GoCbIndex") == 0)
                    {
                        gooseRxMassage->gocd[numGocb-1].device.input[numInput-1].gocbIndex = CharToInt(dict->val[i]);
                    }
                    else if(strcmp(dict->key[i], "GoCbEntryIndex") == 0)
                    {
                        gooseRxMassage->gocd[numGocb-1].device.input[numInput-1].gocbEntryIntex = CharToInt(dict->val[i]);
                    }
                    else if(strcmp(dict->key[i], "Ref") == 0)
                    {
                        strncpy(gooseRxMassage->gocd[numGocb-1].device.input[numInput-1].ref, dict->val[i], strlen(dict->val[i])+1);
                    }
                    else if(strcmp(dict->key[i], "Type") == 0)
                    {
                        strncpy(gooseRxMassage->gocd[numGocb-1].device.input[numInput-1].type, dict->val[i], strlen(dict->val[i])+1);
                    }
                    else if(strcmp(dict->key[i], "OutVarName") == 0)
                    {
                        strncpy(gooseRxMassage->gocd[numGocb-1].device.input[numInput-1].outVarName, dict->val[i], strlen(dict->val[i])+1);
                    }
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

void GooseMessageInit(GooseTxRxMessage* gooseTxMassage, GooseTxRxMessage* gooseRxMassage)
{
    gooseTxMassage->flagTxRx = 1;
    gooseRxMassage->flagTxRx = 2;

    gooseTxMassage->numGoCb = 0;
    gooseRxMassage->numGoCb = 0;

    gooseTxMassage->num.numInput = 0;
    gooseRxMassage->num.numOutput = 0;

    gooseTxMassage->gocd = NULL;
    gooseRxMassage->gocd = NULL;

}

void GocbStructInit(GooseTxRxMessage* gooseMassage)
{
    uint32_t i = 0;
   
    gooseMassage->gocd  = (GooseControlBlock*)calloc(sizeof(GooseControlBlock), gooseMassage->numGoCb);
   
    if (!gooseMassage->gocd)
    {
        printf("DEBUG:%s():%d:gooseMassage->gocd Malloc!\n", __FUNCTION__, __LINE__);
        return;
    }

    for(i=0; i<gooseMassage->numGoCb; i++)
    {
        if(gooseMassage->flagTxRx == 1)
        {
            gooseMassage->gocd[i].device.fcda == NULL;
        }
        if(gooseMassage->flagTxRx == 2)
        {
            gooseMassage->gocd[i].device.input == NULL;
        }
    }
}


void FcdaStructInit(GooseTxRxMessage* gooseMassage)
{
    uint32_t j = 0;
    for(j=0; j<gooseMassage->numGoCb; j++)
    {
        gooseMassage->gocd[j].device.fcda = (FuncConDatAttr*)calloc(sizeof(FuncConDatAttr), gooseMassage->gocd[j].numDatSetEntriess);
    }
    
}


void GoInputStructInit(GooseTxRxMessage* gooseMassage)
{
    uint32_t j = 0;
    for(j=0; j<gooseMassage->numGoCb; j++)
    {
        gooseMassage->gocd[j].device.input = (GooseInput*)calloc(sizeof(GooseInput), gooseMassage->gocd[j].numDatSetEntriess);
    }
    
}

void DictStrTrim(uint8_t* srcStr, uint8_t* destStr)
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

uint32_t CharToInt(uint8_t* str)
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

void AddrToInt(uint8_t* str, uint8_t* ret)
{
    uint32_t i = 0;
    uint8_t* pStr = str;
    uint32_t num = 0;
    uint8_t hexStr[5] = {0};           /* 拆分出单个的十六进制值字符串 */

    for(; *pStr!='\0' && num<6; pStr++)
    {
        hexStr[i] = *pStr;
        i++;
        if(i = 2)
        {
            ret[num] = HexToInt(hexStr, 2);
            num++;
        }
        if(*pStr == '-')
        {
            continue;
            i = 0;
        }
    }

}

uint32_t HexToInt(uint8_t* str, uint8_t size)
{
    uint8_t i = 0;
    uint32_t retVal = 0;            /* 返回值 */
    uint8_t pStr[5] = {0};
    strncpy(pStr, str, size);

    for(i=0; i<size; i++)
    {
        retVal *= 16;
        if((*pStr >= 'A') && (*pStr <= 'F'))
        {
            retVal += *pStr - 'A' + 10;
        }
        else if((*pStr >= 'a') && (*pStr <= 'f'))
        {
            retVal += *pStr - 'a' + 10;
        }
        else if((*pStr >= '0') && (*pStr <= '9'))
        {
            retVal += *pStr - '0';
        }
        else
        {
            return 0;
        }
    }

    return retVal;
}

void myStrCopy(uint8_t* destStr, uint8_t* srcStr, uint32_t size)
{

}