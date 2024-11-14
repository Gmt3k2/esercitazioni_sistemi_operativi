#include "common.h"

void printer(int msgq_print)
{
	int counter = 1, i;
	msgRichiestaServer m;

	printf("[printer] Printer Ready...\n");

	while(1)
	{
		msgrcv(msgq_print, (void*)&m, sizeof(msgRichiestaServer)-sizeof(long), 0, 0);		
		for(i=0; i<BUFFER_DIM; i++)
		{
			if(m.pidBuffer[i] == -1){
				printf("Printer terminato\n");
				return;
			}
			printf("{printer}\t[%u] %u\n", counter++, m.pidBuffer[i]);
		}
	}
	return;
}
