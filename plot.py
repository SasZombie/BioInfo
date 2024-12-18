import pandas as pd
import matplotlib.pyplot as plt

def plot_ods(centers, cg_values, ic_values):
    plt.figure(figsize=(10, 6))

    plt.scatter(cg_values, ic_values, label="ODS Pattern", color='black', s=10)
    plt.xlabel("CG Content (%)")
    plt.ylabel("Information Content (IC)")
    plt.title("Objective Digital Stain (ODS) Pattern")
    plt.grid(True, linestyle='--', alpha=0.5)
    plt.legend()
    plt.gca().invert_yaxis()

    plt.tight_layout()
    plt.show()

def read_csv_and_plot(file_path):
    df = pd.read_csv(file_path)
    cgVals = df['CG_Content'].values
    icVals = df['IC'].values

    plot_ods(None, cgVals, icVals)


file_path1 = "newDna.csv"  
file_path2 = "newDna2.csv"  
read_csv_and_plot(file_path)
read_csv_and_plot(file_path)