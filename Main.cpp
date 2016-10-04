#include "Bot.h"
#include "MyBotLogic/MyBotLogic.h"

#include <iostream>
#include <stdio.h>

/*
    This program will play a single game of AIBootCamp by communicating with
    the engine via IPC input and output pipes.

    The function "FillActionList()" in MyBotLogic.cpp is where you should add your AI code.
	You are allowed to edit any part of any files, remove them, or add your
    own, provided you continue conforming to the IPC pipe input and output protocol.
*/
int main(int argc, char *argv[])
{
	std::cout.sync_with_stdio(0); //this line makes your bot faster

	BotLogicIF* pBotLogic = new MyBotLogic(); //instantiate your BotLogic module here

	Bot bot(pBotLogic);
	bot.Configure(argc, argv);
	bot.Run();

    return 0;
}
