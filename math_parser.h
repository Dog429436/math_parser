#pragma once
#include <stdbool.h>
#define NULL_TOKEN -9999 //����� ����� ������ ����� ������ ���
#define ERROR_CODE 1//����� �� ��� �����

typedef struct mathtoken//���� ������ ���� �����
{
	int value;//����� ��� ������ �� ��� �����
	char sign;//����� ������ �� ����� ������
	bool number;//����� ����� ��� �� ��� �� ����� ��� ����
} tokens;


typedef struct stackForPostfix//���� ������ ������
{
	tokens* elements;//���� ������ ������ �� �������
	int top;//����� �� ������� ������ �������
	int capacity;//����� �� ���� �������
} stack;