#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<conio.h>

FILE *fp;
int n = 0;

struct record
{
	char ID[20]; /*ID��*/ 
	char name[20]; /*ֲ����*/
	char gangmu[40]; /*��Ŀ*/
	char campus[20]; /*����У��*/
	char information[10000]; /*������Ϣ*/
}plants[10000];

int menu()
{
	char s[80];
	int a;/*�������α���*/
    system("cls");
    printf("\t\t------------------����ֲ��԰��ѯϵͳ---------------\n\n");
    printf("\t\t\t0. ��ʾ����ֲ����Ϣ\n");
    printf("\t\t\t1. ���һ��ֲ����Ϣ\n");
    printf("\t\t\t2. ����ID�Ų�ѯĳ��ֲ����Ϣ\n");
	printf("\t\t\t3. ����ֲ������ѯ����ֲ����Ϣ\n");
    printf("\t\t\t4. ɾ��ָ��ID��ֲ����Ϣ\n");
	printf("\t\t\t5. ɾ��ָ��ֲ������ֲ����Ϣ\n");
	printf("\t\t\t6. ɾ��ָ����Ŀֲ����Ϣ\n");
	printf("\t\t\t7. ɾ��ȫ��ֲ����Ϣ\n");
    printf("\t\t\t8. ����ֲ�ﰴID������\n");
    printf("\t\t\t9. ����ֲ�ﰴֲ��������\n");
	printf("\t\t\t10. ��ֲ��������Ŀ��������ʾ����ֲ����Ϣ\n");
    printf("\t\t\t11. �����˳���ѯϵͳ\n");
    printf("\t\t---------------------------------------------------\n\n");
    do{
            printf("\n\t\t�������ѡ�����ֺ��룩:");
            scanf("%s", s);
            a = atoi(s);
     }
    while (a < 0 || a > 11);
		return a;
};

void duru()
{
	if((fp = fopen("plants.bin", "rb"))==NULL)
	{
		printf("\n\t\t�ļ������ڣ�");
		if((fp = fopen("plants.bin", "wb"))==NULL)
		{
			printf("\n\t\t����ʧ�ܣ�");
		}
		else
		{
			printf("\n\t\t�ļ��ѽ�����");
			printf("\n\t\t�����������˵�");
			getch();
		}
	}
	fseek(fp, 0, 2);
	if(ftell(fp) > 0)
	{
		rewind(fp);
		for(n = 0; !feof(fp) && fread(&plants[n], sizeof(struct record), 1, fp); n++);
		printf("\n\t\t�ļ�����ɹ���");
		printf("\n\t\t�����������˵�");
		getch();
	}
	printf("\n\t\t�ļ���û�м�¼��");
	printf("\n\t\t����������ز˵�");
	getch();
}

void list()
{
     int b;
     if(n != 0)
     {
		printf("\t\t\t����%d��ֲ����Ϣ\n", n);
		printf("\t\t\t---------------����Ϊ����ֲ����Ϣ---------------\n");
		for (b = 0; b < n; b++)
		{
			printf("\t\t\tֲ������%s\n", plants[b].name);
			printf("\t\t\tֲ��ID��%s\n", plants[b].ID);
			printf("\t\t\t��Ŀ��%s\n", plants[b].gangmu);
			printf("\t\t\t����У����%s\n", plants[b].campus);
			printf("\t\t\t������Ϣ��%s\n", plants[b].information);
			printf("\t\t\t-------------------------------------------------\n");
		}
		printf("\t\t\t-------------------------------------------------\n");
	}
	else
		printf("\t\t\t���κ���Ϣ��\n");
		printf("\t\t\t��������������˵�");
		getch();
		return;
}

void add()
{
	printf("\t\t\t----------------������ֲ����Ϣ----------------\n");
	printf("\t\t\t����ֲ����:");
	scanf("%s", &plants[n].name);
	printf("\t\t\t����ֲ��ID:");
	scanf("%s", &plants[n].ID);
	printf("\t\t\t�����Ŀ:");
	scanf("%s", &plants[n].gangmu);
	printf("\t\t\t��������У��:");
	scanf("%s", &plants[n].campus);
	printf("\t\t\t����������Ϣ:");
	scanf("%s", &plants[n].information);
	n++;
	printf("\t\t\t�Ƿ�������?(Y/N):\n");
	if(getch()=='y' || getch()=='Y')
	{
		add();
	}
	return;
}

