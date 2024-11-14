#pragma once
#include "math_parser.h" 

int Tokenize(tokens* tokenptr, char* equation, int* p_index, int equationlength); //טענת כניסה: קריאה לפעולה עם מערך טוקנים ריק, מערך של משוואה, מצביע לאינדקס המערך של המשוואה ואורך המערך, טענת יציאה, הפיכת על תו במערך לטוקן מתמטי

int FindNumbers(int* index, char* equation);//טענת כניסה: קריאה לפעולה עם מצביע לאינדקס ומערך, טענת יציאה: הפיכת רצף של טוקנים של ספרות למספר בבסיס 10 החזרתו ועידכון אינדקס המערך

bool isUnaryMinus(char* equation, int minus_index);//טענת כניסה: קריאה לפעולה עם מערך ואינדקס של מינוס, טענת יציאה: החזרת אמת אם המספר שלילי או הוא מספר לאחר סימן מינוס
