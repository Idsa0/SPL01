#include "Action.h"
#include <iostream>

BaseAction::BaseAction() : errorMsg(), status(ActionStatus::ERROR) {}

ActionStatus BaseAction::getStatus() const
{
    return status;
}

void BaseAction::complete()
{
    status = ActionStatus::COMPLETED;
    // TODO do we always want to do this?
}

void BaseAction::error(string errorMsg)
{
    this->errorMsg = errorMsg;
    status = ActionStatus::ERROR;
    // TODO should we change status?
    // TODO print error msg here?
}

string BaseAction::getErrorMsg() const
{
    return errorMsg;
}

SimulateStep::SimulateStep(int numOfSteps) : BaseAction(), numOfSteps(numOfSteps) {}

void SimulateStep::act(WareHouse &wareHouse)
{
    // TODO
}

std::string SimulateStep::toString() const
{
    return "SimulateStep";
    // TODO
}

SimulateStep *SimulateStep::clone() const
{
    SimulateStep *cloned = new SimulateStep(numOfSteps);
    cloned->error(this->getErrorMsg());
    if (this->getStatus() == ActionStatus::COMPLETED)
        cloned->complete();

    return cloned;

    // TODO clone methods are highly likely to cause bugs, putting this todo only here to reduce todos
}

AddOrder::AddOrder(int id) : BaseAction(), customerId(id) {}

void AddOrder::act(WareHouse &wareHouse)
{
    Customer &customer = wareHouse.getCustomer(customerId);

    Order *order = new Order(wareHouse.getNewOrderId(), customerId, customer.getCustomerDistance());
    wareHouse.addOrder(order);
}

string AddOrder::toString() const
{
    return "AddOrder";
    // TODO
}

AddOrder *AddOrder::clone() const
{
    AddOrder *cloned = new AddOrder(customerId);
    cloned->error(this->getErrorMsg());
    if (this->getStatus() == ActionStatus::COMPLETED)
        cloned->complete();

    return cloned;
}

CustomerType stringToCustomerType(const string &ct)
{
    if (ct == "soldier")
        return CustomerType::Soldier;

    return CustomerType::Civilian;

    // TODO "this never results in an error..."
}

string CustomerTypeToString(CustomerType ct)
{
    if (ct == CustomerType::Civilian)
        return "civilian";

    return "soldier";

    // TODO these two functions are terrible
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
    return "AddCustomer";
    // TODO
}

PrintOrderStatus::PrintOrderStatus(int id) : BaseAction(), orderId(id)
{
}

void PrintOrderStatus::act(WareHouse &wareHouse)
{
    // TODO
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
    // TODO
}

PrintCustomerStatus::PrintCustomerStatus(int customerId) : BaseAction(), customerId(customerId)
{
}

void PrintCustomerStatus::act(WareHouse &wareHouse)
{
    // TODO
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
    // TODO
}

PrintVolunteerStatus::PrintVolunteerStatus(int id) : BaseAction(), volunteerId(id) {}

void PrintVolunteerStatus::act(WareHouse &wareHouse)
{
    // TODO
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
    // TODO
}

PrintActionsLog::PrintActionsLog() : BaseAction() {}

void PrintActionsLog::act(WareHouse &wareHouse)
{
    // TODO
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
    // TODO
}

Close::Close() : BaseAction() {}

void Close::act(WareHouse &wareHouse)
{
    int max = wareHouse.getNewOrderId();
    for (int i = 0; i < max; i++)
        std::cout << wareHouse.getOrder(i).toString() << std::endl;
    // TODO is this the string we want?
    wareHouse.close();
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
    // TODO
}

BackupWareHouse::BackupWareHouse() : BaseAction() {}

void BackupWareHouse::act(WareHouse &wareHouse)
{
    extern WareHouse *backup;
    if (backup != nullptr)
        delete backup;

    backup = wareHouse.clone();
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
    // TODO
}

RestoreWareHouse::RestoreWareHouse() : BaseAction()
{
}

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
    // TODO
}