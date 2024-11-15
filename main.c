#define _CRT_SECURE_NO_WARNINGS //מאקרו המאפשר שימוש בפונקציות לא בטוחות
#include "math_parser.h" 
#include "tokenization.h"
#include "stack.h"
#include "numeric_operations.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

void poppingFromStack(stack* st1, tokens currentToken, tokens* postFix, int* postFixCount)//טענת כניסה: קריאה לפעולה עם מצביע למחסנית, טוקן נוכחי, מערך טוקנים, אינדקס מערך טוקנים, טענת יציאה: הוצאת כל הטוקנים שרמת העדיפות שלהם גבוה מהטוקן הנוכחי, הכנסתם למערך הסופי והכנסת הטוקן הנוכחי למחסנית
{
	tokens signToken;//טוקן במחסנית
	while (peek(st1, &signToken) && hasHigherOrEqualPrecedence(signToken, currentToken))//כל עוד המחסנית לא ריקה והטוקן במחסנית בעל רמת עדיפות שווה או גבוה מהטוקן הנוכחי
	{
		if (!pop(st1, &signToken))//הוצאת הטוקן מהמחסנית
		{
			printf("Cannot pop from an empty stack\n");//אם המחסנית ריקה התוכנה תופסק
			free(st1->elements);
			exit(ERROR_CODE);
		}
		postFix[*postFixCount] = signToken;//הכנסת הטוקן שהוצא מהמחסנית למערך הסופי
		(*postFixCount)++;//העלאת אינדקס המערך הסופי
	}
}

int parser(tokens* postFix, int postFixLength)//טענת כניסה: קריאה לפעולה עם מערך סופי וגודל מערך, טענת יציאה: החזרת ערך של המשוואה
{
	stack numbers;//יצירת מחסנית למספרים
	double num1 = 0, num2 = 0;
	numbers.elements = malloc(postFixLength * sizeof(tokens));//הקצבת זיכרון למערך המחסנית
	if (numbers.elements == NULL)//אם יש בעיה בהקצבת הזיכרון
	{
		printf("Memory allocation failed\n");//תוחזר שגיאה
		return 1;
	}
	numbers.capacity = postFixLength;
	numbers.top = -1;
	tokens currentToken;
	for (int i = 0; i < postFixLength; i++)//מעבר על המערך הסופי וחישוב הערך
	{
		if (postFix[i].number)//אם הטוקן הנוכחי הוא מספר
		{
			if (!push(postFix[i], &numbers))//הוא ידחף למחסנית
			{
				printf("The stack is full\n");
				free(numbers.elements);
				return -1;
			}

		}
		else
		{
			if (!pop(&numbers, &currentToken))//אם הטוקן הנוכחי הוא סימן
			{
				printf("The stack is empty\n");
				free(numbers.elements);
				return -1;
			}
			num2 = currentToken.value;

			if (!pop(&numbers, &currentToken))//שני המספרים האחרונים יצאו מהמחסנית
			{
				printf("The stack is empty\n");//אם אין תוחזר שגיאה
				free(numbers.elements);
				return -1;
			}
			num1 = currentToken.value;

			switch (postFix[i].sign)
			{
			case '+':
				num1 += num2;
				break;
			case '-':
				num1 -= num2;
				break;
			case '*':
				num1 *= num2;
				break;
			case '/':
				num1 /= num2;
				break;
			default:
				printf("Invalid equation\n");
				free(numbers.elements);
				return 1;
			}
			tokens resultToken;
			resultToken.value = num1;
			resultToken.sign = 0;
			resultToken.number = true;
			if (!push(resultToken, &numbers))//דחיפת טוקן התוצאה למחסנית
			{
				printf("The stack is full\n");
				free(numbers.elements);
				return -1;
			}

		}
	}
	if (!pop(&numbers, &currentToken))//הוצאת טוקן התוצאה מהמחסנית
	{
		printf("Invalid equation\n");//אם המחסנית ריקה תוחזר שגיאה
		free(numbers.elements);
		return 1;
	}
	free(numbers.elements);
	return currentToken.value;//החזרת הערך הסופי

}

