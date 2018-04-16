
#include <functional>
#include <fstream>
#include <list>
#include <memory>
#include <string>
#include <iostream>
#include <cstring>
#include <sstream>
#include <algorithm>
#include "Meeting.hpp"
#include "User.hpp"
#include "Storage.hpp"
#include "Date.hpp"
#include "Path.hpp" 
using namespace std ;
  /**

  *   default constructor
  */

  shared_ptr<Storage> Storage::m_instance = NULL ;
  
  Storage::Storage() 
  {
    m_dirty = false ;
    readFromFile() ;
  }
vector<string> doSomeThing(string &var)
{
  stringstream ss(var.substr(1,var.size() - 2)) ;
  vector<string> res ;
  string part ;
  while(getline(ss,part,'&'))
  {
    res.push_back(part) ;
  }
  return res ;
}
  /**
  *   read file content into memory
  *   @return if success, true will be returned
  */
  bool Storage::writeToFile(void)
  {     
    ofstream meet_file(Path::meetingPath) , user_file(Path::userPath) ;
    if(!meet_file.is_open() || !user_file.is_open())
    {
      return false ;
    }
    int index = 0 ;
    for(auto it = m_userList.begin() ; it != m_userList.end() ; it ++ )
    {
      user_file << "\"" << it -> getName() << "\"" << "," ;
      user_file << "\"" << it -> getPassword() << "\"" << "," ;
      user_file << "\"" << it -> getEmail() << "\"" << "," ;
      user_file << "\"" << it -> getPhone() << "\"" ;
      if(index < m_userList.size()-1) user_file << endl ;
    }
    index = 0 ;
    for(auto i : m_meetingList)
    {
      meet_file << "\"" << i.getSponsor() << "\"" << "," ;
      meet_file << "\"" ;
      auto participators = i.getParticipator() ;
      auto it = participators.begin() ;
      for( ; it != participators.end() , it + 1 < participators.end() ; it ++ )
      {
        if(participators.size()!=1)
        {
          meet_file << (*it) << '&' ;
        }
        else
        {
          meet_file << (*it) ;
        }
      }
      if(it != participators.end())
      {
        meet_file << (*it) ;
      }
      meet_file << "\"" << "," ;
      meet_file << "\"" << Date::dateToString(i.getStartDate()) << "\""  << "," ;
      meet_file << "\"" << Date::dateToString(i.getEndDate()) << "\""  << "," ;
      meet_file << "\"" << i.getTitle() << "\""  ;
      if(index < m_meetingList.size() -1 )
      {
        meet_file << endl ;
      }
      index ++ ;
    }
    m_dirty = true ;
    return true ;
  }

  /**
  *   write file content from memory
  *   @return if success, true will be returned
  */
  bool Storage::readFromFile(void){
    ifstream meet_file(Path::meetingPath) ;
    if(!meet_file.is_open()) return false ;
    ifstream user_file(Path::userPath) ;
    if(!user_file.is_open()) return false ;
    string tmp ;
    while(getline( meet_file ,tmp ))
    {
      if(tmp == "") return false ;
      stringstream ss(tmp) ;
      string var ;
      getline(ss,var,',') ;
      string sponsor = var.substr(1,var.size() -2) ;
      getline(ss,var,',') ;
      vector<string> participators = doSomeThing(var) ;
      getline(ss,var,',') ;
      Date beg_date(var.substr(1,var.size() -2 )) ;
      getline(ss,var,',') ;
      Date end_date(var.substr(1,var.size() -2 )) ;
      getline(ss,var,',') ;
      string title = var.substr(1,var.size() -2 ) ;
      m_meetingList.push_back(Meeting(sponsor,participators,beg_date,end_date,title)) ; 
    }
    meet_file.close() ;
    while(getline(user_file,tmp))
    {  
      if(tmp =="") return false ;
      stringstream ss(tmp) ;
      string var ;
      getline(ss,var,',') ;
      string name = var.substr(1,var.size() -2) ;
      getline(ss,var,',') ;
      string password = var.substr(1,var.size() -2 ) ;
      //if(password == "") continue;
      getline(ss,var,',') ;
      string email = var.substr(1,var.size() -2 ) ;
      //if(email == "") continue; 
      getline(ss,var,',') ;
      string phone = var.substr(1,var.size() -2 ) ;
      m_userList.push_back(User(name, password, email, phone));
    }
    user_file.close() ;
    return true ;
  }


  
  /**
  * get Instance of storage
  * @return the pointer of the instance
  */
  std::shared_ptr<Storage> Storage::getInstance(void){
      if( m_instance == NULL ){
          m_instance.reset(new Storage) ; 
      }
      return m_instance ;
  }

  /**
  *   destructor
  */
  Storage::~Storage()
  {
      writeToFile() ;
      //m_instance = NULL ;
  }

  // CRUD for User & Meeting
  // using C++11 Function Template and Lambda Expressions

  /**
  * create a user
  * @param a user object
  */
  void Storage::createUser(const User &t_user)
  {
     m_userList.push_back(User(t_user)) ;
     m_dirty = true ;
  }

  /**
  * query users
  * @param a lambda function as the filter
  * @return a list of fitted users
  */
  std::list<User> Storage::queryUser(std::function<bool(const User &)> filter) const
  {
      std::list<User> newlist ;

      for(auto s: m_userList)
      {
          if( filter(s) == true )
          {
              newlist.push_back(s) ;
          }
      }
      return newlist ;
  }

  /**
  * update users
  * @param a lambda function as the filter
  * @param a lambda function as the method to update the user
  * @return the number of updated users
  */
  int Storage::updateUser(std::function<bool(const User &)> filter,
                 std::function<void(User &)> switcher)
  {   
      int  judge = 0 ;

      //auto iter = m_userList.begin() ;

      for(auto &s : m_userList )
      {
          if( filter(s) == true )
          {  
            m_dirty = true ;
             switcher(s) ;
             judge ++ ;
          }
      }
      if( m_dirty  == true )
      {
        sync() ;
      }

      return judge ;
  }

  /**
  * delete users
  * @param a lambda function as the filter
  * @return the number of deleted users
  */
  int Storage::deleteUser(std::function<bool(const User &)> filter)
  {   
      auto it = remove_if(m_userList.begin(),m_userList.end(),filter) ;
      int judge = distance(it,m_userList.end()) ;
      if( judge != 0 )
      {
         m_dirty = true ;
      }
      m_userList.erase(it,m_userList.end()) ;
      return judge ;
  }

  /**
  * create a meeting
  * @param a meeting object
  */
  void Storage::createMeeting(const Meeting &t_meeting)
  {
      m_meetingList.push_back(Meeting(t_meeting)) ;
      m_dirty = true ;
  }

  /**
  * query meetings
  * @param a lambda function as the filter
  * @return a list of fitted meetings
  */
  std::list<Meeting> Storage::queryMeeting(
      std::function<bool(const Meeting &)> filter) const
  {
      std::list<Meeting> newlist ;
      for(auto &s : m_meetingList)
      {
          if( filter(s) == true )
          {
              newlist.push_back(s) ;
          }
      }
      return newlist ;
  }

  /**
  * update meetings
  * @param a lambda function as the filter
  * @param a lambda function as the method to update the meeting
  * @return the number of updated meetings
  */
  int Storage::updateMeeting(std::function<bool(const Meeting &)> filter,
                    std::function<void(Meeting &)> switcher)
  {
      int  judge = 0 ;

      //auto iter = m_meetingList.begin() ;

      for(auto &s : m_meetingList )
      {
          if( filter(s) == true )
          {   
             m_dirty = true ;
             switcher(s) ;
             judge ++ ;
          }
      }
       if( m_dirty  == true )
      {
        sync() ;
      }
      return judge ;
  }

  /**
  * delete meetings
  * @param a lambda function as the filter
  * @return the number of deleted meetings
  */
  int Storage::deleteMeeting(std::function<bool(const Meeting &)> filter)
  {
      auto it = remove_if(m_meetingList.begin(),m_meetingList.end(),filter) ;
      int judge = distance(it,m_meetingList.end()) ;
      if( judge != 0 )
      {
         m_dirty = true ;
      }
      m_meetingList.erase(it,m_meetingList.end()) ;
      return judge ;
  }

  /**
  * sync with the file
  */
  bool Storage::sync(void)
  {     
    if(m_dirty == true)
    {
      return writeToFile();
      //writeToFile() ;
    }
    return false;
  }


