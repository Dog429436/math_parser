#define _CRT_SECURE_NO_WARNINGS //מקראו שמודיע לקומפיילר לדלג על אזהרות בנוגע לפונקציות לא בטוחות
#include <stdio.h>//הוספת ספריית פונקציות של קלט פלט
#include <stdlib.h>//הוספת ספריית פונקציות סטנדרטיות
#include "math_parser.h"//הוספת header file 
#include <string.h>//הוספת ספריית פונקציות של מחרוזות
#include <stdbool.h>//הוספת ספריית שימוש באמת או שקר
#define NULL_TOKEN -9999//מאקרו שיכניס בכל שימוש -999
typedef enum sings { add = 1, sub, mult, divide }operands;
typedef struct mathtoken//יצירת מבנה טוקן ומתן שם קיצור
{
	int value;//אם זה מספר אז ישומר האורך
	char sign;//אם זה סימן אז ישומר הסימן המתמטי
	bool number;//אמת או שקר אם יש מספר
}tokens;

static int FindNumbers(int *index, char * equation)//טענת כניסה: קבלת מצביע לאינדקס ומצביע למערך, טענת יציאה: החזרת המספר המורכב מרצף ספרות במחרוזת ועדכון ערך המוצבע במיקום הנוכחי במערך לצורך דילוג של יצירת טוקן לכל ספרה
{
	int num = 0;//יצירת משתנה שישמור את המספר הסופי
	int i = *index;//יצירת משתנה המכיל את הערך שהמצביע מצביע עליו
	int length = strlen(equation);//יצירת משתנה המכיל את אורך המחרוזת למקרה שכל המחרוזת היא מספר
	while (i < length && equation[i] >= '0' && equation[i] <= '9')//כל עוד האינדקס קטן מסוף המחרוזת והתו באות הנכוחית זה ספרה
	{
		num = num * 10 + equation[i] - '0';//המספר יורכב מהספרה הנוכחית בבסיס 10
		i++;//העלאת המציין הנוכחי כדי שבפעולה של הטוקנים יצרו טוקן אחד לכל המספר ולא טוקן לכל ספרה בנפרד
	}
	*index = i;//עדכון המצביע עם הערך החדש
	return num;//החזרת המספר המורכב מרצף הספרות
}
bool isUnaryMinus(char* equation, int minus_index)//טענת כניסה: הכנסת מצביע למחרוזת ואינדקס לפני הנוכחי כאשר התו באינדקס הנוכחי הוא מספר לצורך בדיקת הסימן המגיע לפני המספר, טענת יציאה: החזרת אמת או שקר האם מספר במיקום הנוכחי הוא מספר שלילי
{
	if (minus_index == 0)//אם אינדקס לפני הנוכחי הוא 0 אז זה מספר שלילי
	{
		return true;//החזרת אמת
	}

	int prev_index = minus_index - 1;//שמירת האינדקס שלפני האינדקס של הסימן
	while (prev_index >= 0 && (equation[prev_index] == ' ' || equation[prev_index] == '\t'))//כל עוד האינדקס לא בהתחלת המערך והתו במיקום האינדקס הוא רווח
	{
		prev_index--;//האינדקס ירד
	}
	if (prev_index < 0)//אם המציין הוא לפני תחילת המערך זה מספר שלילי
	{
		return true;//החזרת אמת
	}
	char prev_char = equation[prev_index];//שמירת התו לפני מינוס
	if (prev_char == '(' || prev_char == '+' || prev_char == '-' ||//אם התו של מינוס בא אחרי אחד מהסימנים זה מספר שלילי
		prev_char == '*' || prev_char == '/')
	{
		return true;//החזרת אמת
	}
	return false;//החזרת שקר
}


