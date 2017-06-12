#include "file_cache_tool.h"

extern char *optarg;
extern int optind, opterr;

static void usage(void){
    fprintf(stderr, "usage: file_cache_tool [options]\n");
    fprintf(stderr, " -%c number: %s\n", OPT_SET_MAX , "set max size (KiB) ex. 16384"); 
    fprintf(stderr, " -%c number: %s\n", OPT_SET_MIN , "set min size (KiB) ex. 83"); 
    fprintf(stderr, " -%c: %s\n", OPT_GET_INFO , "get info");     
    fprintf(stderr, " -%c: %s\n", OPT_FLUSH , "flush");  
    
    exit(1);
}

int main(int argc, char *argv[])
{
    if(argc == 1) usage();
    
    int ch;
    
    while ((ch = getopt(argc, argv, OPT_LIST)) != -1){
                
        switch (ch){
            case OPT_SET_MIN:
                setMin(atoi(optarg));
                break;
            case OPT_SET_MAX:
                setMax(atoi(optarg));
                break;
            case OPT_GET_INFO:
                getInfo();
                break; 
            case OPT_FLUSH:
                flush();
                break;                  
            default:
                usage();
        }
    } 
    
    return 0;
}

void setMin(unsigned int min){
    if(SetPrivilege(SE_INCREASE_QUOTA_NAME)){
        
        SIZE_T max, old;DWORD flags;
        
        if(!GetSystemFileCacheSize(&old, &max, &flags)){
            fprintf(stderr, "GetSystemFileCacheSize error: %lu\n", GetLastError());  
            errorInfo();
        }
        
        if(!SetSystemFileCacheSize(min * 1024, max, FILE_CACHE_MIN_HARD_ENABLE)){
            fprintf(stderr, "SetSystemFileCacheSize error: %lu\n", GetLastError()); 
            errorInfo();
        }
    }
    getInfo();
}

void setMax(unsigned int max){
    
    if(SetPrivilege(SE_INCREASE_QUOTA_NAME)){
        
        SIZE_T min, old;DWORD flags;
        
        if(!GetSystemFileCacheSize(&min, &old, &flags)){
            fprintf(stderr, "GetSystemFileCacheSize error: %lu\n", GetLastError());  
            errorInfo();
        }
        
        if(!SetSystemFileCacheSize(min, max * 1024, FILE_CACHE_MAX_HARD_ENABLE)){
            fprintf(stderr, "SetSystemFileCacheSize error: %lu\n", GetLastError()); 
            errorInfo();
        }
    }
    getInfo();
}

void getInfo(){
    SIZE_T min, max;DWORD flags;
    
    if(!GetSystemFileCacheSize(&min, &max, &flags)){
        fprintf(stderr, "GetSystemFileCacheSize error: %lu\n", GetLastError());  
        errorInfo();
    }
    
    fprintf(stderr, "Minimum File Cache Size:\t%lu bytes\n", min); 
    fprintf(stderr, "Maximum File Cache Size:\t%lu bytes\n", max); 
    fprintf(stderr, "Minimum File Cache Size Enabled:\t%s\n", !!(flags & FILE_CACHE_MIN_HARD_ENABLE) ? "yes" : "no"); 
    fprintf(stderr, "Maximum File Cache Size Enabled:\t%s\n", !!(flags & FILE_CACHE_MAX_HARD_ENABLE) ? "yes" : "no"); 
}

void flush(){
    if (SetPrivilege(SE_INCREASE_QUOTA_NAME)) {

        if(!SetSystemFileCacheSize(-1, -1, 0)){
            fprintf(stderr, "SetSystemFileCacheSize error: %lu\n", GetLastError());
            errorInfo();
        }
    }
}

static void errorInfo(){
    fprintf(stderr, "%lu: %s\n", ERROR_ACCESS_DENIED, "the operation requires higher credentials."); 
    fprintf(stderr, "%lu: %s\n", ERROR_INVALID_PARAMETER, "the value is probably too small or too large."); 
    fprintf(stderr, "For more information: %s\n", "http://msdn.microsoft.com/en-us/library/ms681382(v=vs.85).aspx"); 
}

BOOL SetPrivilege(LPCTSTR lpszPrivilege)
{
    TOKEN_PRIVILEGES tp;
    LUID luid;
    HANDLE hToken = NULL;
    
    if(!OpenProcessToken(GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, &hToken)){
        fprintf(stderr, "OpenProcessToken error: %lu\n", GetLastError()); 
        return FALSE;
    }
    
    if(!LookupPrivilegeValue(NULL, lpszPrivilege, &luid)){
        fprintf(stderr, "LookupPrivilegeValue error: %lu\n", GetLastError()); 
        return FALSE;
    }
    
    tp.PrivilegeCount = 1;
    tp.Privileges[0].Luid = luid;
    tp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;
        
    if(!AdjustTokenPrivileges(hToken, FALSE, &tp, 
                              sizeof(TOKEN_PRIVILEGES), (PTOKEN_PRIVILEGES)NULL, (PDWORD)NULL)){ 
        fprintf(stderr, "AdjustTokenPrivileges error: %lu\n", GetLastError()); 
        return FALSE;
    } 
    
    if(GetLastError() == ERROR_NOT_ALL_ASSIGNED){
        fprintf(stderr, "The token does not have the specified privilege.\n"); 
        return FALSE;
    } 
    
    CloseHandle(hToken);
    
    return TRUE;
}
