#include "Volunteer.h"
#include <iostream>

Volunteer::Volunteer(int id, const string &name) : type(VolunteerType::COLLECTOR), completedOrderId(NO_ORDER), activeOrderId(NO_ORDER), id(id), name(name) {}

int Volunteer::getId() const
{
    return id;
}

const string &Volunteer::getName() const
{
    return name;
}

int Volunteer::getActiveOrderId() const
{
    return activeOrderId;
}

int Volunteer::getCompletedOrderId() const
{
    return completedOrderId;
}

bool Volunteer::isBusy() const
{
    return activeOrderId != NO_ORDER;
}

void Volunteer::setCompletedOrderId(int id)
{
    completedOrderId = id;
}

void Volunteer::finishOrder()
{
    completedOrderId = activeOrderId;
    activeOrderId = NO_ORDER;
}

CollectorVolunteer::CollectorVolunteer(int id, const string &name, int coolDown) : Volunteer(id, name), coolDown(coolDown), timeLeft(0)
{
    type = VolunteerType::COLLECTOR;
}

CollectorVolunteer *CollectorVolunteer::clone() const
{
    return new CollectorVolunteer(*this);
}

void CollectorVolunteer::step()
{
    if (isBusy() && decreaseCoolDown())
        finishOrder();
}

int CollectorVolunteer::getCoolDown() const
{
    return coolDown;
}

int CollectorVolunteer::getTimeLeft() const
{
    return timeLeft;
}

bool CollectorVolunteer::decreaseCoolDown()
{
    if (timeLeft != 0)
        --timeLeft;

    return timeLeft == 0;
}

bool CollectorVolunteer::hasOrdersLeft() const
{
    return true;
}

bool CollectorVolunteer::canTakeOrder(const Order &order) const
{
    return !isBusy() && timeLeft == 0;
}

void CollectorVolunteer::acceptOrder(const Order &order)
{
    if (!CollectorVolunteer::canTakeOrder(order))
        throw("Cannot take order");
    activeOrderId = order.getId();
    timeLeft = coolDown;
}

const string bool_cast(const bool b)
{
    return b ? "true" : "false";
}

string CollectorVolunteer::toString() const
{
    string ordstr = (isBusy()) ? std::to_string(activeOrderId) : "None";
    string timestr = (isBusy()) ? std::to_string(timeLeft) : "None";
    string str = "VolunteerID: " + std::to_string(getId()) + "\n" +
                 "IsBusy: " + bool_cast(isBusy()) + "\n" +
                 "OrderID: " + ordstr + "\n" +
                 "TimeLeft: " + timestr + "\n" +
                 "OrdersLeft: " + "No Limit";

    return str;
}

void CollectorVolunteer::setTimeLeft(int time)
{
    timeLeft = time;
}

LimitedCollectorVolunteer::LimitedCollectorVolunteer(int id, const string &name, int coolDown, int maxOrders) : CollectorVolunteer(id, name, coolDown), maxOrders(maxOrders), ordersLeft(maxOrders) {}

LimitedCollectorVolunteer *LimitedCollectorVolunteer::clone() const
{
    return new LimitedCollectorVolunteer(*this);
}

bool LimitedCollectorVolunteer::hasOrdersLeft() const
{
    return ordersLeft > 0;
}
bool LimitedCollectorVolunteer::canTakeOrder(const Order &order) const
{
    return hasOrdersLeft() && CollectorVolunteer::canTakeOrder(order);
}
void LimitedCollectorVolunteer::acceptOrder(const Order &order)
{
    if (!canTakeOrder(order))
        throw("Cannot take order");

    --ordersLeft;
    CollectorVolunteer::acceptOrder(order);
}
int LimitedCollectorVolunteer::getMaxOrders() const
{
    return maxOrders;
}
int LimitedCollectorVolunteer::getNumOrdersLeft() const
{
    return ordersLeft;
}

void LimitedCollectorVolunteer::step()
{
    CollectorVolunteer::step();
}

string LimitedCollectorVolunteer::toString() const
{
    string ordstr = (isBusy()) ? std::to_string(activeOrderId) : "None";
    string timestr = (isBusy()) ? std::to_string(getTimeLeft()) : "None";
    string str = "VolunteerID: " + std::to_string(getId()) + "\n" +
                 "IsBusy: " + bool_cast(isBusy()) + "\n" +
                 "OrderID: " + ordstr + "\n" +
                 "TimeLeft: " + timestr + "\n" +
                 "OrdersLeft: " + std::to_string(ordersLeft);

    return str;
}

