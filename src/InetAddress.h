// 
// Create by kong on 2024/11/8
// Copyright 2024 Beijing Xinchangcheng Technology Development Co., Ltd.
//
#ifndef CYBER_INETADDRESS_H
#define CYBER_INETADDRESS_H

#include <arpa/inet.h>

namespace WebServer {

class InetAddress {
public:
    InetAddress();
    InetAddress(const char *ip , uint16_t port);
    ~InetAddress();
public:
    struct sockaddr_in addr = {0};
    socklen_t addr_len = 0;
};

}
#endif //CYBER_INETADDRESS_H
