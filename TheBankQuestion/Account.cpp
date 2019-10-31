#include<iostream>
#include"Account.h"
#include<cmath>
#include<string>
#include<iomanip>

using std::setw;
using std::cout;
using std::cin;
using std::string;
using std::endl;

double last_Deposit = 0;

double Account::total = 0;

void Account::showtotal()
{
	cout << "Total: " << this->total << '\t';
}

//显示存取款信息、余额以及相应的操作
void SavingAccount::show()
{
	cout << this->date.the_year << '-' << this->date.the_month << '-' << this->date.the_day << '\t' << setw(10) << this->Idcode;
	cout << '\t' << setw(8) << "Banlance: " << this->Deposite << '\t' << "Interest: " << this->Interest << '\t' << this->operation << endl;
}

//计算利息，在每次有存取款操作时都要调用
void SavingAccount::SavActCalc(Date date, int current_farewell)
{
	this->Interest += floor((this->rate * (this->Deposite * this->date.Days_of_Cal(date))) / 365 * 100 + 0.5) / 100;
	this->Deposite += current_farewell;
	this->total += current_farewell;
}

//利息的总结算，因此要加上此时的账户余额
void SavingAccount::settle(Date date)
{
	this->Interest += floor((this->rate * (this->Deposite * this->date.Days_of_Cal(date))) / 365 * 100 + 0.5) / 100;
	//计算利息，方便起见，均用365天
	this->Deposite += this->Interest;	//加上利息
	this->date.Set_Date(date.the_year, date.the_month, date.the_day);
	this->show();
	//结算后利息清零
	this->Interest = 0;
}

//存款函数
void SavingAccount::deposite(Date date, int deposite, const std::string operation)
{
	if (this->Deposite == 0)	//如果是第一次存款
	{
		this->Deposite += deposite;	//存款更新
		this->date.Set_Date(date.the_year, date.the_month, date.the_day);	//将账户的操作日期刷新
		this->operation = operation;	//记录账户的操作信息
		this->total += deposite;
		show();
		return;
	}
	else
		SavActCalc(date, deposite);
	this->date.Set_Date(date.the_year, date.the_month, date.the_day);	//将账户的操作日期刷新
	this->operation = operation;	//记录账户的操作信息
	show();
}

//取款函数
void SavingAccount::withdraw(Date date, int withdraw, const std::string operation)
{
	int new_withdraw = 0;
	if (withdraw > this->Deposite)	//如果取款大于存款，则不能将钱取出来
	{
		do
		{
			std::cout << "Your withdraw overed your deposite! And your deposite is: " << this->Deposite << '\n';
			std::cout << "Please try again and to make sure you withdraw doesn't over your deposite:";
			std::cin >> new_withdraw;
		} while (new_withdraw > this->Deposite);
		this->SavActCalc(date, -new_withdraw);
	}
	else
		this->SavActCalc(date, -withdraw);
	this->operation = operation;
	this->date.Set_Date(date.the_year, date.the_month, date.the_day);	//更新日期
	show();		//显示相关信息
}

//构造函数初始化
Account::Account(Date date, std::string idcode, double rate)
{
	this->date.Set_Date(date.the_year, date.the_month, date.the_day);
	this->Idcode = idcode;
	this->rate = rate; //初始化每个账户的建立日期信息以及Id、利率
	cout << this->date.the_year << '-' << this->date.the_month << '-' << this->date.the_day << '\t' << setw(10) << this->Idcode;
	cout << " created" << endl;
}

//在派生类中调用基类的构造函数
SavingAccount::SavingAccount(Date date, std::string idcode, double rate) :Account(date, idcode, rate) {}

//同上
CreditAccount::CreditAccount(Date date, std::string idcode, double rate) : Account(date, idcode, rate) {}

//获得账户可用额度和年费以及利率
void CreditAccount::get_Crd_Anf(double Credict_Money, double anual_fee)
{
	this->Credict_Money = Credict_Money;
	this->Save_Crd_MONEY = Credict_Money;	//这个用来存储最大额度
	this->anual_fee = anual_fee;
}

