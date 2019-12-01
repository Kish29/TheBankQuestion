#include<iostream>
#include<algorithm>
using std::cout;
using std::cin;
using std::endl;
using std::ios;
using std::fstream;
using std::ostream;

#include<string>

using std::string;

#include<stdexcept>

#include<fstream>

#include<vector>
using std::vector;

#include"Account.h"

#include"Date.h"

//处理command字符char时运行的判断
void Operator_charEXp(string redundant);

//处理用户定义账户类型时判断，继承自runtime_error
class AccountException :public std::runtime_error
{
private:
	const Account* error_account;
public:			//根据runtime_error的定义，第一个参数时string常量，第二个是类指针
	AccountException(string const& _error_account, Account& account) :std::runtime_error(_error_account)
	{
		this->error_account = &account;	//得到错误账户类型的地址
		cout << "ERROR ACCOUNT!!!" << this->error_account->Idcode << '-' << this->error_account->rate << endl;
		//抛出异常
		throw std::invalid_argument("Input redundant chars to creat accounts");
	}
};


//用于判断日期构造类的错误
void Date_initial_exp(Date& date);

Date date_total = Date(2008, 11, 1);

vector<Date> dmy;
vector<string> _operation;	//记录每次的操作
vector<string> accountIDcode;	//记录账户的相关操作
vector<double> vamount;	//记录账户每次的存取款数额
vector<double> vrate;	//默认位0
vector<double> vquota;	//默认位0
vector<double> vanualfee;	//默认位0

vector<Account*> vaccount;
SavingAccount* Saccount;
CreditAccount* Caccount;

//定义查找用户ID以便确认操作
int Check_Idcode(string idcode);

//定义创建用户的函数
void CreatAccounts(char type_char, Date date, string IDcode, double rate, double quota = 0, double anualfee = 0)
{
	if (type_char == 'S')
	{
		Saccount = new SavingAccount(date, IDcode, rate);
		//存入该容器
		vaccount.push_back(Saccount);
		_operation.push_back("Created a new SavingAccount");
	}
	else
	{
		Caccount = new CreditAccount(date, IDcode, rate);
		Caccount->get_Crd_Anf(quota, anualfee);
		vaccount.push_back(Caccount);
		_operation.push_back("Created a new CreditAccount");
	}
	dmy.push_back(date);
	accountIDcode.push_back(IDcode);
	vamount.push_back(0);
}

//存款
void Deposite(Date date, string IDcode, double amount, string operation)
{
	int the_number = Check_Idcode(IDcode);
	vaccount[the_number]->deposite(date, amount, operation);
	dmy.push_back(date);
	accountIDcode.push_back(IDcode);
	vamount.push_back(amount);
	_operation.push_back(operation);
}

//取款
void Withdraw(Date date, string IDcode, double amount, string operation)
{
	int the_number = Check_Idcode(IDcode);
	vaccount[the_number]->withdraw(date, amount, operation);
	dmy.push_back(date);
	accountIDcode.push_back(IDcode);
	vamount.push_back(-amount);
	_operation.push_back(operation);
}

