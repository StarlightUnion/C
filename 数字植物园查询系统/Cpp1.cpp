#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<conio.h>

FILE *fp;
int n = 0;

struct record
{
	char ID[20]; /*ID号*/ 
	char name[20]; /*植物名*/
	char gangmu[40]; /*纲目*/
	char campus[20]; /*所属校区*/
	char information[10000]; /*其它信息*/
}plants[10000];

int menu()
{
	char s[80];
	int a;/*定义整形变量*/
    system("cls");
    printf("\t\t------------------数字植物园查询系统---------------\n\n");
    printf("\t\t\t0. 显示所有植物信息\n");
    printf("\t\t\t1. 添加一种植物信息\n");
    printf("\t\t\t2. 根据ID号查询某类植物信息\n");
	printf("\t\t\t3. 根据植物名查询该类植物信息\n");
    printf("\t\t\t4. 删除指定ID的植物信息\n");
	printf("\t\t\t5. 删除指定植物名的植物信息\n");
	printf("\t\t\t6. 删除指定纲目植物信息\n");
	printf("\t\t\t7. 删除全部植物信息\n");
    printf("\t\t\t8. 所有植物按ID号排序\n");
    printf("\t\t\t9. 所有植物按植物名排序\n");
	printf("\t\t\t10. 按植物所属纲目，分类显示各类植物信息\n");
    printf("\t\t\t11. 保存退出查询系统\n");
    printf("\t\t---------------------------------------------------\n\n");
    do{
            printf("\n\t\t输入你的选择（数字号码）:");
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
		printf("\n\t\t文件不存在！");
		if((fp = fopen("plants.bin", "wb"))==NULL)
		{
			printf("\n\t\t建立失败！");
		}
		else
		{
			printf("\n\t\t文件已建立！");
			printf("\n\t\t按任意键进入菜单");
			getch();
		}
	}
	fseek(fp, 0, 2);
	if(ftell(fp) > 0)
	{
		rewind(fp);
		for(n = 0; !feof(fp) && fread(&plants[n], sizeof(struct record), 1, fp); n++);
		printf("\n\t\t文件读入成功！");
		printf("\n\t\t按任意键进入菜单");
		getch();
	}
	printf("\n\t\t文件中没有记录！");
	printf("\n\t\t按任意键返回菜单");
	getch();
}

void list()
{
     int b;
     if(n != 0)
     {
		printf("\t\t\t共有%d条植物信息\n", n);
		printf("\t\t\t---------------以下为所有植物信息---------------\n");
		for (b = 0; b < n; b++)
		{
			printf("\t\t\t植物名：%s\n", plants[b].name);
			printf("\t\t\t植物ID：%s\n", plants[b].ID);
			printf("\t\t\t纲目：%s\n", plants[b].gangmu);
			printf("\t\t\t所属校区：%s\n", plants[b].campus);
			printf("\t\t\t其它信息：%s\n", plants[b].information);
			printf("\t\t\t-------------------------------------------------\n");
		}
		printf("\t\t\t-------------------------------------------------\n");
	}
	else
		printf("\t\t\t无任何信息！\n");
		printf("\t\t\t按任意键返回主菜单");
		getch();
		return;
}