tokens* InfixToPostfix(tokens* infix, int equationLength, int* postFixCount)//טענת כניסה: קריאה לפעולה עם מערך טוקנים התחלתי, אורך המערך, ואורך מערך הטוקנים הסופי
{
	tokens* postFix = malloc((2 * equationLength) * sizeof(tokens));//הקצבת מקום למערך הטוקנים הסופי
	if (postFix == NULL)//אם יש שגיאה בהצבת המקום
	{
		printf("Failed to allocate memory for postfix array, exiting program...\n");//התוכנה תופסק
		exit(ERROR_CODE);
	}

	stack st1;//יצירת מבנה מחסנית
	st1.elements = malloc(equationLength * sizeof(tokens));//הקצבת מקום למחסנית
	if (st1.elements == NULL)//אם יש שגיאה בהצבת המקום
	{
		printf("Failed to allocate memory for stack, exiting program...\n");//התוכנה תופסק
		free(postFix);
		exit(ERROR_CODE);
	}
	st1.top = -1;//הוצאת האינדקס הנוכחי מהמחסנית כדי לסמן שהיא ריקה
	st1.capacity = equationLength;//אורך המחסנית היא בוגל אורך מערך הטוקנים ההתחלתי

	*postFixCount = 0;//איפוס אורך מערך הטוקנים הסופי
	tokens signToken;//יצירת טוקן של מחסנית
	for (int i = 0; i < equationLength; i++)
	{
		if (infix[i].value == NULL_TOKEN)//אם הטוקן הנוכחי הוא בעל ערך NULL_TOKEN 
		{
			continue;//הוא ידולג
		}

		if (infix[i].number)//אם הטוקן הנוכחי הוא מספר
		{
			postFix[*postFixCount] = infix[i];//הוא יתווסף ישר למערך הסופי
			(*postFixCount)++;
		}
		else
		{
			switch (infix[i].sign)//אם הטוקן הנוכחי הוא סימן
			{
			case '+':
			case '-':
			case '*':
			case '/':
				poppingFromStack(&st1, infix[i], postFix, postFixCount);//בדיקת אם הטוקן במחסנית בעל ערך עדיפות גדול או שווה לטוקן הנוכחי
				if (!push(infix[i], &st1))//דחיפת הטוקן הנוכחי
				{
					printf("Pushing another element will result in stack overflow\n");
					free(postFix);
					free(st1.elements);
					exit(ERROR_CODE);
				}
				break;
			case '('://אם הטוקן הנוכחי הוא (
				if (!push(infix[i], &st1))//הוא יידחף למחסנית
				{
					printf("Pushing another element will result in stack overflow\n");
					free(postFix);
					free(st1.elements);
					exit(ERROR_CODE);
				}
				break;
			case ')'://אם הטוקן הנוכחי הוא )
				while (peek(&st1, &signToken) && signToken.sign != '(')//כך עוד המחסנית לא ריקה והטוקן במחסנית לא (
				{
					if (!pop(&st1, &signToken))//הטוקן יוצא
					{
						printf("Error: Failed to pop from stack.\n");
						free(postFix);
						free(st1.elements);
						exit(ERROR_CODE);
					}
					postFix[(*postFixCount)++] = signToken;//ויוכנס למערך הסופי
				}
				if (st1.top == -1 || !pop(&st1, &signToken))//אם הגענו לתחילת המחסנית סימן ש ( לא נמצא
				{
					printf("Error with parenthesis\n");//תוחזר שגיאה של סוגריים
					free(postFix);
					free(st1.elements);
					exit(ERROR_CODE);
				}
				break;
			default://בכל מקרה אחר יוחזר שגיאה של תוקן לא חוקי
				printf("Invalid token\n");
				free(postFix);
				free(st1.elements);
				exit(ERROR_CODE);
			}
		}
	}

	tokens remainToken;//יצירת טוקן שנשאר במחסנית
	while (st1.top > -1)//הוצאת כל הטוקנים שנשארו במחסנית והכנסתם למערך הטוקנים הסופי
	{
		if (pop(&st1, &remainToken))//אם המחסנית לא ריקה
		{
			if (remainToken.sign == '(' || remainToken.sign == ')')//והתוקן שהוצא הוא סוגר
			{
				printf("Error with parenthesis\n");//תוחזר שגיאה של סוגריים
				free(postFix);
				free(st1.elements);
				exit(ERROR_CODE);
			}
			postFix[*postFixCount] = remainToken;//אחרת הטוקן ייכנס למערך הטוקנים הסופי
			(*postFixCount)++;
		}
	}
	free(st1.elements);//שחרור המחסנית
	return postFix;//החזרת מערך הטוקנים הסופי
}



