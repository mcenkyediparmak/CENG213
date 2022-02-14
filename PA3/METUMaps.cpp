#include "METUMaps.h"
#include "GraphExceptions.h"

#include <iostream>

void METUMaps::PrintNotInJourney() const
{
    std::cout << "Device is not in a journey!" << std::endl;
}

void METUMaps::PrintUnableToChangeDestination() const
{
    std::cout << "Cannot change Destination during journey!" << std::endl;
}

void METUMaps::PrintUnableToChangeStartingLoc() const
{
    std::cout << "Cannot change Starting Location during journey!" << std::endl;
}

void METUMaps::PrintAlreadyInJourney() const
{
    std::cout << "Device is already in a journey!" << std::endl;
}

void METUMaps::PrintJourneyIsAlreadFinished() const
{
    std::cout << "Journey is already finished!" << std::endl;
}

void METUMaps::PrintLocationNotFound() const
{
    std::cout << "One (or both) of the locations are not found in the maps!" << std::endl;
}

void METUMaps::PrintJourneyCompleted() const
{
    std::cout << "Journey Completed!" << std::endl;
}

void METUMaps::PrintCachedLocationFound(const std::string &location0,
                                        const std::string &location1) const
{
    std::cout << "Route between \""
              << location0 << "\" and \""
              << location1 << "\" is in cache, using that..."
              << std::endl;
}

void METUMaps::PrintCalculatingRoutes(const std::string &location0,
                                      const std::string &location1) const
{
    std::cout << "Calculating Route(s) between \""
              << location0 << "\" and \""
              << location1 << "\"..."
              << std::endl;
}

std::string METUMaps::GenerateKey(const std::string &location0,
                                  const std::string &location1)
{
    // ============================= //
    // This function is implemented  //
    // Do not edit this function !   //
    // ============================= //
    return location0 + "/" + location1;
}

METUMaps::METUMaps(int potentialPathCount,
                   const std::string &mapFilePath)
{
    this->potentialPathCount = potentialPathCount;
    map = Graph(mapFilePath);
    cachedPaths = KeyedHashTable(map.TotalVertexCount() * potentialPathCount);
    inJourney = false;
}

void METUMaps::SetDestination(const std::string &name)
{
    if (inJourney)
    {
        PrintUnableToChangeDestination();
    }
    else
    {
        destination = name;
    }
}

void METUMaps::SetStartingLocation(const std::string &name)
{
    if (inJourney)
    {
        PrintUnableToChangeDestination();
    }
    else
    {
        startingLoc = name;
    }
}

void METUMaps::StartJourney()
{
    if (inJourney)
    {
        PrintAlreadyInJourney();
    }
    else
    {

        PrintCalculatingRoutes(startingLoc, destination);
        try
        {
            inJourney = true;
            std::vector<std::vector<int>> routes;
            int pathsCount = map.MultipleShortPaths(routes, startingLoc, destination, potentialPathCount);
            for (int i = 0; i < pathsCount; i++)
            {
                for (int j = 0; j < routes[i].size(); j++)
                {
                    std::vector<int> subPath(&routes[i][j], &routes[i][routes[i].size()]);
                    cachedPaths.Insert(GenerateKey(map.VertexName(routes[i][j]), destination), subPath);
                }
            }
            currentLoc = startingLoc;
            currentRoute = routes[0];
        }
        catch (std::exception &e)
        {
            PrintLocationNotFound();
        }
    }
}

void METUMaps::EndJourney()
{
    if (!inJourney)
    {
        inJourney = false;
        cachedPaths.ClearTable();
        destination = startingLoc = currentLoc = "";
    }
    else
    {
        PrintJourneyIsAlreadFinished();
    }
}

void METUMaps::UpdateLocation(const std::string &name)
{
    if (!inJourney)
    {
        PrintNotInJourney();
    }
    else
    {
        std::vector<int> path;
        if (name == destination)
        {
            PrintJourneyCompleted();
            return;
        }
        currentLoc = name;
        if (cachedPaths.Find(path, GenerateKey(name, destination)))
        {
            PrintCachedLocationFound(name, destination);
            currentRoute = path;
        }
        else
        {
            PrintCalculatingRoutes(name, destination);
            try
            {
                std::vector<std::vector<int>> routes;
                int pathsCount = map.MultipleShortPaths(routes, name, destination, potentialPathCount);
                for (int i = 0; i < pathsCount; i++)
                {
                    for (int j = 0; j < routes[i].size(); j++)
                    {
                        std::vector<int> subPath(&routes[i][j], &routes[i][routes[i].size()]);
                        cachedPaths.Insert(GenerateKey(map.VertexName(routes[i][j]), destination), subPath);
                    }
                }

                currentRoute = routes[0];
            }
            catch (std::exception &e)
            {
                PrintLocationNotFound();
            }
        }
    }
}

void METUMaps::Display()
{
    // ============================= //
    // This function is implemented  //
    // Do not edit this function !   //
    // ============================= //
    if (!inJourney)
    {
        // We are not in journey, this function
        // shouldn't be called
        PrintNotInJourney();
        return;
    }

    int timeLeft = map.TotalWeightInBetween(currentRoute);

    // Print last known location
    std::cout << "Journey         : " << startingLoc << "->"
              << destination << "\n";
    std::cout << "Current Location: " << currentLoc << "\n";
    std::cout << "Time Left       : " << timeLeft << " minutes\n";
    std::cout << "Current Route   : ";
    map.PrintPath(currentRoute, true);

    std::cout.flush();
}