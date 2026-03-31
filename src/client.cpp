#include <sys/socket.h>
#include <iostream>
#include <netinet/in.h>
#include <cstring> //strerror
#include <string>
#include <arpa/inet.h> //inet_addr把主机地址转网络用的大端地址
#include <unistd.h>    //close函数
using std::string;
int main()
{
    // 1创建socket
    // 2用socket函数确认套接字的各种数据，地址组，套接字类型，协议
    int sockfd = ::socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (sockfd < 0)
    {
        printf("create socket error : errno=%d errmsg=%s\n", errno, strerror(errno));
    }
    else
    {
        printf("create socket successs\n");
    }
    // 使用bind函数去绑定FD和结构体
    // 3memset初始化结构体sockaddr_in，然后用结构体拿到IP协议和地址和端口
    // 客户端不需要绑定端口
    sockaddr_in addr;
    string ip = "127.0.0.1";
    int port = 8080;
    std::memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = inet_addr(ip.c_str());
    addr.sin_port = htons(port);
    // if (::bind(sockfd, (sockaddr *)&addr, sizeof(addr)) < 0)
    // {
    //     printf("socket bind error : errno=%d errmsg=%s\n", errno, strerror(errno));
    // }
    // else
    // {
    //     printf("socket bind success\n");
    // }

    // 不需要listen函数监听了
    // 新建一个connfd来拿到socketFD的数据
    // 但是考虑到不需要监听，所以connfd也不需要了
    while (true)
    {
        int connfd = ::accept(sockfd, nullptr, nullptr);
        if (connfd < 0)
        {
            printf("socket accept error : errno=%d errmsg=%s\n", errno, strerror(errno));
            return 1;
        }
        else
        {
            printf("socket accept success\n");
        }
    }
    char buf[1024] = {0};
    ::recv(sockfd, buf, sizeof(buf), 0);
    // 用recv拿到connfd里面的数据并存到buf数组缓存
    // 关闭
}