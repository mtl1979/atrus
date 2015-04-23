#ifndef BESHAREDEFS_H
#define BESHAREDEFS_H


// Stolen from ShareNetClient.h

enum
{
   ROOT_DEPTH = 0,         // root node
   HOST_NAME_DEPTH,
   SESSION_ID_DEPTH,
   BESHARE_HOME_DEPTH,     // used to separate our stuff from other (non-BeShare) data on the same server
   USER_NAME_DEPTH,        // user's handle node would be found here
   FILE_INFO_DEPTH         // user's shared file list is here
};      

enum
{
   NET_CLIENT_CONNECTED_TO_SERVER = 0,
   NET_CLIENT_DISCONNECTED_FROM_SERVER,
   NET_CLIENT_NEW_CHAT_TEXT,
   NET_CLIENT_CONNECT_BACK_REQUEST
};


#endif

