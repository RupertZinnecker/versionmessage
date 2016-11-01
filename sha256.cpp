/*********************************************************************************************
*																															*
*	A program to send a versionmessage to the Namecoin client.						*
*																															*
*	This class calculates the secure hash algorithm 256 (SHA256)					*
*																															*
*	copyright @ Rupert Zinnecker																		*
*																															*
**********************************************************************************************/


#include <iostream>
#include <cstring>
#include <cmath>

#include "sha256.h"

using namespace std;

string SHA256::calculateSHA256(string in){
	string out, hash, t1, t2;
	string* blocks;
	string** parsedBlocks;
	string** expandedBlocks;
	string* intermediateHash = new string[8];
	string* reg = new string[8];
	
	int help, length;
	int numberOfBlocks;
	
	length = in.length();
	
	//Convert ASCII to hex
	/*for(int i=0;i<length;i++){
		help = (int) in.at(i);
		out += decToHex(help);
	}*/	
	out = in;
		
	length = out.length();
	if(length % 128 > 111)
		numberOfBlocks = length/128 + 2;
	else 
		numberOfBlocks = length/128 + 1;
	
	blocks = makeBlocks(out);
	parsedBlocks = parseExpandedMessageBlocks(blocks, numberOfBlocks);
	expandedBlocks = computeExpandedMessageBlocks(parsedBlocks, numberOfBlocks);
	
	intermediateHash[0] = h0; 
	intermediateHash[1] = h1;
	intermediateHash[2] = h2;
	intermediateHash[3] = h3;
	intermediateHash[4] = h4;
	intermediateHash[5] = h5;
	intermediateHash[6] = h6;
	intermediateHash[7] = h7;
	
	for(int i=0;i<numberOfBlocks;i++){
		for(int j=0;j<8;j++)			
			reg[j] = intermediateHash[j];  
		
		for(int j=0;j<64;j++){			
								
			t1 = modAddition(modAddition(modAddition(modAddition(reg[7],bigSigma1(reg[4])),ch(reg[4],reg[5],reg[6])),k[j]),expandedBlocks[i][j]);			
			t2 = modAddition(bigSigma0(reg[0]),maj(reg[0],reg[1],reg[2]));
			
			reg[7] = reg[6];
			reg[6] = reg[5];
			reg[5] = reg[4];
			reg[4] = modAddition(reg[3],t1);
			reg[3] = reg[2];
			reg[2] = reg[1];
			reg[1] = reg[0];
			reg[0] =	modAddition(t1,t2);			
			
		}		
		for(int j=0;j<8;j++)			
			intermediateHash[j] = modAddition(reg[j],intermediateHash[j]); 
	}
	
	for(int i=0;i<8;i++)
		hash += intermediateHash[i]; 
	
	return hash;
}

string* SHA256::makeBlocks(string in){ 
	int numberOfBlocks, length2;
	int length = in.length();
	string lengthOfInput;
	if(length % 128 > 111){
		numberOfBlocks = length/128 + 2;
		in += "8";
		for(int j=0;j < 128 - length % 128 - 1 + 128 - 16;j++)
			in += "0";
		lengthOfInput = decToHex(length * 4);
		length2 = lengthOfInput.length();
		for(int i=0;i<16 - length2;i++)
			lengthOfInput = "0" + lengthOfInput;
		in += lengthOfInput;
	}		
	else{
		numberOfBlocks = length/128 + 1;
		in += "8";
		for(int j=0;j < 128 - length % 128 - 1 - 16;j++)
			in += "0";
		lengthOfInput = decToHex(length * 4);
		length2 = lengthOfInput.length();
		for(int i=0;i<16 - length2;i++)
			lengthOfInput = "0" + lengthOfInput;
		in += lengthOfInput;
	} 
			
	string* blocks = new string[numberOfBlocks];
	for(int i=0;i<numberOfBlocks;i++){
		blocks[i] = in.substr(i*128,128);	
	}	
	
	return blocks;
}

