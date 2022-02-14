#include "Graph.h"
#include "GraphExceptions.h"

#include <iostream>
#include <iomanip>
#include <queue>
#include <fstream>
#include <sstream>
#include <cstdlib>
#include <unistd.h>
// Literally do nothing here
// default constructors of the std::vector is enough
Graph::Graph()
{
}

Graph::Graph(const std::string &filePath)
{
    // ============================= //
    // This function is implemented  //
    // Do not edit this function !   //
    // ============================= //
    // Tokens
    std::string tokens[3];

    std::ifstream mapFile(filePath.c_str());
    // Read line by line
    std::string line;
    while (std::getline(mapFile, line))
    {
        // Empty Line Skip
        if (line.empty())
            continue;
        // Comment Skip
        if (line[0] == '#')
            continue;

        // Tokenize the line
        int i = 0;
        std::istringstream stream(line);
        while (stream >> tokens[i])
            i++;

        // Single token (Meaning it is a vertex)
        if (i == 1)
        {
            InsertVertex(tokens[0]);
        }
        // Exactly three tokens (Meaning it is an edge)
        else if (i == 3)
        {
            int weight = std::atoi(tokens[0].c_str());
            if (!ConnectVertices(tokens[1], tokens[2], weight))
            {
                std::cerr << "Duplicate edge on "
                          << tokens[0] << "-"
                          << tokens[1] << std::endl;
            }
        }
        else
            std::cerr << "Token Size Mismatch" << std::endl;
    }
}

void Graph::InsertVertex(const std::string &vertexName)
{
    for (int i = 0, size = vertexList.size(); i < size; i++)
    {
        if (!vertexName.compare(vertexList[i].name))
        {
            throw DuplicateVertexNameException();
        }
    }
    GraphVertex newVertex;
    newVertex.name = vertexName;
    newVertex.edgeCount = 0;
    vertexList.push_back(newVertex);
}

bool Graph::ConnectVertices(const std::string &fromVertexName,
                            const std::string &toVertexName,
                            int weight)
{
    int pos1, pos2;
    pos1 = pos2 = -1;
    for (int i = 0, size = vertexList.size(); i < size; i++)
    {
        if (!fromVertexName.compare(vertexList[i].name))
        {
            pos1 = i;
        }
        if (!toVertexName.compare(vertexList[i].name))
        {
            pos2 = i;
        }
    }
    if (pos1 == -1 || pos2 == -1)
    {
        throw VertexNotFoundException();
    }
    else if (pos1 == pos2)
    {
        return false;
    }
    else if (vertexList[pos1].edgeCount + 1 > MAX_EDGE_PER_VERTEX || vertexList[pos2].edgeCount + 1 > MAX_EDGE_PER_VERTEX)
    {
        throw TooManyEdgeOnVertexExecption();
    }
    else
    {
        for (int i = 0; i < vertexList[pos1].edgeCount; i++)
        {
            if (edgeList[vertexList[pos1].edgeIds[i]].vertexId0 == pos2 || edgeList[vertexList[pos1].edgeIds[i]].vertexId1 == pos2)
            {
                return false;
            }
        }
    }
    vertexList[pos1].edgeIds[vertexList[pos1].edgeCount++] = edgeList.size();
    vertexList[pos2].edgeIds[vertexList[pos2].edgeCount++] = edgeList.size();
    GraphEdge newEdge;
    newEdge.weight = weight;
    newEdge.masked = false;
    newEdge.vertexId0 = pos1;
    newEdge.vertexId1 = pos2;
    edgeList.push_back(newEdge);
    return true;
}

