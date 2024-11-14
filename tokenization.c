
#include "math_parser.h"
#include "tokenization.h"
#include <stdbool.h>

int Tokenize(tokens* tokenptr, char* equation, int* p_index, int equationlength) //טענת כניסה: קריאה לפעולה עם מערך טוקנים ריק, מערך של משוואה, מצביע לאינדקס המערך של המשוואה ואורך המערך, טענת יציאה, הפיכת על תו במערך לטוקן מתמטי
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

int FindNumbers(int* index, char* equation)//טענת כניסה: קריאה לפעולה עם מצביע לאינדקס ומערך, טענת יציאה: הפיכת רצף של טוקנים של ספרות למספר בבסיס 10 החזרתו ועידכון אינדקס המערך
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
