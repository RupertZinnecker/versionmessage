/*********************************************************************************************
*																															*
*	A program to send a versionmessage to the Namecoin client.						*
*																															*
*	This class builds up a networkstream as client and sends a 						*
*	message to specified ip and port.																	*
*																															*
*	copyright @ Rupert Zinnecker																		*
*																															*
**********************************************************************************************/

#include <iostream>
#include <string>
#include <boost/asio.hpp>
#include <boost/lexical_cast.hpp>

#include "network.h"

using namespace std;

void Network::send_message(string message, string ip, int port)
{
	try 
	{
		boost::system::error_code error;
		boost::array<char, 109> buf;
		copy (message.begin(), message.end(), buf.data());
		
		boost::asio::io_service ios;
		boost::asio::ip::tcp::socket socket(ios);	
		socket.connect(boost::asio::ip::tcp::endpoint(
			boost::asio::ip::address::from_string(ip),
			boost::lexical_cast<int>(port)));
	
		boost::asio::write(socket, boost::asio::buffer(buf, 109));
		
		boost::array<char, 256> buf2;
		boost::asio::read(socket, boost::asio::buffer(buf2, 256), boost::asio::transfer_all(), error);
		
		string output(buf2.begin(), buf2.end());
		cout << output << endl;
	}
	catch(boost::system::system_error const& e)
	{}
}
