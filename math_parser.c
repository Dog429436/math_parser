#define _CRT_SECURE_NO_WARNINGS //î÷øàå ùîåãéò ì÷åîôééìø ìãìâ òì àæäøåú áðåâò ìôåð÷öéåú ìà áèåçåú
#include <stdio.h>//äåñôú ñôøééú ôåð÷öéåú ùì ÷ìè ôìè
#include <stdlib.h>//äåñôú ñôøééú ôåð÷öéåú ñèðãøèéåú
#include "math_parser.h"//äåñôú header file 
#include <string.h>//äåñôú ñôøééú ôåð÷öéåú ùì îçøåæåú
#include <stdbool.h>//äåñôú ñôøééú ùéîåù áàîú àå ù÷ø
#define NULL_TOKEN -9999//îà÷øå ùéëðéñ áëì ùéîåù -999
typedef enum sings { add = 1, sub, mult, divide }operands;
typedef struct mathtoken//éöéøú îáðä èå÷ï åîúï ùí ÷éöåø
{
	int value;//àí æä îñôø àæ éùåîø äàåøê
	char sign;//àí æä ñéîï àæ éùåîø äñéîï äîúîèé
	bool number;//àîú àå ù÷ø àí éù îñôø
}tokens;

static int FindNumbers(int *index, char * equation)//èòðú ëðéñä: ÷áìú îöáéò ìàéðã÷ñ åîöáéò ìîòøê, èòðú éöéàä: äçæøú äîñôø äîåøëá îøöó ñôøåú áîçøåæú åòãëåï òøê äîåöáò áîé÷åí äðåëçé áîòøê ìöåøê ãéìåâ ùì éöéøú èå÷ï ìëì ñôøä
{
	int num = 0;//éöéøú îùúðä ùéùîåø àú äîñôø äñåôé
	int i = *index;//éöéøú îùúðä äîëéì àú äòøê ùäîöáéò îöáéò òìéå
	int length = strlen(equation);//éöéøú îùúðä äîëéì àú àåøê äîçøåæú ìî÷øä ùëì äîçøåæú äéà îñôø
	while (i < length && equation[i] >= '0' && equation[i] <= '9')//ëì òåã äàéðã÷ñ ÷èï îñåó äîçøåæú åäúå áàåú äðëåçéú æä ñôøä
	{
		num = num * 10 + equation[i] - '0';//äîñôø éåøëá îäñôøä äðåëçéú ááñéñ 10
		i++;//äòìàú äîöééï äðåëçé ëãé ùáôòåìä ùì äèå÷ðéí éöøå èå÷ï àçã ìëì äîñôø åìà èå÷ï ìëì ñôøä áðôøã
	}
	*index = i;//òãëåï äîöáéò òí äòøê äçãù
	return num;//äçæøú äîñôø äîåøëá îøöó äñôøåú
}
bool isUnaryMinus(char* equation, int minus_index)//èòðú ëðéñä: äëðñú îöáéò ìîçøåæú åàéðã÷ñ ìôðé äðåëçé ëàùø äúå áàéðã÷ñ äðåëçé äåà îñôø ìöåøê áãé÷ú äñéîï äîâéò ìôðé äîñôø, èòðú éöéàä: äçæøú àîú àå ù÷ø äàí îñôø áîé÷åí äðåëçé äåà îñôø ùìéìé
{
	if (minus_index == 0)//àí àéðã÷ñ ìôðé äðåëçé äåà 0 àæ æä îñôø ùìéìé
	{
		return true;//äçæøú àîú
	}

	int prev_index = minus_index - 1;//ùîéøú äàéðã÷ñ ùìôðé äàéðã÷ñ ùì äñéîï
	while (prev_index >= 0 && (equation[prev_index] == ' ' || equation[prev_index] == '\t'))//ëì òåã äàéðã÷ñ ìà áäúçìú äîòøê åäúå áîé÷åí äàéðã÷ñ äåà øååç
	{
		prev_index--;//äàéðã÷ñ éøã
	}
	if (prev_index < 0)//àí äîöééï äåà ìôðé úçéìú äîòøê æä îñôø ùìéìé
	{
		return true;//äçæøú àîú
	}
	char prev_char = equation[prev_index];//ùîéøú äúå ìôðé îéðåñ
	if (prev_char == '(' || prev_char == '+' || prev_char == '-' ||//àí äúå ùì îéðåñ áà àçøé àçã îäñéîðéí æä îñôø ùìéìé
		prev_char == '*' || prev_char == '/')
	{
		return true;//äçæøú àîú
	}
	return false;//äçæøú ù÷ø
}


