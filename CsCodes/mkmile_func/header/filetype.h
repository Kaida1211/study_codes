#ifndef FILETYPE_H
#define FILETYPE_H

typedef struct {
    const char* ext;   // ".txt" など
    const char* desc;  // 説明
    const char* mode;  // fopen モード
} FileType;

const FileType* getFileTypes(int* count);  // 一覧取得
void printFileTypes(void);                 // 一覧表示

#endif
