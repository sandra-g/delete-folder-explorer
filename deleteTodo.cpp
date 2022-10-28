# include <stdlib.h>
# include <stdio.h>
# include <dos.h>
# include <dir.h>
# include <io.h>
# include <conio.h>
#include <stdbool.h>
#include <windows.h>
#include <unistd.h>//yo
//yo
#include <sys/stat.h>//para chmod S_IRU

#include <windows.h>
#include <tchar.h>
#include <shellapi.h>
 
 
bool DeleteDirectory(LPCTSTR lpszDir, bool noRecycleBin = true)
{
	printf("Va a borrar %s\n", lpszDir);
    int len = _tcslen(lpszDir);
    TCHAR *pszFrom = new TCHAR[len + 2];
    strcpy(pszFrom,lpszDir);//len+2, 
    pszFrom[len] = 0;
    pszFrom[len + 1] = 0;
 
    SHFILEOPSTRUCT fileop;
    fileop.hwnd = NULL;    // no status display
    fileop.wFunc = FO_DELETE;  // delete operation
    fileop.pFrom = pszFrom;  // source file name as double null terminated string
    fileop.pTo = NULL;    // no destination needed
    fileop.fFlags = FOF_NOCONFIRMATION | FOF_SILENT;  // do not prompt the user
 
    if (!noRecycleBin)
        fileop.fFlags |= FOF_ALLOWUNDO;
 
    fileop.fAnyOperationsAborted = FALSE;
    fileop.lpszProgressTitle = NULL;
    fileop.hNameMappings = NULL;
 
    int ret = SHFileOperation(&fileop);
    delete[] pszFrom;
    return (ret == 0);
}
 
int main()
{
	//
	WIN32_FIND_DATA fdFile;
    HANDLE hFind = NULL;
	char sPath[2048];
	char c;
	//
	DWORD nBufferLength = MAX_PATH;
	char szCurrentDirectory[MAX_PATH + 1];
	szCurrentDirectory[MAX_PATH +1 ] = '\0';
	printf("Este programa borra todos los folders que se encuentren dentro de este directorio\n");
	printf("Es un programa peligroso. Desea continuar?(S/N):");
	c=getchar();
    if (!(c=='S' || c=='s')) {
    	printf("El programa termina.");
	//return 0;
	}
	else {
		GetCurrentDirectory(nBufferLength, szCurrentDirectory);
		//
		sprintf(sPath, "%s\\*.*", szCurrentDirectory);
	    if((hFind = FindFirstFile(sPath, &fdFile)) == INVALID_HANDLE_VALUE)
	    {
	        printf("Path not found: [%s]\n", szCurrentDirectory);
	        return false;
	    }
		printf("\n");    
		
	    do{
			sprintf(sPath, "%s\\%s", szCurrentDirectory, fdFile.cFileName);//
			printf("--Leido:%s\n", sPath);
	    	if(strcmp(fdFile.cFileName, ".") == 0 || strcmp(fdFile.cFileName, "..") == 0){
	    		
			}
			else {			
				if(fdFile.dwFileAttributes &FILE_ATTRIBUTE_DIRECTORY){
					printf("----Lee un folder\n");
					//ListDirectoryContents(sPath);
					DeleteDirectory(sPath, false);
				}
			}
		}
		while(FindNextFile(hFind, &fdFile));
		//
	    //DeleteDirectory(szCurrentDirectory, false);//("C:\\Users\\Kei\\Documents\\Visual Studio 2017\\Projects\\Project2\\Debug\\Nueva carpeta", false);
	}
	
	system("pause");
	return 0;
}
