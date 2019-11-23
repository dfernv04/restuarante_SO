#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <signal.h>
#include <stdlib.h>
#include <time.h>
#include <sys/wait.h>

int calculaAleatorios(int min, int max);
void manejadora_som(int sig);
void manejadora_jefesala(int sig);
void manejadora_pinches(int sig);
void manejadora_mozo(int sig);

int main(int argc, char *argv[]) {	

	srand(time(NULL));
	//comprobamos que los argumentos son correcto
	if(argc!=2){
		printf("Error en el numero de argumentos\n");
		exit (-1);
	}else{
		int num = atoi(argv[1]);
		if(num<=0){
			printf("Error, el numero de pinches ha de ser mayor que 0\n");
			exit(-1);
		}else{
			printf("\tBIENVENIDO A LA SIMULACION DE UN RESTAURANTE. DISFRUTE!!\n\n");
			//si los argumentos son correctos, llega aqui y comienza la ejecucion normal
			//el numero total de trabajadores va a ser el numero de pinches + el somelier + el jefe de sala
			int numtrabajadores = num + 2;
			int i,j,k, espera, status, count_platos=0;
			//vamos a crear las estructuras necesarias
			struct sigaction som = {0};
			struct sigaction jefe = {0};
			struct sigaction pinches = {0};
			//creamos el puntero donde vamos a almacenar los pids de los trabajadores
			pid_t *trabajadores, pid, pid_somelier, pid_jefesala, pid_pinches;
			trabajadores = (pid_t*)malloc(sizeof(pid_t)*numtrabajadores);
			//vamos a crear a los hijos del chef
			for(i=0;i<numtrabajadores;i++){
				//creamos el proceso hijo
				pid=fork();
				if(pid==-1){
					perror("Error al crear el proceso hijo");
				}else{
					if(pid==0){
						switch(i){
								//codigo del somelier
							case 0:
								printf("\tSoy el somelier con pid: %d, y mi padre es el chef con pid: %d\n", getpid(), getppid());
								som.sa_handler = manejadora_som;
								if(-1==sigaction(SIGUSR1, &som, NULL)){
									perror("SOMELIER: sigaction");
									return 1;
								}
								if(-1==sigaction(SIGUSR2, &som, NULL)){
									perror("SOMELIER: sigaction");
									return 1;
								}
								pause();
								//codigo del jefe de sala
							case 1:
								printf("\tSoy el jefe de sala de estar con pid: %d, y mi padre es el chef con pid: %d\n", getpid(), getppid());
								jefe.sa_handler = manejadora_jefesala;
								if(-1==sigaction(SIGUSR1, &jefe, NULL)){
									perror("JEFE DE SALA: sigaction");
									return 1;
								}
								pause();
								//codigo de los pinches
							default:
								printf("\tSoy el pinche %d con pid: %d, y mi padre es el chef con pid: %d\n", (i-1), getpid(), getppid());
								pinches.sa_handler = manejadora_pinches;
								if(-1==sigaction(SIGUSR1, &pinches, NULL)){
									perror("PINCHES: sigaction");
									return 1;
								}
								pause();
						}
					}else{
						trabajadores[i]=pid;
					}
				}
			}
			//el chef duerme 3 segundos
			sleep(3);
			printf("\n");
			//genera un numero aleatorio (0->faltan ingredientes) (1->falta vino)
			int num_ale = calculaAleatorios(0,1);
			if(num_ale==0){
				printf("\tCHEF: Faltan ingredientes\n");
				kill(trabajadores[0], SIGUSR1);
			}else{
				printf("\tCHEF: Falta vino\n");
				kill(trabajadores[0], SIGUSR2);
			}

			//esperamos el valor del somelier que nos diga si hay vino e ingredientes y de esta
			//forma saber si podemos cocinar los platos o cerrar el restaurante
			trabajadores[0] = wait(&status);
			espera = WEXITSTATUS(status);
			printf("\n");
			//como no hemos encontrado el vino cerramos el restaurante y matamos a los procesos
			if(espera==1){
				printf("\tCHEF: Falta el vino y no se ha encontrado.\n");
				printf("\tCHEF: No se puede abrir el restaurante\n");
				printf("\n");
				printf("\t***CERRANDO RESTAURANTE...***\n");
				kill(trabajadores[1], SIGKILL);
				for(j=2;j<numtrabajadores;j++){
					//matamos los procesos que faltan
					kill(trabajadores[j],SIGKILL);
				}
				//vamos a preparar los platos
			}else{
				if(espera==2){
					printf("\tCHEF: Faltan algunos ingredientes\n");
				}else{
					printf("\tCHEF: Tenemos todo lo necesario para preparar los platos\n");
				}
				printf("\n");
				printf("\t***VAMOS A EMPEZAR A COCINAR***\n\n");
				for(k=2;k<numtrabajadores;k++){
					//llamamos a todos los pinches para que preparen los platos
					kill(trabajadores[k],SIGUSR1);
					//esperamos que nos devuelvan 1 valor para saber si han cocinado bien o no
					trabajadores[k] = wait(&status);
					espera = WEXITSTATUS(status);
					//si el valor generado por el pinche es 1 aumentamos el contador de platos preparados
					if(espera==1){
						count_platos++;
					}
				}
				printf("\n");
				//si el contador de platos preparados es 0, cerramos el restaurante y matamos a los procesos
				if(count_platos==0){
					printf("\tCHEF: No hay platos preparados para abrir el restaurante\n");
					printf("\n");
					printf("\t***CERRANDO AL RESTAURANTE...***\n");
					//matamos al jefe de sala que es el unico proceso que no ha sido matado
					kill(trabajadores[1], SIGKILL);
				}else{
					printf("\tCHEF: El numero de platos preparados por los pinches es: %d\n", count_platos);
					//llamamos al jefe de sala para que monte las mesas, abrimos el restaurante y matamos a los procesos
					printf("\n");
					printf("\t***VAMOS A MONTAR LA MESAS DEL RESTAURANTE***\n");
					kill(trabajadores[1], SIGUSR1);
					//esperamos a que el jefe de sala monte las mesas
					trabajadores[1] = wait(&status);
					//abrimos el restaurante
					printf("\n");
					printf("\t***ABRIENDO RESTAURANTE...***\n");
				}
			}
			free(trabajadores);
		}
	}
	printf("\n\n\n");
	return 0;
}

