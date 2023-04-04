#pragma once
#include <vector>
#include <array>
#include "iCellQuery.h"
#include "iAction.h"
#include "cLife.h"

// forward declaration
class cFactory;

class cCellMatrix : public iQuery, public iAction
{
public:
    cCellMatrix() = delete;     // no default constructor - must set width & height
    cCellMatrix(int cellSize, int width, int height);
    virtual ~cCellMatrix();
    void    setup(cFactory* factory);
    void    reset();
    void    update();
    void    draw();
    int     getWidth() const { return m_lifeWidth; }
    int     getHeight() const { return m_lifeHeight; }
    int     getColX(int col) { return col * m_cellSize + m_cellSize / 2; }
    int     getRowY(int row) { return row * m_cellSize + m_cellSize / 2; }
    int     getXCol(int x) { return x / m_cellSize; }
    int     getYRow(int y) { return y / m_cellSize; }
    cLife*  getLifeAtPos(int row, int col) const;
    void    setLifeAtPos(cLife* pLife, int row, int col);
    int     getLivingCellCount(void) const;

    // Query interface - get information
    virtual std::vector<std::string> queryLifeTypes();
    virtual std::vector<cLife*> queryNeighboursWithinDistance(cLife* pLife, int distance);
    virtual cLife* queryNeighbourAt(cLife* pLife, int xOffset, int yOffset) override;
    virtual cLife* queryNearestOfType(cLife* pLife, std::string type) override;
    virtual int    querySiblingCount(cLife* pLife) override;
    virtual int    queryLifeCount(std::string type) override;
    virtual int    queryTypeCount(std::string type) override;
    virtual std::string queryLifeType(cLife* pLife) override;

    // Action interface - take action
    virtual cLife*  actionCreateType(std::string typeName, int x, int y, int health) override;
    virtual cLife*  actionMove(cLife* pLife, int xNew, int yNew, bool swap) override;
    virtual void    actionKill(cLife* pLife) override;
    virtual void    actionKill(int x, int y) override;

private:
    const unsigned int  NUM_ADJACENT_CELLS = 8;

    // grid drawing data
    int m_cellSize{ 0 };                // size of each cell
    int	m_lifeWidth{ 0 };               // width of the matrix, in cells
    int m_lifeHeight{ 0 };              // height of the matrix, in cells

    //LifeSpawnFunction   m_baseSpawner{ nullptr };
    cFactory* mp_factory{ nullptr };
    std::vector <std::vector<cLife*>> m_life;   // the cell matrix is a container for a 2D array of cells
};

