//
// Created by ludwig on 27.11.18.
//

#include "Reachable.h"
using namespace ClassProject;

Reachable::Reachable(unsigned int stateSize) :
ReachableInterface(stateSize)
{
    for (int i=0; i<stateSize; i++) {
        states.push_back(i);
    }
}

const std::vector<BDD_ID> &Reachable::getStates() const
{
    return states;
}

BDD_ID Reachable::xnor2(BDD_ID a, BDD_ID b) 
{

}