void add()
{
	printf("\t\t\t----------------请输入植物信息----------------\n");
	printf("\t\t\t输入植物名:");
	scanf("%s", &plants[n].name);
	printf("\t\t\t输入植物ID:");
	scanf("%s", &plants[n].ID);
	printf("\t\t\t输入纲目:");
	scanf("%s", &plants[n].gangmu);
	printf("\t\t\t输入所属校区:");
	scanf("%s", &plants[n].campus);
	printf("\t\t\t输入其它信息:");
	scanf("%s", &plants[n].information);
	n++;
	printf("\t\t\t是否继续添加?(Y/N):\n");
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
	printf("\n\t\t---------------根据植物ID查找---------------------\n");
	printf("\n\t\t请输入您要查找的植物ID:");
	scanf("%s", &ID);
	for(i = 0; i < n; i++)
	{
		if (strcmp(plants[i].ID, ID) == 0)
		{
			printf("\n\t\t------------以下是您查找的植物信息---------------\n"); 
			printf("\n\t\t植物名： %s\n", plants[i].name);
			printf("\n\t\t植物ID： %s\n", plants[i].ID);
			printf("\n\t\t纲目： %s\n", plants[i].gangmu);
			printf("\n\t\t所属校区： %s\n", plants[i].campus);
			printf("\n\t\t其它信息： %s\n", plants[i].information);
			printf("\n\t\t-------------------------------------------------\n"); 
			mark++;
			getch();
			return;
		}
	}
	if (mark == 0)
	{
		printf("\n\t\t没有找到该ID植物的信息！");
		printf("\n\t\t按任意键返回主菜单");
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
	printf("\n\t\t---------------根据植物名查找---------------------\n");
	printf("\n\t\t请输入您要查找的植物名:");
	scanf("%s", &name);
	for(i = 0; i < n; i++)
	{ 
		if (strcmp(plants[i].name, name) == 0)
		{
			printf("\n\t\t------------以下是您查找的植物信息---------------\n");
			printf("\n\t\t植物名： %s\n", plants[i].name);
			printf("\n\t\t植物ID： %s\n", plants[i].ID);
			printf("\n\t\t纲目： %s\n", plants[i].gangmu);
			printf("\n\t\t所属校区： %s\n", plants[i].campus);
			printf("\n\t\t其它信息： %s\n", plants[i].information);
			printf("\n\t\t-------------------------------------------------\n");
			mark++;
			getch();
			return;
		}
	}
	if (mark == 0)
	{
		printf("\n\t\t没有找到该植物的信息！");
		printf("\n\t\t按任意键返回主菜单");
		getch();
		return;
	}
	return;
}

void IDdelete()
{
	char ID[20];
	printf("\n\t\t---------------根据植物ID删除---------------------\n");
	if(n == 0)
	{
		printf("\n\t\t无任何信息！");
		printf("\n\t\t按任意键返回主菜单");
		getch();
		return;
	}
	int i, j;
	int a = 0, b = 0, c = 0;
	printf("\n\t\t请输入要删除植物ID:");
	scanf("%s", ID);
	for (i = a; i < n; i++)
	{
		if (strcmp(plants[i].ID, ID) == 0)
		{
			printf("\n\t\t以下是您要删除的植物信息:");
			b++;
			printf("\n\t\t-------------------------------------------------\n");
			printf("\n\t\t植物名： %s\n", plants[i].name);
			printf("\n\t\t植物ID： %s\n", plants[i].ID);
			printf("\n\t\t纲目： %s\n", plants[i].gangmu);
			printf("\n\t\t所属校区： %s\n", plants[i].campus);
			printf("\n\t\t其它信息： %s\n", plants[i].information);
			printf("\n\t\t-------------------------------------------------\n");
			printf("\n\t\t是否删除?(Y/N)\n");
			if (getch() == 'y' || getch() == 'Y') //删除
			{
				for (j = i; j < n - 1; j++)
				{
					plants[j] = plants[j + 1]; /*前移*/
					n--;
					c++;
					printf("\n\t\t已删除！");
				}
			}
			printf("\t\t\t是否继续删除?(Y/N):\n");
			if(getch() == 'y' || getch() == 'Y')
			{
				IDdelete();
			}
			return;
		}
		else
		printf("\n\t\t没有该ID的植物信息！");
		printf("\n\t\t按任意键返回主菜单");
		getch();
		return;
	}
}

void namedelete()
{
	char name[20];
	printf("\n\t\t---------------根据植物名删除---------------------\n");
	if(n == 0)
	{
		printf("\n\t\t无任何信息！");
		printf("\n\t\t按任意键返回主菜单");
		getch();
		return;
	}
	int i, j;
	int a = 0, b = 0, c = 0;
	printf("\n\t\t请输入要删除植物名:");
	scanf("%s", name);
	for (i = a; i < n; i++)
	{
		if (strcmp(plants[i].name, name) == 0)
		{
			printf("\n\t\t以下是您要删除的植物信息:");
			b++;
			printf("\n\t\t-------------------------------------------------\n");
			printf("\n\t\t植物名： %s\n", plants[i].name);
			printf("\n\t\t植物ID： %s\n", plants[i].ID);
			printf("\n\t\t纲目： %s\n", plants[i].gangmu);
			printf("\n\t\t所属校区： %s\n", plants[i].campus);
			printf("\n\t\t其它信息： %s\n", plants[i].information);
			printf("\n\t\t-------------------------------------------------\n");
			printf("\n\t\t是否删除?(Y/N)");
			if (getch() == 'y' || getch() == 'Y') //删除
			{
				for (j = i; j < n - 1; j++)
				{
					plants[j] = plants[j + 1]; /*前移*/
					n--;
					c++;
					printf("\n\t\t已删除！");
				}
			}
			printf("\t\t\t是否继续删除?(Y/N):\n");
			if(getch() == 'y' || getch() == 'Y')
			{
				namedelete();
			}
			return;
		}
		else
		printf("\n\t\t没有该名字的植物信息！");
		printf("\n\t\t按任意键返回主菜单");
		getch();
		return;
	}
}

void gangmudelete()
{
	char gangmu[40];
	printf("\n\t\t---------------根据纲目删除----------------------\n");
	if(n == 0)
	{
		printf("\n\t\t无任何信息！");
		printf("\n\t\t按任意键返回主菜单");
		getch();
		return;
	}
	int i, j;
	int a = 0, b = 0, c = 0;
	printf("\n\t\t请输入要删除纲目名:\n");
	scanf("%s", gangmu);
	for (i = a; i < n; i++)
	{
		if (strcmp(plants[i].gangmu, gangmu) == 0)
		{
			printf("\n\t\t以下是您要删除的植物信息:");
			b++;
			printf("\n\t\t-------------------------------------------------\n");
			printf("\n\t\t植物名： %s\n", plants[i].name);
			printf("\n\t\t植物ID： %s\n", plants[i].ID);
			printf("\n\t\t纲目： %s\n", plants[i].gangmu);
			printf("\n\t\t所属校区： %s\n", plants[i].campus);
			printf("\n\t\t其它信息： %s\n", plants[i].information);
			printf("\n\t\t-------------------------------------------------\n");
			printf("\n\t\t是否删除?(Y/N)");
			if (getch() == 'y' || getch() == 'Y') //删除
			{
				for (j = i; j < n - 1; j++)
				{
					plants[j] = plants[j + 1]; /*前移*/
					n--;
					c++;
					printf("\n\t\t已删除！");
				}
			}
			printf("\t\t\t是否继续删除?(Y/N):\n");
			if(getch() == 'y' || getch() == 'Y')
			{
				gangmudelete();
			}
			return;
		}
		else
		printf("\n\t\t没有该纲目的植物信息！");
		printf("\n\t\t按任意键返回主菜单");
		getch();
		return;
	}
}

void alldelete()
{
	printf("\n\t\t---------------删除全部信息----------------------\n");
	if(n == 0)
	{
		printf("\n\t\t无任何信息！");
		printf("\n\t\t按任意键返回主菜单");
		getch();
		return;
	}
	printf("\n\t\t是否删除?(Y/N)");
	if (getch() == 'y' || getch() == 'Y') //删除
	{
		fclose(fp);
		if((fp = fopen("plants.bin", "wb"))==NULL)
		{
			printf("文件不存在！");
		}
		n = 0;
		printf("\n\t\t已删除！");
		printf("\n\t\t按任意键返回主菜单");
		getch();
		return;
	}	
}

void IDlist()
{
	printf("\n\t\t---------------根据ID排序----------------------\n");
	if(n == 0)
	{
		printf("\n\t\t无任何信息！");
		printf("\n\t\t按任意键返回主菜单");
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
	printf("\n\t\t以下为排序结果:\n");
	list();
	return;
}

void namelist()
{
	printf("\n\t\t--------------根据植物名排序---------------------\n");
	if(n == 0)
	{
		printf("\n\t\t无任何信息！");
		printf("\n\t\t按任意键返回主菜单");
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
	printf("\n\t\t以下为排序结果:\n");
	list();
	return;
}

void gangmulist()
{
	int a, b;
	struct record t;
	printf("\n\t\t--------------根据纲目排序---------------------\n");
	if(n == 0)
	{
		printf("\n\t\t无任何信息！");
		printf("\n\t\t按任意键返回主菜单");
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
	printf("\n\t\t以下为排序结果:\n");
	list();
	return;	
}

void baocun()
{
	int a;
	if((fp = fopen("plants.bin", "wb"))==NULL)
	{
		printf("\n\t\t文件不存在！");
	}
	for(a = 0; a < n; a++)
	{
		if(fwrite(&plants[a], sizeof(struct record), 1, fp) != 1)
		{
			printf("\n\t\t文件已写入！");
		}
	}
	fclose(fp);
	printf("\n\t\t文件已保存！");
	printf("\n\t\t按任意键退出查询系统");
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