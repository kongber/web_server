// 
// Create by kong on 2024/11/8
// Copyright 2024 Beijing Xinchangcheng Technology Development Co., Ltd.
//

#include <fcntl.h>
#include "Socket.h"
#include "status_code.h"
#include "InetAddress.h"

using namespace WebServer;

Socket::Socket() : fd_() {
    fd_ = socket(AF_INET, SOCK_STREAM, 0);
    error_if(fd_ == -1, "Socket create error.");
}

Socket::Socket(int fd) : fd_(fd) {
    error_if(fd == -1, "Socket create error.");
}

Socket::~Socket() {
    if (fd_ != -1) {
        close(fd_);
        fd_ = -1;
    }
}

void Socket::Bind(InetAddress *address) const {
    error_if(::bind(fd_, (sockaddr *) &address->addr, address->addr_len == -1),
             "Socket bind error");
}

void Socket::Listen() const {
    error_if(::listen(fd_, SOMAXCONN) == -1,
             "Socket listen error");
}

void Socket::Setnonblocking() const {
    fcntl(fd_, F_SETFL, fcntl(fd_, F_GETFL) | O_NONBLOCK);
}

int Socket::Accept(InetAddress *address) const {
    int clnt_sockfd = ::accept(fd_, (sockaddr *)&address->addr, &address->addr_len);
    error_if(clnt_sockfd == -1, "Socket accept error");
    return clnt_sockfd;
}

int Socket::GetFd() const {
    return fd_;
}


