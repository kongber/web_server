// 
// Create by kong on 2024/11/8
// Copyright 2024 Beijing Xinchangcheng Technology Development Co., Ltd.
//
#ifndef CYBER_SOCKET_H
#define CYBER_SOCKET_H

namespace WebServer {

class InetAddress;
class Socket
{
private:
    int fd_;
public:
    Socket();
    explicit Socket(int);
    ~Socket();

    void Bind(InetAddress *address) const;
    void Listen() const;
    void Setnonblocking() const;

    int Accept(InetAddress *address) const;

    int GetFd() const;
};

}

#endif //CYBER_SOCKET_H
