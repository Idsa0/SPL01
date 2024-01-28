#include "InputHandler.h"

BaseAction *InputHandler::parse(std::string &input)
{
    std::vector<std::string> args;

    std::string word;
    for (char c : input)
        if (c != ' ')
            word += c;
        else if (!word.empty())
        {
            args.push_back(word);
            word.clear();
        }
    if (!word.empty())
        args.push_back(word);

    if (args.size() == 0)
    {
        helpPrinter();
        return &nullAction;
    }

    BaseAction *action;
    if (args[0] == "step")
    {
        if (args.size() != 2)
        {
            helpPrinter();
            return &nullAction;
        }

        action = new SimulateStep(stoi(args[1]));
        // TODO this assumes we are given an int and a valid one, also true for all other actions here pretty much
        // should we check validity of values?
    }
    else if (args[0] == "order")
    {

        if (args.size() != 2)
        {
            helpPrinter();
            return &nullAction;
        }

        action = new AddOrder(stoi(args[1]));
    }
    else if (args[0] == "customer")
    {
        if (args.size() != 5)
        {
            helpPrinter();
            return &nullAction;
        }

        action = new AddCustomer(args[1], args[2], stoi(args[3]), stoi(args[4]));
    }
    else if (args[0] == "orderStatus")
    {
        if (args.size() != 2)
        {
            helpPrinter();
            return &nullAction;
        }

        action = new PrintOrderStatus(stoi(args[1]));
    }
    else if (args[0] == "customerStatus")
    {
        if (args.size() != 2)
        {
            helpPrinter();
            return &nullAction;
        }

        action = new PrintCustomerStatus(stoi(args[1]));
    }
    else if (args[0] == "volunteerStatus")
    {
        if (args.size() != 2)
        {
            helpPrinter();
            return &nullAction;
        }

        action = new PrintVolunteerStatus(stoi(args[1]));
    }
    else if (args[0] == "log")
    {
        if (args.size() != 1)
        {
            helpPrinter();
            return &nullAction;
        }

        action = new PrintActionsLog();
    }
    else if (args[0] == "close")
    {
        if (args.size() != 1)
        {
            helpPrinter();
            return &nullAction;
        }

        action = new Close();
    }
    else if (args[0] == "backup")
    {
        if (args.size() != 1)
        {
            helpPrinter();
            return &nullAction;
        }

        action = new BackupWareHouse();
    }
    else if (args[0] == "restore")
    {
        if (args.size() != 1)
        {
            helpPrinter();
            return &nullAction;
        }

        action = new RestoreWareHouse();
    }
    else
    {
        helpPrinter();
        return &nullAction;
    }
    // There is no way to add volunteers, this action does not exist in the instructions for some reason
    return action;
}

void InputHandler::helpPrinter()
{
    std::cout << "USAGE:\n\nstep (int)step_number\norder (int)ordering_customer\ncustomer (string)customer_name (soldier/civilian)customer_type (int)distance (int)max_orders\norderStatus (int)order_id\ncustomerStatus (int)customer_id\nvolunteerStatus (int)volunteer_id\nlog\nclose\nbackup\nrestore" << std::endl;
}
