#pragma once

//���� Date ��ͷ�ļ�
#include"Date.h"

#include<string>
using std::string;

//���������˻�
class Account
{
public:
	static double total;
public:
	Date date;
	double Interest = 0;	//������Ϣ
	double Deposite = 0;	//���Ǵ��
	void showtotal();
	std::string Idcode = "noname", operation = "nooperation";
	double rate = 0;	//��Ϊ�����˻������ʺ����ÿ��˻�������
	Account() {};
	Account(Date date, std::string idcode, double rate);	
	virtual void deposite(Date date, double deposite, const string operation) = 0;
	virtual void withdraw(Date date, double deposite, const string operation) = 0;
	virtual void settle(Date date) = 0;
	virtual void show()=0;
};

//�����������˻�
class SavingAccount :public Account
{
private:
	double Interest = 0;
public:
	SavingAccount() {};
	SavingAccount(Date date, std::string idcode, double rate);
	void SavActCalc(Date date, int current_farewell);
	virtual void settle(Date date);
	virtual void deposite(Date date, double deposite, const std::string operation);	//������
	virtual void withdraw(Date date, double withdraw, const std::string operation);	//ȡ�����
	virtual void show();
};

//�������ÿ��˻�
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

