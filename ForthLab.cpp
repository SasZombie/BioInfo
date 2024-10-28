#include <iostream>
#include <vector>
#include <string>
#include <unordered_map>
#include <fstream>

// Design a brute force algorithm that generates all the combinations of 2 and 3 letters based on an alphabet of the sequence of 4 letters
// Namely ATCG
// Calculate the % of each combination over a sequence S where S can be taken from a fasta file(a genome, a gene etc.)
// Use the sliding window tehnique to measure the relative frequencies over the input sequence(The sequence found in the fasta file) The output of this step
// Should be a line chart that contains a line of each output from the brute force engine
// Detect the peaks for each combination over the main sequence
// 2 Results are needed in this: case one "A number that a (The marker can be geometrical shape)";
// case 2: A sequence in the console that shows the combination and the position of the peak inside the sequence
std::vector<std::string> createAllCombinations()
{
    std::vector<std::string> allCombinations;
    constexpr char sequence[] = {'A', 'C', 'T', 'G'};

    for (size_t i = 0; i < 4; ++i)
    {
        for (size_t j = 0; j < 4; ++j)
        {
            allCombinations.push_back(std::string{sequence[i], sequence[j]});
        }
    }

    for (size_t i = 0; i < 4; ++i)
    {
        for (size_t j = 0; j < 4; ++j)
        {
            for (size_t k = 0; k < 4; ++k)
            {
                allCombinations.push_back(std::string{sequence[i], sequence[j], sequence[k]});
            }
        }
    }

    return allCombinations;
}

void calculatePercentage(const std::vector<std::string> &combinations, const std::string &file)
{
    std::unordered_map<std::string, float> map;
    std::ifstream input(file);
    std::string line;
    for (const std::string &s : combinations)
    {
        map[s] = 0;
    }
    bool firstLine = true;
    char last = '\0', secondLast = '\0';
    std::vector<std::unordered_map<std::string, float>> maps;
    size_t totalSize = 0;

    while (std::getline(input, line))
    {
        if (line[0] == '>')
        {
            for (const auto &pair : map)
            {
                totalSize += pair.second;
            }

            for (auto &pair : map)
            {
                pair.second = totalSize > 0 ? (pair.second / totalSize * 100) : 0;
            }

            maps.push_back(map);
            totalSize = 0;
            for (const std::string &s : combinations)
            {
                map[s] = 0;
            }

            std::cout << "Completed One Genome\n";
            continue;
        }

        if (!firstLine && line.size() > 2)
        {
            ++map[std::string{last, line[0], line[1]}];
            ++map[std::string{secondLast, last, line[0]}];
        }

        if(line.size() > 2)
        {
            for (size_t i = 0; i < line.size() - 1; ++i)
            {
                ++map[std::string{line.at(i), line.at(i + 1)}];
            }

            for (size_t i = 0; i < line.size() - 2; ++i)
            {
                ++map[std::string{line.at(i), line.at(i+ 1), line.at(i + 2)}];
            }

        }

        if (line.size() >= 2)
        {
            last = line.at(line.size() - 1);
            secondLast = line.at(line.size() - 2);
        }
        else if (line.size() == 1)
        {
            last = line.at(0);
        }
        firstLine = false;
    }

    std::ofstream out("out.csv");

    for(const auto& map :  maps)
    {
        for(const auto& pair : map)
        {
            out << pair.first << ',' << pair.second << ','; 
        }

        out << '\n';
    }

    out.close();
}


int main()
{
    std::vector<std::string> allCombinations = createAllCombinations();

    for (const std::string &str : allCombinations)
    {
        std::cout << str << '\n';
    }

    calculatePercentage(allCombinations, "plastid.1.2.genomic.fna");
}
