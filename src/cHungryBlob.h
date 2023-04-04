#pragma once
#include "cLife.h"

class cHungryBlob :
    public cLife
{
public:
    cHungryBlob(int xpos, int ypos);
    virtual ~cHungryBlob() = default;
    virtual void draw();   
    virtual void applySimulationChanges() override;

    static const std::string  m_LifeName;
    static std::string getLifeName() { return { m_LifeName }; }

    // note - each class that derives from cLife should have its own spawn()
    static cLife* spawn(int x, int y, int health); 


protected:
    virtual NeighbourClassMap countNeighbours(const std::vector<cLife*>& simNeighbours) override;
    virtual int interactWithNeighbours(const std::vector<cLife*>& simNeighbours, NeighbourClassMap& neighbourMap) override;
    virtual int updateHealthChange(int health) override;
};

