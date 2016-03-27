#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <string.h>

typedef struct Hash
{
	char bin[100];
	int last;
}Hash;

struct Hex
{
	int number;
};

typedef struct HuffmanTree
{
	struct Hex hex;
	int data;
	struct HuffmanTree* lChild;
	struct HuffmanTree*  rChild;
}HuTr;

//����ǩ��
int sortForest(HuTr** array, int n);
HuTr* createTree(HuTr* array,int n);
void freeTree(HuTr* pRoot);
void traverseToHash(HuTr* pRoot, Hash* hash,int n,int type);
int compress(FILE* fpIn,FILE* fpOut, Hash* hash,int n);
int uncompress(FILE* fpIn,FILE* fpOut, Hash* hash,int n);
long binToDec(char* bin,int n);
void DecToBin(long dec, char* bin, int n);
int fileSize(FILE* fp);

//������
int main()
{
	Hash hash[256];
	HuTr array[256];
	HuTr* pRoot = NULL;
	FILE* fpIn = NULL;
	FILE* fpOut = NULL;
	time_t start,end;
	char fileName[100];
	int flag = 1;
	int sizeOG,sizeNew;
	int hex;
	int i;
	int n;
	while (flag)
	{
		system("cls");
		printf("++++++++++++++++++++++++++++\n");
		printf("+       �ļ�ѹ������       +\n");
		printf("++++++++++++++++++++++++++++\n");
		printf("��ѡ�������\n");
		printf("0-ѹ�� 1-��ѹ 2-�˳�����\n");
		scanf("%d", &n);
		getchar();
		switch (n)
		{
		case 0:
			do {
				printf("������ѹ�����ļ�����");
				fgets(fileName, 100, stdin);
				i = 0;
				if (fileName[i] != '\n')
					while (fileName[i++] != '\n');
				fileName[--i] = '\0';
				fpIn = fopen(fileName, "rb");
			} while (fpIn == NULL);
			do {
				printf("������洢���ļ�����");
				fgets(fileName, 100, stdin);
				i = 0;
				if (fileName[i] != '\n')
					while (fileName[i++] != '\n');
				fileName[--i] = '\0';
				fpOut = fopen(fileName, "wb");
			} while (fpOut == NULL);
			printf("ѹ���У����Ժ�...\n");
			time(&start);
			printf("һ����׼������.");
			//��ʼ���ַ�ȨֵΪ0
			for (i = 0;i < 256;i++)
			{
				array[i].hex.number = i;
				array[i].data = 0;
			}
			printf("\rһ����׼������..");
			//��ʼ����ϣ��
			for (i = 0;i < 256;i++)
			{
				hash[i].bin[0] = '\0';
				hash[i].last = 0;
			}
			printf("\rһ����׼������...");
			while ((hex = fgetc(fpIn)) != EOF)
			{
				hex &= 0xFF;
				array[hex].data++;
			}
			printf("\rһ����׼������....");
			pRoot = createTree(array, 256);
			printf("\rһ����׼������.....");
			traverseToHash(pRoot, hash, 256, 1);
			printf("\rһ����׼������......\n");
			if(compress(fpIn, fpOut, hash, 256) != 0)
				printf("ѹ��ʧ�ܣ�\n");
			else
			{
				time(&end);
				printf("ѹ����ʱ%.0lfsec\n",difftime(end,start));
				sizeOG = fileSize(fpIn);
				sizeNew = fileSize(fpOut);
				printf("ѹ����%.2f%%\n",(double)sizeNew / sizeOG * 100);
				printf("ѹ����ɣ�\n");
			}
			freeTree(pRoot);
			fclose(fpIn);
			fclose(fpOut);
			break;
		case 1:
			do {
				printf("�������ѹ�����ļ�����");
				fgets(fileName, 100, stdin);
				i = 0;
				if (fileName[i] != '\n')
					while (fileName[i++] != '\n');
				fileName[--i] = '\0';
				fpIn = fopen(fileName, "rb");
			} while (fpIn == NULL);
			do {
				printf("������洢���ļ�����");
				fgets(fileName, 100, stdin);
				i = 0;
				if (fileName[i] != '\n')
					while (fileName[i++] != '\n');
				fileName[--i] = '\0';
				fpOut = fopen(fileName, "wb");
			} while (fpOut == NULL);
			printf("��ѹ�У����Ժ�...\n");
			time(&start);
			if(uncompress(fpIn, fpOut, hash, 256) != 0)
				printf("��ѹʧ�ܣ�\n");
			else
			{
				time(&end);
				printf("��ѹ��ʱ%.0lfsec\n",difftime(end,start));
				sizeOG = fileSize(fpIn);
				sizeNew = fileSize(fpOut);
				printf("��ѹ��%.2f%%\n",(double)sizeNew / sizeOG * 100);
				printf("��ѹ��ɣ�\n");
			}
			fclose(fpIn);
			fclose(fpOut);
			break;
		case 2:
			flag = 0;
			break;
		default:printf("�������\n");break;
		}
		system("pause");
	}
	return 0;
}

