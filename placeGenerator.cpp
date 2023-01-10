#include "placeNames.h"
#include <vector>
#include <iostream>
#include <string>
#include <time.h>
static std::vector<std::string> generateMultipleEnglishPlaceName(int &placeAmount);
int genName(int argc, char const *argv[])
{
    srand(time(NULL));
    int placeAmount = 300;
    std::vector<std::string> names = generateMultipleEnglishPlaceName(placeAmount);
    int length = names.size();
    for (size_t i = 0; i < length; i++)
    {
        std::string t = names[i];
        std::cout << t << std::endl;
    }

    return 0;
}

std::string generateSinglePlaceName()
{
    int nameOneLength = sizeof(_name_original_english_1) / sizeof(_name_original_english_1[0]);
    int nameTwoLength = sizeof(_name_original_english_2) / sizeof(_name_original_english_2[0]);
    int nameThreeLength = sizeof(_name_original_english_3) / sizeof(_name_original_english_3[0]);
    int nameFourLength = sizeof(_name_original_english_4) / sizeof(_name_original_english_4[0]);
    int nameFiveLength = sizeof(_name_original_english_5) / sizeof(_name_original_english_5[0]);
    int nameSixLength = sizeof(_name_original_english_6) / sizeof(_name_original_english_6[0]);

    std::string placeName = "";

    if (rand() % 100 >= 70)
    {
        /* optional first segment */
        int choice1 = rand() % nameOneLength;
        placeName.append(_name_original_english_1[choice1]);
    }

    /* mandatory middle segments */
    int choice2 = rand() % nameTwoLength;
    int choice3 = rand() % nameThreeLength;
    int choice4 = rand() % nameFourLength;
    int choice5 = rand() % nameFiveLength;

    placeName.append(_name_original_english_2[choice2]);
    placeName.append(_name_original_english_3[choice3]);
    placeName.append(_name_original_english_4[choice4]);
    placeName.append(_name_original_english_5[choice5]);

    if (rand() % 100 >= 70)
    {
        /* optional last segment */
        int choice6 = rand() % nameSixLength;
        placeName.append(_name_original_english_6[choice6]);
    }
    /* Ce, Ci => Ke, Ki */
    if (placeName[0] == 'C' && (placeName[1] == 'e' || placeName[1] == 'i'))
    {
        placeName[0] = 'K';
    }

    return placeName;
}

std::vector<std::string> generateMultipleEnglishPlaceName(int &placeAmount)
{

    std::vector<std::string> placeNames;
    int nameOneLength = sizeof(_name_original_english_1) / sizeof(_name_original_english_1[0]);
    int nameTwoLength = sizeof(_name_original_english_2) / sizeof(_name_original_english_2[0]);
    int nameThreeLength = sizeof(_name_original_english_3) / sizeof(_name_original_english_3[0]);
    int nameFourLength = sizeof(_name_original_english_4) / sizeof(_name_original_english_4[0]);
    int nameFiveLength = sizeof(_name_original_english_5) / sizeof(_name_original_english_5[0]);
    int nameSixLength = sizeof(_name_original_english_6) / sizeof(_name_original_english_6[0]);

    for (size_t i = 0; i < placeAmount; i++)
    {
        std::string placeName = "";

        if (rand() % 100 >= 70)
        {
            /* optional first segment */
            int choice1 = rand() % nameOneLength;
            placeName.append(_name_original_english_1[choice1]);
        }

        /* mandatory middle segments */
        int choice2 = rand() % nameTwoLength;
        int choice3 = rand() % nameThreeLength;
        int choice4 = rand() % nameFourLength;
        int choice5 = rand() % nameFiveLength;

        placeName.append(_name_original_english_2[choice2]);
        placeName.append(_name_original_english_3[choice3]);
        placeName.append(_name_original_english_4[choice4]);
        placeName.append(_name_original_english_5[choice5]);

        if (rand() % 100 >= 70)
        {
            /* optional last segment */
            int choice6 = rand() % nameSixLength;
            placeName.append(_name_original_english_6[choice6]);
        }
        /* Ce, Ci => Ke, Ki */
        if (placeName[0] == 'C' && (placeName[1] == 'e' || placeName[1] == 'i'))
        {
            placeName[0] = 'K';
        }
        placeNames.push_back(placeName);
    }

    return placeNames;
}
