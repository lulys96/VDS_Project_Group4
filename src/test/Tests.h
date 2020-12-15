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

TEST_F(ManagerTest, CtorTableSize) 
{
    ASSERT_TRUE(my_manager.uniqueTableSize()==2);
}

TEST_F(ManagerTest, isConstant) 
{
    ASSERT_EQ(my_manager.isConstant(0), true);
    ASSERT_EQ(my_manager.isConstant(1), true);
    ASSERT_EQ(my_manager.isConstant(2), false);
}

TEST_F(ManagerTest, isVariable)
{
    ASSERT_EQ(my_manager.isVariable(0), false);
    ASSERT_EQ(my_manager.isVariable(1), false);
    ASSERT_EQ(my_manager.isVariable(my_manager.createVar("x1")), true);
    ASSERT_EQ(my_manager.isVariable(my_manager.createVar("x2")), true);
    ASSERT_EQ(my_manager.isVariable(my_manager.and2(2,3)), false); //and node is not a var
} 

TEST_F(ManagerTest, CtorTableTermNodes) 
{
    EXPECT_NO_THROW(my_manager.True());
    ASSERT_TRUE(my_manager.True()==1);
    EXPECT_NO_THROW(my_manager.False());
    ASSERT_TRUE(my_manager.False()==0); 
} 

TEST_F(ManagerTest, coFactorFalse)
{
    BDD_ID f = 10;
    ASSERT_THROW(my_manager.coFactorFalse(f), std::out_of_range);
    ASSERT_FALSE(my_manager.coFactorFalse(0));
    ASSERT_TRUE(my_manager.coFactorFalse(1));

    f = my_manager.createVar("x1");
    ASSERT_TRUE(my_manager.coFactorFalse(f)==0);
}

TEST_F(ManagerTest, coFactorPositiv)
{
    BDD_ID f = 5;
    ASSERT_THROW(my_manager.coFactorTrue(f), std::out_of_range);
    ASSERT_FALSE(my_manager.coFactorTrue(0));
    ASSERT_TRUE(my_manager.coFactorTrue(1));

    f = my_manager.createVar("x1");
    ASSERT_TRUE(my_manager.coFactorTrue(f)==1);
 }

TEST_F(ManagerTest, coFactorPositiv2)
{
    BDD_ID f = 5,x;
    ASSERT_THROW(my_manager.coFactorTrue(f,x), std::out_of_range);
    ASSERT_EQ(my_manager.coFactorTrue(0,0), 0);
    ASSERT_EQ(my_manager.coFactorTrue(1,1), 1);
    ASSERT_EQ(my_manager.coFactorTrue(1,0), 1);
    ASSERT_EQ(my_manager.coFactorTrue(0,1), 0);

    f = my_manager.createVar("x1");
    ASSERT_EQ(my_manager.coFactorTrue(f,0), f);
    ASSERT_EQ(my_manager.coFactorTrue(f,1), f);
    ASSERT_EQ(my_manager.coFactorTrue(1,f), 1);
    ASSERT_EQ(my_manager.coFactorTrue(0,f), 0);
    ASSERT_EQ(my_manager.coFactorTrue(f,f), 1);
}

TEST_F(ManagerTest, coFactorNegative2)
{
    BDD_ID f = 5,x;
    ASSERT_THROW(my_manager.coFactorFalse(f,x), std::out_of_range);
    ASSERT_EQ(my_manager.coFactorFalse(0,0), 0);
    ASSERT_EQ(my_manager.coFactorFalse(1,1), 1);
    ASSERT_EQ(my_manager.coFactorFalse(1,0), 1);
    ASSERT_EQ(my_manager.coFactorFalse(0,1), 0);

    f = my_manager.createVar("x1");
    ASSERT_EQ(my_manager.coFactorFalse(f,0), f);
    ASSERT_EQ(my_manager.coFactorFalse(f,1), f);
    ASSERT_EQ(my_manager.coFactorFalse(1,f), 1);
    ASSERT_EQ(my_manager.coFactorFalse(0,f), 0);
    ASSERT_EQ(my_manager.coFactorFalse(f,f), 0);

}

