from collections import OrderedDict
from mechasuite.Read import *
from mechasuite.globs import *
import os, time, random
import numpy as np
import shutil
import mechasuite.Thermo as Thermo
from scipy import optimize
from mechasuite.Landau import *
import yaml, json
import copy

# some constants here as per the NIST (CODATA 2014)




class PlotLabel(object):
    def __init__(self, name, itm, **kwargs):
        # itm has to be of PlotItm class type or None
        self.name = name
        self.itm = itm
        if "text" in kwargs:
            self.text = kwargs["text"]
        else:
            if itm is not None:
                self.text = itm.name
            else:
                self.text = "No text"

        if "color" in kwargs:
            self.color = kwargs["color"]
        else:
            self.color = (0, 0, 0)

        if "coors" in kwargs:
            self.coors = kwargs["coors"]
        else:
            if itm is not None:
                self.coors = itm.coors[:2]
            else:
                self.coors = [0, 0]

        if "font" in kwargs:
            self.font = kwargs["font"]
        else:
            self.font = ""

        if "visible" in kwargs:
            self.visible = kwargs["visible"]
        else:
            self.visible = True

    def to_dict(self):
        selfdict = OrderedDict()
        selfdict["itm"] = self.itm.name
        selfdict["text"] = self.text
        selfdict["color"] = self.color
        selfdict["name"] = self.name
        selfdict["font"] = self.font
        selfdict["coors"] = self.coors
        selfdict["visible"] = self.visible
        return selfdict

    def reset_coors(self, ma=0, mi=0):
        if self.itm is not None:
            self.coors[0] = self.itm.coors[0]
            if ma and mi:
                self.coors[1] = self.itm.coors[1] + (ma - mi)*0.1
            else:
                self.coors[1] = self.itm.coors[1]


class PlotItm(object):
    def __init__(self, itm, ref, **kwargs):
        # ref es la misma que la ref de la instancia de la clase
        # Plot que son todas string con el nombre la referencia
        self.ref = ref
        self.itm = itm
        if "name" in kwargs:
            self.name = kwargs["name"]
        else:
            self.name = itm.name
        if "color" in kwargs:
            self.color = kwargs["color"]
        else:
            self.color = (0, 0, 0)
        if "width" in kwargs:
            self.width = kwargs["width"]
        else:
            self.width = 6
        if "length" in kwargs:
            self.length = kwargs["length"]
        else:
            self.length = 1
        if "style" in kwargs:
            self.style = kwargs["style"]
        else:
            self.style = "solid line"
        if "coors" in kwargs:
            self.coors = kwargs["coors"]
            # fix energy that might change
            self.coors[1] = self.energy()
            self.coors[3] = self.energy()
        else:
            self.coors = [0, self.energy(),
                          0, self.energy()]

        # both are str names lists to
        # connect the bar with the links
        if "blinks" in kwargs:
            self.blinks = kwargs["blinks"]
        else:
            self.blinks = []
        if "alinks" in kwargs:
            self.alinks = kwargs["alinks"]
        else:
            self.alinks = []

    def to_dict(self):
        idict = {}
        # no lo necesito porque al crearlo con
        # add_itm en plot le paso la ref del plot
        # idict["ref"] = self.ref
        idict["name"] = self.name
        idict["color"] = self.color
        idict["width"] = self.width
        idict["length"] = self.length
        idict["style"] = self.style
        idict["coors"] = self.coors
        idict["blinks"] = self.blinks
        idict["alinks"] = self.alinks
        return idict

    def get_temps(self):
        return self.itm.get_ref(self.ref).temps

    def get_name(self):
        return self.itm.name

    def energy(self):
        return self.itm.get_ref(self.ref).energy

    def get_energy(self):
        return self.itm.get_ref(self.ref).energy

    def g(self, T):
        return self.itm.get_ref(self.ref).g(T)

    def get_g_or_energy(self, etype, T):
        # Probably not the best designed method
        # but it is usefull to shorten if-statements
        # because of ambiguity the arguments have no defaults
        if etype == "E":
            energy = self.energy()
        else:
            # there is no easy way to check temperature in refs
            # will put a try
            try:
                energy = self.g(T)
            except:
                energy = None
        return energy

    def reacs(self):
        return self.itm.get_reacs()

    def exc_states(self):
        return self.itm.get_excited_states()
    
    def update_energy(self, etype="E", temp=None):
        if etype == "E":
            self.coors[1] = self.energy()
            self.coors[3] = self.energy()
        elif etype == "G":
            if temp is None:
                return False
            self.coors[1] = self.g(temp)
            self.coors[3] = self.g(temp)
        return True

    def get_reacs(self):
        return self.itm.get_reacs()

    def tp(self):
        return self.itm.tp

    def del_link(self, linkname):
        try:
            self.alinks.remove(linkname)
        except ValueError:
            try:
                self.blinks.remove(linkname)
            except ValueError:
                print("No link found to delete")

    def add_link(self, link, pos):
        if not isinstance(link, str):
            print("Link not added to plotted item")
            print("String expected")
            return
        if pos == "after":
            if not link in self.alinks:
                self.alinks.append(link)
            else:
                print("Link already exists in alinks")
        elif pos == "before":
            if not link in self.blinks:
                self.blinks.append(link)
            else:
                print("Link already exists in blinks")
        else:
            print("Link not added to plotted item")


class PlotLink(object):
    def __init__(self, name, plotitm1, plotitm2, **kwargs):
        if not isinstance(plotitm1, PlotItm) or not isinstance(plotitm2, PlotItm):
            raise TypeError("Expected PlotItm type argument")
        self.name = name
        self.pitm1, self.pitm2 = plotitm1, plotitm2
        # self.walk = None
        if "color" in kwargs:
            self.color = kwargs["color"]
        else:
            self.color = (100, 100, 100)
        if "width" in kwargs:
            self.width = kwargs["width"]
        else:
            self.width = 4
        if "style" in kwargs:
            self.style = kwargs["style"]
        else:
            self.style = "solid line"

        self.coors = [0, 0, 0, 0]
        self.update_energy()

    def to_dict(self):
        idict = {}
        idict["name"] = self.name
        idict["color"] = self.color
        idict["width"] = self.width
        idict["style"] = self.style
        idict["coors"] = self.coors
        idict["pitm1"] = self.pitm1.name
        idict["pitm2"] = self.pitm2.name
        return idict

    def update_energy(self, etype="E", temp=None):
        if etype == "E":
            self.coors = [self.pitm1.coors[2],
                          self.pitm1.energy(),
                          self.pitm2.coors[0],
                          self.pitm2.energy()]

        elif etype == "G":
            if temp is None:
                return False
            self.coors = [self.pitm1.coors[2],
                          self.pitm1.g(temp),
                          self.pitm2.coors[0],
                          self.pitm2.g(temp)]
        return True

    def energy1(self):
        return self.pitm1.energy()

    def energy2(self):
        return self.pitm2.energy()


