#pragma once
#define _CRT_SECURE_NO_WARNINGS 1
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <Windows.h>

#define MaxNum 15//��󾰵����
#define NameLength 40//�������Ƴ���
#define INF 99999

//���ȶ���ջ��ջ�Ľӿں���
typedef struct
{
	int SserialNumber[MaxNum];//��¼���·���ϵĶ������
	int top;
}Stack;

void InitStack(Stack* S)
{
	S->top = 0;
}

bool StackIsEmpty(Stack S)
{
	if (S.top == 0)
	{
		return true;
	}
	return false;
}

bool Push(Stack* S, int x)
{
	if (S->top == MaxNum)
	{
		return false;
	}
	S->SserialNumber[S->top] = x;
	S->top++;
	return true;
}

bool Pop(Stack* S, int* x)
{
	if (S->top == 0) {
		return false;
	}
	S->top--;
	*x = S->SserialNumber[S->top];
	return true;
}

//�˵�
void Menu(void)
{
	printf("\n");
	printf("��ѯ�������£�\n");
	printf("****************************************************\n");
	printf("*     1-�鿴�����ͼ                               *\n");
	printf("*     2-��ѯĳ�����㵽������������·��           *\n");
	printf("*     3-��ѯ�������������·��                   *\n");
	printf("*     0-�˳�ϵͳ                                   *\n");
	printf("****************************************************\n");
	printf("\n����������ѡ��\n");
}

//������Ҫ�õ����ݽṹ
//�ڽӾ���
typedef struct
{
	char vexs[MaxNum + 1][NameLength];//�������Ʊ�
	double arcs[MaxNum + 1][MaxNum + 1];//�ڽӾ���
	int vexnum , arcnum; 
}AMGraph;

//Dijkstra�㷨
typedef struct 
{
	int S[MaxNum + 1];               //����Ƿ���������·��
	double Path[MaxNum + 1];    //��¼��i�����·��
	int PrePath[MaxNum + 1];        //��¼ǰ������
}Dij;

//Floyd�㷨
typedef struct
{
	double Path[MaxNum + 1][MaxNum + 1];//��¼��ǰ���·��
	int PrePath[MaxNum + 1][MaxNum + 1];//��¼��ǰ���·����ǰ������
}Floyd;

//ʹ�õ��ĺ���
//��ӡ�������Ƽ������
void Print(AMGraph G) 
{
	for (int i = 1; i <= G.vexnum; i++) 
	{
		printf("%-3d- %-20s", i, G.vexs[i]);
		if (i % 4 == 0)   //4������һ�� 
		{
			printf("\n");
		}
	}
}

//�󾰵��Ӧ�����
int Locate(AMGraph G, char* name)
{
	for (int i = 1; i <= G.vexnum; i++) 
	{
		if (strcmp(G.vexs[i], name) == 0) 
		{
			return i;
		}
	}
}

//�����ڽӾ���
void CreateMap(AMGraph* G) 
{
	FILE* fp = fopen("sights.txt", "r");
	fscanf(fp, "%d", &G->vexnum);
	for (int i = 1; i <= G->vexnum; i++)
	{
		fscanf(fp, "%s", G->vexs[i]);
	}
	fclose(fp);

	//��ʼ������
	for (int i = 0; i <= G->vexnum; i++)
	{
		for (int j = 0; j <= G->vexnum; j++)
		{
			G->arcs[i][j] = INF;
		}
	}

	fp = fopen("PathDistance.txt", "r");
	int n = 0;//��¼����
	char name1[30], name2[30];
	double distance;
	while (fscanf(fp, "%s%s%lf", name1, name2, &distance) != EOF)
	{
		n++;
		int i = Locate(*G, name1);
		int j = Locate(*G, name2);
		G->arcs[i][j] = distance;
		G->arcs[j][i] = distance;
	}
	G->arcnum = n;//��ǰ����
}

//��ӡ�ڽӾ���
void PrintMap(AMGraph G)
{
	printf("\n���㼰���Ӧ���Ϊ��\n");
	Print(G);
	printf("\n�����ͼΪ��\n");
	printf("���  ");
	for (int i = 1; i <= G.vexnum; i++) {
		printf("%-4d", i);
	}
	printf("\n");
	for (int i = 1; i <= G.vexnum; i++) 
	{
		printf("%-5d", i);
		for (int j = 1; j <= G.vexnum; j++)
		{
			if (G.arcs[i][j] == INF) {
				printf("%-5c", '*');
				continue;
			}
			printf("%-5.1lf", G.arcs[i][j]);
		}
		printf("\n");
	}
	printf("*�ű�ʾ������û��ֱ��·��.\n");
}

