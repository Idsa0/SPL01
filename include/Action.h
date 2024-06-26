#pragma once
#include <string>
#include <vector>
#include "WareHouse.h"
using std::string;
using std::vector;

enum class ActionStatus
{
    COMPLETED,
    ERROR
};

enum class CustomerType
{
    Soldier,
    Civilian,
    Other
};

class BaseAction
{
public:
    BaseAction();
    ActionStatus getStatus() const; 
    virtual void act(WareHouse &wareHouse) = 0; // logic of the operation is contained here
    virtual string toString() const = 0;
    virtual BaseAction *clone() const = 0; // deep copy
    string actionStatusString() const; // actionStatus getter as a string.
	bool isNull; // we added this. 
    virtual ~BaseAction() = default;
	
protected:
    void complete(); // change internal state to a completed order.
    void error(string errorMsg); 
    string getErrorMsg() const;
    void setErrorMsg(string errorMsg); // like error() but without an error print.

private:
    
    string errorMsg;
    ActionStatus status;
};

class SimulateStep : public BaseAction
{

public:
    SimulateStep(int numOfSteps);
    void act(WareHouse &wareHouse) override;
    std::string toString() const override;
    SimulateStep *clone() const override;

private:
    const int numOfSteps;
};

class AddOrder : public BaseAction
{
public:
    AddOrder(int id); // id === customerId
    void act(WareHouse &wareHouse) override;
    string toString() const override;
    AddOrder *clone() const override;
private:
    const int customerId;
};

class AddCustomer : public BaseAction
{
public:
    AddCustomer(const string &customerName, const string &customerType, int distance, int maxOrders);
    void act(WareHouse &wareHouse) override;
    AddCustomer *clone() const override;
    string toString() const override;

private:
    const string customerName;
    const CustomerType customerType;
    const int distance;
    const int maxOrders;
};

class PrintOrderStatus : public BaseAction
{
public:
    PrintOrderStatus(int id);
    void act(WareHouse &wareHouse) override;
    PrintOrderStatus *clone() const override;
    string toString() const override;

private:
    const int orderId;
};

class PrintCustomerStatus : public BaseAction
{
public:
    PrintCustomerStatus(int customerId);
    void act(WareHouse &wareHouse) override;
    PrintCustomerStatus *clone() const override;
    string toString() const override;

private:
    const int customerId;
};

class PrintVolunteerStatus : public BaseAction
{
public:
    PrintVolunteerStatus(int id);
    void act(WareHouse &wareHouse) override;
    PrintVolunteerStatus *clone() const override;
    string toString() const override;

private:
    const int volunteerId;
};

class PrintActionsLog : public BaseAction
{
public:
    PrintActionsLog();
    void act(WareHouse &wareHouse) override;
    PrintActionsLog *clone() const override;
    string toString() const override;

private:
};

class Close : public BaseAction
{
public:
    Close();
    void act(WareHouse &wareHouse) override;
    Close *clone() const override;
    string toString() const override;

private:
};

class BackupWareHouse : public BaseAction
{
public:
    BackupWareHouse();
    void act(WareHouse &wareHouse) override;
    BackupWareHouse *clone() const override;
    string toString() const override;

private:
};

class RestoreWareHouse : public BaseAction
{
public:
    RestoreWareHouse();
    void act(WareHouse &wareHouse) override;
    RestoreWareHouse *clone() const override;
    string toString() const override;

private:
};

class NullAction : public BaseAction
{
public:
    NullAction();
    void act(WareHouse &wareHouse) override;
    NullAction *clone() const override;
    string toString() const override;
};
