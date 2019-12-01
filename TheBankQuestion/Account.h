#pragma once

//包含 Date 的头文件
#include"Date.h"

#include<string>
using std::string;

//创建基类账户
class Account
{
public:
	static double total;
public:
	Date date;
	double Interest = 0;	//这是利息
	double Deposite = 0;	//这是存款
	void showtotal();
	std::string Idcode = "noname", operation = "nooperation";
	double rate = 0;	//分为储蓄账户的利率和信用卡账户的利率
	Account() {};
	Account(Date date, std::string idcode, double rate);	
	virtual void deposite(Date date, double deposite, const string operation) = 0;
	virtual void withdraw(Date date, double deposite, const string operation) = 0;
	virtual void settle(Date date) = 0;
	virtual void show()=0;
};

//创建储蓄类账户
class SavingAccount :public Account
{
private:
	double Interest = 0;
public:
	SavingAccount() {};
	SavingAccount(Date date, std::string idcode, double rate);
	void SavActCalc(Date date, int current_farewell);
	virtual void settle(Date date);
	virtual void deposite(Date date, double deposite, const std::string operation);	//存款操作
	virtual void withdraw(Date date, double withdraw, const std::string operation);	//取款操作
	virtual void show();
};

//创建信用卡账户
class CreditAccount :public Account
{
	double Credict_Money = 0, anual_fee = 0;
	double Save_Crd_MONEY = 0;
	double Interest = 0;
public:
	CreditAccount() {};
	CreditAccount(Date date, std::string idcode, double rate);
	void get_Crd_Anf(double Credict_Money, double anual_fee);
	void CrdSettle(Date date, int curren_money, string operation);
	virtual void deposite(Date date, double deposite, const string operation);
	virtual void withdraw(Date date, double deposite, const string operation);
	virtual void settle(Date date);
	virtual void show();
};

