#pragma once
#include <string>

// forward declaration rather than #include
class cLife;

class iAction 
{
public:
    virtual cLife* actionCreateType(std::string typeName, int x, int y, int health) = 0;
    virtual cLife* actionMove(cLife* pLife, int xNew, int yNew, bool swap) = 0;
    virtual void actionKill(cLife* pLife) = 0;
    virtual void actionKill(int x, int y) = 0;
protected:
private:
};