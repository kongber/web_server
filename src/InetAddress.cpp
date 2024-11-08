// 
// Create by kong on 2024/11/8
// Copyright 2024 Beijing Xinchangcheng Technology Development Co., Ltd.
//

#include "InetAddress.h"
#include <cstring>

using namespace WebServer;

InetAddress::InetAddress() :addr_len(sizeof(addr)) {
    bzero(&addr, sizeof(addr));
}

InetAddress::InetAddress(const char *ip, uint16_t port) {
    if (ip == nullptr) {
        return;
    }
    bzero(&addr, sizeof(addr));
    addr.sin_family      = AF_INET;
    addr.sin_addr.s_addr = inet_addr(ip);
    addr.sin_port        = htons(port);
    addr_len = sizeof(addr);
}

InetAddress::~InetAddress() = default;