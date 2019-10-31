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

//��ʾ��ȡ����Ϣ������Լ���Ӧ�Ĳ���
void SavingAccount::show()
{
	cout << this->date.the_year << '-' << this->date.the_month << '-' << this->date.the_day << '\t' << setw(10) << this->Idcode;
	cout << '\t' << setw(8) << "Banlance: " << this->Deposite << '\t' << "Interest: " << this->Interest << '\t' << this->operation << endl;
}

//������Ϣ����ÿ���д�ȡ�����ʱ��Ҫ����
void SavingAccount::SavActCalc(Date date, int current_farewell)
{
	this->Interest += floor((this->rate * (this->Deposite * this->date.Days_of_Cal(date))) / 365 * 100 + 0.5) / 100;
	this->Deposite += current_farewell;
	this->total += current_farewell;
}

//��Ϣ���ܽ��㣬���Ҫ���ϴ�ʱ���˻����
void SavingAccount::settle(Date date)
{
	this->Interest += floor((this->rate * (this->Deposite * this->date.Days_of_Cal(date))) / 365 * 100 + 0.5) / 100;
	//������Ϣ���������������365��
	this->Deposite += this->Interest;	//������Ϣ
	this->date.Set_Date(date.the_year, date.the_month, date.the_day);
	this->show();
	//�������Ϣ����
	this->Interest = 0;
}

//����
void SavingAccount::deposite(Date date, int deposite, const std::string operation)
{
	if (this->Deposite == 0)	//����ǵ�һ�δ��
	{
		this->Deposite += deposite;	//������
		this->date.Set_Date(date.the_year, date.the_month, date.the_day);	//���˻��Ĳ�������ˢ��
		this->operation = operation;	//��¼�˻��Ĳ�����Ϣ
		this->total += deposite;
		show();
		return;
	}
	else
		SavActCalc(date, deposite);
	this->date.Set_Date(date.the_year, date.the_month, date.the_day);	//���˻��Ĳ�������ˢ��
	this->operation = operation;	//��¼�˻��Ĳ�����Ϣ
	show();
}

//ȡ���
void SavingAccount::withdraw(Date date, int withdraw, const std::string operation)
{
	int new_withdraw = 0;
	if (withdraw > this->Deposite)	//���ȡ����ڴ����ܽ�Ǯȡ����
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
	this->date.Set_Date(date.the_year, date.the_month, date.the_day);	//��������
	show();		//��ʾ�����Ϣ
}

//���캯����ʼ��
Account::Account(Date date, std::string idcode, double rate)
{
	this->date.Set_Date(date.the_year, date.the_month, date.the_day);
	this->Idcode = idcode;
	this->rate = rate; //��ʼ��ÿ���˻��Ľ���������Ϣ�Լ�Id������
	cout << this->date.the_year << '-' << this->date.the_month << '-' << this->date.the_day << '\t' << setw(10) << this->Idcode;
	cout << " created" << endl;
}

//���������е��û���Ĺ��캯��
SavingAccount::SavingAccount(Date date, std::string idcode, double rate) :Account(date, idcode, rate) {}

//ͬ��
CreditAccount::CreditAccount(Date date, std::string idcode, double rate) : Account(date, idcode, rate) {}

//����˻����ö�Ⱥ�����Լ�����
void CreditAccount::get_Crd_Anf(double Credict_Money, double anual_fee)
{
	this->Credict_Money = Credict_Money;
	this->Save_Crd_MONEY = Credict_Money;	//��������洢�����
	this->anual_fee = anual_fee;
}

//���ÿ��˻��Ľ�����
void CreditAccount::CrdSettle(Date date, int current_money, string operation)
{
	last_Deposit = this->Deposite;
	//��ȡ�������,����������
	int days = this->date.Days_of_Cal(date);
	this->date.Set_Date(date.the_year, date.the_month, date.the_day);		//��������

	//������Ϣ
	this->Interest += this->Deposite >= 0 ? 0 : floor(this->Deposite * this->rate * days * 100 + 0.5) / 100;

	//��ȱ䶯
	this->Deposite += current_money;

	//�����ǰ��������
	cout << date.the_year << '-' << date.the_month << '-' << date.the_day << '\t' << setw(10) << this->Idcode;

	//����˻��ǽ������
	if (operation == "settle")
	{
		cout << '\t' << setw(8) << this->Interest;	//�����ʱ��Ϣ������
		//������Ϣ
		this->Deposite += this->Interest;
		//���������Ϣ����
		this->Interest = 0;
		//��ȱ仯�������ʱ��Ϣ���壬���Ȼָ�����󣬷���������Ӧ�Ķ��
		this->Credict_Money = (this->Save_Crd_MONEY + this->Deposite);
		cout << '\t' << setw(8) << this->Deposite << '\t' << setw(12) << "interest";
	}
	else if (operation == "anual fee")
	{
		//������Ϣ���Ҽ������
		this->Deposite = this->Deposite - this->anual_fee + this->Interest;
		//�������Ϣ����
		this->Interest = 0;
		this->Credict_Money = (Save_Crd_MONEY + this->Deposite);
		cout << '\t' << setw(8) << -this->anual_fee << '\t' << setw(8) << this->Deposite << '\t' << setw(12) << operation;
	}
	else
	{
		//���ǽ�����������Զ�Ȳ����仯
		Credict_Money += current_money + Interest;
		cout << '\t' << setw(8) << current_money << '\t' << setw(8) << this->Deposite << '\t' << setw(12) << operation;
	}
	cout << endl;
	//��ʾһ�¿��ö��
	cout << "\n\t" << '\t' << '\t' << "\tAvaliable Balance: " << this->Credict_Money << '\t';
	if (this->Interest < 0)
		cout << "The Account who loawned Interest which haven't been paid: " << this->Interest << endl << '\n';
	cout << endl;
	total += Deposite - last_Deposit;
}

//���ÿ��˻��Ĵ�����
void CreditAccount::deposite(Date date, int deposite, const string operation)
{
	CrdSettle(date, deposite, operation);
}

//�û���ȡ�����
void CreditAccount::withdraw(Date date, int withdraw, const string operation)
{
	//ÿȡһ�ο��ȶ�Ҫ����
	int new_withdraw = 0;
	if (withdraw > this->Credict_Money)	//���ȡ����ڶ�ȣ����ܽ�Ǯȡ����,���������ʱ���˻�Ƿ����
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

//�û����ܽ���
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