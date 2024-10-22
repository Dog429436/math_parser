#define _CRT_SECURE_NO_WARNINGS //מאקרו המאפשר שימוש בפונקציות לא בטוחות
#include <stdio.h>
#include <stdlib.h>
#include "math_parser.h" 
#include <string.h>
#include <stdbool.h>
#define NULL_TOKEN -9999 //מאקרו המסמל טוקנים שצריך להתעלם מהם
#define ERROR_CODE 1//מאקרו של קוד שגיאה


typedef struct mathtoken//מבנה ליצירת טוקן מתמטי
{
	int value;//משתנה ערך שמחזיק את ערך המספר
	char sign;//משתנה המחזיק את הסימן המתמטי
	bool number;//משתנה השומר אמת או שקר אם הטוקן הוא מספר
} tokens;


typedef struct stackForPostfix//מבנה ליצירת מחסנית
{
	tokens* elements;//מערך דינאמי המייצג את המחסנית
	int top;//משתנה של האינדקס הנוכחי במחסנית
	int capacity;//משתנה של אורך המחסנית
} stack;


bool push(tokens token, stack* st1)//מטרת כניסה: קבלת טוקן ומצביע למחסנית, טענת יציאה: דחיפת הטוקן למחסנית והחזרת אמת או שקר אם הפעולה הצליחה
{
	if (st1->top == st1->capacity - 1)//אם המחסנית מלאה
	{
		return false;//יוחזר שקר
	}
	st1->top++;//אחרת האינדקס הנוכחי יעלה
	st1->elements[st1->top] = token;//הטוקן ישמר בראש המחסנית
	return true;//החזרת אמת שהפעולה הצליחה
}


bool pop(stack* st1, tokens* t1)//מטרת כניסה: קבלת מצביע למחסנית ומצביע לטוקן, טענת יציאה: החזרת הטוקן בראש המחסנית והחזרת אמת או שקר אם הפעולה הצליחה
{
	if (st1->top < 0)//אם המחסנית ריקה
	{
		return false;//יוחזר שקר
	}
	*t1 = st1->elements[st1->top];//אחרת יוחזר הטוקן בראש המחסנית
	st1->top--;//האינדקס הנוכחי ירד
	return true;
}


bool peek(stack* st1, tokens* t1)//מטרת כניסה: קבלת מצביע למחסנית ומצביע לטוקן, טענת יציאה: החזרת הטוקן הנוכחי בלי להוציאו מהמחסנית והחזרת אמת או שקר אם הפעולה הצליחה
{
	if (st1->top < 0)//אם המחסנית ריקה
	{
		return false;//יוחזר שקר
	}
	*t1 = st1->elements[st1->top];//אחרת יוחזר הטוקן הנוכחי בלי להוציא אותו מהחמסנית
	return true;
}


