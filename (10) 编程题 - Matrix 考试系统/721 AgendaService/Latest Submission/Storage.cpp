
#include <functional>
#include <fstream>
#include <list>
#include <memory>
#include <string>
#include <iostream>
#include <cstring>
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
    readFromFile() ;
    m_dirty = false ;
  }

  /**
  *   read file content into memory
  *   @return if success, true will be returned
  */
  bool Storage::writeToFile(void)
  {
        ofstream sout(Path::meetingPath) , fout(Path::userPath) ;
        if( fout.fail() || sout.fail() )
        {
            return false ;
        }
        list<User>::iterator Iter ;
        for(Iter = m_userList.begin() ; Iter != m_userList.end() ; Iter ++)
        {   
            auto temp = Iter ;
            fout << "\"" << Iter->getName() << "\",\"" << Iter->getPassword() << "\",\"" << Iter->getEmail()  << "\",\"" << Iter->getPhone() << "\"" ;
            if( ++temp != m_userList.end() )
            {
                fout << endl ;
            } 
        }
        //fout.close() ;
        list<Meeting>::iterator iter ;
        for( iter = m_meetingList.begin() ; iter != m_meetingList.end() ; iter ++ )
        {
           auto temp = iter ;
           sout << "\"" << iter->getSponsor() << "\",\"" ;
           auto p_list = iter -> getParticipator() ;
           for( auto i = p_list.begin() ; i!= p_list.end() ; i ++ )
           {
               if( i == p_list.begin() ){
                   sout << *i ;
               }
               else
               {
                   sout << '&' << *i ;
               }
           }
           sout << "\",\"" ;
           string bdata = Date::dateToString(iter->getStartDate()) ;
           sout<< bdata << "\",\"" ;
           string edata = Date::dateToString(iter->getEndDate()) ;
           sout << edata << "\",\"" ;
           temp ++ ;
           if( temp == m_meetingList.end())
           {
               sout << iter -> getTitle() << "\"" ;
           }
           else
           {
               sout << iter -> getTitle() << "\"" << endl ;
           }
           //fout << iter -> getTitle() << "\"" << endl ;
        }
        //sout.close() ;
      m_dirty = true ;
      return true ;
  }

  /**
  *   write file content from memory
  *   @return if success, true will be returned
  */
  bool Storage::readFromFile(void){
    ifstream fin ;
    if( fin.fail() )
    {
        return false ;
    } 
    fin.open(Path::userPath) ;
    
    if( fin.is_open() )
    {
        string temp ;
        while( getline( fin , temp ))
        {   
            if(temp.size() == 0 )
            {
              break ;
            }
            int commaindex[100] = {0} ;
            int k = 0 ;
            for(int i = 0 ; temp[i] != '\0' ; i ++ )
            {
               if( temp[i] == '"' )
               {
                  commaindex[k++] = i ;
               }    
            }
            User u ;
            //cout << temp.size() << endl;
            //for (int i = 0; i < 8; ++i) {
              //cout << commaindex[i] << endl;
            //}
            u.setName( temp.substr( commaindex[0] + 1 , commaindex[1] - commaindex[0] - 1  )) ;
            u.setPassword( temp.substr( commaindex[2] + 1, commaindex[3] - commaindex[2] - 1 )) ;
            u.setEmail( temp.substr( commaindex[4] + 1 , commaindex[5] - commaindex[4] - 1  )) ;
            //cout << temp << endl ;
            u.setPhone( temp.substr( commaindex[6] + 1 , commaindex[7] - commaindex[6] - 1  )) ;
            if(u.getName().size() == 0 )
            {
              break ;
            }
            m_userList.push_back(u) ;
        }
        fin.close() ;
    }
    else
    {
      return false ;
    }
    fin.open(Path::meetingPath) ;

    if( fin.is_open() )
    {
        string line , sponsor , participators , strstartDate , strendDate , title ;
        Date startDate ;
        Date endDate ;
        while( getline( fin , line ))
        {   
            
            if(line.size()==0)
            {
              break ;
            }
            int arr[100] = {0} ;
            int j = 0 ; 
            for(unsigned int i = 0 ; i < line.size() ; i ++ )
            {
              if(line[i] == '"')
              {
                arr[j++] = i ;
              }
            }
            sponsor = line.substr( arr[0] + 1 , arr[1] - arr[0] - 1 ) ;
            participators =  line.substr( arr[2] + 1 , arr[3] - arr[2] - 1 ) ;
            strstartDate =  line.substr( arr[4] + 1 , arr[5] - arr[4] - 1 ) ;
            strendDate =  line.substr( arr[6] + 1 , arr[7] - arr[6] - 1 ) ; 
            title =  line.substr( arr[8] + 1 , arr[9] - arr[8] - 1 ) ;
            
            vector<string> temp ;
            int arr1[100] = {0} ;
            int k = 1 ;
            for(unsigned int i = 0 ; i < participators.size() ; i ++ )
            {
              if(participators[i] == '&')
              {
                arr1[k] = i ;
                k ++ ;
              }
            }
            if(arr1[1]==0)
            {
              temp.push_back(participators) ;
            }
            else
            {
              for(unsigned int i = 0 ; i < k ; i ++ )
              {
                if( i == 0 )
                {
                  temp.push_back(participators.substr(arr1[i] , arr1[i+1]-arr1[i])) ;
                  //cout << arr1[0]  << " "  <<  arr1[1]  << endl; 
                  //cout << participators.substr(arr1[i],arr[i+1] -arr[i]) << endl;
                }
                else
                {
                  
                  temp.push_back(participators.substr(arr1[i] + 1,arr1[i+1] - arr1[i] - 1 )) ;
                }
              }
            }

            if(sponsor.size() == 0 )
            {
              break ;
            }
            startDate = Date::stringToDate(strstartDate) ;
            endDate = Date::stringToDate(strendDate) ;

            Meeting m(sponsor,temp,startDate,endDate,title);
            m_meetingList.push_back(m) ;
            
        }
        fin.close() ;
    }
    else
    {
        return false ;
    }
    return true ;
  }


  
  /**
  * get Instance of storage
  * @return the pointer of the instance
  */
  std::shared_ptr<Storage> Storage::getInstance(void){
      if( m_instance == NULL ){
          m_instance = shared_ptr<Storage> (new Storage()) ; 
      }
      return m_instance ;
  }

  /**
  *   destructor
  */
  Storage::~Storage()
  {
      writeToFile() ;
      m_instance = NULL ;
  }

  // CRUD for User & Meeting
  // using C++11 Function Template and Lambda Expressions

  /**
  * create a user
  * @param a user object
  */
  void Storage::createUser(const User &t_user)
  {
     m_userList.push_back(t_user) ;
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

      for(auto &s: m_userList)
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
      m_meetingList.push_back(t_meeting) ;
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
    if(m_dirty == true )
    {
      m_dirty = false ;
      return writeToFile() ;
    }
    return false ;
      //writeToFile() ;
  }


