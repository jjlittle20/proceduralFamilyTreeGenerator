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
LocationAmount locationAmount;
class LocationType
{
public:
    std::string displayName;
    std::string icon;
    int seedPopulation;

    LocationType(std::string name, std::string mapIcon, int seedPop)
    {
        displayName = name;
        icon = mapIcon;
        seedPopulation = seedPop;
    };
};
const LocationType locationTypes[] = {{"Capital", "C", 500}, {"City", "c", 250}, {"Large Town", "T", 100}, {"small town", "t", 50}, {"village", "v", 25}, {"small", "s", 10}};

class LocationLookupTable
{
public:
    std::string name;
    int largeTowns;
    int smallTowns;
    int villages;
    int smallCommunities;
    LocationType metaData = locationTypes[0];
};
LocationLookupTable locationLookupTable[] = {{"capital", 5, 3, 5, 5, locationTypes[0]}, {"city", 5, 3, 5, 5, locationTypes[1]}, {"large town", 0, 5, 10, 5, locationTypes[2]}, {"small town", 0, 0, 10, 10, locationTypes[3]}, {"village", 0, 0, 0, 20, locationTypes[4]}, {"small", 0, 0, 0, 0, locationTypes[5]}};

class Biome
{
public:
    std::string name;
    std::string type;
};
class BiomeType
{
};

class Location
{
public:
    std::pair<int, int> coords;
    char mapIcon;
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
    std::vector<Location> locations;
    std::vector<std::vector<char>> map;
    char mapIcon = ' ';
    bool isEmpty = false;

    Region(std::string locationType)
    {
        initMap();
        if (locationType == "capital")
        {
            mapIcon = 'C';
            biome.type = "settlement";
            generateRegionLocations(locationType);
            locationAmount.currentCapitals = 1;
        }
        else if (locationType == "city")
        {
            mapIcon = 'c';
            biome.type = "settlement";
            generateRegionLocations(locationType);
            locationAmount.currentCities = locationAmount.currentCities + 1;
        }
        else if (locationType == "default")
        {
            mapIcon = ' ';
            biome.type = "water";
        }
        else
        {
            mapIcon = '.';
            biome.type = "fields";
        }
        finalizeMap();
        // finalizemap()
    };
    void initMap()
    {
        for (size_t i = 0; i < maxHeight; i++)
        {
            std::vector<char> row;
            for (size_t j = 0; j < maxWidth; j++)
            {

                row.push_back('.');
            }

            {

                map.push_back(row);
            }
        }
    };
    void generateRegionLocations(std::string regionCenter)
    {
        LocationLookupTable centerLocation;
        std::pair<int, int> regionMapCenter = {maxHeight / 2, maxWidth / 2};
        for (size_t i = 0; i < sizeof(locationLookupTable) / sizeof(locationLookupTable[1]); i++)
        {
            if (locationLookupTable[i].name == regionCenter)
            {
                centerLocation = locationLookupTable[i];
            }
        }
        locations.push_back({regionMapCenter, centerLocation.metaData.icon[0]});

        map[regionMapCenter.first][regionMapCenter.second] = centerLocation.metaData.icon[0];
        std::vector<std::pair<int, int>> usedCoords;
        usedCoords.push_back(regionMapCenter);

        for (size_t i = 0; i < centerLocation.villages; i++)
        {
            int x = rand() % maxWidth;
            int y = rand() % maxHeight;
            std::pair<int, int> validCoords = checkCoords(usedCoords, x, y, maxHeight, maxWidth);
            locations.push_back({{x, y}, 'v'});
        }
    }
    std::pair<int, int> checkCoords(
        std::vector<std::pair<int, int>> &usedCoords, int &x, int &y, int maxHeight, int maxWidth)
    {
        for (size_t i = 0; i < usedCoords.size(); i++)
        {
            if (usedCoords[i].first == x && usedCoords[i].second == y)
            {
                if (x >= maxWidth)
                {
                    x = x - 1;
                }
                if (y >= maxWidth)
                {
                    y = y - 1;
                }
                else
                {
                    x = x + 1;
                    y = y + 1;
                }
                return checkCoords(usedCoords, x, y, maxHeight, maxWidth);
            }
            else
            {
                return {x, y};
            }
        }
        // neeed to catch this after return
        return {-1, -1};
    };
    void finalizeMap()
    {
        for (size_t i = 0; i < locations.size(); i++)

        {
            std::pair<int, int> locationCoords = locations[i].coords;
            map[locationCoords.first][locationCoords.second] = locations[i].mapIcon;
        }
    }
};

void printMap(std::vector<std::vector<Region>> &map);
void drunkenWalk(std::vector<std::vector<Region>> &map, int &mapHeight, int &mapWidth);
void createRegion(std::vector<std::vector<Region>> &map, std::pair<int, int> &currentLocation, int &currentStep, int &totalSteps);

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
            newRegion.isEmpty = true;
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

    for (int i = 0; i < totalTiles; i++)
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
        createRegion(map, currentLocation, i, totalTiles);
    }
    // create capital
    Region capital("capital");
    map[startLocation.first][startLocation.second] = capital;

    // draw region
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
void progress(int &current, int &total)
{
    float currentPercentage = (static_cast<float>(current) / static_cast<float>(total)) * 100;
    std::cout << currentPercentage << std::endl;
}
// global vars
float offset = 0.0;
float nextCity = 20;

void createRegion(std::vector<std::vector<Region>> &map, std::pair<int, int> &currentLocation, int &currentStep, int &totalSteps)
{

    float takenStepsPercentage = (static_cast<float>(currentStep) / static_cast<float>(totalSteps)) * 100;
    if (map[currentLocation.first][currentLocation.second].isEmpty)
    {

        if (takenStepsPercentage >= nextCity + offset)
        {
            Region newRegion("city");
            map[currentLocation.first][currentLocation.second] = newRegion;
            nextCity = nextCity + 20;
            offset = 0.0;
        }
        else
        {

            Region newRegion("");
            map[currentLocation.first][currentLocation.second] = newRegion;
        }
    }
    else if (takenStepsPercentage >= nextCity + offset)
    {
        float singleStepPercentage = (1 / static_cast<float>(totalSteps)) * 100;
        offset = offset + singleStepPercentage;
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
