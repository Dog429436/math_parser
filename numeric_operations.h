#pragma once
#include "math_parser.h"

int getPrecedence(char sign);//טענת כניסה: קריאה לפעולה והכנסת תו, טענת יציאה: החזרת ערך העדיפות שלו

bool hasHigherOrEqualPrecedence(tokens stackToken, tokens currentToken);//טענת כניסה: קריאה לפעולה עם טוקן נוכחי וטוקן במחסנית, טענת יציאה: החזרת אמת או שקר אם הטוקנים חוקיים ושהטוקן במחסנית בעל עדיפות גבוה או שווה לטוקן הנוכחי


