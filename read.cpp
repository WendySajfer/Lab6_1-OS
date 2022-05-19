#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <stdlib.h>

#include <iostream>
#include <string>
#include <cstring>

using namespace std;
int main()
{
    char* ptr;
    void* ptr1;
    int* buf;
    int md, flag;
    key_t key;
    int fd;
    char buf_;
    string str;
    while (true) {
        printf("set message:\n");
        str = "";
        cin.clear();
        cin.ignore(1, '\n');
        getline(cin, str);
        if ((key = ftok("./key", 0)) < 0) {
            printf("Errot of generate key\n");
            exit(-1);
        }
        if ((md = shmget(key, 8192 * sizeof(int), 0777 | IPC_CREAT | IPC_EXCL)) < 0) {
            if (errno != EEXIST) {
                printf("Errot of create shared memory\n");
                exit(-1);
            }
            else {
                if ((md = shmget(key, 8192 * sizeof(int), 0)) < 0) {
                    printf("Error of find shared memory\n");
                    exit(-1);
                }
            }
        }
        if ((ptr1 = shmat(md, NULL, 0)) == (void*)(-1)) {
            printf("Error attach shared memory\n");
            exit(-1);
        }
        strcpy((char*)ptr1, (str.c_str() + '\0'));
        if (shmdt(ptr1) < 0) {
            printf("Can't detach shared memory\n");
            exit(-1);
        }
        printf("get message.Input char:\n");
        cin >> buf_;
        if (!((md = shmget(key, 8192 * sizeof(int), 0)) < 0)) {
            while (true) {
                ptr = (char*)shmat(md, NULL, 0);
                buf = (int*)ptr;
                if (buf != (int*)-1) {
                    if (ptr[0] != '\0') {
                        break;
                    }
                }
            }
        }
        int index;
        index = 0;
        while (ptr[index] != '\0') {
            cout << (ptr[index]);
            ++index;
        }
        if (shmctl(md, IPC_RMID, NULL) < 0) {
            printf("Can't free shared memory\n");
        }
        printf("\n");
    }
    return 0;
}
