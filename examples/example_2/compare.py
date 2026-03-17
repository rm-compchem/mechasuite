import pandas as pd
import numpy as np
import matplotlib.pyplot as plt

fig, ax = plt.subplots(2, 4)
spec_colors = {
        "TBAF": "black", 
        "CH3CH2Br":"blue" ,         
        "CH3CH2F": "red",
        "HF": "green",
        "TBABr": "violet",
        "C2H4": "orange"
        }

def plot_openmkm(filename, axis):
    #filename = 'gas_mole_tr.dat' 
    
    df = pd.read_csv(filename, sep=r'\s+', skiprows=1)
    
    time_col = df.columns[0]
    
    #plt.figure(figsize=(10, 6))
    for column in df.columns[1:]:
        axis.plot(pd.to_numeric(df[time_col], errors='coerce'),
                 pd.to_numeric(df[column], errors='coerce'),
                 label=column, color=spec_colors[column])
    
    #axis.xscale('linear')
    
    axis.set_xlabel('Time (s)')
    axis.set_ylabel('Mole Fraction')

def plot_mecha(filename, axis):
    labels = []
    with open(filename) as f:
        l = f.readline()
        labels = l.replace("#", "").split()[1:]

    data = np.loadtxt(filename)
    for n, label in enumerate(labels, start=1):
        d = data[:, n]
        axis.plot(data[:, 0], d, label=label, 
                 color=spec_colors[label])
    axis.set_xlabel('Time (s)')
    axis.set_ylabel('Mole Fraction')


#plt.title('Gas Species Concentrations over Time')
#plt.grid(True, which="both", ls="--", alpha=0.5) 
#plt.savefig("0TBOH.png", dpi=300)
#plt.show()

plot_mecha("reaction_network_0tboh.out", ax[0][0])
plot_mecha("reaction_network_1tboh.out", ax[0][1])
plot_mecha("reaction_network_2tboh.out", ax[0][2])
plot_mecha("reaction_network_3tboh.out", ax[0][3])
plot_openmkm("comparison_openmkm/0_TBOH/gas_mole_tr.dat", ax[1][0])
plot_openmkm("comparison_openmkm/1_TBOH/gas_mole_tr.dat", ax[1][1])
plot_openmkm("comparison_openmkm/2_TBOH/gas_mole_tr.dat", ax[1][2])
plot_openmkm("comparison_openmkm/3_TBOH/gas_mole_tr.dat", ax[1][3])

plt.legend()
plt.show()
