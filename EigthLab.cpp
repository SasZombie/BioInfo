/*
1
Design a software implementatuon that is able to simulate the shotgun process:
A) Design a module that generates a random dna sequence of 100 000 nucleaotides or less for testing.
B) The output(the random DNA sequence) should be used by another module for collecting samples. :3
Above the output sequence that has been generated. These segments should be dna segments between
1000 and 2000 nucleotides in lentgh. The samples are taken randomly from the input sequencies.
At the end of these sequences you should have 200 000 samples, stored line by line in a text file.
*/

/*
2
Design a software application that uses the sample file as an input. Find a method for this implementation
that aligns the sample one by one in order to detect possible matches. Once a match is found betweeen 2 samples
we consider that a small piece from the original sequence is found, thus this repetitive comparation between samples
will allow us to gradually build up the original sequence. In this assigment you may consider that the original
DNA sequence from the previous assigment is unknown. This current assigment presummes that the sample file
is provided by the shot gun sequencing machine, thus our first assigment was able to emulate the results
of the sequencing machine
*/
#include <iostream>
#include <random>
#include <fstream>
#include <unordered_set>
static std::string getSequence(size_t sequenceLength, std::mt19937 &gen, std::uniform_int_distribution<> &nucleotideDist);
static void createRandomDna();
static size_t getSimilarityCount(std::string_view str1, std::string_view str2);
static size_t getSimilarityIndex(std::string_view str, size_t strIndex, const std::vector<std::string> &lines);
static void concatenate(std::vector<std::string> &lines);

std::string getSequence(size_t sequenceLength, std::mt19937 &gen, std::uniform_int_distribution<> &nucleotideDist)
{
    static constexpr char nucleotides[] = {'A', 'C', 'T', 'G'};
    std::string sequence(sequenceLength, ' ');

    for (size_t i = 0; i < sequenceLength; ++i)
    {
        sequence[i] = nucleotides[nucleotideDist(gen)];
    }

    return sequence;
}

size_t getSimilarityCount(std::string_view str1, std::string_view str2)
{
    std::unordered_set<char> charactersInStr2(str2.begin(), str2.end());
    std::unordered_set<char> counted;
    size_t count = 0;

    for (char c : str1)
    {
        if (charactersInStr2.find(c) != charactersInStr2.end() && counted.find(c) == counted.end())
        {
            count++;
            counted.insert(c);
        }
    }

    return count;
}

size_t getSimilarityIndex(std::string_view str, size_t strIndex, const std::vector<std::string> &lines)
{
    size_t currentMax = 0;
    size_t linesLen = lines.size();
    size_t index = 0;
    for (size_t i = 0; i < linesLen; ++i)
    {
        if (i != strIndex)
        {

            size_t count = getSimilarityCount(str, lines[i]);
            if (count > currentMax)
            {
                currentMax = count;
                index = i;
            }
        }
    }

    return index;
}

void concatenate(std::vector<std::string> &lines)
{
    std::random_device rd;

    std::mt19937 gen(rd());

    while (lines.size() != 1)
    {
        std::uniform_int_distribution<> dist(0, lines.size() - 1);
        size_t randomIndex = dist(gen);

        size_t similarIndex = getSimilarityIndex(lines[randomIndex], randomIndex, lines);
        lines[randomIndex] = lines[randomIndex] + lines[similarIndex];

        lines.erase(lines.begin() + similarIndex);
    }
}

void createRandomDna()
{
    constexpr size_t nucleotidesLength = 1000, sequenceMin = 10, sequenceMax = 20;
    std::ofstream out("out.txt");

    if (!out.is_open())
    {
        std::cerr << "Coud not open file to write!\n";
        exit(EXIT_FAILURE);
    }
    std::random_device rd;

    std::mt19937 gen(rd());
    std::uniform_int_distribution<> lengthDist(sequenceMin, sequenceMax);
    std::uniform_int_distribution<> seqDist(0, 3);

    size_t currentLength = 0;

    while (currentLength < nucleotidesLength)
    {

        size_t seqLen = std::min<size_t>(lengthDist(gen), nucleotidesLength - currentLength);

        out << getSequence(seqLen, gen, seqDist) << '\n';
        currentLength = currentLength + seqLen;
    }

    out.close();
}

int main()
{
    createRandomDna();
    std::ifstream in("out.txt");
    std::vector<std::string> lines;

    if (!in.is_open())
    {
        std::cerr << "Cannot open file to read\n";
        exit(EXIT_FAILURE);
    }

    std::string line;

    while (std::getline(in, line))
    {
        if (!line.empty())
        {
            lines.push_back(line);
        }
    }

    size_t charCount = 0;

    for (const std::string &str : lines)
    {
        charCount = charCount + str.size();
    }
    concatenate(lines);

    std::ofstream out("recreatedGenome.txt");

    out << lines[0];

    out.close();
}