#include "header.h"

#include <time.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <sys/wait.h>

void visualizzaposti(posto*, int);

int main(){

	key_t key_shm, key_sem;
	key_shm=IPC_PRIVATE;
	key_sem=IPC_PRIVATE;

	int id_sem,id_shm;

	posto *posti;
	
	int* disponibilita;

	id_shm=shmget(key_shm,sizeof(posto)*MAX_POSTI+sizeof(int),IPC_CREAT | 0664);

	if(id_shm<0){
	
	perror("errore nels shmget \n");
	exit(1);

	}

	posti=(posto*)shmat(id_shm,0,0);

	if(posti==((void*)-1)){
	
	disponibilita=(int*)shmat(id_shm,0,0);

        perror("errore nella shmat \n");
        exit(1);}
	
	//con questa istruzione punta alla prima casella libera dopo un numero di indirizzi pari a MAX_POSTI cioè punta all'indirizzo che spetta a disponibilita

	disponibilita = (int *)(posti+ MAX_POSTI);


	id_sem=semget(key_sem,1,IPC_CREAT | 0664);
	
	if(id_sem<0){
	
	perror("errore nella semget \n");
	exit(1);
	
	}

	semctl(id_sem,MUTEX,SETVAL,1);

	for(int i=0; i<MAX_POSTI; i++) {
	
		posti[i].stato = LIBERO;
		posti[i].id_cliente = 0;
	}

	(*disponibilita)=MAX_POSTI;
	
	for(int i=0;i<NUM_CLIENT;i++){
	
	pid_t pid;
	
	srand(getpid()*time(NULL));
        int temprand=1+rand()%5;
	sleep(temprand);

	visualizzaposti(posti,id_sem);

	pid=fork();
	
	if(pid<0){
	
	perror("errore nella fork \n");
	exit(1);

	}

	if(pid==0){
	
	int numprenot=1+rand()%4;

       	int pidprocesso=getpid();

	produttore(posti ,id_sem,pidprocesso,disponibilita, numprenot);

	
	exit(0);	//questa è la exit del processo figlio

	}	

	}

	int status;

	for(int i=0;i<NUM_CLIENT;i++){
	
		wait(&status);
	
	}

	semctl(id_sem, IPC_RMID, 0);
	shmctl(id_shm, IPC_RMID, 0);

	return 0;

}

void visualizzaposti(posto* posti,int id_sem){

	sleep(1);
	
	Wait_Sem(id_sem,MUTEX);

	for(int i=0;i<MAX_POSTI;i++){
	
	if(posti[i].stato==LIBERO){
	printf("posto: %d LIBERO \n",i);
	}
	
	if(posti[i].stato==OCCUPATO){
	printf("posto: %d OCCUPATO dal cliente: [%d] \n", i, posti[i].id_cliente);
	}
	
	if(posti[i].stato==INAGGIORNAMENTO){
	printf("posto: %d IN AGGIORNAMENTO \n",i);	
	
	}
	
	}

	Signal_Sem(id_sem,MUTEX);

}

