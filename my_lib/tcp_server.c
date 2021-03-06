#include "my_net.h"

int init_tcpserver(in_port_t myport, int backlog) {
    struct sockaddr_in my_adrs;
    int sock_listen;

    /* サーバ(自分自身)の情報をsockaddr_in構造体に格納する */
    memset(&my_adrs, 0, sizeof(my_adrs));
    my_adrs.sin_family = AF_INET;
    my_adrs.sin_port = htons(myport);
    my_adrs.sin_addr.s_addr = htonl(INADDR_ANY);

    /* 待ち受け用ソケットをSTREAMモードで作成する */
    if ((sock_listen = socket(PF_INET, SOCK_STREAM, 0)) == -1) {
        exit_errmesg("socket()");
    }

    /* 待ち受け用のソケットに自分自身のアドレス情報を結びつける */
    if (bind(sock_listen, (struct sockaddr *) &my_adrs, sizeof(my_adrs)) == -1) {
        exit_errmesg("bind()");
    }

    /* クライアントからの接続を受け付ける準備をする */
    if (listen(sock_listen, backlog) == -1) {
        exit_errmesg("listen()");
    }

    return (sock_listen);
}

ssize_t my_receive(int sock, void *buf, size_t buf_size) {
    ssize_t size;
    if ((size = recv(sock, buf, buf_size, 0)) == -1) {
        exit_errmesg("recv()");
    }
    return size;
}

ssize_t my_send(int sock, const void *buf, size_t str_size, int flag) {
    ssize_t size;
    if ((size = send(sock, buf, str_size, flag)) == -1) {
        exit_errmesg("recv()");
    }
    return size;
}

pid_t my_fork() {
    pid_t pid = fork();
    if (pid == -1) {
        exit_errmesg("fork()");
    }
    return pid;
}

void my_echo(int sock, char *buf, size_t buf_size) {
    int str_size;
    do {
        str_size = my_receive(sock, buf, buf_size);
        my_send(sock, buf, str_size,0);
    } while ((buf[str_size - 1]) != '\n');
}