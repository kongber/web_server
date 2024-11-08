// 
// Create by kong on 2024/11/6
//

#ifndef WEB_SERVER_HTTP_SERVER_H
#define WEB_SERVER_HTTP_SERVER_H

#include <iostream>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <sys/epoll.h>
#include <fcntl.h>

namespace WebServer {

#define MAX_EVENTS 1024
#define READ_BUFFER 1024

class HttpServer {
public:
    HttpServer() {
        int sockfd = socket(AF_INET, SOCK_STREAM, 0);
        error_if(sockfd == -1, "socket create error.");

        struct sockaddr_in serv_addr = {0};
        bzero(&serv_addr, sizeof(serv_addr));
        serv_addr.sin_family = AF_INET;
        serv_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
        serv_addr.sin_port = htons(8888);

        int rv = bind(sockfd, (sockaddr*)&serv_addr, sizeof(serv_addr));
        error_if(rv == -1, "socket bind error.");

        rv = listen(sockfd, SOMAXCONN);
        error_if(rv == -1, "socket listen error.");

        int epfd = epoll_create1(0);
        error_if(epfd == -1, "epoll create error");
        struct epoll_event events[MAX_EVENTS] = {0}, ev = {0};
        bzero(&events, sizeof(events));
        bzero(&ev, sizeof(events));
        ev.data.fd = sockfd;
        ev.events  = EPOLLIN | EPOLLET;
        fcntl(epfd, F_SETFL, fcntl(sockfd, F_GETFL) | O_NONBLOCK);
        epoll_ctl(epfd, EPOLL_CTL_ADD, sockfd, &ev);

        do {
            int nfds = epoll_wait(epfd, events, MAX_EVENTS, -1);
            error_if(nfds == -1, "epoll wait error");
            for (int i = 0; i < nfds; ++i) {
                if (events[i].data.fd == sockfd) {
                    struct sockaddr_in clnt_addr = {0};
                    bzero(&clnt_addr, sizeof(clnt_addr));
                    socklen_t clnt_addr_len = sizeof(clnt_addr);
                    int clnt_sockfd = accept(sockfd, (sockaddr *) &clnt_addr, &clnt_addr_len);
                    error_if(clnt_sockfd == -1, "socket accept error.");
                    bzero(&ev, sizeof(ev));
                    ev.data.fd = clnt_sockfd;
                    ev.events = EPOLLIN | EPOLLET;
                    fcntl(epfd, F_SETFL, fcntl(clnt_sockfd, F_GETFL) | O_NONBLOCK);
                    epoll_ctl(epfd, EPOLL_CTL_ADD, clnt_sockfd, &ev);
                } else if (events[i].events & EPOLLIN) {
                    char buf[READ_BUFFER];
                    while(true){    //由于使用非阻塞IO，读取客户端buffer，一次读取buf大小数据，直到全部读取完毕
                        bzero(&buf, sizeof(buf));
                        ssize_t bytes_read = read(events[i].data.fd, buf, sizeof(buf));
                        if(bytes_read > 0){
                            printf("message from client fd %d: %s\n", events[i].data.fd, buf);
                            write(events[i].data.fd, buf, sizeof(buf));
                        } else if(bytes_read == -1 && errno == EINTR){  //客户端正常中断、继续读取
                            printf("continue reading");
                            continue;
                        } else if(bytes_read == -1 && ((errno == EAGAIN) || (errno == EWOULDBLOCK))){//非阻塞IO，这个条件表示数据全部读取完毕
                            printf("finish reading once, errno: %d\n", errno);
                            break;
                        } else if(bytes_read == 0){  //EOF，客户端断开连接
                            printf("EOF, client fd %d disconnected\n", events[i].data.fd);
                            close(events[i].data.fd);   //关闭socket会自动将文件描述符从epoll树上移除
                            break;
                        }
                    }
                } else {
                    printf("something else happened.\n");
                }
            }

            char buf[1024];
            bzero(&buf, sizeof(buf));
            ssize_t read_bytes = read(rv, buf, sizeof(buf));
            if(read_bytes > 0){
                printf("message from client fd %d: %s\n", rv, buf);
                write(rv, buf, sizeof(buf));
            } else if(read_bytes == 0){
                printf("client fd %d disconnected\n", rv);
                close(rv);
                break;
            } else if(read_bytes == -1){
                close(rv);
                error_if(true, "socket read error");
            }
        } while (1);

    }
};

class Session {

};

}

#endif //WEB_SERVER_HTTP_SERVER_H
