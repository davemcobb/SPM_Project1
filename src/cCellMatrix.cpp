#include <array>
#include "ofMain.h"
#include "cCellMatrix.h"
#include "cFactory.h"
#include "cLife.h"


//--------------------------------------------------------------
// cCellMatrix constructor
//  initialize the 2d array of cells; this actually means an array of arrays of (width) cells
//  initialise the Query object the life classes can use to find out about life around themselves
cCellMatrix::cCellMatrix(int cellSize, int width, int height)
    : iQuery{}, m_cellSize{ cellSize }, m_lifeWidth{ width / cellSize }, m_lifeHeight{ height / cellSize },
    m_life { m_lifeHeight, std::vector<cLife*>{ (const unsigned int) m_lifeWidth, nullptr} }
{
    cLife::setupInterfaces(this, this);
}

//--------------------------------------------------------------
// destroy all life! Go through the vectors and delete the life pointers.
// assume OF won't allow draw or update to be called during destruction. that would be bad.
cCellMatrix::~cCellMatrix()
{
    for (auto row : m_life)
    {
        for (auto life : row)
        {
            if (life != nullptr)
                delete life;
        }
        row.resize(0);
    }
    m_life.resize(0);
}

//--------------------------------------------------------------
// create each Life and set the center position of each during construction
void cCellMatrix::setup(cFactory* factory)
{
    mp_factory = factory;

    int xCellsCenter{ m_cellSize / 2 };
    int yCellsCenter{ m_cellSize / 2 };
    for (auto r = 0; r < m_lifeHeight; r++)
    {
        for (auto c = 0; c < m_lifeWidth; c++)
        {
            m_life[r][c] = mp_factory->spawnDefault(c * m_cellSize + xCellsCenter, r * m_cellSize + yCellsCenter, 0);
            m_life[r][c]->setup();
        }
    }
}

//--------------------------------------------------------------
// kills all life in cells; resets life to base cLife with health 0
void cCellMatrix::reset()
{
    int xCellsCenter{ m_cellSize / 2 };
    int yCellsCenter{ m_cellSize / 2 };
    for (auto r = 0; r < m_lifeHeight; r++)
    {
        for (auto c = 0; c < m_lifeWidth; c++)
        {
            // kill off all non-base lifes; replace with base life
            if (m_life[r][c]->getName() != mp_factory->getDefaultLifeName())
            {
                delete  m_life[r][c];
                m_life[r][c] = mp_factory->spawnDefault(c * m_cellSize + xCellsCenter, r * m_cellSize + yCellsCenter, 0);
                m_life[r][c]->setup();
            }
            m_life[r][c]->reset();
        }
    }
}

//--------------------------------------------------------------
// draw all the cells
void    cCellMatrix::draw()
{
    // draw the grid
    ofSetColor(ofColor::gray);
    for (auto x = m_cellSize; x < ofGetWidth(); x += m_cellSize)
        ofDrawLine(x, 0, x, ofGetHeight());
    for (auto y = m_cellSize; y < ofGetHeight(); y += m_cellSize)
        ofDrawLine(0, y, ofGetWidth(), y);

    for (auto row : m_life)
    {
        for (auto life : row)
        {
            if (life != nullptr)
                life->draw();
        }
    }
}

