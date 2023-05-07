#ifndef NETWORK_HEADER_

#include "print.h"

typedef  void* NetLibSessionPtr; /* obscure pointer */

typedef enum NetLibSockEnum
{
   NetLibSockNone,
   NetLibSockTcp,
   NetLibSockUdp,
}NetLibSock;

typedef struct NetLibSetupStruct
{
   int port;
   int isServer;
   char* ipAddr;
   char* peername;
   NetLibSock type;

} NetLibSetup;

int NetLibInit(NetLibSetup* setup);
int NetLibOpen(NetLibSetup* setup);
int NetLibSend(char* buff, size_t len);
int NetLibRecv(char* buff, size_t len);
int NetLibClose(NetLibSetup* setup);
int NetLibFinish(NetLibSetup* setup);

#endif