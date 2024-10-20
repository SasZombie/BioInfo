import csv
import matplotlib.pyplot as plt

def read_file()->dict:
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

def main()->None:
    
    dict_list = read_file()

    plot_lines_evolution(dict_list);
   
    plot_points(dict_list);


    # dict_list = []

    # with open('sliding.csv', mode='r') as file:
    #     reader = csv.reader(file)
        
    #     for row in reader:
    #         if len(row) > 0:
    #             d = {row[i]: float(row[i + 1]) for i in range(0, len(row) - 1, 2) if i + 1 < len(row)}
    #             dict_list.append(d)

    # plt.figure(figsize=(8, 6))

    # for i, d in enumerate(dict_list):
    #     keys = list(d.keys())
    #     values = list(d.values())
    #     plt.plot(keys, values, marker='o', linestyle='-', label=f'Line {i+1}')

    # plt.xlabel('Category')
    # plt.ylabel('Value')
    # plt.title('Ploting of values')
    # plt.grid(True)
    # plt.show()


if __name__ == "__main__":
    main()