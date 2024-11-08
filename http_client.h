// 
// Create by kong on 2024/11/6
//

#ifndef WEB_SERVER_HTTP_CLIENT_H
#define WEB_SERVER_HTTP_CLIENT_H

#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <cstring>
#include <iostream>
#include "status_code.h"
#include "Socket.h"


namespace WebServer {

class HttpClient {
public:
    HttpClient() {
        printf("Connect Servers: \n");
        int sockfd = socket(AF_INET, SOCK_STREAM, 0);
        error_if(sockfd == -1, "socket create error.");

        struct sockaddr_in serv_addr = {0};
        bzero(&serv_addr, sizeof(serv_addr));
        serv_addr.sin_family = AF_INET;
        serv_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
        serv_addr.sin_port = htons(8888);
        printf("Connect Client:\n");
        int rv = connect(sockfd, (sockaddr*)&serv_addr, sizeof(serv_addr));
        error_if(rv == -1, "socket connect error");

        do {
            char buf[1024];
            bzero(&buf, sizeof(buf));
            scanf("%s", buf);
            ssize_t write_bytes = write(sockfd, buf, sizeof(buf));
            if(write_bytes == -1) {
                printf("socket already disconnected, can't write any more!\n");
                break;
            }
            bzero(&buf, sizeof(buf));
            ssize_t read_bytes = read(sockfd, buf, sizeof(buf));
            if (read_bytes > 0) {
                printf("message from server: %s\n", buf);
            } else if(read_bytes == 0) {
                printf("server socket disconnected!\n");
                break;
            } else if(read_bytes == -1) {
                close(sockfd);
                error_if(true, "socket read error");
            }
        } while (1);
        printf("connect: %d\n", rv);
    }

};


class Response {

};




    class Connection {
public:
    explicit Connection(Socket *socket): socket_(socket) {
    }
    int Connect() {
        if (socket_ == nullptr) {
            return error_code(ErrorCode::indata_error);
        }
        int sockfd = socket_->Getfd();
        struct sockaddr_in serv_addr = {0};
        bzero(&serv_addr, sizeof(serv_addr));
        serv_addr.sin_family = AF_INET;
        serv_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
        serv_addr.sin_port   = htons(8888);
        return connect(sockfd, (sockaddr*)&serv_addr, sizeof(serv_addr));
    }
    int DisConnect() {
        if (socket_ == nullptr) {
            return error_code(ErrorCode::indata_error);
        }
        int sockfd = socket_->Getfd();
        if (close(sockfd) == -1) {
            return error_code(ErrorCode::unknown);
        }
        return error_code(ErrorCode::ok);
    }
private:
    Socket *socket_ = nullptr;
};

}

#endif //WEB_SERVER_HTTP_CLIENT_H