class Plot(object):
    def __init__(self, name, ref, col, itms=[], labels=False, *args, **kwargs):
        self.name = name
        self.ref = ref
        self.col = col
        self.itms = {}
        self.links = {}
        self.labels = {}
        if itms:
            for itm in itms:
                self.add_itm(itm)

        self.update_order()
        # self.update_links()
        if labels:
            self.set_labels()

        self.xmax, self.xmin, self.ymax, self.ymin = 10, 0, 0, -10
        if "xmax" in kwargs:
            self.xmax = kwargs["xmax"]

        if "xmin" in kwargs:
            self.xmin = kwargs["xmin"]

        if "ymax" in kwargs:
            self.ymax = kwargs["ymax"]

        if "ymin" in kwargs:
            self.ymin = kwargs["ymin"]

        if self.xmax == 10 and self.xmin == 0:
            self.set_extreme_coors()

        if "etype" in kwargs:
            self.etype = kwargs["etype"]
        else:
            self.etype = "E"

        if "temps" in kwargs:
            self.temps = kwargs["temps"]
        else:
            self.update_temps()

        if "temp" in kwargs:
            self.temp = kwargs["temp"]
        else:
            if self.etype == "E":
                self.temp = None
            elif self.etype == "G":
                if self.temps:
                    self.temp = self.temps[0]
                else:
                    self.etype = "E"
                    self.temp = None
            else:
                self.etype = "E"
                self.temp = None

    def to_dict(self):
        gdict = {}
        gdict["xmax"] = self.xmax
        gdict["ymax"] = self.ymax
        gdict["ymin"] = self.ymin
        gdict["xmin"] = self.xmin
        gdict["name"] = self.name
        gdict["ref"] = self.ref
        gdict["col"] = self.col.name
        gdict["etype"] = self.etype
        gdict["temps"] = self.temps
        gdict["temp"] = self.temp
        gdict["itms"] = [itm.to_dict() for itm in self.itms.values()]
        gdict["links"] = [link.to_dict() for link in self.links.values()]
        gdict["labels"] = [label.to_dict() for label in self.labels.values()]
        return gdict

    def update_type_en(self, t):
        if t == self.etype:
            return
        if t == "G":
            self.update_temps()
            if not self.temps:
                self.etype = "E"
            else:
                if self.temp not in self.temps:
                    self.temp = self.temps[0]
                    self.etype = "G"
                else:
                    self.etype = "G"
        elif t == "E":
            self.etype = "E"
        else:
            return
        self.update_temp_en()

    def update_temp_en(self, temp=0):
        if temp in self.temps:
            self.temp = temp
        for itm in self.itms.values():
            itm.update_energy(self.etype, self.temp)
        for link in self.links.values():
            link.update_energy(self.etype, self.temp)
        self.set_extreme_coors()
        for label in self.labels.values():
            # label.reset_coors(self.xmax, self.xmin)
            label.reset_coors()

    def update_temps(self):
        alltemps = []
        for itm in self.itms.values():
            alltemps += itm.get_temps()
        unique, counts = np.unique(alltemps, return_counts=True)
        self.temps = [unique[n] for n, c in enumerate(counts)
                      if c == len(self.itms)]
        self.temps.sort()

    def set_extreme_coors(self):
        xs = [itm.coors[2]
            for itm in self.itms.values()]
        ys = [itm.coors[1]
              for itm in self.itms.values()]
        try:
            self.xmax = max(xs)+1
            ymx = max(ys)
            ymn = min(ys)
            deltay = ymx - ymn
            self.ymax = ymx + deltay*0.1
            self.ymin = ymn - deltay*0.1
        except Exception as e:
            print(e)

    def add_label(self, itm, name=None, **kwargs):
        if isinstance(itm, str):
            itm = self.get_itm(itm)

        if name is None:
            if itm is not None:
                name = itm.name
            else:
                name = str(random.randint(0, 100000))
                while name in self.labels.keys():
                    name = str(random.randint(0, 100000))

        if name not in self.labels.keys():
            self.labels[itm.name] = PlotLabel(name, itm, **kwargs)

    def set_labels(self):
        for itm in self.itms.values():
            if itm.name not in self.labels.keys():
                self.labels[itm.name] = PlotLabel(itm.name, itm)

    def update_order(self, items=None, lastcoor=0.1):
        if items is None:
            items = self.itms.values()
        itmbefore = None
        for itm in items:
            itm.coors[0] = lastcoor + 1
            itm.coors[2] = lastcoor + 2
            lastcoor += 2
            if itmbefore is not None:
                # linkname = itm.name + itmbefore.name
                newlink = self.add_link(itmbefore, itm)
                # if newlink is not None:
                #     itm.add_link(linkname, "before")
                #     itmbefore.add_link(linkname, "after")
            itmbefore = itm

    def add_link(self, itm1, itm, **kwargs):
        if "name" in kwargs:
            linkname = kwargs["name"]
        else:
            linkname = itm.name + itm1.name
        if not linkname in self.links.keys():
            self.links[linkname] = PlotLink(linkname, itm1, itm, **kwargs)
            itm.add_link(linkname, "before")
            itm1.add_link(linkname, "after")
            return self.links[linkname]
        else:
            return None

    def get_itms(self):
        return self.itms.values()

    def get_itm(self, name):
        if name in self.itms.keys():
            return self.itms[name]
        else:
            return None

    def add_itm(self, itm, **kwargs):
        if itm.name in self.itms: # avoid repeating
            return
        self.itms[itm.name] = PlotItm(itm, self.ref, **kwargs)

    def add_itms(self, items, labels=True):
        # add new PlotItm items to plot and create links
        # within the update_order() function
        if self.get_itms():
            lastcoor = max([itm.coors[0]
                     for itm in self.get_itms()])
        else:
            lastcoor = 0

        newitems = []
        for itm in items:
            if itm.name in self.itms: # avoid repeating
                continue
            self.itms[itm.name] = PlotItm(itm, self.ref)
            newitems.append(self.itms[itm.name])
        self.update_order(newitems, lastcoor)
        if labels:
            for itm in newitems:
                if itm.name not in self.labels.keys():
                    self.labels[itm.name] = PlotLabel(itm.name, self.itms[itm.name])
        self.set_extreme_coors()

    def get_links(self):
        return self.links.values()

    def get_link(self, name):
        if name in self.links.keys():
            return self.links[name]
        else:
            return None

    def get_labels(self):
        return self.labels.values()

    def write_old_react_network_input(self, outdic, filename):
        f = open(filename, "w")
        f.write("Mechanism\n")
        for stepname in outdic["mec"].keys():
            f.write(f"{stepname} \n")
        f.write("End Mechanism\n")
        ksTdic = {}
        for values in outdic["mec"].values():
            for T, ks in values.items():
                if T in ksTdic:
                    ksTdic[T].append(str(ks[0])+" "+str(ks[1]))
                else:
                    ksTdic[T] = [str(ks[0])+" "+str(ks[1])]
        # write T ks
        for T, ks in ksTdic.items():
            f.write("Temperature "+str(T)+"\n")
            for k in ks:
                f.write(k+"\n")
        f.write("Initial Values\n")
        for l, iv in outdic["initial_values"].items():
            f.write(f"{l} {iv} \n")
        f.write(f"time {outdic['time']} \n")
        f.close()
        return 

    def update_export_step_dic(self, itmo, paths_dic, init_val: dict):
        first_min = None 
        second_min = None

        if len(itmo.blinks) == 0:
            init_val[itmo.name] = 1

        for blink in itmo.blinks:
            first_min = None 
            blinko = self.get_link(blink)
            if blinko is not None:
               first_min = blinko.pitm1 # got the first minima

            for alink in itmo.alinks:
                second_min = None
                alinko = self.get_link(alink)
                if alinko is not None:
                    second_min = alinko.pitm2 # got the second minima
            
                if itmo.itm.tp == "ts":
                    # ts cannot be disconnected
                    if first_min is None or second_min is None: 
                        continue
                    #if first_min.itm.merged:
                    #    key = f"{first_min.itm.itms[0].name}+{first_min.itm.itms[1].name}="
                    #else:
                    #    key = f"{first_min.name}="
                    #if second_min.itm.merged:
                    #    key += f"{second_min.itm.itms[0].name}+{second_min.itm.itms[1].name}"
                    #else:
                    #    key = f"{second_min.name}"
                    key = f"{first_min.name}={second_min.name}"
                    paths_dic[key] = [first_min, second_min, itmo] # ts goes to the end
                elif itmo.itm.tp == "min":
                    if first_min.tp != "ts":
                        key = f"{first_min.name}={itmo.name}"
                        paths_dic[key] = [first_min, itmo] # no TS
                    if second_min.itm.tp != "ts":
                        key = f"{itmo.name}={second_min.name}"
                        paths_dic[key] = [itmo, second_min] # no TS
        return paths_dic, init_val
    
    def update_reaction_network_dic(self, outdic, paths_dic, T:float): 
        u = self.col.unit
        for stepname, itms in paths_dic.items():
            if len(itms) == 3:
                first_min = itms[0]
                second_min = itms[1]
                tsobj = itms[2]
            else:
                if itms[0].get_g_or_energy(self.etype, T) > itms[1].get_g_or_energy(self.etype, T):
                    first_min = itms[0]
                    tsobj = itms[0]
                    second_min = itms[1]
                else:
                    first_min = itms[0]
                    tsobj = itms[1]
                    second_min = itms[1]

            dgf = 0; dgr = 0
            g2 = second_min.get_g_or_energy(self.etype, T)
            g1 = first_min.get_g_or_energy(self.etype, T)
            gts = tsobj.get_g_or_energy(self.etype, T)
            dgf = gts - g1
            dgr = gts - g2

            # print(dgf, dgr)
            kf = (Kb[u] * T / h[u]) * (np.e**(-dgf / (R[u] * T)))
            kr = (Kb[u] * T / h[u]) * (np.e**(-dgr / (R[u] * T)))

            if stepname in outdic["mec"]:
                outdic["mec"][stepname][T] = [kf, kr]
            else:
                outdic["mec"][stepname] = {T:[kf, kr]}
        return outdic

    def export_reaction_network(self, filename="reaction_network.csv", temperatures=[], outdic={}):
        print("Exporting reaction network ", self.etype)
        if self.etype == "E":
            print("WARNING! ASSUMING ZERO ENTROPY")

        # T = self.temp
        if not outdic:
            outdic = {"mec":{}, "time": "0 10 1000"}      
        paths_dic, init_val = {}, {}
        for itmo in self.get_itms():
            paths_dic, init_val = self.update_export_step_dic(itmo, paths_dic, init_val)

        T = 298
        network_str = []
        ks_str = ""
        count = 0
        if self.etype == "E":
            ks_str += "Temperature 298\n"
            outdic = self.update_reaction_network_dic(outdic, paths_dic, T)
        else:
            for T in self.temps:
              ks_str += f"Temperature {T:.2f}\n"
              outdic = self.update_reaction_network_dic(outdic, paths_dic, T)
        
        outdic["initial_values"] = init_val

        #self.write_old_react_network_input(outdic, filename)

        if not filename.endswith(".json"):
            filename += ".json"
        with open(filename, "w") as f:
            json.dump(outdic, f, indent=4)
        #try:
        #    if not filename.endswith(".yaml"):
        #      filename += ".yaml"
        #    with open(filename, "w") as f:
        #        yaml.safe_dump(outdic, f)
        #except Exception as e: 
        #    print(e)
        #return


class Struct():
    def __init__(self, sdict=None):
        self.coors = []
        self.cell = [[0, 0, 0], [0, 0, 0], [0, 0, 0]]
        self.labels = []
        self.nat = 0
        self.atoms = OrderedDict()
        self.rot_matrix = [[1, 0, 0], [0, 1, 0], [0, 0, 1]]
        self.showcell = False
        self.uRange, self.dRange = 10, -10
        self.rRange, self.lRange = 10, -10

        if sdict is not None:
            if "coors" in sdict:
                self.coors = sdict["coors"]
            if "cell" in sdict:
                self.cell = sdict["cell"]
            if "labels" in sdict:
                self.labels = sdict["labels"]
            if "nat" in sdict:
                self.nat = sdict["nat"]
            if "atoms" in sdict:
                self.atoms = sdict["atoms"]
            if "rot_mat" in sdict:
                self.rot_matrix = sdict["rot_mat"]
            if "rRange" in sdict:
                self.rRange = sdict["rRange"]
            if "rRange" in sdict:
                self.lRange = sdict["lRange"]
            if "rRange" in sdict:
                self.uRange = sdict["uRange"]
            if "rRange" in sdict:
                self.dRange = sdict["dRange"]

        self.update()

    def update(self, *args, **kwargs):
        if "nat" in kwargs:
            self.nat = kwargs["nat"]
        if "labels" in kwargs:
            self.labels = kwargs["labels"]
        if "coors" in kwargs:
            self.coors = kwargs["coors"]
        if "cell" in kwargs:
            self.cell = kwargs["cell"]

        self.set_atoms()

    def set_atoms(self):
        # reset atoms
        self.atoms = OrderedDict()

        labels_by_elements, num_elements = [], []
        for label in self.labels:
            if not label in labels_by_elements:
                labels_by_elements.append(label)
                num_elements.append(1)
            else:
                index = labels_by_elements.index(label)
                num_elements[index] += 1
        for label, num in zip(labels_by_elements, num_elements):
            self.atoms[label] = num

    def to_dit(self):
        selfdict = OrderedDict()
        selfdict["coors"] = self.coors
        selfdict["cell"] = [list(c) for c in self.cell]
        selfdict["labels"] = self.labels
        selfdict["nat"] = int(self.nat)
        selfdict["atoms"] = self.atoms
        selfdict["rRange"] = self.rRange
        selfdict["lRange"] = self.lRange
        selfdict["uRange"] = self.uRange
        selfdict["dRange"] = self.dRange
        selfdict["rot_mat"] = [list(i) for i in self.rot_matrix]
        return selfdict


class Freq(object):
    def __init__(self, fdict=None):
        if fdict is not None:
            self.set_vibs(fdict["vibs"])
            self.unit = fdict["unit"]
        else:
            self.vibs = []
            self.unit = None

    def to_dict(self):
        selfdict = OrderedDict()
        selfdict["vibs"] = self.vibs
        selfdict["unit"] = self.unit
        return selfdict

    def set_vibs(self, vibs):
        vibs = [float(f) for f in vibs]
        vibs.sort(reverse=True)
        self.vibs = vibs


