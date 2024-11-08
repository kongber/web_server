// 
// Create by kong on 2024/11/8
// Copyright 2024 Beijing Xinchangcheng Technology Development Co., Ltd.
//

#include "Epool.h"
#include "status_code.h"

#define MAX_EVENTS 1000
using namespace WebServer;

Epool::Epool() {
    epfd = epoll_create1(0);
    error_if(epfd == -1, "Epoll create error.");
    events = new epoll_event[MAX_EVENTS];
    bzero(events, sizeof (*events) * MAX_EVENTS);
}

Epool::~Epool() {
    if (epfd == -1) {
        close(epfd);
        epfd = -1;
    }
    delete[] events;
}

void Epool::AddFd(int fd, uint32_t op) const {
    struct epoll_event ev = {0};
    bzero(&ev, sizeof(ev));
    ev.data.fd = fd;
    ev.events  = op;
    error_if(epoll_ctl(epfd, EPOLL_CTL_ADD, fd, &ev) == -1, "Epoll add event error.");
}

std::vector<epoll_event> Epool::Poll(int timeout) {
    std::vector<epoll_event> activeEvents;
    int nfds = epoll_wait(epfd, events, MAX_EVENTS, timeout);
    error_if(nfds == -1, "Epool wait error.");
    for (int i = 0; i < nfds; ++i) {
        activeEvents.push_back(events[i]);
    }
    return activeEvents;
}

