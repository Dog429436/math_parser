#include "math_parser.h"
#include <stdio.h>
int getPrecedence(char sign)//���� �����: ����� ������ ������ ��, ���� �����: ����� ��� ������� ���
{
	switch (sign)
	{
	case '+':
	case '-':
		return 1;
	case '*':
	case '/':
		return 2;
	default:
		return -1;
	}
}

bool hasHigherOrEqualPrecedence(tokens stackToken, tokens currentToken)//���� �����: ����� ������ �� ���� ����� ����� �������, ���� �����: ����� ��� �� ��� �� ������� ������ ������� ������� ��� ������ ���� �� ���� ����� ������
{
	int precedenceStack = getPrecedence(stackToken.sign);//����� ����� ��� ������ �� ����� �������
	int precedenceCurrent = getPrecedence(currentToken.sign);//����� ����� ��� ������ �� ����� ������ 

	if (precedenceStack == -1 || precedenceCurrent == -1) //����� ��� �� ��� ������� �� ������
	{
		return false;
	}
	return precedenceStack >= precedenceCurrent;//����� ��� �� ��� �� ��� ������� �� ����� ������� ���� �� ���� ���� ������� �� ����� ������
}


