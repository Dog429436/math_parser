#pragma once
#include "math_parser.h"

int getPrecedence(char sign);//���� �����: ����� ������ ������ ��, ���� �����: ����� ��� ������� ���

bool hasHigherOrEqualPrecedence(tokens stackToken, tokens currentToken);//���� �����: ����� ������ �� ���� ����� ����� �������, ���� �����: ����� ��� �� ��� �� ������� ������ ������� ������� ��� ������ ���� �� ���� ����� ������


