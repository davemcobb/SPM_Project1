#include "ofApp.h"
#include "cFactory.h"

//--------------------------------------------------------------
cFactory::cFactory()
{
}

//--------------------------------------------------------------
cFactory::~cFactory()
{
} 

//--------------------------------------------------------------
void    cFactory::registerTypeSpawner(const std::string& name, LifeSpawnFunction spawnFunction)
{
    if (m_spawnerMap.count(name))
    {
        ofLog(OF_LOG_WARNING, "cFactory: spawner for %s already registered", name);
        return;
    }
    m_spawnerMap[name] = spawnFunction;
}

//--------------------------------------------------------------
std::vector<std::string> cFactory::getRegisteredTypes()
{
    std::vector<string>  types;
    for (auto itr : m_spawnerMap)
        types.push_back(itr.first);
    return types;
}

//--------------------------------------------------------------
void    cFactory::setDefaultLife(const std::string& name)
{
    m_defaultName = name;
}

//--------------------------------------------------------------
std::string cFactory::getRandomLifeName()
{
    // select a randomly chosen life from the map
    std::map<std::string, LifeSpawnFunction>::const_iterator itr = m_spawnerMap.begin();
    std::advance(itr, ofRandom(0, m_spawnerMap.size() - 1));
    return itr->first;
}

//--------------------------------------------------------------
cLife* cFactory::spawn(std::string name, int x, int y, int health)
{
    cLife* pLife = m_spawnerMap[name](x, y, health);
    return pLife;
}

//--------------------------------------------------------------
cLife* cFactory::spawnDefault(int x, int y, int health)
{
    return spawn(m_defaultName, x, y, health);

}
