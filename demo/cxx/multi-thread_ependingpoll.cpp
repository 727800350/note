#include <stdint.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/syscall.h>
#include "nshead.h"
#include "Configure.h"
#include <string>
#include <comlogplugin.h>
#include <ependingpool.h>
#include <netinet/tcp.h>
#include "ul_net.h"

typedef struct _conf_t{
    int listen_port;
    int timeout;
    int worker_num;
}conf_t;

typedef struct _thread_data_t{
    char buffer[1024];
    int len;
}thread_data_t;

#define FATAL(fmt, arg...) do { \
    com_writelog(COMLOG_FATAL, "[%s:%d:%s] " fmt, __FILE__, __LINE__, __FUNCTION__, ## arg); \
    } while (0)

#define WARNING(fmt, arg...) do { \
    com_writelog(COMLOG_WARNING, "[%s:%d:%s] " fmt, __FILE__, __LINE__, __FUNCTION__, ## arg); \
    } while (0)

#define NOTICE(fmt, arg...) do { \
    com_writelog(COMLOG_NOTICE, "[%s:%d:%s] " fmt, __FILE__, __LINE__, __FUNCTION__, ## arg); \
    } while (0)

#define TRACE(fmt, arg...) do { \
    com_writelog(COMLOG_TRACE, "[%s:%d:%s] " fmt, __FILE__, __LINE__, __FUNCTION__, ## arg); \
    } while (0)

#define DEBUG(fmt, arg...) do { \
    com_writelog(COMLOG_DEBUG, "[%s:%d:%s] " fmt, __FILE__, __LINE__, __FUNCTION__, ## arg); \
    } while (0)

conf_t g_conf;
ependingpool g_workpool;
pthread_key_t g_thread_key;
pthread_once_t g_thread_key_once = PTHREAD_ONCE_INIT;

int usage(const char *prog) {
    fprintf(stderr, "%s -c conf [-h]\n", prog);

    return 0;
}

int init(const char *conf_path) {
    comcfg::Configure conf;
    int ret = conf.load(".", conf_path);
    if (ret != 0) {
        fprintf(stderr, "load conf error\n");
        return -1;
    }

    g_conf.listen_port = conf["listen_port"].to_int32();
    g_conf.timeout = conf["timeout"].to_int32();
    g_conf.worker_num = conf["worker_num"].to_int32();

    ret = com_loadlog("./", conf_path);
    if (ret != 0) {
        fprintf(stderr, "load comlog error\n");
        return -1;
    }

    return 0;
}

void stop(int) {
    g_workpool.stop();
    NOTICE("stop now");
}

int process(){
    thread_data_t *data = (thread_data_t *)pthread_getspecific(g_thread_key);
    NOTICE("%s %d", data->buffer, data->len);

    return 0;
}

void *worker(void *arg) {
    int tid = (intptr_t)(arg);
    int ret = 0;

    ret = com_openlog_r();
    if (ret != 0) {
        fprintf(stderr, "com_openlog_r error\n");
        exit(1);
    }

    thread_data_t data;
    pthread_setspecific(g_thread_key, &data);

    int offset  = -1;
    int fd = -1;
    while (g_workpool.is_run()) {
        ret = g_workpool.fetch_item(&offset, &fd);
        if (ret != 0) {
            continue;
        }
        while (g_workpool.is_run() and (ret = read(fd, data.buffer, sizeof(data.buffer)) > 0)) {
            data.buffer[ret] = '\0';
            data.len = ret;
            ret = process();
            if (ret != 0) {
                FATAL("process error");
                break;
            }
        }
    }

    WARNING("worker %d exiting", tid);
    com_closelog_r();

    return (void *)0;
}

void *listener(void *) {
    int fd = 0;
    if ((fd = ul_tcplisten(g_conf.listen_port, 128)) == -1) {
        FATAL("listening on port[%d]!", g_conf.listen_port);
        exit(1);
    }

    int on = 1;
    setsockopt(fd, IPPROTO_TCP, TCP_NODELAY, &on, sizeof(on));
    g_workpool.set_listen_fd(fd);

    while (g_workpool.is_run()) {
        g_workpool.check_item();
    }
    WARNING("listener exiting");
    return (void *)0;
}

void make_thread_key() {
    pthread_key_create(&g_thread_key, NULL);
}

int main(int argc, char* argv[]) {
    char *conf_path = NULL;
    char c = 0;
    while ((c = getopt(argc, argv, "c:s:fh")) != -1) {
        switch (c) {
            case 'c':
                conf_path = optarg;
                break;
            case 'h':
                usage(argv[0]);
                return 0;
            default:
                usage(argv[0]);
                return -1;
        }
    }

    int ret = init(conf_path);
    if (ret != 0) {
        fprintf(stderr, "int error");
        return -1;
    }

    signal(SIGPIPE, SIG_IGN);
    signal(SIGINT, stop);

    g_workpool.set_epoll_timeo(50);
    g_workpool.set_conn_timeo(6);
    g_workpool.set_sock_num(2000);
    g_workpool.set_queue_len(1000);

    pthread_once(&g_thread_key_once, make_thread_key);

    pthread_t tid_listener;
    pthread_create(&tid_listener, NULL, listener, NULL);

    pthread_t *tid_worker = new pthread_t[g_conf.worker_num];
    for (int i = 0; i < g_conf.worker_num; i++) {
        pthread_create(&tid_worker[i], NULL, worker, (void *)i);
    }

    pthread_join(tid_listener, NULL);
    for (int i = 0; i < g_conf.worker_num; i++) {
        pthread_join(tid_worker[i], NULL);
    }

    delete []tid_worker;
    pthread_key_delete(g_thread_key);
    com_closelog(60000);

    return 0;
}

/* vim: set ts=4 sw=4 sts=4 tw=100: */

