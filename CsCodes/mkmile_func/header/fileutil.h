#ifndef FILEUTIL_H
#define FILEUTIL_H

#include "filetype.h"
#pragma warning(disable:4996)

int selectFileType(const FileType** outType);
int createFile(const char* basename, const FileType* type);

#endif

