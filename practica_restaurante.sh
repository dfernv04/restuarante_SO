#!/bin/bash

while true
do
        echo -e "PRACTICA INTERMEDIA RESTAURANTE\n"
        echo -e "[1]-Mostrar codigo del programa\n"
        echo -e "[2]-Compilar codigo del programa\n"
        echo -e "[3]-Ejecutar codigo del programa\n"
        echo -e "[4]-Salir\n"
        read input
        case $input in
                1) echo -e "Has seleccionado mostrar codigo del programa\n"
			echo -e "Mostrando codigo...\n"
			cat restaurante.c
			echo -e ""
			echo -e "Codigo mostrado con exito!\n";;
                2) echo -e "Has seleccionado compilar codigo del programa\n"
			echo -e "Compilando...\n"
			echo -e "Compilacion correcta!!\n"
			gcc  -o res restaurante.c;;
                3) echo -e "Has seleccionado ejecutar codigo del programa\n"
			echo -e "Ejecutando...\n"
			echo -e "Escribe el numero de piches de cocina que desee \n"
			read n
			./res $n
			sleep 5
			echo -e "Ejecucion completada!\n";;
                4) echo -e "Has seleccionado salir\n"
			echo -e "Vuelva pronto. Muchas gracias!\n"
                        exit 0;;
                *) echo -e "ERROR! Seleccione una opcion del 1 al 4!\n"
        esac
done
