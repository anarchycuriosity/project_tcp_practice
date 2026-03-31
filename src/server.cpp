#include <sys/socket.h>
#include <iostream>
#include <netinet/in.h>
#include <cstring> //strerror
#include <string>
#include <arpa/inet.h> //inet_addr把主机地址转网络用的大端地址
#include <unistd.h>    //close函数
using std::string;
/*
FD  file descriptor     文件描述符
addr    address     地址
BUF     buffer      缓冲区
*/
int main()
{
    // 1创建socket
    // 下面介绍几个参数
    // 1地址族AF，这里的AF_INET是address family INET指的是IPv4告诉系统我们要用4端数字式的地址格式
    // 2套接字类型，表面我们要建立一个可靠的双向流式的连接，这是TCP的特征
    // 3明确指定使用TCP协议，因为写了SOCK_STREAM即使这里写0也是TCP
    int sockfd = ::socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (sockfd < 0)
    {
        printf("create socket error : errno = %d errmsg = %s\n", errno, strerror(errno));
    }
    else
    {
        printf("succeess\n");
    }

    // 绑定socket
    /*
    在绑定我们需要把地址信息填好
    */
    string ip = "127.0.0.1";
    int port = 8080;
    struct sockaddr_in sockaddr;                 // 这是一个专门存IPv4地址的结构体
    std::memset(&sockaddr, 0, sizeof(sockaddr)); // 对结构体初始化为0
    // 协议簇
    sockaddr.sin_family = AF_INET; // 初始化的一个步骤，再次确认我是IPv4协议
    // ip地址
    sockaddr.sin_addr.s_addr = inet_addr(ip.c_str());
    // 端口
    sockaddr.sin_port = htons(port); // host to network short把主机字节序转成网络字节序
    // 数据在高低位内存的存法不同
    //  以上都是一些流程化的东西
    if (::bind(sockfd, (struct sockaddr *)&sockaddr, sizeof(sockaddr)) < 0)
    {
        printf("socket bind error : errno = %d errmsg = %s\n", errno, strerror(errno));
    }
    else
    {
        printf("socket bind success : ip = %d port=%d\n", ip.c_str(), port);
    }

    // 监听socket
    if (::listen(sockfd, 1024) < 0) // 1024是积压队列
    {
        printf("socket listen error:errno=%d errmsg=%s\n", errno, strerror(errno));
    }
    else
    {
        printf("socket listening ...\n");
    }

    while (true) // 不断监听
    {
        // 4接受客户端连接
        // 使用已连接套接字，而不用sockfd
        int connfd = ::accept(sockfd, nullptr, nullptr);
        if (connfd < 0)
        {
            printf("socket accept error:%d,errmsg:%s\n", errno, strerror(errno));
            return 1;
        }
        // 5用缓存数组来接受客户端数据
        char buf[1024] = {0};
        size_t len = ::recv(connfd, buf, sizeof(buf), 0);
        // 设置不同的标志位flags能让recv采取不同的行动，默认是0
        printf("recv: connfd=%d msg=%s\n", connfd, buf);

        // 6send函数向客户端发送数据
        // 原封不动地发回去
        ::send(connfd, buf, len, 0);
    }
    // 关闭socket
    ::close(sockfd);
}
// sudo netstat -anp | grep :8080
// 命令1是列出所有的网络连接和接口然后把这个结果传到第二个，然后查找
// 可以去看正在发生的进程