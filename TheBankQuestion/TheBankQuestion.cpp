#include<iostream>
using std::cout;
using std::cin;
using std::endl;
using std::ios;
using std::fstream;
using std::ostream;

#include<string>

#include<fstream>

#include<vector>
using std::vector;

//involving the function.h
#include"Account.h"

#include"Date.h"


vector<Date> dmy;
vector<string> _operation;	//记录每次的操作
vector<string> accountIDcode;	//记录账户的相关操作
vector<int> vamount;	//记录账户每次的存取款数额

vector<Account*> vaccount;
SavingAccount* Saccount;
CreditAccount* Caccount;

//定义一个类来实例化上一次的操作
class Controler
{
	std::ifstream _controler1;
	std::ofstream _controler2;
	vector<string> dmy;
	vector<char> commander;
	vector<string> _operation;	//记录每次的操作
	vector<string> accountIDcode;	//记录账户的相关操作
	vector<int> vamount;	//记录账户每次的存取款数额
public:
	Controler(vector<char>commander, vector<string> dmy, vector<string> _operation, vector<string> accountIDcode, vector<int> vamount)
		: commander(commander), dmy(dmy), _operation(_operation), accountIDcode(accountIDcode), vamount(vamount) {};	//对该类的实例化
	~Controler();
	void Initialize(string filename);	//对操作类容的初始化
	void Commandstore(char commander, Date);
};
//不能在构造函数中初始化fstream类
void Controler::Initialize(string filename)
{
	string operations;	//所有操作
	this->_controler1.open(filename);		//将所声明的文件打开
	this->_controler2.open(filename, ios::app);		//将所声明的文件打开
	while (!_controler1.eof())
	{
		getline(_controler1, operations);
		this->dmy.push_back(operations);
		getline(_controler1, operations);
		this->accountIDcode.push_back(operations);
		getline(_controler1, operations);
		this->vamount.push_back(atoi(operations.c_str()));
		getline(_controler1, operations);
		this->_operation.push_back(operations);
	}
}

//将操作保存起来
void Controler::Commandstore(char commander, Date)
{

}

//默认析构函数将操作内容写入文件
Controler::~Controler()
{
	this->_controler1.close();
	this->_controler2.close();
}

//定义查找用户ID以便确认操作
int Check_Idcode(string idcode);

