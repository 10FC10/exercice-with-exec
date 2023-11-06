#ifndef HEADER_H
#define HEADER_H


#define MAX_POSTI 80
#define NUM_CLIENT 50

#define LIBERO 0
#define OCCUPATO 1
#define INAGGIORNAMENTO 2

#define MUTEX 0

typedef struct{

	unsigned int id_cliente;
	unsigned int stato;

}posto;


int Wait_Sem(int, int);
int Signal_Sem(int, int);

//la funzione produttore che sarebbe il client vuole in ingresso
//il numero di posti da prenotare, il pid del cliente, il descrittore e il puntatore alla risorsa condivisa disp

void produttore(posto*, int,unsigned int, int*, int );




#endif
