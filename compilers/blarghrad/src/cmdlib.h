/*
===========================================================================
Copyright (C) 1997-2006 Id Software, Inc.

This file is part of Quake 2 Tools source code.

Quake 2 Tools source code is free software; you can redistribute it
and/or modify it under the terms of the GNU General Public License as
published by the Free Software Foundation; either version 2 of the License,
or (at your option) any later version.

Quake 2 Tools source code is distributed in the hope that it will be
useful, but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with Quake 2 Tools source code; if not, write to the Free Software
Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
===========================================================================
*/

// cmdlib.h

#ifndef __CMDLIB__
#define __CMDLIB__

#ifdef _WIN32
#pragma warning(disable : 4244)     // MIPS
#pragma warning(disable : 4136)     // X86
#pragma warning(disable : 4051)     // ALPHA
#pragma warning(disable : 4996)

//#pragma warning(disable : 4018)     // signed/unsigned mismatch
//#pragma warning(disable : 4305)     // truncate from double to float
#endif

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <ctype.h>
#include <time.h>
#include <stdarg.h>

#if defined(_WIN32)
#define PATHSEPARATOR   '\\'
#else
#define PATHSEPARATOR   '/'
#endif

// the dec offsetof macro doesnt work very well...
#define myoffsetof(type,identifier) ((size_t)&((type *)0)->identifier)


// set these before calling CheckParm
extern int myargc;
extern char **myargv;

//char *strupr(char *in);
//char *strlower(char *in);
int Q_strncasecmp(const char *s1, const char *s2, size_t n);
int Q_strcasecmp(const char *s1, const char *s2);
void Q_getwd(char *out);

int Q_filelength(FILE *f);
int	FileTime(char *path);

void	Q_mkdir(const char *path);

extern	char		qdir[1024];
extern	char		gamedir[1024];
void SetQdirFromPath_ORIGINAL(char *path);
char *ExpandArg(char *path);	// from cmd line
char *ExpandPath(char *path);	// from scripts
char *ExpandPathAndArchive(char *path);


double I_FloatTime(void);

void	Error(const char *error, ...);
int		CheckParm(char *check);

FILE	*SafeOpenWrite(const char *filename);
FILE	*SafeOpenRead(const char *filename);
void	SafeRead(FILE *f, void *buffer, int count);
void	SafeWrite(FILE *f, void *buffer, int count);

int		LoadFile(const char *filename, byte **bufferptr);
int		TryLoadFile(const char *filename, void **bufferptr);
void	SaveFile(const char *filename, void *buffer, int count);
qboolean	FileExists(const char *filename);

void 	DefaultExtension(char *path, const char *extension);
void 	StripExtension(char *path);

void 	ExtractFilePath(char *path, char *dest);
void 	ExtractFileBase(char *path, char *dest);
void	ExtractFileExtension(const char *path, char *dest);

int 	ParseNum(char *str);

short	BigShort(short l);
short	LittleShort(short l);
int		BigLong(int l);
int		LittleLong(int l);
float	BigFloat(float l);
float	LittleFloat(float l);


char *COM_Parse(char *data);

extern	char		com_token[1024];
extern	qboolean	com_eof;

char *copystring(const char *s);


void CRC_Init(unsigned short *crcvalue);
void CRC_ProcessByte(unsigned short *crcvalue, byte data);
unsigned short CRC_Value(unsigned short crcvalue);

void	CreatePath(const char *path);
void	QCopyFile(const char *from, const char *to);

extern	qboolean		archive;
extern	char			archivedir[1024];


extern	qboolean verbose;
void qprintf(const char *format, ...);

void ExpandWildcards(int *argc, char ***argv);


// for compression routines
typedef struct
{
    byte	*data;
    int		count;
} cblock_t;


#endif