static int Tokenize(tokens* tokenptr, char* equation, int* p_index, int equationlength)//טענת כניסה: הכנסת מערך טוקנים ריק, מחרוזת, מצביע לאינדקס נוכחי ואורך המחרוזת, טענת יציאה: מילוי מערך הטוקנים בהתאם לתווים במחרוזת
{
	int index = *p_index;//משתנה מציין נוכחי ששווה לערך שהמצביע מצביע עליו

	while (index < equationlength && (equation[index] == ' ' || equation[index] == '\t'))//כל עוד האינדקס לפני סוף המערך והתו בתא הנוכחי הוא רווח
	{

		tokenptr[index].value = NULL_TOKEN;//הטוקן יהיה מספר בעל מאקרו שיגרום להתעלמות כדי לא להחשיב רווח כטוקן
		tokenptr[index].sign = 0;
		tokenptr[index].number = false;

		index++;//העלאת המציין הנוכחי בהתאם לכמות הרווחים שנמצאו
	}

	*p_index = index; //עדכון המצביע לאינדקס הנוכחי בערך האינדקס הנוכחי החדש לצורך דילוג על הרווחים

	if (index < equationlength)//כל עוד האינדקס לפניי סוף המערך
	{
		tokens t1;//יווצר מבנה של טוקן

		if (equation[index] == '-' && isUnaryMinus(equation, index))//אם התו במיקום הנוכחי במחרוזת הוא מינוס והוחזר אמת מבדיקת המינוס
		{
			index++;//עדכון האינדקס הנוכחי לצורך בדיקת רצף המספרים
			int start_index = index;//יצירת משתנה מציין המתחלתי בעל המציין הנוכחי
			int num = FindNumbers(&index, equation);//החזרת המספר בהתאם רצף תווי המספרים מהמציין הנוכחי

			if (start_index == index)//אם המציין ההתחלתי שווה למציין שעודכן מהפעולה סימן שאחרי הסימן - יש סימן אחר וזה לא חוקי
			{
				printf("Input is invalid\n");//החזרת הודעה שקלט לא חוקי
				return -1;//יציאה מהפעולה
			}

			t1.value = -num;//אם המספר חוקי אז הטוקן יהיה מספר בעל הערך של המספר כשלילי
			t1.sign = 0;//סימן 0 אומר שזה לא סימן
			t1.number = true;//אמת לשדה האם זה מספר
			tokenptr[*p_index] = t1; // שמירת הטוקן הנוכחי במערך הטוקנים במציין הנוכחי

			for (int i = start_index; i < index; i++)//התעלמות מכל הטוקנים שהפכו למספר אחד בכך שעוברים מהמציין ההתחלתי עד הסופי של רצף המיספרים והפיכת כל מספר שהפרך לטוקן אחד לטוקן של התעלמות
			{
				tokenptr[i].value = NULL_TOKEN;
				tokenptr[i].sign = 0;
				tokenptr[i].number = true;
			}

			*p_index = index;//עדכון המצביע למציין הנוכחי בערך הנוכחי
		}
		else if (equation[index] >= '0' && equation[index] <= '9')//אם התו במיקום הנוכחי במחרוזת זה מספר
		{
			int start_index = index;//יצירת משתנה ששומר את המיקום הנוכחי במחרוזת
			int num = FindNumbers(&index, equation);//מציאת רצף המיספרים בבסיס 10 והחזרתו ועדכון המציין הנוכחי בהתאם לרצף

			t1.value = num;//יצירת טוקן של מספר ושמירת רצף המספרים כמספר אחד בבסיס 10
			t1.sign = 0;
			t1.number = true;
			tokenptr[start_index] = t1;

			for (int i = start_index + 1; i < index; i++)//התעלמות מכל הספרות שהפכו לטוקן של מספר אחד
			{
				tokenptr[i].value = NULL_TOKEN;
				tokenptr[i].sign = 0;
				tokenptr[i].number = true;
			}

			*p_index = index;//עדכון המציין הנוכחי כך שסדרת ספרות תהפוך למספר אחד ושהמחרוזת תמשיך מהמיקום של הספרה האחרונה
		}
		else if (equation[index] == '+' || equation[index] == '-' ||//אם התו במציין הנוכחי במחרוזת הוא סימן
			equation[index] == '*' || equation[index] == '/' ||
			equation[index] == '(' || equation[index] == ')')
		{
			t1.value = 0;//הוא יישמר כטוקן של מימן
			t1.sign = equation[index];//הסימן יהיה התו במיקום הנוכחי
			t1.number = false;//מכיוון שזה סימן ולא ספרה יסומן שקר על האם זה מספר
			tokenptr[index] = t1;

			index++;//עדכון האינדקס
			*p_index = index;//עדכון המצביע לאינדקס הנוכחי עם האינדקס המעודכן
		}
		else
		{
			printf("Input is invalid\n");//אם זה לא מינוס , מספר או סימן אז הקלט לא חוקי ולכן יוחזר הודעת שגיאה
			return -1; //שליחת קוד של שגיאה
		}

		return Tokenize(tokenptr, equation, p_index, equationlength);//הפעולה ריקורסיבית ולכן תתבצע קריאה נוספת עם הפרמטרים המעודכנים
	}

	return 0; //במידה ויש יציאה מהפעולה יוחזר קוד של הצלחת הפעולה
}




