#include <iostream>
#include <string>
#include <vector>
#include <chrono>
#include <random>
#include "names.h"

using namespace std;

void generateSeeds(const int seedPairs);
void cycle();

int firstNamesLengthMale = maleBritishFirstNames.size();
int firstNamesLengthFemale = femaleBritishFirstNames.size();
int surnamesLength = britishSurnames.size();

class Human;
static std::vector<Human> entities;

class Relationship
{
private:
    /* data */
public:
    int relationId = -1;
    std::pair<string, string> relationName;
    int relationshipLength = 0;
};

class Human
{
    static const int breedAge = 18;
    static const int pregnancyLength = 12;
    static const int maxAge = 45;

private:
    /* data */
public:
    int id;
    bool isDead = false;
    std::pair<string, string> name;
    std::pair<string, string> maidenName;
    std::pair<int, int> age = {0, 0};
    char gender;
    bool isSeed = false;
    bool isMarried = false;
    bool hasPartner = false;
    bool hasLover = false;
    Relationship marriedTo;
    Relationship partner;
    Relationship lover;
    bool isPregnant = false;
    int currentPregnancyLength = 0;
    int maxChildren = rand() % 5;
    std::vector<int> children;

    static int ID;
    Human()
    {
        id = ID++;
    }

    void printStats()
    {
        std::cout << "----------------------------------------" << std::endl;
        std::cout << "id: " << id << std::endl;
        std::cout << "isDead: " << isDead << std::endl;
        std::cout << "name: " << name.first << " " << name.second << std::endl;
        std::cout << "gender: " << gender << std::endl;
        std::cout << "age: " << age.first << " Y"
                  << " " << age.second << " M" << std::endl;
        std::cout << "marriedTo: " << marriedTo.relationName.first << " partner: "
                  << partner.relationName.first << " lover: " << lover.relationName.first << " " << lover.relationName.second << std::endl;
        std::cout << "length: " << marriedTo.relationshipLength << " length: "
                  << partner.relationshipLength << " length: " << lover.relationshipLength << std::endl;
        std::cout << "isPregnant: " << isPregnant << " pregnancyLength: " << currentPregnancyLength << "/" << pregnancyLength << std::endl;
        std::cout << "children: " << children.size() << "/" << maxChildren << std::endl;
        for (size_t i = 0; i < children.size(); i++)
        {
            std::cout << entities[children[i]].name.first << " " << entities[children[i]].name.second << std::endl;
        }
    };

    void relationshipCheck(std::vector<int> &singleMale, std::vector<int> &singleFemale)
    {
        if (hasLover)
        {
            lover.relationshipLength = lover.relationshipLength + 1;
        }
        if (hasPartner)
        {
            partner.relationshipLength = partner.relationshipLength + 1;
        }
        if (isMarried)
        {
            marriedTo.relationshipLength = marriedTo.relationshipLength + 1;
        }

        if (!hasLover && age.first >= breedAge)
        {
            // pick lover from filtered list.

            if (gender == 'm' && !singleFemale.empty())
            {
                int choice = singleFemale[rand() % singleFemale.size()];
                hasLover = true;
                lover.relationName = entities[choice].name;
                lover.relationId = entities[choice].id;
            }
            else if (gender == 'f' && !singleMale.empty())
            {
                int choice = singleMale[rand() % singleMale.size()];
                hasLover = true;
                lover.relationName = entities[choice].name;
                lover.relationId = entities[choice].id;
            }
        }
    };
    void deathCheck()
    {

        if (!isDead && age.first >= maxAge)
        {
            if (isPregnant && currentPregnancyLength >= 8)
            {
                giveBirth();
            }
            isDead = true;
        }
        else
        {
            if (age.second >= 12)
            {
                std::pair<int, int> newAge;
                newAge.first = age.first + 1;
                newAge.second = 0;
                age = newAge;
            }
            else
            {
                std::pair<int, int> newAge = age;
                newAge.second = age.second + 1;
                age = newAge;
            }
        }
    };

    void pregnancyCheck()
    {

        if (gender == 'm')
            return;
        int childAmount = children.size();

        if (childAmount >= maxChildren)
            return;

        else if (age.first >= breedAge && !isPregnant && hasLover)
        {
            int chance = rand() % 100;
            if (childAmount == 0 && chance >= 20)
            {
                isPregnant = true;
                currentPregnancyLength = 0;
            }
            else if (childAmount == 1 && chance >= 40)
            {
                isPregnant = true;
                currentPregnancyLength = 0;
            }
            else if (childAmount >= 2 && chance >= 70)
            {
                isPregnant = true;
                currentPregnancyLength = 0;
            }
        }
        else if (currentPregnancyLength >= pregnancyLength)
        {
            isPregnant = false;
            currentPregnancyLength = 0;

            giveBirth();
        }
        else if (isPregnant)
        {
            currentPregnancyLength = currentPregnancyLength + 1;
        }
    };