bool Graph::ShortestPath(std::vector<int> &orderedVertexIdList,
                         const std::string &from,
                         const std::string &to) const
{

    int startPos, endPos;
    startPos = endPos = -1;
    for (int i = 0; i < vertexList.size(); i++)
    {
        if (vertexList[i].name == from)
        {
            startPos = i;
        }
        if (vertexList[i].name == to)
        {
            endPos = i;
        }
    }
    if (startPos == -1 || endPos == -1)
    {
        throw VertexNotFoundException();
    }
    std::vector<int> dist;
    std::vector<int> prev;
    std::priority_queue<DistanceVertexIdPair> pq;

    for (int i = 0; i < vertexList.size(); i++)
    {
        if (i != startPos)
        {
            dist.push_back(LARGE_NUMBER);
            prev.push_back(-1);
            pq.push(DistanceVertexIdPair(i, LARGE_NUMBER));
        }
        else
        {
            dist.push_back(0);
            prev.push_back(-1);
            pq.push(DistanceVertexIdPair(i, 0));
        }
    }

    while (!pq.empty())
    {

        int v = pq.top().vId;
        pq.pop();

        for (int i = 0; i < vertexList[v].edgeCount; i++)
        {
            int u;
            int weight;
            int edge = vertexList[v].edgeIds[i];
            if (edgeList[edge].masked)
                continue;
            if (edgeList[edge].vertexId0 == v)
            {
                u = edgeList[edge].vertexId1;
            }
            else
            {
                u = edgeList[edge].vertexId0;
            }
            weight = edgeList[edge].weight;

            if (dist[u] > dist[v] + weight)
            {
                dist[u] = dist[v] + weight;
                pq.push(DistanceVertexIdPair(u, dist[u]));
                prev[u] = v;
            }
        }
    }

    if (dist[endPos] == LARGE_NUMBER)
    {
        return false;
    }
    else
    {
        int i = endPos;
        while (i != -1)
        {
            orderedVertexIdList.insert(orderedVertexIdList.begin(), i);
            i = prev[i];
        }
        return true;
    }
}

int Graph::MultipleShortPaths(std::vector<std::vector<int>> &orderedVertexIdList,
                              const std::string &from,
                              const std::string &to,
                              int numberOfShortestPaths)
{
    int pathCount = 0;
    std::vector<int> lastPath;
    if (ShortestPath(lastPath, from, to))
    {
        pathCount++;
        orderedVertexIdList.push_back(lastPath);
    }
    for (; pathCount < numberOfShortestPaths; pathCount++)
    {
        GraphEdge edge;
        edge.weight = 0;
        for (int i = 0; i < lastPath.size() - 1; i++)
        {
            for (int j = 0; j < TotalEdgeCount(); j++)
            {
                if ((edgeList[j].vertexId0 == lastPath[i] && edgeList[j].vertexId1 == lastPath[i + 1]) || (edgeList[j].vertexId1 == lastPath[i] && edgeList[j].vertexId0 == lastPath[i + 1]))
                {
                    if (edge.weight < edgeList[j].weight)
                    {
                        edge = edgeList[j];
                    }
                }
            }
        }
        StringPair MaskPair(vertexList[edge.vertexId0].name, vertexList[edge.vertexId1].name);
        std::vector<StringPair> mask;
        mask.push_back(MaskPair);
        MaskEdges(mask);
        lastPath.clear();
        if (ShortestPath(lastPath, from, to))
        {
            orderedVertexIdList.push_back(lastPath);
        }
        else
            break;
    }
    UnMaskAllEdges();
    return pathCount;
}

void Graph::MaskEdges(const std::vector<StringPair> &vertexNames)
{
    std::string s0, s1;
    int pos0, pos1;
    pos0 = pos1 = -1;
    for (int i = 0; i < vertexNames.size(); i++)
    {
        s0 = vertexNames[i].s0;
        s1 = vertexNames[i].s1;
        for (int j = 0; j < vertexList.size(); j++)
        {
            if (vertexList[j].name == s0)
            {
                pos0 = j;
            }
            if (vertexList[j].name == s1)
            {
                pos1 = j;
            }
        }
        if (pos0 == -1 || pos1 == -1)
        {
            throw VertexNotFoundException();
        }
        for (int k = 0; k < edgeList.size(); k++)
        {
            if ((edgeList[k].vertexId0 == pos0 && edgeList[k].vertexId1 == pos1) || (edgeList[k].vertexId0 == pos1 && edgeList[k].vertexId1 == pos0))
            {
                edgeList[k].masked = true;
            }
        }
    }
}

