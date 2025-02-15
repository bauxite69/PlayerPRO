/*
 *  MADPrivate.h
 *  PlayerPROCore
 *
 *  Created by C.W. Betts on 6/30/09.
 *  Copyright 2009 __MyCompanyName__. All rights reserved.
 *
 */

/*!
 *	@header		MADPrivate.h
 *	@abstract	Private functions used internally by PlayerPROCore.
 */

#ifndef __PLAYERPROCORE_PPPRIVATE__
#define __PLAYERPROCORE_PPPRIVATE__

#include "RDriver.h"
#include "MADDriver.h"

#ifndef BUILDINGPPRO
#warning this header should NOT be used outside of PlayerPROCore! The functions and datatypes here may change FOR ANY REASON, including differing compiler includes and defines.
#endif

#if !(defined(__has_feature) && defined(__has_attribute))
#define __attribute__(...)
#endif

#ifndef __private_extern
#define __private_extern __attribute__((visibility("hidden")))
#endif

#ifdef __cplusplus
extern "C" {
#endif
	
/********************						***********************/
/*** 					INTERNAL ROUTINES						***/
/***			    DO NOT use these routines					***/
/********************						***********************/

/*** Deluxe ***/

void	MADCreateOverShoot(MADDriverRec *intDriver);
void	MADKillOverShoot(MADDriverRec *intDriver);
void 	Sampler16AddDeluxe(MADChannel *curVoice, int *ASCBuffer, MADDriverRec *intDriver);
void 	Sampler16Addin16Deluxe(MADChannel *curVoice, int *ASCBuffer, MADDriverRec *intDriver);
void 	Sample16BufferAddDeluxe(MADChannel *curVoice, int *ASCBuffer, MADDriverRec *intDriver);
void 	Play16StereoDeluxe(MADDriverRec *intDriver);
void 	Sampler8in8AddDeluxe(MADChannel *curVoice, short *ASCBuffer, MADDriverRec *intDriver);
void 	Sampler8in16AddDeluxe(MADChannel *curVoice, short *ASCBuffer, MADDriverRec *intDriver);
void 	Sample8BufferAddDeluxe(MADChannel *curVoice, short *ASCBuffer, MADDriverRec *intDriver);
void 	Play8StereoDeluxe(MADDriverRec *intDriver);

/*** Delay ***/

void	Sampler16AddDelay(MADChannel *curVoice, int *ASCBuffer, MADDriverRec *intDriver);
void	Sampler16Addin16Delay(MADChannel *curVoice, int *ASCBuffer, MADDriverRec *intDriver);
void	Sample16BufferAddDelay(MADChannel *curVoice, int *ASCBuffer, MADDriverRec *intDriver);
void	Play16StereoDelay(MADDriverRec *intDriver);
void	Sampler8in8AddDelay(MADChannel *curVoice, short *ASCBuffer, MADDriverRec *intDriver);
void	Sampler8in16AddDelay(MADChannel *curVoice, short *ASCBuffer, MADDriverRec *intDriver);
void	Sample8BufferAddDelay(MADChannel *curVoice, short *ASCBuffer, MADDriverRec *intDriver);
void	Play8StereoDelay(MADDriverRec *intDriver);

/*** 8 Bits ***/

void 	Sampler8in8Add(MADChannel *curVoice, char *ASCBuffer, MADDriverRec *intDriver);
void 	Sampler8in16Add(MADChannel *curVoice, char *ASCBuffer, MADDriverRec *intDriver);
void 	Sampler8in8AddPoly(MADChannel *curVoice, char *ASCBuffer, short chanNo, MADDriverRec *intDriver);
void 	Sampler8in16AddPoly(MADChannel *curVoice, char *ASCBuffer, short chanNo, MADDriverRec *intDriver);
void 	Sample8BufferAdd(MADChannel *curVoice, char *ASCBuffer, MADDriverRec *intDriver);
void 	Sample8BufferAddPoly(MADChannel *curVoice, char *ASCBuffer, short chanNo, MADDriverRec *intDriver);
void	Play8Mono(MADDriverRec *intDriver);
void 	Play8Stereo(MADDriverRec *intDriver);
void 	Play8PolyPhonic(MADDriverRec *intDriver);

/*** 16 Bits ***/

void 	Sampler16Add(MADChannel *curVoice, short *ASCBuffer, MADDriverRec* intDriver);
void 	Sampler16Addin16(MADChannel *curVoice, short *ASCBuffer, MADDriverRec* intDriver);
void 	Sample16BufferAdd(MADChannel *curVoice, short *ASCBuffer, MADDriverRec* intDriver);
void 	Play16Stereo(MADDriverRec* intDriver);
void 	Sampler16AddPoly(MADChannel *curVoice, short *ASCBuffer, short chanNo, MADDriverRec* intDriver);
void 	Sampler16Addin16Poly(MADChannel *curVoice, short *ASCBuffer, short chanNo, MADDriverRec* intDriver);
void 	Sample16BufferAddPoly(MADChannel *curVoice, short *ASCBuffer, short chanNo, MADDriverRec* intDriver);
void 	Play16PolyPhonic(MADDriverRec* intDriver);
void 	Play16Mono(MADDriverRec* intDriver);

/*** 20 Bits ***/
//TODO

/*** 24 Bits ***/
//TODO

/*** Effects ***/

void 	DoEffect(MADChannel *ch, short call, MADDriverRec *intDriver);
void 	SetUpEffect(MADChannel *ch, MADDriverRec *intDriver);
void	DoVolCmd(MADChannel *ch, short call, MADDriverRec *intDriver);

/*** Interruption Functions ***/

void 	NoteAnalyse(MADDriverRec *intDriver);
void 	ReadNote(MADChannel *curVoice, Cmd *theCommand, MADDriverRec *intDriver);
void 	Filter8Bit(MADByte *myPtr, MADDriverRec *intDriver);
void 	Filter8BitX(MADByte *myPtr, MADDriverRec *intDriver);
void 	Filter16BitX(short *myPtr, MADDriverRec *intDriver);
void 	BufferCopyM(MADDriverRec *intDriver);
void 	BufferCopyS(MADDriverRec *intDriver);

/*** MIDI functions ***/

void 	NoteOff(short oldIns, short oldN, short oldV, MADDriverRec *intDriver);
void 	SampleMIDI(MADChannel *curVoice, short channel, short curN, MADDriverRec *intDriver);
void 	CleanDriver(MADDriverRec *intDriver);
void	SendMIDITimingClock(MADDriverRec *intDriver);

#ifdef _MAC_H
#pragma mark Core Audio Functions
__private_extern MADErr	initCoreAudio(MADDriverRec *inMADDriver);
__private_extern MADErr	closeCoreAudio(MADDriverRec *inMADDriver);
#endif

#ifdef _ESOUND
__private_extern MADErr initESD(MADDriverRec *inMADDriver);
__private_extern MADErr closeESD(MADDriverRec *inMADDriver);
#endif

#ifdef WIN32
__private_extern bool   DirectSoundInit(MADDriverRec * driver);
__private_extern void	DirectSoundClose(MADDriverRec* driver);
__private_extern bool	W95_Init(MADDriverRec* driver);
__private_extern void	W95_Exit(MADDriverRec* driver);
#endif

#ifdef HAVE_PULSEAUDIO
__private_extern MADErr initPulseAudio(MADDriverRec *inMADDriver);
__private_extern MADErr closePulseAudio(MADDriverRec *inMADDriver);
#endif

#ifdef HAVE_PORTAUDIO
__private_extern MADErr initPortAudio(MADDriverRec *inMADDriver);
__private_extern MADErr closePortAudio(MADDriverRec *inMADDriver);
#endif

__private_extern
MADErr	CallImportPlug(MADLibrary	*inMADDriver,
					   int			PlugNo,			// CODE du plug
					   MADFourChar	order,
					   char			*AlienFile,
					   MADMusic		*theNewMAD,
					   MADInfoRec	*info);


MADErr	PPTestFile(MADLibrary *inMADDriver, char *kindFile, char *AlienFile);
MADErr	PPInfoFile(MADLibrary *inMADDriver, char *kindFile, char *AlienFile, MADInfoRec	*InfoRec);
MADErr	PPExportFile(MADLibrary *inMADDriver, char *kindFile, char *AlienFile, MADMusic *theNewMAD);
MADErr	PPImportFile(MADLibrary *inMADDriver, char *kindFile, char *AlienFile, MADMusic **theNewMAD);
MADErr	PPIdentifyFile(MADLibrary *inMADDriver, char *kindFile, char *AlienFile);

MADFourChar	GetPPPlugType(MADLibrary *inMADDriver, short ID, MADFourChar type);
__private_extern void	MInitImportPlug(MADLibrary *inMADDriver, const char*);
__private_extern void	CloseImportPlug(MADLibrary *inMADDriver);
__private_extern MADErr	CheckMADFile(const char *AlienFile);
void ConvertInstrumentIn(MADByte *tempPtr, size_t sSize);
void ConvertInstrumentIn16(short *tempPtr, size_t sSize);

__private_extern uint32_t getfrequency(uint32_t period);

#if defined _MAC_H && TARGET_OS_OSX
__private_extern void SetOSType(CFURLRef, OSType);
#endif

__private_extern sData* inMADCreateSample(void);

#ifdef __cplusplus
}
#endif

#endif
