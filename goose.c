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
    // iniparser_dump(ini, stdout);
    IniToStruct(ini, &gooseTxMassage, &gooseRxMassage);
    iniparser_freedict(ini);


    printf("/***************gooseTxMassage**********************/\r\n");
    printf("gooseTxMassage:\r\n");
    printf("numGoCb: %d\r\n", gooseTxMassage.numGoCb);
    printf("gocd:\r\n");
    printf("gocbRef: %s\r\n", gooseTxMassage.gocd[0].gocbRef);
    printf("appID: %s\r\n", gooseTxMassage.gocd[0].appID);
    printf("datSet: %s\r\n", gooseTxMassage.gocd[0].datSet);
    printf("confRev: %d\r\n", gooseTxMassage.gocd[0].confRev);
    printf("numDatSetEntriess: %d\r\n", gooseTxMassage.gocd[0].numDatSetEntriess);
    printf("addr[5]: %d\r\n", gooseTxMassage.gocd[0].addr[5]);
    printf("priority: %d\r\n", gooseTxMassage.gocd[0].priority);
    printf("vid: %d\r\n", gooseTxMassage.gocd[0].vid);
    printf("appid: 0x%x\r\n", gooseTxMassage.gocd[0].appid);
    printf("minTime: %d\r\n", gooseTxMassage.gocd[0].minTime);
    printf("maxTime: %d\r\n", gooseTxMassage.gocd[0].maxTime);
    printf("fcda1:\r\n");
    printf("ref: %s\r\n", gooseTxMassage.gocd[0].device.fcda[0].ref);
    printf("type: %s\r\n", gooseTxMassage.gocd[0].device.fcda[0].type);
    printf("inVarName: %s\r\n", gooseTxMassage.gocd[0].device.fcda[0].inVarName);
    printf("act: %d\r\n", gooseTxMassage.gocd[0].device.fcda[0].act);
    printf("fcda2:\r\n");
    printf("ref: %s\r\n", gooseTxMassage.gocd[0].device.fcda[1].ref);
    printf("type: %s\r\n", gooseTxMassage.gocd[0].device.fcda[1].type);
    printf("inVarName: %s\r\n", gooseTxMassage.gocd[0].device.fcda[1].inVarName);
    printf("act: %d\r\n", gooseTxMassage.gocd[0].device.fcda[1].act);
    
    printf("/**************gooseRxMassage***********************/\r\n");
    printf("gooseRxMassage:\r\n");
    printf("numGoCb: %d\r\n", gooseRxMassage.numGoCb);
    printf("numInput: %d\r\n", gooseRxMassage.num.numInput);
    printf("gocd:\r\n");
    printf("addr[5]: %d\r\n", gooseRxMassage.gocd[0].addr[5]);
    printf("appid: %d\r\n", gooseRxMassage.gocd[0].appid);
    printf("gocbRef: %s\r\n", gooseRxMassage.gocd[0].gocbRef);
    printf("appID: %s\r\n", gooseRxMassage.gocd[0].appID);
    printf("datSet: %s\r\n", gooseRxMassage.gocd[0].datSet);
    printf("confRev: %d\r\n", gooseRxMassage.gocd[0].confRev);
    printf("numDatSetEntriess: %d\r\n", gooseRxMassage.gocd[0].numDatSetEntriess);
    printf("input1:\r\n");
    printf("gocbIndex: %d\r\n", gooseRxMassage.gocd[0].device.input[0].gocbIndex);
    printf("gocbEntryIntex: %d\r\n", gooseRxMassage.gocd[0].device.input[0].gocbEntryIntex);
    printf("ref: %s\r\n", gooseRxMassage.gocd[0].device.input[0].ref);
    printf("type: %s\r\n", gooseRxMassage.gocd[0].device.input[0].type);
    printf("outVarName: %s\r\n", gooseRxMassage.gocd[0].device.input[0].outVarName);
    printf("input2:\r\n");
    printf("gocbIndex: %d\r\n", gooseRxMassage.gocd[0].device.input[1].gocbIndex);
    printf("gocbEntryIntex: %d\r\n", gooseRxMassage.gocd[0].device.input[1].gocbEntryIntex);
    printf("ref: %s\r\n", gooseRxMassage.gocd[0].device.input[1].ref);
    printf("type: %s\r\n", gooseRxMassage.gocd[0].device.input[1].type);
    printf("outVarName: %s\r\n", gooseRxMassage.gocd[0].device.input[1].outVarName);
    printf("input3:\r\n");
    printf("gocbIndex: %d\r\n", gooseRxMassage.gocd[0].device.input[0].gocbIndex);
    printf("gocbEntryIntex: %d\r\n", gooseRxMassage.gocd[1].device.input[0].gocbEntryIntex);
    printf("ref: %s\r\n", gooseRxMassage.gocd[0].device.input[2].ref);
    printf("type: %s\r\n", gooseRxMassage.gocd[0].device.input[2].type);
    printf("outVarName: %s\r\n", gooseRxMassage.gocd[0].device.input[2].outVarName);


    return 0;
}



