#pragma once
#include "InputHandler.h"

BaseAction *InputHandler::parse(std::string &input)
{
    std::vector<std::string> args{};

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

    BaseAction *action;
    if (args[0] == "step")
    {
        if (args.size() != 2)
            throw;

        action = new SimulateStep(stoi(args[1]));
        // TODO this assumes we are given an int and a valid one, also true for all other actions here pretty much
    }
    else if (args[0] == "order")
    {
        if (args.size() != 2)
            throw;

        action = new AddOrder(stoi(args[1]));
    }
    else if (args[0] == "customer")
    {
        if (args.size() != 5)
            throw;

        action = new AddCustomer(args[1], args[2], stoi(args[3]), stoi(args[4]));
    }
    else if (args[0] == "orderStatus")
    {
        if (args.size() != 2)
            throw;

        action = new PrintOrderStatus(stoi(args[1]));
    }
    else if (args[0] == "customerStatus")
    {
        if (args.size() != 2)
            throw;

        action = new PrintCustomerStatus(stoi(args[1]));
    }
    else if (args[0] == "volunteerStatus")
    {
        if (args.size() != 2)
            throw;

        action = new PrintVolunteerStatus(stoi(args[1]));
    }
    else if (args[0] == "log")
    {
        if (args.size() != 1)
            throw;

        action = new PrintActionsLog();
    }
    else if (args[0] == "close")
    {
        if (args.size() != 1)
            throw;

        action = new Close();
    }
    else if (args[0] == "backup")
    {
        if (args.size() != 1)
            throw;

        action = new BackupWareHouse();
    }
    else if (args[0] == "restore")
    {
        if (args.size() != 1)
            throw;

        action = new RestoreWareHouse();
    }
    // TODO there is no way to add volunteers, this action does not exist in the instructions for some reason

    /*
        TODO what if we received an unrecognized command?
        My proposed solution is creating
        class NullAction : public BaseAction
        with empty methods or a method to print a generic error
        we should also consider std::cerr instead of std::cout
    */
    return action;
}

std::vector<BaseAction *> InputHandler::readConfigurationFile(std::string &path)
{
    std::ifstream config(path);
    if (!config.is_open())
        throw;

    std::string line;
    std::vector<BaseAction *> actions;
    while (std::getline(config, line))
        actions.push_back(parse(line));

    config.close();
    return actions;
}
