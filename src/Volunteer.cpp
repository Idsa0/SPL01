#include "Volunteer.h"

Volunteer::Volunteer(int id, const string &name) : id(id), name(name), completedOrderId(NO_ORDER), activeOrderId(NO_ORDER) {}

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

bool Volunteer::isBusy() const // Signal whether the volunteer is currently processing an order
{
    return activeOrderId != NO_ORDER;
    // TODO this is sketchy
}

CollectorVolunteer::CollectorVolunteer(int id, const string &name, int coolDown) : Volunteer(id, name), coolDown(coolDown), timeLeft(0) {}
// TODO initialized timeLeft to 0 is it ok?

CollectorVolunteer *CollectorVolunteer::clone() const
{
    // TODO should we make a cloned string?
    CollectorVolunteer *cloned = new CollectorVolunteer(getId(), getName(), coolDown);
    cloned->activeOrderId = this->activeOrderId;
    cloned->completedOrderId = this->completedOrderId;
    cloned->timeLeft = this->timeLeft;

    return cloned;
}

void CollectorVolunteer::step()
{
    // TODO
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
} // Decrease timeLeft by 1,return true if timeLeft=0,false otherwise

bool CollectorVolunteer::hasOrdersLeft() const
{
    return true;
}

bool CollectorVolunteer::canTakeOrder(const Order &order) const
{
    return !isBusy() && timeLeft == 0;
    // TODO timeLeft is a sanity check
}

void CollectorVolunteer::acceptOrder(const Order &order)
{
    if (!canTakeOrder(order))
        throw("Cannot take order");
    activeOrderId = order.getId();
    timeLeft = coolDown;
}

string CollectorVolunteer::toString() const
{
    // TODO
    return "CollectorVolunteer";
}

LimitedCollectorVolunteer::LimitedCollectorVolunteer(int id, const string &name, int coolDown, int maxOrders) : CollectorVolunteer(id, name, coolDown), maxOrders(maxOrders), ordersLeft(maxOrders) {}
LimitedCollectorVolunteer *LimitedCollectorVolunteer::clone() const
{
    LimitedCollectorVolunteer *cloned = new LimitedCollectorVolunteer(getId(), getName(), getCoolDown(), maxOrders);
    // TODO timeLeft copy (no way to get)
    cloned->ordersLeft = this->ordersLeft;
    cloned->completedOrderId = this->getCompletedOrderId();
    cloned->activeOrderId = this->getActiveOrderId();

    return cloned;
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

    ordersLeft--;
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
string LimitedCollectorVolunteer::toString() const
{
    return "LimitedCollectorVolunter";
    // TODO
}

DriverVolunteer::DriverVolunteer(int id, const string &name, int maxDistance, int distancePerStep) : Volunteer(id, name), maxDistance(maxDistance), distancePerStep(distancePerStep), distanceLeft(0) {}
// TODO distanceLeft initialized to 0

DriverVolunteer *DriverVolunteer::clone() const
{
    DriverVolunteer * cloned = new DriverVolunteer(getId(), getName(), maxDistance, distancePerStep);
    cloned->distanceLeft = this->distanceLeft;
    cloned->activeOrderId = this->activeOrderId;
    cloned->completedOrderId = this->completedOrderId;

    return cloned;
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
    if (!canTakeOrder(order))
        throw("Cannot take order");
    activeOrderId = order.getId();
    distanceLeft = order.getDistance();
} // Assign distanceLeft to order's distance

void DriverVolunteer::step()
{
    // TODO TODO TTTTTOOOO
} // Decrease distanceLeft by distancePerStep

string DriverVolunteer::toString() const
{
    // TODO
    return "DriverVolunteer";
}

LimitedDriverVolunteer::LimitedDriverVolunteer(int id, const string &name, int maxDistance, int distancePerStep, int maxOrders) : DriverVolunteer(id, name, maxDistance, distancePerStep), maxOrders(maxOrders), ordersLeft(maxOrders) {}

LimitedDriverVolunteer *LimitedDriverVolunteer::clone() const
{
    LimitedDriverVolunteer * cloned = new LimitedDriverVolunteer(getId(), getName(), getMaxDistance(), getDistancePerStep(), maxOrders);
    cloned->ordersLeft = this->ordersLeft;
    cloned->completedOrderId = this->getCompletedOrderId();
    cloned->activeOrderId = this->getActiveOrderId();
    // TODO cannot set distanceLeft no access permission

    return cloned;
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
} // Signal if the volunteer is not busy, the order is within the maxDistance.

void LimitedDriverVolunteer::acceptOrder(const Order &order)
{
    if (!canTakeOrder(order))
        throw ("Cannot take order");
    
    DriverVolunteer::acceptOrder(order);
    ordersLeft--;

    
} // Assign distanceLeft to order's distance and decrease ordersLeft

string LimitedDriverVolunteer::toString() const
{
    return "LimitedDriverVolunteer";
    // TODO
}