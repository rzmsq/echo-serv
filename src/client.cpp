#include "main.hpp"

int main(int argc, char const *argv[])
{

    int fd = Socket(AF_INET, SOCK_STREAM, 0);

    struct sockaddr_in addr;
    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_port = htons(6666);
    Inet_pton(AF_INET, "127.0.0.1", addr.sin_addr);

    Connect(fd, addr, sizeof(addr));

    std::string msg{};
    while (getline(std::cin, msg))
    {
        Write(fd, const_cast<char *>(msg.c_str()), gMaxMsgSz);
        char buff[gMaxMsgSz];
        Read(fd, buff, gMaxMsgSz);

        std::cout << buff << '\n';
    }

    return EXIT_SUCCESS;
}
