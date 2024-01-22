#include "WareHouse.h"
#include "Volunteer.h"
#include <iostream>

using namespace std;

WareHouse::WareHouse(const string &configFilePath) // TODO config parser
{
    isOpen = false; // TODO check reqs
    customerCounter = 0;
    volunteerCounter = 0;
    orderCounter = 0;
}

void WareHouse::start()
{
    // TODO should we print here?
    isOpen = true;
    cout << "Warehouse is open!" << endl;

    while (isOpen)
    {
        // LOOP (endless one for now)
    }
}

void WareHouse::addOrder(Order *order)
{
    pendingOrders.push_back(order);
}

void WareHouse::addAction(BaseAction *action)
{
    actionsLog.push_back(action);
}

void WareHouse::addCustomer(Customer *customer)
{
    customers.push_back(customer);
}

Customer &WareHouse::getCustomer(int customerId) const
{
    for (Customer *customer : customers) // TODO how does this work memory wise
        if (customer->getId() == customerId)
            return *customer;

    // TODO what if the customer doesnt exist?

    // TODO check out this hack to return null values: return *(Customer *)nullptr;
    return nullCustomer;
}

Volunteer &WareHouse::getVolunteer(int volunteerId) const
{
    for (Volunteer *volunteer : volunteers) // TODO how does this work memory wise
        if (volunteer->getId() == volunteerId)
            return *volunteer;

    // TODO what if the customer doesnt exist?

    return nullCollector;
}

Order &WareHouse::getOrder(int orderId) const
{
    for (Order *order : pendingOrders) // TODO how does this work memory wise
        if (order->getId() == orderId)
            return *order;

    for (Order *order : inProcessOrders) // TODO how does this work memory wise
        if (order->getId() == orderId)
            return *order;

    for (Order *order : completedOrders) // TODO how does this work memory wise
        if (order->getId() == orderId)
            return *order;

    // TODO what if the customer doesnt exist?

    return nullOrder;
}

const vector<BaseAction *> &WareHouse::getActions() const
{
    return actionsLog;
}

void WareHouse::close()
{
    isOpen = false;
}

void WareHouse::open()
{
    isOpen = true;
}

int WareHouse::getNewCustomerId()
{
    return customerCounter++;
}

int WareHouse::getNewVolunteerId()
{
    return volunteerCounter++;
}

int WareHouse::getNewOrderId()
{
    return orderCounter++;
}

WareHouse::~WareHouse()
{
    for (BaseAction *action : actionsLog)
        delete action;

    for (Volunteer *volunteer : volunteers)
        delete volunteer;

    for (Order *order : pendingOrders)
        delete order;

    for (Order *order : inProcessOrders)
        delete order;

    for (Order *order : completedOrders)
        delete order;

    for (Customer *customer : customers)
        delete customer;
}

WareHouse *WareHouse::clone() const
{
    WareHouse *cloned = new WareHouse();

    cloned->isOpen = this->isOpen;
    for (BaseAction *action : actionsLog)
        cloned->actionsLog.push_back(action);
    for (Volunteer *volunteer : volunteers)
        cloned->volunteers.push_back(volunteer);
    for (Order *order : pendingOrders)
        cloned->pendingOrders.push_back(order);
    for (Order *order : inProcessOrders)
        cloned->inProcessOrders.push_back(order);
    for (Order *order : completedOrders)
        cloned->completedOrders.push_back(order);
    for (Customer *customer : customers)
        cloned->customers.push_back(customer);

    cloned->customerCounter = this->customerCounter;
    cloned->orderCounter = this->orderCounter;
    cloned->volunteerCounter = this->volunteerCounter;

    return cloned;
}