#include <stdio.h>
#include <string.h>
#pragma warning(disable:4996)


int main(void)
{
	char filename[256];
	char text[256];
	FILE* fp;			//�t�@�C�����������߂̃|�C���^
						//�uFILE�^�i�t�@�C�������Ǘ�����\���́j�ւ̃|�C���^�v
						//��錾���Ă���

	printf("�쐬����t�@�C��������͂��Ă��������i��Fdata.txt�j:\n> ");
	if (fgets(filename, sizeof(filename), stdin) == NULL)
	{
		printf("���̓G���[\n");
		return 1;
	}

	//���s�폜
	filename[strcspn(filename, "\n")] = '\0';

	// �g���q�`�F�b�N
	const char* ext = strrchr(filename, '.');

	//�t�@�C�����J���i�������ݏ�ԁj
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
		//fopen�����s�����Ƃ��iNULL��Ԃ����Ƃ�)
		//�G���[���e���A���b�Z�[�W�t���ŕ\������֐�
		printf("�t�@�C���̍쐬�Ɏ��s���܂����B\n");
		perror("fopen");
		return 1;
	}

	printf("�t�@�C���̍쐬�ɐ������܂����B\n\n");

	if (ext != NULL && strcmp(ext, ".txt") == 0)
	{
		printf("�t�@�C���ɏ������݂������������͂��Ă��������iEnter�Ŋm��):\n> ");
		if (fgets(text, sizeof(text), stdin) != NULL)
		{
			fputs(text, fp);
			printf("�t�@�C�� '%s' �ɏ������݂܂����B\n", filename);
		}
		else {
			printf("���̓G���[���������܂���");
		}
	} 
	else
	{
		printf("'.txt' �ȊO�̃t�@�C���Ȃ̂ŁA�������݂͍s���܂���B\n");
	}
	
	fclose(fp);
	return 0;
}