int parser(tokens* postFix, int postFixLength)
{
	stack numbers;
	int num1 = 0, num2 = 0;
	numbers.elements = malloc(postFixLength * sizeof(tokens));
	if (numbers.elements == NULL)
	{
		printf("Memory allocation failed\n");
		return 1;
	}
	numbers.capacity = postFixLength;
	numbers.top = -1;
	tokens currentToken;
	for (int i = 0; i < postFixLength; i++)
	{
		if (postFix[i].number)
		{
			if (!push(postFix[i], &numbers))
			{
				printf("The stack is full\n");
				free(numbers.elements);
				return -1;
			}

		}
		else
		{
			if (!pop(&numbers, &currentToken))
			{
				printf("The stack is empty\n");
				free(numbers.elements);
				return -1;
			}
			num2 = currentToken.value;

			if (!pop(&numbers, &currentToken))
			{
				printf("The stack is empty\n");
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
			if (!push(resultToken, &numbers))
			{
				printf("The stack is full\n");
				free(numbers.elements);
				return -1;
			}

		}
	}
	if (!pop(&numbers, &currentToken))
	{
		printf("Invalid equation\n");
		free(numbers.elements);
		return 1;
	}
	free(numbers.elements);
	return currentToken.value;

}


int getPrecedence(char sign)//טענת כניסה: קריאה לפעולה והכנסת תו, טענת יציאה: החזרת ערך העדיפות שלו
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


bool hasHigherOrEqualPrecedence(tokens stackToken, tokens currentToken)//טענת כניסה: קריאה לפעולה עם טוקן נוכחי וטוקן במחסנית, טענת יציאה: החזרת אמת או שקר אם הטוקנים חוקיים ושהטוקן במחסנית בעל עדיפות גבוה או שווה לטוקן הנוכחי
{
	int precedenceStack = getPrecedence(stackToken.sign);//קריאת פעולת ערך עדיפות של הטוקן במחסנית
	int precedenceCurrent = getPrecedence(currentToken.sign);//קריאת פעולת ערך עדיפות של הטוקן הנוכחי 

	if (precedenceStack == -1 || precedenceCurrent == -1) //החזרת שקר אם שני הטוקנים לא חוקיים
	{
		return false;
	}
	return precedenceStack >= precedenceCurrent;//החזרת אמת או שקר אם רמת העדיפות של הטוקן במחסנית גבוה או שווה לרמת העדיפות של הטוקן הנוכחי
}


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


static int FindNumbers(int* index, char* equation)//טענת כניסה: קריאה לפעולה עם מצביע לאינדקס ומערך, טענת יציאה: הפיכת רצף של טוקנים של ספרות למספר בבסיס 10 החזרתו ועידכון אינדקס המערך
{
	int num = 0;
	int i = *index;
	int length = strlen(equation);
	while (i < length && equation[i] >= '0' && equation[i] <= '9')//כל עוד התו הנוכחי הוא מספר
	{
		num = num * 10 + equation[i] - '0';//יורכב מספר בבסיס 10
		i++;
	}
	*index = i;//עדכון אינדקס המערך
	return num;//החזרת המספר
}


bool isUnaryMinus(char* equation, int minus_index)//טענת כניסה: קריאה לפעולה עם מערך ואינדקס של מינוס, טענת יציאה: החזרת אמת אם המספר שלילי או הוא מספר לאחר סימן מינוס
{
	if (minus_index == 0)//אם המינוס הוא בתחילת המשוואה אז זה מספר שלילי
	{
		return true;
	}

	int prev_index = minus_index - 1;//השמת התו באינדקס שלפני האינדקס שניתן
	while (prev_index >= 0 && (equation[prev_index] == ' ' || equation[prev_index] == '\t'))//כל עוד האינדקס גדול מ0
	{
		prev_index--;//האינדקס ירד
	}
	if (prev_index < 0)//אם האינדקס יצא מהמערך
	{
		return true;//החזרת אמת
	}
	char prev_char = equation[prev_index];//אם מינוס בא לאחר סימן מתמטי אז זה מספר שלילי
	if (prev_char == '(' || prev_char == '+' || prev_char == '-' ||
		prev_char == '*' || prev_char == '/')
	{
		return true;
	}
	return false;//אחרת יוחזר שקר
}


static int Tokenize(tokens* tokenptr, char* equation, int* p_index, int equationlength) //טענת כניסה: קריאה לפעולה עם מערך טוקנים ריק, מערך של משוואה, מצביע לאינדקס המערך של המשוואה ואורך המערך, טענת יציאה, הפיכת על תו במערך לטוקן מתמטי
{
	int index = *p_index;//השמת האינדקס הנוכחי

	while (index < equationlength && (equation[index] == ' ' || equation[index] == '\t')) //כל עוד התו הנוכחי הוא רווח והאינדקס הנוכחי בתוך המערך הוא יעלה
	{
		index++;
	}

	*p_index = index;//עדכון המצביע של האינדקס הנוכחי

	if (index < equationlength) //אם האינדקס הנוכחי הוא בתוך המערך
	{
		tokens t1;//יצירת טוקן

		if (equation[index] == '-' && isUnaryMinus(equation, index)) //אם הטוקן הנוכחי הוא מספר שלילי
		{
			index++;//האינדקס יעלה
			int start_index = index;//השמת אינדקס התחלתי
			int num = FindNumbers(&index, equation);//קריאה לפעולה המחברת טוקנים של ספרות

			if (start_index == index) //אם האינדקס לא עלה בכלל סימן שיש סימן מינוס ריק
			{
				printf("Input is invalid\n");//תוחזר שגיאה
				return -1;
			}

			t1.value = -num;//ערך הטוקן יהיה המספר השלילי
			t1.sign = 0;//הסימן יהיה 0
			t1.number = true;
			tokenptr[*p_index] = t1;//הטוקן יהיה באינדקס הנוכחי במערך הטוקנים

			for (int i = start_index; i < index; i++) //השמת כל הטוקנים במקומות שהיו בהם ספרות שהפכו למספר אחד לטוקנים שדורשים התעלמות
			{
				tokenptr[i].value = NULL_TOKEN;
				tokenptr[i].sign = 0;
				tokenptr[i].number = false;
			}

			*p_index = index;//עדכון המצביע לאינדקס הנוכחי
		}
		else if (equation[index] >= '0' && equation[index] <= '9') //אם התו הנוכחי הוא מספר
		{
			int start_index = index;//השמת האינדקס הנוכחי
			int num = FindNumbers(&index, equation);//קריאה לפעולה לחיבור טוקנים של ספרות למספר אחד

			t1.value = num;
			t1.sign = 0;
			t1.number = true;
			tokenptr[start_index] = t1;

			for (int i = start_index + 1; i < index; i++) //הפיכת כל הטוקנים שבהם היו ספרות שהפכו למספר אחד לטוקנים שדורשים התעלמות
			{
				tokenptr[i].value = NULL_TOKEN;
				tokenptr[i].sign = 0;
				tokenptr[i].number = false;
			}

			*p_index = index;
		}
		else if (equation[index] == '+' || equation[index] == '-' ||
			equation[index] == '*' || equation[index] == '/' ||
			equation[index] == '(' || equation[index] == ')') //אם הטוקן הנוכחי הוא אחד מהסימנים הבאים
		{
			t1.value = 0;//הערך יהיה 0
			t1.sign = equation[index];//הסימן יהיה התו הנוכחי
			t1.number = false;
			tokenptr[index] = t1;

			index++;
			*p_index = index;
		}
		else {
			printf("Input is invalid\n");
			return -1;
		}

		return Tokenize(tokenptr, equation, p_index, equationlength);//החזרת מערך הטוקנים
	}

	return 0;
}


char* CreateEquation()//טענת כניסה: קריאה לפעולה, טענת יציאה: יצירת מערך של משוואה
{
	char tempBuffer[100];//יצירת מערך זמני
	printf("Enter equation: \n");
	if (fgets(tempBuffer, sizeof(tempBuffer), stdin) == NULL)//אם הייתה בעיה בקליטת קלט
	{
		printf("Input reading failed\n");//תוחזר שגיאה
		return NULL;
	}
	printf("Input reading succeeded\n");

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
	printf("Equation entered: %s\n", equation);//הדפסת המחרוזת שנקלטה
	return equation;//החזרת המחרוזת
}


int main()
{
	char* equation = CreateEquation();//יצירת מערך משוואה
	if (equation == NULL)//אם הייתה בעיה בהקצבת זיכרון
	{
		printf("Failed to create equation.\n");//תוחזר שגיאה
		return -1;
	}
	printf("Memory allocation succeeded for string\n");

	int equationlength = strlen(equation);//שמירת אורך המחרוזת
	tokens* t1 = (tokens*)malloc(equationlength * sizeof(tokens));//יצירת מערך טוקנים בהתאם לאורך מערך המשוואה
	if (t1 == NULL)//אם הייתה בעיה בהקצבת הזיכרון
	{
		printf("Memory allocation failed\n");//תוחזר שגיאה
		free(equation);
		return -1;
	}
	printf("Memory allocation succeeded for tokens\n");

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
	printf("Successfully tokenized the string\n");

	int postFixCount;//יצירת אינדקס מערך סופי
	tokens* postFix = InfixToPostfix(t1, equationlength, &postFixCount);//מילוי המערך הסופי

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
	int value = parser(postFix, postFixCount);
	if (value != 1)
	{
		printf("Value: %d\n", value);
	}
	free(equation);
	free(t1);
	free(postFix);
	return 0;
}
