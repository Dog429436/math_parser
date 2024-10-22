#pragma once

extern char *equation;
extern int size;
typedef enum {//יצירת רשימת בחירה של טוקנים לפי מספר, סימן, סוגריים ולא חוקי
	TOKEN_NUMBER,
	TOKEN_OPERATOR,
	TOKEN_PARENTHESIS,
	TOKEN_INVALID
} TokenType;

typedef struct {//מבנה טוקנים
	TokenType type;
	char* value;
} Token;
