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
        {
            helpPrinter();
			return &nullAction;
		}

        action = new SimulateStep(stoi(args[1]));
        // TODO this assumes we are given an int and a valid one, also true for all other actions here pretty much
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
    else {
		helpPrinter();
		return &nullAction;
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


void InputHandler::helpPrinter(){
	std::ifstream text("/home/tkandel/Desktop/coding/SPL01-input_handling/data/helpText"); // TODO: having external text won't fly in the assignment
    if (!text.is_open())
        throw ("helpText file not found");
        
    std::string line;
	while (std::getline(text, line)){
		std::cout << line << std::endl;
	}

}
