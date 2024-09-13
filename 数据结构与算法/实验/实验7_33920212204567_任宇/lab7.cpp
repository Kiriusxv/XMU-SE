#include "lab7.h"

int main() {
	AMGraph G;
	CreateMap(&G);
	printf("�ṩ·����ѯ����ľ����У�\n");
	Print(G);
	printf("\n");
	Menu();
	int choice;
	scanf("%d", &choice);
	while (choice) 
	{
		switch (choice) 
		{
		case 1:
			PrintMap(G);
			break;
		case 2:
			Print(G);
			printf("\n�����������ѯ�ľ����Ӧ����ţ�\n");
			int x;
			scanf("%d", &x);
			if (x<1 || x>G.vexnum)
			{
				printf("�������\n");
			}
			else {
				Dijkstra(G, x);
				Print_Dij(G, x);
			}
			break;
		case 3:
			Print(G);
			printf("\n��ֱ����������յ�Ķ�Ӧ��ţ�\n");
			int x1, x2;
			scanf("%d %d", &x1, &x2);
			if (x1<1 || x1>G.vexnum || x2<1 || x2>G.vexnum||x1==x2) 
			{
				printf("������󣡣�\n");
			}
			else 
			{
				floyd(G);
				Print_Floyd(G, x1, x2);
			}
			break;
		default:
			printf("����������������룡\n");
			break;
		}
		Menu();
		scanf("%d", &choice);
	}
	printf("\n�������⣡\n");
	printf("�����˳�...\n");
	Sleep(2000);
	return 0;
}