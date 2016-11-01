/*********************************************************************************************
*																															*
*	A program to send a versionmessage to the Namecoin client.						*
*																															*
*	This class builds the version message.															*
*																															*
*	copyright @ Rupert Zinnecker																		*
*																															*
**********************************************************************************************/

#include <iostream>
#include <cstring>
#include <cmath>
#include <ctime>
#include <sstream>
#include <cstdlib>

#include "utils.h"
#include "sha256.h"

using namespace std;

string Utils::dec_to_hex(string in){
	int con = 0; 
	int help = 0;
	int length = in.length();
	string output = "";
	string hex = "0123456789abcdef";
	
	for(int i = 0;i<length;i++) {
		con +=  (in.at(i) - 48)* pow(10, (length-i-1));
	}
	do{
		output = hex.at(con % 16) + output;	
		con /= 16;
	}while(con > 0); 	
	
	return output;	
}

string Utils::convert_IP_to_hex(string in){
	int i = 0;
	string j;
	string message = "";
	char a;
	in += ".";
	for(int k=0;k<4;k++){
		a = in.at(i);
		j = "";
		do{		
			j = j + in.at(i);
			i++;
			a = in.at(i);
		}while(a != '.');
		i++;
		j = dec_to_hex(j);
		if(j.length() == 1)
			j = "0" + j;
		message += j;
	}	
		
	message = "00000000000000000000ffff" + message;
	return message;	
}

string Utils::convert_time_to_hex(){
	time_t posix;
	posix = time(0);
	stringstream ss;
	ss << posix;
	string ts = ss.str();
	ts = dec_to_hex(ts);
	
	int len = ts.length();
	for (int i=0;i<16 - len;i++)
		ts = "0" + ts;
	
	ts = reverse_hex(ts);
	
	return ts;
}

string Utils::reverse_hex(string in){
	string help;
	int i;
	for(i=0;i < in.length() / 2;i++) {
		help = in.substr(i*2,2) + help;
	}
	return help;
}

string Utils::convert_block_to_reverse_hex(string in){
	in = dec_to_hex(in);

	int len = in.length();
	for (int i=0;i<8 - len;i++)
		in = "0" + in;

	in = reverse_hex(in);
	
	return in;
}

string Utils::build_payload(string version, string source, string source_port, string destination, string destination_port,
                                 string block){
	string message;
	version = dec_to_hex(version);
	int len = version.length();
	for(int i=0; i<8-len; i++)
		version = "0" + version;
	version = reverse_hex(version);
	message = version + "0100000000000000";
    message += convert_time_to_hex() + "0100000000000000" + convert_IP_to_hex(destination) + dec_to_hex(destination_port)/*"208e"*/ + "0100000000000000";
    message += convert_IP_to_hex(source) + dec_to_hex(source_port)/*"208e"*/ + "6e913a74ee2ca427" + "00";
	message += convert_block_to_reverse_hex(block);
	
	return message;	
}

string Utils::payload_length(string in){
	string out = to_string(in.length()/2);
	out = dec_to_hex(out);
	int length = out.length();
	for(int i=0;i<8-length;i++){
		out = "0" + out;
	}
	return reverse_hex(out);
}

string Utils::build_version_message(string version, string source, string source_port, string destination, string destination_port,
                                         string block){
	string message;
	SHA256 sha256;
	message = "f9beb4fe";
	message += "76657273696f6e0000000000";
    string payload = build_payload(version, source, source_port, destination, destination_port, block);
	message += payload_length(payload);
	message += sha256.calculateSHA256(sha256.calculateSHA256(payload)).substr(0,8);
	message += payload;
	return convert_hex_to_ascii(message);
}

string Utils::convert_hex_to_ascii(string input)
{
	string output = "";
	int length = input.length();
	for(int i=0;i<length;i+=2)
	{
		int help1 = hex_to_dec(input.substr(i,2));
		char c ( help1 );
		string help2(1,c);
		output += help2;
	}
	return output;
}

int Utils::hex_to_dec(string input)
{
	string hexNumbers = "0123456789abcdef";
	int output = 0;
	for(int i=0;i<input.length();i++)
	{
		char s = input.at(i);
		int help, length = input.length();
		switch (s)
		{
			case '0' : 	case '1' : 	case '2' : 	case '3' : 	case '4' : 	case '5' : 	case '6' : 	case '7' : 	case '8' : 	case '9' : 
				help = s - 48;
				break;
			case 'a':
				help = 10;
				break;
			case 'b' :
				help = 11;
				break;
			case 'c' :
				help = 12;
				break;
			case 'd' :
				help = 13;
				break;
			case 'e' :
				help = 14;
				break;
			case 'f' :
				help = 15;
				break;
		}
		output += help * pow(16,length - i - 1);		
	}
	return output;
}