static int Tokenize(tokens* tokenptr, char* equation, int* p_index, int equationlength)//èòðú ëðéñä: äëðñú îòøê èå÷ðéí øé÷, îçøåæú, îöáéò ìàéðã÷ñ ðåëçé åàåøê äîçøåæú, èòðú éöéàä: îéìåé îòøê äèå÷ðéí áäúàí ìúååéí áîçøåæú
{
	int index = *p_index;//îùúðä îöééï ðåëçé ùùååä ìòøê ùäîöáéò îöáéò òìéå

	while (index < equationlength && (equation[index] == ' ' || equation[index] == '\t'))//ëì òåã äàéðã÷ñ ìôðé ñåó äîòøê åäúå áúà äðåëçé äåà øååç
	{

		tokenptr[index].value = NULL_TOKEN;//äèå÷ï éäéä îñôø áòì îà÷øå ùéâøåí ìäúòìîåú ëãé ìà ìäçùéá øååç ëèå÷ï
		tokenptr[index].sign = 0;
		tokenptr[index].number = false;

		index++;//äòìàú äîöééï äðåëçé áäúàí ìëîåú äøååçéí ùðîöàå
	}

	*p_index = index; //òãëåï äîöáéò ìàéðã÷ñ äðåëçé áòøê äàéðã÷ñ äðåëçé äçãù ìöåøê ãéìåâ òì äøååçéí

	if (index < equationlength)//ëì òåã äàéðã÷ñ ìôðéé ñåó äîòøê
	{
		tokens t1;//éååöø îáðä ùì èå÷ï

		if (equation[index] == '-' && isUnaryMinus(equation, index))//àí äúå áîé÷åí äðåëçé áîçøåæú äåà îéðåñ åäåçæø àîú îáãé÷ú äîéðåñ
		{
			index++;//òãëåï äàéðã÷ñ äðåëçé ìöåøê áãé÷ú øöó äîñôøéí
			int start_index = index;//éöéøú îùúðä îöééï äîúçìúé áòì äîöééï äðåëçé
			int num = FindNumbers(&index, equation);//äçæøú äîñôø áäúàí øöó úååé äîñôøéí îäîöééï äðåëçé

			if (start_index == index)//àí äîöééï ääúçìúé ùååä ìîöééï ùòåãëï îäôòåìä ñéîï ùàçøé äñéîï - éù ñéîï àçø åæä ìà çå÷é
			{
				printf("Input is invalid\n");//äçæøú äåãòä ù÷ìè ìà çå÷é
				return -1;//éöéàä îäôòåìä
			}

			t1.value = -num;//àí äîñôø çå÷é àæ äèå÷ï éäéä îñôø áòì äòøê ùì äîñôø ëùìéìé
			t1.sign = 0;//ñéîï 0 àåîø ùæä ìà ñéîï
			t1.number = true;//àîú ìùãä äàí æä îñôø
			tokenptr[*p_index] = t1; // ùîéøú äèå÷ï äðåëçé áîòøê äèå÷ðéí áîöééï äðåëçé

			for (int i = start_index; i < index; i++)//äúòìîåú îëì äèå÷ðéí ùäôëå ìîñôø àçã áëê ùòåáøéí îäîöééï ääúçìúé òã äñåôé ùì øöó äîéñôøéí åäôéëú ëì îñôø ùäôøê ìèå÷ï àçã ìèå÷ï ùì äúòìîåú
			{
				tokenptr[i].value = NULL_TOKEN;
				tokenptr[i].sign = 0;
				tokenptr[i].number = true;
			}

			*p_index = index;//òãëåï äîöáéò ìîöééï äðåëçé áòøê äðåëçé
		}
		else if (equation[index] >= '0' && equation[index] <= '9')//àí äúå áîé÷åí äðåëçé áîçøåæú æä îñôø
		{
			int start_index = index;//éöéøú îùúðä ùùåîø àú äîé÷åí äðåëçé áîçøåæú
			int num = FindNumbers(&index, equation);//îöéàú øöó äîéñôøéí ááñéñ 10 åäçæøúå åòãëåï äîöééï äðåëçé áäúàí ìøöó

			t1.value = num;//éöéøú èå÷ï ùì îñôø åùîéøú øöó äîñôøéí ëîñôø àçã ááñéñ 10
			t1.sign = 0;
			t1.number = true;
			tokenptr[start_index] = t1;

			for (int i = start_index + 1; i < index; i++)//äúòìîåú îëì äñôøåú ùäôëå ìèå÷ï ùì îñôø àçã
			{
				tokenptr[i].value = NULL_TOKEN;
				tokenptr[i].sign = 0;
				tokenptr[i].number = true;
			}

			*p_index = index;//òãëåï äîöééï äðåëçé ëê ùñãøú ñôøåú úäôåê ìîñôø àçã åùäîçøåæú úîùéê îäîé÷åí ùì äñôøä äàçøåðä
		}
		else if (equation[index] == '+' || equation[index] == '-' ||//àí äúå áîöééï äðåëçé áîçøåæú äåà ñéîï
			equation[index] == '*' || equation[index] == '/' ||
			equation[index] == '(' || equation[index] == ')')
		{
			t1.value = 0;//äåà ééùîø ëèå÷ï ùì îéîï
			t1.sign = equation[index];//äñéîï éäéä äúå áîé÷åí äðåëçé
			t1.number = false;//îëéååï ùæä ñéîï åìà ñôøä éñåîï ù÷ø òì äàí æä îñôø
			tokenptr[index] = t1;

			index++;//òãëåï äàéðã÷ñ
			*p_index = index;//òãëåï äîöáéò ìàéðã÷ñ äðåëçé òí äàéðã÷ñ äîòåãëï
		}
		else
		{
			printf("Input is invalid\n");//àí æä ìà îéðåñ , îñôø àå ñéîï àæ ä÷ìè ìà çå÷é åìëï éåçæø äåãòú ùâéàä
			return -1; //ùìéçú ÷åã ùì ùâéàä
		}

		return Tokenize(tokenptr, equation, p_index, equationlength);//äôòåìä øé÷åøñéáéú åìëï úúáöò ÷øéàä ðåñôú òí äôøîèøéí äîòåãëðéí
	}

	return 0; //áîéãä åéù éöéàä îäôòåìä éåçæø ÷åã ùì äöìçú äôòåìä
}