    void giveBirth()
    {
        Human human;
        human.gender = getGender();
        human.getName();
        std::pair<string, string> newName = human.name;
        newName.second = lover.relationName.second;
        human.name = newName;
        int humanID = human.id;
        children.push_back(humanID);
        entities.push_back(human);
    };
    void singleCycle(std::vector<int> &singleMale, std::vector<int> &singleFemale)
    {
        if (isDead)
        {
            return;
        }
        else
        {
            relationshipCheck(singleMale, singleFemale);
            deathCheck();
            pregnancyCheck();
        }
    };
    char getGender()
    {
        int randomNumber;

        randomNumber = 1 + rand() % 2;
        if (randomNumber == 1)
            return 'm';
        else
            return 'f';
    };
    void getName()
    {
        if (gender == 'm')
        {
            int firstName = rand() % firstNamesLengthMale;
            int lastName = rand() % surnamesLength;
            name = std::make_pair(maleBritishFirstNames[firstName], britishSurnames[lastName]);
        }
        else
        {
            int firstName = rand() % firstNamesLengthFemale;
            int lastName = rand() % surnamesLength;
            maidenName = std::make_pair(femaleBritishFirstNames[firstName], britishSurnames[lastName]);
            name = maidenName;
        }
    };
    void getAge()
    {
        int randomAge = (rand() % 10) + breedAge;
        age = std::make_pair(randomAge, 0);
    }
};

int Human::ID = 0;

int main()
{
    std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
    int seedPairs = 50;
    int months = 1200;

    generateSeeds(seedPairs);
    for (size_t i = 0; i < months; i++)
    {
        std::cout << "cycle: " << i << std::endl;
        cycle();
    }
    int entitiesLength = entities.size();
    // for (size_t i = 0; i < entitiesLength; i++)
    // {
    //     entities[i].printStats();
    // }

    std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
    std::cout << "Time difference (sec) = " << (std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count()) / 1000000.0 << std::endl;
    int areDead = 0;
    for (size_t i = 0; i < entities.size(); i++)
    {
        if (entities[i].isDead)
        {
            areDead++;
        }
    }
    std::cout << "months: " << months << std::endl;
    std::cout << "livingTotal: " << entities.size() - areDead << std::endl;
    std::cout << "deadTotal: " << areDead << std::endl;
    std::cout << "entitiesTotal: " << entities.size() << std::endl;
    return 0;
}

void cycle()
{
    std::vector<Human> entitiesCopy;
    std::vector<int> singleMale;
    std::vector<int> singleFemale;
    for (int i = 0; i < entities.size(); i++)
    {
        if (entities[i].gender == 'm' && !entities[i].isDead)
        {
            singleMale.push_back(entities[i].id);
        }
        else if (entities[i].gender == 'f' && !entities[i].isDead)
        {
            singleFemale.push_back(entities[i].id);
        }
        entitiesCopy.push_back(entities[i]);
    }
    int entitiesCopyLength = entitiesCopy.size();

    for (size_t i = 0; i < entitiesCopyLength; i++)
    {
        if (entities[i].isDead)
        {
            continue;
        }
        entities[i].singleCycle(singleMale, singleFemale);
    }
}

void generateSeeds(const int seedPairs)
{

    for (size_t i = 0; i < seedPairs; i++)
    {
        // this can become an initialise function.
        Human humanM;
        Human humanF;
        humanM.gender = 'm';
        humanM.getName();
        humanM.getAge();
        humanM.isSeed = true;
        humanF.gender = 'f';
        humanF.getName();
        humanF.getAge();
        humanF.isSeed = true;
        humanM.isMarried = true;
        humanM.hasPartner = true;
        humanM.hasLover = true;
        humanM.marriedTo.relationName = humanF.name;
        humanM.partner.relationName = humanF.name;
        humanF.name.second = humanM.name.second;
        humanM.lover.relationName = humanF.name;
        humanF.isMarried = true;
        humanF.hasPartner = true;
        humanF.hasLover = true;
        humanF.marriedTo.relationName = humanM.name;
        humanF.partner.relationName = humanM.name;
        humanF.lover.relationName = humanM.name;

        entities.push_back(humanM);
        entities.push_back(humanF);
    }
}
