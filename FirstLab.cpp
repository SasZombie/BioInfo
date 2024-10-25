#include <iostream>
#include <vector>
#include <sstream>
#include <string>
#include <fstream>
#include <iomanip>
#include <cmath>
#include <cctype>
#include <tuple>
#include <unordered_map>

std::string formatPercentage(float value, int decimalPlaces);
std::array<float, 4> percentceAll(const std::string &str);
std::vector<std::array<float, 4>>  readMultipleGenmoe(const std::string &path);

std::string formatPercentage(float value, int decimalPlaces)
{

    value *= 100;

    float scale = std::pow(10, decimalPlaces);
    value = std::trunc(value * scale) / scale;

    std::ostringstream stream;
    stream << std::fixed << std::setprecision(decimalPlaces) << value << "%";
    return stream.str();
}


std::array<float, 4> percentceAll(const std::vector<char> &str)
{
    std::array<size_t, 4> freqArr = {0, 0, 0, 0};

    for (const char c : str)
    {
        switch (c)
        {
        case 'A':
            ++freqArr[0];
            break;
        case 'C':
            ++freqArr[1];
            break;
        case 'T':
            ++freqArr[2];
            break;
        case 'G':
            ++freqArr[3];
            break;
        default:
            break;
        }
    }

    std::array<float, 4> percentMap = {0.0f, 0.0f, 0.0f, 0.0f};

    if (!str.empty())
    {
        float totalSize = static_cast<float>(str.size());

        percentMap[0] = (static_cast<float>(freqArr[0]) / totalSize) * 100.0f;
        percentMap[1] = (static_cast<float>(freqArr[1]) / totalSize) * 100.0f;
        percentMap[2] = (static_cast<float>(freqArr[2]) / totalSize) * 100.0f;
        percentMap[3] = (static_cast<float>(freqArr[3]) / totalSize) * 100.0f;
    }

    return percentMap;
}

std::vector<std::array<float, 4>> readMultipleGenmoe(const std::string &path)
{
    std::ifstream file(path);

    if (!file.is_open())
    {
        std::cerr << "Could not open file " << path << '\n';
        exit(EXIT_FAILURE);
    }

    std::string line;
    std::vector<std::array<float, 4>> percentes, percAll;
    std::vector<char> processed;
    std::getline(file, line);
    while (std::getline(file, line))
    {
        if (line.empty())
        {
            std::cout << "Processed One Genome\n";
            percentes.push_back(percentceAll(processed));
            processed.clear();
            std::getline(file, line);
        }
        else
        {
            processed.reserve(processed.size() + line.size());
            std::copy(line.begin(), line.end(), std::back_inserter(processed));
        }
    }

    return percentes;
}

int main()
{
    std::ofstream out("out.csv");
    std::ofstream out2("sliding.csv");

    auto lists = readMultipleGenmoe("multipleFasta.fasta");

    constexpr char genomeLetters[] = {'A', 'C', 'T', 'G'};

    for (const auto &arrs : lists)
    {
        for (size_t i = 0; i < 4; ++i)
        {
            out << genomeLetters[i] << ',' << arrs[i] << ',';
        }
        out << '\n';
    }

    out.close();
    out2.close();

    system("python3 PlotingFromFile.py");
}