//--------------------------------------------------------------
void    cCellMatrix::update()
{
    // The simulation of a generation runs in 2 stages

    // Run simulation part 1 of each cell - iterate by row from top to bottom, and column from left to right.
    //   Use a permanent buffer to pass the neighbours of each cell to its life for simulation 
    static std::vector<cLife*>   neighbourLife{ NUM_ADJACENT_CELLS, nullptr };
    for (auto r = 0, rowAbove = r - 1, rowBelow = r + 1; r < m_lifeHeight; r++, rowAbove++, rowBelow++)
    {
        // calc indices for row above and below current row
        for (auto c = 0, colLeft = c - 1, colRight = c + 1; c < m_lifeWidth; c++, colLeft++, colRight++)
        {
            // set the neighbours array - row above
            neighbourLife[0] = (rowAbove >= 0 && colLeft >= 0) ? (m_life[rowAbove][colLeft]) : nullptr;
            neighbourLife[1] = (rowAbove >= 0) ? (m_life[rowAbove][c]) : nullptr;
            neighbourLife[2] = (rowAbove >= 0 && colRight < m_lifeWidth) ? (m_life[rowAbove][colRight]) : nullptr;

            // set the neighbours array - this row
            neighbourLife[3] = (colLeft >= 0) ? (m_life[r][colLeft]) : nullptr;
            neighbourLife[4] = (colRight < m_lifeWidth) ? (m_life[r][colRight]) : nullptr;

            // set the neighbours array - row below
            neighbourLife[5] = (rowBelow < m_lifeHeight&& colLeft >= 0) ? (m_life[rowBelow][colLeft]) : nullptr;
            neighbourLife[6] = (rowBelow < m_lifeHeight) ? (m_life[rowBelow][c]) : nullptr;
            neighbourLife[7] = (rowBelow < m_lifeHeight&& colRight < m_lifeWidth) ? (m_life[rowBelow][colRight]) : nullptr;;

            // now that neighbours are correct, run simulation for life at this cell
            // this cell decide how to respond based on the surrouding life 
            m_life[r][c]->simulate(neighbourLife);
        }
    }

    // 2. Apply health or other changes from part 1 to each cell
    for (auto r = 0, rowAbove = r - 1, rowBelow = r + 1; r < m_lifeHeight; r++, rowAbove++, rowBelow++)
    {
        for (auto c = 0, colLeft = c - 1, colRight = c + 1; c < m_lifeWidth; c++, colLeft++, colRight++)
        {
            m_life[r][c]->applySimulationChanges();
        }
    }
}

//--------------------------------------------------------------
cLife* cCellMatrix::getLifeAtPos(int row, int col) const
{
    // if the query tries to look outside the valid range, return nullptr
    if ((row >= 0) || (row < m_lifeHeight)
        || (col >= 0) || (col < m_lifeWidth))
    {
        return (m_life[row][col]);
    }
    return nullptr;
}

//--------------------------------------------------------------
// assign a new life to a location in the matrix
// previous life is deleted immediately
void cCellMatrix::setLifeAtPos(cLife* pLife, int row, int col)
{
    cLife* pOld = m_life[row][col];
    m_life[row][col] = pLife;
    delete pOld;
}

//--------------------------------------------------------------
// return the number of cells with active life (health != 0)
int     cCellMatrix::getLivingCellCount(void) const
{
    int count = 0;
    for (auto row : m_life)
    {
        for (auto life : row)
        {
            if (life->isAlive())
                count++;
        }
    }
    return count;
}

//--------------------------------------------------------------
// returns a vector of all the registered types
std::vector<std::string> cCellMatrix::queryLifeTypes()
{
    return mp_factory->getRegisteredTypes();
}

//--------------------------------------------------------------
// returns a vector of nearby life objects, live or dead, of any type
// this check the "square" of cells at the specified distance.
std::vector<cLife*> cCellMatrix::queryNeighboursWithinDistance(cLife* pLife, int distance)
{
    int xPos, yPos;
    pLife->getPosition(xPos, yPos);

    // clamp the search range to with the cell bounds: 0 to cellWidth or cellHeight
    int firstRow = (yPos - distance < 0) ? 0 : yPos - distance;
    int lastRow = (yPos + distance >= m_lifeHeight) ? yPos + distance : m_lifeHeight-1;
    int firstCol = (xPos - distance < 0) ? 0 : xPos - distance;
    int lastCol = (xPos + distance >= m_lifeWidth) ? xPos + distance : m_lifeWidth - 1;

    cLife* neighbour{ nullptr };
    m_neighbours.clear();

    // Put all of neighbouring life objects into an array.
    // They are added from top row to bottom row, left to right
    for (auto r = firstRow; r <= lastRow; r++)
    {
        for (auto c = firstCol; c <= lastCol; c++)
        {
            if (m_life[r][c] == pLife)
                continue;
            m_neighbours.push_back(m_life[r][c]);
        }
    }
    return m_neighbours;
}

