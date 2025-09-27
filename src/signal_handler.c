#include "execute.h"

void  handle_signal(int signal)
{
  if (signal == SIGINT)
    printf("Bank got SIGINT\n");
  if (signal == SIGQUIT)
    printf("Bank got SIGQUIT\n");

}
