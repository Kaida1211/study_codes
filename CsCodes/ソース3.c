#include <stdio.h>
#include <stdlib.h>

//�t�@�C�����J���֐�
FILE* FILE_OPEN(const char *filename) {
    FILE* fp = fopen(filename, "r");  // �ǂݍ��݃��[�h
    if (fp == NULL) {
        printf("�t�@�C�����J���܂���ł���\n");
        return NULL;   // ���s����NULL��Ԃ�
    }
    return fp;         // ����������t�@�C���|�C���^��Ԃ�
}

//�������`�F�b�N�֐�
int MEMORIE_CHECK(const char* buffer,FILE *fp)
{
    if (!buffer) {
        printf("�������m�ێ��s\n");
        fclose(fp);
    }
    return 0;
}

int main(void) {
    
    FILE* fp = FILE_OPEN("num.txt");   // �t�@�C�����J��
    if (fp == NULL) return 1;  // �J���Ȃ���ΏI��

    int bufsize = 100;
    char* buffer = malloc(bufsize);
    if (MEMORIE_CHECK(buffer, fp)) return 1;
    

    long long sum = 0;   // ���v��ێ�����ϐ�

    while (fgets(buffer, bufsize, fp) != NULL) {
        long long val = atoll(buffer);  // �s�𐔒l�ɕϊ�

        sum += val;
    }

    printf("���v: %lld\n", sum);  //%lld:long long 

    fclose(fp);
    free(buffer);
    return 0;
}
