#pragma once
#include "math_parser.h" 

int Tokenize(tokens* tokenptr, char* equation, int* p_index, int equationlength); //���� �����: ����� ������ �� ���� ������ ���, ���� �� ������, ����� ������� ����� �� ������� ����� �����, ���� �����, ����� �� �� ����� ����� �����

int FindNumbers(int* index, char* equation);//���� �����: ����� ������ �� ����� ������� �����, ���� �����: ����� ��� �� ������ �� ����� ����� ����� 10 ������ ������� ������ �����

bool isUnaryMinus(char* equation, int minus_index);//���� �����: ����� ������ �� ���� ������� �� �����, ���� �����: ����� ��� �� ����� ����� �� ��� ���� ���� ���� �����
