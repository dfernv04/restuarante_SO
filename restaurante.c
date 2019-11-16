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
			printf("Error, el numero de pinches ha de ser mayor que 0\n");;
			exit(-1);
		}else{
			//si los argumentos son correctos, llega aqui y comienza la ejecucion normal
			//el numero total de trabajadores va a ser el numero de pinches + el somelier + el jefe de sala
			int numtrabajadores = num + 1 +1;
			int i,j,k=0,status, estado, listo, listo2, contador_platos=0;
			pid_t somelier, jefe_sala, *pinche;
			pinche = (pid_t*)malloc(sizeof(pid_t)*num);
			//vamos a crear las estructuras necesarias
			struct sigaction som = {0};
			struct sigaction jefe = {0};
			struct sigaction pinches = {0};
			//creamos el puntero donde vamos a almacenar los pids de los trabajadores
			pid_t *trabajadores, pid;
			trabajadores = (pid_t*)malloc(sizeof(pid_t)*numtrabajadores);
			//vamos a crear a los hijos del chef
			for(i=0;i<numtrabajadores;i++){
				//creamos el proceso hijo
				pid=fork();
				//almacenamos el proceso hijo
				trabajadores[i]=pid;
				if(pid==-1){
					perror("Error al crear el proceso hijo");
				}else{
					if(pid==0){
						switch(i){
							//esta en el codigo del somelier
							case 0:
								printf("Soy el somelier con pid: %d, y mi padre es el chef con pid: %d\n", getpid(), getppid());
								som.sa_handler=manejadora_som;
								if(-1==sigaction(SIGUSR1, &som, NULL)){
									perror("SOMELIER: sigaction");
									return 1;
								}
								som.sa_handler=manejadora_som;								
								if(-1==sigaction(SIGUSR2, &som, NULL)){
									perror("SOMELIER: sigaction");
									return 1;
								}
								pause();
								break;
							//estamos en el codigo del jefe de sala
							case 1:
								printf("Soy el jefe de sala con pid: %d, y mi padre es el chef con pid: %d\n", getpid(), getppid());
								jefe.sa_handler=manejadora_jefesala;
								if(-1==sigaction(SIGUSR1, &jefe, NULL)){
									perror("JEFE DE SALA: sigaction");
									return 1;
								}
								pause();
								break;
							//estamos en el codigo de los pinches
							default:
								printf("Soy el pinche %d con pid: %d, y mi padre es el chef con pid: %d\n", (i-1), getpid(), getppid());
								pinches.sa_handler=manejadora_pinches;
								if(-1==sigaction(SIGUSR1, &pinches, NULL)){
									perror("PINCHES: sigaction");
									return 1;
								}
								pause();
						}
					}else{
						somelier = trabajadores[0];
						jefe_sala = trabajadores[1];
						for(j=2;j<numtrabajadores;j++){
							pinche[k] = trabajadores[j];
							k++;
						}
					}
				}
			}
			//enviamos la señal SIGUSR1 al somelier para que cree al mozo de los recados
			kill(somelier, SIGUSR1);
			//le decimos al chef que duerma 3 segundos
			sleep(3);
			//genera un aleatorio para ver si falta vino o ingredientes
			int num_ale = calculaAleatorios(0,1);
			if(num_ale==0){
				printf("Faltan ingredientes\n");
				kill(somelier,SIGUSR1);
			}else{
				printf("Falta vino\n");
				kill(somelier, SIGUSR2);
			}
			listo=wait(&status);
			listo=WEXITSTATUS(status);
			if(listo==1){
				printf("RESTAURANTE CERRADO\n");
				for(k=0;k<numtrabajadores;k++){
					kill(trabajadores[k], SIGKILL);
				}
			}else{
				if(listo==2){
					printf("Faltan algunos ingredientes\n");
				}
				for(k=0;k<num;k++){
					kill(pinche[k], SIGUSR1);
					listo2=wait(&estado);
					listo2=WEXITSTATUS(estado);
					if(listo2==1){
						contador_platos++;
					}
				}
				if(contador_platos==0){
					printf("CERRANDO RESTAURANTE...\n");
					for(k=0;k<numtrabajadores;k++){
						kill(trabajadores[k], SIGKILL);
					}
				}else{
					printf("El numero de platos preparados por los pinches es de %d\n", contador_platos);
					kill(jefe_sala,SIGUSR1);
					printf("PUEDE ABRIRSE EL RESTAURANTE\n");
					for(k=0;k<numtrabajadores;k++){
						kill(trabajadores[k], SIGKILL);
					}
				}
			}
		}
	}

	return 0;
}

int calculaAleatorios(int min, int max){
	return rand() % (max-min+1) + min;
}

void manejadora_som(int sig){
	srand(time(NULL));
	int status, devolver, esperamos;
	pid_t mozo;
	struct sigaction mozos = {0};
	mozo = fork();
	if(mozo==-1){
		perror("ERROR al crear el proceso hijo");
	}else{
		if(mozo==0){
			printf("Soy el mozo de los recados con pid: %d, y mi padre es el somelier con pid: %d\n", getpid(), getppid());
			mozos.sa_handler=manejadora_mozo;
			if(-1==sigaction(SIGPIPE, &mozos, NULL)){
				perror("MOZO: sigaction");
				exit(-1);
			}
			pause();
		}else{
			kill(mozo, SIGPIPE);
			//esperamos el valor que devuelva el mozo
			esperamos = wait(&status);
			esperamos = WEXITSTATUS(status);
			if(esperamos==0){
				printf("El mozo no encontro lo que buscaba\n");
			}else{
				printf("El mozo encontro lo que buscaba\n");
			}
			//este valor devuelto por el mozo combinado con la señal que mando el chef al somelier nos 
			//indica si mozo encontro el vino, los ingredientes o nada
			if(esperamos==1){
				devolver=3;
			}else{
				if(sig==12){
					devolver=1;
				}else{
					devolver=2;
				}
			}
		}
	}
	exit(devolver);
}

void manejadora_jefesala(int sig){
	printf("Estoy en la manejadora del jefe de sala\n");
	sleep(3);
	printf("El jefe de sala ha terminado de montar las mesas\n");
}

void manejadora_pinches(int sig){
	srand(time(NULL));
	printf("Estoy en la majedora de los pinches\n");
	//generamos el numero aleatorio que los pinches van a dormir
	int num = calculaAleatorios(2,5);
	sleep(num);
	//generamos un numero aleatorio que nos indica si los pinches 
	//han cocinado bien los platos(0->mal && 1->bien)
	int num2 = calculaAleatorios(0,1);
	//devolvemos dicho numero chef
	exit(num2);
}

void manejadora_mozo(int sig){
	srand(time(NULL));
	printf("Estoy en la manejadora del mozo\n");
	//calculamos un aleatorio para saber si encontro lo que buscaba
	int num = calculaAleatorios(0,1);
	//devolvemos el numero al somelier
	exit(num);
}