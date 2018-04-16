#include "Date.hpp"
#include <string>
#include <sstream>


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

  /**
  * @brief constructor with a string
  */
  Date::Date(const std::string &dateString)
  {
   if( dateString.size() != 16 ){
      m_year = 0 ;
      m_month = 0 ;
      m_day = 0 ;
      m_hour = 0 ;
      m_minute = 0 ;
      return  ;
   }
   bool judge = dateString[0] >= '1' &&  dateString[0] <= '9' &&
                dateString[1] >= '0' &&  dateString[1] <= '9' &&
                dateString[2] >= '0' &&  dateString[2] <= '9' &&
                dateString[3] >= '0' &&  dateString[3] <= '9' &&
                dateString[5] >= '0' &&  dateString[5] <= '1' &&
                dateString[6] >= '0' &&  dateString[6] <= '9' &&
                dateString[8] >= '0' &&  dateString[8] <= '3' &&
                dateString[9] >= '0' &&  dateString[9] <= '9' &&
                dateString[11] >= '0' &&  dateString[11] <= '2' &&
                dateString[12] >= '0' &&  dateString[12] <= '3' &&
                dateString[14] >= '0' &&  dateString[14] <= '5' &&
                dateString[15] >= '0' &&  dateString[15] <= '9' &&
                dateString[4] == '-' &&  dateString[7] == '-' &&
                dateString[10] == '/' &&  dateString[13] == ':'  ;

    if( judge && dateString.size() == 16 ){

     m_year = ( dateString[0] - '0' ) * 1000 + ( dateString[1] - '0') * 100 +
              ( dateString[2] - '0' ) * 10 + (dateString[3] - '0') ;

     m_month = ( dateString[5] - '0' ) * 10 + (dateString[6] - '0') ;

     m_day = ( dateString[8] - '0' ) * 10 + (dateString[9] - '0') ;

     m_hour = ( dateString[11] - '0' ) * 10 + (dateString[12] - '0') ;

     m_minute = ( dateString[14] - '0' ) * 10 + (dateString[15] - '0') ;
    }
    else{
        m_year = 0 ;
        m_month = 0 ;
        m_day = 0 ;
        m_minute = 0 ;
        m_hour = 0 ;
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
   int monthday[12] = {31,28,31,30,31,30,31,31,30,31,30,31} ;
    int leap_monthday[12] = {31,29,31,30,31,30,31,31,30,31,30,31} ;
    if( (t_date.getYear() % 4 == 0 && t_date.getYear() % 100 != 0) || t_date.getYear() % 400 == 0){
     if(t_date.getDay() < 1 || t_date.getDay() > leap_monthday[t_date.getMonth()-1]){
      return false ;
     }
     else{
      return true ;
     }
    }
    else{
     if(t_date.getDay() < 1 ||  t_date.getDay() > monthday[t_date.getMonth()-1]){
      return false ;
     }
     else{
      return true ;
     }
    }
  }

  /**
  * @brief convert a string to date, if the format is not correct return
  * 0000-00-00/00:00
  * @return a date
  */
  Date Date::stringToDate(const std::string &dateString)
  { 
    if( dateString.size()!=16 ) 
    {
       return Date("0000-00-00/00:00") ;
    }
   bool judge = dateString[0] >= '1' &&  dateString[0] <= '9' &&
                dateString[1] >= '0' &&  dateString[1] <= '9' &&
                dateString[2] >= '0' &&  dateString[2] <= '9' &&
                dateString[3] >= '0' &&  dateString[3] <= '9' &&
                dateString[5] >= '0' &&  dateString[5] <= '1' &&
                dateString[6] >= '0' &&  dateString[6] <= '9' &&
                dateString[8] >= '0' &&  dateString[8] <= '3' &&
                dateString[9] >= '0' &&  dateString[9] <= '9' &&
                dateString[11] >= '0' &&  dateString[11] <= '2' &&
                dateString[12] >= '0' &&  dateString[12] <= '3' &&
                dateString[14] >= '0' &&  dateString[14] <= '5' &&
                dateString[15] >= '0' &&  dateString[15] <= '9' &&
                dateString[4] == '-' &&  dateString[7] == '-' &&
                dateString[10] == '/' &&  dateString[13] == ':'  ;
    
    if( judge && dateString.size() == 16 ){
     return Date(dateString) ;
    }
    else{
     return Date("0000-00-00/00:00") ;
    }
  }



  /**
  * @brief convert a date to string, if the date is invalid return
  * 0000-00-00/00:00
  */
  std::string Date::dateToString(const Date &t_date)
  {
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
     else{
      return "0000-00-00/00:00" ;
     }

  }

  /**
  *  @brief overload the assign operator
  */
  Date &Date::operator=(const Date &t_date)
  {
   m_year = t_date.getYear() ;
   m_month = t_date.getMonth() ;
   m_day = t_date.getDay() ;
   m_hour = t_date.getHour() ;
   m_minute = t_date.getMinute() ;
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
   return (*this == t_date || *this > t_date ) ;
  }

  /**
  * @brief check whether the CurrentDate is  less than or equal to the t_date
  */
  bool Date::operator<=(const Date &t_date) const
  {
   return ((*this) < t_date || *this == t_date ) ;
  }


