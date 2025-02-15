/********************						***********************/
//
//	Player PRO 5.0 - DRIVER SOURCE CODE -
//
//	Library Version 5.0
//
//	To use with MAD Library for Mac: Symantec, CodeWarrior and MPW
//
//	Antoine ROSSET
//	16 Tranchees
//	1206 GENEVA
//	SWITZERLAND
//
//	COPYRIGHT ANTOINE ROSSET 1996, 1997, 1998
//
//	Thank you for your interest in PlayerPRO !
//
//	FAX:				(+41 22) 346 11 97
//	PHONE: 			(+41 79) 203 74 62
//	Internet: 	RossetAntoine@bluewin.ch
//
/********************						***********************/

#include "RDriver.h"
#include "RDriverInt.h"
#include "MADFileUtils.h"
#include "MADPrivate.h"

//TODO: Move to unicode functions?

MADErr CheckMADFile(const char *name)
{
	UNFILE	refNum;
	char	charl[20];
	MADErr	err;
	
	refNum = iFileOpenRead(name);
	if (!refNum)
		return MADReadingErr;
	else {
		iRead(10, charl, refNum);
		
		if (charl[0] == 'M' &&
			charl[1] == 'A' &&
			charl[2] == 'D' &&
			charl[3] == 'K')
			err = MADNoErr;
		else
			err = MADFileNotSupportedByThisPlug;
		
		iClose(refNum);
	}
	
	return err;
}

MADErr TESTmain(MADFourChar order, char *AlienFileName, MADMusic *MadFile, MADInfoRec *info, MADDriverSettings *init);

MADErr CallImportPlug(MADLibrary*	inMADDriver,
					 int			PlugNo,				// CODE ID
					 MADFourChar	order,
					 char*			AlienFile,
					 MADMusic		*theNewMAD,
					 MADInfoRec		*info)
{
	MADDriverSettings driverSettings;
	
	memset(&driverSettings, 0, sizeof(MADDriverSettings));
	return (inMADDriver->ThePlug[PlugNo].IOPlug)(order, AlienFile, theNewMAD, info, &driverSettings);
}

MADErr PPTestFile(MADLibrary* inMADDriver, char *kindFile, char *AlienFile)
{
	int			i;
	MADMusic	aMAD;
	MADInfoRec	InfoRec;
	
	for (i = 0; i < inMADDriver->TotalPlug; i++) {
		if (!strcmp(kindFile, inMADDriver->ThePlug[i].type)) {
			return CallImportPlug(inMADDriver, i, MADPlugTest, AlienFile, &aMAD, &InfoRec);
		}
	}
	
	return MADCannotFindPlug;
}

MADErr PPMADInfoFile(char *AlienFile, MADInfoRec *InfoRec)
{
	MADSpec	*theMAD;
	long	fileSize;
	UNFILE	fileID;
	MADErr	MADCheck;
	
	if ((MADCheck = CheckMADFile(AlienFile)) != MADNoErr) {
		return MADCheck;
	}
	
	theMAD = (MADSpec*)malloc(sizeof(MADSpec) + 200);
	
	fileID = iFileOpenRead(AlienFile);
	if (!fileID) {
		free(theMAD);
		return MADReadingErr;
	}
	fileSize = iGetEOF(fileID);
	
	iRead(sizeof(MADSpec), theMAD, fileID);
	iClose(fileID);
	
	strcpy_s(InfoRec->internalFileName, sizeof(theMAD->name), theMAD->name);
	
	InfoRec->totalPatterns = theMAD->numPat;
	InfoRec->partitionLength = theMAD->numPointers;
	InfoRec->totalTracks = theMAD->numChn;
	InfoRec->signature = 'MADK';
	strcpy_s(InfoRec->formatDescription, sizeof(InfoRec->formatDescription), "MADK");
	InfoRec->totalInstruments = theMAD->numInstru;
	InfoRec->fileSize = fileSize;
	
	free(theMAD);
	theMAD = NULL;
	
	return MADNoErr;
}

