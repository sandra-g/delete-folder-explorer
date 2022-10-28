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
//	printf("Va a borrar %s\n", lpszDir);
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

bool RevisarVacio(LPCTSTR sDir, bool noRecycleBin = true) 
{
	//
	WIN32_FIND_DATA fdFile;
    HANDLE hFind = NULL;
	char sPath[2048];
	bool vacio = true;
	bool folderinterno_vacio = true;
	//
	sprintf(sPath, "%s\\*.*", sDir);
	printf("\nDirectorio:%s\n",sDir);
    if((hFind = FindFirstFile(sPath, &fdFile)) == INVALID_HANDLE_VALUE)
    {
        printf("Path not found: [%s]\n", sDir);
        return false;
    }
	printf("\n");    
	
    do{
    	int status=0;
		sprintf(sPath, "%s\\%s", sDir, fdFile.cFileName);//
		printf("--Leido:%s\n", sPath);
    	if(strcmp(fdFile.cFileName, ".") == 0 || strcmp(fdFile.cFileName, "..") == 0){
    		
		}
		else {
			if(fdFile.dwFileAttributes &FILE_ATTRIBUTE_ARCHIVE){
				vacio = false;
			}
			if(fdFile.dwFileAttributes &FILE_ATTRIBUTE_READONLY){
				vacio = false;
			}
			if(fdFile.dwFileAttributes &FILE_ATTRIBUTE_HIDDEN){
				vacio = false;
			}			
			if(fdFile.dwFileAttributes &FILE_ATTRIBUTE_DIRECTORY){
				printf("\n----Lee un folder\n");
				folderinterno_vacio = RevisarVacio(sPath);
				if(folderinterno_vacio) {
					//printf("Folder %s se borro\n", sPath);
					//no afecta el estado actual del vacio
				}
				else {
					 vacio = false;
				}
			}
		}
	}
	while(FindNextFile(hFind, &fdFile));
	printf("\nSe decidira borrado de folder %s\n",sDir);
	if (vacio) {
		DeleteDirectory(sDir, false);
		printf("Folder %s borrado\n", sDir);
	}
	else {
		printf("Folder %s no borrado\n", sDir);
	}
	return vacio;
}
 
int main()
{
	//
	WIN32_FIND_DATA fdFile;
    HANDLE hFind = NULL;
	char sPath[2048];
	char c;
	bool folder_vacio = true;
	//
	DWORD nBufferLength = MAX_PATH;
	char szCurrentDirectory[MAX_PATH + 1];
	szCurrentDirectory[MAX_PATH +1 ] = '\0';
	printf("Este programa borra los folders vacios que se encuentren dentro de este directorio\n");
	printf("Desea continuar?(S/N):");
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
					printf("\n----Lee un folder\n");
					//ListDirectoryContents(sPath);
					folder_vacio = RevisarVacio(sPath, false);
					if (folder_vacio) {
						DeleteDirectory(sPath, false);
					}
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
