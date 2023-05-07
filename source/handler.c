#include "net.h"

void file_client_handler(net_client_handler_args_t *args)
{
   PRINT_DBG("handle to file %d", args->sock);
   char recvbuf[1000];
   int recvbuflen = 1000;

   int ret = 0;
   do {
      ret = recv(args->sock, recvbuf, recvbuflen, 0);
      if (ret > 0) {
         printf("Bytes received: %d\n", ret);
         int iSendResult = send(args->sock, recvbuf, ret, 0);
         printf("Bytes sent: %d\n", iSendResult);
      }
      else if (ret == 0)
         printf("Connection closing...\n");
      else  {
         printf("recv failed with error: %d\n", WSAGetLastError());
         break;
      }
    } while (ret > 0);
}
