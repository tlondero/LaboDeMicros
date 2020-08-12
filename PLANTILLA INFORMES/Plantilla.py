import tkinter as tk
from tkinter import filedialog

import shutil
import os

import cv2 as cv
import cvui
import numpy as np
import time

WINDOW_NAME = "Template Maker"
MAXCHAR = 39

Y_SCREEN = 235 #150
X_SCREEN = 315

TEXPATH = "./Ejercicio-N/Ejercicio-N.tex"
PENDPATH = "./Ejercicio-N/ImagenesEjercicioN/pend.jpg"
INFORPATH = "./Informe"
INFOTEXRPATH = "./Informe/Informe.tex"
INFOHEADRPATH = "./Informe/Header.tex"
INFOCARPATH = "./Informe/Caratula.tex"

class cvGui():

    def __init__(self, *args, **kw):
        self.timer = 0
        self.t0 = 0
        self.numberFolder = [5]
        self.startCopy = False

        # String
        self.folderName = ''
        self.folderPath = ''

        # Frames
        self.frame = np.zeros((Y_SCREEN, X_SCREEN, 3), np.uint8)
        cv.namedWindow(WINDOW_NAME)#, cv.WINDOW_NORMAL)
        cvui.init(WINDOW_NAME)

    def onWork(self):
        self.frame[:] = (49, 52, 49)
        char = cv.waitKey(1)
        t1 = 0
        doOverw = False

        stringCopy = ""
        stringCopyColor = 0x10dca1

        while True:

            cvui.window(self.frame, 5, 5, X_SCREEN - 10, 40, "Folder Name")

            if len(self.folderName) < MAXCHAR:
                if 32 == char or 48 <= char <= 57 or 65 <= char <= 90 or 97 <= char <= 122:
                    self.folderName = self.folderName + chr(char)
                elif char == 8 and not len(self.folderName) == 0:
                    self.folderName = self.folderName[:-1]
            elif 8 == char:
                self.folderName = self.folderName[:-1]

            t2 = time.perf_counter()
            if t2 - t1 <= 1:
                cvui.printf(self.frame, 10, 30, 0.4, 0xdd97fb, f'{self.folderName}|')
            elif 1 < t2 - t1 <= 2:
                cvui.printf(self.frame, 10, 30, 0.4, 0xdd97fb, f'{self.folderName}')
            else:
                t1 = t2

            cvui.printf(self.frame, 10, 55, 0.4, 0xdd97fb, f'Number Of Folders/Exercises:')
            cvui.trackbar(self.frame, 10, 70, X_SCREEN - 20, self.numberFolder, 1.0, 15.0, 1, "%1.0Lf", cvui.TRACKBAR_HIDE_SEGMENT_LABELS, 1)
            self.numberFolder[0] = int(self.numberFolder[0])

            if cvui.button(self.frame, 5, 120, "Folder Path") and not doOverw:
                self.folderPath = self.getPath()

            cvui.window(self.frame, 5, 155, X_SCREEN - 10, 40, "Folder Path Selected")
            if len(self.folderPath) < MAXCHAR + 3:
                cvui.printf(self.frame, 10, 180, 0.4, 0xdd97fb, self.folderPath)
            else:
                cvui.printf(self.frame, 10, 180, 0.4, 0xdd97fb, self.folderPath[0:MAXCHAR + 2] + '...')

            if (not self.folderPath == '') and (not self.folderName == ''):
                if cvui.button(self.frame, 5, 200, "Start Copy"):
                    self.startCopy = True
                    stringCopy = "Wait While Copying..."
                    stringCopyColor = 0xdc1076

            x = int(X_SCREEN / 2)
            cvui.printf(self.frame, x, 210, 0.4, stringCopyColor, stringCopy)

            if self.startCopy:
                self.startCopy = False
                finalPath = self.folderPath + '/' + self.folderName

                coping = self.copyFolders(self.numberFolder[0], finalPath, True)

                if not coping == None:
                    if not coping:
                        doOverw = True
                    else:
                        stringCopy = "Copy Done!"
                        stringCopyColor = 0x10dca1
                else:
                    stringCopy = "Template Not Found!!"
                    stringCopyColor = 0xdc1076

            if doOverw:
                check = self.overWrite(finalPath)
                if not check == -1:
                    if check == None:
                        stringCopy = "Template Not Found!!"
                        stringCopyColor = 0xdc1076
                    else:
                        if check == True:
                            stringCopy = "Copy Done!"
                            stringCopyColor = 0x10dca1
                        else:
                            stringCopy = "Unable To Copy"
                            stringCopyColor = 0xdc1076
                    doOverw = False

            cvui.imshow(WINDOW_NAME, self.frame)
            char = cv.waitKey(1)
            self.frame[:] = (49, 52, 49)

            if (char == 27) or not cv.getWindowProperty(WINDOW_NAME, cv.WND_PROP_VISIBLE):
                break

    def getPath(self):
        root = tk.Tk()
        root.withdraw()
        file_path = filedialog.askdirectory(initialdir="./", title="Select Folder Path")
        return file_path

    def copyFolders(self, numberOfExs, finalPath, checkExist):

        if checkExist and os.path.exists(finalPath):
            return False
        elif os.path.exists(TEXPATH) and os.path.exists(PENDPATH) and os.path.exists(INFORPATH) and os.path.exists(INFOTEXRPATH) and os.path.exists(INFOHEADRPATH) and os.path.exists(INFOCARPATH):
            os.mkdir(finalPath)

            os.mkdir(finalPath + "/Informe")
            shutil.copyfile(INFOTEXRPATH, finalPath + "/Informe/Informe.tex")
            shutil.copyfile(INFOHEADRPATH, finalPath + "/Informe/Header.tex")
            shutil.copyfile(INFOCARPATH, finalPath + "/Informe/Caratula.tex")

            for f in range(numberOfExs):
                n = str(f + 1)

                exFolderPath = finalPath + "/Ejercicio-" + n
                imgFolderPath = exFolderPath + "/ImagenesEjercicio" + n
                texName = "/Ejercicio-" + n + ".tex"

                os.mkdir(exFolderPath)              #Creo la carpeta del ejercicio
                os.mkdir(imgFolderPath)             #Creo la carpeta de imagenes del ejercicio

                shutil.copyfile(TEXPATH, exFolderPath + texName)            #Copio y renombro .tex
                shutil.copyfile(PENDPATH, imgFolderPath + "/pend.jpg")      #Copio img

            return True
        else:
            return None

    def overWrite(self, finalPath):

        self.frame[:] = (49, 52, 49)

        cvui.window(self.frame, 5, 5, X_SCREEN - 10, Y_SCREEN - 10, "Folder Alredy Exist")

        cvui.printf(self.frame, 10, 50, 0.4, 0xdd97fb, "Do you want to delete the original folder and")
        cvui.printf(self.frame, 10, 70, 0.4, 0xdd97fb, "overwrite it with the new copy?")
        cvui.printf(self.frame, 10, 90, 0.4, 0xdd97fb, "Everything in the original folder will be")
        cvui.printf(self.frame, 10, 110, 0.4, 0xdd97fb, "deleted!!!!")


        if cvui.button(self.frame, 10, 140, "Yes, Overwrite"):
            shutil.rmtree(finalPath)
            coping = self.copyFolders(self.numberFolder[0], finalPath, False)
            if coping == None:
                return None
            else:
                return True

        if cvui.button(self.frame, 155, 140, "No Fuck! Go Back!"):
            return False

        return -1


def main():
    myGui = cvGui()
    myGui.onWork()

if __name__ == '__main__':
    main()