//FUNCIONES USASADAS

//funcion que nos permite calculas los numero aleatorios
int calculaAleatorios(int min, int max){
	return rand() % (max-min+1) + min;
}

//manejadora del somelier
void manejadora_som(int sig){
	int status, espera, devuelto;
	pid_t pid;
	struct sigaction mozo;
	pid = fork();
	if(pid==-1){
		perror("ERROR al crear el proceso hijo");
		exit(1);
	}else{
		if(pid==0){
			printf("\tSoy el mozo de los recados con pid: %d, y mi padre es el somelier con pid: %d\n", getpid(), getppid());
			mozo.sa_handler = manejadora_mozo;
			if(-1==sigaction(SIGPIPE, &mozo, NULL)){
				perror("MOZO: sigaction");
			}
			pause();
		}else{
			//dormimos al sommelier para dar tiempo a enmascarar la señal
			sleep(2);
			//le mandamos SIGPIPE al mozo para que busque lo que falta
			kill(pid, SIGPIPE);
			pid = wait(&status);
			espera = WEXITSTATUS(status);
			//comparamos el valor devuelto por el mozo con la señal que el 
			//chef envio al mozo para ver si cerramos el restaurante o no
			if(espera==0 && sig==12){
				devuelto = 1;
			}else{
				if(espera==0 && sig==10){
					devuelto = 2;
				}else{
					devuelto = 3;
				}
			}
		}
	}
	exit(devuelto);
}

//manejadora del jefe de sala
void manejadora_jefesala(int sig){
	sleep(3);
	printf("\tJEFE DE SALA: Las mesas han sido montadas\n");
	exit(1);
}

//manejadora de los pinches
void manejadora_pinches(int sig){
	srand(time(NULL));
	//preparamos un aleatorio para que los pinches duerman
	int num_ale = calculaAleatorios(2,5);
	sleep(num_ale);
	//generamos un aleatorio (0-> mal cocinado) y (1-> bien cocinado)
	int ale = calculaAleatorios(0,1);
	if(ale==0){
		printf("\tPINCHE: El plato no se ha cocinado bien\n");
	}else{
		printf("\tPINCHE: El plato esta listo para servirse\n");
	}
	exit(ale);
}

//manejadoras de los mozos
void manejadora_mozo(int sig){
	srand(time(NULL));
	//generamos un aleatorio para saber si el mozo encontro lo que faltaba (1) o no (0)
	int ale = calculaAleatorios(0,1);
	if(ale==1){
		printf("\tMOZO: He encontrado lo que faltaba\n");
	}else{
		printf("\tMOZO: No he encontrado lo que faltaba\n");
	}
	exit(ale);
}
