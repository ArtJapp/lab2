
#include <vector>

#include <iostream>
#include<string>
#include <cstring>

using namespace std;

bool hereIsDelenZero = false;
bool skobka=false;

bool provingLetters(char * str);
bool provingDoubleChar(char * str);												//Проверка, не идет ли после знака операции знак операции
int engenee(char * str);
int calcul(int a, char zn, int b);

int main()
{
	char * mas = new char[200];
	bool flagForLetters = false, flagForDoubleZn=false;
	while (!(flagForDoubleZn && flagForLetters))
	{
		cout << "Введите выражение\n";
		cin >> mas;
		flagForLetters = provingLetters(mas);
		flagForDoubleZn = provingDoubleChar(mas);
		if (!flagForLetters)
		{
			cout << "\nЗдесь не должны быть буквы!\n";
		}
		if (!flagForDoubleZn)
		{
			cout << "\nВозможно, допущена опечатка\n";
		}
	}

	int len = strlen(mas);
	if (mas[len-1] != '=')														//На случай, если пользователь ввел строку, не заканчивающуюся на равно
		{
			mas[len]='=';
		}
	int answer = engenee(mas);
	if(!skobka)
	{
		cout << answer <<endl;
	}

	return 0;
}

int engenee(char * str)
{//TODO переделать массив чар в вектор, чтобы выполнить действие с скобкой-удалить
	while(!skobka)
	{
	if (str[strlen(str)-1]!='=')
		{
			str[strlen(str)]='=';
		}
	vector<int> nums(200);
	vector<char> znaks(200);
	vector<int> poryadok(200);
	int step = 0;
	int curr = 0;
	for (int i = 0; i < strlen(str); i++)
	{
		if (str[i]=='(')
		{
			int ravnoves=1;
			int steps=i;
			while(ravnoves && steps<strlen(str))
			{
				steps++;
				if (str[steps]=='(')
					{
						ravnoves++;
					}
				if (str[steps]==')')
					{
						ravnoves--;
					}
			}
			if (ravnoves)
			{
				cout <<"Скобка не закрыта!!!";
				skobka=true;
			}
			char * stru = new char[steps-i-1];
			for (int j=0;j<steps-i-1;j++)
				stru[j]= str[j+i+1];
			
			
			int zamen=engenee(stru);
			nums[step] = zamen;
			step++;
			i=steps+2;
		}
		if (str[i] >= '0' && str[i] <= '9')
		{
			curr = curr * 10 + (str[i] - '0');
		}
		else
		{
			nums[step] = curr;

			znaks[step] = str[i];
			step++;
			curr = 0;
		}
	}

	int poryadoks = 1;
	for (int i = 0; i < znaks.size(); i++)
	{
		if ((znaks[i] == '*' || znaks[i] == '/'))
		{
			
			poryadok[i] = poryadoks;
            poryadoks++;
		}

	}
	for (int i = 0; i<=step; i++)
	{
		if ((znaks[i] == '+' || znaks[i] == '-'))
		{
			
			poryadok[i] = poryadoks;
            poryadoks++;
		}
	}
	//На этом этапе все работает - все числа и знаки между ними записываются согласно исходному выражению
	//UPD: все шикарно работает и ниже))
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
bool provingLetters(char * str)
{
	bool ans = false;

	for (int i = 0; i < strlen(str); i++)
	{

		if (str[i] <= '9' && str[i] >= '0'
			||
			str[i] == '+' || str[i] == '-' || str[i] == '*' || str[i] == '/' || str[i] == '=' || str[i] == ' '
			||
			str[i] == '(' || str[i] == ')' 
			)
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
bool provingDoubleChar(char * str)						//Проверка, не идет ли после знака операции знак операции
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
int calcul(int a, char zn, int b)
{
	int ans = a;
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
