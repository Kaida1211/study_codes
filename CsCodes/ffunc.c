#include <stdio.h>
#include <string.h>
#pragma warning(disable:4996)


int main(void)
{
	char filename[256];
	char text[256];
	FILE* fp;			//ファイルを扱うためのポインタ
						//「FILE型（ファイル情報を管理する構造体）へのポインタ」
						//を宣言している

	printf("作成するファイル名を入力してください（例：data.txt）:\n> ");
	if (fgets(filename, sizeof(filename), stdin) == NULL)
	{
		printf("入力エラー\n");
		return 1;
	}

	//改行削除
	filename[strcspn(filename, "\n")] = '\0';

	// 拡張子チェック
	const char* ext = strrchr(filename, '.');

	//ファイルを開く（書き込み状態）
	if (ext != NULL && strcmp(ext, ".txt") == 0)
	{
		fp = fopen(filename, "w");
	}
	else
	{
		fp = fopen(filename, "wb");
	}
	
	if (fp == NULL)
	{
		//fopenが失敗したとき（NULLを返したとき)
		//エラー内容を、メッセージ付きで表示する関数
		printf("ファイルの作成に失敗しました。\n");
		perror("fopen");
		return 1;
	}

	printf("ファイルの作成に成功しました。\n\n");

	if (ext != NULL && strcmp(ext, ".txt") == 0)
	{
		printf("ファイルに書き込みたい文字列を入力してください（Enterで確定):\n> ");
		if (fgets(text, sizeof(text), stdin) != NULL)
		{
			fputs(text, fp);
			printf("ファイル '%s' に書き込みました。\n", filename);
		}
		else {
			printf("入力エラーが発生しました");
		}
	} 
	else
	{
		printf("'.txt' 以外のファイルなので、書き込みは行いません。\n");
	}
	
	fclose(fp);
	return 0;
}