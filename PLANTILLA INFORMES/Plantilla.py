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
X_SCREEN = 630 #315

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
        self.t1 = 0
        self.startCopy = False

        # String
        self.folderName = ''
        self.folderPath = ''
        self.subjectName = ''
        self.tempName = ''

        self.names = []

        # Numbers
        self.numberFolder = [5]
        self.day = [1]
        self.month = [10]
        self.year = [20]
        self.group = [3]
        self.tp = [1]

        # Frames
        self.frame = np.zeros((Y_SCREEN, X_SCREEN, 3), np.uint8)
        cv.namedWindow(WINDOW_NAME)#, cv.WINDOW_NORMAL)
        cvui.init(WINDOW_NAME)

    def onWork(self):
        self.frame[:] = (49, 52, 49)
        char = cv.waitKey(1)

        doOverw = False

        writeOnName = False
        writeOnSubject = False
        writeOnProf = False

        stringCopy = ''
        stringCopyColor = 0x10dca1

        while True:

            cursor = cvui.mouse(WINDOW_NAME)
            click = cvui.mouse(cvui.CLICK)

            writeOnName, self.folderName = self.textInput(5, 5, X_SCREEN/2 - 10, 40, "Folder Name:", char, self.folderName, cursor, click, writeOnName)

            writeOnSubject, self.subjectName = self.textInput(5 + X_SCREEN / 2, 5, X_SCREEN / 2 - 10, 40, "Subject Name:", char, self.subjectName, cursor, click, writeOnSubject)

            writeOnProf, self.tempName = self.textInput(5 + X_SCREEN / 2, 105, X_SCREEN / 2 - 10, 40, "Professors:", char, self.tempName, cursor, click, writeOnProf)
            cvui.rect(self.frame, 6 + X_SCREEN / 2, 155, X_SCREEN / 2 - 63, Y_SCREEN - 165, 0x8d9797, 0x3c4747)
            cvui.rect(self.frame, X_SCREEN - 52, 155, 47, Y_SCREEN - 165, 0x8d9797, 0x293939)
            if cvui.button(self.frame, X_SCREEN - 49, 157, "+") and not self.tempName == '' and len(self.names) < 4:
                self.names.append(self.tempName)
                self.tempName = ''
            if cvui.button(self.frame, X_SCREEN - 49, 196, "-") and not len(self.names) == 0:
                del self.names[-1]

            for i in range(len(self.names)):
                cvui.printf(self.frame, 10 + X_SCREEN / 2, 160 + 15*i, 0.4, 0xdd97fb, self.names[i])


            cvui.printf(self.frame, 5, 55, 0.4, 0xdd97fb, f'N Exercises:')
            cvui.counter(self.frame, 5, 70, self.numberFolder)
            if self.numberFolder[0] <= 0:
                cvui.rect(self.frame, 5 + 25, 72, 40, 17, 0x292929, 0x292929)
                self.numberFolder[0] = 1
                cvui.printf(self.frame, 5 + 41, 76, 0.4, 0x9C9C9C, '1')

            cvui.printf(self.frame, 5 + X_SCREEN / 6, 55, 0.4, 0xdd97fb, f'Day:')
            cvui.counter(self.frame, 5 + X_SCREEN / 6, 70, self.day)
            if self.day[0] <= 0:
                cvui.rect(self.frame, 5 + X_SCREEN / 6 + 25, 72, 40, 17, 0x292929, 0x292929)
                self.day[0] = 1
                cvui.printf(self.frame, 5 + X_SCREEN / 6 + 41, 76, 0.4, 0x9C9C9C, '1')
            elif self.day[0] >= 31:
                self.day[0] = 31

            cvui.printf(self.frame, 5 + X_SCREEN * 2 / 6, 55, 0.4, 0xdd97fb, f'Month:')
            cvui.counter(self.frame, 5 + X_SCREEN * 2 / 6, 70, self.month)
            if self.month[0] <= 0:
                cvui.rect(self.frame, 5 + X_SCREEN * 2 / 6 + 25, 72, 40, 17, 0x292929, 0x292929)
                self.month[0] = 1
                cvui.printf(self.frame, 5 + X_SCREEN * 2 / 6 + 41, 76, 0.4, 0x9C9C9C, '1')
            elif self.month[0] >= 12:
                self.month[0] = 12

            cvui.printf(self.frame, 5 + X_SCREEN * 3 / 6, 55, 0.4, 0xdd97fb, f'Year:')
            cvui.counter(self.frame, 5 + X_SCREEN * 3 / 6, 70, self.year)
            if self.year[0] <= 19:
                cvui.rect(self.frame, 5 + X_SCREEN * 3 / 6 + 25, 72, 40, 17, 0x292929, 0x292929)
                self.year[0] = 20
                cvui.printf(self.frame, 5 + X_SCREEN * 3 / 6 + 37, 76, 0.4, 0x9C9C9C, '20')
            elif self.year[0] >= 22:
                self.year[0] = 22

            cvui.printf(self.frame, 5 + X_SCREEN * 4 / 6, 55, 0.4, 0xdd97fb, f'N Group:')
            cvui.counter(self.frame, 5 + X_SCREEN * 4 / 6, 70, self.group)
            if self.group[0] <= 0:
                cvui.rect(self.frame, 5 + X_SCREEN * 4 / 6 + 25, 72, 40, 17, 0x292929, 0x292929)
                self.group[0] = 1
                cvui.printf(self.frame, 5 + X_SCREEN * 4 / 6 + 41, 76, 0.4, 0x9C9C9C, '1')
            elif self.group[0] >= 7:
                self.group[0] = 7

            cvui.printf(self.frame, 5 + X_SCREEN * 5 / 6, 55, 0.4, 0xdd97fb, f'N TP:')
            cvui.counter(self.frame, 5 + X_SCREEN * 5 / 6, 70, self.tp)
            if self.tp[0] <= 0:
                cvui.rect(self.frame, 5 + X_SCREEN * 5 / 6 + 25, 72, 40, 17, 0x292929, 0x292929)
                self.tp[0] = 1
                cvui.printf(self.frame, 5 + X_SCREEN * 5 / 6 + 41, 76, 0.4, 0x9C9C9C, '1')

            if cvui.button(self.frame, 5, 120, "Load Folder Path") and not doOverw:
                self.folderPath = self.getPath()

            cvui.window(self.frame, 5, 155, X_SCREEN/2 - 10, 40, "Folder Path Selected")
            if len(self.folderPath) < MAXCHAR + 3:
                cvui.printf(self.frame, 10, 180, 0.4, 0xdd97fb, self.folderPath)
            else:
                cvui.printf(self.frame, 10, 180, 0.4, 0xdd97fb, self.folderPath[0:MAXCHAR + 2] + '...')

            if (not self.folderPath == '') and (not self.folderName == '') and (not self.subjectName == ''):
                if cvui.button(self.frame, 5, 200, "Start Copy"):
                    self.startCopy = True
                    stringCopy = "Wait While Copying..."
                    stringCopyColor = 0xdc1076

            x = int(X_SCREEN / 4)
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

    def textInput(self, x, y, w, h, title, char, writeOn, cursor, click, writeOnBool):

        cvui.window(self.frame, x, y, w, h, title)
        # cvui.rect(self.frame, x + 1, y + 17, w - 2, h - 18, 0xf20c0c, 0xf20c0c)
        inBox = ((x + 1 <= cursor.x <= x + 1 + w - 2) and (y + 17 <= cursor.y <= y + 17 + h - 18))

        if (inBox and click) or writeOnBool:
            writeOnBool = True
            if len(writeOn) < MAXCHAR:

                validChar = (32 <= char <= 33 or 35 <= char <= 41 or 43 <= char <= 46 or 48 <= char <= 57 or
                             char == 59 or char == 61 or 64 <= char <= 91 or 93 <= char <= 123 or
                             125 <= char <= 127)
                if validChar:
                    writeOn = writeOn + chr(char)
                elif char == 8 and not len(writeOn) == 0:
                    writeOn = writeOn[:-1]
            elif 8 == char:
                writeOn = writeOn[:-1]

            self.t1 = self.darwBar(writeOn, self.t1, x + 5, y + 25)

        else:
            cvui.printf(self.frame, x + 5, y + 25, 0.4, 0xdd97fb, f'{writeOn}')

        if click and not inBox:
            writeOnBool = False

        return writeOnBool, writeOn

    def darwBar(self, text, t1, x, y):
        t2 = time.perf_counter()
        if t2 - t1 <= 1:
            cvui.printf(self.frame, x, y, 0.4, 0xdd97fb, f'{text}|')
            return t1
        elif 1 < t2 - t1 <= 2:
            cvui.printf(self.frame, x, y, 0.4, 0xdd97fb, f'{text}')
            return t1
        else:
            return t2

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

            graphicspath = ''

            for f in range(numberOfExs):
                n = str(f + 1)

                exFolderPath = finalPath + "/Ejercicio-" + n
                imgFolderPath = exFolderPath + "/ImagenesEjercicio" + n
                texName = "/Ejercicio-" + n + ".tex"

                graphicspath = graphicspath + '{../Ejercicio-' + n + '/}'

                os.mkdir(exFolderPath)              #Creo la carpeta del ejercicio
                os.mkdir(imgFolderPath)             #Creo la carpeta de imagenes del ejercicio

                shutil.copyfile(TEXPATH, exFolderPath + texName)            #Copio y renombro .tex
                shutil.copyfile(PENDPATH, imgFolderPath + "/pend.jpg")      #Copio img
                shutil.copyfile(PENDPATH, imgFolderPath + "/Circuits.tex")  # Copio img

            os.mkdir(finalPath + "/Informe")
            shutil.copyfile(INFOTEXRPATH, finalPath + "/Informe/Informe.tex")
            shutil.copyfile(INFOHEADRPATH, finalPath + "/Informe/Header.tex")
            shutil.copyfile(INFOCARPATH, finalPath + "/Informe/Caratula.tex")
            shutil.copyfile(INFOCARPATH, finalPath + "/Informe/Header-Circuits.tex")

            with open(finalPath + '/Informe/Header.tex', 'r') as file:
                data = file.readlines()

            data[37] = '\graphicspath{' + graphicspath + '}\n'
            if not self.subjectName == '':
                data[41] = '\lhead{' + self.subjectName + '}\n'


            with open(finalPath + '/Informe/Header.tex', 'w') as file:
                file.writelines(data)

            with open(finalPath + '/Informe/Caratula.tex', 'r') as file:
                data = file.readlines()

            data[8] = '{ \Huge \bfseries Trabajo práctico N$^{\circ}$' + str(self.tp[0]) + '}\\[0.4cm] \n'
            data[14] = '\emph{Grupo' + str(self.group[0]) + '}\ \ \n'
            data[38] = 'Presentado: & ' + str(self.day[0]) + '/' + str(self.month[0]) + '/' + str(self.year[0]) + '\\ \n'

            if not len(self.names) == 0:
                for i in range(len(self.names)):
                    data[27 + i] = self.names[i] + '\\ \n'

            with open(finalPath + '/Informe/Caratula.tex', 'w') as file:
                file.writelines(data)

            return True
        else:
            return None

    def overWrite(self, finalPath):

        self.frame[:] = (49, 52, 49)

        cvui.window(self.frame, 5, 5, X_SCREEN - 10, Y_SCREEN - 10, "Folder Alredy Exist")

        cvui.printf(self.frame, 10, 50, 0.4, 0xee1c1c, "Do you want to delete the original folder and overwrite it with the new copy? Everything in the")
        cvui.printf(self.frame, X_SCREEN/2 - 100, 70, 0.4, 0xee1c1c, "original folder will be deleted!!!!")

        if cvui.button(self.frame, X_SCREEN/2 - (14*10 + 10), 140, "Yes, Overwrite"):
            shutil.rmtree(finalPath)
            coping = self.copyFolders(self.numberFolder[0], finalPath, False)
            if coping == None:
                return None
            else:
                return True

        if cvui.button(self.frame, X_SCREEN/2 + 5, 140, "No Fuck! Go Back!"):
            return False

        return -1


def main():
    myGui = cvGui()
    myGui.onWork()

if __name__ == '__main__':
    main()

    # params = {"bins": self.MF.hist_filter.bins_opti,
    #           "mask_blur": self.MF.hist_filter.mask_blur_size_opti,
    #           "kernel_blur": self.MF.hist_filter.kernel_blur_size_opti,
    #           "low_pth": self.MF.hist_filter.low_pth_opti}
    # return params
    #
    # self.camShift_bins[0] = params["bins"]