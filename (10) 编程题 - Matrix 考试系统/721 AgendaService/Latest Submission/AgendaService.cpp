#include <iostream>
#include "AgendaService.hpp"
#include <list>
#include <string>
#include "Storage.hpp"
#include "Meeting.hpp"
using namespace std ;
  /**
   * list all meetings the user take part in
   * @param userName user's username
   * @return a meeting list result
   */
  std::list<Meeting> AgendaService::listAllMeetings(const std::string &userName) const
  {
    list<Meeting> rightmeeting = m_storage -> queryMeeting( [&] (const Meeting & meeting ) -> bool 
      {
        if( meeting.getSponsor() == userName || meeting.isParticipator(userName) ) 
        {
          return true  ;
        }
        else
        {
          return false ;
        }
      } 
    );
    return rightmeeting ;
  }
  
  /**
   * list all meetings the user sponsor
   * @param userName user's username
   * @return a meeting list result
   */
  std::list<Meeting> AgendaService::listAllSponsorMeetings(const std::string &userName) const
  {
    list<Meeting> rightmeeting = m_storage -> queryMeeting( [&] ( const Meeting & meeting ) -> bool 
      {
        if( meeting.getSponsor() == userName )
        {
          return true ; 
        } 
        else
        {
          return false ;
        }
      }
    ) ;
    return rightmeeting ;
  }


  /**
   * list all meetings the user take part in and sponsor by other
   * @param userName user's username
   * @return a meeting list result
   */
  std::list<Meeting> AgendaService::listAllParticipateMeetings(
      const std::string &userName) const
  {
    list<Meeting> rightmeeting = m_storage -> queryMeeting( [&] ( const Meeting & meeting ) -> bool 
      {
        if( meeting.isParticipator(userName) )
        {
          return true ;
        }
        else
        {
          return false ;
        }
      }
    );
    return rightmeeting ;
  }



  /**
   * constructor
   */
  AgendaService::AgendaService()
  { 
    //cout << "1"  << endl ;
    startAgenda() ;
    //cout << "1" << endl ;
  }

  /**
   * destructor
   */
  AgendaService::~AgendaService()
  {
    quitAgenda() ;
  }

  /**
   * check if the username match password
   * @param userName the username want to login
   * @param password the password user enter
   * @return if success, true will be returned
   */
  bool AgendaService::userLogIn(const std::string &userName, const std::string &password)
  {  
     list<User> find ;
     User temp(userName,password,"","") ;
     find = m_storage -> queryUser( [&] (const User & user ) -> bool
      {
         if(user.getName() == temp.getName())
         {
          return true ;
         }
         else
         {
          return false ;
         }
      } 
     );

     if ( find.size() == 0 ) 
     {
       return false ;
     }
     if( password == find.begin() -> getPassword() )
     {
      return true ;
     }
     else
     {
      return false ;
     }
  }

  /**
   * regist a user
   * @param userName new user's username
   * @param password new user's password
   * @param email new user's email
   * @param phone new user's phone
   * @return if success, true will be returned
   */
  bool AgendaService::userRegister(const std::string &userName, const std::string &password,
                    const std::string &email, const std::string &phone)
  {

    list<User> find ;
    User temp(userName,password,email,phone) ;
    find = m_storage -> queryUser( [&](const  User & user ) -> bool 
      {
        if(user.getName() == temp.getName())
        {
          return true ;
        }
        else{
          return false ;
        }
      }
    );
    if(find.size()!=0)
    {
      return false ;
    }
    m_storage -> createUser(temp) ;
    return true ;
  }

  /**
   * delete a user
   * @param userName user's username
   * @param password user's password
   * @return if success, true will be returned
   */
