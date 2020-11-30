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
    ASSERT_TRUE(my_manager.uni_table.size()==2);
}

/* TEST_F(ManagerTest, CtorTableTermNodes) {
    ASSERT_TRUE(
    (my_manager.uni_table[0].high == 0) &&
    (my_manager.uni_table[0].low == 0) &&
    (my_manager.uni_table[0].top_var == 0));
    ASSERT_TRUE(
    (my_manager.uni_table[1].high == 1) &&
    (my_manager.uni_table[1].low == 1) &&
    (my_manager.uni_table[1].top_var == 1)); */
}


#endif //VDS_PROJECT_TESTS_H