void IDsearch()
{
	int mark = 0;
	int i;
	char ID[20];
	printf("\n\t\t---------------����ֲ��ID����---------------------\n");
	printf("\n\t\t��������Ҫ���ҵ�ֲ��ID:");
	scanf("%s", &ID);
	for(i = 0; i < n; i++)
	{
		if (strcmp(plants[i].ID, ID) == 0)
		{
			printf("\n\t\t------------�����������ҵ�ֲ����Ϣ---------------\n"); 
			printf("\n\t\tֲ������ %s\n", plants[i].name);
			printf("\n\t\tֲ��ID�� %s\n", plants[i].ID);
			printf("\n\t\t��Ŀ�� %s\n", plants[i].gangmu);
			printf("\n\t\t����У���� %s\n", plants[i].campus);
			printf("\n\t\t������Ϣ�� %s\n", plants[i].information);
			printf("\n\t\t-------------------------------------------------\n"); 
			mark++;
			getch();
			return;
		}
	}
	if (mark == 0)
	{
		printf("\n\t\tû���ҵ���IDֲ�����Ϣ��");
		printf("\n\t\t��������������˵�");
		getch();
		return;
	}
	return;
}

void namesearch()
{
	int mark = 0;
	int i;
	char name[20];
	printf("\n\t\t---------------����ֲ��������---------------------\n");
	printf("\n\t\t��������Ҫ���ҵ�ֲ����:");
	scanf("%s", &name);
	for(i = 0; i < n; i++)
	{ 
		if (strcmp(plants[i].name, name) == 0)
		{
			printf("\n\t\t------------�����������ҵ�ֲ����Ϣ---------------\n");
			printf("\n\t\tֲ������ %s\n", plants[i].name);
			printf("\n\t\tֲ��ID�� %s\n", plants[i].ID);
			printf("\n\t\t��Ŀ�� %s\n", plants[i].gangmu);
			printf("\n\t\t����У���� %s\n", plants[i].campus);
			printf("\n\t\t������Ϣ�� %s\n", plants[i].information);
			printf("\n\t\t-------------------------------------------------\n");
			mark++;
			getch();
			return;
		}
	}
	if (mark == 0)
	{
		printf("\n\t\tû���ҵ���ֲ�����Ϣ��");
		printf("\n\t\t��������������˵�");
		getch();
		return;
	}
	return;
}

void IDdelete()
{
	char ID[20];
	printf("\n\t\t---------------����ֲ��IDɾ��---------------------\n");
	if(n == 0)
	{
		printf("\n\t\t���κ���Ϣ��");
		printf("\n\t\t��������������˵�");
		getch();
		return;
	}
	int i, j;
	int a = 0, b = 0, c = 0;
	printf("\n\t\t������Ҫɾ��ֲ��ID:");
	scanf("%s", ID);
	for (i = a; i < n; i++)
	{
		if (strcmp(plants[i].ID, ID) == 0)
		{
			printf("\n\t\t��������Ҫɾ����ֲ����Ϣ:");
			b++;
			printf("\n\t\t-------------------------------------------------\n");
			printf("\n\t\tֲ������ %s\n", plants[i].name);
			printf("\n\t\tֲ��ID�� %s\n", plants[i].ID);
			printf("\n\t\t��Ŀ�� %s\n", plants[i].gangmu);
			printf("\n\t\t����У���� %s\n", plants[i].campus);
			printf("\n\t\t������Ϣ�� %s\n", plants[i].information);
			printf("\n\t\t-------------------------------------------------\n");
			printf("\n\t\t�Ƿ�ɾ��?(Y/N)\n");
			if (getch() == 'y' || getch() == 'Y') //ɾ��
			{
				for (j = i; j < n - 1; j++)
				{
					plants[j] = plants[j + 1]; /*ǰ��*/
					n--;
					c++;
					printf("\n\t\t��ɾ����");
				}
			}
			printf("\t\t\t�Ƿ����ɾ��?(Y/N):\n");
			if(getch() == 'y' || getch() == 'Y')
			{
				IDdelete();
			}
			return;
		}
		else
		printf("\n\t\tû�и�ID��ֲ����Ϣ��");
		printf("\n\t\t��������������˵�");
		getch();
		return;
	}
}