//��Դ���·��
Dij D;
void Dijkstra(AMGraph G, int v) 
{
	//��ʼ��
	for (int i = 1; i <= G.vexnum; i++) 
	{
		D.S[i] = 0;
		D.Path[i] = G.arcs[v][i];
		if (D.Path[i] < INF)
			D.PrePath[i] = v;
		else
			D.PrePath[i] = -1;//�����ڱ�vi��i��ǰ��������Ϊ-1
	}
	D.S[v] = 1;
	D.Path[v] = 0;

	for (int i = 2; i <= G.vexnum; i++)
	{
		//�ҵ�Ȩֵ��̵ı߶�Ӧ�Ķ���w
		double min = INF;
		int w;
		for (int j = 1; j <= G.vexnum; j++) 
		{
			if (!D.S[j] && D.Path[j] < min) 
			{
				min = D.Path[j];
				w = j;
			}
		}
	    D.S[w] = 1;
		//�Ż�
		for (int j = 1; j <= G.vexnum; j++)
		{
			if (!D.S[j] && D.Path[w] + G.arcs[w][j] < D.Path[j])   
			{
				D.Path[j] = D.Path[w] + G.arcs[w][j];
				D.PrePath[j] = w;
			}
		}
	}
}

void Print_Dij(AMGraph G, int start) {
	printf("��%s��������������������·��Ϊ��\n", G.vexs[start]);
	for (int end = 1; end <= G.vexnum; end++)
	{
		if (end == start) 
		{
			continue;
		}
		Stack S;
		InitStack(&S);
		Push(&S, end);
		//��end�����·����ǰ���ڵ㣬������start���ջ
		int tem1, tem2;
		for (tem1 = D.PrePath[end]; tem1 != start; tem1 = D.PrePath[tem2]) 
		{
			Push(&S, tem1);
			tem2 = tem1;
		}
		printf("%s", G.vexs[start]);
		tem1 = start;//tem1��¼ǰ������
		double sum = 0;
		while (!StackIsEmpty(S))
		{
			Pop(&S, &tem2);
			printf("-%.1lfkm->%s", G.arcs[tem1][tem2], G.vexs[tem2]);
			sum += G.arcs[tem1][tem2];
			tem1 = tem2;
		}
		printf("\n��·��Ϊ%.1lfkm.\n\n", sum);
	}
}

//������������·��
Floyd F;
void floyd(AMGraph G) 
{
	//��ʼ��
	for (int i = 1; i <= G.vexnum; i++) 
	{
		for (int j = 1; j <= G.vexnum; j++) 
		{
			if (i == j) 
			{
				F.Path[i][j] = 0;
			}
			else 
			{
				F.Path[i][j] = G.arcs[i][j];
			}
			if (F.Path[i][j] < INF && i != j) 
			{
				F.PrePath[i][j] = i;
			}
			else
			{
				F.PrePath[i][j] = -1;
			}
		}
	}
	//k��ʾÿ�μӵĶ���
	for (int k = 1; k <= G.vexnum; k++)
	{
		for (int i = 1; i <= G.vexnum; i++) 
		{
			for (int j = 1; j <= G.vexnum; j++) 
			{
				if (F.Path[i][j] > F.Path[i][k] + F.Path[k][j]) 
				{
					F.Path[i][j] = F.Path[i][k] + F.Path[k][j];
					F.PrePath[i][j] = F.PrePath[k][j];
				}
			}
		}
	}
}

void Print_Floyd(AMGraph G, int start, int end) 
{
	printf("%s��%s�����·��Ϊ��\n\n", G.vexs[start], G.vexs[end]);
	Stack S;
	InitStack(&S);
	Push(&S, end);
	int tem1, tem2;
	for (tem1 = F.PrePath[start][end]; tem1 != start; tem1 = F.PrePath[start][tem2])
	{
		Push(&S, tem1);
		tem2 = tem1;
	}
	printf("%s", G.vexs[start]);
	tem1 = start;//tem1��¼ǰ������
	double sum = 0;
	while (!StackIsEmpty(S)) {
		Pop(&S, &tem2);
		printf("-%.1lfkm->%s", G.arcs[tem1][tem2], G.vexs[tem2]);
		sum += G.arcs[tem1][tem2];
		tem1 = tem2;
	}
	printf("\n��·��Ϊ%.1lfkm.\n\n", sum);
}