#include <iostream>
#include <unordered_map>
#include <fstream>
#include <algorithm>
/*
Design an appplication that is able to detect repetitive sub-sequences(patterns) between 3-8 letters.
Use a chromozome from homo-sapiens(ex: chromosome 14)
Your application MUST be able to show a list with the most frequent repetitions and provide a bar chart
with their relative frequencies.
Also the console MUST show the positions of these repetitive regions across chromosome 14.
The length of these repetitions should be of at least 20 repetitions in sequence
(example: ATGATGATG ... ATG)
*/

void processChunk(const std::string &chunk, std::unordered_map<std::string, int> &patternCount, int minLength, int maxLength);

void processChunk(const std::string &chunk, std::unordered_map<std::string, int> &patternCount, int minLength, int maxLength)
{
    int n = chunk.size();
    for (int len = minLength; len <= maxLength; ++len)
    {
        for (int i = 0; i <= n - len; ++i)
        {
            std::string subPattern = chunk.substr(i, len);
            
            subPattern.erase(std::remove(subPattern.begin(), subPattern.end(), '\n'), subPattern.end());
            
            if (subPattern.length() == len) {
                patternCount[subPattern]++;
            }
        }
    }
}


int main()
{
    constexpr size_t minLength = 3, maxLength = 8;
    const std::string filename = "chromosome14";

    std::ifstream file(filename);
    if (!file)
    {
        std::cerr << "Could not open the file!\n";
        return 1;
    }

    std::unordered_map<std::string, int> patternCount;
    const size_t bufferSize = 1024;
    std::string buffer;
    buffer.reserve(bufferSize + maxLength - 1);

    while (file)
    {
        char chunk[bufferSize];
        file.read(chunk, bufferSize);
        std::streamsize bytesRead = file.gcount();
        buffer.append(chunk, bytesRead);

        if (buffer.size() < minLength)
            break;

        processChunk(buffer, patternCount, minLength, maxLength);

        buffer = buffer.substr(buffer.size() - (maxLength - 1));
    }

    std::ofstream out("out.csv");
    
    bool found = false;
    for (const auto &pair : patternCount)
    {
        if (pair.second > 1)
        {
            found = true;
            out << pair.first << ", " << pair.second << "\n";
        }
    }

    if (!found)
    {
        std::cout << "No repeated patterns of specified lengths found.\n";
    }

}