#include <stdio.h>
#include "filetype.h"

static FileType fileTypes[] = {
    { ".txt",  "テキストファイル",             "w"  },
    { ".csv",  "CSV ファイル",                 "w"  },
    { ".bin",  "バイナリファイル",             "wb" },
    { ".dat",  "データファイル",               "wb" },
    { ".docx", "Word 文書（空だと開けない）",   "wb" },
    { ".xlsx", "Excel 文書（空だと開けない）",  "wb" },
};

const FileType* getFileTypes(int* count)
{
    if (count) {
        *count = sizeof(fileTypes) / sizeof(fileTypes[0]);
    }
    return fileTypes;
}

void printFileTypes(void)
{
    int count;
    const FileType* list = getFileTypes(&count);

    printf("選択可能な拡張子:\n");
    for (int i = 0; i < count; i++) {
        printf("%d: %s (%s)\n", i + 1, list[i].ext, list[i].desc);
    }
}
