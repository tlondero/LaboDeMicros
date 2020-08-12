#import tkinter as tk
#from tkinter.ttk import *
#from PIL import Image, ImageTk
#import glob
import shutil
#from shutil import * #copyfile
import os

i = 5

#tex = glob.glob("./Ejercicio-N/Ejercicio-N.tex", recursive=True)
#folder = glob.glob("./Ejercicio-N/ImagenesEjercicioN/pend.jpg", recursive=True)
for f in range(i):
    n = str(f + 1)
    renameTex = "./Ejercicio-" + n + "/Ejercicio-N.tex"

    os.mkdir("./Ejercicio-" + n)                                        #Creo la carpeta del ejercicio
    os.mkdir("./Ejercicio-" + n + "/ImagenesEjercicio" + n)             #Creo la carpeta de imagenes del ejercicio

    shutil.copyfile("./Ejercicio-N/Ejercicio-N.tex", "./Ejercicio-" + n + "/Ejercicio-" + n + ".tex")          #Copio y renombro .tex
    shutil.copyfile("./Ejercicio-N/ImagenesEjercicioN/pend.jpg", "./Ejercicio-" + n + "/ImagenesEjercicio" + n + "/pend.jpg")         #Copio img


shutil.rmtree("./Ejercicio-N/")
