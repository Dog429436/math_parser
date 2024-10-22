#pragma once
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

bool push(tokens token, stack* st1);//מטרת כניסה: קבלת טוקן ומצביע למחסנית, טענת יציאה: דחיפת הטוקן למחסנית והחזרת אמת או שקר אם הפעולה הצליחה

bool pop(stack* st1, tokens* t1);//מטרת כניסה: קבלת מצביע למחסנית ומצביע לטוקן, טענת יציאה: החזרת הטוקן בראש המחסנית והחזרת אמת או שקר אם הפעולה הצליחה

bool peek(stack* st1, tokens* t1);//מטרת כניסה: קבלת מצביע למחסנית ומצביע לטוקן, טענת יציאה: החזרת הטוקן הנוכחי בלי להוציאו מהמחסנית והחזרת אמת או שקר אם הפעולה הצליחה

int parser(tokens* postFix, int postFixLength);//טענת כניסה: קריאה לפעולה עם מערך סופי וגודל מערך, טענת יציאה: החזרת ערך של המשוואה

int getPrecedence(char sign);//טענת כניסה: קריאה לפעולה והכנסת תו, טענת יציאה: החזרת ערך העדיפות שלו

bool hasHigherOrEqualPrecedence(tokens stackToken, tokens currentToken);//טענת כניסה: קריאה לפעולה עם טוקן נוכחי וטוקן במחסנית, טענת יציאה: החזרת אמת או שקר אם הטוקנים חוקיים ושהטוקן במחסנית בעל עדיפות גבוה או שווה לטוקן הנוכחי

void poppingFromStack(stack* st1, tokens currentToken, tokens* postFix, int* postFixCount);//טענת כניסה: קריאה לפעולה עם מצביע למחסנית, טוקן נוכחי, מערך טוקנים, אינדקס מערך טוקנים, טענת יציאה: הוצאת כל הטוקנים שרמת העדיפות שלהם גבוה מהטוקן הנוכחי, הכנסתם למערך הסופי והכנסת הטוקן הנוכחי למחסנית

tokens* InfixToPostfix(tokens* infix, int equationLength, int* postFixCount);//טענת כניסה: קריאה לפעולה עם מערך טוקנים התחלתי, אורך המערך, ואורך מערך הטוקנים הסופי

int FindNumbers(int* index, char* equation);//טענת כניסה: קריאה לפעולה עם מצביע לאינדקס ומערך, טענת יציאה: הפיכת רצף של טוקנים של ספרות למספר בבסיס 10 החזרתו ועידכון אינדקס המערך

bool isUnaryMinus(char* equation, int minus_index);//טענת כניסה: קריאה לפעולה עם מערך ואינדקס של מינוס, טענת יציאה: החזרת אמת אם המספר שלילי או הוא מספר לאחר סימן מינוס

static int Tokenize(tokens* tokenptr, char* equation, int* p_index, int equationlength); //טענת כניסה: קריאה לפעולה עם מערך טוקנים ריק, מערך של משוואה, מצביע לאינדקס המערך של המשוואה ואורך המערך, טענת יציאה, הפיכת על תו במערך לטוקן מתמטי

char* CreateEquation();//טענת כניסה: קריאה לפעולה, טענת יציאה: יצירת מערך של משוואה
