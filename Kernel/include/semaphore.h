#ifndef SEMAPHORE_H
#define SEMAPHORE_H
#include "stdio.h"
#include "defs.h"
#include "scheduler.h"
#define MAX_NAME 10
#define MAX_SEM 10
typedef struct processNode
{
    struct processNode *next;
    uint64_t pid;
} process_t;
typedef struct
{
    char name[MAX_NAME];
    process_t *firstProcess; // primer proceso esperando (waiting) en la fila
    process_t *lastProcess;  // ultimo proceso esperando en la fila
    uint64_t lock;
    uint64_t value;
    uint64_t size; // cantidad de procesos que usan el sem
    uint64_t sizeList;  // cantidad de procesos bloqueados
} sem_t;
extern uint64_t _xchg(uint64_t *lock, uint64_t value);
void initSems();
uint64_t semOpen(char *name, uint64_t initValue);
uint64_t semClose(char *name);
uint64_t semWait(uint64_t semIndex); // lock a semaphore. If successful (the lock was acquired), sem_wait() and sem_trywait() will return 0.  Otherwise, -1 is returned and errno is set, and the state of the semaphore is unchanged.
uint64_t semPost(uint64_t semIndex); // the value of the semaphore is incremented, and all threads which are waiting on the semaphore are awakened. If successful, sem_post() will return 0.  Otherwise, -1 is returned
#endif