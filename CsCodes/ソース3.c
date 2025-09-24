#include <stdio.h>
#include <stdlib.h>

//ファイルを開く関数
FILE* FILE_OPEN(const char *filename) {
    FILE* fp = fopen(filename, "r");  // 読み込みモード
    if (fp == NULL) {
        printf("ファイルを開けませんでした\n");
        return NULL;   // 失敗時はNULLを返す
    }
    return fp;         // 成功したらファイルポインタを返す
}

//メモリチェック関数
int MEMORIE_CHECK(const char* buffer,FILE *fp)
{
    if (!buffer) {
        printf("メモリ確保失敗\n");
        fclose(fp);
    }
    return 0;
}

int main(void) {
    
    FILE* fp = FILE_OPEN("num.txt");   // ファイルを開く
    if (fp == NULL) return 1;  // 開けなければ終了

    int bufsize = 100;
    char* buffer = malloc(bufsize);
    if (MEMORIE_CHECK(buffer, fp)) return 1;
    

    long long sum = 0;   // 合計を保持する変数

    while (fgets(buffer, bufsize, fp) != NULL) {
        long long val = atoll(buffer);  // 行を数値に変換

        sum += val;
    }

    printf("合計: %lld\n", sum);  //%lld:long long 

    fclose(fp);
    free(buffer);
    return 0;
}
