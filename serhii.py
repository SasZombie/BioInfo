import matplotlib.pyplot as plt
import numpy as np

# Function to compute CpG content in a DNA sequence
def compute_cg_content(sequence):
    cg_count = sequence.count('C') + sequence.count('G')
    cg_percentage = (cg_count / len(sequence)) * 100
    return cg_percentage

# Function to compute the Index of Coincidence (IC) as per formula
def compute_kappa_index(sequence):
    n = len(sequence)
    ic_sum = 0
    for u in range(1, n):
        match_count = 0
        for i in range(n - u):
            if sequence[i] == sequence[i + u]:
                match_count += 1
        ic_sum += (match_count / (n - u)) * 100
    ic = 100 - (ic_sum / (n - 1))
    return ic

# Refined function for plotting to match ODS-style visuals
def plot_ods(centers, cg_values, ic_values):
    plt.figure(figsize=(10, 6))

    # Combined CG content and IC on the same graph for ODS-like visualization
    plt.scatter(cg_values, ic_values, label="ODS Pattern", color='black', s=10)
    plt.xlabel("CG Content (%)")
    plt.ylabel("Information Content (IC)")
    plt.title("Objective Digital Stain (ODS) Pattern")
    plt.grid(True, linestyle='--', alpha=0.5)
    plt.legend()

    plt.tight_layout()
    plt.show()

# Function to process sliding window and calculate metrics
def process_sliding_window(sequence, window_size):
    cg_values = []
    ic_values = []
    centers = []

    for i in range(len(sequence) - window_size + 1):
        window = sequence[i:i + window_size]
        cg_values.append(compute_cg_content(window))
        ic_values.append(compute_kappa_index(window))
        centers.append(i + window_size // 2)

    
    # print(cg_values)
    print(ic_values)

    return cg_values, ic_values, centers

# Define DNA sequence and window size
sequence = "GTAAAAATCTAGTTTTCCGTACTCTGTCCCCTTTTTAATCCTTCTCTGAATGGGTGTCCTTGATTCACTGACATTTCCTAATTAACCAAGTTCTGCCAAGTTTCACATGAAACTTGGGGAACCGATTTCATTCCCCCAGTATCACCCTGTGGCGCCACCTTCCGGAGCTGTGAGGAAACTCCGGACTTCCCCCCAACACCGCCCCCTCCCTCCACCCTCGGTCTCCGCTTTCTGCGCTCTGCCGCGTTGGTTTTCGGAGGTGTCTGGGCGCATGCGCTTTGGACGGGCCGCCTAGCCTAGGAGAGACTACAATTCCCAGAAGACAGTGCGAAAGAAAAAAAAAATCCCGCGGTCCGTGGGGGTGGGAGAAATAAACGCTCGCGAGAGAACGAGGTTCAGGCGGCTGCACGAAGGGGGTGGAGGGGGGCTGGAGAGAGTGAGGAGGAAGGGGAGGAGGTGCCGTCCCACAATACCAGGCGGGAGGGCGGGTAGGCGGTTTGTATCCGGGCTGTGAGGTGCTCGGAGCCTCGGCGGACCTTGCTGCCTCTGTCTCTTTAACGCGAGAGGAAGCGATGCAGAGGGGTGGAAAATGGCAGAGCT"
window_size = 30

# Process the DNA sequence
cg_values, ic_values, centers = process_sliding_window(sequence, window_size)

# Plot CG content and Kappa Index as ODS
plot_ods(centers, cg_values, ic_values)

# Calculate the center of weight of the CG pattern
def center_of_weight(values, centers):
    weighted_sum = sum(value * center for value, center in zip(values, centers))
    total_weight = sum(values)
    return weighted_sum / total_weight

cg_center_weight = center_of_weight(cg_values, centers)
ic_center_weight = center_of_weight(ic_values, centers)

print(f"Center of Weight for CG Content: {cg_center_weight:.2f}")
print(f"Center of Weight for Kappa Index: {ic_center_weight:.2f}")

# Plot centers of weight
plt.figure(figsize=(6, 4))
plt.scatter(cg_center_weight, max(cg_values), label="CG Center of Weight", color='blue', s=50)
plt.scatter(ic_center_weight, max(ic_values), label="IC Center of Weight", color='orange', s=50)
plt.xlabel("Center of Weight")
plt.ylabel("Pattern Values")
plt.title("Centers of Weight")
plt.legend()
plt.grid(True, linestyle='--', alpha=0.5)
plt.show()