//// A minimalistic BDD library, following Wolfgang Kunz lecture slides
////
//// Written by Markus Wedler 2014

#ifndef mwBDD_H
#define mwBDD_H

#include <cassert>
#include <iostream>
#include <list>
#include <vector>
#include <unordered_map>
#include <set>
#include <string>
#include <stdexcept>
#include <algorithm> 
#include "ManagerInterface.h"
#include <boost/functional/hash.hpp>

class ManagerTest;
namespace ClassProject {
class Manager : public ManagerInterface {
    public:
    Manager(); /** Class constructer, initializes terminal nodes */

/// createVar(const std::string &label)
/** Creates a new variable in the OBDD 
*   \param label std::string defining the name of the new variable
*   \return Returns the BDD_ID of the new variable 
******************************************************************/
    BDD_ID createVar(const std::string &label) override;

/// &True()
/** Checks the BDD_ID of the terminal node true
*   \return returns the BDD_ID of the terminal node true
******************************************************************/
    const BDD_ID &True() override;

/// &False()
/** Checks the BDD_ID of the terminal noe false
*   \return returns the BDD_ID of the terminal node true
******************************************************************/
    const BDD_ID &False() override;

/// isConstant(const BDD_ID f))
/** Checks if node passed as input is a constant or variable
*   \return returns true if the node is a terminal case
******************************************************************/
    bool isConstant(const BDD_ID f) override;

/// isVariable(const BDD_ID x)
/** Checks if node passed as input is a constant or variable
*   \return true if the node is a variable
******************************************************************/
    bool isVariable(const BDD_ID x) override;

/// topVar(const BDD_ID f))
/** Checks the BDD_ID of the top variable of the BDD_ID passed as input
*   \return BDD_ID of the top variable
******************************************************************/
    BDD_ID topVar(const BDD_ID f) override;

/// ite(const BDD_ID i, const BDD_ID t, const BDD_ID e)
/** implements the i-t-e algorithm and creates a new BDD_ID for the set, if not already present in unique table
*   \return BDD_ID of new node
******************************************************************/
    BDD_ID ite(const BDD_ID i, const BDD_ID t, const BDD_ID e) override;

/// coFactorTrue(const BDD_ID f, BDD_ID x)
/** Calculates the positive cofactor of a node with respect to x
*   \return BDD_ID of the positive cofactor
******************************************************************/
    BDD_ID coFactorTrue(const BDD_ID f, BDD_ID x) override;

/// coFactorFalse(const BDD_ID f, BDD_ID x)
/** Calculates negative cofactor of a node with respect to x
*   \return BDD_ID of the negative cofactor
******************************************************************/
    BDD_ID coFactorFalse(const BDD_ID f, BDD_ID x) override;

/// coFactorTrue(const BDD_ID f)
/** Calculates the positive cofactor of a node with respect to top variable
*   \return BDD_ID of the negative cofactor
******************************************************************/
    BDD_ID coFactorTrue(const BDD_ID f) override;

/// coFactorFalse(const BDD_ID f)
/** Calculates the negative cofactor of a node with respect to top variable
*   \return BDD_ID of the negative cofactor
******************************************************************/
    BDD_ID coFactorFalse(const BDD_ID f) override;

/// and2(const BDD_ID a, const BDD_ID b)
/** Computes the "and"-function of two nodes and creates a new new BDD-entry if not already present
*   \return BDD_ID of the entry in the unique-table
******************************************************************/
    BDD_ID and2(const BDD_ID a, const BDD_ID b) override;

/// or2(const BDD_ID a, const BDD_ID b)
/** Computes the "or"-function of two nodes and creates a new new BDD-entry if not already present
*   \return BDD_ID of the entry in the unique-table
******************************************************************/
    BDD_ID or2(const BDD_ID a, const BDD_ID b) override;

/// xor2(const BDD_ID a, const BDD_ID b)
/** Computes the "xor"-function of two nodes and creates a new new BDD-entry if not already present
*   \return BDD_ID of the entry in the unique-table
******************************************************************/
    BDD_ID xor2(const BDD_ID a, const BDD_ID b) override;

/// neg(const BDD_ID a)
/** negates the given node
*   \return BDD_ID of the entry in the unique-table
******************************************************************/
    BDD_ID neg(const BDD_ID a) override;

/// nand2(const BDD_ID a, const BDD_ID b)
/** Computes the "nand"-function of two nodes and creates a new new BDD-entry if not already present
*   \return BDD_ID of the entry in the unique-table
******************************************************************/
    BDD_ID nand2(const BDD_ID a, const BDD_ID b) override;

/// nor2(const BDD_ID a, const BDD_ID b)
/** Computes the "nor"-function of two nodes and creates a new new BDD-entry if not already present
*   \return BDD_ID of the entry in the unique-table
******************************************************************/
    BDD_ID nor2(const BDD_ID a, const BDD_ID b) override;

/// getTopVarName(const BDD_ID &root)
/** Finds the top variable from the given root
*   \return Name of the top variable
******************************************************************/
    std::string getTopVarName(const BDD_ID &root) override;

/// findNodes(const BDD_ID &root, std::set<BDD_ID> &nodes_of_root)
/** finds all reachable nodes from the root(including itself) and inserts into a set of BDD_IDs
*   \return void
******************************************************************/
    void findNodes(const BDD_ID &root, std::set<BDD_ID> &nodes_of_root) override;

/// findVars(const BDD_ID &root, std::set<BDD_ID> &vars_of_root)
/** finds all reachable variables from the given root and inserts into a set of BDD_IDs
*   \return void
******************************************************************/
    void findVars(const BDD_ID &root, std::set<BDD_ID> &vars_of_root) override;

/// size_t uniqueTableSize()
/**
*   \return size of the unique table
******************************************************************/
	size_t uniqueTableSize() override;

/// topVarFromSet(const BDD_ID i, const BDD_ID t, const BDD_ID e)
/** Computes the top variable of the set passed to the function
*   \return BDD_ID of the top variable
******************************************************************/
    BDD_ID topVarFromSet(const BDD_ID i, const BDD_ID t, const BDD_ID e);

    private:
    struct TableEntry {
        BDD_ID id;
        BDD_ID high;
        BDD_ID low;
        BDD_ID top_var;
        std::string label;
    };
   
    typedef std::array<BDD_ID,3> key_type;
    std::vector<TableEntry> uni_table;
    std::unordered_map<key_type,BDD_ID,boost::hash<key_type>> comp_table;
   
};
}
#endif

// Local Variables:
// mode: c++
// End:
