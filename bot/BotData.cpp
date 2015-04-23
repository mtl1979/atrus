#include "muscle/reflector/StorageReflectConstants.h"

#include "BotData.h"

namespace BeShareBot {

// ---------------------------------------------------------
BotData::BotData(const char* Name)
        :quit(false),
         exitCode(0),
         fName(Name),
         fSessionID("<unset>")
{
}
// ---------------------------------------------------------
void BotData::Name(const char *TheName)
{
	fName = TheName;
	
	if( fMessageGateway() )
	{
		// Set my name for other BeShare users to see
		Message myName;
		myName.AddString("name", fName.Cstr());
		myName.AddInt32("port", 0);
		myName.AddBool("bot", true);
		MessageRef setNameMessage = GetMessageFromPool(PR_COMMAND_SETDATA);
		setNameMessage()->AddMessage("beshare/name", myName);
		fMessageGateway()->AddOutgoingMessage(setNameMessage);
	}
}
// ---------------------------------------------------------
const char *
BotData::Name() const
{
	return fName.Cstr();
}
// ---------------------------------------------------------
void 
BotData::SessionID(const char *TheSessionID)
{
	fSessionID = TheSessionID;
}
// ---------------------------------------------------------
const char* BotData::SessionID() const
{
	return fSessionID.Cstr();
}
// ---------------------------------------------------------
void BotData::Gateway(const AbstractMessageIOGatewayRef & TheGateway)
{
	fMessageGateway = TheGateway;
}
// ---------------------------------------------------------
const AbstractMessageIOGatewayRef & BotData::Gateway()
{
	return fMessageGateway;
}
// ---------------------------------------------------------
void BotData::AddUser(const User &Data)
{
	fUsers.AddUser(Data);
}
// ---------------------------------------------------------
User* BotData::UserByID(const char *SessionID)
{
	return fUsers.UserByID(SessionID);
}
// ---------------------------------------------------------
User* BotData::UserByName(const char *Name)
{
	return fUsers.UserByName(Name);
}
// ---------------------------------------------------------
User* BotData::UserByIndex(uint32 Index)
{
	return fUsers.UserByIndex(Index);
}
// ---------------------------------------------------------
uint32 BotData::Users() const
{
	return fUsers.Count();
}
// ---------------------------------------------------------

}	// ns BeShare

