#include <iostream>
#include <vector>
#include <string>
#include <unordered_map>
#include <map>
#include <fstream>
#include <optional>
#include <cmath>

/*
1. The melting tempereture (TM) is themperature at wtich 0.5% of a particular DNA duplex will dissociate and become a single strand of DNA. Primel length
and sequence are of crucial importance in desiging the parameters of a successful amplification. The melting temperature (TM) of a nucleic acid duplex
increases both with its length and with increasing CG content. A simple formula for calculating TM  is: TM = 4(G+C) + 2(A + T)
The actual TM is influenced by the concentration of differite ions(Mg2+, k+) and cosolvents.
An alternativ formula is: TM = 81.5 + 16.6(log10(Na+)) + 0.41* (%GC) - 600/length
Input = string DNA up to 15 boses
Output = temp in C
*/

/*
Implement an algorithm that converts the coding region of a gene into an amino acid sequence. In order to start, use the genetic code (The table is present on
curs.upb.ro). Note that the translation process MUST start with an ATG codon, so the reading frame is based on a multiple of 3 letters until a stop codon is encountered.
Note that there are theree versions of a stop codon, please see the table. The input of this appliocation will be a DNA sequence and the output be an aminoacid
sequece, each amino acid being represented using single letters.
Look on the internet for the amino acid table that shows single letter reprezentations for aminoacids
*/
consteval static std::array<size_t, 4> percentceAll(const std::string_view str);
static void calculateTemp();
static void translate();

consteval std::array<size_t, 4> percentceAll(const std::string_view str)
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

    return freqArr;
}

void calculateTemp()
{
    constexpr std::string_view str = "GCATGCAGTTTAAA";
    constexpr auto percent = percentceAll(str);

    std::cout << (4.f * (percent[1] + percent[3]) + 2.f * (percent[0] + percent[2])) << '\n';
    constexpr float percG = (static_cast<float>(percent[1]) / str.size()) * 100.f, percC = (static_cast<float>(percent[3]) / str.size()) * 100.f;
    std::cout << 81.5f - 16.6f + (0.41f * (percG + percC)) - 600.f / str.size() << '\n';
}

consteval int offsetStart(const std::string_view str)
{
    for (size_t i = 0; i < str.size() - 2; ++i)
    {
        if (str[i] == 'A' && str[i + 1] == 'T' && str[i + 2] == 'G')
            return i;
    }

    return -1;
}

consteval int offsetEnd(const std::string_view str)
{
    // UAA, UGA, UAG
    for (size_t i = 0; i < str.size() - 2; ++i)
    {
        if ((str[i] == 'T' && str[i + 1] == 'A' && str[i + 2] == 'A') || (str[i] == 'T' && str[i + 1] == 'G' && str[i + 2] == 'A') || (str[i] == 'T' && str[i + 1] == 'G' && str[i + 2] == 'G'))
            return i;
    }

    return -1;
}

void translate()
{
    constexpr std::string_view str = "AAAATGCCGCGGCACAACCGCGATAAGGG";
    constexpr int startOffset = offsetStart(str);

    if (startOffset == -1)
    {
        std::cerr << "I cant find start point\n";
        return;
    }

    constexpr int endOffset = offsetEnd(str.begin() + (startOffset + 3));

    const std::unordered_map<std::string_view, std::string_view> geneticCode =
    {
        {"TTT", "F"}, {"TTC", "F"}, {"TTA", "L"}, {"TTG", "L"},
        {"CTT", "L"}, {"CTC", "L"}, {"CTA", "L"}, {"CTG", "L"},
        {"ATT", "I"}, {"ATC", "I"}, {"ATA", "I"}, {"ATG", "M"},
        {"GTT", "V"}, {"GTC", "V"}, {"GTA", "V"}, {"GTG", "V"},
        {"TCT", "S"}, {"TCC", "S"}, {"TCA", "S"}, {"TCG", "S"},
        {"CCT", "P"}, {"CCC", "P"}, {"CCA", "P"}, {"CCG", "P"},
        {"ACT", "T"}, {"ACC", "T"}, {"ACA", "T"}, {"ACG", "T"},
        {"GCT", "A"}, {"GCC", "A"}, {"GCA", "A"}, {"GCG", "A"},
        {"TAT", "Y"}, {"TAC", "Y"}, {"TAA", "STOP"}, {"TAG", "STOP"},
        {"CAT", "H"}, {"CAC", "H"}, {"CAA", "Q"}, {"CAG", "Q"},
        {"AAT", "N"}, {"AAC", "N"}, {"AAA", "K"}, {"AAG", "K"},
        {"GAT", "D"}, {"GAC", "D"}, {"GAA", "E"}, {"GAG", "E"},
        {"TGT", "C"}, {"TGC", "C"}, {"TGA", "STOP"}, {"TGG", "W"},
        {"CGT", "R"}, {"CGC", "R"}, {"CGA", "R"}, {"CGG", "R"},
        {"AGT", "S"}, {"AGC", "S"}, {"AGA", "R"}, {"AGG", "R"},
        {"GGT", "G"}, {"GGC", "G"}, {"GGA", "G"}, {"GGG", "G"}
    };

    for (size_t i = startOffset; i + 2 < endOffset; i += 3) 
    {
        std::string_view codon = str.substr(i, 3); 

        auto it = geneticCode.find(codon);
        if (it != geneticCode.end()) 
        {
            std::cout << it->second;
        } else
        {
            std::cout << "?"; 
        }
    }

}

int main()
{
    calculateTemp();
    translate();
}
