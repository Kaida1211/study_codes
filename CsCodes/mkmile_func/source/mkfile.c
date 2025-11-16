#include <stdio.h>
#include <string.h>
#include "filetype.h"
#include "fileutil.h"

int main(void)
{
    const FileType* selected;

    // 拡張子選択
    if (!selectFileType(&selected)) {
        return 1;
    }

    // ファイル名入力
    char basename[256];
    printf("ファイル名（拡張子なし）を入力:\n> ");
    fgets(basename, sizeof(basename), stdin);
    basename[strcspn(basename, "\n")] = '\0';

    // ファイル作成
    createFile(basename, selected);

    return 0;
}
