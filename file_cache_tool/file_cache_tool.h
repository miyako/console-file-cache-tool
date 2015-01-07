#ifndef __FILE_CACHE_TOOL_H__
#define __FILE_CACHE_TOOL_H__

#define OPT_SET_MAX         'm'
#define OPT_SET_MIN         'n'
#define OPT_GET_INFO        'i'
#define OPT_FLUSH           'l'

#define OPT_LIST            "m:n:il"

#include <stdlib.h>
#include <stdio.h>
#include <getopt.h>
#ifdef WIN32
#include <windows.h>
//#include <strsafe.h>
//#include <pdh.h>
//#include <pdhmsg.h>
//#include <Psapi.h>
#endif

void setMin(unsigned int);
void setMax(unsigned int);
void getInfo(void);
void flush(void);
BOOL SetPrivilege(LPCTSTR);
static void errorInfo(void);
#endif