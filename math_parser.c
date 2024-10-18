#define _CRT_SECURE_NO_WARNINGS //����� ������ ��������� ���� �� ������ ����� ��������� �� ������
#include <stdio.h>//����� ������ �������� �� ��� ���
#include <stdlib.h>//����� ������ �������� ���������
#include "math_parser.h"//����� header file 
#include <string.h>//����� ������ �������� �� �������
#include <stdbool.h>//����� ������ ����� ���� �� ���
#define NULL_TOKEN -9999//����� ������ ��� ����� -999
typedef enum sings { add = 1, sub, mult, divide }operands;
typedef struct mathtoken//����� ���� ���� ���� �� �����
{
	int value;//�� �� ���� �� ����� �����
	char sign;//�� �� ���� �� ����� ����� ������
	bool number;//��� �� ��� �� �� ����
}tokens;

static int FindNumbers(int *index, char * equation)//���� �����: ���� ����� ������� ������ �����, ���� �����: ����� ����� ������ ���� ����� ������� ������ ��� ������ ������ ������ ����� ����� ����� �� ����� ���� ��� ����
{
	int num = 0;//����� ����� ������ �� ����� �����
	int i = *index;//����� ����� ����� �� ���� ������� ����� ����
	int length = strlen(equation);//����� ����� ����� �� ���� ������� ����� ��� ������� ��� ����
	while (i < length && equation[i] >= '0' && equation[i] <= '9')//�� ��� ������� ��� ���� ������� ���� ���� ������� �� ����
	{
		num = num * 10 + equation[i] - '0';//����� ����� ������ ������� ����� 10
		i++;//����� ������ ������ ��� ������� �� ������� ���� ���� ��� ��� ����� ��� ���� ��� ���� �����
	}
	*index = i;//����� ������ �� ���� ����
	return num;//����� ����� ������ ���� ������
}
bool isUnaryMinus(char* equation, int minus_index)//���� �����: ����� ����� ������� ������� ���� ������ ���� ��� ������� ������ ��� ���� ����� ����� ����� ����� ���� �����, ���� �����: ����� ��� �� ��� ��� ���� ������ ������ ��� ���� �����
{
	if (minus_index == 0)//�� ������ ���� ������ ��� 0 �� �� ���� �����
	{
		return true;//����� ���
	}

	int prev_index = minus_index - 1;//����� ������� ����� ������� �� �����
	while (prev_index >= 0 && (equation[prev_index] == ' ' || equation[prev_index] == '\t'))//�� ��� ������� �� ������ ����� ���� ������ ������� ��� ����
	{
		prev_index--;//������� ���
	}
	if (prev_index < 0)//�� ������ ��� ���� ����� ����� �� ���� �����
	{
		return true;//����� ���
	}
	char prev_char = equation[prev_index];//����� ��� ���� �����
	if (prev_char == '(' || prev_char == '+' || prev_char == '-' ||//�� ��� �� ����� �� ���� ��� �������� �� ���� �����
		prev_char == '*' || prev_char == '/')
	{
		return true;//����� ���
	}
	return false;//����� ���
}


