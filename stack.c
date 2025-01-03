#include <stdbool.h>
#include "math_parser.h"
#include "stack.h"


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