/*--------------------------------------------------------------------------*/
void IniToStruct(const dictionary* dict, GooseTxRxMessage* gooseTxMassage, GooseTxRxMessage* gooseRxMassage)
{
    uint32_t i = 0, numGocb = 0, numFcda = 0, numInput = 0;
    uint8_t flag = 0;
    uint8_t strTrim[20] = {0};
uint8_t a = 0;
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
                        FcdaStructInit(gooseTxMassage, numGocb-1);
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
                    if(strcmp(strTrim, "Addr") == 0)
                    {
                        AddrToInt(dict->val[i], gooseRxMassage->gocd[numGocb-1].addr);
                    }
                    else if(strcmp(strTrim, "Appid") == 0)
                    {
                        gooseRxMassage->gocd[numGocb-1].appid = HexToInt(dict->val[i], 4);
                    }
                    else if(strcmp(strTrim, "GoCBRef") == 0)
                    {
                        strncpy(gooseRxMassage->gocd[numGocb-1].gocbRef, dict->val[i], strlen(dict->val[i])+1);
                    }
                    else if(strcmp(strTrim, "AppID") == 0)
                    {
                        strncpy(gooseRxMassage->gocd[numGocb-1].appID, dict->val[i], strlen(dict->val[i])+1);
                    }
                    else if(strcmp(strTrim, "DatSet") == 0)
                    {
                        strncpy(gooseRxMassage->gocd[numGocb-1].datSet, dict->val[i], strlen(dict->val[i])+1);
                    }
                    else if(strcmp(strTrim, "ConfRev") == 0)
                    {
                        gooseRxMassage->gocd[numGocb-1].confRev = CharToInt(dict->val[i]);
                    }
                    else if(strcmp(strTrim, "numDatSetEntries") == 0)
                    {
                        gooseRxMassage->gocd[numGocb-1].numDatSetEntriess = CharToInt(dict->val[i]);
                        GoInputStructInit(gooseRxMassage, numGocb-1);
                    }
                }
                else if(strncmp(dict->key[i], "INPUT", strlen("INPUT")) == 0)
                {
                    DictStrTrim(dict->key[i], strTrim);
                    printf("%s\r\n", strTrim);
                    if(strcmp(strTrim, "GoCbIndex") == 0)
                    {
                        gooseRxMassage->gocd[numGocb-1].device.input[numInput-1].gocbIndex = CharToInt(dict->val[i]);
                    }
                    else if(strcmp(strTrim, "GoCbEntryIndex") == 0)
                    {
                        gooseRxMassage->gocd[numGocb-1].device.input[numInput-1].gocbEntryIntex = CharToInt(dict->val[i]);
                    }
                    else if(strcmp(strTrim, "Ref") == 0)
                    {
                        strncpy(gooseRxMassage->gocd[numGocb-1].device.input[numInput-1].ref, dict->val[i], strlen(dict->val[i])+1);
                    }
                    else if(strcmp(strTrim, "Type") == 0)
                    {
                        strncpy(gooseRxMassage->gocd[numGocb-1].device.input[numInput-1].type, dict->val[i], strlen(dict->val[i])+1);
                    }
                    else if(strcmp(strTrim, "OutVarName") == 0)
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
        printf("DEBUG:%s():%d:gooseMassage->gocd Malloc Fail!\n", __FUNCTION__, __LINE__);
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


void FcdaStructInit(GooseTxRxMessage* gooseMassage, uint32_t num)
{
    
    gooseMassage->gocd[num].device.fcda = (FuncConDatAttr*)calloc(sizeof(FuncConDatAttr), gooseMassage->gocd[num].numDatSetEntriess);

    if (!gooseMassage->gocd[num].device.fcda)
    {
        printf("DEBUG:%s():%d:gooseMassage->gocd[%d].device.fcda Malloc Fail!\n", __FUNCTION__, __LINE__, num);
        return;
    }

}


void GoInputStructInit(GooseTxRxMessage* gooseMassage, uint32_t num)
{

    gooseMassage->gocd[num].device.input = (GooseInput*)calloc(sizeof(GooseInput), gooseMassage->gocd[num].numDatSetEntriess);
    
    if (!gooseMassage->gocd[num].device.input)
    {
        printf("DEBUG:%s():%d:gooseMassage->gocd[%d].device.input Malloc Fail!\n", __FUNCTION__, __LINE__, num);
        return;
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

uint32_t HexToInt(uint8_t* str, uint8_t size)
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
