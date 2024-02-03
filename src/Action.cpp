#include "Action.h"
#include <iostream>
#include <algorithm>

BaseAction::BaseAction() : isNull(false), errorMsg(), status(ActionStatus::ERROR) {}

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
    setErrorMsg(errorMsg);
    status = ActionStatus::ERROR;
    std::cout << errorMsg << std::endl;
}

void BaseAction::setErrorMsg(string errorMsg)
{
    this->errorMsg = errorMsg;
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
    wareHouse.addAction(this);

    if (numOfSteps <= 0)
    {
        error("Number of steps must be positive!");
        return;
    }

    for (int i = 0; i < numOfSteps; i++)
        wareHouse.simulateStep();

    complete();
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
    return new SimulateStep(*this);
}

AddOrder::AddOrder(int id) : BaseAction(), customerId(id) {}

void AddOrder::act(WareHouse &wareHouse)
{
    wareHouse.addAction(this);

    if (customerId < 0)
    {
        error("Customer ID must be positive");
        return;
    }

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
        error("FATAL @ AddOrder::Act");
        return;
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
    return new AddOrder(*this);
}

// helper function I -> string to CustomerType enum
CustomerType stringToCustomerType(const string &ct)
{
    if (ct == "soldier")
        return CustomerType::Soldier;

    else if (ct == "civilian")
        return CustomerType::Civilian;

    return CustomerType::Other;
}

// helper function II -> CustomerType enum to string
string CustomerTypeToString(CustomerType ct)
{
    if (ct == CustomerType::Civilian)
        return "civilian";

    else if (ct == CustomerType::Soldier)
        return "soldier";

    return "other";
}

AddCustomer::AddCustomer(const string &customerName, const string &customerType, int distance, int maxOrders) : BaseAction(), customerName(customerName), customerType(stringToCustomerType(customerType)), distance(distance), maxOrders(maxOrders) {}

void AddCustomer::act(WareHouse &wareHouse)
{
    wareHouse.addAction(this);

    if (customerType == CustomerType::Other)
    {
        error("Invalid customer type");
        return;
    }

    if (distance <= 0)
    {
        error("Distance must be positive");
        return;
    }

    if (maxOrders <= 0)
    {
        error("Max orders must be positive");
        return;
    }

    Customer *customer;
    switch (customerType)
    {
    case CustomerType::Civilian:
        customer = new CivilianCustomer(wareHouse.getNewCustomerId(), customerName, distance, maxOrders);
        break;

    case CustomerType::Soldier:
        customer = new SoldierCustomer(wareHouse.getNewCustomerId(), customerName, distance, maxOrders);
        break;

    default:
        // This should never happen
        return;
    }

    wareHouse.addCustomer(customer);
    complete();
}

AddCustomer *AddCustomer::clone() const
{
    return new AddCustomer(*this);
}

string AddCustomer::toString() const
{
    string name = "AddCustomer";
    string args = customerName + " " + CustomerTypeToString(customerType) + " " + std::to_string(distance) + " " + std::to_string(maxOrders);
    string s = actionStatusString();
    return name + " " + args + " " + s;
}

PrintOrderStatus::PrintOrderStatus(int id) : BaseAction(), orderId(id) {}

void PrintOrderStatus::act(WareHouse &wareHouse)
{
    wareHouse.addAction(this);

    if (orderId < 0)
    {
        error("Order ID must be non-negative");
        return;
    }

    Order &order = wareHouse.getOrder(orderId);
    if (order.getId() == -1)
    {
        error("Order doesn't exist");
        return;
    }

    std::cout << order.toString() << std::endl;
    complete();
}

PrintOrderStatus *PrintOrderStatus::clone() const
{
    return new PrintOrderStatus(*this);
}

string PrintOrderStatus::toString() const
{
    string name = "PrintOrderStatus";
    string args = std::to_string(orderId);
    string s = actionStatusString();
    return name + " " + args + " " + s;
}

PrintCustomerStatus::PrintCustomerStatus(int customerId) : BaseAction(), customerId(customerId) {}

void PrintCustomerStatus::act(WareHouse &wareHouse)
{
    wareHouse.addAction(this);

    if (customerId < 0)
    {
        error("Customer ID must be non-negative");
        return;
    }

    Customer &customer = wareHouse.getCustomer(customerId);
    if (customer.getId() == -1)
    {
        error("Customer doesn't exist");
        return;
    }
    string str = "CustomerID: " + std::to_string(customerId) + "\n";
    vector<int> orders = customer.getOrdersIds();
    for (int i : orders)
    {
        str += "OrderID: " + std::to_string(i) + "\n";
        str += "OrderStatus: " + wareHouse.getOrder(i).statusToString() + "\n";
    }
    str += "numOrdersLeft: " + std::to_string(customer.getMaxOrders() - customer.getNumOrders());

    std::cout << str << std::endl;
    complete();
}

PrintCustomerStatus *PrintCustomerStatus::clone() const
{
    return new PrintCustomerStatus(*this);
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
    wareHouse.addAction(this);

    if (volunteerId < 0)
    {
        error("Volunteer ID must be non-negative");
        return;
    }

    Volunteer &vol = wareHouse.getVolunteer(volunteerId);
    if (vol.getId() == -1)
    {
        error("Volunteer doesn't exist");
        return;
    }
    std::cout << vol.toString() << std::endl;
    complete();
}

PrintVolunteerStatus *PrintVolunteerStatus::clone() const
{
    return new PrintVolunteerStatus(*this);
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
    wareHouse.addAction(this);
}

PrintActionsLog *PrintActionsLog::clone() const
{
    return new PrintActionsLog(*this);
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
    wareHouse.addAction(this);
    int max = wareHouse.getNewOrderId();
    for (int i = 0; i < max; i++)
        std::cout << wareHouse.getOrder(i).toStringCompact() << std::endl;
    
    wareHouse.close();
    complete();
}

Close *Close::clone() const
{
    return new Close(*this);
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
    wareHouse.addAction(this);
}

BackupWareHouse *BackupWareHouse::clone() const

{
    return new BackupWareHouse(*this);
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
    extern WareHouse *backup;

    if (backup == nullptr)
    {
        error("No Backup Available");
        wareHouse.addAction(this);
        return;
    }
    wareHouse = *backup;

    wareHouse.addAction(this);
    complete();
}

RestoreWareHouse *RestoreWareHouse::clone() const
{
    return new RestoreWareHouse(*this);
}

string RestoreWareHouse::toString() const
{
    string name = "RestoreWareHouse";
    string s = actionStatusString();
    return name + " " + s;
}

NullAction::NullAction() : BaseAction()
{
    isNull = true;
}

void NullAction::act(WareHouse &wareHouse)
{
    wareHouse.addAction(this);
    error("FATAL: NULL ACTION HAS BEEN ACTED ON!");
    throw;
}

NullAction *NullAction::clone() const
{
    return new NullAction(*this);
}

string NullAction::toString() const
{
    return "";
}
