#include "Graph.h"
#include "GraphExceptions.h"
#include <iostream>
#include "Graph.cpp"
int main()
{
    Graph g;
    std::vector<std::vector<int>> list;
    g.InsertVertex("A");
    g.InsertVertex("B");
    g.InsertVertex("C");
    g.InsertVertex("D");
    g.InsertVertex("E");
    g.InsertVertex("F");
    g.InsertVertex("G");
    g.InsertVertex("H");
    g.InsertVertex("I");
    g.InsertVertex("J");

    g.ConnectVertices("A", "B", 1);
    g.ConnectVertices("B", "D", 1);
    g.ConnectVertices("A", "C", 2);
    g.ConnectVertices("A", "D", 3);
    g.ConnectVertices("A", "E", 4);
    g.ConnectVertices("A", "F", 5);
    g.ConnectVertices("A", "G", 6);
    g.ConnectVertices("A", "H", 7);
    g.ConnectVertices("A", "I", 8);
    std::cout << g.MultipleShortPaths(list, "A", "I", 2) << "pathsfounded" << std::endl;
    for (int i = 0; i < list[0].size(); i++)
    {
        std::cout << list[0][i] << " ";
    }
    std::cout << std::endl;
    for (int i = 0; i < list[1].size(); i++)
    {
        std::cout << list[1][i] << " ";
    }

    try
    {
        g.ConnectVertices("B", "J", 9);
    }
    catch (const TooManyEdgeOnVertexExecption &e)
    {
        std::cout << e.what() << '\n';
    }
    return 0;
}