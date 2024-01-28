#include "Order.h"

Order::Order(int id, int customerId, int distance) : id(id), customerId(customerId),
                                                     distance(distance), status(OrderStatus::PENDING), collectorId(NO_VOLUNTEER), driverId(NO_VOLUNTEER) {}

int Order::getId() const
{
    return id;
}

int Order::getCustomerId() const
{
    return customerId;
}

void Order::setStatus(OrderStatus status)
{
    this->status = status;
}

void Order::setCollectorId(int collectorId)
{
    this->collectorId = collectorId;
}

void Order::setDriverId(int driverId)
{
    this->driverId = driverId;
}

int Order::getCollectorId() const
{
    return collectorId;
}

int Order::getDriverId() const
{
    return driverId;
}

OrderStatus Order::getStatus() const
{
    return status;
}

const string Order::statusToString() const
{
    switch (status)
    {
    case OrderStatus::COLLECTING:
        return "COLLECTING";

    case OrderStatus::PENDING:
        return "PENDING";

    case OrderStatus::DELIVERING:
        return "DELIVERING";

    case OrderStatus::COMPLETED:
        return "COMPLETED";
    }
}
const string Order::toString() const
{
    string col = (collectorId == -1) ? "None" : std::to_string(collectorId);
    string drv = (driverId == -1) ? "None" : std::to_string(driverId);
    string str = "OrderId: " + std::to_string(id) + "\n" +
                 "OrderStatus: " + statusToString() + "\n" +
                 "CustomerId: " + std::to_string(customerId) + "\n" +
                 "CollectorId: " + col + "\n" +
                 "DriverId: " + drv + "\n";

    return str;
}

const string Order::toStringCompact() const
{
    string str = "OrderId: " + std::to_string(id) + ", " +
                 "CustomerId: " + std::to_string(customerId) + ", " +
                 "OrderStatus: " + statusToString();

    return str;
}

const int Order::getDistance() const
{
    return distance;
}

Order *Order::clone() const
{
    Order *order = new Order(id, customerId, distance);
    order->setStatus(this->status);
    order->collectorId = this->collectorId;
    order->driverId = this->driverId;
    return order;
}