void Graph::UnMaskEdges(const std::vector<StringPair> &vertexNames)
{
    std::string s0, s1;
    int pos0, pos1;
    pos0 = pos1 = -1;
    for (int i = 0; i < vertexNames.size(); i++)
    {
        s0 = vertexNames[i].s0;
        s1 = vertexNames[i].s1;
        for (int j = 0; j < vertexList.size(); j++)
        {
            if (vertexList[j].name == s0)
            {
                pos0 = j;
            }
            if (vertexList[j].name == s1)
            {
                pos1 = j;
            }
        }
        if (pos0 == -1 || pos1 == -1)
        {
            throw VertexNotFoundException();
        }
        for (int k = 0; k < edgeList.size(); k++)
        {
            if ((edgeList[k].vertexId0 == pos0 && edgeList[k].vertexId1 == pos1) || (edgeList[k].vertexId0 == pos1 && edgeList[k].vertexId1 == pos0))
            {
                edgeList[k].masked = false;
            }
        }
    }
}

void Graph::UnMaskAllEdges()
{
    for (int i = 0; i < edgeList.size(); i++)
    {
        edgeList[i].masked = false;
    }
}

void Graph::MaskVertexEdges(const std::string &name)
{
    for (int i = 0; i < vertexList.size(); i++)
    {
        if (vertexList[i].name == name)
        {
            for (int j = 0; j < vertexList[i].edgeCount; j++)
            {
                edgeList[vertexList[i].edgeIds[j]].masked = true;
            }
            return;
        }
    }
    throw VertexNotFoundException();
}

void Graph::UnMaskVertexEdges(const std::string &name)
{
    for (int i = 0; i < vertexList.size(); i++)
    {
        if (vertexList[i].name == name)
        {
            for (int j = 0; j < vertexList[i].edgeCount; j++)
            {
                edgeList[vertexList[i].edgeIds[j]].masked = false;
            }
            return;
        }
    }
    throw VertexNotFoundException();
}

void Graph::ModifyEdge(const std::string &vName0,
                       const std::string &vName1,
                       float newWeight)
{
    int vId0, vId1;
    vId0 = vId1 = -1;
    for (int j = 0; j < vertexList.size(); j++)
    {
        if (vertexList[j].name == vName0)
        {
            vId0 = j;
        }
        if (vertexList[j].name == vName1)
        {
            vId1 = j;
        }
    }
    if (vId0 == -1 || vId1 == -1)
    {
        throw VertexNotFoundException();
    }

    for (int i = 0; i < edgeList.size(); i++)
    {
        if ((edgeList[i].vertexId0 == vId0 && edgeList[i].vertexId1 == vId1) || (edgeList[i].vertexId0 == vId1 && edgeList[i].vertexId1 == vId0))
        {
            edgeList[i].weight = newWeight;
        }
    }
}

void Graph::ModifyEdge(int vId0, int vId1,
                       float newWeight)
{
    for (int i = 0; i < edgeList.size(); i++)
    {
        if ((edgeList[i].vertexId0 == vId0 && edgeList[i].vertexId1 == vId1) || (edgeList[i].vertexId0 == vId1 && edgeList[i].vertexId1 == vId0))
        {
            edgeList[i].weight = newWeight;
        }
    }
}

void Graph::PrintAll() const
{
    // ============================= //
    // This function is implemented  //
    // Do not edit this function !   //
    // ============================= //
    for (size_t i = 0; i < vertexList.size(); i++)
    {
        const GraphVertex &v = vertexList[i];
        std::cout << v.name << "\n";
        for (int j = 0; j < v.edgeCount; j++)
        {
            int edgeId = v.edgeIds[j];
            const GraphEdge &edge = edgeList[edgeId];
            // Skip printing this edge if it is masked
            if (edge.masked)
                continue;

            // List the all vertex names and weight
            std::cout << "-" << std::setfill('-')
                      << std::setw(2) << edge.weight
                      << "-> ";
            int neigVertexId = (static_cast<int>(i) == edge.vertexId0)
                                   ? edge.vertexId1
                                   : edge.vertexId0;
            std::cout << vertexList[neigVertexId].name << "\n";
        }
    }
    // Reset fill value because it "sticks" to the std out
    std::cout << std::setfill(' ');
    std::cout.flush();
}

