# Individual based model of Fox movements C++

The foxes disperse move and reproduce between zones with or without hunting
and the parameters change according to that.
All the 

## Descripcion de archivos de parametros

## Estructura de las estaciones del año asumidas en el modelo
1	Repro	Disp
2	Caza	Disp
3	Caza
4	Repro

## ARCHIVO de extension INP

Archivo de parametros del modelo y especies

10 10			Dimensiones del CA: X Y
100 1			Numero de Iteraciones y Numero de Repeticiones
2			Numero de especies y a continuacion los parms de las sp.
0 3 1  15 5 3 2  5 5  .02 .80 .90 .04 .50 .50  15 20  0 .00 .50 .00 .20 .20 .20
...

Parametros de Especies
1: Numero de especie comenzando desde 0 y en orden creciente

2: Numero de estadios (clases de edades)
	3
3: Numero de sitios (Estancias con o sin caza = 2 sitios)
	2

4: Distancia minima de Establecimiento, cuando un individuo se dispersa
   	3
   En el caso de dispersion exponencial es la distancia media de dispersion (Km)
	20

5: Distancia a la cual el individuo evalua si sigue moviendose o no en el
   proceso de dispersion
   	3

6: Distancia de evaluacion (hacia atras) de prescencia de individuos 
   en el camino recorrido
   	3

7: Cantidad de minima de celdas vacias para que se pueda establecer
	2

8: Distancia maxima de alejamiento de la madre 
	Ahora es el numero de intentos que hace un cachorro para encontrar un lugar vacio
	al dispersarse
	1

9: Edad Maxima 
	11 años o sea 44 (periodos de 3 months)

10 11 12: Tamaño de camada (Por estadio y sitio)
		0 3 5 0 3 5 

12 13 14: Probabilidad de supervivencia por edad y sitio
		.71 .81 .81 .90 1.0 1.0

15 16 17: Probabilidad de reproducion
		.00 .25 .25 .00 .25 .25

18 19 20: Distancia de dispersion Maxima
		60 60 60 60 60 60

21 22 23: Probabilidad de Dispersion inicial 
		.50 .37 .00 .67 .33 .00

	  (Era antes la P dispersion Maxima, 
		y disminuia con (1 - Dist.Recorrida / Dist.Maxima) )

24 25 26: Probabilidad de mortalidad por Caza
		.0 .12 .12 .70 .33 .33

## ARCHIVO de extension SED
Ejemplo de dimension 4x4, en SP esta la hubicacion de los individuos de
las especies ( en nuestro caso siempre 1), en AG esta la edad de los
individuos y en SI esta la distribucion de sitios.

         SP
         0 1 0 0
         0 1 0 0 
         0 0 0 1
         0 1 0 0 
         AG
         0 2  0 0
         0 12 0 0 
         0 0  0 5
         0 6  0 0
         SI
         0 0 1 1
         0 0 1 1
         1 1 0 0
         1 1 0 0 


## ARCHIVO de extension SET

Primero esta el nro de especie (comenzando desde cero pa'complicar la
cosa) para nosotros siempre 0. luego esta la edad, luego la cantidad y 
finalmente la posicion en el eje x a partir de la cual se ubican, si
es 0 se ubican al azar en todo el espacio.

          0  2 300 0
          0  6 100 0
          0 16 100 0

## EJECUCION del programa

        gcafox d150.inp d150e15.sed k.set s 1 s n d150e15 > d150e15.out
                 1        2          3    4 5 6 7 8         9

1: Archivo de parametros
2: Archivo de configuracion inicial sp,ag,si (estatico)
3: Archivo de configuracion inicial de sp (al azar)
4: Graficos? (S/N)
5: Cantidad de pasos para mostrar graficos y calcular densidad
6: Densidad? (S/N)
7: Salvar estado al final de la simulacion
8: Nombre de archivo a salvar de 8: (es opcional)
9: Archivo de salida de los datos de densidad 

Hay un archivo de ejemplo: simul.bat
los archivos D150e75.sed y D150e15.sed corresponden a estancias
con tamanios 75x75 y 15x15 en un tamanio total de 150 celdas.
Ergo 4 y 100 estancias respectivamente. 

## PARA HACER

Mejorar los graficos, mostrar en pantalla Tiempo y Estacion

Agregar tipo de sitio reflejante

Cambiar opcionalmente a bordes no continuos 

## License


	Copyright 2001 Leonardo A. Saravia
 
    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
 
