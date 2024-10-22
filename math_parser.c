#define _CRT_SECURE_NO_WARNINGS //����� ������ ����� ��������� �� ������
#include <stdio.h>
#include <stdlib.h>
#include "math_parser.h" 
#include <string.h>
#include <stdbool.h>
#define NULL_TOKEN -9999 //����� ����� ������ ����� ������ ���
#define ERROR_CODE 1//����� �� ��� �����


typedef struct mathtoken//���� ������ ���� �����
{
	int value;//����� ��� ������ �� ��� �����
	char sign;//����� ������ �� ����� ������
	bool number;//����� ����� ��� �� ��� �� ����� ��� ����
} tokens;


typedef struct stackForPostfix//���� ������ ������
{
	tokens* elements;//���� ������ ������ �� �������
	int top;//����� �� ������� ������ �������
	int capacity;//����� �� ���� �������
} stack;


bool push(tokens token, stack* st1)//���� �����: ���� ���� ������ �������, ���� �����: ����� ����� ������� ������ ��� �� ��� �� ������ ������
{
	if (st1->top == st1->capacity - 1)//�� ������� ����
	{
		return false;//����� ���
	}
	st1->top++;//���� ������� ������ ����
	st1->elements[st1->top] = token;//����� ���� ���� �������
	return true;//����� ��� ������� ������
}


bool pop(stack* st1, tokens* t1)//���� �����: ���� ����� ������� ������ �����, ���� �����: ����� ����� ���� ������� ������ ��� �� ��� �� ������ ������
{
	if (st1->top < 0)//�� ������� ����
	{
		return false;//����� ���
	}
	*t1 = st1->elements[st1->top];//���� ����� ����� ���� �������
	st1->top--;//������� ������ ���
	return true;
}