MADErr PPInfoFile(MADLibrary* inMADDriver, char *kindFile, char *AlienFile, MADInfoRec *InfoRec)
{
	int			i;
	MADMusic	aMAD;
	
	if (!strcmp(kindFile, "MADK")) {
		return PPMADInfoFile(AlienFile, InfoRec);
	}
	
	for (i = 0; i < inMADDriver->TotalPlug; i++) {
		if (!strcmp(kindFile, inMADDriver->ThePlug[i].type)) {
			return CallImportPlug(inMADDriver, i, MADPlugInfo, AlienFile, &aMAD, InfoRec);
		}
	}
	
	return MADCannotFindPlug;
}

MADErr PPExportFile(MADLibrary* inMADDriver, char *kindFile, char *AlienFile, MADMusic *theNewMAD)
{
	int			i;
	MADInfoRec	InfoRec;
	
	for (i = 0; i < inMADDriver->TotalPlug; i++) {
		if (!strcmp(kindFile, inMADDriver->ThePlug[i].type)) {
			return CallImportPlug(inMADDriver, i, MADPlugExport, AlienFile, theNewMAD, &InfoRec);
		}
	}
	
	return MADCannotFindPlug;
}

MADErr PPImportFile(MADLibrary* inMADDriver, char *kindFile, char *AlienFile, MADMusic **theNewMAD)
{
	int			i;
	MADInfoRec	InfoRec;
	
	for (i = 0; i < inMADDriver->TotalPlug; i++) {
		if (!strcmp(kindFile, inMADDriver->ThePlug[i].type)) {
			MADErr theErr = MADNoErr;
			*theNewMAD = (MADMusic*)calloc(sizeof(MADMusic), 1);
			if (!*theNewMAD)
				return MADNeedMemory;
			
			theErr = CallImportPlug(inMADDriver, i, MADPlugImport, AlienFile, *theNewMAD, &InfoRec);
			if (theErr != MADNoErr) {
				free(*theNewMAD);
			}
			return theErr;
		}
	}
	
	return MADCannotFindPlug;
}

MADErr PPIdentifyFile(MADLibrary* inMADDriver, char *type, char *AlienFile)
{
	FILE*		refNum;
	int			i;
	MADInfoRec	InfoRec;
	MADErr		iErr;
	
	strcpy_s(type, 5, "!!!!");
	
	// Check if we have access to this file
	refNum = iFileOpenRead(AlienFile);
	if (!refNum)
		return MADReadingErr;
	iClose(refNum);
	
	// Is it a MAD file?
	iErr = CheckMADFile(AlienFile);
	if (iErr == MADNoErr) {
		strcpy_s(type, 5, "MADK");
		return MADNoErr;
	}
	
	for (i = 0; i < inMADDriver->TotalPlug; i++) {
		if (CallImportPlug(inMADDriver, i, MADPlugTest, AlienFile, NULL, &InfoRec) == MADNoErr) {
			strcpy_s(type, 5, inMADDriver->ThePlug[i].type);
			
			return MADNoErr;
		}
	}
	
	strcpy_s(type, 5, "!!!!");
	return MADCannotFindPlug;
}

bool MADPlugAvailable(const MADLibrary* inMADDriver, const char *kindFile)
{
	int i;
	
	if (!strcmp(kindFile, "MADK"))
		return true;
	
	for (i = 0; i < inMADDriver->TotalPlug; i++) {
		if (!strcmp(kindFile, inMADDriver->ThePlug[i].type))
			return true;
	}
	
	return false;
}

typedef MADErr (*PLUGFILLDLLFUNC)(PlugInfo*);

