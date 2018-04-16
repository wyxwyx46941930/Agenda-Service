#include "Date.hpp"
#include <string>
#include <sstream>
#include <iostream>
using namespace std ;

  bool isLeapYear(int y )
  {
    if((y%4 == 0 && y % 100 != 0) || y%400 == 0) return true ;
    return false ;
  }
  /**
  * @brief default constructor
  */
  Date::Date()
  {
      m_year = 0 ;
      m_month = 0 ;
      m_day = 0 ;
      m_hour = 0 ;
      m_minute = 0 ;       
  }


  /**
  * @brief constructor with arguments
  */
  Date::Date(int t_year, int t_month, int t_day, int t_hour, int t_minute)
  {
   m_year = t_year ;
   m_month = t_month ;
   m_day = t_day ;
   m_hour = t_hour ;
   m_minute = t_minute ;
  }


  bool stringIsValid(const string &tar)
  {
    if(tar.size()!=16)
    {
      return false ;
    }
    if(tar[4] != '-' || tar[7]!= '-' || tar[10] != '/' || tar[13] != ':') return false ;
    for(int i = 0 ; i < 16 ; i ++ )
    {
      if( i != 4 && i != 7 && i != 10 && i != 13 )
      {
        if(tar[i] > '9'  || tar[i] < '0' ) return false ;
      }
    }
    return true ;
  }
  /**
  * @brief constructor with a string
  */
  Date::Date(const std::string &dateString):Date()
  {
    if(stringIsValid(dateString))
    {
      stringstream ss(dateString) ;
      char tmp ;
      ss >> m_year >> tmp ;
      ss >> m_month >> tmp ;
      ss >> m_day >> tmp ;
      ss >> m_hour >> tmp ;
      ss >> m_minute;
    }
      
  }
  /**
  * @brief return the year of a Date
  * @return   a integer indicate the year of a date
  */
  int Date::getYear(void) const
  {
   return m_year ;
  }

  /**
  * @brief set the year of a date
  * @param a integer indicate the new year of a date
  */
  void Date::setYear(const int t_year)
  { 
    m_year = t_year ;
  }

  /**
  * @brief return the month of a Date
  * @return   a integer indicate the month of a date
  */
  int Date::getMonth(void) const
  {
    return m_month ;
  }

  /**
  * @brief set the month of a date
  * @param a integer indicate the new month of a date
  */
  void Date::setMonth(const int t_month)
  {
   m_month = t_month ;
  }

  /**
  * @brief return the day of a Date
  * @return   a integer indicate the day of a date
  */
  int Date::getDay(void) const
  {
     return m_day ;
  }

  /**
  * @brief set the day of a date
  * @param a integer indicate the new day of a date
  */
  void Date::setDay(const int t_day)
  {
   m_day = t_day ;
  }

  /**
  * @brief return the hour of a Date
  * @return   a integer indicate the hour of a date
  */
  int Date::getHour(void) const
  {
    return m_hour ;
  }

  /**
  * @brief set the hour of a date
  * @param a integer indicate the new hour of a date
  */
  void Date::setHour(const int t_hour)
  {
    m_hour = t_hour ;
  }

  /**
  * @brief return the minute of a Date
  * @return   a integer indicate the minute of a date
  */
  int Date::getMinute(void) const
  {
   return m_minute ;
  }

  /**
  * @brief set the minute of a date
  * @param a integer indicate the new minute of a date
  */
  void Date::setMinute(const int t_minute)
  {
   m_minute = t_minute ;
  }

  /**
  *   @brief check whether the date is valid or not
  *   @return the bool indicate valid or not
  */
  bool Date::isValid(const Date &t_date)
  {
   if( t_date.getYear() < 1000 || t_date.getYear() > 9999 ){
    return false ;
   }
   else if(t_date.getMonth() < 1 || t_date.getMonth() > 12 ){
    return false ;
   }
   else if(t_date.getDay() < 1 || t_date.getDay() > 31 ){
    return false ;
   }
   else if( t_date.getHour() < 0 || t_date.getHour() > 23 ){
    return false ;
   }
   else if(t_date.getMinute() < 0 || t_date.getMinute() > 59){
    return false ;
   }
   int day = t_date.m_day , month = t_date.m_month , year = t_date.m_year ;
    if( month == 1 || month == 3 || month == 5 ||month == 7 ||month == 8 ||month == 10 ||month == 12) return true ;
    if(isLeapYear(year))
    {
      if(day == 31) return false ;
      if(month == 2 && (day == 30 || day ==31)) return false ;
      return true ;
    }
    else
    {
      if(day == 31) return false ;
      if(month == 2 && (day == 29 || day == 30 || day == 31 )) return false ;
      return true ;
    }
  }

  /**
  * @brief convert a string to date, if the format is not correct return
  * 0000-00-00/00:00
  * @return a date
  */
  Date Date::stringToDate(const std::string &dateString)
  { 
    if(!stringIsValid(dateString)) return Date() ;
    Date res(dateString) ;
    if(isValid(res))
    {
      return res ;
    }
    else
    {
      return Date() ;
    }
  }



  /**
  * @brief convert a date to string, if the date is invalid return
  * 0000-00-00/00:00
  */
  std::string Date::dateToString(const Date &t_date)
  {   
     if(!isValid(t_date))
     {
      return "0000-00-00/00:00" ;
     }
     std::stringstream sstr;
     int judge = isValid(t_date) ;
     if( judge == 1){
        sstr << t_date.getYear() << '-' ;
        if(t_date.getMonth() < 10 )
        {
         sstr << '0' ;
        }
        sstr<< t_date.getMonth() << '-' ;
        if(t_date.getDay() < 10 )
        {
         sstr << '0' ;
        }
        sstr << t_date.getDay() << '/' ;
        if(t_date.getHour() < 10)
        {
         sstr << '0' ;
        }
        sstr << t_date.getHour() << ':' ;
        if(t_date.getMinute() < 10)
        {
         sstr << '0' ;
        }
        sstr << t_date.getMinute() ;
        return sstr.str() ;
     }
  }

  /**
  *  @brief overload the assign operator
  */
  Date &Date::operator=(const Date &t_date)
  {
   m_year = t_date.m_year ;
   m_month = t_date.m_month ;
   m_day = t_date.m_day ;
   m_hour = t_date.m_hour ;
   m_minute = t_date.m_minute ;
   return *this ;

  }

  /**
  * @brief check whether the CurrentDate is equal to the t_date
  */
  bool Date::operator==(const Date &t_date) const
  {
   if(m_year == t_date.getYear() && m_month == t_date.getMonth() && m_day == t_date.getDay() && m_hour == t_date.getHour() && m_minute == t_date.getMinute()){
    return true ;
   }
   else{
    return false ;
   }
  }

  /**
  * @brief check whether the CurrentDate is  greater than the t_date
  */
  bool Date::operator>(const Date &t_date) const{
   if( m_year > t_date.getYear() ){
    return true ;
   }
   else if (m_year == t_date.getYear()){
        if( m_month > t_date.getMonth() ){
         return true ;
        }
        else if(m_month == t_date.getMonth()){
           if(m_day > t_date.getDay()){
            return true ;
           }
           else if(m_day == t_date.getDay()){
             if(m_hour > t_date.getHour()){
              return true ;
             }
             else if(m_hour == t_date.getHour() ){
                if(m_minute > t_date.getMinute()){
                 return true ;
                }
                else{
                    return false ;
                }
             }
             else {
              return false ;
             }
           }
           else{
            return  false;
           }
        }
        else{
         return false ;
        }
   }
   else{
    return false ;
   }
  }

  /**
  * @brief check whether the CurrentDate is  less than the t_date
  */
  bool Date::operator<(const Date &t_date) const
  {
   return !(*this == t_date || *this > t_date ) ;
  }

  /**
  * @brief check whether the CurrentDate is  greater or equal than the t_date
  */
  bool Date::operator>=(const Date &t_date) const
  {
   return !(*this < t_date ) ;
  }

  /**
  * @brief check whether the CurrentDate is  less than or equal to the t_date
  */
  bool Date::operator<=(const Date &t_date) const
  {
   return !((*this) > t_date) ;
  }


