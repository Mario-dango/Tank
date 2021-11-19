#!/usr/bin/python 
# -*- coding: utf -8 -*-

########### ****Referencia
# Comandos que interpreta el pcb:
# A : Avanzar 
# R : Retroceder
# D : Girar derecha
# I : Girar Izquierda
#
# Ejemplo:
#   A100  -> 100% de velocidad hacía delante
#   R25 -> 25% de velocidad retrocediendo
#   D49 -> 49% de velocidad girando a la derecha
#

###### Clases particulares
from typing import Counter
from sensor_temp_class import sensor_temp
from motor_class import motor
from PCB_class import PCB
###############
import serial
import time
import os



#### Constantes previsorias, luego pedirlas a la PCB
tension = 12
modelo = 'Paso a paso'
autor = "Mario Papetti"
vers = "v1.0"
fecha = "17/12/1996"
flag = False
Counter = 0
 

#pcb = serial.Serial("/dev/ttyAMA0", 9600) #objeto Puerto para la comunicación UART
mensaje = []

#### MAIN CLASS TANK
class tank():
    
    def __init__(self, bat) -> None:
        self.cant_celdas = bat

    
    def avanzar():
        print("El tanque está avanzando")
        mensaje = "A:100"
        #pcb.write(mensaje.encode('utf-8'))
        return
                
    def retroceder():
        print("El tanque retrocede.")
        mensaje = "R:100"
        #pcb.write(mensaje.encode('utf-8'))
        return

    def girar_izquierad():
        print("El tanque está girando a la izquierda.")
        mensaje = "I:100"
        #pcb.write(mensaje.encode('utf-8'))
        return

    def girar_derecha():
        print("El tanque gira a la derecha.")
        mensaje = "D:100"
        #pcb.write(mensaje.encode('utf-8'))
        return
    
    
    ##creación de objetos claves del sistema
    sensor1 = sensor_temp(1)
    sensor2 = sensor_temp(2)
    #mother = PCB(vers, autor, fecha)
    motor_izq = motor(tension, modelo)
    motor_der = motor(tension, modelo)
    #####################################

    ### While para esperar comunicación con la placa
    while 1:
        #mensaje = pcb.readline()
        time.sleep(1)
        Counter = Counter+1
        if Counter == 10:
            print("No se ha logrado establecer comunicación con la placa")
            break
        elif mensaje != "OK":
            print ("Se estableció conección con la placa.")            
            break
    
    #### Instanciar objeto PCB con los datos recibidos de la placa controladora
    # La idea es encontrar datos ordenados de la siguiente forma:
    # "versión 1.0 \nCreador Mario Papetti \nFecha: 20/10/2021" 
    mensaje.readline()
    try:
        mensaje = mensaje.split("\n")
        mother = PCB(mensaje[0], mensaje[1], mensaje[2])
    except:
        None



def main():
    print ("Iniciando programa..\n")
    print ("Cargando datos de la placa controladora ...\n")
    




if __name__ == "__main__":
    try:
        main()
        
    except KeyboardInterrupt:

        None
        
        #if pcb != None:
#
         #   pcb.close()