void namedelete()
{
	char name[20];
	printf("\n\t\t---------------����ֲ����ɾ��---------------------\n");
	if(n == 0)
	{
		printf("\n\t\t���κ���Ϣ��");
		printf("\n\t\t��������������˵�");
		getch();
		return;
	}
	int i, j;
	int a = 0, b = 0, c = 0;
	printf("\n\t\t������Ҫɾ��ֲ����:");
	scanf("%s", name);
	for (i = a; i < n; i++)
	{
		if (strcmp(plants[i].name, name) == 0)
		{
			printf("\n\t\t��������Ҫɾ����ֲ����Ϣ:");
			b++;
			printf("\n\t\t-------------------------------------------------\n");
			printf("\n\t\tֲ������ %s\n", plants[i].name);
			printf("\n\t\tֲ��ID�� %s\n", plants[i].ID);
			printf("\n\t\t��Ŀ�� %s\n", plants[i].gangmu);
			printf("\n\t\t����У���� %s\n", plants[i].campus);
			printf("\n\t\t������Ϣ�� %s\n", plants[i].information);
			printf("\n\t\t-------------------------------------------------\n");
			printf("\n\t\t�Ƿ�ɾ��?(Y/N)");
			if (getch() == 'y' || getch() == 'Y') //ɾ��
			{
				for (j = i; j < n - 1; j++)
				{
					plants[j] = plants[j + 1]; /*ǰ��*/
					n--;
					c++;
					printf("\n\t\t��ɾ����");
				}
			}
			printf("\t\t\t�Ƿ����ɾ��?(Y/N):\n");
			if(getch() == 'y' || getch() == 'Y')
			{
				namedelete();
			}
			return;
		}
		else
		printf("\n\t\tû�и����ֵ�ֲ����Ϣ��");
		printf("\n\t\t��������������˵�");
		getch();
		return;
	}
}

void gangmudelete()
{
	char gangmu[40];
	printf("\n\t\t---------------���ݸ�Ŀɾ��----------------------\n");
	if(n == 0)
	{
		printf("\n\t\t���κ���Ϣ��");
		printf("\n\t\t��������������˵�");
		getch();
		return;
	}
	int i, j;
	int a = 0, b = 0, c = 0;
	printf("\n\t\t������Ҫɾ����Ŀ��:\n");
	scanf("%s", gangmu);
	for (i = a; i < n; i++)
	{
		if (strcmp(plants[i].gangmu, gangmu) == 0)
		{
			printf("\n\t\t��������Ҫɾ����ֲ����Ϣ:");
			b++;
			printf("\n\t\t-------------------------------------------------\n");
			printf("\n\t\tֲ������ %s\n", plants[i].name);
			printf("\n\t\tֲ��ID�� %s\n", plants[i].ID);
			printf("\n\t\t��Ŀ�� %s\n", plants[i].gangmu);
			printf("\n\t\t����У���� %s\n", plants[i].campus);
			printf("\n\t\t������Ϣ�� %s\n", plants[i].information);
			printf("\n\t\t-------------------------------------------------\n");
			printf("\n\t\t�Ƿ�ɾ��?(Y/N)");
			if (getch() == 'y' || getch() == 'Y') //ɾ��
			{
				for (j = i; j < n - 1; j++)
				{
					plants[j] = plants[j + 1]; /*ǰ��*/
					n--;
					c++;
					printf("\n\t\t��ɾ����");
				}
			}
			printf("\t\t\t�Ƿ����ɾ��?(Y/N):\n");
			if(getch() == 'y' || getch() == 'Y')
			{
				gangmudelete();
			}
			return;
		}
		else
		printf("\n\t\tû�иø�Ŀ��ֲ����Ϣ��");
		printf("\n\t\t��������������˵�");
		getch();
		return;
	}
}

