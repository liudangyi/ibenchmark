#include <pthread.h>
#include <sys/socket.h>
#include <netinet/in.h>

void *bandwidth_server_accept(void *sockfd_ptr) {
    int sockfd = *(int *)sockfd_ptr;
    struct sockaddr_in client_addr;
    uint32_t addr_size = sizeof(client_addr);
    const int SIZE = 32 << 20; // 32 MB
    char *buffer = malloc(SIZE);

    while (1) {
        int connfd = check_error(accept(sockfd, (struct sockaddr*) &client_addr, &addr_size));

        while (1) {
            int count = recv(connfd, buffer, SIZE, 0);
            if (count == 0) {
                break;
            }
        }
        close(connfd);
    }
    close(sockfd);
    return NULL;
}

void bandwidth_server(pthread_t *tid) {
    static int sockfd;
    struct sockaddr_in serv_addr;

    sockfd = check_error(socket(PF_INET, SOCK_STREAM, 0));
    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(5000);
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);

    check_error(bind(sockfd, (struct sockaddr*) &serv_addr, sizeof(serv_addr)));
    check_error(listen(sockfd, 5));

    if (tid) {
        pthread_create(tid, NULL, &bandwidth_server_accept, &sockfd);
    } else {
        bandwidth_server_accept(&sockfd);
    }
}

void bandwidth_client(const char *addr) {
    int sockfd = check_error(socket(PF_INET, SOCK_STREAM, 0));
    struct sockaddr_in serv_addr;

    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(5000);
    serv_addr.sin_addr.s_addr = inet_addr(addr);

    check_error(connect(sockfd, (struct sockaddr*) &serv_addr, sizeof(serv_addr)));
    const int SIZE = 32 << 20; // 32 MB
    char *buffer = malloc(SIZE);
    int count;

    BEGIN_TEST_ONCE
        count = send(sockfd, buffer, SIZE, 0);
        assert(check_error(count) == SIZE);
    END_TEST_ONCE

    close(sockfd);
}

pthread_t bandwidth_tid;

BEGIN_TEST_PREP(tcp_bandwidth)
    if (!bandwidth_tid) {
        bandwidth_server(&bandwidth_tid);
    }
    bandwidth_client("127.0.0.1");
END_TEST_PREP

BEGIN_TEST_PREP(tcp_bandwidth_server)
    bandwidth_server(NULL);
END_TEST_PREP

BEGIN_TEST_PREP(tcp_bandwidth_client)
    bandwidth_client("70.95.174.115");
END_TEST_PREP