class Reference(object):
    def __init__(self, parent_itm, name, refs, coefs=[]):
        """
        :param parent_itm:  the name of the itm for which this reference is created
        :param name: name of the reference, equal to the name of reference in mechanism
        :param refs: list of Itm objects that make up the reference
        :param coefs: list of coefficients for every element of the list of refs
        """
        if not isinstance(refs, list):
            raise TypeError("Expected type list for itms arg")

        if len(refs) == 1:
            if len(refs[0].struct.atoms) != len(parent_itm.struct.atoms):
                raise ValueError("Bad reference list to create a Reference, "
                                 "got only one item as reference and it does not have "
                                 "the same number of atom type as the parent item")

            # if refs[0].struct.nat != parent_itm.struct.nat:
            #     raise ValueError("Bad reference list to create a Reference, "
            #                      "got only one Itm as reference and it does not have "
            #                      "the same number of atoms as the parent Itm")

        self.parent_itm = parent_itm
        self.name = name
        self.coefs = []
        self.temps = []
        # refs are itm object that serve as reference
        self.refs = refs
        # energy relative to parent itm
        self.energy = 0.0
        # absolute energy calculated from self.coefs
        self.abs_en = 0.0
        self.abs_zpe = 0.0
        self.zpe = 0.0

        self.thermo = {
            "g": {}, "h": {}, "stot": {},
            "qtot": {}, "k": {}, "kq": {},
            "a":{}
        }
        if not coefs:
            self.set_coefficients()
        else:
            self.coefs = coefs
        self.update()

    def to_dict(self):
        selfdict = OrderedDict()
        selfdict["parent_itm"] = self.parent_itm.name
        selfdict["refs"] = [ref.name for ref in self.refs]
        selfdict["coefs"] = self.coefs
        selfdict["name"] = self.name
        selfdict["thermo"] = self.thermo
        return selfdict

    def set_coefficients(self):
        coefs = []
        zeroindices = []
        """
        sols is the number of atoms of the parent(intermediate) which are
        the solution vector V of the system of equations
        coefX = V
        """
        sols = list(self.parent_itm.struct.atoms.values())
        """ coefs is a MxN matrix where every row represent one atom of the parent
            itm and every column represent a ref item. Every element Cij is the number
            of atoms of type i present if ref j
        """
        #print(self.parent_itm.struct.atoms)
        for atom in self.parent_itm.struct.atoms:
            coefs.append([])
            for ref in self.refs:
                if atom in ref.struct.atoms:
                    coefs[-1].append(ref.struct.atoms[atom])
                else:
                    coefs[-1].append(0)
        # print("coefs: ", coefs)

        """
         Set the number of equations to the number
         of variables Because I could get more equations
         than needed depending on the length of the basis ref
         if there are 10 atoms and 2 refs obj, 8 rows of coefs 
         are left out
        """

        coefs2, sols2 = [], []
        if len(coefs) > len(self.refs): # len(coefs) is num of rows, num of atoms
            # zeros = [i.count(0) for i in coefs]
            # # get the number of zeros in each column
            # maxzero = max(zeros)
            # for i in range(maxzero+1):
            #     for coef, sol in zip(coefs, sols):
            #         if len(coefs2) == len(self.refs):
            #             break
            #         if coef.count(0) == i:
            #             coefs2.append(coef)
            #             sols2.append(sol)
            listdic = []
            for n, coef in enumerate(coefs):
                found = False
                # take all the indices that are equal to zero in equation n of coefs
                zero_indices = [i for i, c in enumerate(coef) if c == 0]
                # check if it is equal to an already parsed equation to group it
                for ld in listdic:
                    # check if there is an element if listdic with equal number of zeros
                    if ld["nzeros"] == len(zero_indices):
                        # print("sii mismo numero de zeros")
                        # check whether the indices equal to zero are the same
                        # if so, the two equations are equal
                        intersection = ld["zindices"].intersection(zero_indices)
                        if len(intersection) == len(zero_indices):
                            # print("siii, los zeronidis son iguales")
                            # add the index of the equation that is equal to the corresponding
                            # dictionaty in listdic
                            ld["coefindex"].append(n)
                            found = True
                            break
                if not found:
                    listdic.append({"nzeros":len(zero_indices), "zindices":set(zero_indices), "coefindex":[n]})
                    # print(type(listdic[-1]["zindices"]), listdic[-1]["zindices"])
            # print(len(listdic), listdic)
            if len(listdic) >= len(self.refs):
                for i in range(len(self.refs)):
                    index = listdic[i]["coefindex"][0]
                    coefs2.append(coefs[index])
                    sols2.append(sols[index])
            else:
                raise IndexError("There are not enough independent equations")
        else:
            coefs2 = coefs
            sols2 = sols
        # print(coefs2, sols2)

        # check if the dimensions of the equations equals
        # the number of variables
        if len(coefs2[0]) > len(sols2):
            zeroindices = [i for coefi in coefs2
                           for i, c in enumerate(coefi)
                           if c == 0]
            unique, count = np.unique(zeroindices, return_counts=True)

            zeroindices = [u for u, c in zip(unique, count)
                           if c == len(coefs)]

            for i, c in enumerate(coefs2):
                coefs2[i] = [coefi for n, coefi in enumerate(coefs2[i])
                            if n not in zeroindices]

        final_coefs = list(np.linalg.solve(coefs2, sols2))
        if np.any(final_coefs) < 0:
            raise ValueError("There are negative coefficients !")

        self.coefs = final_coefs[:]

        # verify whether legth of coefs is smaller than len of refs
        # every reference species need to have a coeficient which is
        # zero if it was not considered in the calculation of the ref
        for index in zeroindices:
            self.coefs.insert(index, 0)
        print("Final Coefficients for reference ", self.coefs)

    def update(self):
        self.set_e()
        self.update_thermo()

    def add_itm(self, itm):
        # aqui tengo que verficar si es un objeto itm
        self.refs.append(itm)
        self.update()

    def update_temps(self):
        # This method searches the temperatues of all basis
        # intermediates and the temps of parent_item
        # and keeps only the temperature that are common to
        # all : reference basis set and parent itm
        # So that the relative values (G, H) are calculates for
        # the matched temperatures
        self.temps = []
        for ref in self.refs:
            self.temps += ref.temps
        self.temps += self.parent_itm.temps
        unique, counts = np.unique(self.temps, return_counts=True)
        self.temps = [unique[n] for n, c in enumerate(counts)
                      if c == len(self.refs) + 1]
        self.temps.sort()

    def set_e(self):
        self.abs_en = 0.0
        self.abs_zpe = 0.0
        calc_zpe = True
        for n, ref in enumerate(self.refs):
            self.abs_en += ref.energy * self.coefs[n]
            if ref.zpe == 0.0:
                calc_zpe = False
                self.abs_zpe = 0.0
                continue
            if calc_zpe:
                self.abs_zpe += ref.zpe * self.coefs[n]

        self.energy = self.parent_itm.energy - self.abs_en
        if calc_zpe and self.parent_itm.zpe != 0.0:
            self.zpe = self.parent_itm.zpe - self.abs_zpe

    def update_thermo(self, *args):
        self.thermo["g"] = {}
        self.thermo["h"] = {}
        self.thermo["stot"] = {}
        self.thermo["qtot"] = {}
        self.thermo["k"] = {}
        self.thermo["kq"] = {}
        self.thermo["a"] = {}
        self.update_temps()

        keys = ["g", "h", "stot", "qtot"]

        for T in self.temps:
            self.thermo["k"][T] = 0.0
            self.thermo["kq"][T] = 0.0
            self.thermo["a"][T] = 0.0
            for key in keys:
                absp = 0  # sum of absolute props from references
                for n, ref in enumerate(self.refs):
                    absp += ref.thermo[key][T] * self.coefs[n]
                relp = self.parent_itm.thermo[key][T] - absp
                self.thermo[key][T] = relp

    def get_refitm_index(self, name):
        for n, refob in enumerate(self.refs):
            if refob.name == name:
                return n
        return None

    def g(self, T):
        return self.thermo["g"].get(T)

    def h(self, T):
        return self.thermo["h"][T]

    def stot(self, T):
        return self.thermo["stot"][T]

    def k(self, T):
        return self.thermo["k"][T]

    def kq(self, T):
        return self.thermo["kq"][T]

    def a(self, T):
        return self.thermo["a"][T]

    def qtot(self, T):
        return self.thermo["qtot"][T]

    def tkey(self, k, T):
        return self.thermo[k][T]


class Reac(object):
    def __init__(self, parent_itm, name, ref, *args, **kwargs):
        if not isinstance(ref, Itm):
            raise TypeError("Expected type item class for reference argument")

        self.parent_itm = parent_itm
        self.name = name
        self.temps = []
        # refs are itm object that serve as referece
        self.ref = ref
        # energy relative to parent itm
        self.energy = 0.0
        # absolute energy calculated from self.coefs
        self.abs_en = 0.0
        self.abs_zpe = 0.0
        self.zpe = 0.0

        self.thermo = {
            "g": {}, "h": {}, "stot": {},
            "qtot": {}, "k": {}, "kq": {},
            "a":{}, "ae":{}
        }

        if "relref" in kwargs and kwargs["relref"] != "":
            self.relref = kwargs["relref"]
            self.parent_itm_ref = self.parent_itm.get_ref(self.relref)
            self.ref_ref = self.ref.get_ref(self.relref)
            # print("relrel: ", self.relref)
            # print("parent itm ref: ", str(self.parent_itm.get_ref(self.relref)))
            # print("ref ref: ", str(self.ref.get_ref(self.relref)))
            if self.ref_ref is None or self.parent_itm_ref is None:
                self.relref = ""
                self.parent_itm_ref = None
                self.ref_ref = None
                warningtext = "WARNING".join([" - " for i in range(10)])
                print(warningtext)
                print("Tried to set self.ref_ref and self.parent_irm_ref but could not")
                # print("ref: ", self.name, "parent itm name: ", self.parent_itm.name, "ref name: ", self.ref.name)
                print(warningtext)

        else:
            self.relref = ""
            self.parent_itm_ref = None
            self.ref_ref = None

        self.update()

    def to_dict(self):
        selfdict = OrderedDict()
        selfdict["parent_itm"] = self.parent_itm.name
        selfdict["ref"] = self.ref.name
        selfdict["name"] = self.name
        selfdict["thermo"] = self.thermo
        selfdict["relref"] = self.relref
        return selfdict

    def update(self):
        self.set_e()
        self.update_thermo()

    def update_temps(self):
        # This method searches the temperatues of all basis
        # intermediates and the temps of parent_item
        # and keeps only the temperature that are common to
        # all : reference basis set and parent itm
        # So that the relative values (G, H) are calculates for
        # the matched temperatures
        self.temps = []

        if self.relref:
            self.temps += self.ref_ref.temps
            self.temps += self.parent_itm_ref.temps
        else:
            self.temps += self.ref.temps
            self.temps += self.parent_itm.temps
        unique, counts = np.unique(self.temps, return_counts=True)
        self.temps = [unique[n] for n, c in enumerate(counts)
                      if c == 2]
        self.temps.sort()

    def set_e(self):
        self.abs_en = 0.0
        self.abs_zpe = 0.0
        calc_zpe = True
        if self.relref:
            self.abs_en = self.ref_ref.energy
            self.energy = self.parent_itm_ref.energy - self.abs_en

            self.abs_zpe = self.ref_ref.zpe
            if self.parent_itm_ref.zpe != 0 and self.abs_zpe != 0:
                self.zpe = self.parent_itm_ref.zpe - self.abs_zpe
            else:
                self.zpe = 0.0
        else:
            self.abs_en = self.ref.energy
            self.energy = self.parent_itm.energy - self.abs_en
            self.abs_zpe = self.ref.zpe

            if self.abs_zpe != 0 and self.parent_itm.zpe != 0:
                self.zpe = self.parent_itm.zpe - self.abs_zpe
            else:
                self.zpe = 0.0

    def update_thermo(self, *args):  # Cálculo de constante MECP
        u = self.parent_itm.mech.unit
        self.thermo["g"] = {}
        self.thermo["h"] = {}
        self.thermo["stot"] = {}
        self.thermo["qtot"] = {}
        self.thermo["k"] = {}
        self.thermo["kq"] = {}
        self.thermo["a"] = {}
        self.thermo["ae"] = {}
        self.update_temps()

        keys = ["g", "h", "stot", "qtot"]

        for T in self.temps:
            self.thermo["k"][T] = 0.0
            self.thermo["kq"][T] = 0.0
            self.thermo["a"][T] = 0.0
            self.thermo["ae"][T] = 0.0
            for key in keys:
                if self.relref:
                    relp = self.parent_itm_ref.tkey(key, T) - self.ref_ref.tkey(key, T)
                    self.thermo[key][T] = relp
                else:
                    relp = self.parent_itm.tkey(key, T) - self.ref.tkey(key, T)
                    self.thermo[key][T] = relp

            if self.parent_itm.tp == "ts":
                try:
                    self.thermo["k"][T] = (Kb[u] * T / h[u]) * (np.e**(-self.thermo["g"][T] / (R[u] * T)))
                except Exception as e:
                    print("There was an error computing K", e)
                try:
                    # No se porque hay un e multiplicando lo que deberia ser KbT/h * exp^R/T
                    self.thermo["a"][T] = (np.e * Kb[u] * T / h[u])*(np.e**(self.thermo["stot"][T]/R[u])) 
                except Exception as e:
                    print("There was an error computing A", e)
                try:
                    self.thermo["ae"][T] = self.thermo["h"][T] + R[u]*T
                except Exception as e:
                    print("There was an error computing Ae", e)

                try:
                    self.thermo["kq"][T] = \
                        (self.parent_itm.qtot(T) / self.ref.qtot(T)) * \
                        (Kb[u] * T / h[u]) * \
                        np.e ** (-(self.thermo["h"][T] + R[u]*T)/ (R[u] * T))
                except Exception as e:
                    print("There was an error computing Kq. Not very important", e)
      
            elif self.parent_itm.tp == "mecp":
                try:
                    print(self.parent_itm, self.parent_itm.SOC, self.parent_itm.diffgrad, self.parent_itm.redmass)
                    print("prefactor data", (Kb[u] * T / h[u]))
                    if self.thermo["g"][T] < 0:
                        print("WARNING: Negative G value for MECP rate calculation")
                        self.thermo["k"][T] = landau_zener_rate(0.0, self.parent_itm.SOC, self.parent_itm.diffgrad, self.parent_itm.redmass, T, u, model='double')
                    else:
                        self.thermo["k"][T] = landau_zener_rate(self.thermo["g"][T], self.parent_itm.SOC, self.parent_itm.diffgrad, self.parent_itm.redmass, T, u, model='double')
                    #self.thermo["k"][T] = (Kb[u] * T / h[u]) * (np.e**(-self.thermo["g"][T] / (R[u] * T))) #* 
                except Exception as e:
                    print("There was an error computing K", e)
                try:
                    # No se porque hay un e multiplicando lo que deberia ser KbT/h * exp^R/T
                    self.thermo["a"][T] = (np.e * Kb[u] * T / h[u])*(np.e**(self.thermo["stot"][T]/R[u])) 
                except Exception as e:
                    print("There was an error computing A", e)
                try:
                    self.thermo["ae"][T] = self.thermo["h"][T] + R[u]*T
                except Exception as e:
                    print("There was an error computing Ae", e)


            elif self.parent_itm.tp == "min":
                self.thermo["k"][T] = np.e ** (-self.thermo["g"][T]/ (R[u] * T))
                self.thermo["kq"][T] = 0.0

    def g(self, T):
        return self.thermo["g"].get(T)

    def h(self, T):
        return self.thermo["h"][T]

    def stot(self, T):
        return self.thermo["stot"][T]

    def k(self, T):
        return self.thermo["k"][T]

    def kq(self, T):
        return self.thermo["kq"][T]

    def a(self, T):
        return self.thermo["a"][T]

    def qtot(self, T):
        return self.thermo["qtot"][T]

    def tkey(self, k, T):
        return self.thermo[k][T]

    def tkeys(self, k):
        return [self.thermo[k][T] for T in self.temps]

    def get_kinetic_temp_dependency(self, trange=[]):
        from matplotlib import pyplot as plt

        def depen_func(T, a1, a2, a3, a4, a5, a6, a7):
            return a1*np.log(T)+a2/T+a3+a4*T \
                           +a5*T**2+a6*T**3+a7*T**4
            # return a1 + a2*T + a3*T**2 + a4*T**3

        As = self.tkeys("a")
        # As = self.tkeys("ae")
        if trange:
            temps = []
            final_As = []
            for i, t in enumerate(self.temps):
                if t >= trange[0] and t <= trange[1]:
                    temps.append(t)
                    final_As.append(As[i])
        else:
            temps = self.temps
            final_As = As

        params, params_covar = optimize.curve_fit(depen_func, temps, final_As)
        print(params)
        plt.plot(temps, final_As, "*")
        predAs = []
        for T in self.temps:
            predAs.append(params[0]*np.log(T)+params[1]/T+params[2]+params[3]*T+params[4]*T**2+params[5]*T**3+params[6]*T**4)
        plt.plot(self.temps, predAs, "-")
        plt.show()

