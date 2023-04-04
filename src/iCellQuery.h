#pragma once
#include <vector>
#include <string>

// forward declaration rather than #include
class cLife;

class iQuery
{
public:
    virtual ~iQuery() {};
    virtual std::vector<std::string> queryLifeTypes() = 0;
    virtual std::vector<cLife*> queryNeighboursWithinDistance(cLife* pLife, int distance) = 0;
    virtual cLife* queryNeighbourAt(cLife* pLife, int xOffset, int yOffset) = 0;
    virtual cLife* queryNearestOfType(cLife* pLife, std::string type) = 0;
    virtual int    querySiblingCount(cLife* pLife) = 0;
    virtual int    queryLifeCount(std::string type) = 0;
    virtual int    queryTypeCount(std::string type) = 0;
    virtual std::string queryLifeType(cLife* pLife) = 0;

protected:
    iQuery() = default;
    std::vector<cLife*>  m_neighbours;
};
