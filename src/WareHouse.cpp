#include "WareHouse.h"
#include "Volunteer.h"
#include <iostream>
#include <vector>
#include <algorithm>
#include "Action.h"

WareHouse::WareHouse(const string &configFilePath) : WareHouse()
{
    buildFromConfigurationFile(configFilePath);
}

WareHouse::WareHouse() : nullCustomer(new CivilianCustomer(-1, "nobody", -1, -1)), nullCollector(new CollectorVolunteer(-1, "nobody", -1)), nullDriver(new DriverVolunteer(-1, "nobody", -1, -1)), nullAction(new NullAction()), nullOrder(new Order(-1, -1, -1)),
                         isOpen{false}, actionsLog(vector<BaseAction *>{}), volunteers(vector<Volunteer *>{}), pendingOrders(vector<Order *>{}), inProcessOrders(vector<Order *>{}), completedOrders(vector<Order *>{}), customers(vector<Customer *>{}),
                         customerCounter{0}, volunteerCounter{0}, orderCounter{0} {}

WareHouse::WareHouse(const WareHouse &other) : WareHouse()
{
    for (BaseAction *action : other.actionsLog)
        this->actionsLog.push_back(action->clone());
    for (Volunteer *volunteer : other.volunteers)
        this->volunteers.push_back(volunteer->clone());
    for (Order *order : other.pendingOrders)
        this->pendingOrders.push_back(order->clone());
    for (Order *order : other.inProcessOrders)
        this->inProcessOrders.push_back(order->clone());
    for (Order *order : other.completedOrders)
        this->completedOrders.push_back(order->clone());
    for (Customer *customer : other.customers)
        this->customers.push_back(customer->clone());

    this->isOpen = other.isOpen;
    this->customerCounter = other.customerCounter;
    this->volunteerCounter = other.volunteerCounter;
    this->orderCounter = other.orderCounter;
}

WareHouse::WareHouse(WareHouse &&other) : WareHouse()
{
    for (BaseAction *action : other.actionsLog)
        this->actionsLog.push_back(action->clone());
    for (Volunteer *volunteer : other.volunteers)
        this->volunteers.push_back(volunteer->clone());
    for (Order *order : other.pendingOrders)
        this->pendingOrders.push_back(order->clone());
    for (Order *order : other.inProcessOrders)
        this->inProcessOrders.push_back(order->clone());
    for (Order *order : other.completedOrders)
        this->completedOrders.push_back(order->clone());
    for (Customer *customer : other.customers)
        this->customers.push_back(customer->clone());

    this->isOpen = other.isOpen;
    this->customerCounter = other.customerCounter;
    this->volunteerCounter = other.volunteerCounter;
    this->orderCounter = other.orderCounter;
}

void WareHouse::start()
{
    open();
    std::cout << "Warehouse is open!" << std::endl;

    while (isOpen)
    {
        string inputString;
        std::getline(std::cin, inputString);
        BaseAction *action = WareHouse::parse(inputString);
        if (!action->isNull)
            action->act(*this);
    }
}

