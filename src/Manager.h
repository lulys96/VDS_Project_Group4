// A minimalistic BDD library, following Wolfgang Kunz lecture slides
//
// Written by Markus Wedler 2014
/********************************************************************
* \file Manager.h
*       Manager program written by Luiza Correa and Simon Loevald   *
*       as a class project for the VDS-lecture WS20/21              *
*       under supervision of Dipl.Ing. Tobias Ludwig                *
*********************************************************************/

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
    Manager();

    BDD_ID createVar(const std::string &label) override;

    const BDD_ID &True() override;

    const BDD_ID &False() override;

    bool isConstant(const BDD_ID f) override; /** Checks if node passed as input is a constant or variable */

    bool isVariable(const BDD_ID x) override;

    BDD_ID topVar(const BDD_ID f) override;

    BDD_ID ite(const BDD_ID i, const BDD_ID t, const BDD_ID e) override;

    BDD_ID coFactorTrue(const BDD_ID f, BDD_ID x) override;

    BDD_ID coFactorFalse(const BDD_ID f, BDD_ID x) override;

    BDD_ID coFactorTrue(const BDD_ID f) override;

    BDD_ID coFactorFalse(const BDD_ID f) override;

    BDD_ID and2(const BDD_ID a, const BDD_ID b) override;

    BDD_ID or2(const BDD_ID a, const BDD_ID b) override;

    BDD_ID xor2(const BDD_ID a, const BDD_ID b) override;

    BDD_ID neg(const BDD_ID a) override;

    BDD_ID nand2(const BDD_ID a, const BDD_ID b) override;

    BDD_ID nor2(const BDD_ID a, const BDD_ID b) override;

    std::string getTopVarName(const BDD_ID &root) override;

    void findNodes(const BDD_ID &root, std::set<BDD_ID> &nodes_of_root) override;

    void findVars(const BDD_ID &root, std::set<BDD_ID> &vars_of_root) override;

	size_t uniqueTableSize() override;

    BDD_ID topVarFromSet(const BDD_ID i, const BDD_ID t, const BDD_ID e);

    private:
    struct TableEntry {
        BDD_ID id;
        BDD_ID high;
        BDD_ID low;
        BDD_ID top_var;
        std::string label;
    };

    BDD_ID comp_table[3];

    std::vector<TableEntry> uni_table;

    
};
}
#endif

// Local Variables:
// mode: c++
// End:
