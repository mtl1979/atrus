#include "VoidBot.h"
#include "system/SetupSystem.h"

int main(int ArgC, char** ArgV)
{
	muscle::CompleteSetupSystem css;
	VoidBot bot;
	return bot.Run(ArgC, ArgV);
}

