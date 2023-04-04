#pragma once

#include <string>
#include <map>

// forward declaration
class cLife;

/*
A simple class to manage the Factory Design pattern

It can create multiple types of Life by using a
std::map of each life's "id" to its spawn function.

Designed to be a singleton without following the full pattern.
*/

typedef cLife* (*LifeSpawnFunction)(int x, int y, int health);

class cFactory
{
public:
    cFactory();
    ~cFactory();
    void    registerTypeSpawner(const std::string& name, LifeSpawnFunction spawnFunction);
    std::vector<string>    getRegisteredTypes();
    void    setDefaultLife(const std::string& name);
    std::string  getDefaultLifeName() { return m_defaultName; }
    std::string  getRandomLifeName();
    cLife*  spawn(std::string, int x, int y, int health);
    cLife*  spawnDefault(int x, int y, int health);

private:
    std::map<std::string, LifeSpawnFunction>  m_spawnerMap;
    std::string     m_defaultName;
};

