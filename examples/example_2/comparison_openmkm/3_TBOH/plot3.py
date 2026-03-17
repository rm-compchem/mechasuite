import pandas as pd
import matplotlib.pyplot as plt

filename = 'gas_mole_tr.dat' 

df = pd.read_csv(filename, sep=r'\s+', skiprows=1)

time_col = df.columns[0]

plt.figure(figsize=(10, 6))
for column in df.columns[1:]:
    plt.plot(pd.to_numeric(df[time_col], errors='coerce'),
             pd.to_numeric(df[column], errors='coerce'),
             label=column)

plt.xscale('linear')

plt.xlabel('Time (s)')
plt.ylabel('Mole Fraction')
plt.title('Gas Species Concentrations over Time')
plt.legend()
plt.grid(True, which="both", ls="--", alpha=0.5) 
plt.savefig("3TBOH.png", dpi=300)
plt.show()