void Graph::PrintPath(const std::vector<int> &orderedVertexIdList,
                      bool sameLine) const
{
    // ============================= //
    // This function is implemented  //
    // Do not edit this file !       //
    // ============================= //
    for (size_t i = 0; i < orderedVertexIdList.size(); i++)
    {
        int vertexId = orderedVertexIdList[i];
        if (vertexId >= static_cast<int>(vertexList.size()))
            throw VertexNotFoundException();

        const GraphVertex &vertex = vertexList[vertexId];
        std::cout << vertex.name;
        if (!sameLine)
            std::cout << "\n";
        // Only find and print the weight if next is available
        if (i == orderedVertexIdList.size() - 1)
            break;
        int nextVertexId = orderedVertexIdList[i + 1];
        if (nextVertexId >= static_cast<int>(vertexList.size()))
            throw VertexNotFoundException();

        // Find the edge between these two vertices
        int edgeId = INVALID_INDEX;
        if (vertexId < static_cast<int>(vertexList.size()) &&
            nextVertexId < static_cast<int>(vertexList.size()))
        {
            // Check all of the edges of vertex
            // and try to find
            const GraphVertex &fromVert = vertexList[vertexId];
            for (int i = 0; i < fromVert.edgeCount; i++)
            {
                int eId = fromVert.edgeIds[i];
                // Since the graph is not directional
                // check the both ends
                if ((edgeList[eId].vertexId0 == vertexId &&
                     edgeList[eId].vertexId1 == nextVertexId) ||
                    (edgeList[eId].vertexId0 == nextVertexId &&
                     edgeList[eId].vertexId1 == vertexId))
                {
                    edgeId = eId;
                    break;
                }
            }
        }
        if (edgeId != INVALID_INDEX)
        {
            const GraphEdge &edge = edgeList[edgeId];
            std::cout << "-" << std::setfill('-')
                      << std::setw(2)
                      << edge.weight << "->";
        }
        else
        {
            std::cout << "-##-> ";
        }
    }
    // Print endline on the last vertex if same line is set
    if (sameLine)
        std::cout << "\n";
    // Reset fill value because it "sticks" to the std out
    std::cout << std::setfill(' ');
    std::cout.flush();
}

int Graph::TotalVertexCount() const
{
    return vertexList.size();
}

int Graph::TotalEdgeCount() const
{
    return edgeList.size();
}

std::string Graph::VertexName(int vertexId) const
{
    std::string name = "";
    if (vertexId < vertexList.size())
    {
        name = vertexList[vertexId].name;
    }
    return name;
}

int Graph::TotalWeightInBetween(std::vector<int> &orderedVertexIdList)
{
    int total = -1;
    for (int i = 0; i < orderedVertexIdList.size() - 1; i++)
    {
        if (orderedVertexIdList[i] > vertexList.size() || orderedVertexIdList[i + 1] > vertexList.size())
        {
            throw VertexNotFoundException();
        }
        for (int j = 0; j < vertexList[orderedVertexIdList[i]].edgeCount; j++)
        {
            if (edgeList[vertexList[orderedVertexIdList[i]].edgeIds[j]].vertexId0 == orderedVertexIdList[i + 1] || edgeList[vertexList[orderedVertexIdList[i]].edgeIds[j]].vertexId1 == orderedVertexIdList[i + 1])
            {
                total += edgeList[vertexList[orderedVertexIdList[i]].edgeIds[j]].weight;
            }
        }
    }
    if (total > -1)
    {
        total++;
    }
    return total;
}