int main(int argc, int* argv[])
{
	int year = 0, month = 0, day = 0;
	double quota, anualfee;

	Date date = Date(2008, 11, 1);

	SavingAccount a = SavingAccount(date, "noname", 0);

	string IDcode = "noidcode";
	double rate;

	cout << "(a)add new accout   (d)deposit   (w)withdraw   (s)show   (c)change day    (n)next month   (e)exit   (i)inquiry\n";
	char operator_char;

	do {
		int amount;	//定义索引和当前操作金额
		string operation;

		//输出当前日期和银行总余额
		cout << date.the_year << '-' << date.the_month << '-' << date.the_day << '\t' << "Total:" << a.total << "\t\t";

		cout << "your commond>";
		cin >> operator_char;
		if (operator_char == 'a' || operator_char == 'd' || operator_char == 'w' || operator_char == 's' || operator_char == 'c' || operator_char == 'n' || operator_char == 'i') {
			getchar();	//清理最后一个回车
			switch (operator_char)
			{
				char type_char;
			case 'a':
				cout << "Please input the type you want to create: (and S for savingaccount, C for creditaccount)\n";
				cout << "And the date now is:\t" << date.the_year << '-' << date.the_month << '-' << date.the_day << '\t' << "Your type> ";
				cin >> type_char;
				cout << "Please input your IDcode and rate> ";
				getchar();	//清理回车
				std::getline(cin, IDcode);
				cin >> rate;

				if (type_char == 'S')
				{
					Saccount = new SavingAccount(date, IDcode, rate);
					//存入该容器
					vaccount.push_back(Saccount);
					operation = "Created a new SavingAccount";
				}
				else
				{
					Caccount = new CreditAccount(date, IDcode, rate);
					cout << "Input the quota and anual fee>";
					cin >> quota >> anualfee;
					Caccount->get_Crd_Anf(quota, anualfee);
					vaccount.push_back(Caccount);
					operation = "Created a new CreditAccount";
				}
				vamount.push_back(0);
				break;
			case 'i':	//按日期查询账户日历
			{
				cout << "Please input the date you want to inquiry> ";
				cin >> year >> month >> day;
				Date new_date(year, month, day);
				int index = 0;
				auto pin = dmy.begin();
				while (pin != dmy.end() && dmy[index] <= new_date)
				{
					dmy[index].show();
					cout << accountIDcode[index] << "\t\t";
					if (accountIDcode[index] != "noidcode" && _operation[index] != "Created a new SavingAccount" && _operation[index] != "Created a new CreditAccount")
						cout << vamount[index] << "\t\t";
					cout << _operation[index] << endl;
					index++;
					pin++;
				}
				operation = "Inquiried the information for all accounts";
				IDcode = "noidcode";
				vamount.push_back(0);
				break;
			}
			case 's':	//遍历输出各账户信息
				for (int n = 0; (rsize_t)n < vaccount.size(); n++)
					vaccount[n]->show();
				operation = "Output all accounts' information";
				IDcode = "noidcode";
				vamount.push_back(0);
				break;
			case 'c':	//改变日期
				int day;
				cout << "Please input the day you want to change>\t";
				cin >> day;
				if (day <= 0 || day > date.The_Max_Day_of_Month_Function())
				{
					cout << "Invalid date:";
					break;
				}
				else if (day < date.the_day)
				{
					cout << "You cannot specify a previous day";
					break;
				}
				else
					date.Set_Date(date.the_year, date.the_month, day);
				operation = "Change the date";
				IDcode = "noidcode";
				vamount.push_back(0);
				break;
			case 'n':
				if (date.the_month == 12)
					date.Set_Date(++date.the_year, 1, 1);
				else
					date.Set_Date(date.the_year, ++date.the_month, 1);
				for (int n = 0; (rsize_t)n < vaccount.size(); n++)
					vaccount[n]->settle(date);
				operation = "Moved to the next month";
				IDcode = "noidcode";
				vamount.push_back(0);
				break;
			case 'd': //存入现金
			{
				cout << "Please input the idcode for your next operation> ";
				std::getline(cin, IDcode);
				//找到是第几号用户
				int the_number = Check_Idcode(IDcode);
				if (the_number == -1)
				{
					do
					{

						cout << "Haven't found the IDcode you insert, please try again>";
						std::getline(cin, IDcode);
						the_number = Check_Idcode(IDcode);
					} while (the_number == -1);
				}
				cout << "Input the amount and information for deposite: \n";
				cin >> amount;	//cin后面需要加 getchar() 来清空cin输入流的回车符号
				getchar();
				std::getline(cin, operation);
				operation.append("<-deposite");
				vaccount[the_number]->deposite(date, amount, operation);
				vamount.push_back(amount);
				break;
			}
			case 'w'://取款操作
			{
				cout << "Please input the idcode for your next operation> ";
				std::getline(cin, IDcode);
				//找到是第几号用户
				int the_number = Check_Idcode(IDcode);
				if (the_number == -1)
				{
					do
					{
						cout << "Haven't found the IDcode you insert, please try again>";
						std::getline(cin, IDcode);
						the_number = Check_Idcode(IDcode);
					} while (the_number == -1);
				}
				cout << "Input the amount and information for withdraw: \n";
				cin >> amount;
				getchar();
				std::getline(cin, operation);
				operation.append("<-withdraw");
				vaccount[the_number]->withdraw(date, amount, operation);
				vamount.push_back(-amount);
				break;
			}
			}

			//记录每次的操作
			dmy.push_back(date);
			accountIDcode.push_back(IDcode);
			_operation.push_back(operation);
		}
		else if (operator_char == 'e') {
			cout << "You have exited the Bank.";
		}
		else {
			cout << "Bash: Invalid commond!!!\n";
			string clear;
			std::getline(cin, clear);
			clear.clear();
		}
	} while (operator_char != 'e');
	system("pause");
	return 0;
}

//查找函数定义
int Check_Idcode(string idcode)
{
	auto pin = vaccount.begin();
	for (int n = 0; (rsize_t)n < vaccount.size(); n++)
	{
		if (vaccount[n]->Idcode == idcode)
			return n;
	}
	//如果未查找打用户ID,则返回错误信息
	return -1;
}