bool AgendaService::deleteUser(const std::string &userName, const std::string &password)
  {
    list<User> find ;
    find = m_storage -> queryUser([&](const User &user ) -> bool 
      {
        if( user.getName() == userName )
        {
          return true ;
        }
        else
        {
          return false ;
        }
      }
    );
    if(find.size() == 0 )
    {
      return false ;
    }
    for( auto &i : find )
    {
      if( i.getPassword() == password )
      {
        m_storage -> deleteMeeting( [&](const Meeting &meeting ) -> bool 
            {
              if( meeting.getSponsor() == userName )
              {
                return true ;
              }
              else
              {
                return false ;
              }
            } 
          );
        list<Meeting> all_meeting = m_storage -> queryMeeting( [&]( const Meeting & meeting ) -> bool {return true ;} ) ;
        for( auto s : all_meeting )
        {
          for( auto x : s.getParticipator() )
          {
            if( userName == x )
            {
              removeMeetingParticipator(s.getSponsor(),s.getTitle(),userName) ;
            }
          }
        }
        m_storage -> deleteUser([&] (const User & user ) -> bool 
          {
            if(user.getName() == userName )
            {
              return true ;
            }
            else
            {
              return false ;
            }
          }
        );
        m_storage -> sync() ;
        return true ;
      }
    }
    return false ;
  }

  /**
   * list all users from storage
   * @return a user list result
   */
  std::list<User> AgendaService::listAllUsers(void) const
  {
     list<User> find = m_storage -> queryUser( [&]( const User &user ) -> bool 
        {
          return true ;
        } 
      ) ;
     return find ;
  }

  /**
   * create a meeting
   * @param userName the sponsor's userName
   * @param title the meeting's title
   * @param participator the meeting's participator
   * @param startData the meeting's start date
   * @param endData the meeting's end date
   * @return if success, true will be returned
   */
  bool AgendaService::createMeeting(const std::string &userName, const std::string &title,
                     const std::string &startDate, const std::string &endDate,
                     const std::vector<std::string> &participator)
  {
    for( auto it = participator.begin() ; it != participator.end() ; it ++ )
    {
      for(auto itj = participator.begin() ; itj != it ; itj ++ )
      {
        if( (*itj) == (*it))
        {
          return false ;
        }
      }
    }
    Date sTime ;
    Date eTime ;
    sTime = Date::stringToDate(startDate) ;
    eTime = Date::stringToDate(endDate) ;
    if(!Date::isValid(sTime))
    {
      return false ;
    }
    if(!Date::isValid(eTime))
    {
      return false ;
    }
    if( sTime >= eTime )
    {
      return  false ;
    }
    for(auto s : participator)
    {
      if( userName == s )
      {
        return false ;
      }
    }
    list<User> find ;
    list<Meeting> sind;
    for(auto it = participator.begin() ; it != participator.end() ; it ++ )
    {
      if( (*it) == userName )
      {
        return false ;
      }
      string puserName = *it ; 
      find = m_storage -> queryUser([&] (const User & user)  -> bool 
        {
            if(puserName == user.getName())
            {
              return true  ;
            }
            else{
              return false ;
            }
        }
      );
      if( find.size() == 0 )
      {
        return false ;
      }
      sind = m_storage -> queryMeeting([&](const Meeting & meeting)  -> bool 
        {
            if(meeting.getSponsor() == userName || meeting.isParticipator(userName))
          {
            if( !(meeting.getStartDate() >= eTime || meeting.getEndDate() <= sTime))
              {
                return true ;
              }
            return false ;
          }
          else
          {
            return false ;
          }
        }
      );
      if(sind.size()!=0)
      {
        return false ;
      }    
    }
    find = m_storage -> queryUser([&](const User & user) -> bool 
      { 
        if(user.getName() == userName)
        {
          return true ;
        }
        else{
          return false ;
        }
      }
    );
    if(find.size()==0)
    {
      return false ;
    }
    sind = m_storage -> queryMeeting([&](const Meeting &meeting) -> bool 
      {
          if(meeting.getSponsor() == userName || meeting.isParticipator(userName))
          {
            if( !(meeting.getStartDate() >= eTime || meeting.getEndDate() <= sTime))
              {
                return true ;
              }
            return false ;
          }
          else
          {
            return false ;
          }
      }
    );
    if(sind.size()!=0)
    {
      return false ;
    }
    sind = m_storage -> queryMeeting([&](const Meeting &meeting) -> bool 
      { 
          if(meeting.getTitle() == title )
          {
            return true ;
          }
          else
          {
            return false ;
          }
      }
    );
    if(sind.size()!= 0)
    {
      return false ;
    }
    Meeting temp(userName,participator,sTime,eTime,title) ;
    m_storage -> createMeeting(temp) ;
    return true ;
  }



  /**
   * add a participator to a meeting
   * @param userName the sponsor's userName
   * @param title the meeting's title
   * @param participator the meeting's participator
   * @return if success, true will be returned
   */
  bool AgendaService::addMeetingParticipator(const std::string &userName,
                              const std::string &title,
                              const std::string &participator)
  { 
    list<User> all_users = listAllUsers() ;
    int user_judge = 0 ;
    for( auto s : all_users )
    {
      if( s.getName() == participator )
      {
        user_judge = 1 ; 
      }
    }
    if( user_judge == 0 )
    {
      return false ;
    }
    list<Meeting> all_meeting = m_storage -> queryMeeting( [&]( const Meeting & meeting ) -> bool {return true ;} ) ;
    Meeting my_meeting ;
    int judge = 0 ;
    for( auto s : all_meeting )
    {
      if( s.getSponsor() == userName && s.getTitle() == title )
      {
        my_meeting = s ;
        judge ++ ;
      }
    }
    if(judge == 0 )
    {
      return false ;
    }
    if(my_meeting.isParticipator(participator))
    {
      return false ;
    }
    if(my_meeting.getSponsor() == participator)
    {
      return false ;
    }
    list<Meeting> first =  m_storage -> queryMeeting( [&]( const Meeting & meeting ) -> bool 
      { 
        return ( meeting.getSponsor() == participator ) ;
      } 
    ) ;
    list<Meeting> second =  m_storage -> queryMeeting( [&]( const Meeting & meeting ) -> bool 
      { 
        if(meeting.getParticipator().size() == 0 )
        {
          return false ;
        }
        for( auto  s : meeting.getParticipator() )
        { 
          if(s == participator){
            return true ;
          }
        }
        return false ;
      } 
    );
    Date start = my_meeting.getStartDate() ;
    Date end = my_meeting.getEndDate() ;
    if( start >= end )
    {
      return false ;
    }
    for(auto s : first) 
    { 
      if(!(end <= s.getStartDate() || start >= s.getEndDate()))
      {
        return false ;
      }
    }
    for(auto s : second) 
    { 
      if(!(end <= s.getStartDate() || start >= s.getEndDate()))
      {
        return false ;
      }     
    }
    return m_storage -> updateMeeting([userName,title](const Meeting & meeting) -> bool 
      { 
        return ( meeting.getTitle() == title && meeting.getSponsor() == userName ) ;
      } , [participator](Meeting & meeting){
             meeting.addParticipator(participator);
        } 
    );
  }

  /**
   * remove a participator from a meeting
   * @param userName the sponsor's userName
   * @param title the meeting's title
   * @param participator the meeting's participator
   * @return if success, true will be returned
   */
  bool AgendaService::removeMeetingParticipator(const std::string &userName,
                                 const std::string &title,
                                 const std::string &participator)
  { 
    list<User> all_users = listAllUsers() ;
    int user_judge = 0 ;
    for( auto s : all_users )
    {
      if( s.getName() == participator )
      {
        user_judge = 1 ; 
      }
    }
    if( user_judge == 0 )
    {
      return false ;
    }
    list<Meeting> all_meeting = m_storage -> queryMeeting( [&]( const Meeting & meeting ) -> bool {return true ;} ) ;
    Meeting my_meeting ;
    int judge = 0 ;
    for( auto s : all_meeting )
    {
      if( s.getSponsor() == userName && s.getTitle() == title )
      {
        my_meeting = s ;
        judge ++ ;
      }
    }
    if(judge == 0 )
    {
      return false ;
    }
    if(!my_meeting.isParticipator(participator))
    {
      return false ;
    }
    if(my_meeting.getSponsor() == participator)
    {
      return false ;
    }
    m_storage -> updateMeeting([userName,title](const Meeting & meeting) -> bool 
      { 
        return ( meeting.getTitle() == title && meeting.getSponsor() == userName ) ;

      } , [participator](Meeting & meeting){
             meeting.removeParticipator(participator);
          } 
    ); 
    if( my_meeting.getParticipator().size() == 1 && my_meeting.isParticipator(participator) )
    {
      deleteMeeting(userName,title) ;
    }
    return true ;
  }

  /**
   * quit from a meeting
   * @param userName the current userName. no need to be the sponsor
   * @param title the meeting's title
   * @return if success, true will be returned
   */
  bool AgendaService::quitMeeting(const std::string &userName, const std::string &title)
  {  
    list<Meeting> all_meeting = m_storage -> queryMeeting( [&]( const Meeting & meeting ) -> bool {return true ;} ) ;
    Meeting my_meeting ;
    int judge = 0 ;
    for( auto s : all_meeting )
    {
      if( s.getSponsor() == userName && s.getTitle() == title )
      {
        my_meeting = s ;
        judge ++ ;
      }
    }
    if(judge == 0 )
    {
      return false ;
    }
    if(!my_meeting.isParticipator(userName))
    {
      return false ;
    }
    if(my_meeting.getSponsor() == userName)
    {
      return false ;
    }

    m_storage -> updateMeeting([title](const Meeting & meeting) -> bool 
      { 
        return ( meeting.getTitle() == title ) ;
      } , [userName](Meeting & meeting){
             meeting.removeParticipator(userName);
          } 
    );

    if(my_meeting.getParticipator().size() == 1 && my_meeting.isParticipator(userName))
    {
      deleteMeeting(my_meeting.getSponsor(),title) ;
    }
    return true ;
  }

  /**
   * search a meeting by username and title
   * @param uesrName the sponsor's userName
   * @param title the meeting's title
   * @return a meeting list result
   */
  std::list<Meeting> AgendaService::meetingQuery(const std::string &userName,
                                  const std::string &title) const
  {
    list<Meeting> rightmeeting = m_storage -> queryMeeting([&] (const Meeting &meeting) -> bool 
      {
        if(meeting.getSponsor() == userName || meeting.isParticipator(userName) )
        {
          if(meeting.getTitle() == title)
          {
            return true ;
          }
          else
          {
            return false ;
          }
        }
        else
        {
          return false ;
        }
      }
    ) ;
    return rightmeeting ;
  }
  /**
   * search a meeting by username, time interval
   * @param uesrName the sponsor's userName
   * @param startDate time interval's start date
   * @param endDate time interval's end date
   * @return a meeting list result
   */
  std::list<Meeting> AgendaService::meetingQuery(const std::string &userName,
                                  const std::string &startDate,
                                  const std::string &endDate) const
  {
    Date start = Date::stringToDate(startDate) ;
    Date end = Date::stringToDate(endDate) ;
    list<Meeting>  rightmeeting ;
    Date sTime = Date::stringToDate(startDate) ;
    Date eTime = Date::stringToDate(endDate) ;
    if(!Date::isValid(sTime))
    {
      return rightmeeting ;
    }
    if(!Date::isValid(eTime))
    {
      return rightmeeting ;
    }
    if( sTime >= eTime )
    {
      return rightmeeting ;
    }
    rightmeeting = m_storage -> queryMeeting([&] (const Meeting & meeting) -> bool 
      {
          if(meeting.getSponsor() == userName || meeting.isParticipator(userName))
            {
              if((meeting.getStartDate() >= sTime && meeting.getEndDate() <= eTime))
                {
                  return true ;
                }
              return false ;
            }
          else
            {
              return false ;
            }
      } 
    );
    return rightmeeting ;
  }

  /**
   * delete a meeting by title and its sponsor
   * @param userName sponsor's username
   * @param title meeting's title
   * @return if success, true will be returned
   */
  bool AgendaService::deleteMeeting(const std::string &userName, const std::string &title)
  {
    int find = m_storage -> deleteMeeting( [&] (const Meeting &meeting ) -> bool 
    {
      if ( userName == meeting.getSponsor() && title == meeting.getTitle() )
      {
          return true ;
      }
      else
      {
          return false ;
      }
    }
    ) ;
    if( find > 0 )
    {
      return true ;
    } 
    else
    {
      return false ;
    }
  }

  /**
   * delete all meetings by sponsor
   * @param userName sponsor's username
   * @return if success, true will be returned
   */
  bool AgendaService::deleteAllMeetings(const std::string &userName)
  {
    int find = m_storage -> deleteMeeting( [&] (const Meeting &meeting ) -> bool 
      {
        if( meeting.getSponsor() == userName )
        {
          return true ;
        }
        else
        {
          return false ;
        }
      }
    ) ;
    if( find > 0 )
    {
      return true ;
    }
    else
    {
      return false ;
    }
  }

  /**
   * start Agenda service and connect to storage
   */
  void AgendaService::startAgenda()
  {
    m_storage = Storage::getInstance() ;
  }

  /**
   * quit Agenda service
   */
  void AgendaService::quitAgenda() {
    m_storage -> sync() ;
  } 



