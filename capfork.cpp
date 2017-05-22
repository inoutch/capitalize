#include <vector>
#include <iostream>
#include "capfork.h"
#include "capitalize.h"

#define BUFF_SIZE 10
//  WARN: should be more Fragment size (4 bytes)

typedef struct {
    unsigned int id;
} Fragment;

void capfork(char *args, size_t size) {
    int fildes[2]; // 0:r 1:w
    pid_t pid;

    if (pipe(fildes) == -1) {
        perror("pipe");
        exit(1);
    }

    if ((pid = fork()) == 0) {
        close(fildes[0]);

        capitalize(args);
        for (unsigned int i = 0; i < size; i++)
            write(fildes[1], &args[i], 1);
        close(fildes[1]);
        exit(1);

    } else if (pid > 0) {
        close(fildes[1]);

        int status;
        for (unsigned int i = 0; read(fildes[0], &args[i], 1) > 0; i++);

        close(fildes[0]);
        waitpid(pid, &status, 0);

    } else {
        perror("fork");
        exit(1);

    }
}

void append(const char *src, size_t size, std::vector<char> &dst) {
    for (size_t i = 0; i < size; i++) {
        dst.push_back(src[i]);
    }
}

void writeAll(int fd, const char *data, size_t size) {
    size_t r = size, sent = 0;
    unsigned int cid = 0;
    char buff[BUFF_SIZE] = {};

    while (r > 0) {
        size_t fsize = r > BUFF_SIZE - sizeof(Fragment) ? BUFF_SIZE - sizeof(Fragment) : r;
        auto fragment = (Fragment *) buff;
        fragment->id = cid++;
        memcpy(buff + sizeof(Fragment), data + sent, fsize);
        if (write(fd, buff, fsize + sizeof(Fragment)) <= 0) {
            printf("error: write");
            return;
        }
        r -= fsize;
        sent += fsize;
    }

    write(fd, buff, 1);
}

std::vector<char> readAll(int fd) {
    std::vector<char> result;
    char buff[BUFF_SIZE] = {};
    unsigned int cid = 0;

    while (true) {
        ssize_t ret = read(fd, buff, sizeof(buff));
        auto fragment = (Fragment *) buff;
        if (ret < sizeof(Fragment) || ret > BUFF_SIZE || fragment->id != cid++) {
            break;
        }
        append(buff + sizeof(Fragment), (size_t) ret - sizeof(Fragment), result);
        memset(buff, 0, sizeof(buff));
    }

    return result;
}

std::vector<char> capfork2(char *args, size_t size) {
    int c2pFd[2];
    int p2cFd[2];
    pid_t pid;

    if (pipe(p2cFd) == -1 || pipe(c2pFd) == -1) {
        perror("pipe");
        exit(1);
    }

    if ((pid = fork()) > 0) {
        close(p2cFd[1]);
        close(c2pFd[0]);

        std::vector<char> received = readAll(p2cFd[0]);
        close(p2cFd[0]);

        capitalize(&received[0]);

        writeAll(c2pFd[1], &received[0], received.size());
        close(c2pFd[1]);

        exit(1);

    } else if (pid == 0) {
        close(p2cFd[0]);
        close(c2pFd[1]);

        writeAll(p2cFd[1], args, size);
        close(p2cFd[1]);

        auto ret = readAll(c2pFd[0]);
        close(c2pFd[0]);

        int status;
        waitpid(pid, &status, 0);

        return ret;

    } else {
        perror("fork");
        exit(1);

    }
}