int sortForest(HuTr** array, int n)
{
	HuTr* temp = NULL;
	for (int i = 0;i < n - 1;i++)
	{
		for (int j = 0;j < n - 1 - i;j++)
			if (array[j]->data < array[j + 1]->data)
			{
				temp = array[j];
				array[j] = array[j + 1];
				array[j + 1] = temp;
			}
	}
	int k;
	for (k = n;k > 0;k--)
		if (array[k - 1]->data != 0)
			break;
	return k;
}

//������������
HuTr* createTree(HuTr* array,int n)
{
	HuTr* pRoot = NULL;
	HuTr* pLeaf = NULL;
	HuTr** pTree = (HuTr**)malloc(sizeof(HuTr*) * n);
	for(int i = 0;i < n;i++)
	{
		pLeaf = (HuTr*)malloc(sizeof(HuTr));
		*pLeaf = array[i];
		pLeaf->lChild = pLeaf->rChild = NULL;
		pTree[i] = pLeaf;
	}
	n = sortForest(pTree, n);
	int j = 0;
	while(n > 1)
	{
		if(pTree[j]->data <= pTree[n - 1]->data + pTree[n - 2]->data)
		{
			pRoot = (HuTr*)malloc(sizeof(HuTr));
			pRoot->hex.number = -1;
			pRoot->data =  pTree[n - 1]->data + pTree[n - 2]->data;
			pRoot->lChild = pTree[n - 2];
			pRoot->rChild = pTree[n - 1];
			pTree[n - 2] = pRoot;
			n--;
			j = -1;
			sortForest(pTree, n);
		}
		j++;
	}

	free(pTree);
	pTree = NULL;
	return pRoot;
}

//�ݹ������������,�����ϣ��
void traverseToHash(HuTr* pRoot, Hash* hash,int n,int type)
{
	static int i = 0;
	static char result[100] = {'\0'};
	if(type == 1)
	{
		i=0;
		for(int j = 0;j < 100;j++)
			result[j] = '\0';
	}
	if (pRoot->lChild == NULL && pRoot->rChild == NULL)
	{
		result[i--] = '\0';
		strcpy(hash[pRoot->hex.number].bin,result);
		hash[pRoot->hex.number].last = i + 1;
	}
	else
	{
		result[i++] = '0';
		traverseToHash(pRoot->lChild, hash, n, 0);
		result[i++] = '1';
		traverseToHash(pRoot->rChild, hash, n, 0);
		i--;
	}
}

void freeTree(HuTr* pRoot)
{
	if(pRoot != NULL)
	{
		freeTree(pRoot->lChild);
		freeTree(pRoot->rChild);
		free(pRoot);
		pRoot = NULL;
	}
}

