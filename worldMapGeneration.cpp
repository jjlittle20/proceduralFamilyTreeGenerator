#include <time.h>
#include <iostream>
#include <vector>
#include <string>
#include "placeGenerator.cpp"
#include "humanGeneration.cpp"
#include <cstring>
class Progress
{
public:
    int current;
    int total;

    Progress(int currentAmount, int totalAmount)
    {
        current = currentAmount;
        total = totalAmount;
    };
    void printProgress()
    {
        float currentPercentage = (static_cast<float>(current) / static_cast<float>(total)) * 100;
        std::cout << currentPercentage << std::endl;
    };
};
struct LocationAmount
{

    int maxCapitals = 1;
    int maxCities = 4;
    int maxLargeTowns = 25;

    int maxSmallTowns = 50;
    int maxVillages = 95;
    int maxSmallCommunities = 135;

    int currentCapitals = 0;
    int currentCities = 0;
    int currentLargeTowns = 0;

    int currentSmallTowns = 0;
    int currentVillages = 0;
    int currentSmallCommunities = 0;

    void printStats()
    {
        std::cout << "Capitals: " << currentCapitals << "/" << maxCapitals << std::endl;
        std::cout << "Cities: " << currentCities << "/" << maxCities << std::endl;
        std::cout << "Large Towns: " << currentLargeTowns << "/" << maxLargeTowns << std::endl;
        std::cout << "Small Towns: " << currentSmallTowns << "/" << maxSmallTowns << std::endl;
        std::cout << "Villages: " << currentVillages << "/" << maxVillages << std::endl;
        std::cout << "Small Communities: " << currentSmallCommunities << "/" << maxSmallCommunities << std::endl;
    };
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
LocationLookupTable locationLookupTable[] = {{"capital", 3, 3, 2, 2, locationTypes[0]}, {"city", 3, 3, 2, 2, locationTypes[1]}, {"largeTown", 0, 2, 3, 2, locationTypes[2]}, {"smallTown", 0, 0, 3, 3, locationTypes[3]}, {"village", 0, 0, 0, 5, locationTypes[4]}, {"small", 0, 0, 0, 0, locationTypes[5]}};

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
    std::string name;
    int seedPairs = 0;
    std::vector<Human> population;
};
class Region
{
private:
    /* data */
public:
    std::string name = "";
    int maxHeight = 10;
    int maxWidth = 10;
    Biome biome;
    Biome subBiome;
    std::vector<Location> locations;
    std::vector<std::vector<char>> map;
    char mapIcon = ' ';
    bool isEmpty = false;
    bool isLand = true;

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
        else if (locationType == "largeTown")
        {
            mapIcon = 'T';
            biome.type = "settlement";
            generateRegionLocations(locationType);
            locationAmount.currentLargeTowns = locationAmount.currentLargeTowns + 1;
        }
        else if (locationType == "smallTown")
        {
            mapIcon = 't';
            biome.type = "settlement";
            generateRegionLocations(locationType);
            locationAmount.currentSmallTowns = locationAmount.currentSmallTowns + 1;
        }
        else if (locationType == "village")
        {
            mapIcon = 'v';
            biome.type = "settlement";
            generateRegionLocations(locationType);
            locationAmount.currentVillages = locationAmount.currentVillages + 1;
        }
        else if (locationType == "small")
        {
            mapIcon = 's';
            biome.type = "settlement";
            generateRegionLocations(locationType);
            locationAmount.currentSmallCommunities = locationAmount.currentSmallCommunities + 1;
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

            map.push_back(row);
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
        locations.push_back({regionMapCenter, centerLocation.metaData.icon[0], generateLocationName(), centerLocation.metaData.seedPopulation, generateSeeds(centerLocation.metaData.seedPopulation)});

        map[regionMapCenter.first][regionMapCenter.second] = centerLocation.metaData.icon[0];
        std::vector<std::pair<int, int>> usedCoords;
        usedCoords.push_back(regionMapCenter);

        for (size_t i = 0; i < centerLocation.largeTowns; i++)
        {
            int seedPairs = locationLookupTable[2].metaData.seedPopulation;
            int x = rand() % maxWidth;
            int y = rand() % maxHeight;
            std::pair<int, int> validCoords = checkCoords(usedCoords, x, y, maxHeight, maxWidth);
            locations.push_back({{x, y}, 'T', generateLocationName(), seedPairs, generateSeeds(seedPairs)});
            locationAmount.currentLargeTowns = locationAmount.currentLargeTowns + 1;
        }

        for (size_t i = 0; i < centerLocation.smallTowns; i++)
        {
            if (locationAmount.currentSmallTowns >= locationAmount.maxSmallTowns)
            {
                break;
            }
            int seedPairs = locationLookupTable[3].metaData.seedPopulation;
            int x = rand() % maxWidth;
            int y = rand() % maxHeight;
            std::pair<int, int> validCoords = checkCoords(usedCoords, x, y, maxHeight, maxWidth);
            locations.push_back({{x, y}, 't', generateLocationName(), seedPairs, generateSeeds(seedPairs)});
            locationAmount.currentSmallTowns = locationAmount.currentSmallTowns + 1;
        }

        for (size_t i = 0; i < centerLocation.villages; i++)
        {
            if (locationAmount.currentVillages >= locationAmount.maxVillages)
            {
                break;
            }
            int seedPairs = locationLookupTable[4].metaData.seedPopulation;
            int x = rand() % maxWidth;
            int y = rand() % maxHeight;
            std::pair<int, int> validCoords = checkCoords(usedCoords, x, y, maxHeight, maxWidth);
            locations.push_back({{x, y}, 'v', generateLocationName(), seedPairs, generateSeeds(seedPairs)});
            locationAmount.currentVillages = locationAmount.currentVillages + 1;
        }

        for (size_t i = 0; i < centerLocation.smallCommunities; i++)
        {
            if (locationAmount.currentSmallCommunities >= locationAmount.maxSmallCommunities)
            {
                break;
            }
            int seedPairs = locationLookupTable[5].metaData.seedPopulation;
            int x = rand() % maxWidth;
            int y = rand() % maxHeight;
            std::pair<int, int> validCoords = checkCoords(usedCoords, x, y, maxHeight, maxWidth);
            locations.push_back({{x, y}, 's', generateLocationName(), seedPairs, generateSeeds(seedPairs)});
            locationAmount.currentSmallCommunities = locationAmount.currentSmallCommunities + 1;
        }
    }
    std::string generateLocationName()
    {
        std::string placeName =
            generateSinglePlaceName();
        return placeName;
    };
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
void drunkenWalk(std::vector<std::vector<Region>> &map, int &mapHeight, int &mapWidth);
void createRegion(std::vector<std::vector<Region>> &map, std::pair<int, int> &currentLocation, int currentStep = 0, int totalSteps = 0, std::string regionType = "");
class WorldMap
{
public:
    int mapHeight = 40;
    int mapWidth = 30;
    // vector 2d of map
    std::vector<std::vector<Region>> map;
    //  vector of empty land
    std::vector<std::pair<int, int>> emptyLandRegion;
    // coords of overword settlement regions.
    std::vector<std::pair<int, int>> capital;
    std::vector<std::pair<int, int>> cities;
    std::vector<std::pair<int, int>> largeTowns;
    std::vector<std::pair<int, int>> smallTowns;
    std::vector<std::pair<int, int>> villages;
    std::vector<std::pair<int, int>> smallCommunities;

