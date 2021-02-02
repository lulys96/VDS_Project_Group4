//
// Created by ludwig on 27.11.18.
//

#include "Reachable.h"
using namespace ClassProject;

Reachable::Reachable(unsigned int stateSize) :
ReachableInterface(stateSize)
{
    for (int i=0; i<stateSize; i++) {
        states.push_back(False()); //Initialize with all zeroes?
    }
}

const std::vector<BDD_ID> &Reachable::getStates() const
{
    return states;
}

BDD_ID Reachable::xnor2(BDD_ID a, BDD_ID b) 
{
    if(uniqueTableSize() > a && uniqueTableSize() > b){
        if(a == b)
            return True();
        else{
            BDD_ID newID = ite(a,b,neg(b));
            return newID;
        }
    }
    else
        throw std::out_of_range("No existing entry for given ID!!!");
}


void Reachable::setDelta(const std::vector<BDD_ID> &transitionFunctions)
{

}