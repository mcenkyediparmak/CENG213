#include "HashTable.h"

#include <cassert>
#include <iostream>
#include <iomanip>

const int KeyedHashTable::PRIME_LIST[PRIME_TABLE_COUNT] =
    {
        2, 3, 5, 7, 11, 13, 17, 19,
        23, 29, 31, 37, 41, 43, 47, 53,
        59, 61, 67, 71, 73, 79, 83, 89,
        97, 101, 103, 107, 109, 113, 127, 131,
        137, 139, 149, 151, 157, 163, 167, 173,
        179, 181, 191, 193, 197, 199, 211, 223,
        227, 229, 233, 239, 241, 251, 257, 263,
        269, 271, 277, 281, 283, 293, 307, 311,
        313, 317, 331, 337, 347, 349, 353, 359,
        367, 373, 379, 383, 389, 397, 401, 409,
        419, 421, 431, 433, 439, 443, 449, 457,
        461, 463, 467, 479, 487, 491, 499, 503,
        509, 521, 523, 541};

int KeyedHashTable::Hash(const std::string &key) const
{
    int hash = 0;
    for (int i = 0; key[i] != '\0'; i++)
    {
        hash += key[i] * PRIME_LIST[i];
    }
    hash %= tableSize;
    return hash;
}

void KeyedHashTable::ReHash()
{
    int oldsize = tableSize;
    int hashCode;
    int newHashCode;
    tableSize = FindNearestLargerPrime(2 * tableSize);
    HashData *newTable = new HashData[tableSize];

    for (int i = 0; i < oldsize; i++)
    {
        if (table[i].key != "" && table[i].key != DELETED)
        {
            hashCode = Hash(table[i].key);
            newHashCode = hashCode;
            for (int j = 0; newTable[hashCode].key != ""; j++)
            {
                newHashCode = (hashCode + j * j) % tableSize;
            }

            newTable[newHashCode].key = table[i].key;
            newTable[newHashCode].intArray = table[i].intArray;
        }
    }
    delete[] table;
    table = newTable;
}

int KeyedHashTable::FindNearestLargerPrime(int requestedCapacity)
{
    for (int i = 0; i < PRIME_TABLE_COUNT; i++)
    {
        if (requestedCapacity < PRIME_LIST[i])
        {
            return PRIME_LIST[i];
        }
    }
}

KeyedHashTable::KeyedHashTable()
{
    table = new HashData[PRIME_LIST[0]];
    tableSize = PRIME_LIST[0];
    occupiedElementCount = 0;
}

KeyedHashTable::KeyedHashTable(int requestedCapacity)
{
    int size = FindNearestLargerPrime(requestedCapacity);
    table = new HashData[size];
    tableSize = size;
    occupiedElementCount = 0;
}

KeyedHashTable::KeyedHashTable(const KeyedHashTable &other)
{
    table = new HashData[other.tableSize];
    tableSize = other.tableSize;
    occupiedElementCount = other.occupiedElementCount;
    for (int i = 0; i < tableSize; i++)
    {
        table[i] = other.table[i];
    }
}

KeyedHashTable &KeyedHashTable::operator=(const KeyedHashTable &other)
{
    delete[] table;
    table = new HashData[other.tableSize];
    tableSize = other.tableSize;
    occupiedElementCount = other.occupiedElementCount;
    for (int i = 0; i < tableSize; i++)
    {
        table[i] = other.table[i];
    }
    return *this;
}

KeyedHashTable::~KeyedHashTable()
{
    delete[] table;
    tableSize = 0;
    occupiedElementCount = 0;
}

bool KeyedHashTable::Insert(const std::string &key,
                            const std::vector<int> &intArray)
{
    int hashCode = Hash(key);

    int newHashCode = hashCode;
    for (int i = 0; table[newHashCode].key != ""; i++)
    {
        newHashCode = (hashCode + i * i) % tableSize;
        if (table[newHashCode].key == key)
        {
            return false;
        }
    }

    table[newHashCode].key = key;
    table[newHashCode].intArray = intArray;
    occupiedElementCount++;
    if (occupiedElementCount * EXPAND_THRESHOLD >= tableSize)
    {

        ReHash();
    }

    return true;
}

bool KeyedHashTable::Remove(const std::string &key)
{
    int hashCode = Hash(key);
    for (int i = 0; table[hashCode].key != ""; i++)
    {
        hashCode = (hashCode + i * i) % tableSize;
        if (table[hashCode].key == key)
        {
            table[hashCode].intArray.clear();
            table[hashCode].key = DELETED;
            occupiedElementCount--;
            return true;
        }
    }
    return false;
}

void KeyedHashTable::ClearTable()
{
    for (int i = 0; i < tableSize; i++)
    {
        table[i].key = "";
        table[i].intArray.clear();
    }
    occupiedElementCount = 0;
}

bool KeyedHashTable::Find(std::vector<int> &valueOut,
                          const std::string &key) const
{
    int hashCode = Hash(key);
    for (int i = 0; table[hashCode].key != ""; i++)
    {
        hashCode = (hashCode + i * i) % tableSize;
        if (table[hashCode].key == key)
        {
            valueOut = table[hashCode].intArray;
            return true;
        }
    }
    return false;
}

void KeyedHashTable::Print() const
{
    // ============================= //
    // This function is implemented  //
    // Do not edit this function !   //
    // ============================= //
    std::cout << "HT:";
    if (occupiedElementCount == 0)
    {
        std::cout << " Empty";
    }
    std::cout << "\n";
    for (int i = 0; i < tableSize; i++)
    {
        if (table[i].key == "")
            continue;

        std::cout << "[" << std::setw(3) << i << "] ";
        std::cout << "[" << table[i].key << "] ";
        std::cout << "[";
        for (size_t j = 0; j < table[i].intArray.size(); j++)
        {
            std::cout << table[i].intArray[j];
            if ((j + 1) != table[i].intArray.size())
                std::cout << ", ";
        }
        std::cout << "]\n";
    }
    std::cout.flush();
}