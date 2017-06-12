/*
 * We define setup time as the time required to open a connection to given
 * host immediately before the first packet could be sent.
 *
 * The teardown time is defined as the time required so that we can perform
 * the next setup operation.
 */

void *overhead_server_accept(void *sockfd_ptr) {
    int sockfd = *(int *)sockfd_ptr;
    struct sockaddr_in client_addr;
    uint32_t addr_size = sizeof(client_addr);
    char buffer[16];

    while (1) {
        int connfd = check_error(accept(sockfd, (struct sockaddr*) &client_addr, &addr_size));
        check_error(recv(connfd, buffer, 16, 0));
        check_error(close(connfd));
    }

    check_error(close(sockfd));
    return NULL;
}

int overhead_server(pthread_t *tid) {
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
        pthread_create(tid, NULL, &overhead_server_accept, &sockfd);
    } else {
        overhead_server_accept(&sockfd);
    }
    return sockfd;
}

void overhead_client(const char *addr, int setup) {
    struct sockaddr_in serv_addr;

    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(5000);
    serv_addr.sin_addr.s_addr = inet_addr(addr);

    if (setup) {
        timeit_start();
        int sockfd = check_error(socket(PF_INET, SOCK_STREAM, 0));
        check_error(connect(sockfd, (struct sockaddr*) &serv_addr, sizeof(serv_addr)));
        timeit_end();
        check_error(close(sockfd));
    } else {
        int sockfd = check_error(socket(PF_INET, SOCK_STREAM, 0));
        check_error(connect(sockfd, (struct sockaddr*) &serv_addr, sizeof(serv_addr)));
        timeit_start();
        check_error(close(sockfd));
        timeit_end();
    }
}

pthread_t overhead_tid;

BEGIN_TEST_PREP(tcp_overhead_setup)
    if (!overhead_tid) {
        overhead_server(&overhead_tid);
    }
    overhead_client("127.0.0.1", 1);
END_TEST_PREP

BEGIN_TEST_PREP(tcp_overhead_teardown)
    if (!overhead_tid) {
        overhead_server(&overhead_tid);
    }
    overhead_client("127.0.0.1", 0);
END_TEST_PREP

BEGIN_TEST_PREP(tcp_overhead_remote_setup)
    overhead_client("100.83.8.10", 1);
END_TEST_PREP

BEGIN_TEST_PREP(tcp_overhead_remote_teardown)
    overhead_client("100.83.8.10", 0);
END_TEST_PREP
