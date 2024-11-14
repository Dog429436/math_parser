
#include "math_parser.h"
#include "tokenization.h"
#include <stdbool.h>

int Tokenize(tokens* tokenptr, char* equation, int* p_index, int equationlength) //���� �����: ����� ������ �� ���� ������ ���, ���� �� ������, ����� ������� ����� �� ������� ����� �����, ���� �����, ����� �� �� ����� ����� �����
{
	int index = *p_index;//���� ������� ������

	while (index < equationlength && (equation[index] == ' ' || equation[index] == '\t')) //�� ��� ��� ������ ��� ���� �������� ������ ���� ����� ��� ����
	{
		index++;
	}

	*p_index = index;//����� ������ �� ������� ������

	if (index < equationlength) //�� ������� ������ ��� ���� �����
	{
		tokens t1;//����� ����

		if (equation[index] == '-' && isUnaryMinus(equation, index)) //�� ����� ������ ��� ���� �����
		{
			index++;//������� ����
			int start_index = index;//���� ������ ������
			int num = FindNumbers(&index, equation);//����� ������ ������ ������ �� �����

			if (start_index == index) //�� ������� �� ��� ���� ���� ��� ���� ����� ���
			{
				printf("Input is invalid\n");//����� �����
				return -1;
			}

			t1.value = -num;//��� ����� ���� ����� ������
			t1.sign = 0;//����� ���� 0
			t1.number = true;
			tokenptr[*p_index] = t1;//����� ���� ������� ������ ����� �������

			for (int i = start_index; i < index; i++) //���� �� ������� ������� ���� ��� ����� ����� ����� ��� ������� ������� �������
			{
				tokenptr[i].value = NULL_TOKEN;
				tokenptr[i].sign = 0;
				tokenptr[i].number = false;
			}

			*p_index = index;//����� ������ ������� ������
		}
		else if (equation[index] >= '0' && equation[index] <= '9') //�� ��� ������ ��� ����
		{
			int start_index = index;//���� ������� ������
			int num = FindNumbers(&index, equation);//����� ������ ������ ������ �� ����� ����� ���

			t1.value = num;
			t1.sign = 0;
			t1.number = true;
			tokenptr[start_index] = t1;

			for (int i = start_index + 1; i < index; i++) //����� �� ������� ���� ��� ����� ����� ����� ��� ������� ������� �������
			{
				tokenptr[i].value = NULL_TOKEN;
				tokenptr[i].sign = 0;
				tokenptr[i].number = false;
			}

			*p_index = index;
		}
		else if (equation[index] == '+' || equation[index] == '-' ||
			equation[index] == '*' || equation[index] == '/' ||
			equation[index] == '(' || equation[index] == ')') //�� ����� ������ ��� ��� �������� �����
		{
			t1.value = 0;//���� ���� 0
			t1.sign = equation[index];//����� ���� ��� ������
			t1.number = false;
			tokenptr[index] = t1;

			index++;
			*p_index = index;
		}
		else {
			printf("Input is invalid\n");
			return -1;
		}

		return Tokenize(tokenptr, equation, p_index, equationlength);//����� ���� �������
	}

	return 0;
}

int FindNumbers(int* index, char* equation)//���� �����: ����� ������ �� ����� ������� �����, ���� �����: ����� ��� �� ������ �� ����� ����� ����� 10 ������ ������� ������ �����
{
	int num = 0;
	int i = *index;
	int length = strlen(equation);
	while (i < length && equation[i] >= '0' && equation[i] <= '9')//�� ��� ��� ������ ��� ����
	{
		num = num * 10 + equation[i] - '0';//����� ���� ����� 10
		i++;
	}
	*index = i;//����� ������ �����
	return num;//����� �����
}

bool isUnaryMinus(char* equation, int minus_index)//���� �����: ����� ������ �� ���� ������� �� �����, ���� �����: ����� ��� �� ����� ����� �� ��� ���� ���� ���� �����
{
	if (minus_index == 0)//�� ������ ��� ������ ������� �� �� ���� �����
	{
		return true;
	}

	int prev_index = minus_index - 1;//���� ��� ������� ����� ������� �����
	while (prev_index >= 0 && (equation[prev_index] == ' ' || equation[prev_index] == '\t'))//�� ��� ������� ���� �0
	{
		prev_index--;//������� ���
	}
	if (prev_index < 0)//�� ������� ��� ������
	{
		return true;//����� ���
	}
	char prev_char = equation[prev_index];//�� ����� �� ���� ���� ����� �� �� ���� �����
	if (prev_char == '(' || prev_char == '+' || prev_char == '-' ||
		prev_char == '*' || prev_char == '/')
	{
		return true;
	}
	return false;//���� ����� ���
}