//--------------------------------------------------------------
// check a specific cell location at relative to this life
cLife* cCellMatrix::queryNeighbourAt(cLife* pLife, int xOffset, int yOffset)
{
    int xPos, yPos;
    pLife->getPosition(xPos, yPos);
    return getLifeAtPos(yPos + yOffset, xPos + xOffset);
}

//--------------------------------------------------------------
// find the nearest life of the specified class. Note this is a simple test, 
//  and will return the first found if more than one at the same distance.
cLife* cCellMatrix::queryNearestOfType(cLife* pLife, std::string type)
{
    cLife* pNearest { nullptr };
    int distance { m_lifeWidth * m_lifeWidth };
    int xPos, yPos;
    pLife->getPosition(xPos, yPos);
    for (auto row : m_life)
    {
        for (auto life : row)
        {
            if (life->getName() == pLife->getName())
            {
                int x, y;
                life->getPosition(x, y);
                int dist = sqrt((xPos - x) * (xPos - x) + (yPos - y) * (yPos - y));
                if (dist < distance)
                {
                    distance = dist;
                    pNearest = life;
                }
            }
        }
    }
    return pNearest;
}

//--------------------------------------------------------------
// returns the total number of cells of the same type
int    cCellMatrix::querySiblingCount(cLife* pLife)
{
    return queryTypeCount(pLife->getName());
}

//--------------------------------------------------------------
// returns the total number of cells of the type specified
int    cCellMatrix::queryTypeCount(std::string type)
{
    int count{ 0 };
    for (auto row : m_life)
    {
        for (auto life : row)
        {
            if (life->getName() == type)
                count++;
        }
    }
    return count;
}

//--------------------------------------------------------------
// returns the total number of living cells 
int    cCellMatrix::queryLifeCount(std::string type)
{
    return getLivingCellCount();
}

//--------------------------------------------------------------
// get the name of a specific life object
std::string cCellMatrix::queryLifeType(cLife* pLife)
{
    if (nullptr == pLife)
        return { "" };
    return pLife->getName();
}

//--------------------------------------------------------------
cLife* cCellMatrix::actionCreateType(std::string typeName, int x, int y, int health)
{
    mp_factory->spawn(typeName, x, y, health);
    return nullptr;
}

//--------------------------------------------------------------
cLife* cCellMatrix::actionMove(cLife* pLife, int xNew, int yNew, bool swap)
{
    // get the life at the new location
    int rowNew = getYRow(yNew);
    int colNew = getXCol(xNew);
    cLife* pOld = m_life[rowNew][colNew];

    int xOld, yOld;
    pLife->getPosition(xOld, yOld);

    // move the life to the new location
    int rowOld = getYRow(yOld);
    int colOld = getXCol(xOld);
    m_life[rowNew][colNew] = pLife;
    pLife->setPosition(xNew, yNew);
    
    // now, what to do a the old location? swap or reset
    if (swap)
    {
        m_life[rowOld][colOld] = pOld;
        pOld->setPosition(xOld, yOld);
        return pOld;
    } 
    // otherwise delete life at destination and create a new life in the old location
    delete pOld;
    m_life[rowOld][colOld] = mp_factory->spawnDefault(xOld, yOld, 0);
    m_life[rowOld][colOld]->setup();
    return m_life[rowOld][colOld];
}

//--------------------------------------------------------------
void  cCellMatrix::actionKill(cLife* pLife)
{
    pLife->reset();
}

//--------------------------------------------------------------
void  cCellMatrix::actionKill(int x, int y)
{
    m_life[getYRow(y)][getXCol(x)]->reset();
}
