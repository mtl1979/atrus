#include "UserList.h"

namespace BeShareBot {

// --------------------------------------------------------------------------
UserList::UserList()
{
}
// --------------------------------------------------------------------------
void UserList::AddUser(const User& Data)
{
	fUsersVec.push_back(Data);
}
// --------------------------------------------------------------------------
User* UserList::UserByID(const char *SessionID)
{
	User* result = NULL;
	for( uint32 i = 0; i < fUsersVec.size(); ++i )
	{
		if( 0 == strcmp(fUsersVec[i].SessionID(), SessionID) )
		{
			result = &fUsersVec[i];
			break;
		}
	}
	return result;
}
// --------------------------------------------------------------------------
User* UserList::UserByName(const char *Name)
{
	User* result = NULL;
	for( uint32 i = 0; i < fUsersVec.size(); ++i )
	{
		if( 0 == strcmp(fUsersVec[i].Name(), Name) )
		{
			result = &fUsersVec[i];
			break;
		}
	}
	
	return result;
}
// --------------------------------------------------------------------------
User* UserList::UserByIndex(uint32 Index)
{
	User* result = NULL;
	
	if( fUsersVec.size() > Index )
	{
		result = &fUsersVec[Index];
	}
	
	return result; 
}
// --------------------------------------------------------------------------
uint32 UserList::Count() const
{
	return fUsersVec.size();
}


}	// ns BeShare

