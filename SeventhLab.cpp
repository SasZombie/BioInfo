#include <iostream>
#include <fstream>
#include <unordered_map>

void processChunk(const std::string &chunk, std::ofstream& out);

void processChunkWithPattern(std::string chunk, const std::string &pattern, size_t associatedNumber, std::ofstream &out)
{
    size_t pos = chunk.find(pattern);

    while (pos != std::string::npos)
    {
        out << chunk.substr(0, pos) << ',';

        std::string remainingChunk = chunk.substr(pos + pattern.size());
        std::cout << "Rest of the string: " << remainingChunk << '\n';

        std::cout << "-----\n";
        chunk = remainingChunk;

        processChunkWithPattern(remainingChunk, pattern, associatedNumber, out);

        return;
    }
}

void processChunk(const std::string &chunk, std::ofstream& out)
{
    static const std::unordered_map<std::string, size_t> listOfRe = {
        {"ATTCCTAC", 7},
        {"CCTCC", 2},
        {"ATGGGA", 3},
        {"AGCCA", 3},
        {"TTTCA", 3},
        {"GGCCTTT", 1}};

    std::string currentChunk = chunk;

    for (const auto &patternPair : listOfRe)
    {
        const std::string &pattern = patternPair.first;
        size_t associatedNumber = patternPair.second;
        std::cout << "Searching for pattern: " << pattern << '\n';
        processChunkWithPattern(currentChunk, pattern, associatedNumber, out);
        std::cout << "Done Searching!\n=============================================================\n";
    }
}

int main()
{
    constexpr size_t maxSize = 8, minSize = 5;

    std::ifstream file("chromosome14");
    std::ofstream out("out");

    if (!file.is_open())
    {
        std::cerr << "Cannot find file\n";
        return -1;
    }

    const size_t bufferSize = 1024;
    size_t chunkNumber = 0;
    std::string buffer;
    buffer.reserve(bufferSize + maxSize - 1);

    while (file)
    {
        char chunk[bufferSize];
        file.read(chunk, bufferSize);
        std::streamsize bytesRead = file.gcount();
        buffer.append(chunk, bytesRead);

        if (buffer.size() < minSize)
            break;

        buffer.erase(std::remove_if(buffer.begin(), buffer.end(), [](unsigned char c)
                                    { return c == '\n' || c == '\r'; }),
                     buffer.end());

        std::cout << "Processing chunk " << chunkNumber << '\n';
        processChunk(buffer, out);
        std::cout << "Done with chunk " << ++chunkNumber << "\n++++++++++++++++++++++++++++++++++++++++++++++++++++++";
        buffer = buffer.substr(buffer.size() - (maxSize - 1));
    }
}