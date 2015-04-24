// BeShare Bot class.
//
// By Alan Ellis
//
// Origial 'C' framework by Jeremy Friesner (jaf@lcsaudio.com).
// Enormous portions of the code ripped off from there.
// Public domain code.  Hack away!

#include <stdio.h>
#include <stdlib.h>
#ifndef WIN32
#include <unistd.h>
#endif
#include <string.h>
#include <time.h>

#include "muscle/dataio/TCPSocketDataIO.h"
#include "reflector/StorageReflectConstants.h"
#include "regex/PathMatcher.h"
#include "util/NetworkUtilityFunctions.h"
#include "util/SocketMultiplexer.h"
#include "util/StringTokenizer.h"

#include "BeShareDefs.h"
#include "User.h"

#include "Bot.h"

namespace BeShareBot {

//-------------------------------------------------------------------------------
Bot::Bot(const char* Name)
    :fData(Name),
     fQuitCommand("begone")
{
}
//-------------------------------------------------------------------------------
Bot::~Bot()
{
}
//-------------------------------------------------------------------------------
void Bot::Quit(int Code)
{
	fData.quit = true;
	fData.exitCode = Code;
}
//-------------------------------------------------------------------------------
int Bot::Setup(int ArgC, char **ArgV, ConstSocketRef & Socket)
{
	int result = 0;

	if (ArgC < 2)
	{
		LogTime(MUSCLE_LOG_INFO, "Usage:  Atrus hostname[:port] [BotName] [--quit-command <command>]\n");
		return 5;
	}

	Socket = Connect(IPAddressAndPort(ArgV[1], 2960, true), NULL, "Connecting", false);
	if (Socket() == NULL) return 10;

	AbstractMessageIOGatewayRef gwRef(new MessageIOGateway);
	gwRef()->SetDataIO(DataIORef(new TCPSocketDataIO(Socket, false)));
	fData.Gateway(gwRef);

	// set up messages
	{
		// Ask the server what our host name is
		fData.Gateway()()->AddOutgoingMessage(GetMessageFromPool(PR_COMMAND_GETPARAMETERS));

		// Subscribe to watch all arriving BeShare users and what they say
		MessageRef watchBeShareUsers = GetMessageFromPool(PR_COMMAND_SETPARAMETERS);
		watchBeShareUsers()->AddBool("SUBSCRIBE:beshare/name", true);
//		watchBeShareUsers()->AddBool(PR_NAME_SUBSCRIBE_QUIETLY, true);  // so as not to spam when we are starting up
		fData.Gateway()()->AddOutgoingMessage(watchBeShareUsers);

		// kinda silly ...
		const char* name = fData.Name();

		if( ArgC > 2 )
		{
			name = ArgV[2];
		}

		fData.Name(name);
	}

	if( ArgC > 4 )
	{
		if( 0 == strcmp( ArgV[3], "--quit-command" ) )
		{
			fQuitCommand = ArgV[4];
		}
	}
	return result;
}
//-------------------------------------------------------------------------------
int Bot::Run(int ArgC, char **ArgV)
{
	ConstSocketRef Socket;
	int result = Setup(ArgC, ArgV, Socket);
	if( result ) return result;

	// todo: this should be broken into separate functions, and
	//       there needs to be a way to test the connection and reconnect if
	//       necessary.
	QueueGatewayMessageReceiver incomingMessagesQueue;
	SocketMultiplexer multiplexer;
	while(!fData.quit)
	{
		(void) multiplexer.RegisterSocketForReadReady(Socket.GetFileDescriptor());
		if (fData.Gateway()()->HasBytesToOutput()) (void) multiplexer.RegisterSocketForWriteReady(Socket.GetFileDescriptor());

		if (multiplexer.WaitForEvents() < 0)
		{
				 LogTime(MUSCLE_LOG_CRITICALERROR, "BeShareBot::Bot : select() failed!\n");
				 return -1;
		}

		bool reading = multiplexer.IsSocketReadyForRead(Socket.GetFileDescriptor());
		bool writing = multiplexer.IsSocketReadyForWrite(Socket.GetFileDescriptor());
		bool writeError = ((writing)&&(fData.Gateway()()->DoOutput() < 0));
		bool readError  = ((reading)&&(fData.Gateway()()->DoInput(incomingMessagesQueue) < 0));
		if ((readError)||(writeError))
		{
			LogTime(MUSCLE_LOG_ERROR, "Connection to server closed, reconnecting...\n");
			Socket.Reset();
			while (Socket() == NULL)
			{
				(void) Setup(ArgC, ArgV, Socket);
				if (Socket() == NULL)
				{
					LogTime(MUSCLE_LOG_ERROR, "Oops, reconnect failed!  I'll wait 5 seconds, then try again.\n");
					Snooze64(5000000);
				}
			}
		}

		MessageRef incoming;
		while((false == fData.quit)&&(incomingMessagesQueue.RemoveHead(incoming) == B_NO_ERROR))
		{
			switch(incoming.GetItemPointer()->what)
			{
				case PR_RESULT_DATAITEMS:
					HandleResultData(incoming);
				break;

				case NET_CLIENT_NEW_CHAT_TEXT:
					HandleChatText(incoming.GetItemPointer()); // Someone has sent a line of chat text to display
				break;

				case PR_RESULT_PARAMETERS:
					// session root is of form "/hostname/sessionID"; parse these out
					fData.SessionID(incoming()->GetString(PR_NAME_SESSION_ROOT).Substring("/")());
				break;
			}
		}
	}
	return fData.exitCode;
}
//-------------------------------------------------------------------------------
void Bot::Name(const char *TheName)
{
	fData.Name(TheName);
}
//-------------------------------------------------------------------------------
const char* Bot::Name()
{
	return fData.Name();
}
//-------------------------------------------------------------------------------
void Bot::SendPublicMessage(const char *Message)
{
	MessageRef chatMessage = GetMessageFromPool(NET_CLIENT_NEW_CHAT_TEXT);
	chatMessage()->AddString(PR_NAME_KEYS, "/*/*/beshare"); // send message to all BeShare clients...
	chatMessage()->AddString("session", fData.SessionID());
	chatMessage()->AddString("text", Message);
	fData.Gateway()()->AddOutgoingMessage(chatMessage);
}
//-------------------------------------------------------------------------------
void Bot::SendPrivateMessage(const char *SessionID, const char *Message)
{
	MessageRef chatMessage = GetMessageFromPool(NET_CLIENT_NEW_CHAT_TEXT);
	String toString("/*/");  // send message to all hosts...
	toString += SessionID;           // who have the given sessionID (or "*" == all session IDs)
	toString += "/beshare";          // and are beshare sessions.
	chatMessage()->AddString(PR_NAME_KEYS, toString.Cstr());
	chatMessage()->AddString("session", fData.SessionID());
	chatMessage()->AddString("text", Message);
	chatMessage()->AddBool("private", true);
	fData.Gateway()()->AddOutgoingMessage(chatMessage);
}
//-------------------------------------------------------------------------------
void Bot::SendMOTDMessage(const char *SessionID, const char *Message)
{
	MessageRef chatMessage = GetMessageFromPool(NET_CLIENT_NEW_CHAT_TEXT);
	String toString("/*/");			// send message to all hosts...
	toString += SessionID;			// who have the given sessionID (or "*" == all session IDs)
	toString += "/beshare";			// and are beshare sessions.
	chatMessage()->AddString(PR_NAME_KEYS, toString.Cstr());
	chatMessage()->AddString("session", fData.SessionID());
	chatMessage()->AddString("text", Message);
	fData.Gateway()()->AddOutgoingMessage(chatMessage);
}
//-------------------------------------------------------------------------------
void Bot::HandleResultData(const MessageRef& msgRef)
{
	Message * msg = msgRef();
	for (MessageFieldNameIterator it(*msgRef(), B_MESSAGE_TYPE); it.HasData(); it++)
	{
		const String & fieldText = it.GetFieldName();
		int pathDepth = GetPathDepth(fieldText.Cstr());
		if (pathDepth >= USER_NAME_DEPTH)
		{
			MessageRef tempRef;
			if (msg->FindMessage(fieldText.Cstr(), tempRef) == B_NO_ERROR)
			{
				switch (pathDepth)
				{
					case USER_NAME_DEPTH:
					{
						HandleNameInfo(fieldText.Cstr(), tempRef);
						break;
					}
				}
			}
		}
	}
	// Look for sub-messages that indicate that nodes were removed from the tree
	String nodepath;
	for (int i = 0; (msg->FindString(PR_NAME_REMOVED_DATAITEMS, i, nodepath) == B_NO_ERROR); i++)
	{
		int pathDepth = GetPathDepth(nodepath.Cstr());
		if (pathDepth >= USER_NAME_DEPTH)
		{
			String sessionID = GetPathClause(SESSION_ID_DEPTH, nodepath.Cstr());
			sessionID = sessionID.Substring(0, sessionID.IndexOf('/'));

			switch (GetPathDepth(nodepath.Cstr()))
			{
				case USER_NAME_DEPTH:
					if (strncmp(GetPathClause(USER_NAME_DEPTH, nodepath.Cstr()), "name", 4) == 0)
					{
						UserLoggedOut(sessionID.Cstr());
					}
			}
		}
	}

}
//-------------------------------------------------------------------------------
void Bot::HandleNameInfo(const char* FieldText, const MessageRef& Message)
{
	String sessionID = GetPathClause(SESSION_ID_DEPTH, FieldText);
	sessionID = sessionID.Substring(0, sessionID.IndexOf('/'));

	const char * nodeName = GetPathClause(USER_NAME_DEPTH, FieldText);
	if (strncmp(nodeName, "name", 4) == 0)
	{
		bool isBot = false;  // default value
		Message.GetItemPointer()->FindBool("bot", &isBot);  // set if present

		const char* name;
		if (Message.GetItemPointer()->FindString("name", &name) == B_NO_ERROR)
		{
			User usr(sessionID.Cstr(), String(name).ToLowerCase().Cstr(), isBot);
			fData.AddUser(usr);
			UserLoggedInOrChangedName(sessionID.Cstr(), String(name).ToLowerCase().Cstr());
		}
	}
}
//-------------------------------------------------------------------------------
void Bot::HandleChatText(const Message* Message)
{
	bool isBot = true;
	if ( B_ERROR == Message->FindBool("bot", &isBot) )
	{
		isBot = false;
	}

	bool botToBotMessage = false;
	Message->FindBool("btbmessage", &botToBotMessage);

	if ( isBot && (!botToBotMessage) )
	{
		// Ignore Bots
		return;
	}

	const char *text;
	const char *session;
	if ((Message->FindString("text", &text) == B_NO_ERROR) &&
	    (Message->FindString("session", &session) == B_NO_ERROR))
	{
		if (Message->HasName("private") || botToBotMessage)
		{
			if ( false == CheckOwnerMessage(session, text) )
			{
				ReceivedPrivateMessage(session, text);
			}
		}
		else
		{
			ReceivedChatMessage(session, text);
		}
	}
}
//-------------------------------------------------------------------------------
bool Bot::CheckOwnerMessage(const char *SessionID, const char *Message)
{
	bool result = false;

	if ( ! strcmp("owner", Message) )
	{
		SendPrivateMessage(SessionID, OwnerName());
		result = true;
	}

	return result;
}
//-------------------------------------------------------------------------------
void Bot::ReceivedPrivateMessage(const char *, const char *Command)
{
	if ( strstr(Command, fQuitCommand.Cstr() ) )
	{
		SendPublicMessage("I'm outta here!");
		Quit(0);
	}
}
//-------------------------------------------------------------------------------
/*
void
Bot::UserLoggedInOrChangedName(const char *SessionID, const char *Name)
{
}
//-------------------------------------------------------------------------------
void
Bot::UserLoggedOut(const char *SessionID)
{
}
//-------------------------------------------------------------------------------
*/

}	// NS BeShare