class State_Conv(Reac):
    def __init__(self, parent_itm, name, ref, df_value, process_type, *args, **kwargs):
        super().__init__(parent_itm, name, ref, *args, **kwargs)
        #self.parent_itm = parent_itm
        #self.name = name
        self.temps = []
        print("Parent_itm is: ", parent_itm.name)
        # refs are itm object that serve as referece
        self.ref = ref
        # energy relative to parent itm
        self.energy = 0.0
        # absolute energy calculated from self.coefs
        self.abs_en = 0.0
        self.abs_zpe = 0.0
        self.zpe = 0.0
        self.process_type = process_type
        self.egap = 0.0

        self.thermo = {
            "g": {}, "h": {}, "stot": {},
            "qtot": {}, "k": {}, "kq": {},
            "a":{}, "ae":{}, "k(0)": {}
        }

        # State_Conv inherits from Reac class, but changes the way rate constants are calculated
        print("Creating State Conversion process: ", name, " of type ", process_type)
        self.df_mag = df_value  # Driving force magnitude for the process (in eV)
        possible_processes = ["NR_decay", "RISC", "IC"]
        if process_type not in possible_processes:
            raise ValueError("process_type must be one of the following: "
                             + ", ".join(possible_processes))
        else:
            self.process_type = process_type

        # Define driving force attribute
        if self.process_type == "NR_decay" or self.process_type == "RISC":
            self.HR_factor = 0.1  # Huang-Rhys factor for the process (Weak coupling limit)
            self.max_freq_GS = None # Maximum vibrational frequency of the ground state (in cm-1)

        self.update2()

    def update2(self):
        self.update_thermo_2()

    def set_driving_force(self, df): # Establece el driving force para el proceso de conversion de estado
        self.driving_force = df

    def update_thermo_2(self, *args):  # Cálculo de constantes no radiativas
        u = self.parent_itm.mech.unit
        self.thermo["g"] = {}
        self.thermo["h"] = {}
        self.thermo["stot"] = {}
        self.thermo["qtot"] = {}
        self.thermo["k"] = {}
        self.thermo["kq"] = {}
        self.thermo["a"] = {}
        self.thermo["ae"] = {}
        self.thermo["k(0)"] = {}
        self.thermo["k(T)"] = {}
        self.thermo["Egap"] = {}

        self.update_temps()

        keys = ["g", "h", "stot", "qtot"]

        for T in self.temps:
            self.thermo["k"][T] = 0.0
            self.thermo["kq"][T] = 0.0
            self.thermo["a"][T] = 0.0
            self.thermo["ae"][T] = 0.0
            self.thermo["k(0)"][T] = 0.0
            for key in keys:
                if self.relref:
                    relp = self.parent_itm_ref.tkey(key, T) - self.ref_ref.tkey(key, T)
                    self.thermo[key][T] = relp
                else:
                    relp = self.parent_itm.tkey(key, T) - self.ref.tkey(key, T)
                    self.thermo[key][T] = relp

            if self.parent_itm.tp == "min":
                if self.process_type == "NR_decay" or self.process_type == "RISC":
                    try:
                        if self.parent_itm.energy < self.energy:
                            lower_state = self.parent_itm
                            upper_state = self.ref
                        else:
                            lower_state = self.ref
                            upper_state = self.parent_itm
                    except:
                        lower_state = None
                        upper_state = None
                    print(lower_state.name, upper_state.name)
                    # Get higher vibration of lower state
                    if lower_state.freq_is_set:
                        try:
                            freqs_lower = lower_state.freqs.vibs
                            if lower_state.freqs.unit == "cm-1":
                                max_freq_lower = max(freqs_lower)
                            elif lower_state.freqs.unit == "eV":
                                max_freq_lower = max(freqs_lower) * 8065.54  # Convert eV to cm-1
                            else:
                                raise ValueError("Unknown frequency unit for lower state")
                        except:
                            max_freq_lower = None
                    # Calculate gamma:
                    if self.HR_factor and max_freq_lower:
                        self.max_freq_GS = max_freq_lower
                        gamma = np.log((upper_state.energy - lower_state.energy)*conv[u]["cm1"]/(self.HR_factor * self.max_freq_GS)) - 1  # unitless
                        en_gap = (upper_state.energy - lower_state.energy)*conv[u]["cm1"]
                        self.thermo["Egap"][T] = en_gap
                    if self.process_type == "NR_decay":
                        exp_factor = 1.0
                    elif self.process_type == "RISC":
                        try:
                            # Calculate exponential factor
                            exp_factor = np.e**(-abs(self.thermo["g"][T]) / (R[u] * T))
                        except Exception as e:
                            print("There was an error computing exponential factor for RISC", e)
                            exp_factor = 1.0
                    # Calclation of phonon count
                    try: 
                        p = en_gap / self.max_freq_GS
                        if p > 5:
                            print("WARNING: Phonon count is high (p=", p, "). Non-radiative rate approaches zero. Temperature dependance has a large error!")
                    except Exception as e:
                        p = None
                    # Final calculation of non-radiative rate at 0 K:
                    try:
                        #print("Driving force magnitude (|ΔE|) for non-radiative decay: ", self.df_mag)
                        preexponential_factor = ((self.df_mag**2) * np.sqrt(2*np.pi)) / (hbar["cm1"] * np.sqrt(self.max_freq_GS * en_gap))
                        #print("Pre-exponential factor for non-radiative decay: ", preexponential_factor)
                        #print("Calculating exponential factor with gamma: ", gamma, " and energy gap: ", en_gap)
                        #print("exponential factor for non-radiative decay: ", (np.e**((-gamma*en_gap)/self.max_freq_GS)))
                        k0 = (np.e**((-gamma*en_gap)/self.max_freq_GS)) * preexponential_factor * exp_factor # Temperature dependence from classical movement of nuclei but not from excited vibrational levels.
                    except Exception as e:
                        #print("There was an error computing preexponential factor", e)
                        k0 = 0.0
                    self.thermo["k(0)"][T] = k0
                    # Final calculation of non-radiative rate at temperature T:
                    try:
                        x = self.max_freq_GS / (2 * R["cm1"] * T)
                        temp_factor = (coth(x))**p
                        self.thermo["k(T)"][T] = k0 * temp_factor
                    except Exception as e:
                        print("There was an error computing temperature factor", e)
                        self.thermo["k(T)"][T] = k0

                        
                    try:

                        # Calculate non-radiative decay rate in weak coupling limit (Englman and Jortner model)
                        # Antes habría que coger el oscilador de la vibración más alta del estado fundamental
                        lambda_reorg = 0.2  # Reorganization energy in eV (example value)
                        delta_G = self.df_mag  # Driving force in eV
                        #k_nr = (2 * np.pi / h[u]) * (self.HR_factor * self.max_freq_GS * 1.23981e-4) *
                    except:
                        pass
                try:
                    self.thermo["k"][T] = (Kb[u] * T / h[u]) * (np.e**(-self.thermo["g"][T] / (R[u] * T)))
                except Exception as e:
                    print("There was an error computing K", e)


      
            else:
                # Non-radiative processes are only defined for minima
                pass

    def k0(self, T):
        return self.thermo["k(0)"][T]
    def kT(self, T):
        return self.thermo["k(T)"][T]
    def egap(self, T):
        return self.thermo["Egap"][T]
    

