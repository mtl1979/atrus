#ifndef BESHARE_USER_H
#define BESHARE_USER_H

#include "muscle/util/String.h"

namespace BeShareBot {

using namespace muscle;

class User
{
	public:
		User();
		User(const char* SessionID, const char* Name, bool IsBot = false);
		User(const User& Other);
	
		const char*	Name() const;
		void		Name(const char* TheName);
		
		bool		IsBot() const;
		const char*	SessionID() const;
		
	protected:
	
	private:
		String	fName;
		String	fSessionID;
		bool			fIsBot;


};

}	// NS BeShare

#endif