int compress(FILE* fpIn, FILE* fpOut, Hash* hash,int n)
{
	int begin = 0;
	char bit[9];
	bit[8] = '\0';
	int hex;
	int code;
	int sum = 0;
	long codeSum = 0;
	rewind(fpIn);rewind(fpOut);
	fputc('C',fpOut);fputc('F',fpOut);
	//Ԥ��λ��
	if (fwrite(&codeSum, sizeof(long), 1, fpOut) != 1)
		return -1;
	for (int l = 0;l < n;l++)
		if (hash[l].last > 0)
			sum++;
	//��ϣ����
	if (fwrite(&sum, sizeof(int), 1, fpOut) != 1)
		return -1;
	for (int k = 0;k < n;k++)
	{
		code = hash[k].last;
		if (code > 0)
		{
			if (fwrite(&k, sizeof(char), 1, fpOut) != 1)
				return -1;
			if (fwrite(&code, sizeof(int), 1, fpOut) != 1)
				return -1;
			code = binToDec(hash[k].bin, code);
			if (fwrite(&code, sizeof(int), 1, fpOut) != 1)
				return -1;
		}
	}
	while ((hex = fgetc(fpIn)) != EOF)
	{
		codeSum++;
		for (int i = 0;hash[hex].bin[i] != '\0';i++, begin++)
		{
			bit[begin] = hash[hex].bin[i];
			if (begin == 7)
			{
				begin = -1;
				fputc(binToDec(bit, 8), fpOut);
			}
		}
	}
	if (begin != 0)
	{
		for (int j = begin;j < 8;j++)
			bit[j] = '0';
		fputc(binToDec(bit, 8), fpOut);
	}
	rewind(fpOut);
	fseek(fpOut,2L,SEEK_SET);
	if (fwrite(&codeSum,sizeof(long),1,fpOut) != 1)
		return -1;
	return 0;
}

int uncompress(FILE* fpIn, FILE* fpOut, Hash* hash, int n)
{
	int begin = 0;
	char temp[9];
	temp[8] = '\0';
	char bit[100];
	int hex;
	long codeSum = 0;
	long time = 0;
	int sum = 0;
	int num = 0;
	int code;
	rewind(fpIn);rewind(fpOut);
	if (fgetc(fpIn) != 'C' || fgetc(fpIn) != 'F')
		return -1;
	if (fread(&codeSum, sizeof(long), 1, fpIn) != 1)
		return -1;
	//�����ϣ��
	if (fread(&sum, sizeof(int), 1, fpIn) != 1)
		return -1;
	for (int k = 0;k < sum;k++)
	{
		if (fread(&num, sizeof(char), 1, fpIn) != 1)
			return -1;
		if (fread(&hash[num].last, sizeof(int), 1, fpIn) != 1)
			return -1;
		if (fread(&code, sizeof(int), 1, fpIn) != 1)
			return -1;
		DecToBin(code, hash[num].bin, hash[num].last);
		hash[num].bin[hash[num].last] = '\0';
	}
	do
	{
		hex = fgetc(fpIn);
		DecToBin(hex,temp,8);
		for (int j = 0;temp[j] != '\0';j++)
		{
			bit[begin++] = temp[j];
			bit[begin] = '\0';
			for (int i = 0;i < n;i++)
				if (strcmp(hash[i].bin, bit) == 0)
				{
					time++;
					printf("\r%.2lf%%",(double)time / codeSum * 100);
					fputc(i, fpOut);
					bit[0] = '\0';
					begin = 0;
					if (time == codeSum)
						temp[j + 1] = '\0';
					break;
				}
		}
	} while (feof(fpIn) == 0);
	printf("\n");
	return 0;
}

long binToDec(char* bin,int n)
{
	long dec = 0;
	for(int i = 0;i < n;i++)
	{
		dec += ((long)pow(2, i) * (bin[i] - '0'));
	}
	return dec;
}

void DecToBin(long dec,char* bin, int n)
{
	for (int i = 0;i < n;i++)
	{
		bin[i] = dec % 2 + '0';
		dec /= 2;
	}
}

int fileSize(FILE* fp)
{
	int size=0;
	fseek(fp,0,SEEK_END);
	size=ftell(fp);
	rewind(fp);
	return size;
}