//移至下一个月
void MoveNextMonth(Date date, string IDcode, double amount, string operation)
{
	int n = Check_Idcode(IDcode);
	vaccount[n]->settle(date);
}
//定义一个类来实例化上一次的操作
class Controler
{
public:
	std::ifstream _controler1;
	std::ofstream _controler2;
public:
	Controler() {};
	~Controler();
	void Initialize();	//对操作类容的初始化
	void Commandstore(string commander, Date date, string accountID, string operation, double amount = 0, double rate = 0, double quota = 0, double anualfee = 0);	//存储
};
//不能在构造函数中初始化fstream类
void Controler::Initialize()
{
	string operations;	//所有操作

	while (!_controler1.eof())		//以回车键来控制读取
	{
		string command_cr;
		getline(_controler1, command_cr);
		int date[3];
		int i = 0;
		getline(_controler1, operations);
		int n = operations.find(" ");
		while (n != string::npos)
		{
			string strdate = operations.substr(0, n);
			operations.erase(0, n + 1);
			date[i++] = atoi(strdate.c_str());
			n = operations.find(" ");
		}
		dmy.push_back(Date(date[0], date[1], date[2]));
		getline(_controler1, operations);
		accountIDcode.push_back(operations);

		getline(_controler1, operations);
		vamount.push_back(atof(operations.c_str()));		//将string类型转化位double类型,获得存取款额

		getline(_controler1, operations);
		vrate.push_back(atof(operations.c_str()));		//获得利率

		getline(_controler1, operations);
		vquota.push_back(atof(operations.c_str()));		//将string类型转化位int类型，欠款额度

		getline(_controler1, operations);
		vanualfee.push_back(atof(operations.c_str()));		//获得年费

		getline(_controler1, operations);
		_operation.push_back(operations);

		if (command_cr == "S")
			CreatAccounts('S', dmy.back(), accountIDcode.back(), vrate.back());

		else if (command_cr == "C")
			CreatAccounts('C', dmy.back(), accountIDcode.back(), vrate.back(), vquota.back(), vanualfee.back());

		else if (command_cr == "d")
			Deposite(dmy.back(), accountIDcode.back(), vamount.back(), _operation.back());

		else if (command_cr == "w")
			Withdraw(dmy.back(), accountIDcode.back(), vamount.back(), _operation.back());

		else if (command_cr == "n")
			MoveNextMonth(dmy.back(), accountIDcode.back(), vamount.back(), _operation.back());

		if (date[2] > 0 && date[0])
			date_total.Set_Date(date[0], date[1], date[2]);
	}
}
//将操作保存起来
void Controler::Commandstore(string commander, Date date, string accountID, string operation, double amount, double rate, double quota, double anualfee)
{
	_controler2 << commander << '\n';
	_controler2 << date.the_year << ' ' << date.the_month << ' ' << date.the_day << ' ' << '\n';
	_controler2 << accountID << '\n';
	_controler2 << amount << '\n';
	_controler2 << rate << '\n';
	_controler2 << quota << '\n';
	_controler2 << anualfee << '\n';
	_controler2 << operation << '\n';
}

//默认析构函数将文件关闭
Controler::~Controler()
{
	this->_controler1.close();
	this->_controler2.close();
}

//先增加一个保存所有账户和用户名的文件
fstream allaccountsinfo("C:\\Users\\asus\\Desktop\\Account\\AccountInfo.txt", ios::in | ios::app);


//直接增加一个用户类，每个类中有两个成员函数SavingAccount类和CreditAccount类
class Users
{
private:
	//创建其用户名和密码，放在这个用户的专属文本开头
	string name, passwords;
	Controler ctr;	//用于实现每个用户的初始化
	SavingAccount savc;
	CreditAccount cdtc;
public:
	Users(string name = "name", string passwords = "passwords") :name(name), passwords(passwords) {}	//构造函数初始化其用户名和密码，并为其创建新的文本文件
	void get_initialized() {
		this->ctr.Initialize();
	}	//初始化函数
	void Regestrator(string name) {
		this->ctr._controler2.open("c:\\users\\asus\\desktop\\Account\\" + this->name + ".txt", ios::app);
	}	//注册函数
	void Login_in() {
		this->ctr._controler1.open("c:\\users\\asus\\desktop\\Account\\" + this->name + ".txt");
		this->ctr._controler2.open("c:\\users\\asus\\desktop\\Account\\" + this->name + ".txt", ios::app);
	}	//会在该函数中调用 get_initialized 函数
	//操作保存函数
	void getCommandstore(string commander, Date date, string accountID, string operation, double amount, double rate, double quota, double anualfee)
	{
		this->ctr.Commandstore(commander, date, accountID, operation, amount, rate, quota, anualfee);
	}
};

//用户名的查找函数
int UsersCheck(string name)
{
	allaccountsinfo.clear();
	allaccountsinfo.seekg(0);
	string _name;
	while (getline(allaccountsinfo, _name) || !allaccountsinfo.eof())
	{
		if (_name == name)
			return 1;
	}
	return 0;
}

//密码匹配函数
int PasswordsCheck(string name, string passwords)
{
	allaccountsinfo.clear();
	allaccountsinfo.seekg(0);
	string _name, _passwords;
	while (getline(allaccountsinfo, _name) || !allaccountsinfo.eof())
	{
		getline(allaccountsinfo, _passwords);
		if (_name == name && _passwords == passwords)
			return 1;
	}
	return 0;
}

//添加一个能按照金额进行排序的类
class CheckAccordingtoamount1
{
public:
	Date _date;
	string accountIDcd;
	double amoutcurrent;
	string _operations;
	CheckAccordingtoamount1(Date _date, string accountIDcd, double amoutcurrent, string _operations) :_date(_date), accountIDcd(accountIDcd), amoutcurrent(amoutcurrent), _operations(_operations) {}
};

