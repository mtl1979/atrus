// BeShare Bot class.
//
// By Alan Ellis (alan@cgsoftware.com)
//
// Origial 'C' framework by Jeremy Friesner (jaf@lcsaudio.com).
// Enormous portions of the code ripped off from there.
// Public domain code.  Hack away!

#ifndef BESHARE_BOT_H
#define BESHARE_BOT_H

#include "BotData.h"

namespace BeShareBot {

using namespace muscle;

class Bot
{
	public:
					Bot(const char* Name);
		virtual 	~Bot();
		
		int			Run(int ArgC, char** ArgV);

	protected:
		
		void		Name(const char* TheName);
		const char*	Name();
		void		Quit(int Code = 0);
		
		void		SendPublicMessage(const char* Message);
		void		SendPrivateMessage(const char* SessionID, const char* Message);
		void		SendMOTDMessage(const char* SessionID, const char* Message);
		
		
		// Hook Functions		
		virtual void	ReceivedChatMessage(const char* SessionID, const char* Message) = 0;
		virtual void	ReceivedPrivateMessage(const char* SessionID, const char* Message);
		virtual void	UserLoggedInOrChangedName(const char* SessionID, const char* Name) = 0;
		virtual void	UserLoggedOut(const char* SessionID) = 0;
		
		virtual const char*	OwnerName() = 0;

	protected:
		virtual int	Setup(int ArgC, char** ArgV, ConstSocketRef & retSocket);

		BotData			fData;
		String	fQuitCommand;

	private:
		
		void		HandleResultData(const MessageRef& Message);
		void		HandleNameInfo(const char* SessionID, const MessageRef& Message);
		void		HandleChatText(const Message* Message);
		bool		CheckOwnerMessage(const char *SessionID, const char *Message);

	private:
};


}	// NS BeShare

#endif
