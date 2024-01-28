#include "Action.h"
#include <iostream>
#include <algorithm>

BaseAction::BaseAction() : errorMsg(), status(ActionStatus::ERROR), isNull(false) {}

ActionStatus BaseAction::getStatus() const
{
    return status;
}

void BaseAction::complete()
{
    status = ActionStatus::COMPLETED;
}

void BaseAction::error(string errorMsg)
{
    this->errorMsg = errorMsg;
    status = ActionStatus::ERROR;
    std::cout << errorMsg << std::endl;
}

string BaseAction::getErrorMsg() const
{
    return errorMsg;
}

string BaseAction::actionStatusString() const
{
    if (status == ActionStatus::ERROR)
        return "ERROR";

    return "COMPLETED";
}

SimulateStep::SimulateStep(int numOfSteps) : BaseAction(), numOfSteps(numOfSteps) {}

void SimulateStep::act(WareHouse &wareHouse)
{
    for (int i = 0; i < numOfSteps; i++)
        wareHouse.simulateStep();

    complete();
    // TODO anything else?
}

std::string SimulateStep::toString() const
{
    string name = "SimulateStep";
    string args = std::to_string(numOfSteps);
    string s = actionStatusString();
    return name + " " + args + " " + s;
}

SimulateStep *SimulateStep::clone() const
{
    SimulateStep *cloned = new SimulateStep(numOfSteps);
    cloned->error(this->getErrorMsg());
    if (this->getStatus() == ActionStatus::COMPLETED)
        cloned->complete();

    return cloned;

    // TODO clone methods are highly likely to cause bugs, putting this only here
}

AddOrder::AddOrder(int id) : BaseAction(), customerId(id)
{
    if (customerId == -1)
        isNull = true;
}

void AddOrder::act(WareHouse &wareHouse)
{
    Customer &customer = wareHouse.getCustomer(customerId);
    if (customer.getId() == -1)
    {
        error("Customer does not exist");
        return;
    }

    if (!customer.canMakeOrder())
    {
        error("Customer has reached max order amount");
        return;
    }

    Order *order = new Order(wareHouse.getNewOrderId(), customerId, customer.getCustomerDistance());

    wareHouse.addOrder(order);
    if (customer.addOrder(order->getId()) == -1)
    {
        std::cout << "FATAL @ AddOrder::Act" << std::endl;
    }
    complete();
}

string AddOrder::toString() const
{
    string name = "AddOrder";
    string args = std::to_string(customerId);
    string s = actionStatusString();
    return name + " " + args + " " + s;
}

AddOrder *AddOrder::clone() const
{
    AddOrder *cloned = new AddOrder(customerId);
    cloned->error(this->getErrorMsg());
    if (this->getStatus() == ActionStatus::COMPLETED)
        cloned->complete();

    return cloned;
}

// helper function I -> string to CustomerType enum
CustomerType stringToCustomerType(const string &ct)
{
    if (ct == "soldier")
        return CustomerType::Soldier;

    return CustomerType::Civilian;
}

// helper function II -> CustomerType enum to string
string CustomerTypeToString(CustomerType ct)
{
    if (ct == CustomerType::Civilian)
        return "civilian";

    return "soldier";
}

AddCustomer::AddCustomer(const string &customerName, const string &customerType, int distance, int maxOrders) : BaseAction(), customerName(customerName), customerType(stringToCustomerType(customerType)), distance(distance), maxOrders(maxOrders) {}

void AddCustomer::act(WareHouse &wareHouse)
{
    Customer *customer;
    switch (customerType)
    {
    case CustomerType::Civilian:
        customer = new CivilianCustomer(wareHouse.getNewCustomerId(), customerName, distance, maxOrders);
        break;

    case CustomerType::Soldier:
        customer = new SoldierCustomer(wareHouse.getNewCustomerId(), customerName, distance, maxOrders);
        break;
    }

    wareHouse.addCustomer(customer);
    complete();
}

AddCustomer *AddCustomer::clone() const
{
    AddCustomer *cloned = new AddCustomer(customerName, CustomerTypeToString(this->customerType), distance, maxOrders);
    cloned->error(this->getErrorMsg());
    if (this->getStatus() == ActionStatus::COMPLETED)
        cloned->complete();

    return cloned;

    // TODO all clones are memory leaks until proven otherwise :(
}

string AddCustomer::toString() const
{
    string name = "AddCustomer";
    string args = customerName + " " + CustomerTypeToString(customerType) + " " + std::to_string(distance) + " " + std::to_string(maxOrders);
    string s = actionStatusString();
    return name + " " + args + " " + s;
}

PrintOrderStatus::PrintOrderStatus(int id) : BaseAction(), orderId(id)
{
}

void PrintOrderStatus::act(WareHouse &wareHouse)
{
    Order &order = wareHouse.getOrder(orderId);
    if (order.getId() == -1)
    {
        error("Order doesn’t exist");
        return;
    }

    std::cout << order.toString() << std::endl;
    complete();
}

PrintOrderStatus *PrintOrderStatus::clone() const
{
    PrintOrderStatus *cloned = new PrintOrderStatus(orderId);
    cloned->error(this->getErrorMsg());
    if (this->getStatus() == ActionStatus::COMPLETED)
        cloned->complete();

    return cloned;
}