string** SHA256::parseExpandedMessageBlocks(string* in, int numberOfBlocks){
	string** out = new string*[numberOfBlocks];
	for(int i=0;i<numberOfBlocks;i++){
		out[i] = new string[16];
		for(int j=0;j<16;j++)
			out[i][j] = in[i].substr(j*8,8);
	}
	return out;
}

string** SHA256::computeExpandedMessageBlocks(string**  in, int numberOfBlocks){
	string** out = new string*[numberOfBlocks];
	for(int i=0;i<numberOfBlocks;i++){
		out[i] = new string[64];
		for(int j=0;j<16;j++)
			out[i][j] = in[i][j];
		for(int j=16;j<64;j++)
			out[i][j] = modAddition(modAddition(sigma1(out[i][j-2]),out[i][j-7]),modAddition(sigma0(out[i][j-15]),out[i][j-16])); 
	}
	return out;
}

string SHA256::ch(string in1, string in2, string in3){
	return _xor(_and(in1,in2),_and(negate(in1),in3));
}

string SHA256::maj(string in1, string in2, string in3){
	return _xor(_xor(_and(in1,in2),_and(in1,in3)),_and(in2,in3));
}

string SHA256::bigSigma0(string in){
	return _xor(_xor(rightRotation(in,2),rightRotation(in,13)),rightRotation(in,22));
}

string SHA256::bigSigma1(string in){
	return _xor(_xor(rightRotation(in,6),rightRotation(in,11)),rightRotation(in,25));
}

string SHA256::sigma0(string in){
	return _xor(_xor(rightRotation(in,7),rightRotation(in,18)),rightShift(in,3));
}

string SHA256::sigma1(string in){
	return _xor(_xor(rightRotation(in,17),rightRotation(in,19)),rightShift(in,10));
}


string SHA256::negate(string in){
	string out;
	in = hexToBits(in);
	int length = in.length(), help;
	for(int i=0;i<length;i++){
		help = (int) in.at(i) - 48;
		if(help)
			out += "0";
		else
			out += "1";
	} 
	return bitsToHex(out);
}

string SHA256::_or(string in1, string in2){
	string out;
	in1 = hexToBits(in1);
	in2 = hexToBits(in2);
	int length = in1.length(), help1, help2;
	for(int i=0;i<length;i++){
		help1 = (int) in1.at(i) - 48;
		help2 = (int) in2.at(i) - 48;
		if(help1 + help2)
			out += "1"; 
		else
			out += "0";
	}
	return bitsToHex(out);
}

string SHA256::_and(string in1, string in2){
	string out;
	in1 = hexToBits(in1);
	in2 = hexToBits(in2);
	int length = in1.length(), help1, help2;
	for(int i=0;i<length;i++){
		help1 = (int) in1.at(i) - 48;
		help2 = (int) in2.at(i) - 48;
		if(help1 + help2 == 2)
			out += "1";
		else
			out += "0";
	}
	return bitsToHex(out);
}

string SHA256::_xor(string in1, string in2){
	string out;
	in1 = hexToBits(in1);
	in2 = hexToBits(in2);
	int length = in1.length(), help1, help2;
	for(int i=0;i<length;i++){
		help1 = (int) in1.at(i) - 48;
		help2 = (int) in2.at(i) - 48;
		if(help1 + help2 == 1)
			out += "1";
		else
			out += "0";
	}
	return bitsToHex(out);
}

string SHA256::rightShift(string in, int n){
	string out;
	in = hexToBits(in);
	int length = in.length();
	for(int i=0;i<n;i++){
		out += "0";
	}
	for(int i=0;i<length - n;i++){
		out += in.at(i);
	}
	return bitsToHex(out);
}

