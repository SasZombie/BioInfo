#include <iostream>
#include <string>
#include <random>
#include <algorithm>
#include <tuple>
#include <vector>

std::string generateSegment(std::mt19937 &gen, std::uniform_int_distribution<> &nucleotideDist) noexcept;
std::string generateFullSequence(size_t fullLength, std::mt19937 &gen, std::uniform_int_distribution<> &nucleotideDist) noexcept;
std::string getSequence(size_t sequenceLength, std::mt19937 &gen, std::uniform_int_distribution<> &nucleotideDist) noexcept;

std::string getSequence(size_t sequenceLength, std::mt19937 &gen, std::uniform_int_distribution<> &nucleotideDist) noexcept
{
    static constexpr char nucleotides[] = {'A', 'C', 'T', 'G'};
    std::string sequence(sequenceLength, ' ');

    for (size_t i = 0; i < sequenceLength; ++i)
    {
        sequence[i] = nucleotides[nucleotideDist(gen)];
    }

    return sequence;
}

std::string mirrorSequence(const std::string &sequence)
{
    std::string mirrored;
    for (char base : sequence)
    {
        switch (base)
        {
        case 'A':
            mirrored += 'T';
            break;
        case 'C':
            mirrored += 'G';
            break;
        case 'G':
            mirrored += 'C';
            break;
        case 'T':
            mirrored += 'A';
            break;
        default:
            mirrored += base;
            break;
        }
    }
    return mirrored;
}

std::string generateSegment(std::mt19937 &gen, std::uniform_int_distribution<> &nucleotideDist) noexcept
{
    const size_t firstHalfLength = 8;

    const size_t middleLength = 2 + (gen() % 19);

    const std::string firstHalf = getSequence(firstHalfLength, gen, nucleotideDist);

    const std::string middlePart = getSequence(middleLength, gen, nucleotideDist);

    std::string secondHalf = firstHalf.substr(0, firstHalfLength);
    secondHalf = mirrorSequence(secondHalf);

    const std::string segment = firstHalf + middlePart + secondHalf;

    return segment;
}

std::string generateFullSequence(size_t fullLength, std::mt19937 &gen, std::uniform_int_distribution<> &nucleotideDist) noexcept
{
    std::string fullSequence;

    while (fullSequence.size() < fullLength)
    {
        std::string segment = generateSegment(gen, nucleotideDist);
        if (fullSequence.size() + segment.size() > fullLength)
        {
            segment = segment.substr(0, fullLength - fullSequence.size());
        }
        fullSequence += segment;
    }

    return fullSequence;
}

std::vector<std::tuple<size_t, std::string, std::string>> findInversePatterns(const std::string &fullSequence, size_t segmentLength = 8)
{
    std::vector<std::tuple<size_t, std::string, std::string>> patterns;

    size_t sequenceLength = fullSequence.size();

    for (size_t i = 0; i <= sequenceLength - segmentLength; ++i)
    {
        std::string segment = fullSequence.substr(i, segmentLength);

        std::string mirroredSegment = mirrorSequence(segment);

        for (size_t j = i + segmentLength; j <= sequenceLength - segmentLength; ++j)
        {
            std::string middlePart = fullSequence.substr(i + segmentLength, j - (i + segmentLength));
            std::string potentialMirroredSegment = fullSequence.substr(j, segmentLength);

            if (potentialMirroredSegment == mirroredSegment)
            {
                patterns.push_back(std::make_tuple(i, middlePart, potentialMirroredSegment));
            }
        }
    }

    return patterns;
}

int main()
{
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> nucleotideDist(0, 3);

    constexpr size_t fullLength = 400;
    const std::string fullSequence = generateFullSequence(fullLength, gen, nucleotideDist);

    std::cout << "Generated Sequence Length: " << fullSequence.size() << '\n';

    auto patterns = findInversePatterns(fullSequence);

    std::cout << "Found inverse patterns: \n";

    for (const auto &pattern : patterns)
    {
        size_t idx;
        std::string middlePart, mirroredSegment;
        std::tie(idx, middlePart, mirroredSegment) = pattern;

        std::cout << "Pattern starting at index " << idx << ": "
                  << "segment + [" << middlePart << "] + " << mirroredSegment << std::endl;
    }
}