//信用卡账户的金额计算
void CreditAccount::CrdSettle(Date date, int current_money, string operation)
{
	last_Deposit = this->Deposite;
	//获取间隔天数,并更新日期
	int days = this->date.Days_of_Cal(date);
	this->date.Set_Date(date.the_year, date.the_month, date.the_day);		//更新日期

	//结算利息
	this->Interest += this->Deposite >= 0 ? 0 : floor(this->Deposite * this->rate * days * 100 + 0.5) / 100;

	//额度变动
	this->Deposite += current_money;

	//输出当前操作日期
	cout << date.the_year << '-' << date.the_month << '-' << date.the_day << '\t' << setw(10) << this->Idcode;

	//如果账户是结算操作
	if (operation == "settle")
	{
		cout << '\t' << setw(8) << this->Interest;	//输出此时利息的数量
		//加上利息
		this->Deposite += this->Interest;
		//结算完后利息清零
		this->Interest = 0;
		//额度变化：如果此时利息还清，则额度恢复到最大，否则会减少相应的额度
		this->Credict_Money = (this->Save_Crd_MONEY + this->Deposite);
		cout << '\t' << setw(8) << this->Deposite << '\t' << setw(12) << "interest";
	}
	else if (operation == "anual fee")
	{
		//加上利息并且加上年费
		this->Deposite = this->Deposite - this->anual_fee + this->Interest;
		//结算后利息清零
		this->Interest = 0;
		this->Credict_Money = (Save_Crd_MONEY + this->Deposite);
		cout << '\t' << setw(8) << -this->anual_fee << '\t' << setw(8) << this->Deposite << '\t' << setw(12) << operation;
	}
	else
	{
		//不是结算操作仅仅对额度产生变化
		Credict_Money += current_money + Interest;
		cout << '\t' << setw(8) << current_money << '\t' << setw(8) << this->Deposite << '\t' << setw(12) << operation;
	}
	cout << endl;
	//显示一下可用额度
	cout << "\n\t" << '\t' << '\t' << "\tAvaliable Balance: " << this->Credict_Money << '\t';
	if (this->Interest < 0)
		cout << "The Account who loawned Interest which haven't been paid: " << this->Interest << endl << '\n';
	cout << endl;
	total += Deposite - last_Deposit;
}

//信用卡账户的存款操作
void CreditAccount::deposite(Date date, int deposite, const string operation)
{
	CrdSettle(date, deposite, operation);
}

//用户的取款操作
void CreditAccount::withdraw(Date date, int withdraw, const string operation)
{
	//每取一次款，额度都要减少
	int new_withdraw = 0;
	if (withdraw > this->Credict_Money)	//如果取款大于额度，则不能将钱取出来,并且输出此时的账户欠款额度
	{
		do
		{
			std::cout << "Your withdraw overed your credit_money! And your credit_money is: " << this->Credict_Money << '\n';
			std::cout << "Please try again and to make sure you withdraw doesn't over your deposite:";
			std::cin >> new_withdraw;
		} while (new_withdraw > this->Deposite);
		CrdSettle(date, -new_withdraw, operation);
	}
	else
		CrdSettle(date, -withdraw, operation);
}

//用户的总结算
void CreditAccount::settle(Date date)
{
	if (date.the_month == 1)
	{
		this->Credict_Money -= this->anual_fee;
		CrdSettle(date, 0, "anual fee");
	}
	else
		CrdSettle(date, 0, "settle");
}

void CreditAccount::show()
{
	cout << this->date.the_year << '-' << this->date.the_month << '-' << this->date.the_day << '\t' << setw(10) << this->Idcode;
	cout << '\t' << setw(8) << "Banlance: " << this->Deposite << '\t' << "Interest: " << this->Interest << '\t';
	cout << "Avaliable Banlance: " << this->Credict_Money << endl;
}