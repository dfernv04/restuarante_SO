#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <signal.h>
#include <stdlib.h>
#include <sys/wait.h>

void manejadora_somelier(int signum);

int main(int argc, char *argv[]) {
	pid_t som;
	if(argc!=2){
		printf("ERROR! El numero de argumentos es superior al valido\n");
		exit(-1);
	}else{
		int num;
		num = atoi(argv [1]);
		if(num==0){
			printf("El numero de pinches de cocina ha de ser mayor que 0\n");
			exit(-1);
		}else{
			//sumamos a los pinches el jefe de sala y somelier 
			struct sigaction somelier;
			int numtotal;
			numtotal = num + 1 + 1;
			int i,j=0;
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
		 						pause();
								break;
							case 1:
								printf("Soy el jefe de sala con pid: %d, y mi padre es el chef con pid: %d\n", getpid(), getppid());
								pause();
								break;
							default:
								printf("Soy el pinche numero %d con pid: %d, y mi padre es el chef com pid: %d\n", i, getpid(), getppid());
								pause();
						}
					}else{
						som=trabajadores[0];
					}
				}
			}
			kill(som,SIGUSR1);
		}
	}

	return 0;
}

void manejadora_somelier(int signum){

	pid_t mozo = fork();
	if(mozo==-1){
		perror("Error al crear el proceso hijo\n");
	}else{
		if(mozo==0){
			printf("Soy el mozo de los recados con pid: %d, y mi padre es el somelier con pid: %d\n", getpid(), getppid());
			pause();
		}else{
			printf("Estoy aqui\n");
		}
	}
	exit (1);
}