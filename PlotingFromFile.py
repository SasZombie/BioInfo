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
            print(number)
            number +=1 
            count = int(row[1].strip())
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
            positions.append(pos)
            start = pos + 1 
        pattern_positions[pattern] = positions

    plt.figure(figsize=(12, 8))

    y_labels = [pattern for pattern, _ in sorted_patterns]
    y_positions = range(len(y_labels))  

    for i, pattern in enumerate(y_labels):
        positions = pattern_positions[pattern]
        plt.hlines(y=i, xmin=0, xmax=chromosome_length, color='lightgrey', linewidth=2)
        plt.plot(positions, [i] * len(positions), 'x', color='blue')  

    plt.yticks(y_positions, y_labels)  # 
    plt.xlabel('Position on Chromosome 14 (number of characters)')
    plt.ylabel('Patterns')
    plt.title('Pattern Occurrences on Chromosome 14')
    plt.xlim(0, chromosome_length)  
    plt.gca().invert_yaxis()
    plt.tight_layout()
    plt.show()




def main()->None:
    relative_bar_chart()


if __name__ == "__main__":
    main()