#include <time.h>
#include <iostream>
#include <vector>
#include <string>

struct LocationAmount
{

    int maxCapitals = 1;
    int maxCities = 4;
    int maxLargeTowns = 15;
    int maxMediumTowns = 25;
    int maxSmallTowns = 50;
    int maxVillages = 60;
    int maxSmallCommunities = 150;

    int currentCapitals = 0;
    int currentCities = 0;
    int currentLargeTowns = 0;
    int currentMediumTowns = 0;
    int currentSmallTowns = 0;
    int currentVillages = 0;
    int currentSmallCommunities = 0;
};

class Location
{
};
class Biome
{
public:
    std::string name;
    std::string type;
};
class BiomeType
{
};

class LocationType
{
public:
    int name;
    char mapIcon;
    int seedPopulation;
};

class Region
{
private:
    /* data */
public:
    int maxHeight = 10;
    int maxWidth = 10;
    Biome biome;
    Biome subBiome;

    std::vector<std::vector<char>> map;
    char mapIcon = ' ';

    Region(std::string locationType)
    {
        initMap();
        if (locationType == "capital")
        {
            mapIcon = 'C';
            biome.type = "settlement";
            map[maxHeight / 2][maxWidth / 2] = 'C';
        }
        else if (locationType == "default")
        {
            mapIcon = ' ';
            biome.type = "water";
        }
        else
        {
            mapIcon = 'X';
            biome.type = "fields";
        }
    };
    void initMap()
    {
        for (size_t i = 0; i < maxHeight; i++)
        {
            std::vector<char> row;
            for (size_t j = 0; j < maxWidth; j++)
            {
                // if()

                row.push_back(' ');
            }

            {

                map.push_back(row);
            }
        }
    };
};

void printMap(std::vector<std::vector<Region>> &map);
void drunkenWalk(std::vector<std::vector<Region>> &map, int &mapHeight, int &mapWidth);

struct Direction
{
    char direction;
    std::pair<int, int> coords;
};

int main(int argc, char const *argv[])
{

    int mapHeight = 40;
    int mapWidth = 30;
    std::vector<std::vector<Region>> worldMap;

    for (size_t i = 0; i < mapHeight; i++)
    {
        std::vector<Region> row;
        for (size_t j = 0; j < mapWidth; j++)
        {
            Region newRegion("default");
            row.push_back(newRegion);
        }

        {

            worldMap.push_back(row);
        }
    }
    drunkenWalk(worldMap, mapHeight, mapWidth);
    printMap(worldMap);
    std::cout << "Finished" << std::endl;
    return 0;
}

void drunkenWalk(std::vector<std::vector<Region>> &map, int &mapHeight, int &mapWidth)
{
    int totalTiles = 2000;
    std::pair<int, int> startLocation{mapHeight / 2, mapWidth / 2};
    std::pair<int, int> currentLocation{mapHeight / 2, mapWidth / 2};

    const Direction directions[] = {{'u', {-1, 0}}, {'d', {1, 0}}, {'l', {0, -1}}, {'r', {0, 1}}};
    srand(time(NULL));

    for (size_t i = 0; i < totalTiles; i++)
    {
        int dir = rand() % 4;
        std::pair<int, int> coords = directions[dir].coords;

        int height = currentLocation.first + coords.first;
        int width = currentLocation.second + coords.second;
        if (height >= mapHeight - 1 || height <= 0)
        {

            int currentHeight = currentLocation.first;
            currentLocation = std::make_pair(currentHeight, width);
        }
        else if (width >= mapWidth - 1 || width <= 0)
        {

            int currentWidth = currentLocation.second;
            currentLocation = std::make_pair(height, currentWidth);
        }
        else
        {

            currentLocation = std::make_pair(height, width);
        }
        Region newRegion("");

        map[currentLocation.first][currentLocation.second] = newRegion;
    }
    Region capital("capital");
    map[startLocation.first][startLocation.second] = capital;
    Region region = map[startLocation.first][startLocation.second];
    for (int i = 0; i < region.map.size(); i++)
    {
        for (int j = 0; j < region.map[i].size(); j++)
        {
            std::cout << region.map[i][j] << " ";
        }
        std::cout << std::endl;
    }
}

void printMap(std::vector<std::vector<Region>> &map)
{
    for (int i = 0; i < map.size(); i++)
    {
        for (int j = 0; j < map[i].size(); j++)
        {
            std::cout << map[i][j].mapIcon << " ";
        }
        std::cout << std::endl;
    }
}
