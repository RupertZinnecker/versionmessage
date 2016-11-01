/*********************************************************************************************
*																															*
*	A program to send a versionmessage to the Namecoin client.						*
*																															*
*	This class calls the method to build the message and then sends the			*
*	message to the ip given as command line parameter.									*
*																															*
*	copyright @ Rupert Zinnecker																		*
*																															*
**********************************************************************************************/

#include <iostream>
#include <string>

#include "utils.h"
#include "sha256.h"
#include "network.h"

using namespace std;

int main(int argc, char* argv[]) {	
    
	if(argc != 2)
	{
		cout << "usage: ./message <destination ip>" << endl;
		return 0;
	}
	else
	{
		Utils utils;
		Network network;
		
		
		string message = utils.build_version_message("37601","127.0.0.1", "8334", argv[1], "8334", "250000");
		network.send_message(message,argv[1],8334);
		
		return 0;
	}	
}
