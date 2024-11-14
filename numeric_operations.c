#include "math_parser.h"
#include <stdio.h>
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