class Itm(object):
    def __init__(self, name, *args, **kwargs):

        self.name = name
        self.itms = []
        self.refs = OrderedDict()
        self.reacs = OrderedDict()
        self.states = OrderedDict() # Excited states

        self.tp = None

        self.picture = {}

        if "name_font" in kwargs:
            self.name_font = kwargs["name_font"]
        else:
            self.name_font = "Arial,11,-1,0,100,0,0,0,0,0,Regular"  # family, psize, ?, ?, bold,italic,underline,stroke,?,?

        if "energy_font" in kwargs:
            self.energy_font = kwargs["energy_font"]
        else:
            self.energy_font = ""

        if "energy" in kwargs:
            self.energy = kwargs["energy"]
        else:
            self.energy = 0.0

        if "zpe" in kwargs:
            self.zpe = kwargs["zpe"]
        else:
            self.zpe = 0.0

        if "cm" in kwargs:
            self.cm = kwargs["cm"]
        else:
            self.cm = ""

        if "spin" in kwargs:
            self.spin = kwargs["spin"]
        else:
            self.spin = 0

        if "mech" in kwargs:
            self.mech = kwargs["mech"]
        else:
            self.mech = None

        if "temps" in kwargs:
            self.temps = kwargs["temps"]
        else:
            self.temps = []

        if "pg" in kwargs:
            self.pg = kwargs["pg"]
        else:
            self.pg = None

        if "tp" in kwargs:
            self.set_tp(kwargs["tp"])

        if "freq_is_set" in kwargs:
            self.freq_is_set = kwargs["freq_is_set"]
        else:
            self.freq_is_set = False

        if "photos" in kwargs:
            self.photos = kwargs["photos"]
        else:
            self.photos = []

        if "struct" in kwargs:
            self.struct = Struct(sdict=kwargs["struct"])
        else:
            self.struct = Struct()

        if "freqs" in kwargs:
            self.freqs = Freq(fdict=kwargs["freqs"])
            self.freq_is_set = True
        else:
            self.freqs = Freq()

        if "thermo" in kwargs:
            self.thermo = kwargs["thermo"]
        else:
            # dictionary with the relative energies at
            # at the temperatues matched by the basis set and parent intermediate
            self.thermo = {
                "g": {}, "h": {}, "stot": {},
                "svib": {}, "strans": {}, "srot": {}, "select":{},
                "etot": {}, "evib": {}, "etrans": {},
                "erot": {}, "qtot": {}, "qrot": {}, "qtrans": {},
                "qvib": {}, "qvibg":{}, "qelect": {}
            }

        if "merged" in kwargs and kwargs["merged"]:
            try:
                self.add_itms(kwargs["itms"])
                self.merged = kwargs["merged"]
            except TypeError:
                self.merged = False
        else:
            self.merged = False

        # MECP specific variables
        if "redmass" in kwargs:
            self.redmass = kwargs["redmass"]
        else:
            self.redmass = None

        if "SOC" in kwargs:
            self.SOC = kwargs["SOC"]
        else:
            self.SOC = None

        if "grad" in kwargs:
            self.grad = kwargs["grad"]
        else:
            self.grad = None

        if "diffgrad" in kwargs:
            self.diffgrad = kwargs["diffgrad"]
        else:
            self.diffgrad = None


    def to_dict(self):
        selfdict = OrderedDict()
        selfdict["name"] = self.name
        selfdict["energy"] = self.energy
        selfdict["zpe"] = self.zpe
        selfdict["tp"] = self.tp
        selfdict["cm"] = self.cm
        selfdict["mech"] = self.mech.name
        selfdict["temps"] = self.temps
        selfdict["thermo"] = self.thermo
        selfdict["struct"] = self.struct.to_dit()
        selfdict["refs"] = [ref.to_dict() for ref in self.refs.values()]
        selfdict["reacs"] = [reac.to_dict() for reac in self.reacs.values()]
        selfdict["states"] = [state.to_dict() for state in self.states.values()]
        selfdict["itms"] = [itm.name for itm in self.itms]
        selfdict["merged"] = self.merged
        selfdict["freqs"] = self.freqs.to_dict()
        selfdict["freq_is_set"] = self.freq_is_set
        selfdict["pg"] = self.pg
        selfdict["spin"] = self.spin
        selfdict["photos"] = self.photos
        if self.tp == "mecp":
            selfdict["redmass"] = self.redmass
            selfdict["SOC"] = self.SOC
            selfdict["grad"] = self.grad
            selfdict["diffgrad"] = self.diffgrad
        return selfdict

    def set_tp(self, tp):
        if tp in ["min", "ts", "ref", "mecp"]:
            self.tp = tp
        else:
            self.tp = None

    def full_update(self):
        self.update_rel_thermo()

    def update_data_from_itm(self, itm):
        for k in ["energy", "tp", "cm", "struct", "freqs", "pg", "spin", "redmass", "SOC", "grad", "diffgrad"]:
            value = getattr(itm, k)
            if value is not None:
                setattr(self, k, value)
        return
    
    def update_merged_data(self):
        for itm in self.itms:
            self.energy += itm.energy
            self.zpe += itm.zpe
            for k in self.thermo.keys():
                for T in itm.thermo[k].keys():
                    if T not in self.thermo[k]:
                        self.thermo[k][T] = 0.0
                    self.thermo[k][T] += itm.thermo[k][T]
        self.update_rel_thermo()
        return

    def del_thermo(self):
        self.thermo = {
            "g": {}, "h": {}, "stot": {},
            "svib": {}, "strans": {}, "srot": {}, "select":{},
            "etot": {}, "evib": {}, "etrans": {},
            "erot": {}, "qtot": {}, "qrot": {}, "qtrans": {},
            "qvib": {},  "qvibg": {}, "qelect": {}
        }
        self.update_rel_thermo()

    def add_itms(self, itms):
        if not isinstance(itms, list):
            raise TypeError("Expected type list for argument items in add_items")

        for itm in itms:
            if isinstance(itm, str):
                itmobj = self.mech.get_itm(itm)
                if itmobj is None:
                    raise TypeError("Received None type instead of type item")
                self.itms.append(itmobj)
            elif isinstance(itm, Itm):
                self.itms.append(itm)
            else:
                raise TypeError("Unkown type for self.itms in item instance")

    def set_temps(self):
        # verficar a ver si esto necesita mas codigo
        self.temps = list(self.thermo["g"].keys())

    def update_rel_thermo(self):
        """ method to update the relative energy and free energy of
            references and reaction intermediates"""
        self.set_temps()
        for ref in self.refs.values():
            ref.update()
        for reac in self.reacs.values():
            reac.update()
        for state in self.states.values():
            state.update2()

    def photo_from_files(self, photonames):
        databasedir = os.environ["HOME"] + "/.datamanager/photos/"
        try:
            os.makedirs(databasedir)
        except Exception as e:
            print(e)

        for photo in photonames:
            n = str(len(self.photos))  # number of photos to index the next one
            newname = self.mech.name + "_" + self.name + "_" + n \
                      + time.ctime().replace(" ", "_") + ".png"
            newname = newname.replace(":", "_")
            # copy file to home/user/.datamanager/photos
            shutil.copy(photo, databasedir + newname)
            self.photos.append(newname)

    def struct_from_file(self, infile):
        """
        :param infile:
        :return bool:
        Si no hay nada que leer no da error, simplemente initializa las
        variables en cero
        """
        nat, labels, coors, celda = 0, [], [], []
        if infile.endswith(".xyz"):
            nat, labels, coors = read_xyz(infile)
            if nat and labels and coors:
                self.struct.update(nat=nat, labels=labels, coors=coors)
        elif "POSCAR" in infile or "CONTCAR" in infile:
            nat, labels, coors, celda = read_poscar(infile)
            if nat and labels and coors:
                self.struct.update(nat=nat, labels=labels, coors=coors, cell=celda)
        if not coors:
            self.cm += "ERROR REDING STRUCT FROM "+ infile+"\n"
            return False
        else:
            return True

    def set_atoms(self, labels, numbers):
        nat = 0
        # esta funcion la tengo que quitar de aqui
        if len(labels) != len(numbers):
            raise Exception("Length of labels and number do not match in set_atoms in item")

        self.struct.atoms = OrderedDict()
        for l, n in zip(labels, numbers):
            nat += n
            self.struct.atoms[l] = n
        self.struct.nat = nat

    def merge_structs(self, itmobjs=[]):
        if not itmobjs:
            itmobjs = self.get_itms()
        labels, numbers = [], []
        for itm in itmobjs:
            if not isinstance(itm, Itm):
                raise TypeError("Expected list of item type")
            for label, number in itm.struct.atoms.items():
                if label not in labels:
                    labels.append(label)
                    numbers.append(number)
                else:
                    index = labels.index(label)
                    numbers[index] += number
        self.set_atoms(labels, numbers)

    def set_comment(self, cm):
        if type(cm) != str:
            return False
        self.cm = cm
        return True

    def set_e(self, e):
        try:
            e = float(e)
        except ValueError:
            return False
        else:
            self.energy = e
            for ref in self.refs.values():
                ref.update()
            for reac in self.reacs.values():
                reac.update()
            return True

    def set_pg(self, pg):
        self.pg = pg

    def add_ref(self, name, itms, coefs=[]):
        """
        :param name: is the one of the reference
        :param itms: is the list of itm objects that will be the basis of the reference
        :param coefs: the list of coefficients, one for every element of the list of itms
        :return: the reference obj if it is created, None otherwise
        """
        if name is None:
            name = "ref_" + len(self.refs)

        if not isinstance(itms, list):
            raise Exception("Expected list for argument "
                            " items in add_ref in class item")

        if name not in self.refs:
            self.refs[name] = Reference(self, name, itms, coefs)
            return self.refs[name]
        else:
            print(f"refname {name} found in self.refs ", self.name) 
            return None

    def get_refs(self):
        return list(self.refs.values())

    def get_refs_names(self):
        return list(self.refs.keys())

    def get_ref(self, ref)-> list | None:
        if ref in self.refs:
            return self.refs[ref]
        else:
            return None

    def mech(self):
        return self.mech

    def del_ref(self, ref):
        if type(ref) == str:
            if ref in self.refs.keys():
                del self.refs[ref]
                return True
            else:
                return False
        elif isinstance(ref, Reference):
            if ref in self.refs.values():
                del self.refs[ref.name]
                return True
            else:
                return False

    def add_reac(self, reac, relref=""):
        # reac is the itm object that serve
        # as reaction reference
        if reac.name not in self.reacs:
            self.reacs[reac.name] = Reac(self,
                                         reac.name,
                                         reac,
                                         relref=relref)
            return True
        return False
    

    def add_excited_state(self, state, proc_type, df_value, relref=""):
        # state is the itm object that serve
        # as reaction reference
        state.name = state.name + "_" + proc_type
        if state.name not in self.states:
            self.states[state.name] = State_Conv(self,
                                         state.name,
                                         state,
                                         relref=relref,
                                         df_value=df_value,
                                         process_type = proc_type)
            return True
        return False

    def get_reacs(self):
        return list(self.reacs.values())

    def get_excited_states(self):
        print(self.states)
        return list(self.states.values())

    def get_reac(self, reac):
        if reac in self.reacs:
            return self.reacs[reac]
        else:
            return None

    def del_reac(self, reac):
        # Parece que se está ejecutando dos veces, en otra parte del código
        if type(reac) == str and reac in self.reacs: 
            del self.reacs[reac]
        elif isinstance(reac, Reac) and reac in self.reacs:
            del self.reacs[reac.name]

    def set_freqs(self, freqlist):
        if freqlist is None:
            self.freq_is_set = False
            return

        vibs = []
        for i in freqlist:
            if i == "cm-1":
                self.freqs.unit = "cm-1"
            else:
                vibs.append(i)
        self.freqs.set_vibs(vibs)

        self.freq_is_set = True

    def freqs_from_mfreq(self, f, format=""):
        if "MECP.info.molden" in f:
            try:
                vibs, unit = read_mfreq(f)
            except Exception as e:
                self.cm += "COULD NOT FIND VIBS IN FILE: " + f + "\n"
                return False
            else:
                self.freqs.set_vibs(vibs[:])
                self.freqs.unit = unit
                return True

    def freqs_from_file(self, f, format=""):
        if format == "vasp" or "OUTCAR" in f:
            try:
                vibs, unit = read_outcar_freqs(f)
            except Exception as e:
                self.cm += "COULD NOT FIND VIBS IN FILE: " + f + "\n"
                return False
            else:
                self.freqs.set_vibs(vibs[:])
                self.freqs.unit = unit
                return True
        elif format == "gaussian":
            vibs = read_freqs_gaussian(f)
            if not vibs:
                self.cm += "COULD NOT FIND VIBS IN FILE: " + f + "\n"
                return False
            self.freqs.set_vibs(vibs[:])
            self.freqs.unit = "cm-1"
            return True
        elif "job.out" in f:
            vibs = read_orca_freqs(f)
            if not vibs:
                self.cm += "COULD NOT FIND VIBS IN FILE: " + f + "\n"
                return False
            self.freqs.set_vibs(vibs[:])
            self.freqs.unit = "cm-1"
            return True
        else:
            return False
    
    def pg_from_orca_file(self, infile):
        pg = None
        if "job.out" in infile:
            pg = read_orca_pg(infile)
        if pg is None:
            self.cm += "COULD NOT FIND PG IN FILE: " + infile + "\n"
            return False
        else:
            self.pg = pg
            return True

    def g(self, T):
        return self.thermo["g"][T]

    def h(self, T):
        return self.thermo["h"][T]

    def stot(self, T):
        return self.thermo["stot"][T]

    def qtot(self, T):
        return self.thermo["qtot"][T]

    def tkey(self, k, T):
        return self.thermo[k][T]

    def add_default_reference(self):
        if "Def Ref" not in self.refs:
            # print(self.mech.name, self.mech.def_basis_ref)

            if self.mech.def_basis_ref:
                self.refs["Def Ref"] = Reference(self, "Def Ref",
                                                 self.mech.def_basis_ref)
            return True
        else:
            return False

    def get_def_ref_energy(self):
        if "Def Ref" in self.refs:
            return self.refs["Def Ref"].energy
        else:
            return 0.0



    def calc_thermo(self, temps=[], modes=0, P=1.0, V=0, nnfreq=False):
        if not temps:
            return
        if V != 0:
            iprecalc = False
        else:
            iv = None
            iprecalc = True
        #print("Calculating thermo for item ", self.name)
        if self.merged:
            for itm in self.get_itms():
                if itm.freqs.vibs:
                    if itm.tp == "ts":
                        freqs = itm.freqs.vibs[:-1]
                        if modes == 0:
                            modes = len(freqs)
                    elif itm.tp == "mecp":
                        freqs = itm.freqs.vibs[:-2]  # 3N - 2 for MECP                    
                    else: 
                        freqs = itm.freqs.vibs[:]
                else:
                    freqs = []

                E = itm.energy
                unit = self.mech.unit
                pg = itm.pg
                Thermo.xyz_setup(itm.struct.labels, itm.struct.coors)
                Thermo.set_input(temps, ipg=pg, unit=unit, imodes=modes,
                                 iprecalc=iprecalc, ip=P, iv=V, ispin=True,
                                 ispin_mult=itm.spin)
                Thermo.set_freq(freqs, E, nnfreq=nnfreq)
                zpe, thermo = Thermo.calc_all()
                for prop, dictT in thermo.items():
                    for T, value in dictT.items():
                        itm.thermo[prop][T] = value
                itm.zpe = zpe
                itm.update_rel_thermo()
            self.recalc_thermo_from_itms()
        else:
            if self.freqs.vibs:
                #print("Calculating thermo for item ", self.name)
                if self.tp == "ts":
                    freqs = self.freqs.vibs[:-1]
                    if modes == 0:
                        modes = len(freqs)
                elif self.tp == "mecp":
                    freqs = self.freqs.vibs[:-2] # 3N - 2 for MECP 
                    #if modes == 0:
                    #    modes = len(freqs)
                else:
                    freqs = self.freqs.vibs[:]
            else:
                freqs = []

            E = self.energy
            unit = self.mech.unit
            pg = self.pg
            Thermo.xyz_setup(self.struct.labels, self.struct.coors)
            Thermo.set_input(temps, ipg=pg, unit=unit, imodes=modes,
                             iprecalc=iprecalc, ip=P, iv=V, ispin=True,
                                 ispin_mult=self.spin)
            Thermo.set_freq(freqs, E, nnfreq=nnfreq)
            zpe, thermo = Thermo.calc_all()
            for prop, dictT in thermo.items():
                for T, value in dictT.items():
                    self.thermo[prop][T] = value
            self.zpe = zpe

        self.update_rel_thermo()

    def recalc_thermo_from_itms(self):
        commontemps = [T for i in self.get_itms() for T in i.temps]
        unique, counts = np.unique(commontemps, return_counts=True)
        commontemps = [unique[n] for n, c in enumerate(counts)
                       if c == len(self.get_itms())]
        for prop in self.thermo:
            for T in commontemps:
                self.thermo[prop][T] = 0
                for itm in self.get_itms():
                    self.thermo[prop][T] += itm.tkey(prop, T)

    def get_itms(self):
        return self.itms

    def scale_freqs(self, coeff):
        if not isinstance(coeff, float):
            raise ValueError("Expected float in scale frequencies method")

        self.freqs.vibs = [vib*coeff for vib in self.freqs.vibs]
        # self.cm += "\nLas freqs han sido reescaladas con coeff = " + str(coeff) + "\n"
        self.cm += "\nThe frequencies have been rescaled with coefficient = " + str(coeff) + "\n"


