#include<iostream>
#include"Date.h"

//闰年的判断
bool Year_judgement(int year);

const int The_Max_Day_of_Month[] = { 0, 31, 59, 90, 120, 151, 181, 212, 243, 273, 304, 334, 365 };

//获得账户的建立日期以及更新日期
void Date::Set_Date(int year, int month, int day)
{
	this->the_year = year;
	this->the_month = month;
	this->the_day = day;
}

//日期的计算
int Date::Days_of_Cal(Date date)
{
	int days= (date.the_year - this->the_year) * 365 + (date.the_year - this->the_year) / 4 - (date.the_year - this->the_year) / 100 +
		The_Max_Day_of_Month[date.the_month-1] - The_Max_Day_of_Month[this->the_month-1] + date.the_day - this->the_day;
	if (Year_judgement(date.the_year) && date.the_month > 2)
		return days++;
	return days;
}

//返回一个月的天数来判断日期的修改正确与否
int Date::The_Max_Day_of_Month_Function()
{
	if (Year_judgement(the_year) && the_month == 2)
		return 29;
	else
		return The_Max_Day_of_Month[the_month] - The_Max_Day_of_Month[the_month - 1];
}

//闰年的判断
bool Year_judgement(int year)
{
	return ((year % 4 == 0 && year % 100 != 0) || year % 400 == 0) ? true : false;
}
