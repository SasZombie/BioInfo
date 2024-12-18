#include <iostream>
#include <string>
#include <string_view>
#include <unordered_map>
#include <vector>
#include <algorithm>
#include <fstream>

double CpGContent(std::string_view sequence, size_t windowSize) noexcept;
double calculateIC(std::string_view input) noexcept;
bool isGC(char c) noexcept;

bool isGC(char c) noexcept
{
    return (c == 'G' || c == 'C');
}

double calculateIC(std::string_view sequence) noexcept
{
    size_t n = sequence.size();
    double ic_sum = 0.0;

    for (size_t u = 1; u < n; ++u)
    {
        size_t match_count = 0;

        for (size_t i = 0; i < n - u; ++i)
        {
            if (sequence[i] == sequence[i + u])
            {
                match_count++;
            }
        }
        ic_sum += (static_cast<double>(match_count) / (n - u)) * 100;
    }

    double ic = 100 - (ic_sum / (n - 1));
    return ic;
}

double CpGContent(std::string_view sequence, size_t windowSize = 30) noexcept
{
    if (sequence.empty())
        return 0;

    size_t cAppearance = std::count(sequence.begin(), sequence.end(), 'C');
    size_t gAppearance = std::count(sequence.begin(), sequence.end(), 'G');

    return ((static_cast<float>(cAppearance) + static_cast<float>(gAppearance)) / sequence.size()) * 100.f;
}

void writeFile(std::string_view input, std::string_view output)
{
    std::ifstream file(input.data());

    if (!file.is_open())
    {
        std::cerr << "File not found\n";
        exit(EXIT_FAILURE);
    }

    std::ofstream out(output.data());
    out << "CG_Content,IC\n";

    std::string line;
    std::vector<double> cgVals, icVals;

    while (std::getline(file, line))
    {
        if (line[0] == '>')
            continue;

        line.erase(std::remove_if(line.begin(), line.end(), [](unsigned char c)
                                  { return c == '\n' || c == '\r'; }),
                   line.end());

        cgVals.push_back(CpGContent(line));
        icVals.push_back(calculateIC(line));
    }
    
    for (size_t i = 0; i < cgVals.size() - 1; ++i)
    {
        out << cgVals[i] << ',' << icVals[i] << '\n';
    }

    file.close();
    out.close();
}

int main()
{
#if 0
    const std::string sequence = "GTAAAAATCTAGTTTTCCGTACTCTGTCCCCTTTTTAATCCTTCTCTGAATGGGTGTCCTTGATTCACTGACATTTCCTAATTAACCAAGTTCTGCCAAGTTTCACATGAAACTTGGGGAACCGATTTCATTCCCCCAGTATCACCCTGTGGCGCCACCTTCCGGAGCTGTGAGGAAACTCCGGACTTCCCCCCAACACCGCCCCCTCCCTCCACCCTCGGTCTCCGCTTTCTGCGCTCTGCCGCGTTGGTTTTCGGAGGTGTCTGGGCGCATGCGCTTTGGACGGGCCGCCTAGCCTAGGAGAGACTACAATTCCCAGAAGACAGTGCGAAAGAAAAAAAAAATCCCGCGGTCCGTGGGGGTGGGAGAAATAAACGCTCGCGAGAGAACGAGGTTCAGGCGGCTGCACGAAGGGGGTGGAGGGGGGCTGGAGAGAGTGAGGAGGAAGGGGAGGAGGTGCCGTCCCACAATACCAGGCGGGAGGGCGGGTAGGCGGTTTGTATCCGGGCTGTGAGGTGCTCGGAG CCTCGGCGGACCTTGCTGCCTCTGTCTCTTTAACGCGAGAGGAAGCGATGCAGAGGGGTGGAAAATGGCAGAGCT";

    std::vector<double> cgVals, icVals;
    constexpr size_t windowSize = 30;

    for (size_t i = 0; i <= sequence.size() - windowSize; ++i)
    {
        cgVals.push_back(CpGContent(sequence.substr(i, windowSize)));
        icVals.push_back(calculateIC(sequence.substr(i, windowSize)));
    }

    std::ofstream out("newDna.csv");
    out << "CG_Content,IC\n";
    for (size_t i = 0; i < cgVals.size() - 1; ++i)
    {
        out << cgVals[i] << ',' << icVals[i] << '\n';
    }

    out.close();
#else

    writeFile("homosapiens.fasta", "newDna.csv");
    writeFile("Promotori lista completa.fasta", "newDna2.csv");

#endif
}