class Mechanism(object):
    def __init__(self, name, *args, **kwargs):
        self.name = name
        # every element of the dictionary is a list of Itm objects
        self.refs = {}
        # every element of the dict is an Itm object
        self.itms = OrderedDict()
        # list of Itm objects, same nature of the elements of
        # the self.refs attribute
        self.def_basis_ref = []

        if "unit" in kwargs:
            self.unit = kwargs["unit"]
        else:
            self.unit = "eV"

        if "def_basis_ref" in kwargs:
            self.set_def_basis_ref(kwargs["def_basis_ref"])

        if "refs" in kwargs:
            self.add_refs(kwargs["refs"])

    def to_dict(self):
        selfdict = OrderedDict()
        selfdict["name"] = self.name
        selfdict["unit"] = self.unit
        selfdict["def_basis_ref"] = [ref.name for ref in self.def_basis_ref]
        selfdict["refs"] = dict([(refname, [obj.name for obj in objlist]) for refname, objlist in self.refs.items()])
        selfdict["itms"] = [itm.to_dict() for itm in self.itms.values()]
        return selfdict

    def duplicate_itm(self, name):
        if name not in self.itms.keys():
            return None
        else:
            import copy
            newname = name+"_II"
            newitmobj = copy.deepcopy(self.itms[name])
            newitmobj.name = newname
            self.itms[newname] = newitmobj
            return self.itms[newname]

    def chg_unit(self, unit=None):
        #print(unit); print(self.unit); print("Unit")
        if unit is None:
            return

        itmobjs = self.get_itms()
        for itmobj in itmobjs:
            print(conv[self.unit][unit])
            print(itmobj.energy)
            itmobj.energy *= conv[self.unit][unit]
            itmobj.zpe *= conv[self.unit][unit]
            for prop in itmobj.thermo.keys():
                for temp in itmobj.thermo[prop].keys():
                    itmobj.thermo[prop][temp] *= conv[self.unit][unit]
        self.unit = unit
        for itmobj in itmobjs:
            itmobj.full_update()

    def get_itm_index(self, iname):
        for n, itmo in enumerate(self.itms.values()):
            if itmo.name == iname:
                return n
        return -1

    def set_def_basis_ref(self, reflist):
        # this method sets up the default reference of the mechanism
        # from a list of strings with the names of the intermediates
        # with the list the intermediates object are included in the ref list
        if not isinstance(reflist, list):
            print("No instance")
            return False
        def_basis_ref = []
        for i in reflist:
            if isinstance(i, str):
                i = self.get_itm(i)
                if i is None:
                    return False
            elif isinstance(i, Itm):
                if i.struct.nat == 0:
                    return False
            def_basis_ref.append(i)
        self.def_basis_ref = def_basis_ref[:]
        return True

    def add_refs(self, refdict):
        returnmessage = ""
        badref = False
        for name, refs in refdict.items():
            if not isinstance(name, str):
                continue
            refobjs = []
            for ref in refs:
                if isinstance(ref, Itm):
                    if ref.struct.nat == 0:
                        badref = True
                        break
                elif isinstance(ref, str):
                    ref = self.get_itm(ref)
                    if ref is None or ref.struct.nat == 0:
                        badref = True
                        break
                refobjs.append(ref)
            if badref:
                returnmessage += "\n Bad item for :" + name
                continue

            if name in self.refs:
                returnmessage += "\n" + name + " already exists!!!"
                continue

            self.refs[name] = refobjs
        return returnmessage

    def add_ref(self, name: str, reflist: list):
        """
        :param name: name of the new reference
        :param reflist: list of itm objects with which
        reference is made up
        :return: new reference (list of itm objects)
        """
        refobjs = []
        if not isinstance(name, str) or not name:
            raise TypeError("Expected string for name of reference")
        # verify that all are objects
        for ref in reflist:
            if isinstance(ref, str):
                ref = self.get_itm(ref)
                if ref is None or ref.struct.nat == 0:
                    return None
            elif isinstance(ref, Itm):
                if ref.struct.nat == 0:
                    return None
            else:
                return None
            refobjs.append(ref)

        if name in self.refs:
            raise NameError("Name already exists!")
        self.refs[name] = refobjs
        return self.refs[name]

    def del_ref_from_itms(self, zero):
        for itm in self.itms.values():
            itm.del_ref(zero)

    def del_itm(self, name):
        del self.itms[name]

    def itm_exists(self, name):
        if name.strip() in self.itms.keys():
            return True
        else:
            return False

    def itm_change_name(self, oname, nname):
        # Busco en el self.items por clave valor y devuelvo nueva clave, valor
        # si la clave coincide con oname
        # esto tiene el problema de que crea una nueva instancia y accedo con referencias a la 
        # vieja instancia tendre un valor desactualizado
        # debo garantizar que todas las referencias directas a self.items sea temporales, o sea
        # dentro de funciones o que se accedan solo una vez
        updated_key_value_list = [(nname, v) if c == oname else (c, v) for c, v in self.itms.items()]
        self.itms = OrderedDict(updated_key_value_list)
        self.itms[nname].name = nname

    #        self.itms[oname].name = nname
    #        self.itms[nname] = self.itms[oname]
    #        del self.itms[oname]

    def add_itm(self, name, *args, **kwargs):
        if isinstance(name, Itm):
            #  verify an itm with that name
            # does not exist to add it
            if name.name not in self.itms.keys():
                itmd = name.to_dict()
                itmd["refs"] = {}
                itmd["reacs"] = {}
                if itmd["tp"] == "mecp":
                    self.itms[name.name] = Itm(name.name, mech=self,
                                               energy=itmd["energy"],
                                               tp=itmd["tp"], freqs=itmd["freqs"],
                                               cm=itmd["cm"], temps=itmd["temps"],
                                               struct=itmd["struct"],
                                               pg=itmd["pg"], photos=itmd["photos"],
                                               thermo=itmd["thermo"], zpe=itmd["zpe"],
                                               spin=itmd["spin"],
                                               redmass=itmd["redmass"],
                                               SOC=itmd["SOC"],
                                               grad=itmd["grad"],
                                               diffgrad=itmd["diffgrad"]
                                               )
                else:
                    self.itms[name.name] = Itm(name.name, mech=self,
                                           energy=itmd["energy"],
                                           tp=itmd["tp"], freqs=itmd["freqs"],
                                           cm=itmd["cm"], temps=itmd["temps"],
                                           struct=itmd["struct"],
                                           pg=itmd["pg"], photos=itmd["photos"],
                                           thermo=itmd["thermo"], zpe=itmd["zpe"],
                                           spin=itmd["spin"]
                                           )
                return self.itms[name.name]
        elif name not in self.itms.keys():
            self.itms[name] = Itm(name, mech=self, *args, **kwargs)
            return self.itms[name]
        else:
            print("There is already an item with that name")

        return None

    def get_itm(self, name):
        if name in self.itms.keys():
            return self.itms[name]
        return None

    def get_itms(self, tp=None, exclude=None):
        if tp is not None:
            return [itm for itm in self.itms.values()
                    if itm.tp == tp]
        if exclude is not None:
            return [itm for itm in self.itms.values()
                    if itm.tp != exclude]
        return list(self.itms.values())

    def get_itms_names(self, tp=None,  exclude=None):
        if tp is not None:
            return [itm.name
                    for itm in self.itms.values()
                    if itm.tp == tp]
        if exclude is not None:
            return [itm.name
                    for itm in self.itms.values()
                    if itm.tp != exclude]
        return list(self.itms.keys())

    def itms_common_refs(self, itms):
        #     search for common ref among a group of itms
        commonrefs = []
        allrefs = []
        for itm in itms:
            allrefs += itm.get_refs_names()
        unique, counts = np.unique(allrefs, return_counts=True)
        commonrefs = [unique[n] for n, c in enumerate(counts)
                      if c == len(itms)]

        return commonrefs

    def get_refs_names(self):
        return list(self.refs.keys())

    def get_refs(self):
        return list(self.refs.values())

    def get_ref(self, name):
        if name in self.refs.keys():
            return self.refs[name]
        else:
            return None

    def del_ref(self, name):
        if name in self.refs.keys():
            del self.refs[name]
        self.del_ref_from_itms(name)

    def merge_itms(self, itmlist, name=None):
        # esto lo tengo que arreglar para avisar que alguno no se añadio
        itmbobjs = []
        etot = 0.0
        ezpe = 0.0
        spintot = 0
        names = []
        tp = "min"
        commonrefs = []
        for itm in itmlist:
            itmobj = self.get_itm(itm)
            print(itm, itmobj)
            if itmobj is None:
                raise TypeError("Expected item, got None")
            etot += itmobj.energy
            ezpe += itmobj.zpe
            spintot += itmobj.spin
            names.append(itmobj.name)
            if itmobj.tp == "ts":
                tp = "ts"
            itmbobjs.append(itmobj)
            commonrefs += itmobj.get_refs_names()

        # ---------- Set up name -----------------------------
        if name is None:
            # This can be also done with collections.Counter
            name_pairs = [[x, names.count(x)] for x in set(names)]
            # print(name_pairs)
            name = " + ".join([str(count)+"_"+nm for nm, count in name_pairs])
            name = name.replace("1_", "")
            #print(name)
        # ---------- Set up name -----------------------------

        newitm = self.add_itm(name, energy=etot, zpe=ezpe, spin=spintot,
                            tp=tp, merged=True, itms=itmbobjs)
        if newitm is None:
            raise ValueError("Name already exists!")

        # search for common references in itmlist
        unique, counts = np.unique(commonrefs, return_counts=True)
        commonrefs = [u for u, c in zip(unique, counts)
                      if c == len(itmlist)]
        # search for common references in itmlist

        newitm.merge_structs()
        newitm.recalc_thermo_from_itms()
        newitm.set_temps()
        for cr in commonrefs:
            # list of Itm Objs that make up the reference
            lobjrefs = self.get_ref(cr)
            # list of coefficients, one for every Itm object in lobjrefs
            # they must be in the same order in order to add a correct ref
            # to newitm
            # initialize with zeros
            lcoefs = [0 for lo in lobjrefs]
            for itmo in itmbobjs:
                refo = itmo.get_ref(cr)
                for n, lo in enumerate(lobjrefs):
                    # n is the index of the list of object lobjrefs to
                    # make the reference for newitm
                    index = refo.get_refitm_index(lo.name)
                    if index is not None:
                        lcoefs[n] += refo.coefs[index]
                    else:
                        newitm.cm += "\n coefficient for "+lo.name+" might be wrong!\n"

            if lobjrefs is not None:
                newitm.add_ref(cr, lobjrefs, lcoefs)

    def fullmerge_itms(self, itmlist):
        # esto lo tengo que arreglar para avisar que alguno no se añadio
        itmobjs = []
        etot = 0.0
        name = ""
        vibs = []
        unit = None
        spintot = 0.0
        for itm in itmlist:
            itmobj = self.get_itm(itm)
            if itmobj is None:
                raise TypeError("Expected item, got None")
            itmobjs.append(itmobj)
            etot += itmobj.energy
            spintot += itmobj.spin
            vibs += itmobj.freqs.vibs
            unit = itmobj.freqs.unit
            if name == "":
                name += itmobj.name
            else:
                name += " + " + itmobj.name
        name += "_full"

        newitm = self.add_itm(name, energy=etot,
                              freqs={"vibs":vibs, "unit":unit},
                              tp="min", merged=False)
        if newitm is None:
            raise ValueError("Name already exists!")
        newitm.merge_structs(itmobjs)
        newitm.set_temps()

    def itm_from_folder_custom(self, folder, add=True):
        itmname = os.path.basename(folder)
        energy = 0
        unit = "eV"
        structfile = ""
        en_file = ""
        freqs_file = ""
        pg = ""
        spin = ""
        tp = ""
        cm = ""

        with open(folder+"/.data") as f:
            lines = f.readlines()
        program = lines[0].split()[0]

        for line in lines:
            if not line: continue

            if "program" in line.lower():
                try:
                    program = line.split()[1]
                except IndexError:
                    pass
            if "energy" in line.lower():
                try:
                    en_file = line.split()[1]
                except IndexError:
                    pass
            if "freq" in line.lower():
                try:
                    freqs_file = line.split()[1]
                except IndexError:
                    pass
            if "struct" in line:
                try:
                    structfile = line.split()[1]
                except IndexError:
                    pass
            if "pg" in line:
                try:
                    pg = line.split()[1]
                except IndexError:
                    pass
            if "spin" in line:
                try:
                    spin = float(line.split()[1])
                except IndexError:
                    pass
                except ValueError:
                    pass
            if "tp" in line:
                try:
                    tp = line.split()[1]
                except IndexError:
                    pass
            if "unit" in line:
                try:
                    unit = line.split()[1]
                except IndexError:
                    pass

        if program.lower() == "gaussian":
            unit = "Ha"
            if os.path.isfile(folder + "/" + en_file):
                try:
                    energy = read_energy_from_gaussian(folder + "/" + en_file)
                except ValueError:
                    energy = 0.0
                    cm = "ERROR READING ENERGY FROM: " + en_file+"\n"
        elif program.lower() == "vasp":
            unit = "eV"
            if os.path.isfile(folder + "/" + en_file):
                try:
                    energy, spin = read_oszicar_energy(folder + "/" + en_file)
                    print("energy: ", energy)
                except ValueError as e:
                    energy = 0.0
                    cm = "ERROR READING ENERGY FROM: " + en_file + "\n"
                    print(e)

        if not energy:
            try:
                energy = float(en_file)
            except ValueError:
                energy = 0.0
                cm = "ERROR READING ENERGY FROM .data."
                cm += str(en_file) + " IS NOT A NUMBER\n"

        if unit != self.unit:
            energy *= conv[unit][self.unit]
        itmobj = Itm(itmname, energy=energy, spin=spin)
        if add:
            itmobj = self.add_itm(itmname, energy=energy, spin=spin)
        if itmobj is None:
            return None

        if os.path.isfile(folder + "/" + freqs_file):
            itmobj.freqs_from_file(folder + "/" + freqs_file, program)
        else:
            itmobj.cm += "FILE " + freqs_file + " TO READ FREQUENCIES DOES NOT EXIST\n"

        #print(folder + "/" + structfile)
        if os.path.isfile(folder + "/" + structfile):
            itmobj.struct_from_file(folder + "/" + structfile)
        else:
            itmobj.cm += structfile + " DOES NOT EXISTS \n"
        if pg:
            itmobj.pg = pg
        if tp:
            itmobj.set_tp(tp)
        if spin:
            itmobj.spin = spin
        if cm:
            itmobj.cm += cm
        return itmobj

    def itm_from_folder(self, folder, add=True):
        if os.path.isfile(folder+"/.data"):
            return self.itm_from_folder_custom(folder, add=add)

        itmname = os.path.basename(folder)
        # Try VASP, if not try ORCA.
        energy = None
        poss_files = ["OSZICAR.opt", "job.out"]
        for pfile in poss_files:
            path = os.path.join(folder, pfile)
            if not os.path.isfile(path):
                continue
            try:
                if pfile == "OSZICAR.opt":
                    energy, spin = read_oszicar_energy(path)
                    unit = "eV"
                elif pfile == "job.out":
                    energy = read_orca_energy(path)
                    unit = "Ha"
            except Exception as e:
                print("Error reading energy for: " + itmname)
                print(e)
                return None
        if energy is None:
            print("No energy file found for: " + itmname)
            return None
        