char* CreateEquation()//טענת כניסה: קריאה לפעולה, טענת יציאה: החזרת מצביע למערך בעל אורך מדוייק של התווים שנכנסו בו תוך שימוש בהקצאת זיכרון
{
	char tempBuffer[100];//יצירת מערך זמני שישמור את המחרוזת שתיקלט
	printf("Enter equation: \n");
	if (fgets(tempBuffer, sizeof(tempBuffer), stdin) == NULL)//שימוש בפונקציית קלט המאפשרת קליטת רווחים ובדיקת אם הערך במיקום המערך הוא null
	{
		printf("Input reading failed\n");//אם כן תוחזר הודעת שגיאה
		return NULL;//יוחזר סימן null
	}
	printf("Input reading succeeded\n");//אחרת תודפס הודעת הצלחה

	size_t len = strlen(tempBuffer);//שימירת אורך המחרוזת שנקלטה במשתנה מסוג מספר ללא סימן מכיוון שגודל מערך לא יכול להיות שלילי
	if (len > 0 && tempBuffer[len - 1] == '\n')//אם אורך הקלט גדול מ0 והתו במיקום האחרון בקלט הוא סימן של ירידת שורה
	{
		tempBuffer[len - 1] = '\0';//השמת סימן סיום המחרוזת במיקום של סימן הורדת השורה
	}

	int size = strlen(tempBuffer);//שמירת ערך המחרוזת הזמנית במשתנה מספר
	char* equation = (char*)malloc((size + 1) * sizeof(char));//יצירת מצביע למחרוזת בעל מיקום בזיכרון בגודל המחרוזת הזמנית ועוד 1
	if (equation == NULL)//אם המצביע מצביע על null 
	{
		printf("Memory allocation failed\n");//לא ניתן להקדיש מקום בזיכון אז תודפס הודעת שגיאה
		return NULL;//החזרת null
	}

	strcpy(equation, tempBuffer);//העתקת המחרוזת שנקלטה למחרוזת הזמנית למחרוזת שתישמר
	printf("Equation entered: %s\n", equation);//התפסת המחרוזת
	return equation;//החזרת המצביע למחרוזת
}

int main()//פעולה ראשית שתרוץ בתחילת התוכנית
{
	char* equation = CreateEquation();//יצירת מצביע למחרוזת שיצביע על אותו מיקום שמצביע המצביע בפעולה
	if (equation == NULL)//אם המצביע מצביע על null
	{
		printf("Failed to create equation.\n");//הייתה בעייה בהקדשת המיקום בזיכרון אז תודפס הודעת שגיאה 
		return -1;//החזרת קוד שגיאה ויציאה מהתוכנית
	}
	printf("Memory allocation succeeded for string\n");//הדפסת הודעת הצלחה

	int equationlength = strlen(equation);//שמירת אורך המערך במשתנה מספר
	tokens* t1 = (tokens*)malloc(equationlength * sizeof(tokens));//יצירת מערך טוקנים באורך המחרוזת
	if (t1 == NULL)//אם המצביע מצביע על null
	{
		printf("Memory allocation failed\n");//הייתה בעייה בהקדשת המיקום בזיכרון אז תודפס הודעת שגיאה
		free(equation);//שחרור המיקום שתפס המחרוזת מכיוון שלא ניתן להמשיך
		return -1;//החזרת קוד שגיאה ויציאה מהתוכנית
	}
	printf("Memory allocation succeeded for tokens\n");//הדפסת הודעת הצלחה

	for (int i = 0; i < equationlength; i++)//הפיכת כל טוקן במערך הטוקנים לטוקן התעלמות
	{
		t1[i].value = NULL_TOKEN;
		t1[i].sign = 0;
		t1[i].number = false;
	}

	int index = 0;//יצירת משתנה של מצביע נוכחי
	int answer = Tokenize(t1, equation, &index, equationlength);//מילוי המערך בטוקנים ושמירת קוד הביצוע במשתנה

	if (answer != 0)//אם הוחזר מספר שלא 0
	{
		printf("One of the tokens is invalid.\n");//סימן שהייתה בעייה באחד מהתווים ולכן תודפס הודעת שגיאה
		free(equation);//שחרור המקום שהמחרוזת תפסה בזיכרון
		free(t1);//שחרור המקום שמערך הטוקנים תפס בזיכרון
		return -1;//החזרת קוד שגיאה ויציאה מהתוכנית
	}
	printf("Successfully tokenized the string\n");//הדפסת הודעת הצלחה

	for (int i = 0; i < equationlength; i++)//הדפסת מערך הטוקנים
	{
		if (t1[i].value == NULL_TOKEN)//התעלמות מהטוקן הנוכחי
		{
			continue;
		}
		else
		{
			if (t1[i].number)
			{
				printf("Token: %d, value: %d\n", i, t1[i].value);
			}
			else
			{
				printf("Token: %d, sign: %c\n", i, t1[i].sign);
			}
		}
	}

	free(equation);//שחרור הזיכרון ששויך למחרוזת
	free(t1);//שחרור הזיכרון ששויך למערך הטוקנים
	return 0;//החזרת קוד הצלחה ויציאה מהתוכנית
}