string PrintOrderStatus::toString() const
{
    string name = "PrintOrderStatus";
    string args = std::to_string(orderId);
    string s = actionStatusString();
    return name + " " + args + " " + s;
}

PrintCustomerStatus::PrintCustomerStatus(int customerId) : BaseAction(), customerId(customerId)
{
}

void PrintCustomerStatus::act(WareHouse &wareHouse)
{
    Customer &customer = wareHouse.getCustomer(customerId);
    if (customer.getId() == -1)
    {
        error("Customer doesn’t exist");
        return;
    }
    string str = "CustomerID: " + std::to_string(customerId) + "\n";
    vector<int> orders = customer.getOrdersIds();
    for (int i : orders)
    {
        str += "OrderID: " + std::to_string(i) + "\n";
        str += wareHouse.getOrder(i).statusToString() + "\n";
    }
    str += "numOrdersLeft: " + std::to_string(customer.getMaxOrders() - customer.getNumOrders());

    std::cout << str << std::endl;
    complete();
}

PrintCustomerStatus *PrintCustomerStatus::clone() const
{
    PrintCustomerStatus *cloned = new PrintCustomerStatus(customerId);
    cloned->error(this->getErrorMsg());
    if (this->getStatus() == ActionStatus::COMPLETED)
        cloned->complete();

    return cloned;
}

string PrintCustomerStatus::toString() const
{
    string name = "PrintCustomerStatus";
    string args = std::to_string(customerId);
    string s = actionStatusString();
    return name + " " + args + " " + s;
}

PrintVolunteerStatus::PrintVolunteerStatus(int id) : BaseAction(), volunteerId(id) {}

void PrintVolunteerStatus::act(WareHouse &wareHouse)
{
    Volunteer &vol = wareHouse.getVolunteer(volunteerId);
    if (vol.getId() == -1)
    {
        error("Volunteer doesn’t exist");
        return;
    }
    std::cout << vol.toString() << std::endl;
    complete();
}

PrintVolunteerStatus *PrintVolunteerStatus::clone() const
{
    PrintVolunteerStatus *cloned = new PrintVolunteerStatus(volunteerId);
    cloned->error(this->getErrorMsg());
    if (this->getStatus() == ActionStatus::COMPLETED)
        cloned->complete();

    return cloned;
}

string PrintVolunteerStatus::toString() const
{
    string name = "PrintVolunteerStatus";
    string args = std::to_string(volunteerId);
    string s = actionStatusString();
    return name + " " + args + " " + s;
}

PrintActionsLog::PrintActionsLog() : BaseAction() {}

void PrintActionsLog::act(WareHouse &wareHouse)
{
    const std::vector<BaseAction *> &actions = wareHouse.getActions();
    for (BaseAction *action : actions)
        std::cout << action->toString() << std::endl;

    complete();
}

PrintActionsLog *PrintActionsLog::clone() const
{
    PrintActionsLog *cloned = new PrintActionsLog();
    cloned->error(this->getErrorMsg());
    if (this->getStatus() == ActionStatus::COMPLETED)
        cloned->complete();

    return cloned;
}

string PrintActionsLog::toString() const
{
    string name = "PrintActionsLog";
    string s = actionStatusString();
    return name + " " + s;
}

Close::Close() : BaseAction() {}

void Close::act(WareHouse &wareHouse)
{
    int max = wareHouse.getNewOrderId();
    for (int i = 0; i < max; i++)
        std::cout << wareHouse.getOrder(i).toStringCompact() << std::endl;
    // TODO is this the string we want?
    wareHouse.close();
    complete();
}

Close *Close::clone() const
{
    Close *cloned = new Close();
    cloned->error(this->getErrorMsg());
    if (this->getStatus() == ActionStatus::COMPLETED)
        cloned->complete();

    return cloned;
}

string Close::toString() const
{
    string name = "Close";
    string s = actionStatusString();
    return name + " " + s;
}

BackupWareHouse::BackupWareHouse() : BaseAction() {}

void BackupWareHouse::act(WareHouse &wareHouse)
{
    extern WareHouse *backup;
    if (backup != nullptr)
        delete backup;

    backup = wareHouse.clone();
    complete();
}

BackupWareHouse *BackupWareHouse::clone() const

{
    BackupWareHouse *cloned = new BackupWareHouse();
    cloned->error(this->getErrorMsg());
    if (this->getStatus() == ActionStatus::COMPLETED)
        cloned->complete();

    return cloned;
}

string BackupWareHouse::toString() const
{
    string name = "BackupWareHouse";
    string s = actionStatusString();
    return name + " " + s;
}

RestoreWareHouse::RestoreWareHouse() : BaseAction() {}

void RestoreWareHouse::act(WareHouse &wareHouse)
{
    // TODO
}

RestoreWareHouse *RestoreWareHouse::clone() const
{
    RestoreWareHouse *cloned = new RestoreWareHouse();
    cloned->error(this->getErrorMsg());
    if (this->getStatus() == ActionStatus::COMPLETED)
        cloned->complete();

    return cloned;
}

string RestoreWareHouse::toString() const
{
    string name = "RestoreWareHouse";
    string s = actionStatusString();
    return name + " " + s;
}
