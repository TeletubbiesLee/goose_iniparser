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

    printf("main: %d\r\n", gooseTxMassage.numGoCb);

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
                        strncpy(gooseTxMassage->gocd[numGocb]->gocbRef, dict->val[i], 20);
                        printf("%s\r\n", dict->val[i]);
                        printf("%s\r\n", gooseTxMassage->gocd[numGocb]->gocbRef);
                        printf("end\n");
                    }
                    else if(strcmp(strTrim, "AppID") == 0)
                    {
                        strncpy(gooseTxMassage->gocd[numGocb]->appID, dict->val[i], strlen(dict->val[i]));
                    }
                    else if(strcmp(strTrim, "DatSet") == 0)
                    {
                        strncpy(gooseTxMassage->gocd[numGocb]->datSet, dict->val[i], strlen(dict->val[i]));
                    }
                    else if(strcmp(strTrim, "ConfRev") == 0)
                    {
                        gooseTxMassage->gocd[numGocb]->confRev = CharToInt(dict->val[i]);
                    }
                    else if(strcmp(strTrim, "numDatSetEntries") == 0)
                    {
                        gooseTxMassage->gocd[numGocb]->numDatSetEntriess = CharToInt(dict->val[i]);
                        FcdaStructInit(gooseTxMassage);
                    }
                }
                else if(strncmp(dict->key[i], "DstAddr", strlen("DstAddr")) == 0)
                {
                    DictStrTrim(dict->key[i], strTrim);
                    if(strcmp(strTrim, "Addr") == 0)
                    {
                        AddrToInt(dict->val[i], gooseTxMassage->gocd[numGocb]->addr);
                    }
                    else if(strcmp(strTrim, "Priority") == 0)
                    {
                        gooseTxMassage->gocd[numGocb]->priority = CharToInt(dict->val[i]);
                    }
                    else if(strcmp(strTrim, "VID") == 0)
                    {
                        gooseTxMassage->gocd[numGocb]->vid = CharToInt(dict->val[i]);
                    }
                    else if(strcmp(strTrim, "Appid") == 0)
                    {
                        gooseTxMassage->gocd[numGocb]->appid = HexToInt(dict->val[i], 4);
                    }
                    else if(strcmp(strTrim, "MinTime") == 0)
                    {
                        gooseTxMassage->gocd[numGocb]->minTime = CharToInt(dict->val[i]);
                    }
                    else if(strcmp(strTrim, "MaxTime") == 0)
                    {
                        gooseTxMassage->gocd[numGocb]->maxTime = CharToInt(dict->val[i]);
                    }
                }
                else if(strncmp(dict->key[i], "FCDA", strlen("FCDA")) == 0)
                {
                    DictStrTrim(dict->key[i], strTrim);
                    if(strcmp(strTrim, "Ref") == 0)
                    {
                        strncpy(gooseTxMassage->gocd[numGocb]->device.fcda[numFcda].ref, dict->val[i], strlen(dict->val[i]));
                    }
                    else if(strcmp(strTrim, "Type") == 0)
                    {
                        strncpy(gooseTxMassage->gocd[numGocb]->device.fcda[numFcda].type, dict->val[i], strlen(dict->val[i]));
                    }
                    else if(strcmp(strTrim, "InVarName") == 0)
                    {
                        strncpy(gooseTxMassage->gocd[numGocb]->device.fcda[numFcda].inVarName, dict->val[i], strlen(dict->val[i]));
                    }
                    else if(strcmp(strTrim, "ACT") == 0)
                    {
                        gooseTxMassage->gocd[numGocb]->device.fcda[numFcda].act = CharToInt(dict->val[i]);
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
                        AddrToInt(dict->val[i], gooseRxMassage->gocd[numGocb]->addr);
                    }
                    else if(strcmp(dict->key[i], "Appid") == 0)
                    {
                        gooseRxMassage->gocd[numGocb]->appid = HexToInt(dict->val[i], 4);
                    }
                    else if(strcmp(dict->key[i], "GoCBRef") == 0)
                    {
                        strncpy(gooseRxMassage->gocd[numGocb]->gocbRef, dict->val[i], strlen(dict->val[i]));
                    }
                    else if(strcmp(dict->key[i], "AppID") == 0)
                    {
                        strncpy(gooseRxMassage->gocd[numGocb]->appID, dict->val[i], strlen(dict->val[i]));
                    }
                    else if(strcmp(dict->key[i], "DatSet") == 0)
                    {
                        strncpy(gooseRxMassage->gocd[numGocb]->datSet, dict->val[i], strlen(dict->val[i]));
                    }
                    else if(strcmp(dict->key[i], "ConfRev") == 0)
                    {
                        gooseRxMassage->gocd[numGocb]->confRev = CharToInt(dict->val[i]);
                    }
                    else if(strcmp(dict->key[i], "numDatSetEntries") == 0)
                    {
                        gooseRxMassage->gocd[numGocb]->numDatSetEntriess = CharToInt(dict->val[i]);
                        GoInputStructInit(gooseRxMassage);
                    }
                }
                else if(strncmp(dict->key[i], "INPUT", strlen("INPUT")) == 0)
                {
                    DictStrTrim(dict->key[i], strTrim);
                    if(strcmp(dict->key[i], "GoCbIndex") == 0)
                    {
                        gooseRxMassage->gocd[numGocb]->device.input[numInput].gocbIndex = CharToInt(dict->val[i]);
                    }
                    else if(strcmp(dict->key[i], "GoCbEntryIndex") == 0)
                    {
                        gooseRxMassage->gocd[numGocb]->device.input[numInput].gocbEntryIntex = CharToInt(dict->val[i]);
                    }
                    else if(strcmp(dict->key[i], "Ref") == 0)
                    {
                        strncpy(gooseRxMassage->gocd[numGocb]->device.input[numInput].ref, dict->val[i], strlen(dict->val[i]));
                    }
                    else if(strcmp(dict->key[i], "Type") == 0)
                    {
                        strncpy(gooseRxMassage->gocd[numGocb]->device.input[numInput].type, dict->val[i], strlen(dict->val[i]));
                    }
                    else if(strcmp(dict->key[i], "OutVarName") == 0)
                    {
                        strncpy(gooseRxMassage->gocd[numGocb]->device.input[numInput].outVarName, dict->val[i], strlen(dict->val[i]));
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
    GooseControlBlock* goCB[gooseMassage->numGoCb];
    gooseMassage->gocd = goCB;
    for(i=0; i<gooseMassage->numGoCb; i++)
    {
        goCB[i] = malloc(sizeof(GooseControlBlock));
    }

    for(i=0; i<gooseMassage->numGoCb; i++)
    {
        if(gooseMassage->flagTxRx == 1)
        {
            gooseMassage->gocd[i]->device.fcda == NULL;
        }
        if(gooseMassage->flagTxRx == 2)
        {
            gooseMassage->gocd[i]->device.input == NULL;
        }
    }
}


void FcdaStructInit(GooseTxRxMessage* gooseMassage)
{
    uint32_t j = 0;
    for(j=0; j<gooseMassage->numGoCb; j++)
    {
        FuncConDatAttr funcConDatAttr[gooseMassage->gocd[j]->numDatSetEntriess];
        gooseMassage->gocd[j]->device.fcda = funcConDatAttr;
    }
    
}


void GoInputStructInit(GooseTxRxMessage* gooseMassage)
{
    uint32_t j = 0;
    for(j=0; j<gooseMassage->numGoCb; j++)
    {
        FuncConDatAttr GooseInput[gooseMassage->gocd[j]->numDatSetEntriess];
        gooseMassage->gocd[j]->device.fcda = GooseInput;
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