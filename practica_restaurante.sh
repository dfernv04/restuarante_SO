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
                2) echo -e "Has seleccionado compilar el programa\n"
			echo -e "Compilando...\n"
			echo -e "Compilacion correcta!!\n"
			gcc  -o res restaurante1.c;;
                3) echo -e "Has seleccionado ejecutar el programa\n"
			echo -e "Ejecutando...\n"
			if test -a res
			then
				echo -e "Escribe el numero de piches de cocina que desee \n"
				read n
				./res $n
				sleep 3
				echo -e "Ejecucion completada!!\n"
			else
				echo -e "Tienes que compilar antes de ejecutar!\n"
			fi;;
                4) echo -e "Has seleccionado salir\n"
			echo -e "Vuelva pronto. Muchas gracias!\n"
                        exit 0;;
                *) echo -e "ERROR! Seleccione una opcion del 1 al 4!\n"
        esac
done