DriverVolunteer::DriverVolunteer(int id, const string &name, int maxDistance, int distancePerStep) : Volunteer(id, name), maxDistance(maxDistance), distancePerStep(distancePerStep), distanceLeft(0)
{
    type = VolunteerType::DRIVER;
}

DriverVolunteer *DriverVolunteer::clone() const
{
    return new DriverVolunteer(*this);
}

int DriverVolunteer::getDistanceLeft() const
{
    return distanceLeft;
}

int DriverVolunteer::getMaxDistance() const
{
    return maxDistance;
}

int DriverVolunteer::getDistancePerStep() const
{
    return distancePerStep;
}

bool DriverVolunteer::decreaseDistanceLeft()
{
    if (distanceLeft > 0)
        distanceLeft -= distancePerStep;

    if (distanceLeft < 0)
        distanceLeft = 0;

    return distanceLeft <= 0;
} // Decrease distanceLeft by distancePerStep,return true if distanceLeft<=0,false otherwise

bool DriverVolunteer::hasOrdersLeft() const
{
    return true;
}

bool DriverVolunteer::canTakeOrder(const Order &order) const
{
    return !isBusy() && order.getDistance() <= maxDistance && distanceLeft <= 0;
} // Signal if the volunteer is not busy and the order is within the maxDistance

void DriverVolunteer::acceptOrder(const Order &order)
{
    if (!DriverVolunteer::canTakeOrder(order))
        throw("Cannot take order");
    activeOrderId = order.getId();
    distanceLeft = order.getDistance();
} // Assign distanceLeft to order's distance

void DriverVolunteer::step()
{
    if (isBusy() && decreaseDistanceLeft())
        finishOrder();
} // Decrease distanceLeft by distancePerStep

string DriverVolunteer::toString() const
{
    string ordstr = (isBusy()) ? std::to_string(activeOrderId) : "None";
    string dststr = (isBusy()) ? std::to_string(distanceLeft) : "None";
    string str = "VolunteerID: " + std::to_string(getId()) + "\n" +
                 "IsBusy: " + bool_cast(isBusy()) + "\n" +
                 "OrderID: " + ordstr + "\n" +
                 "TimeLeft: " + dststr + "\n" +
                 "OrdersLeft: " + "No Limit";

    return str;
}

void DriverVolunteer::setDistanceLeft(int distance)
{
    distanceLeft = distance;
}

LimitedDriverVolunteer::LimitedDriverVolunteer(int id, const string &name, int maxDistance, int distancePerStep, int maxOrders) : DriverVolunteer(id, name, maxDistance, distancePerStep), maxOrders(maxOrders), ordersLeft(maxOrders) {}

LimitedDriverVolunteer *LimitedDriverVolunteer::clone() const
{

    return new LimitedDriverVolunteer(*this);
}

void LimitedDriverVolunteer::step()
{
    DriverVolunteer::step();
}
int LimitedDriverVolunteer::getMaxOrders() const
{
    return maxOrders;
}

int LimitedDriverVolunteer::getNumOrdersLeft() const
{
    return ordersLeft;
}

bool LimitedDriverVolunteer::hasOrdersLeft() const
{
    return ordersLeft > 0;
}

bool LimitedDriverVolunteer::canTakeOrder(const Order &order) const
{
    return DriverVolunteer::canTakeOrder(order) && hasOrdersLeft();
}

void LimitedDriverVolunteer::acceptOrder(const Order &order)
{
    if (!canTakeOrder(order))
        throw("Cannot take order");

    DriverVolunteer::acceptOrder(order);
    --ordersLeft;
}

string LimitedDriverVolunteer::toString() const
{
    string ordstr = (isBusy()) ? std::to_string(activeOrderId) : "None";
    string dststr = (isBusy()) ? std::to_string(getDistanceLeft()) : "None";
    string str = "VolunteerID: " + std::to_string(getId()) + "\n" +
                 "IsBusy: " + bool_cast(isBusy()) + "\n" +
                 "OrderID: " + ordstr + "\n" +
                 "TimeLeft: " + dststr + "\n" +
                 "OrdersLeft: " + std::to_string(ordersLeft);

    return str;
}