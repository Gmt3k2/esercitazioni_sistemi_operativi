#include "common.h"
#include <stdio.h>

void server(int msgq_guest, int msgq_print)
{
	int i;
	
	msgRichiestaClient msgRicevuto;
	msgRichiestaServer msgRichiestaAPrinter;
	msgRichiestaAPrinter.message_type = MSG_SERVER_PRINTER;


	printf("[server ] Server ready...\n");
	while(1)
	{
		for(i=0; i<BUFFER_DIM; i++)
		{
			msgrcv(msgq_guest, (void*)&msgRicevuto, sizeof(msgRichiestaClient) - sizeof(long), 0, 0);
			
			if(msgRicevuto.message_type == MSG_EXIT){
				msgRichiestaAPrinter.pidBuffer[i] = -1;

				msgRichiestaServer msgEndPrinter;
				msgEndPrinter.message_type = MSG_EXIT;
				msgEndPrinter.pidBuffer[0] = -1;
				msgsnd(msgq_print,&msgEndPrinter, sizeof(msgRichiestaServer)-sizeof(long), 0);
				
				printf("Server terminato\n");
				return;
			}
			else
			{
				msgRichiestaAPrinter.pidBuffer[i] = msgRicevuto.msgProcesso;
			}
			
		}
		
	msgsnd(msgq_print, (void*)&msgRichiestaAPrinter, sizeof(msgRichiestaServer) - sizeof(long),0);	
	}
	return;
}
