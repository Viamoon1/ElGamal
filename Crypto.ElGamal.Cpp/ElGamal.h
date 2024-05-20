#pragma once

#include "Algorithms.h"

#define _CRT_SECURE_NO_WARNINGS

typedef std::pair<Int, Int> Group;

class ElGamal{
public:
	static std::pair<std::pair<Int, Group>, Int> keyGen(unsigned int bits);
	static std::pair<std::pair<Int, Group>, Int> keyGenOfP(Int bits);
	static std::pair<Int, Int> encrypt(std::pair<Int, Group> public_key, Int message);
	static Int decrypt(Int privat_key,std::pair<Int, Group> public_key, std::pair<Int, Int> crypt_message);
};