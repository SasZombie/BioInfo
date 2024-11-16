import csv
import matplotlib.pyplot as plt

def read_file()->list:
    dict_list = []

    with open('out.csv', mode='r') as file:
        reader = csv.reader(file)
        
        for row in reader:
            if len(row) > 0:
                d = {row[i]: float(row[i + 1]) for i in range(0, len(row) - 1, 2) if i + 1 < len(row)}
                dict_list.append(d)
    
    return dict_list;

def plot_lines_evolution(dict_list: dict)->None:
    plt.figure(figsize=(8, 6))

    for i, d in enumerate(dict_list):
        keys = list(d.keys())
        values = list(d.values())
        plt.plot(keys, values, marker='o', linestyle='-', label=f'Line {i+1}')

    plt.xlabel('Category')
    plt.ylabel('Value')
    plt.title('Ploting of values')
    plt.grid(True)
    plt.show()

def plot_lines_with_peaks(dict_list: dict)->None:
    plt.figure(figsize=(8, 6))

    for i, d in enumerate(dict_list):
        keys = list(d.keys())
        values = list(d.values())
        plt.plot(keys, values, marker='o', linestyle='-', label=f'Line {i + 1}')

    for i, d in enumerate(dict_list):
        keys = list(d.keys())
        values = list(d.values())
        
        max_value = max(values)
        max_key = keys[values.index(max_value)]
        
        plt.plot(max_key, max_value, marker='*', color='red', markersize=12, label=f'Peak {i + 1}')

    plt.xlabel('Category')
    plt.ylabel('Value')
    plt.title('Plot of Values with Peaks')
    plt.grid(True)
    plt.legend()
    plt.show()

def plot_points(dict_list: dict)->None:
    
    #Y-axis = made of % (a + T) and X-axis % (g + C)
    for i in dict_list:
        x_coord = i['A'] + i['T']
        y_coord = i['G'] + i['C']
        plt.scatter(x_coord, y_coord, marker='o', color='blue')

    plt.xlabel('Category')
    plt.ylabel('Value')
    plt.title('Ploting of values')
    plt.grid(True)
    plt.show()

def read_list()->dict:
    pattern_counts = {}

    with open('out.csv', 'r') as file:
        reader = csv.reader(file)
        for row in reader:
            pattern = row[0].strip()  
            count = int(row[1].strip())  
            pattern_counts[pattern] = count

    return pattern_counts

def plot_bar()->None:
    pattern_counts = {}

    with open('out.csv', 'r') as file:
        reader = csv.reader(file)
        for row in reader:
            pattern = row[0].strip()  
            count = int(row[1].strip())  
            pattern_counts[pattern] = count

    patterns = list(pattern_counts.keys())
    counts = list(pattern_counts.values())

    plt.figure(figsize=(10, 6))
    plt.bar(patterns, counts, color='skyblue')
    plt.xlabel('Patterns')
    plt.ylabel('Count')
    plt.title('Pattern Frequency')
    plt.xticks(rotation=45)
    plt.tight_layout()  
    plt.show()

def relative_bar_chart()->None:

    with open('chromosome14', 'r') as file:
        chromosome_data = file.read().replace('\n', '')
    chromosome_length = len(chromosome_data)

    number = 0
    pattern_counts = {}
    with open('out.csv', 'r') as file:
        reader = csv.reader(file)
        for row in reader:
            pattern = row[0].strip()
            count = int(row[1].strip())
            if count >= 20:
                number += 1
                pattern_counts[pattern] = count

    sorted_patterns = sorted(pattern_counts.items(), key=lambda x: x[1], reverse=True)

    pattern_positions = {}
    for pattern, count in sorted_patterns:
        positions = []
        start = 0
        while True:
            pos = chromosome_data.find(pattern, start)
            if pos == -1:
                break
            end_pos = pos + len(pattern)  
            positions.append((pos, end_pos))  
            start = pos + 1
        pattern_positions[pattern] = positions

    plt.figure(figsize=(12, 8))

    y_labels = [pattern for pattern, _ in sorted_patterns]
    y_positions = range(len(y_labels))

    for i, pattern in enumerate(y_labels):
        positions = pattern_positions[pattern]
        
        plt.hlines(y=i, xmin=0, xmax=chromosome_length, color='lightgrey', linewidth=2)
        
        start_positions = [pos[0] for pos in positions]
        end_positions = [pos[1] for pos in positions]
        
        plt.plot(start_positions, [i] * len(start_positions), 'x', color='blue', label='Start' if i == 0 else "") 
        plt.plot(end_positions, [i] * len(end_positions), 'o', color='red', label='End' if i == 0 else "") 

    plt.yticks(y_positions, y_labels)
    plt.xlabel('Position on Chromosome 14 (number of characters)')
    plt.ylabel('Patterns')
    plt.title('Pattern Occurrences on Chromosome 14')
    plt.xlim(0, chromosome_length)
    plt.gca().invert_yaxis()
    plt.legend()
    plt.tight_layout()
    plt.show()

def read_gel()->None:
    with open("out", "r") as file:
        reader = csv.reader(file)
        sequences = next(reader)

    sequences = sorted(sequences, key=len, reverse=True)

    fig, ax = plt.subplots(figsize=(len(sequences), 8))
    ax.set_xlim(-1, len(sequences))
    ax.set_ylim(0, max(len(seq) for seq in sequences))

    for i, seq in enumerate(sequences):
        end_position = len(seq) - 1  
        ax.plot(i, end_position, 's', color='black', markersize=8) 

    ax.set_xticks(range(len(sequences)))
    ax.set_xticklabels([f'Seq {i+1}' for i in range(len(sequences))], rotation=90)
    ax.set_yticks([])
    ax.set_title("Gel Electrophoresis Representation of Sequences")

    plt.gca().invert_yaxis()  
    plt.show()

def main()->None:
    read_gel()


if __name__ == "__main__":
    main()