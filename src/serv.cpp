#include "main.hpp"

int main(int argc, char *argv[])
{
    int fd = Socket(AF_INET, SOCK_STREAM, 0);

    const int trueFlg = 1;
    setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &trueFlg, sizeof(int));

    struct sockaddr_in addr;
    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = INADDR_ANY;
    addr.sin_port = htons(6666);

    Bind(fd, addr, sizeof(addr));

    Listen(fd, SOMAXCONN);

    struct sockaddr_in cliAddr;
    socklen_t cliAddrSz{sizeof(cliAddr)};
    memset(&cliAddr, 0, cliAddrSz);

    bool isRun{true};
    while (isRun)
    {
        int connfd = Accept(fd, cliAddr, cliAddrSz);

        for (;;)
        {
            char buff[gMaxMsgSz];
            if(!Read(connfd, buff, gMaxMsgSz))
                break;
            Write(connfd, buff, gMaxMsgSz);
        }

        close(connfd);
    }

    return EXIT_SUCCESS;
}