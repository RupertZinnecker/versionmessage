/*********************************************************************************************
*																															*
*	A program to send a versionmessage to the Namecoin client.						*
*																															*
*	copyright @ Rupert Zinnecker																		*
*																															*
**********************************************************************************************/

#ifndef _UTILS
#define _UTILS

#include <string>

using namespace std;

class Utils
{
	private:
		
	public:
		string dec_to_hex(string);
		string convert_IP_to_hex(string);
		string convert_time_to_hex();
		string reverse_hex(string);
		string convert_block_to_reverse_hex(string);
        string build_payload(string, string, string, string, string, string);
		string payload_length(string);
        string build_version_message(string, string, string, string, string, string);
		string convert_hex_to_ascii(string);
		int hex_to_dec(string);
};

#endif //_UTILS
