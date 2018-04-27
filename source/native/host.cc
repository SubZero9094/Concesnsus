// This is the main function for the NATIVE version of this project.

#include <iostream>

#include "base/vector.h"
#include "config/command_line.h"
#include "../graph.h"

int main(int argc, char* argv[])
{
	Graph G("Toroidal", 3,1);
	std::cout << std::endl;
	std::cout << "*********************************************************************" << std::endl;
	std::cout << G.GetDim() << std::endl;
	std::cout << G.GetName() << std::endl;
	std::cout << std::endl;

	G.PrintFriends();
	std::cout << std::endl;

	G.PrintSchedule();
	std::cout <<std::endl;
	G.ShuffleSchedule();
	G.PrintSchedule();
	std::cout <<std::endl;
	G.ShuffleSchedule();
	std::cout <<std::endl;
	G.PrintSchedule();

	return 0;
}