bool LoadPlugLib(char *name, PlugInfo* plug)
{
	PLUGFILLDLLFUNC	fpFuncAddress;
	MADErr			err;
	
	strcpy_s(plug->file, sizeof(plug->file), name);
	
	plug->hLibrary = LoadLibraryA(name);
	if (!plug->hLibrary)
		return false;
	
	plug->IOPlug = (PLUGDLLFUNC)GetProcAddress(plug->hLibrary, "PPImpExpMain");
	if (!plug->IOPlug) {
		FreeLibrary(plug->hLibrary);
		return false;
	}
	
	fpFuncAddress = (PLUGFILLDLLFUNC)GetProcAddress(plug->hLibrary, "FillPlug");
	if (!fpFuncAddress) {
		FreeLibrary(plug->hLibrary);
		return false;
	}
	
	err = (*fpFuncAddress)(plug);
	
	if(err != MADNoErr) {
		FreeLibrary(plug->hLibrary);
		return false;
	}
	
	return true;
}

#define MAXFOLDLEN (MAX_PATH * 2)

void MInitImportPlug(MADLibrary* inMADDriver, char *PlugsFolderName)
{
	HANDLE				hFind;
	WIN32_FIND_DATAA	fd;
	BOOL				bRet = TRUE;
	char				FindFolder[MAXFOLDLEN], inPlugsFolderName[MAXFOLDLEN];
	
	inMADDriver->TotalPlug = 0;
	
	if (PlugsFolderName) {
		strcpy_s(inPlugsFolderName, MAXFOLDLEN, PlugsFolderName);
		strcat_s(inPlugsFolderName, MAXFOLDLEN, "/");
		
		strcpy_s(FindFolder, MAXFOLDLEN, inPlugsFolderName);
	} else {
		strcpy_s(inPlugsFolderName, MAXFOLDLEN, "/");
		strcpy_s(FindFolder, MAXFOLDLEN, inPlugsFolderName);
	}
	strcat_s(FindFolder, MAXFOLDLEN, "*.PLG");
	
	hFind = FindFirstFileA(FindFolder, &fd);
	
	inMADDriver->ThePlug = (PlugInfo*) calloc(MAXPLUG, sizeof(PlugInfo));
	
	while(hFind != INVALID_HANDLE_VALUE && bRet) {
		if ((fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) == 0) {
			if (inMADDriver->TotalPlug < MAXPLUG) {
				char myCompleteFilename[MAXFOLDLEN];
				
				strcpy_s(myCompleteFilename, MAXFOLDLEN, inPlugsFolderName);
				strcat_s(myCompleteFilename, MAXFOLDLEN, fd.cFileName);
				
				if (LoadPlugLib(myCompleteFilename, &inMADDriver->ThePlug[inMADDriver->TotalPlug]))
					inMADDriver->TotalPlug++;
			}
		}
		
		bRet = FindNextFileA(hFind, &fd);
	}
	
	FindClose(hFind);
}

void CloseImportPlug(MADLibrary* inMADDriver)
{
	int i;
	
	for (i = 0; i < inMADDriver->TotalPlug; i++) {
		FreeLibrary(inMADDriver->ThePlug[i].hLibrary);
	}
	
	free(inMADDriver->ThePlug);
	inMADDriver->ThePlug = NULL;
}

MADFourChar GetPPPlugType(MADLibrary *inMADDriver, short ID, MADFourChar mode)
{
	int i, x;
	
	if (ID >= inMADDriver->TotalPlug) MADDebugStr(__LINE__, __FILE__, "PP-Plug ERROR. ");
	
	for (i = 0, x = 0; i < inMADDriver->TotalPlug; i++) {
		if (inMADDriver->ThePlug[i].mode == mode || inMADDriver->ThePlug[i].mode == MADPlugImportExport) {
			if (ID == x) {
				short		xx;
				MADFourChar	type = '    ';
				
				xx = strlen(inMADDriver->ThePlug[i].type);
				if (xx > 4)
					xx = 4;
				memcpy(&type, inMADDriver->ThePlug[i].type, xx);
				MADBE32(&type);
				
				return type;
			}
			x++;
		}
	}
	
	MADDebugStr(__LINE__, __FILE__, "PP-Plug ERROR II.");
	
	return MADNoErr;
}
