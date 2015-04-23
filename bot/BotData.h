#ifndef BESHARE_BOT_DATA_H
#define BESHARE_BOT_DATA_H

#include "muscle/iogateway/MessageIOGateway.h"

#include "UserList.h"

namespace BeShareBot {

class User;

class BotData
{
	public:
		BotData(const char* Name);
		
		void		Name(const char* TheName);
		const char*	Name() const;
		
		void		SessionID(const char* TheSessionID);
		const char*	SessionID() const;
		
		void						Gateway(const AbstractMessageIOGatewayRef & TheGateway);
		const AbstractMessageIOGatewayRef &	Gateway();
		
		void		AddUser(const User& Data);
		User*		UserByID(const char* SessionID);
		User*		UserByName(const char* Name);
		User*		UserByIndex(uint32 Index);
		uint32		Users() const;
		
	public:
		bool						quit;
		int							exitCode;		
	
	private:
	
		UserList					fUsers;

		AbstractMessageIOGatewayRef 	fMessageGateway;
		String				fName;	
		String 				fSessionID;
};

}	// ns BeShare

#endif

