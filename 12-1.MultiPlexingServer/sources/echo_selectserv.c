#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <sys/select.h>

#define BUF_SIZE 100
void error_handling(char* buf);

int main(int argc, char* argv[]) {
    printf("hello5 \n");
    int serv_sock, clnt_sock;
    struct sockaddr_in serv_adr, clnt_adr;
    struct timeval timeout;
    fd_set reads, cpy_reads;

    socklen_t adr_sz;
    int fd_max, str_len, fd_num;
    char buf[BUF_SIZE];

    if (argc != 2) {
        printf("Usage : %s <port> \n", argv[0]);
        exit(1);
    }

    serv_sock = socket(PF_INET, SOCK_STREAM, 0);
    memset(&serv_adr, 0, sizeof(serv_adr));
    serv_adr.sin_family = AF_INET;
    serv_adr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_adr.sin_port = htons(atoi(argv[1]));

    if (bind(serv_sock, (struct sockaddr*) &serv_adr, sizeof(serv_adr)) == -1) {
        error_handling("bind() error");
    }

    if (listen(serv_sock, 5) == -1) {
        error_handling("listen() error");
    }

    FD_ZERO(&reads);
    FD_SET(serv_sock, &reads);      // 데이터 수신여부를 관찰하는 관찰대상에 서버 소켓을 포함시킴
    fd_max = serv_sock;
    printf("File Descriptor MAX: %d \n", fd_max);

    while (1) {
        cpy_reads = reads;
        timeout.tv_sec = 5;
        timeout.tv_usec = 5000;

        if ((fd_num = select(fd_max + 1, &cpy_reads, 0, 0, &timeout)) == -1) {
            // Error
            error_handling("select error");
            break;
        }
        if (fd_num == 0) {
            // Timeout
            printf("Timeout \n");
            continue;
        }

        for (int i = 0; i < fd_max + 1; i++) {
            if (FD_ISSET(i, &cpy_reads)) {      // 상태 변화가 있었던(수신된 데이터가 있는 소켓의) 파일 디스크립터를 찾고 있음
                if (i == serv_sock)     // connection request!
                {
                    adr_sz = sizeof(clnt_adr);
                    clnt_sock = accept(serv_sock, (struct sockaddr*)&clnt_adr, &adr_sz);
                    FD_SET(clnt_sock, &reads);
                    if (fd_max < clnt_sock) {
                        fd_max = clnt_sock;
                    }
                    printf("connected client: %d \n", clnt_sock);
                }
                else {      // read message
                    str_len = read(i, buf, BUF_SIZE);
                    if (str_len == 0) {     // close request
                        FD_CLR(i, &reads);
                        close(i);
                        printf("closed client: %d \n", i);
                    } else {
                        printf("send message to client: %s \n", buf);
                        write(i, buf, str_len);     // echo
                    }
                }
            }
        }
    }
    close(serv_sock);
    return 0;
}

void error_handling(char* buf) {
    fputs(buf, stderr);
    fputc('\n', stderr);
    exit(1);
}