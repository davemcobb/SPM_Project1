#include <string>
#include <vector>
#include <algorithm>
#include "ofMain.h"
#include "cLife.h"


const std::string   cLife::mk_LifeName{ "life" };
iQuery*             cLife::msp_query{ nullptr };
iAction*            cLife::msp_action{ nullptr };

//--------------------------------------------------------------
cLife::cLife(int x, int y)
    : m_xCentre{ x }, m_yCentre{y}
{
    m_health = 0;
}

//--------------------------------------------------------------
cLife::~cLife()
{
    m_health = 0;
}

//--------------------------------------------------------------
cLife::cLife(const cLife& other)
    : cLife{other.m_xCentre, other.m_yCentre }
{
}

//--------------------------------------------------------------
cLife& cLife::operator=(const cLife& other)
{
    if (this == &other)
        return *this;
    this->m_health = other.m_health;
    this->m_name = other.m_name;
    this->m_xCentre = other.m_xCentre;
    this->m_yCentre = other.m_yCentre;
    this->m_drawSize = other.m_drawSize;
    this->m_color = other.m_color;
    return *this;
}

//--------------------------------------------------------------
void cLife::setup()
{
}

//--------------------------------------------------------------
void cLife::draw()
{
    if (m_health == 0)
        return;
    ofFill();
    ofSetColor(m_color);
    ofDrawRectangle(m_xCentre - m_drawSize * .5, m_yCentre - m_drawSize * .5, m_drawSize, m_drawSize);
}

//--------------------------------------------------------------
void cLife::simulate(const std::vector<cLife*>& simNeighbours)
{
    // Setup: check all the neighbouring cell passed to see if they're alive, and if so what type
    NeighbourClassMap neighbourMap = countNeighbours(simNeighbours);

    // 2. apply simuation rules here - resulting is the pending change to cell's health
    //    add code here if needed to call additional life functions

    // NOTE: The base cLife type will only spawn more base cLife! To make it spawn 
    //  other types, this code must be changed, or the default life class must be changed
    //  to one that can check the surrounding life types
    int healthChange = interactWithNeighbours(simNeighbours, neighbourMap);
    updateHealthChange(healthChange);
}

//--------------------------------------------------------------
NeighbourClassMap cLife::countNeighbours(const std::vector<cLife*>& simNeighbours)
{
    // map of neighbor classes for each type, how many alive?
    NeighbourClassMap livingNeighbours;
    for (cLife* pLife : simNeighbours)
    {
        if (nullptr == pLife)
            continue;
        if (pLife->isAlive())
        {
            livingNeighbours[pLife->getName()]++;
        }
    }
    return livingNeighbours;
}

//--------------------------------------------------------------
int cLife::interactWithNeighbours(const std::vector<cLife*>& simNeighbours, NeighbourClassMap& livingNeighbours)
{
    // count the number of live neighbors this cell has; go through the map
    size_t livingNeighbourCount{ 0 };
    for (auto life : livingNeighbours)
        livingNeighbourCount += life.second;

    int healthChange{ 0 };
    switch (livingNeighbourCount)
    {
    // over/under population if neighbours < 2 or > 3
    default:
    case 0:
    case 1:
        if (isAlive())
        {
            healthChange = -1; // decrement health 
#ifdef _DEV_DEBUG
            ofLog(OF_LOG_NOTICE, "death at x=%d,y=%d", m_xCentre, m_yCentre);
#endif
        }
        break;

    // if alive, this life lives on if 2 neighbours - do nothing
    case 2:
#ifdef _DEV_DEBUG
        if (isAlive())
            ofLog(OF_LOG_NOTICE, "living at x=%d,y=%d", m_xCentre, m_yCentre);
#endif
        break;
    
    // if alive, this life lives on if 3 neighbours - do nothing
    case 3:
        // if !alive, this life springs to life - as if through reproduction of the neighbours
        if (!isAlive())
        {
#ifdef _DEV_DEBUG
            ofLog(OF_LOG_NOTICE, "birth at x=%d,y=%d", m_xCentre, m_yCentre);
#endif
            healthChange = 1;
        }
#ifdef _DEV_DEBUG
        else
        {

            ofLog(OF_LOG_NOTICE, "living at x=%d,y=%d", m_xCentre, m_yCentre);
        }
#endif
        break;
    }
    return healthChange;   // no change
}

//--------------------------------------------------------------
int  cLife::updateHealthChange(int health)
{ 
    m_healthChange += health;
    if (m_healthChange > 0)
        return m_healthChange;
    return m_healthChange;
}

//--------------------------------------------------------------
void cLife::applySimulationChanges()
{
    m_health += m_healthChange;

    // for base life, limit health to 0 or 1
    m_health = (m_health < 0) ? 0 : (m_health > MAX_LIFE) ? MAX_LIFE : m_health;
    m_healthChange = 0;
}


//--------------------------------------------------------------
void cLife::setPosition(int x, int y)
{
    m_xCentre = x;
    m_yCentre = y;
}

//--------------------------------------------------------------
void cLife::getPosition(int& x, int& y)
{
    x = m_xCentre;
    y = m_yCentre;
}

//--------------------------------------------------------------
void  cLife::setupInterfaces(iQuery* query, iAction* action)
{
    msp_query = query;
    msp_action = action;
}

//--------------------------------------------------------------
cLife* cLife::spawn(int x, int y, int health)
{
    cLife* pLife = new cLife(x, y);
    pLife->updateHealthChange(health);
    return pLife;
}