bool peek(stack* st1, tokens* t1)//���� �����: ���� ����� ������� ������ �����, ���� �����: ����� ����� ������ ��� ������� �������� ������ ��� �� ��� �� ������ ������
{
	if (st1->top < 0)//�� ������� ����
	{
		return false;//����� ���
	}
	*t1 = st1->elements[st1->top];//���� ����� ����� ������ ��� ������ ���� ��������
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


int getPrecedence(char sign)//���� �����: ����� ������ ������ ��, ���� �����: ����� ��� ������� ���
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


bool hasHigherOrEqualPrecedence(tokens stackToken, tokens currentToken)//���� �����: ����� ������ �� ���� ����� ����� �������, ���� �����: ����� ��� �� ��� �� ������� ������ ������� ������� ��� ������ ���� �� ���� ����� ������
{
	int precedenceStack = getPrecedence(stackToken.sign);//����� ����� ��� ������ �� ����� �������
	int precedenceCurrent = getPrecedence(currentToken.sign);//����� ����� ��� ������ �� ����� ������ 

	if (precedenceStack == -1 || precedenceCurrent == -1) //����� ��� �� ��� ������� �� ������
	{
		return false;
	}
	return precedenceStack >= precedenceCurrent;//����� ��� �� ��� �� ��� ������� �� ����� ������� ���� �� ���� ���� ������� �� ����� ������
}


void poppingFromStack(stack* st1, tokens currentToken, tokens* postFix, int* postFixCount)//���� �����: ����� ������ �� ����� �������, ���� �����, ���� ������, ������ ���� ������, ���� �����: ����� �� ������� ���� ������� ���� ���� ������ ������, ������ ����� ����� ������ ����� ������ �������
{
	tokens signToken;//���� �������
	while (peek(st1, &signToken) && hasHigherOrEqualPrecedence(signToken, currentToken))//�� ��� ������� �� ���� ������ ������� ��� ��� ������ ���� �� ���� ������ ������
	{
		if (!pop(st1, &signToken))//����� ����� ��������
		{
			printf("Cannot pop from an empty stack\n");//�� ������� ���� ������ �����
			free(st1->elements);
			exit(ERROR_CODE);
		}
		postFix[*postFixCount] = signToken;//����� ����� ����� �������� ����� �����
		(*postFixCount)++;//����� ������ ����� �����
	}
}


tokens* InfixToPostfix(tokens* infix, int equationLength, int* postFixCount)//���� �����: ����� ������ �� ���� ������ ������, ���� �����, ����� ���� ������� �����
{
	tokens* postFix = malloc((2 * equationLength) * sizeof(tokens));//����� ���� ����� ������� �����
	if (postFix == NULL)//�� �� ����� ����� �����
	{
		printf("Failed to allocate memory for postfix array, exiting program...\n");//������ �����
		exit(ERROR_CODE);
	}

	stack st1;//����� ���� ������
	st1.elements = malloc(equationLength * sizeof(tokens));//����� ���� �������
	if (st1.elements == NULL)//�� �� ����� ����� �����
	{
		printf("Failed to allocate memory for stack, exiting program...\n");//������ �����
		free(postFix);
		exit(ERROR_CODE);
	}
	st1.top = -1;//����� ������� ������ �������� ��� ���� ���� ����
	st1.capacity = equationLength;//���� ������� ��� ���� ���� ���� ������� �������

	*postFixCount = 0;//����� ���� ���� ������� �����
	tokens signToken;//����� ���� �� ������
	for (int i = 0; i < equationLength; i++)
	{
		if (infix[i].value == NULL_TOKEN)//�� ����� ������ ��� ��� ��� NULL_TOKEN 
		{
			continue;//��� �����
		}

		if (infix[i].number)//�� ����� ������ ��� ����
		{
			postFix[*postFixCount] = infix[i];//��� ������ ��� ����� �����
			(*postFixCount)++;
		}
		else
		{
			switch (infix[i].sign)//�� ����� ������ ��� ����
			{
			case '+':
			case '-':
			case '*':
			case '/':
				poppingFromStack(&st1, infix[i], postFix, postFixCount);//����� �� ����� ������� ��� ��� ������ ���� �� ���� ����� ������
				if (!push(infix[i], &st1))//����� ����� ������
				{
					printf("Pushing another element will result in stack overflow\n");
					free(postFix);
					free(st1.elements);
					exit(ERROR_CODE);
				}
				break;
			case '('://�� ����� ������ ��� (
				if (!push(infix[i], &st1))//��� ����� �������
				{
					printf("Pushing another element will result in stack overflow\n");
					free(postFix);
					free(st1.elements);
					exit(ERROR_CODE);
				}
				break;
			case ')'://�� ����� ������ ��� )
				while (peek(&st1, &signToken) && signToken.sign != '(')//�� ��� ������� �� ���� ������ ������� �� (
				{
					if (!pop(&st1, &signToken))//����� ����
					{
						printf("Error: Failed to pop from stack.\n");
						free(postFix);
						free(st1.elements);
						exit(ERROR_CODE);
					}
					postFix[(*postFixCount)++] = signToken;//������ ����� �����
				}
				if (st1.top == -1 || !pop(&st1, &signToken))//�� ����� ������ ������� ���� � ( �� ����
				{
					printf("Error with parenthesis\n");//����� ����� �� �������
					free(postFix);
					free(st1.elements);
					exit(ERROR_CODE);
				}
				break;
			default://��� ���� ��� ����� ����� �� ���� �� ����
				printf("Invalid token\n");
				free(postFix);
				free(st1.elements);
				exit(ERROR_CODE);
			}
		}
	}

	tokens remainToken;//����� ���� ����� �������
	while (st1.top > -1)//����� �� ������� ������ ������� ������� ����� ������� �����
	{
		if (pop(&st1, &remainToken))//�� ������� �� ����
		{
			if (remainToken.sign == '(' || remainToken.sign == ')')//������ ����� ��� ����
			{
				printf("Error with parenthesis\n");//����� ����� �� �������
				free(postFix);
				free(st1.elements);
				exit(ERROR_CODE);
			}
			postFix[*postFixCount] = remainToken;//���� ����� ����� ����� ������� �����
			(*postFixCount)++;
		}
	}
	free(st1.elements);//����� �������
	return postFix;//����� ���� ������� �����
}


static int FindNumbers(int* index, char* equation)//���� �����: ����� ������ �� ����� ������� �����, ���� �����: ����� ��� �� ������ �� ����� ����� ����� 10 ������ ������� ������ �����
{
	int num = 0;
	int i = *index;
	int length = strlen(equation);
	while (i < length && equation[i] >= '0' && equation[i] <= '9')//�� ��� ��� ������ ��� ����
	{
		num = num * 10 + equation[i] - '0';//����� ���� ����� 10
		i++;
	}
	*index = i;//����� ������ �����
	return num;//����� �����
}


bool isUnaryMinus(char* equation, int minus_index)//���� �����: ����� ������ �� ���� ������� �� �����, ���� �����: ����� ��� �� ����� ����� �� ��� ���� ���� ���� �����
{
	if (minus_index == 0)//�� ������ ��� ������ ������� �� �� ���� �����
	{
		return true;
	}

	int prev_index = minus_index - 1;//���� ��� ������� ����� ������� �����
	while (prev_index >= 0 && (equation[prev_index] == ' ' || equation[prev_index] == '\t'))//�� ��� ������� ���� �0
	{
		prev_index--;//������� ���
	}
	if (prev_index < 0)//�� ������� ��� ������
	{
		return true;//����� ���
	}
	char prev_char = equation[prev_index];//�� ����� �� ���� ���� ����� �� �� ���� �����
	if (prev_char == '(' || prev_char == '+' || prev_char == '-' ||
		prev_char == '*' || prev_char == '/')
	{
		return true;
	}
	return false;//���� ����� ���
}


static int Tokenize(tokens* tokenptr, char* equation, int* p_index, int equationlength) //���� �����: ����� ������ �� ���� ������ ���, ���� �� ������, ����� ������� ����� �� ������� ����� �����, ���� �����, ����� �� �� ����� ����� �����
{
	int index = *p_index;//���� ������� ������

	while (index < equationlength && (equation[index] == ' ' || equation[index] == '\t')) //�� ��� ��� ������ ��� ���� �������� ������ ���� ����� ��� ����
	{
		index++;
	}

	*p_index = index;//����� ������ �� ������� ������

	if (index < equationlength) //�� ������� ������ ��� ���� �����
	{
		tokens t1;//����� ����

		if (equation[index] == '-' && isUnaryMinus(equation, index)) //�� ����� ������ ��� ���� �����
		{
			index++;//������� ����
			int start_index = index;//���� ������ ������
			int num = FindNumbers(&index, equation);//����� ������ ������ ������ �� �����

			if (start_index == index) //�� ������� �� ��� ���� ���� ��� ���� ����� ���
			{
				printf("Input is invalid\n");//����� �����
				return -1;
			}

			t1.value = -num;//��� ����� ���� ����� ������
			t1.sign = 0;//����� ���� 0
			t1.number = true;
			tokenptr[*p_index] = t1;//����� ���� ������� ������ ����� �������

			for (int i = start_index; i < index; i++) //���� �� ������� ������� ���� ��� ����� ����� ����� ��� ������� ������� �������
			{
				tokenptr[i].value = NULL_TOKEN;
				tokenptr[i].sign = 0;
				tokenptr[i].number = false;
			}

			*p_index = index;//����� ������ ������� ������
		}
		else if (equation[index] >= '0' && equation[index] <= '9') //�� ��� ������ ��� ����
		{
			int start_index = index;//���� ������� ������
			int num = FindNumbers(&index, equation);//����� ������ ������ ������ �� ����� ����� ���

			t1.value = num;
			t1.sign = 0;
			t1.number = true;
			tokenptr[start_index] = t1;

			for (int i = start_index + 1; i < index; i++) //����� �� ������� ���� ��� ����� ����� ����� ��� ������� ������� �������
			{
				tokenptr[i].value = NULL_TOKEN;
				tokenptr[i].sign = 0;
				tokenptr[i].number = false;
			}

			*p_index = index;
		}
		else if (equation[index] == '+' || equation[index] == '-' ||
			equation[index] == '*' || equation[index] == '/' ||
			equation[index] == '(' || equation[index] == ')') //�� ����� ������ ��� ��� �������� �����
		{
			t1.value = 0;//���� ���� 0
			t1.sign = equation[index];//����� ���� ��� ������
			t1.number = false;
			tokenptr[index] = t1;

			index++;
			*p_index = index;
		}
		else {
			printf("Input is invalid\n");
			return -1;
		}

		return Tokenize(tokenptr, equation, p_index, equationlength);//����� ���� �������
	}

	return 0;
}


char* CreateEquation()//���� �����: ����� ������, ���� �����: ����� ���� �� ������
{
	char tempBuffer[100];//����� ���� ����
	printf("Enter equation: \n");
	if (fgets(tempBuffer, sizeof(tempBuffer), stdin) == NULL)//�� ����� ���� ������ ���
	{
		printf("Input reading failed\n");//����� �����
		return NULL;
	}
	printf("Input reading succeeded\n");

	size_t len = strlen(tempBuffer);
	if (len > 0 && tempBuffer[len - 1] == '\n')//����� ����� newline
	{
		tempBuffer[len - 1] = '\0';//����� ��� ������
	}

	int size = strlen(tempBuffer);//����� ���� �������
	char* equation = (char*)malloc((size + 1) * sizeof(char));//����� ������ ������ ����� ���� ������� ������
	if (equation == NULL)//�� ����� ���� ������ ������
	{
		printf("Memory allocation failed\n");//����� �����
		return NULL;
	}

	strcpy(equation, tempBuffer);//���� ������� ������ ����� ����� ����� ����
	printf("Equation entered: %s\n", equation);//����� ������� ������
	return equation;//����� �������
}


int main()
{
	char* equation = CreateEquation();//����� ���� ������
	if (equation == NULL)//�� ����� ���� ������ ������
	{
		printf("Failed to create equation.\n");//����� �����
		return -1;
	}
	printf("Memory allocation succeeded for string\n");

	int equationlength = strlen(equation);//����� ���� �������
	tokens* t1 = (tokens*)malloc(equationlength * sizeof(tokens));//����� ���� ������ ����� ����� ���� �������
	if (t1 == NULL)//�� ����� ���� ������ �������
	{
		printf("Memory allocation failed\n");//����� �����
		free(equation);
		return -1;
	}
	printf("Memory allocation succeeded for tokens\n");

	for (int i = 0; i < equationlength; i++)//����� ���� �������
	{
		t1[i].value = NULL_TOKEN;
		t1[i].sign = 0;
		t1[i].number = false;
	}

	int index = 0;//������ ������
	int answer = Tokenize(t1, equation, &index, equationlength);//����� ���� �������

	if (answer != 0)//�� ����� ��� �����
	{
		printf("One of the tokens is invalid.\n");//����� �����
		free(equation);
		free(t1);
		return -1;
	}
	printf("Successfully tokenized the string\n");

	int postFixCount;//����� ������ ���� ����
	tokens* postFix = InfixToPostfix(t1, equationlength, &postFixCount);//����� ����� �����

	for (int i = 0; i < postFixCount; i++)//����� ����� �����
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
