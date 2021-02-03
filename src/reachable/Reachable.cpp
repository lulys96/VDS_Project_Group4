//
// Created by ludwig on 27.11.18.
//

#include "Reachable.h"
using namespace ClassProject;

Reachable::Reachable(unsigned int stateSize) :
ReachableInterface(stateSize)
{
    for (int i=0; i<stateSize; i++) {
        std::string var_label = "s"+std::to_string(i);
        states.push_back(createVar(var_label)); 
    }
    for (int i=0; i<stateSize; i++) {
        std::string var_label = "next_s"+std::to_string(i);
        next_states.push_back(createVar(var_label)); 
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
    if (sizeof(transitionFunctions) == sizeof(states)) {
        for (int i=0; i<sizeof(states); i++) {
            delta.push_back(transitionFunctions[i]); 
        }
    }
    else
        throw std::out_of_range("transitionFunction size isn't correct!!!");
}

void Reachable::setInitState(const std::vector<bool>& stateVector)
{
    if (sizeof(stateVector) == sizeof(states)) {
        for (int i=0; i<sizeof(states); i++) {
            if (stateVector[i]) initState.push_back(states[i]); 
            else initState.push_back(neg(states[i]));
        }
    }
    else
        throw std::out_of_range("stateVector size isn't correct!!!");
}

BDD_ID Reachable::compute_reachable_states()
{
    BDD_ID tau = compute_transition_relation();
    BDD_ID c_s0 = compute_cs0();

    BDD_ID c_R;
    BDD_ID c_Rit = c_s0;
    do {
        c_R = c_Rit;
        BDD_ID img_r = compute_existential_quantification_S(and2(tau,c_R));
        BDD_ID img_s = imgr_to_imgs(img_r);//form img(s) by renaming s' into s
        c_Rit = or2(c_R,img_s);
    }
    while (c_R != c_Rit);
    return c_R;
}

BDD_ID Reachable::compute_transition_relation()
{
    std::vector<BDD_ID> xnor_id; 
    for (int i=0; i<sizeof(states); i++) {
        xnor_id[i] = xnor2((next_states[i]),delta[i]);   
    }
    BDD_ID last_id = xnor_id[0];
    for (int i=1; i<sizeof(states); i++) {
        last_id = and2(last_id,xnor_id[i]);  
    }
    return last_id;
}

BDD_ID Reachable::compute_cs0()
{
    BDD_ID last_id = initState[0];
    for (int i=1; i<sizeof(states); i++) {
        last_id = and2(last_id,initState[i]);   
    }
    return last_id
}

BDD_ID Reachable::compute_existential_quantification_S(BDD_ID node)
{
    BDD_ID exQuantS = or2(coFactorTrue(node,states[0]),
                          coFactorFalse(node,states[0])); //s0
    for (int i=1; i<sizeof(states); i++) {
        exQuantS = or2(coFactorTrue(exQuantS,states[i]),
                      coFactorFalse(exQuantS,states[i])); //s1
    }

}

BDD_ID Reachable::imgr_to_imgs(img_r)
{

}