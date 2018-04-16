#include "Meeting.hpp"
#include "Date.hpp"
#include <iostream>
#include <string>
#include <vector>

  /**
  * @brief default constructor
  */
  /**
  *   @brief constructor with argument
  */
  Meeting::Meeting(const std::string &t_sponsor,
          const std::vector<std::string> &t_participator,
          const Date &t_startTime, const Date &t_endTime,
          const std::string &t_title)
  {
     m_sponsor = t_sponsor ;
     m_startDate = t_startTime ;
     m_endDate = t_endTime ;
     std::vector<std::string> vec(t_participator) ;
     m_participators = vec ;
     m_title = t_title ;
  }

  /**
  * @brief copy constructor of left value
  */
  Meeting::Meeting(const Meeting &t_meeting)
  {
     m_sponsor = t_meeting.m_sponsor ;
     m_startDate = t_meeting.m_startDate ;
     m_endDate = t_meeting.m_endDate ;
     std::vector<std::string> vec(t_meeting.m_participators) ;
     m_participators = vec ;
     m_title = t_meeting.m_title ;
  }

  /**
  *   @brief get the meeting's sponsor
  *   @return a string indicate sponsor
  */
  std::string Meeting::getSponsor(void) const
  {
   return m_sponsor ;
  }

  /**
  * @brief set the sponsor of a meeting
  * @param  the new sponsor string
  */
  void Meeting::setSponsor(const std::string &t_sponsor)
  {
   m_sponsor = t_sponsor ;
  }

  /**
  * @brief  get the participators of a meeting
  * @return return a string vector indicate participators
  */
  std::vector<std::string> Meeting::getParticipator(void) const
  {
    return m_participators ;
  }

  /**
  *   @brief set the new participators of a meeting
  *   @param the new participators vector
  */
  void Meeting::setParticipator(const std::vector<std::string> &t_participators)
  {
    std::vector<std::string> vec(t_participators) ;
    m_participators = vec ;
  }

  /**
  * @brief add a new participator to the meeting
  * @param the new participator
  */
  void Meeting::addParticipator(const std::string &t_participator)
  {
   for(auto s:m_participators){
    if(s == t_participator){
     return ;
    }
   }
    m_participators.push_back(t_participator) ;
  }

  /**
  * @brief remove a participator of the meeting
  * @param the participator to be removed
  */
  void Meeting::removeParticipator(const std::string &t_participator)
  {
   auto iter = m_participators.begin() ;
   for( ; iter != m_participators.end() ; )
   {
    if( (*iter) == t_participator )
    {
     iter = m_participators.erase(iter) ;
     return ;
    }

    else
    {
     iter ++ ;
    }
   }
  }

  /**
  * @brief get the startDate of a meeting
  * @return return a string indicate startDate
  */
  Date Meeting::getStartDate(void) const
  {
   return m_startDate ;
  }

  /**
  * @brief  set the startDate of a meeting
  * @param  the new startdate of a meeting
  */
  void Meeting::setStartDate(const Date &t_startTime)
  {
   m_startDate = t_startTime ;
  }

  /**
  * @brief get the endDate of a meeting
  * @return a date indicate the endDate
  */
  Date Meeting::getEndDate(void) const
  {
    return m_endDate ;
  }

  /**
  * @brief  set the endDate of a meeting
  * @param  the new enddate of a meeting
  */
  void Meeting::setEndDate(const Date &t_endTime)
  {
   m_endDate = t_endTime ;
  }

  /**
  * @brief get the title of a meeting
  * @return a date title the endDate
  */
  std::string Meeting::getTitle(void) const
  {
   return m_title ;
  }

  /**
  * @brief  set the title of a meeting
  * @param  the new title of a meeting
  */
  void Meeting::setTitle(const std::string &t_title)
  {
   m_title = t_title ;
  }

  /**
  * @brief check if the user take part in this meeting
  * @param t_username the source username
  * @return if the user take part in this meeting
  */
  bool Meeting::isParticipator(const std::string &t_username) const
  {
    for(auto s : m_participators )
    {
     if(t_username == s)
     {
      return true ;
     }
    }
    return false ;
  }

