#ifndef BESHARE_USERLIST_H
#define BESHARE_USERLIST_H

#include <map>
#include <vector>

#include "User.h"

namespace BeShareBot {

class UserList
{
	public:
		UserList();

		void		AddUser(const User& Data);
		User*		UserByID(const char* SessionID);
		User*		UserByName(const char* Name);
		User*		UserByIndex(uint32 Index);
		uint32		Count() const; 
		
	protected:
	
	private:
		// the list of users
		std::vector<User>				fUsersVec;
};

}	// ns BeShare

#endif

