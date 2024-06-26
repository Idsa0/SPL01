#include "Customer.h"

Customer::Customer(int id, const string &name, int locationDistance, int maxOrders) : id(id), name(name), locationDistance(locationDistance), maxOrders(maxOrders), ordersId(vector<int>()) {}

const string &Customer::getName() const
{
    return name;
}

int Customer::getId() const
{
    return id;
}

int Customer::getCustomerDistance() const
{
    return locationDistance;
}

int Customer::getMaxOrders() const
{
    return maxOrders;
} // Returns maxOrders

int Customer::getNumOrders() const
{
    return ordersId.size();
} // Returns num of orders the customer has made so far

bool Customer::canMakeOrder() const
{
    return this->getNumOrders() < maxOrders;
} // Returns true if the customer didn't reach max orders

const vector<int> &Customer::getOrdersIds() const
{
    return ordersId;
}

int Customer::addOrder(int orderId)
{
    if (!canMakeOrder())
    {
        return -1;
    }
    ordersId.push_back(orderId);
    return 0;
}

SoldierCustomer::SoldierCustomer(int id, const string &name, int locationDistance, int maxOrders) : Customer(id, name, locationDistance, maxOrders) {}

SoldierCustomer *SoldierCustomer::clone() const
{
    return new SoldierCustomer(*this);
}

CivilianCustomer::CivilianCustomer(int id, const string &name, int locationDistance, int maxOrders) : Customer(id, name, locationDistance, maxOrders) {}

CivilianCustomer *CivilianCustomer::clone() const
{
    return new CivilianCustomer(*this);
}

string CivilianCustomer::toString() const
{
    return "CivilianCustomer";
}

string SoldierCustomer::toString() const
{
    return "SoldierCustomer";
}