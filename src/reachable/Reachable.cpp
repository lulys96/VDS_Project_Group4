//
// Created by ludwig on 27.11.18.
//

#include "Reachable.h"
using namespace ClassProject;

Reachable::Reachable(unsigned int stateSize) :
ReachableInterface(stateSize)
{
    //Create a variable for each state bit and save ID in a vector
    for (int i=0; i<stateSize; i++) {
        std::string var_label = "s"+std::to_string(i);
        states.push_back(createVar(var_label)); 
    }
    //Create a variable for each next state bit save ID in a vector
    for (int i=0; i<stateSize; i++) {
        std::string var_label = "next_s"+std::to_string(i);
        next_states.push_back(createVar(var_label)); 
    }
}

const std::vector<BDD_ID> &Reachable::getStates() const
{   
    //return vector with ID of each state bit
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
    // Save the BDD_ID of the transition function of each state bit in a vector
    if (transitionFunctions.size() == states.size()) {
        for (int i=0; i<states.size(); i++) {
            delta.push_back(transitionFunctions[i]); 
        }
    }
    else
        throw std::out_of_range("setDelta: transitionFunction size isn't correct!!!");
}

void Reachable::setInitState(const std::vector<bool>& stateVector)
{
    //Save initial state 
    if (stateVector.size() == states.size()) {
       for (int i=0; i<states.size(); i++) {
           if (stateVector[i]) initState.push_back(states[i]); 
           else initState.push_back(neg(states[i]));
       }
    }
    else
        throw std::out_of_range("setInitState: stateVector size isn't correct!!!");
}


bool Reachable::is_reachable(const std::vector<bool>& stateVector)
{   
    BDD_ID reachables = compute_reachable_states();
    BDD_ID next_id = reachables;
    //For every state bit we select the cofactor true or false, from the result node
    //we apply the same thing for the next state bit until we reach a terminal node.
    for (int i=0; i<states.size(); i++) {
        if(stateVector[i]) next_id = coFactorTrue(next_id,states[i]);
        else               next_id = coFactorFalse(next_id,states[i]); 
    }
    if (next_id==False()) return false;
    else if (next_id==True())  return true;
    else throw std::runtime_error("Result should be a terminal node");
    return false;
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
    //Transition relation = (next_s0 XOR trans_F(0)) + (next_s1 XOR trans_F(1)) ...
    std::vector<BDD_ID> xnor_id; 
    xnor_id.resize(states.size());
    for (int i=0; i<states.size(); i++) {
        xnor_id[i] = xnor2((next_states[i]),delta[i]);   
    }
    BDD_ID last_id = xnor_id[0];
    for (int i=1; i<states.size(); i++) {
        last_id = and2(last_id,xnor_id[i]);  
    }
    return last_id;
}

BDD_ID Reachable::compute_cs0()
{
    //characteristic function of initial state 
    //AND all initial state bits
    BDD_ID last_id = initState[0];
    for (int i=1; i<states.size(); i++) {
        last_id = and2(last_id,initState[i]);   
    }
    return last_id;
}

BDD_ID Reachable::compute_existential_quantification_S(const BDD_ID node)
{
    //Disjunction of cofactors of "node" with respect to state bits.
    BDD_ID exQuantS = or2(coFactorTrue(node,states[0]),
                          coFactorFalse(node,states[0])); //s0
    for (int i=1; i<states.size(); i++) {
        exQuantS = or2(coFactorTrue(exQuantS,states[i]),
                      coFactorFalse(exQuantS,states[i])); //s1
    }
    return exQuantS;
}

BDD_ID Reachable::imgr_to_imgs(const BDD_ID img_r)
{
    std::vector<BDD_ID> xnor_id; 
    xnor_id.resize(states.size());
    for (int i=0; i<states.size(); i++) {
        xnor_id[i] = xnor2(states[i],next_states[i]);   
    }
    BDD_ID temp1 = xnor_id[0];
    for (int i=1; i<states.size(); i++) {
        temp1 = and2(temp1,xnor_id[i]);  
    }
    BDD_ID temp2 = and2(temp1,img_r);
    BDD_ID temp3 = or2(coFactorTrue(temp2, next_states[0]),
                       coFactorFalse(temp2, next_states[0]));
    for (int i=1; i<states.size(); i++) {
        temp3 = or2(coFactorTrue(temp3, next_states[i]),
                       coFactorFalse(temp3, next_states[i]));  
    }
    return temp3;
}