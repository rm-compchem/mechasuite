#!/usr/bin/env python
import numpy as np
import math
import sys

class Poscar:
    def __init__(self):
        self.label = []
        self.label_extended = []
        self.zipped = []
        self.celda = ""
        self.atomos = ""
        self.coor = ""
        self.totalat = 0
        self.l = ""
        self.xyz = []
        self.xyzlabel = []
        self.xyztotalat = ""
        self.a = ""
        self.b = ""
        self.c = ""
        self.sen_alpha = ""
        self.sen_beta = ""
        self.sen_gamma = ""
        self.cos_alpha = ""
        self.cos_beta = ""
        self.cos_gamma = ""
        self.v = ""
        self.lf = 0    # lineas del archivo
        self.dyn = []
        self.coortype = "Direct"
        self.vapcomment = ""
        self.vasp_old = True

    def load_poscar(self, poscar):
        # este modulo carga los datos de un archivo POSCAR e inicializa
        # las variables: labels, coor, celda, atomos, totalat"""
        with open(poscar) as f:
            self.l = f.readlines()
        self.lf = len(self.l)
        for i in range(len(self.l)):
            self.l[i] = self.l[i].split()

        self.vaspcomment = self.l[0]
        self.celda = self.l[2:5]
        self.celda = np.array(self.celda, dtype=float)

        if self.l[6][0].startswith('S') or self.l[6][0].startswith("s"):
            self.vasp_old = True
            self.label = self.l[0]
            self.atomos = self.l[5]
            if self.l[7][0].startswith("D") or self.l[7][0].startswith("d"):
                self.coortype = "Direct"
            else:
                self.coortype = "Cartesian"
            readcoorindex = 8
        elif self.l[6][0].startswith('D') or self.l[6][0].startswith("d"):
            self.label = self.l[0]
            self.vasp_old = True
            self.atomos = self.l[5]
            self.coortype = "Direct"
            readcoorindex = 7
        elif self.l[6][0].startswith('C') or self.l[6][0].startswith("c"):
            self.vasp_old = True
            self.label = self.l[0]
            self.atomos = self.l[5]
            self.coortype = "Cartesian"
            readcoorindex = 7
        else:
            self.vasp_old = False
            self.atomos = self.l[6]
            self.label = self.l[5]
            if self.l[7][0][0] in ["c", "C"]:
                self.coortype  = "Cartesian"
                readcoorindex = 8
            elif self.l[7][0][0] in ["D", "d"]:
                self.coortype = "Direct"
                readcoorindex = 8
            elif self.l[7][0][0] in ["s", "S"]:
                if self.l[8][0][0] in ["c", "C"]:
                    self.coortype = "Cartesian"
                elif self.l[8][0][0] in ["D", "d"]:
                    self.coortype = "Direct"
                readcoorindex = 9

        if len(self.atomos) > 0:
            self.atomos = np.array(self.atomos, dtype=int)
            self.atomos = list(self.atomos)
        else: print ("There are no atoms on line 6 of the POSCAR file")
        # calculos el total de atomos
        for i in self.atomos: self.totalat = self.totalat + i        
        self.coor = self.l[readcoorindex:readcoorindex + self.totalat]
        self.coor_to_float()
        self.ll = self.l[:readcoorindex]

        if len(self.coor[0]) == 6:
            for i in range(self.totalat):
                self.dyn.append([])
                for j in range(3,6):
                    if self.coor[i][j] == "F" or self.coor[i][j] == "T":
                        self.dyn[i].append(self.coor[i][j])
                    else:
                        print ("=========================================")
                        print ("CAUTION !!!!!")
                        print ("   bad format for sel dynamics in POSCAR")
                        print ("=========================================")
                del self.coor[i][3:] 
        else:
            print ("Sel dynamics flags not found")

        # calculo los coses y los senos de los angulos de la celda
        # unidad con los que se hara la matrix de transformacion
        self.sen_alpha, self.cos_alpha = trig(self.celda[1], self.celda[2], result="sin,cos")
        self.sen_beta, self.cos_beta   = trig(self.celda[0],self.celda[2], result="sin,cos")
        self.sen_gamma, self.cos_gamma = trig(self.celda[0],self.celda[1], result="sin,cos")
        # este v es necesario para la matrix de transformacion
        self.v = math.sqrt( 1 - self.cos_alpha**2 - self.cos_beta**2 - 
        self.cos_gamma**2 + 2 * self.cos_alpha * self.cos_beta * self.cos_gamma )
        
        self.a = np.linalg.norm(self.celda[0])
        self.b = np.linalg.norm(self.celda[1])
        self.c = np.linalg.norm(self.celda[2])

    def set_cell(self, celda):
        self.celda = np.array(celda, dtype=float)
        #calculo los coses y los senos de los angulos de la celda 
        #unidad con los que se hara la matrix de transformacion
        self.sen_alpha, self.cos_alpha = trig(self.celda[1], self.celda[2], result="sin,cos")
        self.sen_beta, self.cos_beta   = trig(self.celda[0],self.celda[2], result="sin,cos")
        self.sen_gamma, self.cos_gamma = trig(self.celda[0],self.celda[1], result="sin,cos")
        #este v es necesario para la matrix de transformacion
        self.v = math.sqrt( 1 - self.cos_alpha**2 - self.cos_beta**2 -
        self.cos_gamma**2 + 2 * self.cos_alpha * self.cos_beta * self.cos_gamma )

        self.a = np.linalg.norm(self.celda[0])
        self.b = np.linalg.norm(self.celda[1])
        self.c = np.linalg.norm(self.celda[2])


    def coor_to_float(self):
        #esta funcion convierte en flotante la lista 
        #que contiene las coordenadas del POSCAR
        #la lista ya debe estar dividida osea debe 
        #ser nx3 n filas y 3 columnas de strings

        for i in range(len(self.coor)):
            try:
                for j in range(3):
                    self.coor[i][j] = float(self.coor[i][j])
            except:
                print (self.coor[i])
                print ("The coordinates of atom "+str(i+1),)
                print ("could not be converted to float")
                sys.exit()

    def xyz_to_float(self):
        #esta funcion convierte en flotante la lista 
        #que contiene las coordenadas del POSCAR
        #la lista ya debe estar dividida osea debe 
        #ser nx3 n filas y 3 columnas de strings

        for i in range(len(self.xyz)):
            try:
                for j in range(3):
                    self.xyz[i][j] = float(self.xyz[i][j])
            except:
                print(self.xyz[i])
                print ("The coordinates of atom "+str(i+1),)
                print ("could not be converted to float")
                sys.exit()

    def load_xyz(self, archivo):
        with open(archivo) as f:
            l = f.readlines()
        for i in range(len(l)): l[i] = l[i].split()
        self.xyztotalat = int(l[0][0])
        for i in range(2,2 + self.xyztotalat):
            self.xyzlabel.append(l[i][0])
            self.xyz.append([l[i][1], l[i][2], l[i][3]])
        self.xyz_to_float()

    def frac_cart(self, coordenadas):
        # este metodo convierte de fraccionarias a cartesinas
        # usando los paremetros de la celda del objeto
        xyz = []
        matrix = np.array([[self.a ,self.b * self.cos_gamma, self.c * self.cos_beta ],
                    [0, self.b * self.sen_gamma, 
            self.c * (self.cos_alpha - self.cos_beta * self.cos_gamma)/(self.sen_gamma)],
                    [0,0, self.c*self.v/self.sen_gamma ]])
        coordenadas = np.array(coordenadas)
        if coordenadas.ndim == 1:
            coordenadas = coordenadas.reshape(1,3)    
            for i in range(len(coordenadas)):
                xyz.append(list(np.dot(matrix, coordenadas[i])))
                xyz = np.asarray(xyz)
            xyz = xyz.reshape(3,)
        else:
            for i in range(len(coordenadas)):
                xyz.append(list(np.dot(matrix, coordenadas[i])))
                
        #xyz = np.asarray(xyz)
        return xyz


    def cart_frac(self, coordenadas):
        # este metodo convierte de cartesianas a fraccionarias 
        # usando los paremetros de la celda del objeto
        xyz = []
        matrix = np.array([[1/self.a  , -self.cos_gamma/(self.a*self.sen_gamma) ,
         (self.cos_alpha * self.cos_gamma - self.cos_beta)/(self.a*self.v*self.sen_gamma)],
                    [0 , 1/(self.b * self.sen_gamma) ,
         (self.cos_beta * self.cos_gamma - self.cos_alpha)/(self.b*self.v*self.sen_gamma)],
                    [0 , 0 , self.sen_gamma/(self.c*self.v)]])
        coordenadas = np.array(coordenadas)
        if coordenadas.ndim == 1:
            coordenadas = coordenadas.reshape(1,3)    
            for i in range(len(coordenadas)):
                xyz.append(np.dot(matrix, coordenadas[i]))
                xyz = np.asarray(xyz)
            xyz = xyz.reshape(3,)
        else:
            for i in range(len(coordenadas)):
                xyz.append(np.dot(matrix, coordenadas[i]))
            
        #xyz = np.asarray(xyz)
        return xyz

    def check_dyn(self, typecoor):
        def gen():
            for i in self.dyn:
                for j in i:
                    yield j

        CAUTION_1 = '''
        ========================================
        =          CAUTION !!!!                =
        = Length of Selective dynamics flags   =
        = do not match the number of atoms     =
        = Selective dynamics flags not written =
        ========================================'''
        CAUTION_2 = '''
        ==========================================
        =          CAUTION !!!!                  =
        = Not valid flags for Selective dynamics =
	=========================================='''
        cautionmsg = None
        checked        = True
        dynflags = ["T", "F"]
        if typecoor == "xyz":
            if len(self.dyn)==len(self.xyz):
                allseldyn = gen()
                for i in allseldyn:
                    if i not in dynflags:
                        cautionmsg = CAUTION_2
                        checked = False
            else:
                cautionmsg = CAUTION_1  
                checked = False
        elif typecoor == "coor":
            if len(self.dyn)==len(self.coor):
                allseldyn = gen()
                for i in allseldyn:
                    if i not in dynflags:
                        cautionmsg = CAUTION_2
                        checked = False
            else:
                cautionmsg = CAUTION_1  
                checked = False
        return cautionmsg, checked

    def poscar_out(self, archivo):
        celda_format = " {0[0]: > 12.8f} {0[1]: > 12.8f} {0[2]: > 12.8f} \n" 
        coor_format = " {0[0]: .15f} {0[1]: .15f} {0[2]: .15f}" 
        seldyn_format = "   {0[0]}   {0[0]}   {0[0]} \n"
        f = open(archivo + '.tra','w')

        f.write(" " + "   ".join(self.label))

        f.write("\n   1.0 \n")
    
        for i in self.celda:
            f.write(celda_format.format(i))
            
        for i in self.atomos:
            f.write(" " + str(i))
        f.write(" \n")

        if self.coortype == "Cartesian":
            cautionmsg, checked = self.check_dyn("xyz")
            if checked:
                f.write("Selective dynamics\n")
                f.write("Cartesian\n")
                for xyz, dyn in zip(self.xyz,self.dyn):
                    f.write(coor_format.format(xyz))
                    f.write(seldyn_format.format(dyn))
            else:
                print (cautionmsg)
                f.write("Cartesian\n")
                for xyz in self.xyz:
                    f.write(coor_format.format(xyz))
                    f.write(" \n")
        else:
            cautionmsg, checked = self.check_dyn("coor")
            if checked:
                f.write("Selective dynamics\n")
                f.write("Direct\n")
                for coor, dyn in zip(self.coor,self.dyn):
                    f.write(coor_format.format(coor))
                    f.write(seldyn_format.format(dyn))
            else:
                print (cautionmsg)
                f.write("Direct\n")
                for coor in self.coor:
                    f.write(coor_format.format(coor))
                    f.write(" \n")
        f.close()

    def xyz_out(self, archivo):
        # este metodo escribe en formato xyz las coodenadas
        # cartesianas de la molecula

        str_format = " {0[0]: > 10f} {0[1]: > 10f} {0[2]: > 10f} \n"
        archivo = archivo.lower()
        f = open(archivo + '.xyz', 'w')
        f.write(str(len(self.xyz)) + '\n\n')

        for xyz, label in zip(self.xyz, self.xyzlabel):
            f.write("{0: <3}".format(label))
            f.write(str_format.format(xyz))
        f.close()

    def ord_z(self):
        # this function arranges the coordinates in increasing order of
        # of the z coordinates
        # the label_extended can be ordered using this function 
        z = []
        coor_ordenadas = []
        last = len(self.coor[0]) - 1
        for i in range(len(self.coor)):
            z.append(self.coor[i][last])
        z.sort()
    
        for i in z:
            for j in range(len(self.coor)):
                if i == self.coor[j][last]:
                    coor_ordenadas.append(self.coor[j])
                    self.coor.pop(j)
                    break
        self.coor = coor_ordenadas[:]
        coor_ordenadas = []

    def zip_coorlabel(self):
        # returns a list in which self.coor and self.label_extended have been join
        # every label atom correspond to the same coordinates of the atom
        # the value is stored in self.zipped
        self.zipped = []
        if len(self.coor) == len(self.label_extended):
            for i in range(len(self.coor)):
                self.zipped.append([self.label_extended[i]] + list(self.coor[i]))

        else:
            print ("There are more labels than atoms")
            print ("labels_extended ", len(self.label_extended) )
            print ("coor ", len(self.coor))



    def cleave_zipped(self):
        #the variable self.zipped is cleaved and the labels are stored in
        # self.label_extended and the coordinates in self.coor. the latter 
        # two are reset
        del self.coor[:]
        del self.label_extended[:]
        for i in range(len(self.zipped)):
            self.coor.append(self.zipped[i][1:4])
            self.label_extended.append(self.zipped[i][0])

    def ord_zipped(self):
        # orders self.zipped according to the labels, that is,
        # if zipped is [ ["Ni",2],["Co",1], ["Ni", 0.5] ], the 
        # result will be [ ["Ni",2],["Ni", 0.5], ["Co",1]]
        llabels = []
        lista_ord = []
        for i in self.zipped:
            if i[0] not in llabels:
                llabels.append(i[0])
        for i in llabels:
            for j in self.zipped:
                if j[0] == i:
                    lista_ord.append(j)
        del self.zipped[:]
        self.zipped.extend(lista_ord)

    def extend_label(self):
        del self.label_extended[:]
        for i in range(len(self.atomos)):
            for j in range(self.atomos[i]):
                self.label_extended.append(self.label[i])

    def extend_at(self):
        self.label_extended.append("xxxx")
        del self.atomos[:]
        cont =  0
        lprev = self.label_extended[0]
        for i in range(len(self.label_extended)):
            if self.label_extended[i] == lprev:
                cont = cont + 1
            else:
                self.atomos.append(cont)
                cont = 1
                lprev = self.label_extended[i] 
        self.label_extended.remove("xxxx")
	

    def shrink_label(self):
        #esta funcion toma una label extendendida y la recoge
        # ejemplo Ni Ni Ni H H   la convierte en Ni H
        # lee listas
        del self.label[:]
        for i in self.label_extended:
            if i not in self.label:
                self.label.append(i)
	

    def exp_coor(self, *args):
        if len(args) == 1:
            expander = args[0]
            for i in range(3):
                num = expander[i]
                filas = len(self.coor)
                for j in range(1,num):
                    for k in range(filas):
                        bloque = self.coor[k][:]
                        bloque[i] = bloque[i] + j
                        self.coor.append(bloque)
        
        else:
            expander = args[0]
            for i in range(3):
                num = expander[i]
                filas = len(self.coor)
                for j in range(1,num):
                    for k in range(filas):
                        bloque = self.coor[k][:]
                        bloque[i] = bloque[i] + j
                        self.coor.append(bloque)
                        self.label_extended.append(self.label_extended[k])

    def exp_coor_neg(self, *args):

        if len(args) == 0:
            for i in range(len(self.coor)):
                if self.coor[i][0] != 0 and self.coor[i][1] != 0 and self.coor[i][2] != 0:
                    self.coor.append([-self.coor[i][0], -self.coor[i][1], -self.coor[i][2]])
                    self.coor.append([-self.coor[i][0], -self.coor[i][1], self.coor[i][2]])
                    self.coor.append([-self.coor[i][0], self.coor[i][1], -self.coor[i][2]])
                    self.coor.append([self.coor[i][0], -self.coor[i][1], -self.coor[i][2]])
                    self.coor.append([-self.coor[i][0], self.coor[i][1], self.coor[i][2]])
                    self.coor.append([self.coor[i][0], -self.coor[i][1], self.coor[i][2]])
                    self.coor.append([self.coor[i][0], self.coor[i][1], -self.coor[i][2]])

                elif self.coor[i][0] != 0 and self.coor[i][1] != 0:
                    self.coor.append([-self.coor[i][0], -self.coor[i][1], self.coor[i][2]])
                    self.coor.append([-self.coor[i][0], self.coor[i][1], self.coor[i][2]])
                    self.coor.append([self.coor[i][0], -self.coor[i][1], self.coor[i][2]])
                elif self.coor[i][0] != 0 and self.coor[i][2] != 0:
                    self.coor.append([-self.coor[i][0], self.coor[i][1], -self.coor[i][2]])
                    self.coor.append([-self.coor[i][0], self.coor[i][1], self.coor[i][2]])
                    self.coor.append([self.coor[i][0], self.coor[i][1], -self.coor[i][2]])
                elif self.coor[i][1] != 0 and self.coor[i][2] != 0:
                    self.coor.append([self.coor[i][0], -self.coor[i][1], -self.coor[i][2]])
                    self.coor.append([self.coor[i][0], -self.coor[i][1], self.coor[i][2]])
                    self.coor.append([self.coor[i][0], self.coor[i][1], -self.coor[i][2]])
    
                elif self.coor[i][0] != 0:
                    self.coor.append([-self.coor[i][0], self.coor[i][1], self.coor[i][2]])
                elif self.coor[i][1] != 0:
                    self.coor.append([self.coor[i][0], -self.coor[i][1], self.coor[i][2]])
                elif self.coor[i][2] != 0:
                    self.coor.append([self.coor[i][0], self.coor[i][1], -self.coor[i][2]])
        else:
            for i in range(len(self.coor)):
                if self.coor[i][0] != 0 and self.coor[i][1] != 0 and self.coor[i][2] != 0:
                    self.coor.append([-self.coor[i][0], -self.coor[i][1], -self.coor[i][2]])
                    self.coor.append([-self.coor[i][0], -self.coor[i][1], self.coor[i][2]])
                    self.coor.append([-self.coor[i][0], self.coor[i][1], -self.coor[i][2]])
                    self.coor.append([self.coor[i][0], -self.coor[i][1], -self.coor[i][2]])
                    self.coor.append([-self.coor[i][0], self.coor[i][1], self.coor[i][2]])
                    self.coor.append([self.coor[i][0], -self.coor[i][1], self.coor[i][2]])
                    self.coor.append([self.coor[i][0], self.coor[i][1], -self.coor[i][2]])

                    for j in range(7):
                        self.label_extended.append(self.label_extended[i])    

                elif self.coor[i][0] != 0 and self.coor[i][1] != 0:
                    self.coor.append([-self.coor[i][0], -self.coor[i][1], self.coor[i][2]])
                    self.coor.append([-self.coor[i][0], self.coor[i][1], self.coor[i][2]])
                    self.coor.append([self.coor[i][0], -self.coor[i][1], self.coor[i][2]])
 
                    for j in range(3):
                        self.label_extended.append(self.label_extended[i])    

                elif self.coor[i][0] != 0 and self.coor[i][2] != 0:
                    self.coor.append([-self.coor[i][0], self.coor[i][1], -self.coor[i][2]])
                    self.coor.append([-self.coor[i][0], self.coor[i][1], self.coor[i][2]])
                    self.coor.append([self.coor[i][0], self.coor[i][1], -self.coor[i][2]])

                    for j in range(3):
                        self.label_extended.append(self.label_extended[i])    

                elif self.coor[i][1] != 0 and self.coor[i][2] != 0:
                    self.coor.append([self.coor[i][0], -self.coor[i][1], -self.coor[i][2]])
                    self.coor.append([self.coor[i][0], -self.coor[i][1], self.coor[i][2]])
                    self.coor.append([self.coor[i][0], self.coor[i][1], -self.coor[i][2]])

                    for j in range(3):
                        self.label_extended.append(self.label_extended[i])    
    
                elif self.coor[i][0] != 0:
                    self.coor.append([-self.coor[i][0], self.coor[i][1], self.coor[i][2]])
                    self.label_extended.append(self.label_extended[i])    

                elif self.coor[i][1] != 0:
                    self.coor.append([self.coor[i][0], -self.coor[i][1], self.coor[i][2]])
                    self.label_extended.append(self.label_extended[i])    

                elif self.coor[i][2] != 0:
                    self.coor.append([self.coor[i][0], self.coor[i][1], -self.coor[i][2]])
                    self.label_extended.append(self.label_extended[i])    