string SHA256::rightRotation(string in, int n){
	string out;
	in = hexToBits(in);
	int length = in.length();
	for(int i=0;i<n;i++){
		out += in.at(length - n + i);
	}
	for(int i=0;i<length - n;i++){
		out += in.at(i);
	}
	return bitsToHex(out);
}

string SHA256::decToHex(int dec){
	string bits;
	int residual, length;
	do{
		residual = dec % 2;
		dec /= 2;
		bits = to_string(residual) + bits;
	}while(dec != 0);
	length = bits.length();
	if(length % 4 != 0)
		for(int i=0;i<4 - length % 4;i++){
			bits = "0" + bits;	
		}
	return bitsToHex(bits);
}

//Transforms a bit string into a hexadecimal string. Bit string needs to be 0 mod 4.
string SHA256::bitsToHex(string in){
	int hexLength = in.length() / 4;
	int dec;
	string help, out, hex;
	hex = "0123456789abcdef";
	for(int i = 0;i<hexLength;i++){
		help = in.substr(i*4,4);
		dec = 0;
		for(int j=0;j<4;j++){
			dec += ( (int) (help.at(3-j)) - 48 ) * pow(2,j);	
		}		  
		out += hex.at(dec); 
	}	
	return out;
}

//Transforms a hexadecimal string into a bit string.
string SHA256::hexToBits(string in){
	int hexLength = in.length(), hex;
	string* bits = new string[16];
	string hexAlphabet = "0123456789abcdef", out, help;
	bits[0] = "0000";
	for(int i=1;i<16;i++){
		bits[i] = bitAddition(bits[i-1],"0001"); 
	}
	for(int i=0;i<hexLength;i++){
		for(int j=0;j<16;j++){
			help = hexAlphabet.at(j);
			if(!in.compare(i,1,help))
				out = out + bits[j];
		}		
	}
	return out;
}

string SHA256::bitAddition(string in1, string in2){
	int bitLength = in1.length(), helpLength;
	string out, carry;
	carry = "0";
	for(int i=0;i<bitLength;i++){
		if(!carry.compare("0")){
			if(!in1.compare((bitLength - i - 1),1,"1") && !in2.compare((bitLength - i - 1),1,"1")){
				carry = "1";
				out = "0" + out;
			}
			else if((!in1.compare((bitLength - i - 1),1,"0") && !in2.compare((bitLength - i - 1),1,"1")) || (!in1.compare((bitLength - i - 1),1,"1") && !in2.compare((bitLength - i - 1),1,"0"))){
				carry = "0";
				out = "1" + out;
			}
			else{
				carry = "0";
				out = "0" + out;
			}
		}	
		else if(!carry.compare("1")){
			if(!in1.compare((bitLength - i - 1),1,"1") && !in2.compare((bitLength - i - 1),1,"1")){
				carry = "1";
				out = "1" + out;
			}
			else if((!in1.compare((bitLength - i - 1),1,"0") && !in2.compare((bitLength - i - 1),1,"1")) || (!in1.compare((bitLength - i - 1),1,"1") && !in2.compare((bitLength - i - 1),1,"0"))){
				carry = "1";
				out = "0" + out;
			}
			else{
				carry = "0";
				out = "1" + out;
			}			
		}
	}
	if(!carry.compare("1"))
		out = "1" + out;
	helpLength = out.length() % 4;
	if(helpLength)
		for(int i=0;i<(4 - helpLength);i++)
			out = "0" + out;
			
	return out;
}

string SHA256::modAddition(string in1, string in2){
	string sum, out;
	in1 = hexToBits(in1);
	in2 = hexToBits(in2);
	sum = bitAddition(in1, in2);
	sum = bitsToHex(sum);
	if(sum.length() > 8){
		for(int i=0;i<8;i++)
			out = sum.at(sum.length() - i - 1) + out;	
	}
	else if(sum.length() < 8){
		for(int i=0;i<8-sum.length();i++){
			out = "0" + out;
		}
	}
	else
		out = sum;
	return out;
}
