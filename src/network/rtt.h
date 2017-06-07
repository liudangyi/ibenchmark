#include <pthread.h>
#include <sys/socket.h>
#include <netinet/in.h>

#define check_error(val) _check_error(val, __FILE__, __LINE__, __func__)

int _check_error(int val, const char *file, const int line, const char* func) {
    if (val == -1) {
        char info[64];
        sprintf(info, "%s:%d %s", file, line, func);
        perror(info);
        exit(-1);
    }
    return val;
}

void *rtt_server_accept(void *sockfd_ptr) {
    int sockfd = *(int *)sockfd_ptr;
    struct sockaddr_in client_addr;
    uint32_t addr_size = sizeof(client_addr);
    int connfd = check_error(accept(sockfd, &client_addr, &addr_size));
    char buffer[16];
    int count = 16;

    while (1) {
        count = recv(connfd, buffer, 16, 0);
        if (count == 0) {
            break;
        }
        send(connfd, buffer, count, 0);
    }

    close(connfd);
    close(sockfd);
    return NULL;
}

void rtt_server(pthread_t *tid) {
    static int sockfd;
    struct sockaddr_in serv_addr;

    sockfd = check_error(socket(PF_INET, SOCK_STREAM, 0));
    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(5000);
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);

    check_error(bind(sockfd, &serv_addr, sizeof(serv_addr)));
    check_error(listen(sockfd, 5));

    if (tid) {
        pthread_create(tid, NULL, &rtt_server_accept, &sockfd);
    } else {
        rtt_server_accept(&sockfd);
    }
}

void rtt_client(const char *addr) {
    int sockfd = check_error(socket(PF_INET, SOCK_STREAM, 0));
    struct sockaddr_in serv_addr;

    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(5000);
    serv_addr.sin_addr.s_addr = inet_addr(addr);

    check_error(connect(sockfd, &serv_addr, sizeof(serv_addr)));
    char buffer[16] = { 'h', 'e', 'l', 'l', 'o' };
    int count = 5;

    BEGIN_TEST_LOOP(1000)
        count = send(sockfd, buffer, count, 0);
        assert(check_error(count) == 5);
        count = recv(sockfd, buffer, 16, 0);
        assert(check_error(count) == 5);
    END_TEST_LOOP

    close(sockfd);
}

BEGIN_TEST_PREP(tcp_rtt)
    pthread_t tid;

    rtt_server(&tid);
    rtt_client("127.0.0.1");
    pthread_join(tid, NULL);
END_TEST_PREP
