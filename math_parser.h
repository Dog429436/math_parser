#pragma once
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
