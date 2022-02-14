#include "HashTable.cpp"

int main()
{
    KeyedHashTable ht(11);
    ht.Insert("TestString", std::vector<int>());
    ht.Insert("Val", std::vector<int>());
    ht.Insert("VeryVeryLongTestString", std::vector<int>());

    ht.Print();
}