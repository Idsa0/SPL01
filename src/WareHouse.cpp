#include "WareHouse.h"
#include "Volunteer.h"
#include <iostream>
#include "Action.h"
#include "InputHandler.h"

WareHouse::WareHouse(const string &configFilePath) : WareHouse()
{
    buildFromConfigurationFile(configFilePath);
}

WareHouse::WareHouse() : isOpen{false}, customerCounter{0}, volunteerCounter{0}, orderCounter{0} {}

void WareHouse::start()
{
    isOpen = true;
    std::cout << "Warehouse is open!" << std::endl;

    while (isOpen)
    {
        string inputString;
        std::getline(std::cin, inputString);
        BaseAction *action = InputHandler::parse(inputString);
        if (!action->isNull)
        {
            action->act(*this);
            addAction(action);
        }
    }
}

void WareHouse::buildFromConfigurationFile(const std::string &path)
{
    if (path == "adminPass") // TODO: quick method to bypass config. to remove.
        return;
    std::ifstream config(path);
    if (!config.is_open())
        throw;

    std::string line;
    while (std::getline(config, line))
    {
        std::vector<std::string> args{};

        std::string word;
        for (char c : line)
            if (c != ' ')
                word += c;
            else if (!word.empty())
            {
                args.push_back(word);
                word.clear();
            }
        if (!word.empty())
            args.push_back(word);
        if (args[0] == "customer")
        {
            if (args.size() != 5)
                throw;

            AddCustomer action = AddCustomer(args[1], args[2], stoi(args[3]), stoi(args[4]));

            action.act(*this); // using the action to reuse code. This does not get added to the warehouse actions log.
        }
        else if (args[0] == "volunteer")
        {
            if (args.size() <= 3)
                throw;

            if (args[3] == "collector")
            {
                if (args.size() != 4)
                    throw;
                CollectorVolunteer *colvol = new CollectorVolunteer(getNewVolunteerId(), args[2], std::stoi(args[4]));
                addVolunteer(colvol);
            }
            else if (args[3] == "limited_collector")
            {
                if (args.size() != 5)
                    throw;
                LimitedCollectorVolunteer *limcolvol = new LimitedCollectorVolunteer(getNewVolunteerId(), args[2], std::stoi(args[4]), std::stoi(args[5]));
                addVolunteer(limcolvol);
            }
            else if (args[3] == "driver")
            {
                if (args.size() != 5)
                    throw;
                DriverVolunteer *drivol = new DriverVolunteer(getNewVolunteerId(), args[2], std::stoi(args[4]), std::stoi(args[5]));
                addVolunteer(drivol);
            }
            else if (args[3] == "limited_driver")
            {
                if (args.size() != 5)
                    throw;
                LimitedDriverVolunteer *drivol = new LimitedDriverVolunteer(getNewVolunteerId(), args[2], std::stoi(args[4]),
                                                                            std::stoi(args[5]), std::stoi(args[6]));

                addVolunteer(drivol);
            }
            else
                throw;
        }
        else
            throw;
    }

    config.close();
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

void WareHouse::addVolunteer(Volunteer *volunteer)
{
    volunteers.push_back(volunteer);
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

void simulateStep()
{
    

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
        cloned->actionsLog.push_back(action->clone());
    for (Volunteer *volunteer : volunteers)
        cloned->volunteers.push_back(volunteer->clone());
    for (Order *order : pendingOrders)
        cloned->pendingOrders.push_back(order->clone());
    for (Order *order : inProcessOrders)
        cloned->inProcessOrders.push_back(order->clone());
    for (Order *order : completedOrders)
        cloned->completedOrders.push_back(order->clone());
    for (Customer *customer : customers)
        cloned->customers.push_back(customer->clone());

    cloned->customerCounter = this->customerCounter;
    cloned->orderCounter = this->orderCounter;
    cloned->volunteerCounter = this->volunteerCounter;

    return cloned;
}