#        try:
#            energy = read_oszicar_energy(folder + "/OSZICAR.opt")
#        except Exception as e:
#            print("Error reading energy for: " + itmname)
#            print(e)
#            return None

        if unit != self.unit:
            energy *= conv[unit][self.unit]
        itmobj = Itm(itmname, energy=energy, spin=spin)
        if add:
            itmobj = self.add_itm(itmname, energy=energy, spin=spin)

        if itmobj is None:
            print("Item no added, that name is already used")
            return None

        if os.path.isfile(folder + "/DIMCAR") or os.path.isfile(folder + "/DIMCAR.out"):
            tp = "ts"
        elif os.path.isfile(folder + "/MECP.info.molden"):
            tp = "mecp"
        else:
            tp = "min"
        itmobj.set_tp(tp)

        # Try VASP, if not try ORCA.
        poss_files = ["MECP.info.molden", "OUTCAR.freq", "job.out", "OUTCAR"]
        for pfile in poss_files:
            path = os.path.join(folder, pfile)
            try:
                path2 = os.path.join(folder, "OUTCAR.fake")
            except:
                print("No OUTCAR.fake for frequencies")
                path2 = None
            if not os.path.isfile(path):
                continue
            try:
                if pfile == "MECP.info.molden":
                    freqs, unit, redmass, grad, grad_diff= read_mfreq(path, path2)
                    if redmass is not None:
                        itmobj.redmass = redmass
                    if grad is not None:
                        itmobj.grad = grad
                    if grad_diff is not None:
                        itmobj.diffgrad = grad_diff
                elif pfile == "OUTCAR.freq":
                    freqs, unit = read_outcar_freqs(path)
                elif pfile == "OUTCAR":
                    try: 
                        freqs, unit = read_outcar_freqs(path)
                    except Exception as e:
                        print("Error reading frequencies from OUTCAR for: " + itmname)
                        print(e)
                        continue
                elif pfile == "job.out":
                    freqs = read_orca_freqs(path)
                    unit = "cm-1"
                    # Also get Point group
                    pg = read_orca_pg(path)
                    if pg is not None:
                        itmobj.pg = pg
            except Exception as e:
                print("Error reading frequencies for: " + itmname)
                print(e)
            else:
                itmobj.freqs.set_vibs(freqs[:])
                itmobj.freqs.unit = unit
                break
        #try:
        #    freqs, unit = read_outcar_freqs(folder + "/OUTCAR.freq")
        #except Exception as e:
        #    print("Error reading frequencies for: " + itmname)
        #    print(e)
        #else:
        #    itmobj.freqs.set_vibs(freqs[:])
        #    itmobj.freqs.unit = unit
