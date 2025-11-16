#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "fileutil.h"

int selectFileType(const FileType** outType)
{
    int count;
    const FileType* list = getFileTypes(&count);

    printFileTypes();

    int choice;
    printf("\n> 番号を入力してください: ");
    scanf("%d", &choice);
    getchar(); // 改行吸収

    if (choice < 1 || choice > count) {
        printf("無効な番号です。\n");
        return 0;
    }

    *outType = &list[choice - 1];
    return 1;
}

int createFile(const char* basename, const FileType* type)
{
    char filename[300];
    snprintf(filename, sizeof(filename), "%s%s", basename, type->ext);

    FILE* fp = fopen(filename, type->mode);
    if (!fp) {
        perror("ファイル作成に失敗しました");
        return 0;
    }

    printf("ファイル '%s' を作成しました。\n", filename);
    fclose(fp);

    return 1;
}
