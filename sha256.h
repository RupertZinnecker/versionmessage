/*********************************************************************************************
*																															*
*	A program to send a versionmessage to the Namecoin client.						*	
*																															*
*	copyright @ Rupert Zinnecker																		*
*																															*
**********************************************************************************************/

#ifndef _SHA256
#define _SHA256

#include <cstring>

using namespace std;

class SHA256{
	private:
		string h0 = "6a09e667";
		string h1 = "bb67ae85";
		string h2 = "3c6ef372";
		string h3 = "a54ff53a";	
		string h4 = "510e527f";
		string h5 = "9b05688c";
		string h6 = "1f83d9ab";
		string h7 = "5be0cd19";
		
		string k[64] =  {"428a2f98","71374491","b5c0fbcf","e9b5dba5","3956c25b","59f111f1","923f82a4","ab1c5ed5",
								"d807aa98","12835b01","243185be","550c7dc3","72be5d74","80deb1fe","9bdc06a7","c19bf174",
								"e49b69c1","efbe4786","0fc19dc6","240ca1cc","2de92c6f","4a7484aa","5cb0a9dc","76f988da",
								"983e5152","a831c66d","b00327c8","bf597fc7","c6e00bf3","d5a79147","06ca6351","14292967",
								"27b70a85","2e1b2138","4d2c6dfc","53380d13","650a7354","766a0abb","81c2c92e","92722c85",
								"a2bfe8a1","a81a664b","c24b8b70","c76c51a3","d192e819","d6990624","f40e3585","106aa070",
								"19a4c116","1e376c08","2748774c","34b0bcb5","391c0cb3","4ed8aa4a","5b9cca4f","682e6ff3",
								"748f82ee","78a5636f","84c87814","8cc70208","90befffa","a4506ceb","bef9a3f7","c67178f2"};
		
	public:
		string calculateSHA256(string);
		
		string* makeBlocks(string);
		string** parseExpandedMessageBlocks(string*, int);
		string** computeExpandedMessageBlocks(string**, int);
		
		string ch(string, string, string);
		string maj(string, string, string);
		string bigSigma0(string);
		string bigSigma1(string);
		string sigma0(string);
		string sigma1(string);
		
		string negate(string);
		string _or(string, string);
		string _and(string, string);
		string _xor(string, string);
		string rightShift(string, int);
		string rightRotation(string, int);
		
		string decToHex(int);
		string bitsToHex(string);
		string hexToBits(string);
		string bitAddition(string, string);
		string modAddition(string, string);
};

#endif //_SHA256