char* CreateEquation()//èòðú ëðéñä: ÷øéàä ìôòåìä, èòðú éöéàä: äçæøú îöáéò ìîòøê áòì àåøê îãåéé÷ ùì äúååéí ùðëðñå áå úåê ùéîåù áä÷öàú æéëøåï
{
	char tempBuffer[100];//éöéøú îòøê æîðé ùéùîåø àú äîçøåæú ùúé÷ìè
	printf("Enter equation: \n");
	if (fgets(tempBuffer, sizeof(tempBuffer), stdin) == NULL)//ùéîåù áôåð÷öééú ÷ìè äîàôùøú ÷ìéèú øååçéí åáãé÷ú àí äòøê áîé÷åí äîòøê äåà null
	{
		printf("Input reading failed\n");//àí ëï úåçæø äåãòú ùâéàä
		return NULL;//éåçæø ñéîï null
	}
	printf("Input reading succeeded\n");//àçøú úåãôñ äåãòú äöìçä

	size_t len = strlen(tempBuffer);//ùéîéøú àåøê äîçøåæú ùð÷ìèä áîùúðä îñåâ îñôø ììà ñéîï îëéååï ùâåãì îòøê ìà éëåì ìäéåú ùìéìé
	if (len > 0 && tempBuffer[len - 1] == '\n')//àí àåøê ä÷ìè âãåì î0 åäúå áîé÷åí äàçøåï á÷ìè äåà ñéîï ùì éøéãú ùåøä
	{
		tempBuffer[len - 1] = '\0';//äùîú ñéîï ñéåí äîçøåæú áîé÷åí ùì ñéîï äåøãú äùåøä
	}

	int size = strlen(tempBuffer);//ùîéøú òøê äîçøåæú äæîðéú áîùúðä îñôø
	char* equation = (char*)malloc((size + 1) * sizeof(char));//éöéøú îöáéò ìîçøåæú áòì îé÷åí áæéëøåï áâåãì äîçøåæú äæîðéú åòåã 1
	if (equation == NULL)//àí äîöáéò îöáéò òì null 
	{
		printf("Memory allocation failed\n");//ìà ðéúï ìä÷ãéù î÷åí áæéëåï àæ úåãôñ äåãòú ùâéàä
		return NULL;//äçæøú null
	}

	strcpy(equation, tempBuffer);//äòú÷ú äîçøåæú ùð÷ìèä ìîçøåæú äæîðéú ìîçøåæú ùúéùîø
	printf("Equation entered: %s\n", equation);//äúôñú äîçøåæú
	return equation;//äçæøú äîöáéò ìîçøåæú
}

