#include "User.h"

namespace BeShareBot {


User::User()
     :fName("<unassigned>"),
      fSessionID("<unassigned>"),
      fIsBot(true)
{
}
// ---------------------------------------------------------------
User::User(const char *SessionID, const char *Name, bool IsBot)
     :fName(Name),
      fSessionID(SessionID),
      fIsBot(IsBot)
{
}
// ---------------------------------------------------------------
User::User(const User &Other)
{
	fName = Other.fName;
	fSessionID = Other.fSessionID;
	fIsBot = Other.fIsBot;
}
// ---------------------------------------------------------------
const char *
User::Name() const
{
	return fName.Cstr();
}
// ---------------------------------------------------------------
void 
User::Name(const char *TheName)
{
	fName = TheName;
}
// ---------------------------------------------------------------
bool 
User::IsBot() const
{
	return fIsBot;
}
// ---------------------------------------------------------------
const char *
User::SessionID() const
{
	return fSessionID.Cstr();
}
// ---------------------------------------------------------------


}	// ns BeShare

