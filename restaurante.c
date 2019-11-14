#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <signal.h>
#include <stdlib.h>
#include <sys/wait.h>

void manejadora_somelier(int sig);
void manejadora_jefesala(int sig);
void manejadora_pinches(int sig);
void manejadora_mozo(int sig);
int calculaAleatorios(int min, int max);

int main(int argc, char *argv[]) {
	//srand(time(NULL));
	pid_t som, jefe;
	if(argc!=2){
		printf("ERROR! El numero de argumentos es superior al valido\n");
		exit(-1);
	}else{
		int num;
		num = atoi(argv [1]);
		if(num<=0){
			printf("El numero de pinches de cocina ha de ser mayor que 0\n");
			exit(-1);
		}else{
			//sumamos a los pinches el jefe de sala y somelier 
			struct sigaction somelier;
			struct sigaction jefe_sala;
			struct sigaction pinches;
			int numtotal,num_ale;
			numtotal = num + 1 + 1;
			int i,j;
			pid_t *trabajadores, pid, pid_mozo;
			trabajadores = (pid_t*)malloc(sizeof(pid_t)*numtotal);

			for(i=0;i<numtotal;i++) {
				pid = fork();
				trabajadores[i]=pid;
				if(pid==-1){
					perror("Error al crear el proceso hijo\n");
				}else{
					if(pid==0){
						switch(i){
							//creamos al somelier
							case 0:
								printf("Soy el somelier con pid: %d, y mi padre es el chef con pid: %d\n", getpid(), getppid());
								somelier.sa_handler=manejadora_somelier;
								if (-1==sigaction(SIGUSR1,&somelier,NULL)) {
									perror("SOMELIER: sigaction");
							 		return 1;
		 						}
		 						if (-1==sigaction(SIGUSR2,&somelier,NULL)) {
									perror("SOMELIER: sigaction");
							 		return 1;
		 						}
		 						pause();
								break;
							case 1:
								printf("Soy el jefe de sala con pid: %d, y mi padre es el chef con pid: %d\n", getpid(), getppid());
								jefe_sala.sa_handler=manejadora_jefesala;
								if(-1==sigaction(SIGUSR1,&jefe_sala,NULL)){
									perror("JEFESALA: sigaction");
									return 1;
								}
								pause();
								break;
							default:
								printf("Soy el pinche numero %d con pid: %d, y mi padre es el chef com pid: %d\n", i, getpid(), getppid());
								pinches.sa_handler=manejadora_pinches;
								if(-1==sigaction(SIGUSR1,&pinches,NULL)){
									perror("PINCHES: sigaction");
									return 1;
								}
								pause();
						}
					}else{
						som = trabajadores[0];
						jefe = trabajadores[1];
					}
				}
			}
			kill(som,SIGUSR1);
			sleep(2);
			num_ale = calculaAleatorios(0,1);
			if(num_ale==0){
				printf("Faltan ingredinetes\n");
				kill(som,SIGUSR1);
			}else{
				printf("Falta vino\n");
				kill(som,SIGUSR2);
			}
		}
	}

	return 0;
}




int calculaAleatorios(int min, int max){
	return rand() % (max-min+1) + min;
}

void manejadora_somelier(int sig){
	printf("Prie");
	struct sigaction mozo2;
	int status, found;
	pid_t mozo = fork();
	if(mozo==-1){
		perror("Error al crear el proceso hijo\n");
	}else{
		if(mozo==0){
			printf("Soy el mozo de los recados con pid: %d, y mi padre es el somelier con pid: %d\n", getpid(), getppid());
			mozo2.sa_handler=manejadora_mozo;
			if(-1==sigaction(SIGUSR1,&mozo2,NULL)){
				perror("MOZO: sigaction");
			}
			if(-1==sigaction(SIGPIPE,&mozo2,NULL)){
				perror("MOZO: sigaction");
			}
			pause();
			found = WEXITSTATUS(status);
			if(found==1){
				printf("El mozo encontro lo que faltaba\n");
			}else{
				printf("El mozo no encontro lo que faltaba\n");
			}
		}else{
			kill(mozo,SIGPIPE);
		}
	}
	exit (1);
}

void manejadora_jefesala(int sig){
	printf("Estoy en la manejadora de jefe de sala\n");
	exit(1);
}

void manejadora_pinches(int sig){
	printf("Estoy en la manejadora de los pinches\n");
	exit(1);
}

void manejadora_mozo(int sig){
	struct sigaction mozo;
	mozo.sa_handler=manejadora_mozo;
	if(-1==sigaction(SIGPIPE,&mozo,NULL)){
		perror("MOZO: sigaction");
	}
	if(-1==sigaction(SIGUSR1,&mozo,NULL)){
		perror("MOZO: sigaction");
	}
	pause();
	int num;
	num = calculaAleatorios(0,1);
	exit(num);
}