#include "main.hpp"

int Socket(const int &_domain, const int &_type, const int &_protocol)
{
    int fd = socket(_domain, _type, _protocol);
    if (fd < 0)
    {
        std::cerr << "bad socket: " << errno << '\n';
        exit(EXIT_FAILURE);
    }
    return fd;
}

void Bind(const int &_fd, const struct sockaddr_in &_addr, const socklen_t &_len)
{
    int bn = bind(_fd, (struct sockaddr *)&_addr, _len);
    if (bn < 0)
    {
        std::cerr << "bad bind: " << errno << '\n';
        exit(EXIT_FAILURE);
    }
}

void Listen(const int &_fd, const int &_n)
{
    int lsn = listen(_fd, _n);
    if (lsn < 0)
    {
        std::cerr << "bad lsn: " << errno << '\n';
        exit(EXIT_FAILURE);
    }
}

int Accept(const int &_fd, const struct sockaddr_in &_cliAddr, socklen_t &_len)
{
    int acp = accept(_fd, (struct sockaddr *)&_cliAddr, &_len);
    if (acp < 0)
    {
        std::cerr << "bad acp: " << errno << '\n';
        exit(EXIT_FAILURE);
    }
    return acp;
}

void Recv(const int &_connfd, void *buff, const size_t &_n, const int &_flg)
{
    ssize_t rv = recv(_connfd, buff, _n, _flg);
    if (rv < 0)
    {
        std::cerr << "bad recv: " << errno << '\n';
        exit(EXIT_FAILURE);
    }
}

void Send(const int &_connfd, const void *buff, const size_t &_n, const int &_flg)
{
    ssize_t sd = send(_connfd, buff, _n, _flg);
    if (sd < 0)
    {
        std::cerr << "bad send: " << errno << '\n';
        exit(EXIT_FAILURE);
    }
}

int main(int argc, char *argv[])
{
    int fd = Socket(AF_INET, SOCK_STREAM, 0);

    struct sockaddr_in addr;
    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = INADDR_ANY;
    addr.sin_port = htons(0);

    Bind(fd, addr, sizeof(addr));

    Listen(fd, SOMAXCONN);

    struct sockaddr_in cliAddr;
    socklen_t cliAddrSz{sizeof(cliAddr)};
    memset(&cliAddr, 0, cliAddrSz);

    bool isRun{true};
    while (isRun)
    {
        int connfd = Accept(fd, cliAddr, cliAddrSz);

        void *buff{};
        Recv(connfd, buff, gMaxMsgSz, MSG_PEEK);
        Send(connfd, buff, gMaxMsgSz, MSG_EOR);

        close(connfd);
    }

    return EXIT_SUCCESS;
}