#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <iostream>
#include <cstring>

class DBConnection {
public:
    virtual ~DBConnection() {}
    virtual void openConnection() = 0;
    virtual void useConnection() = 0;
    virtual void closeConnection() = 0;
    virtual void execQuery() = 0;
};

class ClassThatUseDb {
    DBConnection* _dataBase;
public:
    explicit ClassThatUseDb(DBConnection* dataBase) : _dataBase(dataBase) {};

    const char* startConnection(const char* cmdOpen) {
        if (strcmp(cmdOpen, "open") == 0) {
            _dataBase->openConnection();
            return "start";
        }
        else {
            return "error";
        }
    }

    int useConnection(int one, int two) {
        if (two == one * one) {
            _dataBase->useConnection();
            _dataBase->execQuery();
            return two * one;
        }
        else {
            return -1;
        }
    }

    bool closeConnection(const char* cmdOpen) {
        if (strcmp(cmdOpen, "close") == 0) {
            _dataBase->closeConnection();
            return true;
        }
        else {
            return false;
        }
    }
};

class MockDatabaseConnection : public DBConnection {
public:
    MOCK_METHOD(void, openConnection, (), (override));
    MOCK_METHOD(void, useConnection, (), (override));
    MOCK_METHOD(void, closeConnection, (), (override));
    MOCK_METHOD(void, execQuery, (), (override));
};

using namespace testing;

TEST(ClassThatUseDbTest, StartConnection) {
    MockDatabaseConnection connection;
    EXPECT_CALL(connection, openConnection()).Times(1);
    ClassThatUseDb classThatUseDb(&connection);
    EXPECT_STREQ("start", classThatUseDb.startConnection("open"));
}

TEST(ClassThatUseDbTest, UseConnection) {
    MockDatabaseConnection connection;
    EXPECT_CALL(connection, useConnection()).Times(1);
    EXPECT_CALL(connection, execQuery()).Times(1);
    ClassThatUseDb classThatUseDb(&connection);
    EXPECT_EQ(4, classThatUseDb.useConnection(2, 4));
}

TEST(ClassThatUseDbTest, CloseConnection) {
    MockDatabaseConnection connection;
    EXPECT_CALL(connection, closeConnection()).Times(1);
    ClassThatUseDb classThatUseDb(&connection);
    EXPECT_TRUE(classThatUseDb.closeConnection("close"));
}

int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}