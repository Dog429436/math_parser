#pragma once

extern char *equation;
extern int size;
typedef enum {//����� ����� ����� �� ������ ��� ����, ����, ������� ��� ����
	TOKEN_NUMBER,
	TOKEN_OPERATOR,
	TOKEN_PARENTHESIS,
	TOKEN_INVALID
} TokenType;

typedef struct {//���� ������
	TokenType type;
	char* value;
} Token;
