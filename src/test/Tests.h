//
// Created by tobias on 21.11.16.
//

#ifndef VDS_PROJECT_GTESTS_H
#define VDS_PROJECT_GTESTS_H

#include "gtest/gtest.h"
#include "../Manager.h"
#include "../ManagerInterface.h"

using namespace ClassProject;
class ManagerTest : public ::testing::Test {
protected:
    Manager my_manager;
};

TEST_F(ManagerTest, CtorTableSize) {
    ASSERT_TRUE(my_manager.uniqueTableSize()==2);
}

TEST_F(ManagerTest, CtorTableTermNodes) {
    ASSERT_TRUE(my_manager.True()==1);
    ASSERT_TRUE(my_manager.False()==0); 
} 

TEST_F(ManagerTest, createVar) {
//NEXT_ID_ava(id) - "x1"(label) - NEXT_ID_ava(TopVar) - 1(high) - 0(low)
    my_manager.createVar("x1");
    ASSERT_TRUE(my_manager.uniqueTableSize()==3);
    ASSERT_TRUE(my_manager.getTopVarName(2)=="x1");
    std::set<BDD_ID> nodes;
    my_manager.findNodes(2,nodes);
    const bool is_in = (nodes.find(0) != nodes.end()) &
                       (nodes.find(1) != nodes.end()) &
                       (nodes.find(2) != nodes.end());
    ASSERT_TRUE(is_in);
    ASSERT_TRUE(my_manager.topVar(2)==2);
}

TEST_F(ManagerTest, and2)
{   
    BDD_ID idA = my_manager.createVar("a");
    BDD_ID idB = my_manager.createVar("b");
    BDD_ID andID = my_manager.and2(idA,idB);
    std::set<BDD_ID> nodes;
    my_manager.findNodes(andID,nodes);
    const bool is_in = (nodes.find(4) != nodes.end()) &
                       (nodes.find(0) != nodes.end()) &
                       (nodes.find(3) != nodes.end());
    
    ASSERT_TRUE(andID==4);
    ASSERT_TRUE(my_manager.getTopVarName(andID)=="a");
    ASSERT_TRUE(my_manager.topVar(andID)==2);
    ASSERT_TRUE(is_in);
}
  

#endif //VDS_PROJECT_TESTS_H