def cart_frac0(coordenadas, celda):
    # esta funcion convierte de cartesianas a fraccionarios
    # pero esta ligado a ningun objecto, por eso se le pasa
    # la celda como parametro
    xyz = []
    sen_alpha, cos_alpha = trig(celda[1], celda[2], result="sin,cos")
    sen_beta, cos_beta   = trig(celda[0],celda[2], result="sin,cos")
    sen_gamma, cos_gamma = trig(celda[0],celda[1], result="sin,cos")
    # este v es necesario para la matrix de transformacion
    v = math.sqrt( 1 - cos_alpha**2 - cos_beta**2 - cos_gamma**2 + 2 * cos_alpha * cos_beta * cos_gamma )

    a = np.linalg.norm(celda[0])
    b = np.linalg.norm(celda[1])
    c = np.linalg.norm(celda[2])

    matrix = np.array([[1/a  , -cos_gamma/(a*sen_gamma) , 
        (cos_alpha * cos_gamma - cos_beta)/(a*v*sen_gamma) ],
        [0 , 1/(b * sen_gamma) , (cos_beta * cos_gamma
        - cos_alpha)/(b*v*sen_gamma) ],
        [0 , 0 , sen_gamma/(c*v)]])
    coordenadas = np.array(coordenadas)
    if coordenadas.ndim == 1:
        coordenadas = coordenadas.reshape(1,3)
        for i in range(len(coordenadas)):
            xyz.append(np.dot(matrix, coordenadas[i]))
            xyz = np.asarray(xyz)
        xyz = xyz.reshape(3,)
        xyz = list(xyz)
    else:
        for i in range(len(coordenadas)):
            xyz.append(np.dot(matrix, coordenadas[i]))
        # xyz = np.asarray(xyz)

    return xyz


def frac_cart0(coordenadas, celda):
    # esta funcion convierte de cartesianas a fraccionarios
    # pero esta ligado a ningun objecto, por eso se le pasa
    # la celda como parametro
    xyz = []
    sen_alpha, cos_alpha = trig(celda[1], celda[2], result="sin,cos")
    sen_beta, cos_beta   = trig(celda[0],celda[2], result="sin,cos")
    sen_gamma, cos_gamma = trig(celda[0],celda[1], result="sin,cos")
    # este v es necesario para la matrix de transformacion
    v = math.sqrt( 1 - cos_alpha**2 - cos_beta**2 - cos_gamma**2
                + 2 * cos_alpha * cos_beta * cos_gamma )

    a = np.linalg.norm(celda[0])
    b = np.linalg.norm(celda[1])
    c = np.linalg.norm(celda[2])

    matrix = np.array([[a ,b * cos_gamma, c * cos_beta ],
                [0, b * sen_gamma, c * (cos_alpha - cos_beta * cos_gamma)/(sen_gamma)],
                [0,0, c*v/sen_gamma ]])
    coordenadas = np.array(coordenadas)
    if coordenadas.ndim == 1:
        coordenadas = coordenadas.reshape(1,3)
        for i in range(len(coordenadas)):
            xyz.append(np.dot(matrix, coordenadas[i]))
            xyz = np.asarray(xyz)
        xyz = xyz.reshape(3,)
        xyz = list(xyz)
    else:
        for i in range(len(coordenadas)):
            xyz.append(np.dot(matrix, coordenadas[i]))
            # xyz = np.asarray(xyz)

    return xyz

def slice_coor(coor, n):
    # parece que era para sacar un numero de capas pero esta mal
    # ya tengo la funcion slice_slab
    coor = list(coor)
    sliced = []
    last = len(coor[0]) - 1
    lprev = coor[0][last]
    cont =  0
    for i in coor:
        if i[last] == lprev:
            sliced.append(i)
        else:
            cont = cont + 1
            if cont < n:
                sliced.append(i)
                lprev = i[last]
            elif cont == n:
                break
            else:
                print("The number of wanted layers is not valid")
                break
    return sliced


def extract_label(coorT, coorSlice, label_ext):
    # esta funcion toma las coordenadas coorSlice y busca las que
    # sean iguales en coorT y para saber a que atomo corresponde
    # ese atomo esta en la misma posicion en label_ext
    # label_ext es la label extendida que corresponde a cooT
    labelfinal = []
    for i in range(len(coorSlice)):
        for j in range(len(coorT)):
            if list(coorSlice[i]) == list(coorT[j]):
                labelfinal.append(label_ext[j])
    return labelfinal


def slice_slab(coor, n, p):
    # esta funcion corta un slab dejando solamente n numero de capas
    # si le coor con 10 de atomos y se pasa n = 5, solo dejara 5 capas
    # p corresponde a la ecuacion del plano base, a partir del cual se 
    # determinaran las capa
    # esta funcion corta un slab dejando solamente n numero de capas
    # si le coor con 10 de atomos y se pasa n = 5, solo dejara 5 capas
    # p corresponde a la ecuacion del plano base, a partir del cual se 
    # determinaran las capass

    # D lista donde almaceno el valor de la constante de la ecuacion
    # del plano ax + by + cz + d = 0
    # para coor calculo la d segun la ecuacion que le pase con p
    # si ya esta en D rompo del bucle asigno cond = 1 para no agregarlo 
    # a D, si no esta (math.fabs de la resta tiene que ser mayor que 1)
    # cond seguira siendo 0 y se agregara a D
    # luego organizo D y bucar hasta n veces las coordenadas que cumplan
    # con D en orden y las devuelvo
    # esta funcion no sirve para coordenadas de superceldas que ya se 
    # hayan optimizado y por tanto las capas se hayan movido, por los 
    # atomos de una mima capa no tendran la misma coodenada z
    coor = list(coor)
    D = []
    sliced = []
    cond = 0
    for i in coor:
        di = i[0]*p[0] + i[1]*p[1] + i[2]*p[2]
        for j in D:
            if math.fabs(di - j) < 0.00001: 
                cond = 1
                break
        if cond == 0:
            D.append(di)
        elif cond == 1:
            cond = 0
    
    D.sort()
    
    for j in range(n):
        for i in coor:
            if math.fabs(i[0]*p[0] + i[1]*p[1] + i[2]*p[2] - D[j]) < 0.0001:
                sliced.append(i)
     
    return sliced



def rotar(coor, vector, ang ):
    # esta funcion rota y conjunto de coordenadas "coor" en determinado
    #angulo "ang" alrededor del vector "vector" para ello emplea la matriz 
    # de transformacion "matriz" compuesta por los elementos del vector y 
    # el seno y el coseno del angulo "ang"
    xyz = []
    ux = vector[0]
    uy = vector[1]
    uz = vector[2]
    cos = math.cos(math.radians(ang))
    sen = math.sin(math.radians(ang))

    matrix = np.array([[cos + ux**2*(1-cos), ux*uy*(1-cos)-uz*sen, 
                        ux*uz*(1-cos) + uy*sen], [ uy*ux*(1-cos) + uz*sen,
            cos + uy**2*(1-cos), uy*uz*(1-cos) - ux*sen ],
            [ uz*ux*(1-cos) - uy*sen, uz*uy*(1-cos) + ux*sen,
            cos + uz**2*(1-cos) ]])

    for i in range(len(coor)):
            xyz.append(np.dot(matrix, coor[i]))
    xyz = np.asarray(xyz)
    return xyz


def vec_unit(coor_a1, coor_a2):
    # esta funcion devuelve el vector unitario del vector definido por
    # por dos puntos x, y, z 
    coor_a1 = np.array(coor_a1)
    coor_a2 = np.array(coor_a2)
    vec =  coor_a1 - coor_a2
    vec_mod = math.sqrt(vec[0]**2 + vec[1]**2 + vec[2]**2)
    vunit = np.array([vec[0]/vec_mod , vec[1]/vec_mod , vec[2]/vec_mod])
    return vunit

def trans_axis(coor, a1, direccion):
    # esta funcion redefine el cero de un conjunto de coordenadas respecto
    # a un punto x, y, z cartesiano
    # a1 es ese punto x,y,z y la variable direccion indica el tipo de trans-
    # formacion. Si direccion es 1 se le resta el punto a1 a las coordenadas
    # coor, de ese modo a1 seria 0,0,0 en las nuevas coordenadas
    xyz = []
    if direccion == "minus":
        for i in range(len(coor)):
            xyz.append(np.array([coor[i][0] - a1[0], 
            coor[i][1] - a1[1], coor[i][2] - a1[2]]))
    elif direccion == "plus":
        for i in range(len(coor)):
            xyz.append(np.array([coor[i][0] + a1[0], 
            coor[i][1] + a1[1], coor[i][2] + a1[2]]))
    else:
        print("Warning: the coordinates were not changed!")
        xyz = coor[:]
    return xyz


def trans_coor(coor, axis, desp):
    # this function translates the coordinates coor along the axis
    # x, y, z
    # it means that to every coordinate will be added the amount desp
    # desp in the direction axis,which can be x, y, or z
    xyz = []
    if axis == 'x':
        for i in coor:
            i[0] = i[0] + desp
            xyz.append(i)

    elif axis == 'y':
        for i in coor:
            i[1] = i[1] + desp
            xyz.append(i)

    elif axis == 'z':
        for i in coor:
            i[2] = i[2] + desp
            xyz.append(i)

    return xyz


