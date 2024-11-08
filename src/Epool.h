// 
// Create by kong on 2024/11/8
// Copyright 2024 Beijing Xinchangcheng Technology Development Co., Ltd.
//
#ifndef CYBER_EPOOL_H
#define CYBER_EPOOL_H

#include <sys/epoll.h>
#include <vector>

namespace WebServer {

class Epool {
private:
    int epfd;
    struct epoll_event *events;
public:
    Epool();
    ~Epool();

    void AddFd(int fd, uint32_t op) const;
    std::vector<epoll_event> Poll(int timeout = -1);
};

}

#endif //CYBER_EPOOL_H
