//Лабораторная работа 2.
//Условия задачи: проверить что выражение является математическим выражением и посчитать. Выражение может содерржать скобки () знаки +-*/. 

#include <vector>
#include <iostream>
#include <cstring>

using namespace std;

bool hereIsDelenZero = false;													//флаг "деление на 0", если он тру, то программа останавливает вычисления
bool skobka = false;															//флаг закрытой скобки. если флаг тру, значит скобка в выражении не закрыта и программа не начинает вычисления

bool provingLetters(char * str);												//Проверка на отсутствие букв в выражении
bool provingDoubleChar(char * str);												//Проверка, не идет ли после знака операции знак операции
float engenee(char * str);
float calcul(float a, char zn, float b);										//функция для вычисления простых выражений(1 операция)

int main()
{
	char * xmas = new char[200];
	
	bool flagForLetters = false;												//Блок проверки введеных данных на валидность
	bool flagForDoubleZn = false;							
	while (!(flagForDoubleZn) || !(flagForLetters))
	{
		cout << "Введите выражение\n";
		cin.getline(xmas, 200);
		flagForLetters = provingLetters(xmas);
		flagForDoubleZn = provingDoubleChar(xmas);
		if (!flagForLetters)
		{
			cout << "\nЗдесь не должны быть буквы!\n";
		}
		if (!flagForDoubleZn)													//сработает, если введено 2 знака операции, например 4--5
		{
			cout << "\nВозможно, допущена опечатка\n";
		}
	}
	
	char * mas = new char[200]; 												//Блок преобразования исходной строки к строке, где нет пробелов
	mas[0] = 0;																	//на случай, если строка состоит только из пробелов
	int ind = -1;
	for (int i=0; i<strlen(xmas);i++)
	{
		if (xmas[i]!=' ')
		{
			ind++;
			mas[ind] = xmas[i];
			
		}
	}
	if (ind == -1)
	{
		cout << '0' <<endl;
		return 0;
	}
	delete [] xmas;
	xmas = NULL;
	
	for (int i = 0; i<strlen(mas)-1; i++)											//Если пользователь введет равно в середине выражения, то все, что идет после него, обнуляется
	{
		if (mas[i] == '=')
		{
			for (int j = i+1; j<strlen(mas); j++ )
			{
				mas[j] = 0;
			}
			continue;
		}
	}
	
	int len = strlen(mas);
	if (mas[len-1] != '=')														//На случай, если пользователь ввел строку, не заканчивающуюся на равно
		{
			mas[len] = '=';
		}
	float answer = engenee(mas);
	if(!skobka)
	{
		cout << answer <<endl;
	}

	return 0;
}

float engenee(char * str)
{
	while(!skobka)
	{
		if (str[strlen(str)-1]!='=')
		{
			str[strlen(str)]='=';
		}
		vector<float> nums(200);												//все числа в выражении
		vector<char> znaks(200);												//все знаки операций
		vector<int> poryadok(200);												//порядок следования операций
		
		int step = 0;
		float curr = 0;
		
		for (int i = 0; i < strlen(str); i++)
		{
			if (str[i]=='(')
			{
				int ravnoves = 1;												//отслеживаем равновесие скобок - открытые=закрытые
				int steps = i;
				while(ravnoves && steps<strlen(str))
				{
					steps++;
					if (str[steps] == '(')
						{
							ravnoves++;
						}
					if (str[steps] == ')')
						{
							ravnoves--;
						}
				}
				if (ravnoves)
				{
					cout <<"Скобка не закрыта!!!";
					skobka=true;
				}
				//В чем суть решения? если найдены скобки, то выделяю все выражение, лежащее между ними и отправляю реккурсивно в эту же функцию
				char * stru = new char[steps - i - 1];								
				for (int j = 0; j < steps - i - 1; j++)
				{
					stru[j] = str[j + i + 1];
				}
				curr = engenee(stru);
				i = steps + 1;
			}
			
			if (str[i] >= '0' && str[i] <= '9')									//считываем данные для вектора чисел
			{
				curr = curr * 10 + (str[i] - '0');
			}
			else
			{																	//считываем данные для вектора знаков
				nums[step] = curr;
				znaks[step] = str[i];
				step++;
				curr = 0;
			}
		}
		
		int poryadoks = 1;
		for (int i = 0; i < znaks.size(); i++)									//проставляем приоритеты операций
		{
			if ((znaks[i] == '*' || znaks[i] == '/'))
			{
				
				poryadok[i] = poryadoks;
    	        poryadoks++;
			}
		}
		
		for (int i = 0; i<step; i++)
		{
			if ((znaks[i] == '+' || znaks[i] == '-'))
			{
				
				poryadok[i] = poryadoks;
    		    poryadoks++;
			}
		}


		int shag = 1;
		for (int i = 0; i< poryadoks&& !hereIsDelenZero; i++)
		{
			for (int j = 0; j<poryadoks; j++)
				if (shag == poryadok[j])
				{
					nums[j] = calcul(nums[j], znaks[j], nums[j + 1]);				//записываем результат вычисления j и j+1 чисел
					nums.erase(nums.begin() + (j + 1));								//выкидываем из потока старых чисел использованное число j+1
					znaks.erase(znaks.begin() + j);									//выкидываем из потока старых знак использованный знак j
					poryadok.erase(poryadok.begin() + j);							//
	
					shag++;
				}
		
		}
		return nums[0];
	}		
			
}
bool provingLetters(char * str)													//проверка на отсутствие букв
{
	bool ans = false;

	for (int i = 0; i < strlen(str); i++)
	{

		if (str[i] <= '9' && str[i] >= '0' ||
			str[i] == '+' || str[i] == '-' || str[i] == '*' || str[i] == '/' || 
			str[i] == '=' || str[i] == ' ' || str[i] == '(' || str[i] == ')' )
		{
			ans = true;
		}
		else
		{
			return false;
		}

	}
	return ans;

}
bool provingDoubleChar(char * str)												//Проверка, не идет ли после знака операции знак операции
{
	bool hereIsEqual = false;
	for (int i = 0; i < strlen(str) - 1; i++)
	{
		if (str[i] == '=')
		{
			hereIsEqual = true;
		}
		if (str[i] == '+' || str[i] == '-' || str[i] == '*' || str[i] == '/')
		{
			if (str[i + 1] == '+' || str[i + 1] == '-' || str[i + 1] == '*' || str[i + 1] == '/' || str[i + 1] == '=')
			{
				return false;
			}
	
		}
	}
	return true;
}

float calcul(float a, char zn, float b)
{
	float ans = a;
	switch (zn)
	{
		case '+': ans += b; break;
		case '-': ans -= b; break;
		case '*': ans *= b; break;
		case '/': {
						if (b==0)
						{
							cout <<"На нуль делить нельзя!\n"<<endl;
							hereIsDelenZero = true;
							return 0;
						}
						ans /= b; 
						break;
		
				}
	}
	return ans;
}
