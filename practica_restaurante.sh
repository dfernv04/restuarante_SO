#!/bin/bash

while true
do
	echo -e "8888888b.  8888888888 .d8888b. 88888888888     d8888 888     888 8888888b.         d8888 888b    888 88888888888 8888888888"
	echo -e "888   Y88b 888       d88P  Y88b    888        d88888 888     888 888   Y88b       d88888 8888b   888     888     888"
	echo -e "888    888 888       Y88b.         888       d88P888 888     888 888    888      d88P888 88888b  888     888     888"
	echo -e "888   d88P 8888888    Y888b.       888      d88P 888 888     888 888   d88P     d88P 888 888Y88b 888     888     8888888"
	echo -e "8888888P   888           Y88b.     888     d88P  888 888     888 8888888P      d88P  888 888 Y88b888     888     888"
	echo -e "888 T88b   888             888     888    d88P   888 888     888 888 T88b     d88P   888 888  Y88888     888     888"
	echo -e "888  T88b  888       Y88b  d88P    888   d8888888888 Y88b. .d88P 888  T88b   d8888888888 888   Y8888     888     888"
	echo -e "888   T88b 8888888888  Y8888P      888  d88P     888   Y88888P   888   T88b d88P     888 888    Y888     888     8888888888"
	echo -e "\n\n"
	echo -e "################################\n"
        echo -e "PRACTICA INTERMEDIA RESTAURANTE\n"
        echo -e "[1]-Mostrar codigo del programa\n"
        echo -e "[2]-Compilar codigo del programa\n"
        echo -e "[3]-Ejecutar codigo del programa\n"
        echo -e "[4]-Salir\n"
	echo -e "################################\n"
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
			gcc  -o res restaurante.c
			clear;;
                3) echo -e "Has seleccionado ejecutar el programa\n"
			echo -e "Ejecutando...\n"
			if test -f res
			then
				if test -x res
				then
					echo -e "Escribe el numero de piches de cocina que desee \n"
					read n
					clear
					./res $n
					sleep 3
					echo -e "Ejecucion completada!!\n"	
				else
					echo -e "El ejecutable no tiene permisos de ejecucion!\n"
				fi
			else
				echo -e "Tienes que compilar antes de ejecutar!\n"
			fi;;
                4) echo -e "Has seleccionado salir\n"
			echo -e "Vuelva pronto. Muchas gracias!\n"
                        exit 0;;
                *) echo -e "ERROR! Seleccione una opcion del 1 al 4!\n"
        esac
done