TEST_F(ManagerTest, createVar) 
{
//NEXT_ID_ava(id) - "x1"(label) - NEXT_ID_ava(TopVar) - 1(high) - 0(low)
    my_manager.createVar("x1");
    my_manager.createVar("x1"); // try to create an already existing var
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

TEST_F(ManagerTest, ite_terminal_var)
{
    ASSERT_EQ(my_manager.ite (0,0,1),1);
    ASSERT_EQ(my_manager.ite (0,1,0),0);
    ASSERT_EQ(my_manager.ite (1,1,0),1);
    ASSERT_EQ(my_manager.ite (1,0,1),0);
    BDD_ID idA = my_manager.createVar("a");
    ASSERT_EQ(my_manager.ite (idA,1,0),2);
}

TEST_F(ManagerTest, ite_cofactor)
{
    BDD_ID idA = my_manager.createVar("a");
    BDD_ID idB = my_manager.createVar("b");
    //And function
    ASSERT_EQ(my_manager.ite (idA,idB,0),4);
}

TEST_F(ManagerTest, findNodes)
{
    BDD_ID idA = my_manager.createVar("a");
    BDD_ID idB = my_manager.createVar("b");
    BDD_ID idC = my_manager.createVar("c");
    BDD_ID idD = my_manager.createVar("d");
    BDD_ID andID1 = my_manager.ite (idA,idB,0);
  //  BDD_ID andID2 = my_manager.ite (idC,andID1,0);

    BDD_ID andID3= my_manager.ite (idA,idD,0);

    // std::set<BDD_ID> nodes;
    // my_manager.findNodes(andID2,nodes);
    // const bool is_in = (nodes.find(0) != nodes.end()) &
    //                    (nodes.find(1) != nodes.end()) &
    //                    (nodes.find(idB) != nodes.end()) &
    //                    (nodes.find(andID1) != nodes.end()) &
    //                    (nodes.find(andID2) != nodes.end());
                       
    // const bool not_in = (nodes.find(andID3) != nodes.end()) & // different operation
    //                     (nodes.find(idA) != nodes.end()) & //top var, not high/low
    //                     (nodes.find(idC) != nodes.end()) & //top var, not high/low
    //                     (nodes.find(idD) != nodes.end()); //not related to andID2
    // ASSERT_TRUE(is_in);
    // ASSERT_FALSE(not_in);
}

TEST_F(ManagerTest, or2_var)
{
    BDD_ID idA = my_manager.createVar("a");
    BDD_ID idB = my_manager.createVar("b");
    BDD_ID orID1 = my_manager.or2(0,idA);
    BDD_ID orID2 = my_manager.or2(1,idA);
    BDD_ID orID3 = my_manager.or2(idA,0);
    BDD_ID orID4 = my_manager.or2(idA,1);
    BDD_ID orID5 = my_manager.or2(idA,idA);
    BDD_ID orID6 = my_manager.or2(idA,idB);
    BDD_ID orID7 = my_manager.or2(idB,idA);



    ASSERT_EQ(orID1, idA);
    ASSERT_EQ(orID2, 1);
    ASSERT_EQ(orID3, idA);
    ASSERT_EQ(orID4, 1);
    ASSERT_EQ(orID5, idA);
    ASSERT_EQ(orID6, 4);
    ASSERT_EQ(orID7, 4);
}
TEST_F(ManagerTest, or2_terminal)
{
    BDD_ID orID1 = my_manager.or2(0,0);
    BDD_ID orID2 = my_manager.or2(1,1);
    BDD_ID orID3 = my_manager.or2(1,0);
    BDD_ID orID4 = my_manager.or2(0,1);    


    ASSERT_TRUE(orID1 == 0);
    ASSERT_TRUE(orID2 == 1); 
    ASSERT_TRUE(orID3 == 1);
    ASSERT_TRUE(orID4 == 1);


}

TEST_F(ManagerTest, xor2_terminal)
{
    BDD_ID xorID1 = my_manager.xor2(0,0);
    BDD_ID xorID2 = my_manager.xor2(1,1);
    BDD_ID xorID3 = my_manager.xor2(1,0);
    BDD_ID xorID4 = my_manager.xor2(0,1);    


    ASSERT_EQ(xorID1, 0);
    ASSERT_EQ(xorID2, 0); 
    ASSERT_EQ(xorID3, 1);
    ASSERT_EQ(xorID4, 1);


}

TEST_F(ManagerTest, xor2_var)
{
    BDD_ID idA = my_manager.createVar("a");
    BDD_ID idB = my_manager.createVar("b");
    BDD_ID xorID1 = my_manager.xor2(idA,idA);
    BDD_ID xorID2 = my_manager.xor2(0,idA);
    BDD_ID xorID3 = my_manager.xor2(idA,0);
    BDD_ID xorID4 = my_manager.xor2(idA,1);
    BDD_ID xorID5 = my_manager.xor2(1,idA);
    BDD_ID xorID6 = my_manager.xor2(idA,idB);
    BDD_ID xorID7 = my_manager.xor2(idB,idA);



    ASSERT_EQ(xorID1, idA);
    ASSERT_EQ(xorID2, idA);
    ASSERT_EQ(xorID3, idA);
    //ASSERT_EQ(xorID4, 1);
    //ASSERT_EQ(xorID5, idA);
    //ASSERT_EQ(xorID6, 4);
    //ASSERT_EQ(xorID7, 4);
}

TEST_F(ManagerTest, and2_terminals)
{   
    BDD_ID andID1 = my_manager.and2(0,0);
    BDD_ID andID2 = my_manager.and2(0,1);
    BDD_ID andID3 = my_manager.and2(1,0);
    BDD_ID andID4 = my_manager.and2(1,1);
            
    ASSERT_TRUE(andID1==0);  
    ASSERT_TRUE(andID2==0);
    ASSERT_TRUE(andID3==0);
    ASSERT_TRUE(andID4==1);
}

TEST_F(ManagerTest, and2)
{   
    BDD_ID idA = my_manager.createVar("a");
    BDD_ID idB = my_manager.createVar("b");
    BDD_ID andID = my_manager.and2(idA,idB);
    BDD_ID andID2 = my_manager.and2(idA,idB);
    std::set<BDD_ID> nodes;
    my_manager.findNodes(andID,nodes);
    const bool is_in = (nodes.find(andID) != nodes.end()) &
                       (nodes.find(0) != nodes.end()) &
                       (nodes.find(idB) != nodes.end());
    
    ASSERT_TRUE(andID==4);
    ASSERT_TRUE(andID2==4);
    ASSERT_TRUE(my_manager.getTopVarName(andID)=="a");
    ASSERT_TRUE(my_manager.topVar(andID)==2);
    ASSERT_TRUE(is_in);
    //TODO: extend tests to work with ands of ands
}

TEST_F(ManagerTest, nand2_terminals)
{   
    BDD_ID andID1 = my_manager.nand2(0,0);
    BDD_ID andID2 = my_manager.nand2(0,1);
    BDD_ID andID3 = my_manager.nand2(1,0);
    BDD_ID andID4 = my_manager.nand2(1,1);
            
    ASSERT_TRUE(andID1==1);  
    ASSERT_TRUE(andID2==1);
    ASSERT_TRUE(andID3==1);
    ASSERT_TRUE(andID4==0);
}

TEST_F(ManagerTest, nand2)
{   
    BDD_ID idA = my_manager.createVar("a");
    BDD_ID idB = my_manager.createVar("b");
    BDD_ID andID = my_manager.nand2(idA,idB);
    BDD_ID andID2 = my_manager.nand2(idA,idB);
    std::set<BDD_ID> nodes;
    my_manager.findNodes(andID,nodes);
    const bool is_in = (nodes.find(andID) != nodes.end()) &
                       (nodes.find(1) != nodes.end()) &
                       (nodes.find(idB) != nodes.end());
    
    ASSERT_TRUE(andID==andID2);
    ASSERT_TRUE(my_manager.getTopVarName(andID)=="a");
    ASSERT_TRUE(my_manager.topVar(andID)==2);
    ASSERT_TRUE(is_in);
    //TODO: extend tests to work with nands of nands
}

TEST_F(ManagerTest, neg_terminal)
{   
    BDD_ID negID1 = my_manager.neg(0);
    BDD_ID negID2 = my_manager.neg(1);
            
    ASSERT_TRUE(negID1==1);  
    ASSERT_TRUE(negID2==0);
}

TEST_F(ManagerTest, neg)
{   
    BDD_ID idA = my_manager.createVar("a");
    BDD_ID idB = my_manager.createVar("b");
    BDD_ID negA = my_manager.neg(idA);
    BDD_ID negB = my_manager.neg(idB);
    
    ASSERT_NE(my_manager.neg(idA),idA);
    ASSERT_EQ(my_manager.neg(negA),idA);
    //TODO: extend tests to work with neg of other functions
}

TEST_F(ManagerTest, topVarFromSet)
{
    BDD_ID idA = my_manager.createVar("a");
    BDD_ID idB = my_manager.createVar("b");
    BDD_ID topVariable = my_manager.topVarFromSet(idB,1,idA);
}


#endif //VDS_PROJECT_TESTS_H