def trig(*args, **keys):
    # returns the angle or the cosine of the angle, or the sin
    # of the angle or both the cosine and sine, or everything
    # arguments are the two vectors and a keyword result which is
    # equal to "sin", or "cos" or "ang" or "sin,cos" or "all" 
    # depending of what you want
    if keys["result"] == "ang":
        v1,v2 = args
        n1 = np.linalg.norm(v1)
        n2 = np.linalg.norm(v2)
        coseno = np.dot(v1,v2)/(n1 * n2)        
        angle = math.degrees(math.acos(coseno))
        return angle
    elif keys["result"] == "sin":
        v1,v2 = args
        n1 = np.linalg.norm(v1)
        n2 = np.linalg.norm(v2)
        coseno = np.dot(v1,v2)/(n1 * n2)        
        seno2 = 1 - coseno**2
        seno = math.sqrt(seno2)
        return seno
    elif keys["result"] == "cos":
        v1,v2 = args
        n1 = np.linalg.norm(v1)
        n2 = np.linalg.norm(v2)
        coseno = np.dot(v1,v2)/(n1 * n2)        
        return coseno
    elif keys["result"] == "sin,cos":
        v1,v2 = args
        n1 = np.linalg.norm(v1)
        n2 = np.linalg.norm(v2)
        coseno = np.dot(v1,v2)/(n1 * n2)        
        seno2 = 1 - coseno**2
        seno = math.sqrt(seno2)
        return seno, coseno
    elif keys["result"] == "all":
        v1,v2 = args
        n1 = np.linalg.norm(v1)
        n2 = np.linalg.norm(v2)
        coseno = np.dot(v1,v2)/(n1 * n2)        
        angle = math.degrees(math.acos(coseno))
        seno2 = 1 - coseno**2
        seno = math.sqrt(seno2)
        return seno, coseno, angulo


def distancia(p1,p2):
        p1 = np.array(p1)
        p2 = np.array(p2)
        v = p1 - p2
        r = np.linalg.norm(v)
        return r


def plane_eq(x, y, z):
    blank = [0,0,0]
    eq = ["","","",""]
    if x != blank and y != blank and z == blank:
        m = (y[1] - y[0])/(x[1] - x[0])
        eq[0] = -m
        eq[1] = 1
        eq[2] = 0
        eq[3] = eq[0]*x[0] + eq[1]*x[1]
    elif x != blank and y == blank and z != blank:
        m = (z[2] - z[0])/(x[1] - x[0])
        eq[0] = -m
        eq[1] = 0
        eq[2] = 1
        eq[3] = eq[0]*x[0] + eq[2]*x[1]
    elif x != blank and y == blank and z == blank:
        eq[0] = 1
        eq[1] = 0
        eq[2] = 0
        eq[3] = eq[0]*x[0] 
    elif x == blank and y != blank and z == blank:
        eq[0] = 0
        eq[1] = 1
        eq[2] = 0
        eq[3] = eq[1]*y[1] 
    elif x == blank and y == blank and z != blank:
        eq[0] = 0
        eq[1] = 0
        eq[2] = 1
        eq[3] = eq[2]*z[2] 
    else: 
        x = np.array(x)
        y = np.array(y)
        z = np.array(z)
        v1 = z - y
        v2 = x - y
        vnormal = np.cross(v1,v2)
        eq = list(vnormal)
        eq.append([])
        eq[3] = eq[0]*x[0] + eq[1]*x[1] + eq[2]*x[2]
    return eq

def atoms_plane(*args):
    atomos_plano = []
    if len(args) == 3:
        coor, ecuacion, location = args

        if location == "same":
            for i in range(len(coor)):
                r = coor[i][0]*ecuacion[0] + coor[i][1]*ecuacion[1] + coor[i][2]*ecuacion[2]
                menos_a = ecuacion[3] - 0.0001
                mas_a = ecuacion[3] + 0.0001
			        
                if menos_a < r and r < mas_a :
                    atomos_plano.append(coor[i])
            return atomos_plano

        elif location == "under":
            for i in range(len(coor)):
                if coor[i][0]*ecuacion[0] + coor[i][1]*ecuacion[1] + coor[i][2]*ecuacion[2] <= ecuacion[3] + 0.0001:
                    atomos_plano.append(coor[i])
            return atomos_plano
	
        elif location == "above":
            for i in range(len(coor)):
                if coor[i][0]*ecuacion[0] + coor[i][1]*ecuacion[1] + coor[i][2]*ecuacion[2] >= ecuacion[3] - 0.0001:
                     atomos_plano.append(coor[i])
            return atomos_plano
	
        else:
            print("You must enter the correct reference of the atoms with respect to the plane (same, under, above)")
    elif len(args) == 4:
        coor, ecuacion1, ecuacion2, location = args
        if ecuacion1[3] > ecuacion2[3]:
            for i in range(len(coor)):
                if (coor[i][0]*ecuacion1[0] + coor[i][1]*ecuacion1[1] + coor[i][2]*ecuacion1[2] <= ecuacion1[3] + 0.0001
			        and coor[i][0]*ecuacion1[0] + coor[i][1]*ecuacion1[1] + coor[i][2]*ecuacion1[2] >= ecuacion2[3] - 0.0001):
                        atomos_plano.append(coor[i])
            return atomos_plano
        elif ecuacion1[3] < ecuacion2[3]:
            for i in range(len(coor)):
                if (coor[i][0]*ecuacion1[0] + coor[i][1]*ecuacion1[1] + coor[i][2]*ecuacion1[2] <= ecuacion2[3] + 0.0001
			        and coor[i][0]*ecuacion1[0] + coor[i][1]*ecuacion1[1] + coor[i][2]*ecuacion1[2] >= ecuacion1[3] - 0.0001):
                        atomos_plano.append(coor[i])
            return atomos_plano
				
    else:
        print("Invalid number of arguments")

	



def new_basis_vector(atomos_plano_base, ldmin, eq_base):
    tb = [atomos_plano_base[0]]
    for i in atomos_plano_base:
        d = distancia(i, tb[0])
        if d >= ldmin[0] - 0.0001 and d <= ldmin[0] + 0.0001:
            tb.append(i)
            break
    if len(tb) == 2:
        for j in ldmin:
            if len(tb) == 3: break
            for i in atomos_plano_base:
                d = distancia(i, tb[0])
                if d >= j - 0.000001 and d <= j + 0.000001:
                    v1 = vec_unit(tb[0], tb[1])
                    v2 = vec_unit(tb[0], i)
                    try:
                        angulo = trig(v1,v2, result="ang")
                        print(angulo)
                    except Exception as e:
                        print(e)
                        angulo = 0
                    if angulo >= 90 and angulo < 180:
                        tb.append(i)
                        break 
    
    else:
        print("There is an error: the second atom of tb has not been found")


    tbnp = tb[:]
    v1 = vec_unit(tb[0],tb[1])
    v2 = vec_unit(tb[0],tb[2])
    v3 = list(np.cross(v1,v2) + tb[0])
    v4 = list(np.cross(v2,v1) + tb[0])
    vref = eq_base[:3]
    ang1 = trig(v3, vref, result="ang")
    ang2 = trig(v4, vref, result="ang")
    # print "angulo 1 y angulo 2", ang1, ang2
    
    '''if same_quad(v3, eq_base[:3]) == False:
        v3 = list(np.cross(v2,v1) + tb[0])
    print (v3)'''
    tb.append(v3)
    return tb