static int Tokenize(tokens* tokenptr, char* equation, int* p_index, int equationlength)//���� �����: ����� ���� ������ ���, ������, ����� ������� ����� ����� �������, ���� �����: ����� ���� ������� ����� ������ �������
{
	int index = *p_index;//����� ����� ����� ����� ���� ������� ����� ����

	while (index < equationlength && (equation[index] == ' ' || equation[index] == '\t'))//�� ��� ������� ���� ��� ����� ���� ��� ������ ��� ����
	{

		tokenptr[index].value = NULL_TOKEN;//����� ���� ���� ��� ����� ������ �������� ��� �� ������ ���� �����
		tokenptr[index].sign = 0;
		tokenptr[index].number = false;

		index++;//����� ������ ������ ����� ����� ������� ������
	}

	*p_index = index; //����� ������ ������� ������ ���� ������� ������ ���� ����� ����� �� �������

	if (index < equationlength)//�� ��� ������� ����� ��� �����
	{
		tokens t1;//����� ���� �� ����

		if (equation[index] == '-' && isUnaryMinus(equation, index))//�� ��� ������ ������ ������� ��� ����� ������ ��� ������ ������
		{
			index++;//����� ������� ������ ����� ����� ��� �������
			int start_index = index;//����� ����� ����� ������� ��� ������ ������
			int num = FindNumbers(&index, equation);//����� ����� ����� ��� ���� ������� ������� ������

			if (start_index == index)//�� ������ ������� ���� ������ ������ ������� ���� ����� ����� - �� ���� ��� ��� �� ����
			{
				printf("Input is invalid\n");//����� ����� ���� �� ����
				return -1;//����� �������
			}

			t1.value = -num;//�� ����� ���� �� ����� ���� ���� ��� ���� �� ����� ������
			t1.sign = 0;//���� 0 ���� ��� �� ����
			t1.number = true;//��� ���� ��� �� ����
			tokenptr[*p_index] = t1; // ����� ����� ������ ����� ������� ������ ������

			for (int i = start_index; i < index; i++)//������� ��� ������� ����� ����� ��� ��� ������� ������� ������� �� ����� �� ��� �������� ������ �� ���� ����� ����� ��� ����� �� �������
			{
				tokenptr[i].value = NULL_TOKEN;
				tokenptr[i].sign = 0;
				tokenptr[i].number = true;
			}

			*p_index = index;//����� ������ ������ ������ ���� ������
		}
		else if (equation[index] >= '0' && equation[index] <= '9')//�� ��� ������ ������ ������� �� ����
		{
			int start_index = index;//����� ����� ����� �� ������ ������ �������
			int num = FindNumbers(&index, equation);//����� ��� �������� ����� 10 ������� ������ ������ ������ ����� ����

			t1.value = num;//����� ���� �� ���� ������ ��� ������� ����� ��� ����� 10
			t1.sign = 0;
			t1.number = true;
			tokenptr[start_index] = t1;

			for (int i = start_index + 1; i < index; i++)//������� ��� ������ ����� ����� �� ���� ���
			{
				tokenptr[i].value = NULL_TOKEN;
				tokenptr[i].sign = 0;
				tokenptr[i].number = true;
			}

			*p_index = index;//����� ������ ������ �� ����� ����� ����� ����� ��� ��������� ����� ������� �� ����� �������
		}
		else if (equation[index] == '+' || equation[index] == '-' ||//�� ��� ������ ������ ������� ��� ����
			equation[index] == '*' || equation[index] == '/' ||
			equation[index] == '(' || equation[index] == ')')
		{
			t1.value = 0;//��� ����� ����� �� ����
			t1.sign = equation[index];//����� ���� ��� ������ ������
			t1.number = false;//������ ��� ���� ��� ���� ����� ��� �� ��� �� ����
			tokenptr[index] = t1;

			index++;//����� �������
			*p_index = index;//����� ������ ������� ������ �� ������� �������
		}
		else
		{
			printf("Input is invalid\n");//�� �� �� ����� , ���� �� ���� �� ���� �� ���� ���� ����� ����� �����
			return -1; //����� ��� �� �����
		}

		return Tokenize(tokenptr, equation, p_index, equationlength);//������ ���������� ���� ����� ����� ����� �� �������� ���������
	}

	return 0; //����� ��� ����� ������� ����� ��� �� ����� ������
}




