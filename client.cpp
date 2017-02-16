#include <stdio.h>
#include <ace/SOCK_Connector.h>
#include <ace/Log_Msg.h>

int main(int argc, char* argv[])
{
    if (argc < 2) {
        ACE_DEBUG((LM_DEBUG, "Usage:%s port\n", argv[0]));
        return -1;
    }
    short port = atoi(argv[1]);
    ACE_SOCK_Connector connector;
    ACE_SOCK_Stream peer;
    ACE_INET_Addr addr(port, "127.0.0.1");
    if (connector.connect(peer, addr) == -1) {
        ACE_DEBUG((LM_DEBUG, "connect failed!\n"));
        return -1;
    }
    const char buf[] = "hello world!";
    peer.send_n(buf, sizeof(buf)-1);
    peer.close();
    return 0;
}