void WareHouse::buildFromConfigurationFile(const std::string &path)
{
    std::ifstream config(path);
    if (!config.is_open())
    {
        std::cout << "could not open file!" << std::endl;
        exit(-1);
    }

    std::string line;
    while (std::getline(config, line))
    {
        if (line.empty())
            continue;

        std::vector<std::string> args = WareHouse::argLineBreaker(line, true);

        if (args.empty())
            continue;

        if (args[0] == "customer")
        {
            if (args.size() != 5)
                std::cout << "Could not parse: " << line << std::endl;

            Customer *customer;
            if (args[2] == "civilian")
                customer = new CivilianCustomer(getNewCustomerId(), args[1], std::stoi(args[3]), std::stoi(args[4]));
            else if (args[2] == "soldier")
                customer = new SoldierCustomer(getNewCustomerId(), args[1], std::stoi(args[3]), std::stoi(args[4]));

            addCustomer(customer);
        }
        else if (args[0] == "volunteer")
        {
            if (args.size() <= 3)
                std::cout << "Could not parse: " << line << std::endl;

            if (args[2] == "collector")
            {
                if (args.size() != 4)
                    std::cout << "Could not parse: " << line << std::endl;
                CollectorVolunteer *colvol = new CollectorVolunteer(getNewVolunteerId(), args[1], std::stoi(args[3]));
                addVolunteer(colvol);
            }
            else if (args[2] == "limited_collector")
            {
                if (args.size() != 5)
                    std::cout << "Could not parse: " << line << std::endl;
                LimitedCollectorVolunteer *limcolvol = new LimitedCollectorVolunteer(getNewVolunteerId(), args[1], std::stoi(args[3]), std::stoi(args[4]));
                addVolunteer(limcolvol);
            }
            else if (args[2] == "driver")
            {
                if (args.size() != 5)
                    std::cout << "Could not parse: " << line << std::endl;
                DriverVolunteer *drivol = new DriverVolunteer(getNewVolunteerId(), args[1], std::stoi(args[3]), std::stoi(args[4]));
                addVolunteer(drivol);
            }
            else if (args[2] == "limited_driver")
            {
                if (args.size() != 6)
                    std::cout << "Could not parse: " << line << std::endl;
                LimitedDriverVolunteer *drivol = new LimitedDriverVolunteer(getNewVolunteerId(), args[1], std::stoi(args[3]),
                                                                            std::stoi(args[4]), std::stoi(args[5]));

                addVolunteer(drivol);
            }
            else
                std::cout << "Could not parse: " << line << std::endl;
        }
        else
            std::cout << "Could not parse: " << line << std::endl;
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
    for (Customer *customer : customers)
        if (customer->getId() == customerId)
            return *customer;

    return *nullCustomer;
}

Volunteer &WareHouse::getVolunteer(int volunteerId) const
{
    for (Volunteer *volunteer : volunteers)
        if (volunteer->getId() == volunteerId)
            return *volunteer;

    return *nullCollector;
}

Order &WareHouse::getOrder(int orderId) const
{
    for (Order *order : pendingOrders)
        if (order->getId() == orderId)
            return *order;

    for (Order *order : inProcessOrders)
        if (order->getId() == orderId)
            return *order;

    for (Order *order : completedOrders)
        if (order->getId() == orderId)
            return *order;

    return *nullOrder;
}

Order *WareHouse::getOrderPointer(int orderId) const
{
    for (Order *order : pendingOrders)
        if (order->getId() == orderId)
            return order;

    for (Order *order : inProcessOrders)
        if (order->getId() == orderId)
            return order;

    for (Order *order : completedOrders)
        if (order->getId() == orderId)
            return order;

    return nullOrder;
}

template <typename T>
void eraseElement(std::vector<T> &vec, const T &element)
{
    auto it = std::find(vec.begin(), vec.end(), element);
    if (it != vec.end())
        vec.erase(it);
}

void WareHouse::simulateStep()
{
    // step 1
    int n = pendingOrders.size();
    Order **orderArray = new Order *[n];

    for (int i = 0; i < n; i++)
        orderArray[i] = pendingOrders[i];

    for (int i = 0; i < n; i++)
    {
        Order *order = orderArray[i];
        if (order->getStatus() == OrderStatus::PENDING)
        {
            for (Volunteer *volunteer : volunteers)
                if (volunteer->getType() == VolunteerType::COLLECTOR)
                    if (volunteer->canTakeOrder(*order))
                    {
                        volunteer->acceptOrder(*order);
                        order->setStatus(OrderStatus::COLLECTING);
                        order->setCollectorId(volunteer->getId());
                        inProcessOrders.push_back(order);
                        eraseElement(pendingOrders, order);
                        break;
                    }
        }
        else if (order->getStatus() == OrderStatus::COLLECTING)
        {
            for (Volunteer *volunteer : volunteers)
                if (volunteer->getType() == VolunteerType::DRIVER)
                    if (volunteer->canTakeOrder(*order))
                    {
                        volunteer->acceptOrder(*order);
                        order->setStatus(OrderStatus::DELIVERING);
                        order->setDriverId(volunteer->getId());
                        inProcessOrders.push_back(order);
                        eraseElement(pendingOrders, order);
                        break;
                    }
        }
        else
        {
            std::cout << "Order of illegal type in pendingOrders" << std::endl;
            std::cout << order->toString();
            throw;
        }
    }

    // step 2
    for (Volunteer *volunteer : volunteers)
        volunteer->step();

    // step 3
    for (Volunteer *volunteer : volunteers)
        if (!volunteer->isBusy() && volunteer->getCompletedOrderId() != NO_ORDER)
        {
            Order *order = getOrderPointer(volunteer->getCompletedOrderId());
            if (order->getStatus() == OrderStatus::COLLECTING)
            {
                pendingOrders.push_back(order);
                eraseElement(inProcessOrders, order);
                volunteer->setCompletedOrderId(NO_ORDER);
            }

            else if (order->getStatus() == OrderStatus::DELIVERING)
            {
                completedOrders.push_back(order);
                eraseElement(inProcessOrders, order);
                order->setStatus(OrderStatus::COMPLETED);
                volunteer->setCompletedOrderId(NO_ORDER);
            }

            else
            {
                std::cout << order->toString() << std::endl;
                std::cout << volunteer->toString() << std::endl;
                std::cout << "Order of illegal type in inProcessOrders" << std::endl;
                throw;
            }
        }

    // step 4
    n = volunteers.size();
    Volunteer **volArray = new Volunteer *[n];

    for (int i = 0; i < n; i++)
        volArray[i] = volunteers[i];

    for (int i = 0; i < n; i++)
    {
        Volunteer *volunteer = volArray[i];
        if (!volunteer->hasOrdersLeft() && !volunteer->isBusy())
        {
            eraseElement(volunteers, volunteer);
            delete volunteer;
        }
    }

    delete[] orderArray;
    delete[] volArray;
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

    delete nullCustomer;
    delete nullCollector;
    delete nullDriver;
    delete nullAction;
    delete nullOrder;
}

WareHouse &WareHouse::operator=(const WareHouse &other)
{
    if (this != &other)
    {
        for (BaseAction *action : actionsLog)
            delete action;
        actionsLog.clear();

        for (Volunteer *volunteer : volunteers)
            delete volunteer;
        volunteers.clear();

        for (Order *order : pendingOrders)
            delete order;
        pendingOrders.clear();

        for (Order *order : inProcessOrders)
            delete order;
        inProcessOrders.clear();

        for (Order *order : completedOrders)
            delete order;
        completedOrders.clear();

        for (Customer *customer : customers)
            delete customer;
        customers.clear();

        this->isOpen = other.isOpen;
        for (BaseAction *action : other.actionsLog)
            this->actionsLog.push_back(action->clone());
        for (Volunteer *volunteer : other.volunteers)
            this->volunteers.push_back(volunteer->clone());
        for (Order *order : other.pendingOrders)
            this->pendingOrders.push_back(order->clone());
        for (Order *order : other.inProcessOrders)
            this->inProcessOrders.push_back(order->clone());
        for (Order *order : other.completedOrders)
            this->completedOrders.push_back(order->clone());
        for (Customer *customer : other.customers)
            this->customers.push_back(customer->clone());

        this->customerCounter = other.customerCounter;
        this->orderCounter = other.orderCounter;
        this->volunteerCounter = other.volunteerCounter;
    }

    return *this;
}

WareHouse *WareHouse::clone() const
{
    return new WareHouse(*this);
}

WareHouse &WareHouse::operator=(WareHouse &&other)
{
    if (this != &other)
    {
        for (BaseAction *action : actionsLog)
            delete action;
        actionsLog.clear();

        for (Volunteer *volunteer : volunteers)
            delete volunteer;
        volunteers.clear();

        for (Order *order : pendingOrders)
            delete order;
        pendingOrders.clear();

        for (Order *order : inProcessOrders)
            delete order;
        inProcessOrders.clear();

        for (Order *order : completedOrders)
            delete order;
        completedOrders.clear();

        for (Customer *customer : customers)
            delete customer;
        customers.clear();

        this->isOpen = other.isOpen;
        for (BaseAction *action : other.actionsLog)
            this->actionsLog.push_back(action->clone());
        for (Volunteer *volunteer : other.volunteers)
            this->volunteers.push_back(volunteer->clone());
        for (Order *order : other.pendingOrders)
            this->pendingOrders.push_back(order->clone());
        for (Order *order : other.inProcessOrders)
            this->inProcessOrders.push_back(order->clone());
        for (Order *order : other.completedOrders)
            this->completedOrders.push_back(order->clone());
        for (Customer *customer : other.customers)
            this->customers.push_back(customer->clone());

        this->customerCounter = other.customerCounter;
        this->orderCounter = other.orderCounter;
        this->volunteerCounter = other.volunteerCounter;
    }

    return *this;
}

// HELPER FUNCTIONS

BaseAction *WareHouse::parse(std::string &input)
{
    std::vector<std::string> args = argLineBreaker(input);

    if (args.size() == 0)
    {
        helpPrinter();
        return this->nullAction;
    }

    BaseAction *action;
    if (args[0] == "step")
    {
        if (args.size() != 2)
        {
            helpPrinter();
            return nullAction;
        }

        action = new SimulateStep(stoi(args[1]));
    }
    else if (args[0] == "order")
    {

        if (args.size() != 2)
        {
            helpPrinter();
            return nullAction;
        }

        action = new AddOrder(stoi(args[1]));
    }
    else if (args[0] == "customer")
    {
        if (args.size() != 5)
        {
            helpPrinter();
            return nullAction;
        }

        action = new AddCustomer(args[1], args[2], stoi(args[3]), stoi(args[4]));
    }
    else if (args[0] == "orderStatus")
    {
        if (args.size() != 2)
        {
            helpPrinter();
            return nullAction;
        }

        action = new PrintOrderStatus(stoi(args[1]));
    }
    else if (args[0] == "customerStatus")
    {
        if (args.size() != 2)
        {
            helpPrinter();
            return nullAction;
        }

        action = new PrintCustomerStatus(stoi(args[1]));
    }
    else if (args[0] == "volunteerStatus")
    {
        if (args.size() != 2)
        {
            helpPrinter();
            return nullAction;
        }

        action = new PrintVolunteerStatus(stoi(args[1]));
    }
    else if (args[0] == "log")
    {
        if (args.size() != 1)
        {
            helpPrinter();
            return nullAction;
        }

        action = new PrintActionsLog();
    }
    else if (args[0] == "close")
    {
        if (args.size() != 1)
        {
            helpPrinter();
            return nullAction;
        }

        action = new Close();
    }
    else if (args[0] == "backup")
    {
        if (args.size() != 1)
        {
            helpPrinter();
            return nullAction;
        }

        action = new BackupWareHouse();
    }
    else if (args[0] == "restore")
    {
        if (args.size() != 1)
        {
            helpPrinter();
            return nullAction;
        }

        action = new RestoreWareHouse();
    }
    else
    {
        helpPrinter();
        return nullAction;
    }

    return action;
}

void WareHouse::helpPrinter()
{
    std::cout << "USAGE:\n\nstep (int)step_number\norder (int)ordering_customer\ncustomer (string)customer_name (soldier/civilian)customer_type (int)distance (int)max_orders\norderStatus (int)order_id\ncustomerStatus (int)customer_id\nvolunteerStatus (int)volunteer_id\nlog\nclose\nbackup\nrestore" << std::endl;
}

vector<std::string> WareHouse::argLineBreaker(std::string line, bool ignoreComments)
{
    std::vector<std::string> args;

    std::string word;
    for (char c : line)
    {
        if (ignoreComments && c == '#')
            break;
        else if (c != ' ')
            word += c;
        else if (!word.empty())
        {
            args.push_back(word);
            word.clear();
        }
    }
    if (!word.empty())
        args.push_back(word);

    return args;
}