def dmin(coor):
    ldmin = []
    for i in range(len(coor)):
        if len(ldmin) >=10:
            break
        for j in range(i):
            dist = round(distancia(coor[i], coor[j]), 7)
        if dist in ldmin:
            pass
        else:
            ldmin.append(dist)

            if len(ldmin) >= 10:
                break

    ldmin.sort()
    ldmin = ldmin[:2]
    return ldmin

def redondear(coor):
    coor = list(coor)
    for i in range(len(coor)):
        coor[i] = list(coor[i])
        for j in range(len(coor[i])):
            if math.fabs(coor[i][j]) < 1.0e-14 and math.fabs(coor[i][j]) > 0:
                coor[i][j] = 0.0
            elif (coor[i][j] <= 1.001 and coor[i][j] >= 0.99):
                coor[i][j] = 1.0
    return coor

def same_quad(v1,v2):
	if (v1[0]*v2[0] >= 0 and v1[1]*v2[1] >= 0
	and v1[2]*v2[2] >= 0):
		return True
	else:
		return False

def del_at_rep(coor):
    coor = list(coor)
    Nx,Ny = [],[]
    for i in range(len(coor)):
        coor[i] = list(coor[i])
        if coor[i][0] == 1.0:
            Nx.append(coor[i])
        elif coor[i][1] == 1.0:
            Ny.append(coor[i])
    for i in Nx:    coor.remove(i)
    for i in Ny:    coor.remove(i)
    return coor

def del_at_rep_general(coor):
    coor = list(coor)
    Xs = []
    Ys = []
    for i in range(len(coor)):
        coor[i] = list(coor[i])
        Xs.append(coor[i][0])
        Ys.append(coor[i][1])
    maxX = max(Xs)
    maxY = max(Ys)
    Nx,Ny = [],[]
    for i in coor:
        if i[0] == maxX:
            Nx.append(i)
        elif i[1] == maxY:
            Ny.append(i)
	
    for i in Nx:	coor.remove(i)
    for i in Ny:    coor.remove(i)
    return coor

def new_cell(tb, z):
    cell = [[],[],[]]
    cell[0] = tb[1]
    cell[1] = tb[2]
    cell[2] = [0.0,0.0,z]
    return cell


def matriz_transf(c1, c2, coor):
    xyz = []
    c1 = np.array(c1)
    c2 = np.array(c2)
    coor = np.array(coor)
    X,Y,Z = c1
    U,V,W = c2

    X = X/np.linalg.norm(X)
    Y = Y/np.linalg.norm(Y)
    Z = Z/np.linalg.norm(Z)
    U = U/np.linalg.norm(U)
    V = V/np.linalg.norm(V)
    W = W/np.linalg.norm(W)

    cos_xu = trig(X,U,result="cos")
    cos_xv = trig(X,V,result="cos")
    cos_xw = trig(X,W,result="cos")
	
    cos_yu = trig(Y,U,result="cos")
    cos_yv = trig(Y,V,result="cos")
    cos_yw = trig(Y,W,result="cos")
	
    cos_zu = trig(Z,U,result="cos")
    cos_zv = trig(Z,V,result="cos")
    cos_zw = trig(Z,W,result="cos")
	
    matrix = np.array([[cos_xu, cos_yu, cos_zu],
		[cos_xv, cos_yv, cos_zv],
		[cos_xw, cos_yw, cos_zw]])
	#print matrix
    for i in range(len(coor)):
        xyz.append(list(np.dot(matrix, coor[i])))
    return xyz

def thickness(coor):
    z = []
    for i in coor:
        z = np.append(z, i[2])
    maximo = np.amax(z)
    minimo = np.amin(z)
    return maximo - minimo

def parameters(file0):
    # reads the parameters to build the slab
    # from a file. file0 is the name of the file 
    # to read

    f = open(file0)
    l = f.readlines()
    f.close()
    plane = np.array(l[0].split()[1:], dtype=float)
    supercell = l[1].split()[1:]
    for i in range(2): supercell[i] = int(supercell[i])
    layers = int(l[2].split()[1])
    vacuum = float(l[3].split()[1])
    return plane, supercell, layers,vacuum

def planecoor(plane):
    plane = list(plane)
    xplane, yplane, zplane = [],[],[]
    n = np.count_nonzero(plane)
    if n == 3:
        xplane = [1/plane[0], 0, 0]
        yplane = [0, 1/plane[1], 0]
        zplane = [0, 0, 1/plane[2]]
    elif n == 2:
        if plane[2] == 0:
            xplane = [1/plane[0], 0, 0]
            yplane = [0, 1/plane[1], 0]
            zplane = [0,0,0]
        elif plane[1] == 0:
            xplane = [1/plane[0], 0, 0]
            yplane = [0, 0, 0]
            zplane = [0,0,1/plane[2]]
        else:
            xplane = [0, 0, 0]
            yplane = [0, 1/plane[1], 0]
            zplane = [0,0,1/plane[2]]
    elif n == 1:
        if plane[0] != 0:
            xplane = [1/plane[0], 0, 0]
            yplane = [0, 0, 0]
            zplane = [0,0,0]
        elif plane[1] != 0:
            xplane = [0, 0, 0]
            yplane = [0, 1/plane[1], 0]
            zplane = [0,0,0]
        elif plane[2] != 0:
            xplane = [0, 0, 0]
            yplane = [0, 0, 0]
            zplane = [0,0,1/plane[2]]
    else:
        print("Error in the plane specifications")
        
    return xplane, yplane,zplane
         
