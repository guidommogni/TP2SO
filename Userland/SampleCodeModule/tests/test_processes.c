#include "../include/test.h"

//TO BE INCLUDED
void endless_loop()
{
  while (1)
    ;
}

#define MAX_PROCESSES 10 //Should be around 80% of the the processes handled by the kernel

enum State
{
  ERROR,
  RUNNING,
  BLOCKED,
  KILLED
};

typedef struct P_rq
{
  uint32_t pid;
  enum State state;
} p_rq;

void test_processes()
{
  p_rq p_rqs[MAX_PROCESSES];
  uint8_t rq;
  uint8_t alive = 0;
  uint8_t action;

  while (1)
  {

    // Create MAX_PROCESSES processes
    for (rq = 0; rq < MAX_PROCESSES; rq++)
    {
      char *argv[] = {"endless_loop"};
      p_rqs[rq].pid = _createProcess(&endless_loop, 1, argv, 0); //// TODO: Port this call as required
      
      printInt(p_rqs[rq].pid);
      print("\n");

      if (p_rqs[rq].pid == -1)
      {                                    // TODO: Port this as required
        print("Error creating process\n"); // TODO: Port this as required
        return;
      }
      else
      {
        p_rqs[rq].state = RUNNING;
        alive++;
      }
    }

    // Randomly kills, blocks or unblocks processes until every one has been killed
    while (alive > 0)
    {

      for (rq = 0; rq < MAX_PROCESSES; rq++)
      {
        action = GetUniform(2) % 2;

        switch (action)
        {
        case 0:
          if (p_rqs[rq].state == RUNNING || p_rqs[rq].state == BLOCKED)
          {
            if (_kill(p_rqs[rq].pid) == -1)
            {                                   // TODO: Port this as required
              print("Error killing process\n"); // TODO: Port this as required
              return;
            }
            p_rqs[rq].state = KILLED;
            alive--;
          }
          break;

        case 1:
          if (p_rqs[rq].state == RUNNING)
          {
            if (_block(p_rqs[rq].pid) == -1)
            {
              _ps();                             // TODO: Port this as required
              print("Error blocking process\n"); // TODO: Port this as required
              return;
            }
            p_rqs[rq].state = BLOCKED;
          }
          break;
        }
      }

      // Randomly unblocks processes
      for (rq = 0; rq < MAX_PROCESSES; rq++)
        if (p_rqs[rq].state == BLOCKED && GetUniform(2) % 2)
        {
          if (_unblock(p_rqs[rq].pid) == -1)
          {                                      // TODO: Port this as required
            print("Error unblocking process\n"); // TODO: Port this as required
            return;
          }
          p_rqs[rq].state = RUNNING;
        }
    }
  }
}