int main()//ôòåìä øàùéú ùúøåõ áúçéìú äúåëðéú
{
	char* equation = CreateEquation();//éöéøú îöáéò ìîçøåæú ùéöáéò òì àåúå îé÷åí ùîöáéò äîöáéò áôòåìä
	if (equation == NULL)//àí äîöáéò îöáéò òì null
	{
		printf("Failed to create equation.\n");//äééúä áòééä áä÷ãùú äîé÷åí áæéëøåï àæ úåãôñ äåãòú ùâéàä 
		return -1;//äçæøú ÷åã ùâéàä åéöéàä îäúåëðéú
	}
	printf("Memory allocation succeeded for string\n");//äãôñú äåãòú äöìçä

	int equationlength = strlen(equation);//ùîéøú àåøê äîòøê áîùúðä îñôø
	tokens* t1 = (tokens*)malloc(equationlength * sizeof(tokens));//éöéøú îòøê èå÷ðéí áàåøê äîçøåæú
	if (t1 == NULL)//àí äîöáéò îöáéò òì null
	{
		printf("Memory allocation failed\n");//äééúä áòééä áä÷ãùú äîé÷åí áæéëøåï àæ úåãôñ äåãòú ùâéàä
		free(equation);//ùçøåø äîé÷åí ùúôñ äîçøåæú îëéååï ùìà ðéúï ìäîùéê
		return -1;//äçæøú ÷åã ùâéàä åéöéàä îäúåëðéú
	}
	printf("Memory allocation succeeded for tokens\n");//äãôñú äåãòú äöìçä

	for (int i = 0; i < equationlength; i++)//äôéëú ëì èå÷ï áîòøê äèå÷ðéí ìèå÷ï äúòìîåú
	{
		t1[i].value = NULL_TOKEN;
		t1[i].sign = 0;
		t1[i].number = false;
	}

	int index = 0;//éöéøú îùúðä ùì îöáéò ðåëçé
	int answer = Tokenize(t1, equation, &index, equationlength);//îéìåé äîòøê áèå÷ðéí åùîéøú ÷åã äáéöåò áîùúðä

	if (answer != 0)//àí äåçæø îñôø ùìà 0
	{
		printf("One of the tokens is invalid.\n");//ñéîï ùäééúä áòééä áàçã îäúååéí åìëï úåãôñ äåãòú ùâéàä
		free(equation);//ùçøåø äî÷åí ùäîçøåæú úôñä áæéëøåï
		free(t1);//ùçøåø äî÷åí ùîòøê äèå÷ðéí úôñ áæéëøåï
		return -1;//äçæøú ÷åã ùâéàä åéöéàä îäúåëðéú
	}
	printf("Successfully tokenized the string\n");//äãôñú äåãòú äöìçä

	for (int i = 0; i < equationlength; i++)//äãôñú îòøê äèå÷ðéí
	{
		if (t1[i].value == NULL_TOKEN)//äúòìîåú îäèå÷ï äðåëçé
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

	free(equation);//ùçøåø äæéëøåï ùùåéê ìîçøåæú
	free(t1);//ùçøåø äæéëøåï ùùåéê ìîòøê äèå÷ðéí
	return 0;//äçæøú ÷åã äöìçä åéöéàä îäúåëðéú
}