def build_slab(poscarfile, plane, supercell, layers, vacuum):

    xplano, yplano, zplano = planecoor(plane)

    f = Poscar()
    f.load_poscar(poscarfile)

    xplano = frac_cart0(xplano, f.celda)
    yplano = frac_cart0(yplano, f.celda)
    zplano = frac_cart0(zplano, f.celda)
    f.extend_label()        
    f.exp_coor([6,6,6], "labels") 
    f.exp_coor_neg("labels")
    f.coor = f.frac_cart(f.coor)
    eq_base = plane_eq(xplano, yplano, zplano)
    at_pb = atoms_plane(f.coor, eq_base, "same")

    ldmin = dmin(at_pb)

    if len(f.label) == 1:
        alloy = 1
    elif len(f.label) == 2:
        alloy = 2
        for i in range(2): ldmin[i] = ldmin[i]*2
        for i in range(2): supercell[i] = supercell[i]/2

    print (ldmin)

    at_sobre_pb = atoms_plane(f.coor, eq_base, "above")

    tb = new_basis_vector(at_pb, ldmin, eq_base)

    #saco las ecuaciones de los otros dos planos
    d1 = distancia(tb[1],tb[0])
    d2 = distancia(tb[2],tb[0])
    d3 = distancia(tb[3],tb[0])
    eq_1 = plane_eq(tb[1],tb[0],tb[3])
    eq_2 = plane_eq(tb[2],tb[0],tb[3])

    Db2 = eq_base[0]*tb[3][0] + eq_base[1]*tb[3][1] + eq_base[2]*tb[3][2]
    D12 = eq_1[0]*tb[2][0] + eq_1[1]*tb[2][1] + eq_1[2]*tb[2][2]
    D22 = eq_2[0]*tb[1][0] + eq_2[1]*tb[1][1] + eq_2[2]*tb[1][2]

    eq_base2 = []
    eq_base2.extend(eq_base)
    eq_base2[3] = Db2

    eq_12 = []
    eq_12.extend(eq_1)
    eq_12[3] = D12

    eq_22 = []
    eq_22.extend(eq_2)
    eq_22[3] = D22

    #quito lo quede fuera de los planos
    at_medio_base = atoms_plane(at_sobre_pb, eq_1, eq_12, "under")
    at_medio_base = atoms_plane(at_medio_base, eq_2, eq_22, "under")

    #ahora corto las capas
    at_medio_base = slice_slab(at_medio_base, layers, eq_base)

    #organizo por atomos, para eso tengo que tener actualizado 
    #f.coor y f.label_extended
    f.label_extended = extract_label(f.coor, at_medio_base, f.label_extended)
    f.coor = at_medio_base[:] 
    f.zip_coorlabel()  
    f.ord_zipped()     
    f.cleave_zipped()  

    f.coor = trans_axis(f.coor, tb[0], "minus")   
    tb = trans_axis(tb, tb[0], "minus")

    tb2_t = np.cross(tb[1], tb[3])
    if same_quad(tb2_t, tb[2]) == False:
        tb2_t = np.cross(tb[3],tb[1])
    t_celda = [tb[1], tb2_t,tb[3]]
    
    f.coor = matriz_transf(f.celda, t_celda, f.coor)

    tbb = np.array(tb)
    vv1 = tbb[1]-tbb[0]
    vv2 = tbb[2]-tbb[0]

    print ("ang vec base",trig(vv1,vv2, result="ang"))
    tb = matriz_transf(f.celda, t_celda, tb)
    tbb = np.array(tb)
    vv1 = tbb[1]-tbb[0]
    vv2 = tbb[2]-tbb[0]
    print ("ang vec base",trig(vv1,vv2, result="ang"))

    thick = thickness(f.coor)

    n_celda = np.asarray([tb[1], tb[2], [0,0,thick + vacuum]])

    #lo hago asi, porque f.a, f.b , f.c no se han cambiado
    f.coor = cart_frac0(f.coor, n_celda)

    f.coor = redondear(f.coor)
    sin_at_rep = del_at_rep(f.coor)

    f.label_extended = extract_label(f.coor, sin_at_rep, f.label_extended)
    f.coor = sin_at_rep[:]

    f.exp_coor([supercell[0], supercell[1],1], "labels") 
    f.coor = frac_cart0(f.coor, n_celda)

    n_celda[0] = (supercell[0])*n_celda[0]
    n_celda[1] = (supercell[1])*n_celda[1]

    f.celda = n_celda[:]

    f.coor = cart_frac0(f.coor, n_celda)

    f.zip_coorlabel()
    f.ord_zipped()
    f.cleave_zipped()

    f.extend_at()
    f.poscar_out("salida")

class radios(object):
    def __init__(self):
        self.Ni = 1.24
        self.Co = 1.24
        self.C  = 0.77
        self.H  = 0.37
        self.O  = 0.73
        self.N  = 0.75
        self.Cu = 1.26
        self.Si = 1.11
        self.Al = 1.18
        self.P  = 1.06
	
radios = {
	"Ni": 1.35,
	"Co": 1.3,
        "C": 0.99,
	"H": 0.37,
	"O": 0.9,
	"N": 0.75,
	"Cu": 1.3,
	"Si": 1.11,
	"Al": 1.18,
	"P": 1.06,

}

spheres = {
	"Ni": 1.32,
	"Co": 1.3,
        "C": 0.476,
	"H": 0.308,
	"O": 0.4256,
	"N": 0.434,
	"Cu": 1.3,
	"Si": 1.11,
	"Al": 1.18,
	"P": 1.06,

}


cylinders = {
	"Ni": 0.2,
	"Co": 0.2,
    "C": 0.2,
	"H": 0.2,
	"O": 0.2,
	"N": 0.2,
	"Cu": 0.2,
	"Si": 0.2,
	"Al": 0.2,
	"P": 0.2,

}

colors = {
	"Ni": (0.31, 0.82, 0.31, 0),
	"Co": (1, 1, 1, 0),
    "C": (0.4, 0.4, 0.4, 0),
	"H": (0.75, 0.75, 0.75, 0),
	"O": (1, 0.05, 0.05, 0),
	"N": (0.05, 0.05, 1, 0),
	"Cu": 0.2,
	"Si": 0.2,
	"Al": 0.2,
	"P": 0.2,

}


if __name__  == "__main__":
	pass 

