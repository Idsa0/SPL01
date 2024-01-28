#pragma once
#include <iostream>
#include <vector>
#include <fstream>
#include "Action.h"

class InputHandler
{
    public:
        static BaseAction *parse(std::string &input);
		static void helpPrinter();
};