char* CreateEquation()//���� �����: ����� ������, ���� �����: ����� ����� ����� ��� ���� ������ �� ������ ������ �� ��� ����� ������ ������
{
	char tempBuffer[100];//����� ���� ���� ������ �� ������� ������
	printf("Enter equation: \n");
	if (fgets(tempBuffer, sizeof(tempBuffer), stdin) == NULL)//����� ��������� ��� ������� ����� ������ ������ �� ���� ������ ����� ��� null
	{
		printf("Input reading failed\n");//�� �� ����� ����� �����
		return NULL;//����� ���� null
	}
	printf("Input reading succeeded\n");//���� ����� ����� �����

	size_t len = strlen(tempBuffer);//������ ���� ������� ������ ������ ���� ���� ��� ���� ������ ����� ���� �� ���� ����� �����
	if (len > 0 && tempBuffer[len - 1] == '\n')//�� ���� ���� ���� �0 ���� ������ ������ ���� ��� ���� �� ����� ����
	{
		tempBuffer[len - 1] = '\0';//���� ���� ���� ������� ������ �� ���� ����� �����
	}

	int size = strlen(tempBuffer);//����� ��� ������� ������ ������ ����
	char* equation = (char*)malloc((size + 1) * sizeof(char));//����� ����� ������� ��� ����� ������� ����� ������� ������ ���� 1
	if (equation == NULL)//�� ������ ����� �� null 
	{
		printf("Memory allocation failed\n");//�� ���� ������ ���� ������ �� ����� ����� �����
		return NULL;//����� null
	}

	strcpy(equation, tempBuffer);//����� ������� ������ ������� ������ ������� ������
	printf("Equation entered: %s\n", equation);//����� �������
	return equation;//����� ������ �������
}

int main()//����� ����� ����� ������ �������
{
	char* equation = CreateEquation();//����� ����� ������� ������ �� ���� ����� ������ ������ ������
	if (equation == NULL)//�� ������ ����� �� null
	{
		printf("Failed to create equation.\n");//����� ����� ������ ������ ������� �� ����� ����� ����� 
		return -1;//����� ��� ����� ������ ��������
	}
	printf("Memory allocation succeeded for string\n");//����� ����� �����

	int equationlength = strlen(equation);//����� ���� ����� ������ ����
	tokens* t1 = (tokens*)malloc(equationlength * sizeof(tokens));//����� ���� ������ ����� �������
	if (t1 == NULL)//�� ������ ����� �� null
	{
		printf("Memory allocation failed\n");//����� ����� ������ ������ ������� �� ����� ����� �����
		free(equation);//����� ������ ���� ������� ������ ��� ���� ������
		return -1;//����� ��� ����� ������ ��������
	}
	printf("Memory allocation succeeded for tokens\n");//����� ����� �����

	for (int i = 0; i < equationlength; i++)//����� �� ���� ����� ������� ����� �������
	{
		t1[i].value = NULL_TOKEN;
		t1[i].sign = 0;
		t1[i].number = false;
	}

	int index = 0;//����� ����� �� ����� �����
	int answer = Tokenize(t1, equation, &index, equationlength);//����� ����� ������� ������ ��� ������ ������

	if (answer != 0)//�� ����� ���� ��� 0
	{
		printf("One of the tokens is invalid.\n");//���� ������ ����� ���� ������� ���� ����� ����� �����
		free(equation);//����� ����� �������� ���� �������
		free(t1);//����� ����� ����� ������� ��� �������
		return -1;//����� ��� ����� ������ ��������
	}
	printf("Successfully tokenized the string\n");//����� ����� �����

	for (int i = 0; i < equationlength; i++)//����� ���� �������
	{
		if (t1[i].value == NULL_TOKEN)//������� ������ ������
		{
			continue;
		}
		else
		{
			if (t1[i].number)
			{
				printf("Token: %d, value: %d\n", i, t1[i].value);
			}
			else
			{
				printf("Token: %d, sign: %c\n", i, t1[i].sign);
			}
		}
	}

	free(equation);//����� ������� ����� �������
	free(t1);//����� ������� ����� ����� �������
	return 0;//����� ��� ����� ������ ��������
}



