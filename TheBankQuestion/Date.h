#pragma once
#include<iostream>

#include<string>

using std::string;

const int number = 3;

class Date
{
public:

	//定义三个数据成员
	int the_year = 0;
	int the_month = 0;
	int the_day = 0;  //定义Date的年、月、日
	Date() {};
	Date(int year, int month, int day) { Set_Date(year, month, day); };
	void Set_Date(int year, int month, int day);
	int The_Max_Day_of_Month_Function();
	int Days_of_Cal(Date date);
	bool operator !=(Date date) const	//Date的运算符 != 重载
	{
		if (this->the_year == date.the_year && this->the_month == date.the_month && this->the_day == date.the_day)
			return false;
		return true;
	}
	void operator =(Date date)	//Date的运算符 = 重载
	{
		this->the_year = date.the_year;
		this->the_month = date.the_month;
		this->the_day = date.the_day;
	}

	bool operator <=(Date date) const //Date的运算符 <= 重载 判断日期的小于
	{
		if (this->the_year < date.the_year)
			return true;
		else if (this->the_year == date.the_year && this->the_month < date.the_month)
			return true;
		else if (this->the_year == date.the_year && this->the_month == date.the_month && this->the_day <= date.the_day)
			return true;
		return false;
	}
	void show() const {
		std::cout << this->the_year << '-' << this->the_month << '-' << this->the_day << '\t';
	}
};