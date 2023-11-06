#include "header.h"

#include <unistd.h>
#include <stdio.h>
#include <errno.h>
#include <sys/ipc.h>
#include <sys/types.h>
#include <sys/sem.h>

int Wait_Sem (int id_sem, int numsem) {
       int err;
       struct sembuf sem_buf;

       sem_buf.sem_num=numsem;
       sem_buf.sem_flg=0;
       sem_buf.sem_op=-1;

       err = semop(id_sem,&sem_buf,1);   //semaforo rosso

       if(err<0) {
         perror("Errore WAIT");
       }

       return err;
}


int Signal_Sem (int id_sem, int numsem) {
       int err;
       struct sembuf sem_buf;

       sem_buf.sem_num=numsem;
       sem_buf.sem_flg=0;
       sem_buf.sem_op=1;

       err = semop(id_sem,&sem_buf,1);   //semaforo verde

       if(err<0) {
         perror("Errore SIGNAL");
       }

       return err;
}

void produttore(posto* posti, int id_sem, unsigned int pidprocesso, int* disp, int numpren){

Wait_Sem(id_sem,MUTEX);

if(numpren>(*disp)){

	printf("spazio insufficiente \n");
	Signal_Sem(id_sem,MUTEX);
	return;

}

else{

	(*disp)=((*disp)-numpren);
	
	int index=0;

	while((index<MAX_POSTI)&&(posti[index].stato!=LIBERO)){
		
		index++;
	}

	for(int i=index;i<(index+numpren);i++){
	
	posti[i].stato=INAGGIORNAMENTO;
	
	}

	Signal_Sem(id_sem,MUTEX);
	
	for(int i=index;i<(index+numpren);i++){
	
	posti[i].id_cliente=pidprocesso;
	posti[i].stato=OCCUPATO;
	printf("il cliente [%d]: ha prenotato il posto: %d \n",pidprocesso ,i);
//	sleep(1);
	
	}

}

}

