#include <map>
#include <vector>
#include <deque>

#include "Bot.h"

using namespace muscle;

class VoidBot: public BeShareBot::Bot
{
	public:
		VoidBot();
		
	protected:
		virtual void	ReceivedChatMessage(const char* SessionID, const char* Message);
		virtual void	ReceivedPrivateMessage(const char* SessionID, const char* Message);
		virtual void	UserLoggedInOrChangedName(const char* SessionID, const char* Name);
		virtual void	UserLoggedOut(const char* SessionID);
		
		virtual bool	SendTextfile(const char* SessionID, const char* Message, bool MType);
		virtual void	SendMessage(const char* SessionID, const char* Message);
		
		virtual String		Time();
		virtual String		MyTime();
		virtual String		NetTime();
		
		virtual bool		IsBotCommand(const String& Command);
		virtual void		Command(const char* SessionID, const String& Command);
		virtual bool		PublicCommands(const char* SessionID, const String& Command);
		virtual bool		PrivateCommands(const char* SessionID, const String& Command);
		
		virtual const char*	OwnerName();
		
	private:
		int				WholeWordPos(const String& WordIn, const char* Word);
		String	GetNick(const String& From, int32 StartFrom);
		void			ReportMessageExistence(const char* SessionID, const char* Name);
		time_t			SecondsSinceJan1970();


	private:
		
		
		// Name, words
		std::map<String, String>					fLastWords;
		
		// Name, messages
		std::map<String, std::vector<String> >		fMessagesForName;
		
		// Name, reported existence of new messages
		std::map<String, bool>								fReportedNewMessages;

		std::deque<String>									fChatHistory;
		
		std::map<String, String>						fLogged;

		String	fQuitCommand;
		
		bool	fHandled;
		bool	fSendPublic;
		bool	fPrivateCommand;

};