char* CreateEquation(bool verbose)//טענת כניסה: קריאה לפעולה, טענת יציאה: יצירת מערך של משוואה
{
	char tempBuffer[100];//יצירת מערך זמני
	printf("Enter equation: ");
	if (fgets(tempBuffer, sizeof(tempBuffer), stdin) == NULL)//אם הייתה בעיה בקליטת קלט
	{
		printf("Input reading failed\n");//תוחזר שגיאה
		return NULL;
	}
	if (verbose)
	{
		printf("Input reading succeeded\n");
	}
	size_t len = strlen(tempBuffer);
	if (len > 0 && tempBuffer[len - 1] == '\n')//הפיכת הסימן newline
	{
		tempBuffer[len - 1] = '\0';//לסימן סוף מחרוזת
	}

	int size = strlen(tempBuffer);//שמירת גודל המחרוזת
	char* equation = (char*)malloc((size + 1) * sizeof(char));//יצירת מחרוזת משוואה בגודל אורך המחרוזת הזמנית
	if (equation == NULL)//אם הייתה בעיה בהקצבת זיכרון
	{
		printf("Memory allocation failed\n");//תוחזר שגיאה
		return NULL;
	}

	strcpy(equation, tempBuffer);//אחרת המחרוזת מהמערך הזמני תועתק למערך החדש
	if (verbose)
	{
		printf("Equation entered: %s\n", equation);//הדפסת המחרוזת שנקלטה
	}
	return equation;//החזרת המחרוזת
}
#include <stdio.h>

void printMenu() {
	printf("===================================\n");
	printf("       Welcome to Math Parser\n");
	printf("===================================\n");
	printf("Please select an option:\n\n");
	printf("  1. Exit the parser\n");
	printf("  2. Enter an equation\n\n");
	printf("===================================\n");
	printf("Input: ");
}
int Verbose(bool verbose)
{
	char command = '1';
	printMenu();
	scanf("%c", &command);
	if (command != '2')
	{
		return 0;
	}
	do
	{
		getc(stdin);
		char *equation = CreateEquation(verbose);//יצירת מערך משוואה
		if (equation == NULL)//אם הייתה בעיה בהקצבת זיכרון
		{
			printf("Failed to create equation.\n");//תוחזר שגיאה
			return -1;
		}
		if (verbose)
		{
			printf("Memory allocation succeeded for string\n");
		}
		int equationlength = strlen(equation);//שמירת אורך המחרוזת
		tokens* t1 = (tokens*)malloc(equationlength * sizeof(tokens));//יצירת מערך טוקנים בהתאם לאורך מערך המשוואה
		if (t1 == NULL)//אם הייתה בעיה בהקצבת הזיכרון
		{
			printf("Memory allocation failed\n");//תוחזר שגיאה
			free(equation);
			return -1;
		}
		if (verbose)
		{
			printf("Memory allocation succeeded for tokens\n");
		}

		for (int i = 0; i < equationlength; i++)//איפוס מערך הטוקנים
		{
			t1[i].value = NULL_TOKEN;
			t1[i].sign = 0;
			t1[i].number = false;
		}

		int index = 0;//אינדקס התחלתי
		int answer = Tokenize(t1, equation, &index, equationlength);//מילוי מערך הטוקנים

		if (answer != 0)//אם הוחזר קוד שגיאה
		{
			printf("One of the tokens is invalid.\n");//תוחזר שגיאה
			free(equation);
			free(t1);
			return -1;
		}
		if (verbose)
		{
			printf("Successfully tokenized the string\n");
		}

		int postFixCount;//יצירת אינדקס מערך סופי
		tokens* postFix = InfixToPostfix(t1, equationlength, &postFixCount);//מילוי המערך הסופי
		if (verbose)
		{
			printf("Postfix notation: ");
			for (int i = 0; i < postFixCount; i++)//הדפסת המערך הסופי
			{
				if (postFix[i].number)
				{
					printf("%d", postFix[i].value);
				}
				else
				{
					printf("%c", postFix[i].sign);
				}
			}
			printf("\n");
		}
		int value = parser(postFix, postFixCount);
		if (value != 1)//אם מהפעולה הוחזר -1
		{
			printf("Result: %d\n", value);//תוחזר שגיאה
		}
		free(equation);
		free(t1);
		free(postFix);
		printf("Input: ");
		scanf("%c", &command);
	} while (command != '2');
	return 0;
}
int main(int argc, char *argv[])
{
	if (argc > 1 && strcmp(argv[1], "-v") == 0)
	{
		return Verbose(true);
	}
	return Verbose(false);
	
}