void alldelete()
{
	printf("\n\t\t---------------ɾ��ȫ����Ϣ----------------------\n");
	if(n == 0)
	{
		printf("\n\t\t���κ���Ϣ��");
		printf("\n\t\t��������������˵�");
		getch();
		return;
	}
	printf("\n\t\t�Ƿ�ɾ��?(Y/N)");
	if (getch() == 'y' || getch() == 'Y') //ɾ��
	{
		fclose(fp);
		if((fp = fopen("plants.bin", "wb"))==NULL)
		{
			printf("�ļ������ڣ�");
		}
		n = 0;
		printf("\n\t\t��ɾ����");
		printf("\n\t\t��������������˵�");
		getch();
		return;
	}	
}

void IDlist()
{
	printf("\n\t\t---------------����ID����----------------------\n");
	if(n == 0)
	{
		printf("\n\t\t���κ���Ϣ��");
		printf("\n\t\t��������������˵�");
		getch();
		return;
	}
	int a, b;
	struct record t;
	for (a = 1; a < n; a++)
	{
		if(strcmp(plants[a].ID, plants[a-1].ID) <0)
		{
			t = plants[a];
			b = a - 1;
			do
			{
				plants[b + 1] = plants[b];
				b--;
			}
			while ((strcmp(t.ID, plants[b].ID) < 0&&b >= 0));
			plants[b + 1] = t;
		}
	}
	printf("\n\t\t����Ϊ������:\n");
	list();
	return;
}

void namelist()
{
	printf("\n\t\t--------------����ֲ��������---------------------\n");
	if(n == 0)
	{
		printf("\n\t\t���κ���Ϣ��");
		printf("\n\t\t��������������˵�");
		getch();
		return;
	}
	int a, b;
	struct record t;
	for (a = 1; a < n; a++)
	{
		if(strcmp(plants[a].name, plants[a-1].name) <0)
		{
			t = plants[a];
			b = a - 1;
			do
			{
				plants[b + 1] = plants[b];
				b--;
			}
			while ((strcmp(t.ID, plants[b].name) < 0&&b >= 0));
			plants[b + 1] = t;
		}
	}
	printf("\n\t\t����Ϊ������:\n");
	list();
	return;
}

void gangmulist()
{
	int a, b;
	struct record t;
	printf("\n\t\t--------------���ݸ�Ŀ����---------------------\n");
	if(n == 0)
	{
		printf("\n\t\t���κ���Ϣ��");
		printf("\n\t\t��������������˵�");
		getch();
		return;
	}
	for (a = 1; a < n; a++)
	{
		if(strcmp(plants[a].gangmu, plants[a-1].gangmu) <0)
		{
			t = plants[a];
			b = a - 1;
			do
			{
				plants[b + 1] = plants[b];
				b--;
			}
			while ((strcmp(t.ID, plants[b].gangmu) < 0&&b >= 0));
			plants[b + 1] = t;
		}
	}
	printf("\n\t\t����Ϊ������:\n");
	list();
	return;	
}

void baocun()
{
	int a;
	if((fp = fopen("plants.bin", "wb"))==NULL)
	{
		printf("\n\t\t�ļ������ڣ�");
	}
	for(a = 0; a < n; a++)
	{
		if(fwrite(&plants[a], sizeof(struct record), 1, fp) != 1)
		{
			printf("\n\t\t�ļ���д�룡");
		}
	}
	fclose(fp);
	printf("\n\t\t�ļ��ѱ��棡");
	printf("\n\t\t��������˳���ѯϵͳ");
	getch();
	exit(0);
}

void main()
{
	duru();
	while(1)
	{
    	switch(menu())
    	{
			
			case 0:
			{
				list();
				break;
			}
			case 1:
			{
                add();
                break;
			}
			case 2:
			{
                IDsearch();
                break;
			}
			case 3:
			{
                namesearch();
                break;
			}
			case 4:
			{
                IDdelete();
                break;
			}
			case 5:
			{
                namedelete();
                break;
			}
			case 6:
			{
                gangmudelete();
                break;
			}
			case 7:
			{
                alldelete();
                break;
			}
			case 8:
			{
                IDlist();
                break;
			}
			case 9:
			{
                namelist();
                break;
			}
			case 10:
			{
				gangmulist();
				break;
			}
			case 11:
			{
				baocun();
				system("pause");
			}
		
    		}
	}
	return ;
}