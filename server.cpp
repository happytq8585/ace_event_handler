#include <ace/SOCK_Acceptor.h>
#include <ace/Log_Msg.h>
#include <stdio.h>

int main(int argc, char* argv[])
{
    if (argc < 2) {
        ACE_DEBUG((LM_DEBUG, "Usage:%s port\n", argv[0]));
        return -1;
    }
    int port = atoi(argv[1]);
    ACE_SOCK_Acceptor acceptor;
    ACE_INET_Addr port_to_listen(port);

    if (acceptor.open(port_to_listen, 1) == -1) {
        ACE_DEBUG((LM_DEBUG, "acceptor open failed!\n"));
        return -1;
    }

    while (1) {
        ACE_SOCK_Stream peer;
        ACE_INET_Addr peer_addr;

        if (acceptor.accept(peer, &peer_addr) == -1) {
            ACE_DEBUG((LM_DEBUG, "accept failed!\n"));
            continue;
        }
        ACE_TCHAR peer_name[128];
        peer_addr.addr_to_string(peer_name, sizeof(peer_name));
        ACE_DEBUG((LM_DEBUG, "connection from %s\n", peer_name));
        
        char buffer[1024];
        ssize_t bytes_received;
        while ((bytes_received = peer.recv(buffer, sizeof(buffer))) > 0) {
            buffer[bytes_received] = 0;
            ACE_DEBUG((LM_DEBUG, "received: %s\n", buffer));
        }
        peer.close();
    }
    return 0;
}