//自定义一个从大到小的比价定义
bool compare(const CheckAccordingtoamount1& a, const CheckAccordingtoamount1& b)
{
	return a.amoutcurrent > b.amoutcurrent;
}

//show函数
void show(const CheckAccordingtoamount1& checkam)
{
	checkam._date.show();
	cout << checkam.accountIDcd << '\t' << checkam.amoutcurrent << '\t' << checkam._operations << endl;
}

//添加一个按交易金额查找的函数
void CheckAccordingtoamount()
{
	vector<CheckAccordingtoamount1> checkam;
	auto pin1 = dmy.begin();
	auto pin2 = accountIDcode.begin();
	auto pin3 = vamount.begin();
	auto pin4 = _operation.begin();
	for (; pin3 != vamount.end(); pin1++, pin2++, pin3++, pin4++)
	{
		checkam.push_back(CheckAccordingtoamount1(*pin1, *pin2, *pin3, *pin4));
	}
	std::sort(checkam.begin(), checkam.end(), compare);
	std::for_each(checkam.begin(), checkam.end(), show);
}


int main(int argc, int* argv[])
{
	cout << "If you use this program for the first time, please input \"1\", or input \"0\"> ";
	int num = 0;
	cin >> num;
	getchar();

	//创建User用户类
	Users* users;
	users = NULL;
	switch (num)
	{
	case 1: {	//如果是注册账户
		string name, passwords;
		cout << "Please input you User name> ";
		getline(cin, name);		//getline 会自动计入回车符号
		if (UsersCheck(name))
		{
			do {

				cout << "This name has existed already, please input another name> ";
				getline(cin, name);
			} while (UsersCheck(name));
		}
		cout << "Please input you passswords>";
		getline(cin, passwords);
		users = new Users(name, passwords);
		users->Regestrator(name);
		cout << "Welcome!" << '\t' << name << endl;
		allaccountsinfo.clear();
		allaccountsinfo << name << endl;
		allaccountsinfo << passwords << endl;	//将用户名和密码存进去
	}
		  break;
	case 0: {	//如果是登录账户
		string name, passwords;
		cout << "Please input you User name to Check> ";
		getline(cin, name);
		if (!UsersCheck(name))
		{
			do {
				cout << "Have not found this Users name, Please Input again> ";
				getline(cin, name);
			} while (!UsersCheck(name));
		}
		cout << "Please input you passwords> ";
		getline(cin, passwords);
		if (!PasswordsCheck(name, passwords))
		{
			do {
				cout << "The passwords are wrong, please input you passwords again>";
				getline(cin, passwords);
			} while (!PasswordsCheck(name, passwords));
		}
		users = new Users(name, passwords);
		users->Login_in();
		cout << "Welcome back!" << '\t' << name << endl;
		users->get_initialized();
	}
		  break;
	default:
		break;
	}
	//让用户按照接下来的操作提示进行操作
	int year = 0, month = 0, day = 0;
	double quota, anualfee;

	SavingAccount a = SavingAccount(date_total, "noname", 0);

	string IDcode = "noidcode";
	double rate;

	cout << "(a)add new accout   (d)deposit   (w)withdraw   (s)show   (c)change day    (n)next month   (e)exit   (i)inquiry\n";
	char operator_char;

	do {
		int amount;	//定义索引和当前操作金额
		string operation;

		//输出当前日期和银行总余额
		cout << date_total.the_year << '-' << date_total.the_month << '-' << date_total.the_day << '\t' << "Total:" << a.total << "\t\t";

		cout << "your commond>";
		cin >> operator_char;
		string Exp_string;
		getline(cin, Exp_string);
		Operator_charEXp(Exp_string);
		if (operator_char == 'a' || operator_char == 'd' || operator_char == 'w' || operator_char == 's' || operator_char == 'c' || operator_char == 'n' || operator_char == 'i') {
			switch (operator_char)
			{
				char type_char;
			case 'a':
				cout << "Please input the type you want to create: (and S for savingaccount, C for creditaccount)\n";
				cout << "And the date now is:\t" << date_total.the_year << '-' << date_total.the_month << '-' << date_total.the_day << '\t' << "Your type> ";
				cin >> type_char;
				cout << "Please input your IDcode and rate> ";
				if (getchar() != '\n') {
					string exp_account;
					getline(cin, exp_account);
					SavingAccount errora(date_total, exp_account, atof(exp_account.c_str()));
					throw AccountException::AccountException("ErrorAccount", errora);
				}
				std::getline(cin, IDcode);
				cin >> rate;
				if (type_char == 'S')
				{
					Saccount = new SavingAccount(date_total, IDcode, rate);
					//存入该容器
					vaccount.push_back(Saccount);
					_operation.push_back("Created a new SavingAccount");
					vquota.push_back(0);
					vrate.push_back(rate);
					vanualfee.push_back(0);
				}
				else
				{
					Caccount = new CreditAccount(date_total, IDcode, rate);
					cout << "Input the quota and anual fee>";
					cin >> quota >> anualfee;
					Caccount->get_Crd_Anf(quota, anualfee);
					vaccount.push_back(Caccount);
					_operation.push_back("Created a new CreditAccount");
					vquota.push_back(quota);
					vrate.push_back(rate);
					vanualfee.push_back(anualfee);
				}
				dmy.push_back(date_total);
				accountIDcode.push_back(IDcode);
				vamount.push_back(0);
				users->getCommandstore(type_char == 'S' ? "S" : "C", dmy.back(), accountIDcode.back(), _operation.back(), vamount.back(), vrate.back(), vquota.back(), vanualfee.back());
				break;
			case 'i':	//按日期查询账户日历
			{
				cout << "If you want to Inquiry info by the sort of amounts, please input 'y',or 'n'>";
				char Y_N;
				cin >> Y_N;
				if (Y_N == 'y' || Y_N == 'Y')
				{
					CheckAccordingtoamount();
					break;
				}
				cout << "Please input the date you want to inquiry> ";
				cin >> year >> month >> day;
				Date new_date(year, month, day);
				Date_initial_exp(new_date);
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
				break;
			}
			case 's':	//遍历输出各账户信息
				for (int n = 0; (rsize_t)n < vaccount.size(); n++)
					vaccount[n]->show();
				break;
			case 'c':	//改变日期
				int day;
				cout << "Please input the day you want to change>\t";
				cin >> day;
				if (day <= 0 || day > date_total.The_Max_Day_of_Month_Function())
				{
					cout << "Invalid date:";
					break;
				}
				else if (day < date_total.the_day)
				{
					cout << "You cannot specify a previous day";
					break;
				}
				else
					date_total.Set_Date(date_total.the_year, date_total.the_month, day);
				break;
			case 'n':
			{
				if (date_total.the_month == 12)
					date_total.Set_Date(++date_total.the_year, 1, 1);
				else
					date_total.Set_Date(date_total.the_year, ++date_total.the_month, 1);
				for (int n = 0; (rsize_t)n < vaccount.size(); n++)
				{
					vaccount[n]->settle(date_total);
					users->getCommandstore("n", date_total, vaccount[n]->Idcode, vaccount[n]->operation, vaccount[n]->Deposite, vaccount[n]->rate, vaccount[n]->Interest, 0);
				}
				break;
			}
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
				vaccount[the_number]->deposite(date_total, amount, operation);
				dmy.push_back(date_total);
				accountIDcode.push_back(IDcode);
				vamount.push_back(amount);
				_operation.push_back(operation);
				users->getCommandstore("d", dmy.back(), accountIDcode.back(), _operation.back(), vamount.back(), vrate.back(), vquota.back(), vanualfee.back());
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
				vaccount[the_number]->withdraw(date_total, amount, operation);
				dmy.push_back(date_total);
				accountIDcode.push_back(IDcode);
				vamount.push_back(amount);
				_operation.push_back(operation);
				users->getCommandstore("w", dmy.back(), accountIDcode.back(), _operation.back(), vamount.back(), vrate.back(), vquota.back(), vanualfee.back());
				break;
			}
			}
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

//处理command字符char时运行的判断
void Operator_charEXp(string redundant)
{
	if (redundant.size() > 1)	//通过判断超过范围则抛出异常
		throw std::overflow_error("Input redundant chars");
	else
		return;
}

//用于判断日期构造类的错误
void Date_initial_exp(Date& date)
{
	if (date.the_year > 3500 || date.the_month > 13 || date.the_day > date.The_Max_Day_of_Month_Function())		//如果构造的日期大于了4位数或者月份，日数不一样就抛出异常
		throw std::range_error("Please check the leagal of years or monthes or days");
	else
		return;
}