#
        poss_files = ["POSCAR.opt", "poscar.xyz", "job.xyz", "CONTCAR.opt", "CONTCAR", "POSCAR"]
        for pfile in poss_files:
            path = os.path.join(folder, pfile)
            if os.path.isfile(path):
                structfile = path
                break

        itmobj.struct_from_file(structfile)
        listdirs = os.listdir(folder)
        listdirs = [folder + "/" + p for p in listdirs if "foto" in p and p.endswith(".png")]
        itmobj.photo_from_files(listdirs)

        return itmobj


class Data(object):
    def __init__(self):
        self.mechs = OrderedDict()
        self.diagrams = OrderedDict()

    def to_dict(self):
        selfdict = OrderedDict()
        selfdict["mechs"] = [mech.to_dict() for mech in self.mechs.values()]
        selfdict["diagrams"] = [d.to_dict() for d in self.diagrams.values()]
        return selfdict

    def change_mec_name(self, oname, nname):
        updated_key_value_list = [(nname, v) if c == oname else (c, v) for c, v in self.mechs.items()]
        self.mechs = OrderedDict(updated_key_value_list)
        self.mechs[nname].name = nname

    def get_plots(self, filter=[]):
        if not filter:
            return list(self.diagrams.values())
        else:
            return [self.diagrams[key] for key in self.diagrams.keys()
                    if key in filter]

    def get_plot(self, gname):
        if gname in self.diagrams.keys():
            return self.diagrams[gname]
        else:
            return None

    def clone_plot(self, gname, newname):
        self.diagrams[newname] = copy.deepcopy(self.diagrams[gname])
        self.diagrams[newname].name = newname
        #print(self.diagrams.keys())

    def get_plots_names(self):
        return list(self.diagrams.keys())

    def del_plot(self, name):
        if name in self.diagrams.keys():
            del self.diagrams[name]
            return True
        else:
            return False

    def add_plot(self, name=None, ref=None, col=None, *args, **kwargs):
        """ref and name are both string with names"""
        if col is None:
            return
        else:
            if isinstance(col, str):
                col = self.get_mech(col)
                if col is None:
                    return
            elif not isinstance(col, Mechanism):
                return
        if ref is None:
            return
        if name is None:
            name = "Plot " + str(len(self.diagrams))
        if name not in self.diagrams:
            self.diagrams[name] = Plot(name, ref, col, *args, **kwargs)
        return self.diagrams[name]

    def update_plot_energies(self):
        for plot in self.diagrams.values():
            plot.update_temp_en()

    def add_m(self, name, *args, **kwargs):
        if isinstance(name, Mechanism):
            if name.name not in self.mechs.keys():
                self.mechs[name.name] = name
                return self.mechs[name.name]
        elif name not in self.mechs:
            self.mechs[name] = Mechanism(name, *args, **kwargs)
            return self.mechs[name]
        return None

    def get_mech(self, name):
        return self.mechs[name]

    def get_mech_index(self, mname):
        for n, m in enumerate(self.mechs.values()):
            if m.name == mname:
                return n
        return -1

    def get_mechs(self):
        return self.mechs.values()

    def get_mechs_names(self):
        return list(self.mechs.keys())

    def mec_from_folder(self, folder):
        listdirs = os.listdir(folder)
        mecname = os.path.basename(folder)
        mecobj = self.add_m(mecname)
        if mecobj is None:
            print(mecname + " That Mechanism alredy exists")
            return None

        for subfolder in listdirs:
            mecobj.itm_from_folder(os.path.join(folder, subfolder))
        return mecobj
    
    def mec_from_folders(self, listdirs, mecname):
        mecobj = self.add_m(mecname)
        if mecobj is None:
            print(mecname + " That Mechanism alredy exists")
            return None

        for subfolder in listdirs:
            mecobj.itm_from_folder(subfolder)
        return mecobj

    def add_from_data(self, data):
        mecs_names = self.get_mechs_names()
        for mecobj in data.get_mechs():
            if mecobj.name not in mecs_names:
                self.mechs[mecobj.name] = mecobj
            else:
                self.mechs[mecobj.name+"_copy"] = mecobj

    def dict_to_data(self, data):
        # tengo que pulirlo para detectar errores
        if "mechs" in data:
            for mec in data["mechs"]:
                mecobj = self.add_m(mec["name"], unit=mec["unit"],)
                if mecobj is None:
                    continue
                for itm in mec["itms"]:
                    itmobj = mecobj.add_itm(itm["name"], energy=itm["energy"],
                                   tp=itm["tp"], freqs=itm["freqs"],
                                   cm=itm["cm"], temps=itm["temps"],
                                   struct=itm["struct"], 
                                   pg=itm["pg"], photos=itm["photos"])
                    print(itmobj)
                    if itmobj.tp == "mecp":
                        itmobj.redmass = itm["redmass"]
                        try:
                            itmobj.SOC = itm["SOC"]
                        except KeyError:
                            itmobj.SOC = 0.0
                        itmobj.grad = itm["grad"]
                        itmobj.diffgrad = itm["diffgrad"]
                    if "thermo" in itm:
                        for key, value in itm["thermo"].items():
                            # I have to do it becasue the dic load with json loses
                            # the type float of the keys of temperature
                            itm["thermo"][key] = dict([(float(k), v) for k, v in value.items()])
                        itmobj.thermo = itm["thermo"]
                    if "zpe" in itm:
                        itmobj.zpe = itm["zpe"]
                    if "merged" in itm and itm["merged"]:
                        itmobj.add_itms(itm["itms"])
                        itmobj.merged=itm["merged"]
                    else:
                        itmobj.merged = False
                    if "spin" in itm:
                        itmobj.spin = itm["spin"]

                for itm in mec["itms"]:
                    # iterate over the intermediates because they are already
                    # instantiated
                    # get the instance of the intermediate I want to update the references of
                    itmobj = mecobj.get_itm(itm["name"])
                    if itmobj is None:
                        print("No object found for intermediate: " + itm["name"])
                        continue
                    # iterate over all the reference objects of the intermadiate
                    # whose references are being updated
                    for ref in itm["refs"]:
                        # for every reference obj, get the list of intermediates objects
                        # of type "ref" that form the basis set of reference of the
                        # reference object
                        refobjs = [mecobj.get_itm(refitm) for refitm in ref["refs"]]
                        if "coefs" in ref:
                            coefs = ref["coefs"]
                        else:
                            coefs = []
                        try:
                            itmobj.add_ref(ref["name"], refobjs, coefs)
                        except np.linalg.LinAlgError:
                            itmobj.add_ref(ref["name"], refobjs, [0 for i in refobjs])
                            itmobj.cm += "There was a problem loading calculating " \
                                         "the coefficients for : " + ref["name"]

                for itm in mec["itms"]:
                    # iterate over the intermediates because they are already
                    # instantiated
                    # get the instance of the intermediate I want to update the references of
                    itmobj = mecobj.get_itm(itm["name"])
                    if itmobj is None:
                        print("No object found for intermediate: " + itm["name"])
                        continue

                    for reac in itm["reacs"]:
                        if "refs" in reac:
                            if isinstance(reac["refs"], list):
                                # esto pronto lo eliminare, es solo para cargar los datos que ya he metido
                                reacobjs = [mecobj.get_itm(reacitm) for reacitm in reac["refs"]]
                                if len(reacobjs) != 1:
                                    print("Error reading reacobj")
                                    continue
                                # add reac does not need a name, the name is taken
                                # from the reference: reac obj
                                itmobj.add_reac(reacobjs[0])
                        else:
                            if "relref" in reac:
                                # busco el object itm que es de referencia para itmobj
                                # y le añado una referencia pasando el objecto reac["ref"]
                                try:
                                    itmobj.add_reac(mecobj.get_itm(reac["ref"]), reac["relref"])
                                except:
                                    pass

                if "refs" in mec:
                    message = mecobj.add_refs(mec["refs"])
                    print(message)
                if "def_basis_ref" in mec:
                    mecobj.set_def_basis_ref(mec["def_basis_ref"])

        if "diagrams" in data:
            for diag in data["diagrams"]:

                plotobj = self.add_plot(name=diag["name"],
                                        ref=diag["ref"],
                                        col=diag["col"],
                                        etype=diag["etype"],
                                        xmax=diag["xmax"],
                                        xmin=diag["xmin"],
                                        ymax=diag["ymax"],
                                        ymin=diag["ymin"])
                if "itms" in diag:
                    for itm in diag["itms"]:
                        itmobj = self.get_mech(diag["col"]).get_itm(itm["name"])
                        if itmobj is None:
                            continue
                        pitmobj = plotobj.add_itm(itmobj, name=itm["name"],
                                                  color=itm["color"],
                                                  width=itm["width"],
                                                  length=itm["length"],
                                                  style=itm["style"],
                                                  coors=itm["coors"])

                if "labels" in diag:
                    for label in diag["labels"]:
                        itmname = label["itm"]
                        pitmobj = plotobj.get_itm(itmname)
                        if pitmobj is None:
                            print(itmname + " not found! Label will not be created")
                            continue
                        plotobj.add_label(**label)

                        # plotobj.add_label(pitmobj, label["name"],
                        #                   text=label["text"],
                        #                   color=label["color"],
                        #                   coors=label["coors"],
                        #                   font=label["font"],
                        #                   # )
                        #                   visible=label["visible"]) # cuidado si la el archivo no tenia visible para la etiqueta va a dar error
                if "links" in diag:
                    for link in diag["links"]:
                        pitmobj1 = plotobj.get_itm(link["pitm1"])
                        pitmobj2 = plotobj.get_itm(link["pitm2"])
                        if pitmobj1 is None:
                            print(link["pitm1"] + " not found! Link will not be created")
                            continue
                        if pitmobj2 is None:
                            print(link["pitm2"] + " not found! Link will not be created")
                            continue

                        plotobj.add_link(pitmobj1, pitmobj2,
                                         color=link["color"],
                                         width=link["width"],
                                         style=link["style"],
                                         coors=link["coors"])


def dict_to_data(indict):
    dataobj = Data()

    for col in indict["c_o"]:
        if "unit" in indict[col]:
            unit = indict[col]["unit"]
        mec = dataobj.add_m(col, unit=unit)

        for itm in indict[col]["i_o"]:
            itmobj = mec.add_itm(itm)

            if "cm" in indict[col][itm]:
                itmobj.set_comment(indict[col][itm]["cm"])
            # esto lo tengo que arreglar
            if not itmobj.set_e(indict[col][itm]["e"]):
                print("Bad energy value for item: " + itmobj.name)
                itmobj.set_e(0.0)

            if "tp" in indict[col][itm]:
                itmobj.tp = indict[col][itm]["tp"]
            if "freq" in indict[col][itm]:
                itmobj.set_freqs(indict[col][itm]["freq"])

            if "pg" in indict[col][itm]:
                itmobj.set_pg(indict[col][itm]["pg"])

            if "0reac" in indict[col][itm] and \
                    indict[col][itm]["0reac"] is not None:
                # get the name of reac reference according to file
                zreac = indict[col][itm]["0reac"]
                # get the itm object by the name from file
                # to verify whether or not it exists
                zreac = mec.get_itm(zreac)
                if zreac is not None:
                    mec.get_itm(itm).add_reac(zreac)

    return dataobj