    WorldMap()
    {
        for (size_t i = 0; i < mapHeight; i++)
        {
            std::vector<Region> row;
            for (size_t j = 0; j < mapWidth; j++)
            {
                Region newRegion("default");
                newRegion.isEmpty = true;
                newRegion.isLand = false;
                row.push_back(newRegion);
            }

            {

                map.push_back(row);
            }
        }
    };

    void printMap()
    {
        std::cout << "  ";
        std::vector<char> secondRow;
        for (size_t k = 0; k < map[0].size(); k++)
        {

            std::string columnIntToString = std::to_string(k);

            if (columnIntToString[1] == '\0')
            {
                secondRow.push_back(' ');
            }
            else
            {
                secondRow.push_back(columnIntToString[1]);
            }
            std::cout << columnIntToString[0];
            std::cout << " ";
        }
        std::cout << std::endl;
        std::cout << "  ";
        for (size_t l = 0; l < secondRow.size(); l++)
        {
            std::cout << secondRow[l];
            std::cout << " ";
        }

        std::cout << std::endl;
        for (int i = 0; i < map.size(); i++)
        {

            std::string rowIntToString = std::to_string(i);

            if (rowIntToString[1] == '\0')
            {
                std::cout << " ";
            }
            std::cout << i;
            for (int j = 0; j < map[i].size(); j++)
            {
                std::cout << map[i][j].mapIcon << " ";
            }
            std::cout << std::endl;
        }
    }
    void generateOtherSettlements()
    {

        int largeTownsAmount = locationAmount.maxLargeTowns - locationAmount.currentLargeTowns;

        for (size_t i = 0; i < largeTownsAmount; i++)
        {
            int citySelector = rand() % cities.size();
            std::pair<int, int> centerCity = cities[citySelector];
            std::pair<int, int> validCoords = getEmptyRegion(centerCity);

            createRegion(map, validCoords, 0, 0, "largeTown");
        }
        int smallTownsAmount = locationAmount.maxSmallTowns - locationAmount.currentSmallTowns;
        for (size_t i = 0; i < smallTownsAmount; i++)
        {
            int largeTownSelector = rand() % largeTowns.size();
            std::pair<int, int> centerLargeTown = largeTowns[largeTownSelector];
            std::pair<int, int> validCoords = getEmptyRegion(centerLargeTown);

            createRegion(map, validCoords, 0, 0, "smallTown");
        }
        int villagesAmount = locationAmount.maxVillages - locationAmount.currentVillages;
        for (size_t i = 0; i < villagesAmount; i++)
        {
            int smallTownSelector = rand() % smallTowns.size();
            std::pair<int, int> centerSmallTown = smallTowns[smallTownSelector];
            std::pair<int, int> validCoords = getEmptyRegion(centerSmallTown);

            createRegion(map, validCoords, 0, 0, "village");
        }
        int smallCommunitiesAmount = locationAmount.maxSmallCommunities - locationAmount.currentSmallCommunities;
        for (size_t i = 0; i < smallCommunitiesAmount; i++)
        {
            int villageSelector = rand() % villages.size();
            std::pair<int, int> centerVillage = villages[villageSelector];
            std::pair<int, int> validCoords = getEmptyRegion(centerVillage);

            createRegion(map, validCoords, 0, 0, "small");
        }
    }
    std::pair<int, int> getEmptyRegion(std::pair<int, int> centerPoint)
    {

        int minDistance = 1;
        int maxDistance = 6;

        std::vector<std::pair<int, int>> emptyLandRegionCopy = emptyLandRegion;

        for (size_t i = 0; i < emptyLandRegionCopy.size(); i++)
        {
            bool isValidPositiveYCoords = emptyLandRegion[i].first >= centerPoint.first + minDistance && emptyLandRegion[i].first <= centerPoint.first + maxDistance;
            bool isValidPositiveXCoords = emptyLandRegion[i].second >= centerPoint.second + minDistance && emptyLandRegion[i].second <= centerPoint.second + maxDistance;
            bool isValidNegativeYCoords = emptyLandRegion[i].first <= centerPoint.first - minDistance && emptyLandRegion[i].first >= centerPoint.first - maxDistance;
            bool isValidNegativeXCoords = emptyLandRegion[i].second <= centerPoint.second - minDistance && emptyLandRegion[i].second >= centerPoint.second - maxDistance;
            if (isValidPositiveXCoords && isValidPositiveYCoords || isValidNegativeXCoords && isValidNegativeYCoords)
            {
                std::pair<int, int> validCoords = emptyLandRegion[i];

                emptyLandRegion.erase(emptyLandRegion.begin() + i);
                return validCoords;
            }
        }
        return {-1, -1};
    };
    void printLocationNames()
    {
        int regionAmount = map.size();
        for (size_t i = 0; i < regionAmount; i++)
        {

            for (int j = 0; j < map[i].size(); j++)
            {
                if (map[i][j].locations.size() > 0)
                {
                    std::vector<Location> locs = map[i][j].locations;
                    std::cout << "---------" << std::endl;
                    for (size_t k = 0; k < locs.size(); k++)
                    {
                        std::cout << locs[k].name << std::endl;
                    }
                }
            }
        }
    };
    void printRegion(int x, int y)
    {

        // check if within map boundries.
        Region regionToPrint = map[y][x];
        for (int i = 0; i < regionToPrint.map.size(); i++)
        {
            for (int j = 0; j < regionToPrint.map[i].size(); j++)
            {
                std::cout << regionToPrint.map[i][j] << " ";
            }
            std::cout << std::endl;
        }
        std::vector<Location> locs = map[y][x].locations;
        std::cout << "-----------------------" << std::endl;
        for (size_t k = 0; k < locs.size(); k++)
        {
            std::cout << k << ": ";
            std::cout << locs[k].name;
            std::cout << " " << locs[k].population.size() << std::endl;
        }

        std::cout << "select a location to view population : "
                  << "0-" << locs.size() - 1 << " : ";

        int locationToView;

        cin >> locationToView;
        std::vector<Human> populationToPrint = locs[locationToView].population;
        printLocationPopulation(populationToPrint);
    };
    void printLocationPopulation(std::vector<Human> &pop)
    {
        for (size_t i = 0; i < pop.size(); i++)
        {

            std::cout << pop[i].name.first << " " << pop[i].name.second << std::endl;
        }
    };
};

WorldMap worldMap;

struct Direction
{
    char direction;
    std::pair<int, int> coords;
};
const Direction directions[] = {{'u', {-1, 0}}, {'d', {1, 0}}, {'l', {0, -1}}, {'r', {0, 1}}};
int main(int argc, char const *argv[])
{

    drunkenWalk(worldMap.map, worldMap.mapHeight, worldMap.mapWidth);
    worldMap.generateOtherSettlements();
    worldMap.printMap();
    // worldMap.printLocationNames();
    locationAmount.printStats();
    std::cout << "Finished Generation" << std::endl;
    std::cout << std::endl;
    std::cout << "To explore the map enter the coords you would like to view..." << std::endl;
    std::cout << "X Axis: ";
    int x;
    int y;
    std::cin >> x;
    std::cout << "Y Axis: ";
    std::cin >> y;
    worldMap.printRegion(x, y);
    return 0;
}

void drunkenWalk(std::vector<std::vector<Region>> &map, int &mapHeight, int &mapWidth)
{
    int totalTiles = 2000;
    std::pair<int, int> startLocation{mapHeight / 2, mapWidth / 2};
    std::pair<int, int> currentLocation{mapHeight / 2, mapWidth / 2};

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
    worldMap.capital.push_back(startLocation);
}

// global vars
float offset = 0.0;
float nextCity = 20;

void createRegion(std::vector<std::vector<Region>> &map, std::pair<int, int> &currentLocation, int currentStep, int totalSteps, std::string regionType)
{

    float takenStepsPercentage = (static_cast<float>(currentStep) / static_cast<float>(totalSteps)) * 100;
    if (regionType == "largeTown")
    {

        Region newRegion(regionType);

        map[currentLocation.first][currentLocation.second] = newRegion;
        worldMap.largeTowns.push_back(currentLocation);
    }
    else if (regionType == "smallTown")
    {

        Region newRegion(regionType);

        map[currentLocation.first][currentLocation.second] = newRegion;
        worldMap.smallTowns.push_back(currentLocation);
    }
    else if (regionType == "village")
    {

        Region newRegion(regionType);

        map[currentLocation.first][currentLocation.second] = newRegion;
        worldMap.villages.push_back(currentLocation);
    }
    else if (regionType == "small")
    {

        Region newRegion(regionType);

        map[currentLocation.first][currentLocation.second] = newRegion;
        worldMap.smallCommunities.push_back(currentLocation);
    }
    else if (map[currentLocation.first][currentLocation.second].isEmpty)
    {

        if (takenStepsPercentage >= nextCity + offset)
        {
            Region newRegion("city");
            map[currentLocation.first][currentLocation.second] = newRegion;
            worldMap.cities.push_back(currentLocation);
            nextCity = nextCity + 20;
            offset = 0.0;
        }
        else
        {

            Region newRegion(regionType);

            map[currentLocation.first][currentLocation.second] = newRegion;
            if (regionType == "")
            {
                worldMap.emptyLandRegion.push_back({currentLocation.first, currentLocation.second});
            }
        }
    }
    else if (takenStepsPercentage >= nextCity + offset)
    {
        float singleStepPercentage = (1 / static_cast<float>(totalSteps)) * 100;
        offset = offset + singleStepPercentage;
    }
}
