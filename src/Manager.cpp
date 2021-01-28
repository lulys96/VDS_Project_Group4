/*********************************************************** 
*   @file Manager.h
* Manager program written by Luiza Correa and Simon Loevald   
* as a class project for the VDS-lecture WS20/21              
* under supervision of Dipl.Ing. Tobias Ludwig                
***********************************************************/

#include <cassert>
#include "Manager.h"

// Local Variables:
// mode: c++
// End:

using namespace ClassProject;

Manager::Manager() 
{
    //TableEntry new_node = TableEntry();
    uni_table.push_back(TableEntry()); //use push_back function
    uni_table[0].id=0;
    uni_table[0].high=0;
    uni_table[0].low=0;
    uni_table[0].top_var=0;
    uni_table[0].label="0";

    uni_table.push_back(TableEntry());
    uni_table[1].id=1;
    uni_table[1].high=1;
    uni_table[1].low=1;
    uni_table[1].top_var=1;
    uni_table[1].label="1";
}



bool Manager::isConstant(const BDD_ID f)
{
    if(f == False() || f == True()) //if terminal node return true
        return true;
    else return false;
}

bool Manager::isVariable(const BDD_ID x)
{
    if(uni_table[x].low == False() && uni_table[x].high  == True() 
                                   && uni_table[x].top_var  == x)
        return true;
    else return false;
}

const BDD_ID &Manager::True()
{
    if ((uni_table[1].high == 1) &&
        (uni_table[1].low == 1) &&
        (uni_table[1].top_var == 1)) {
            return uni_table[1].id;
    }
    throw std::out_of_range("Terminal node 1 wrongly initialized"); 
}

const BDD_ID &Manager::False()
{
    if ((uni_table[0].high == 0) &&
        (uni_table[0].low == 0) &&
        (uni_table[0].top_var == 0)) { 
            return uni_table[0].id;
    }
    else throw std::out_of_range("Terminal node 0 wrongly initialized"); 
}

size_t Manager::uniqueTableSize()
{
    return uni_table.size();
}

BDD_ID Manager::ite(const BDD_ID i, const BDD_ID t, const BDD_ID e)
{  
    bool is_terminal;
    BDD_ID term_ID = terminalCaseSolver(i,t,e,is_terminal);
    if (is_terminal) return term_ID; 
    else if (i==t) return ite(i,1,e);
    else if (i==e) return ite(i,t,0);    
    else {
        //Check if exists, return existent 
        key_type key = {i,t,e}; 
        auto search = comp_table.find(key);
        if (search != comp_table.end()) {
            return search->second;
        }
        else {
            //Find topVar
            BDD_ID topVariable = topVarFromSet (i,t,e);
            BDD_ID rhigh = ite(coFactorTrue(i,topVariable),
                       coFactorTrue(t,topVariable),
                       coFactorTrue(e,topVariable));
    
            BDD_ID rlow = ite(coFactorFalse(i,topVariable),
                      coFactorFalse(t,topVariable),
                      coFactorFalse(e,topVariable));
            if (rhigh == rlow) return rhigh;

            BDD_ID R = findOrAddUniTable(topVariable,rhigh,rlow);
            comp_table.insert(std::make_pair(key, R));
            return R;
        }
    }
}

BDD_ID Manager::terminalCaseSolver(const BDD_ID i, const BDD_ID t,
                                   const BDD_ID e, bool &is_terminal)
{
    is_terminal = true;
    //If it is a terminal case, return the result 
    if (isConstant(i)) {
        if (i==1) return t;
        else return e;
    }
    else if (t==e) return t ;
    else if ((i==t) && (e==0)) return i; //AND(A,A,0) = A
    else if ((i==e) && (t==1)) return i;//ite(A,1,A) = or(A,A) = A
    else if (t==0 && uni_table[i].high==0 && e==1 && uni_table[i].low==1) 
        return topVar(i); //neg(neg(A))=A 
    else {
        is_terminal = false;
        return 0;
    }
}


BDD_ID Manager::findOrAddUniTable(const BDD_ID topVariable, const BDD_ID rhigh, const BDD_ID rlow)
{
    //Check if exists, return existent 
    key_type key = {topVariable,rhigh,rlow}; 
    auto search = rev_uni_table.find(key);
    if (search != rev_uni_table.end()) {
        return search->second;
    }
    TableEntry new_node = TableEntry();
    new_node.label = ""; //no label yet
    new_node.high = rhigh;
    new_node.low = rlow;
    new_node.id = uniqueTableSize();
    new_node.top_var = topVariable;
    uni_table.push_back(new_node);
    rev_uni_table.insert(std::make_pair(key, new_node.id));
    return new_node.id;
}

BDD_ID Manager::topVarFromSet (const BDD_ID i, const BDD_ID t, const BDD_ID e)
{
    std::set<BDD_ID> nodes;
    if (!isConstant(i)) nodes.insert(topVar(i)); 
    if (!isConstant(t)) nodes.insert(topVar(t));
    if (!isConstant(e)) nodes.insert(topVar(e));
    BDD_ID topVariable = *nodes.begin();

    return topVariable;
}

