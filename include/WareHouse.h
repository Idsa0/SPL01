#pragma once
#include <string>
#include <vector>

#include "Order.h"
#include "Customer.h"
#include "Volunteer.h"
#include <fstream>
#include <iostream>

class BaseAction;
class Volunteer;

// Warehouse responsible for Volunteers, Customers Actions, and Orders.


class WareHouse {

    public:
        WareHouse();// TODO we added this
        WareHouse(const string &configFilePath);
        void start();
        void addOrder(Order* order);
        void addAction(BaseAction* action);
        void addCustomer(Customer* customer);
        void addVolunteer(Volunteer* volunteer);
        Customer &getCustomer(int customerId) const;
        Volunteer &getVolunteer(int volunteerId) const;
        Order &getOrder(int orderId) const;
        const vector<BaseAction*> &getActions() const;
        void close();
        void open();
        int getNewCustomerId();  // TODO we added this
        int getNewVolunteerId(); // TODO we added this
        int getNewOrderId(); // TODO we added this
        ~WareHouse(); // TODO rule of 3/5
        WareHouse *clone() const;
        

    private:
        bool isOpen;
        vector<BaseAction*> actionsLog;
        vector<Volunteer*> volunteers;
        vector<Order*> pendingOrders;
        vector<Order*> inProcessOrders;
        vector<Order*> completedOrders;
        vector<Customer*> customers;
        int customerCounter; //For assigning unique customer IDs
        int volunteerCounter; //For assigning unique volunteer IDs
        int orderCounter; // TODO we added this
        void buildFromConfigurationFile(const std::string &path); // TODO we added this
};