std::string Manager::getTopVarName(const BDD_ID &root)
{
    BDD_ID topVar_ID = topVar(root);
    return uni_table[topVar_ID].label;
}

BDD_ID Manager::topVar(const BDD_ID f) 
{
    return uni_table[f].top_var;
}

void Manager::findNodes(const BDD_ID &root, std::set<BDD_ID> &nodes_of_root)
{
    nodes_of_root.insert(uni_table[root].id);
    nodes_of_root.insert(uni_table[root].high);
    nodes_of_root.insert(uni_table[root].low);
    //add current node, check if terminal, then add high and low if not.
    if (uni_table[root].high > 1) {
        findNodes(uni_table[root].high, nodes_of_root );
    }

    if (uni_table[root].low > 1) {
        findNodes(uni_table[root].low, nodes_of_root );
    }
}

void Manager::findVars(const BDD_ID &root, std::set<BDD_ID> &vars_of_root)
{
    std::set<BDD_ID> nodes_of_root;
    //exclude terminal nodes
    findNodes(root,nodes_of_root);
    for(auto it : nodes_of_root) {
        if (it>1) vars_of_root.insert(topVar(it));
    } 
}

BDD_ID Manager::createVar(const std::string &label)
{
    for (auto& it : uni_table) {
        if (it.label == label) return it.id;
    }
    TableEntry new_node = TableEntry();
    new_node.label = label; //no label yet
    new_node.high = 1;
    new_node.low = 0;
    new_node.id = uniqueTableSize();
    new_node.top_var = new_node.id;
    uni_table.push_back(new_node);
    key_type key = {new_node.id,1,0};
    comp_table.insert(std::make_pair(key, new_node.id));
    rev_uni_table.insert(std::make_pair(key, new_node.id));
    return new_node.id;
}

BDD_ID Manager::coFactorTrue(const BDD_ID f)
{
    if(uni_table.size() > f)
        return uni_table[f].high;
    else
        throw std::out_of_range("No existing entry for given ID!!!");
}

BDD_ID Manager::coFactorFalse(const BDD_ID f)
{
    if(uni_table.size() > f)
        return uni_table[f].low;
    else
        throw std::out_of_range("No existing entry for given ID!!!");
        
        
}

BDD_ID Manager::coFactorTrue(const BDD_ID f, BDD_ID x)
{
    if(uni_table.size() > f){
        BDD_ID top = topVar(f);
        if(isConstant(x) || isConstant(f) || top > x)
            return f;
        else if(top == x)
            return uni_table[f].high;
        else {
            BDD_ID    T = coFactorTrue(uni_table[f].high, x);
            BDD_ID    F = coFactorTrue(uni_table[f].low, x);
            return ite(top, T, F);
        }   
    }
    else
        throw std::out_of_range("No existing entry for given ID!!!");
}

BDD_ID Manager::coFactorFalse(const BDD_ID f, BDD_ID x)
{
    if(uni_table.size() > f){
        BDD_ID top = topVar(f);
        if(isConstant(x) || isConstant(f) || top > x)
            return f;
        if(top == x)
            return uni_table[f].low;
        else {
            BDD_ID    T = coFactorFalse(uni_table[f].high, x);
            BDD_ID    F = coFactorFalse(uni_table[f].low, x);
            return ite(top, T, F);
        }
    }
    else
        throw std::out_of_range("No existing entry for given ID!!!");
}


BDD_ID Manager::and2(const BDD_ID a, const BDD_ID b)
{   
    if(uni_table.size() > a && uni_table.size() > b){
        BDD_ID newID = ite(a,b,0);
        return newID;
    }    
    else
        throw std::out_of_range("No existing entry for given ID!!!");    
}

BDD_ID Manager::or2(const BDD_ID a, const BDD_ID b)
{   
    if(uni_table.size() > a && uni_table.size() > b){
        if(a == b)
            return a;
        else
            return ite(a,1,b);
    }
    else
        throw std::out_of_range("No existing entry for given ID!!!");       
}

BDD_ID Manager::nand2(const BDD_ID a, const BDD_ID b)
{
    //is it a problem to always create a neg(b)?
    if(uni_table.size() > a && uni_table.size() > b){
        BDD_ID newID = ite(a,neg(b),1); 
        return newID;
    }
    else
        throw std::out_of_range("No existing entry for given ID!!!");         
}

BDD_ID Manager::neg(const BDD_ID a)
{
    if(uni_table.size() > a){
        BDD_ID newID = ite(a,0,1);
        return newID;
    }
    else
        throw std::out_of_range("No existing entry for given ID!!!");
}

 BDD_ID Manager::xor2(const BDD_ID a, const BDD_ID b)
 {
    if(uni_table.size() > a && uni_table.size() > b){
        if(a == b)
            return 0;
        else{
            BDD_ID newID = ite(a,neg(b),b);
            return newID;
        }
    }
    else
        throw std::out_of_range("No existing entry for given ID!!!");
 }

BDD_ID Manager::nor2(const BDD_ID a, const BDD_ID b)
{
    if(uni_table.size() > a && uni_table.size() > b){
        BDD_ID newID = ite(a,0,neg(b));
        return newID;
    }
    else
        throw std::out_of_range("No existing entry for given ID!!!");
}
