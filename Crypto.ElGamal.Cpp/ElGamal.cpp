#include "ElGamal.h"
#include <boost/math/common_factor_rt.hpp>
#include <boost/multiprecision/random.hpp>

typedef std::pair<Int, Int> Group;

/**
Генерація пари private_key і public_key
*/
std::pair<std::pair<Int, Group>, Int> ElGamal::keyGen(unsigned int bits){
	Algorithms::seedRNG();
	// Генерація простого числа p
	Int p = 0;
	switch (bits)
	{
		case 4: p = Algorithms::randomSafePrime<4>(40);
			break;
		case 8: p = Algorithms::randomSafePrime<8>(40);
			break;
		case 16: p = Algorithms::randomSafePrime<16>(40);
			break;
		case 32: p = Algorithms::randomSafePrime<32>(40);
			break;
		case 64: p = Algorithms::randomSafePrime<64>(20);
			break;
		case 128: p = Algorithms::randomSafePrime<128>(10);
			break;
		default: p = Algorithms::randomSafePrime<64>(20);
			break;
	}
	//Створення генератору g
	Int g = 0;											
	while (true){
		// генерація 1 < g < p-1
		g = Algorithms::getRandomNumber(2, p-1);
		//Перевірка чи лежить g в полі Zp 
		if (boost::math::gcd(g, p-1) == 1)
		{
			//Перевірка g^(p-1)  mod p = 1;                                           
			if (powm(g, p-1, p) == 1)		
				break;
		}			
	}

	//Створення приватного ключа
	Int private_key = Algorithms::getRandomNumber(2, p - 1);

	// Утворення публічного ключа
	Int y = powm(g, private_key, p);
	Group P_G = std::make_pair(p, g);
	std::pair<Int, Group> public_key = std::make_pair(y, P_G);


#ifdef _DEBUG
	std::cout << "Safeprime p = " << p << std::endl;
	std::cout << "Generator g = " << g << std::endl;
	std::cout << "Public key y = " << y << std::endl;
	std::cout << "Private key x = " << private_key << std::endl;
	
#endif
	return std::make_pair(public_key, private_key);
}

std::pair<std::pair<Int, Group>, Int> ElGamal::keyGenOfP(Int p) {
	Algorithms::seedRNG();

	// Генерація простого числа p
	mt19937 gen2(clock());
	if (!miller_rabin_test(p, 40, gen2)) {
		std::cout << "Number is not prime! " << std::endl;
		Group G_P = std::make_pair(0, 0);
		Int t = 0;
		std::pair<Int, Group> hh= std::make_pair(t, G_P);
		return std::make_pair(hh, t);
	}

	//Створення генератору g
	Int g = 0;
	while (true) {
		// генерація 1 < g < p-1
		g = Algorithms::getRandomNumber(2, p - 1);

		//Перевірка чи лежить g в полі Zp 
		if (boost::math::gcd(g, p - 1) == 1)
		{
			//Перевірка g^(p-1)  mod p = 1;                                           
			if (powm(g, p - 1, p) == 1)
				break;
		}
	}

	//Створення приватного ключа
	Int private_key = Algorithms::getRandomNumber(2, p - 1);

	// Утворення публічного ключа
	Int y = powm(g, private_key, p);
	Group P_G = std::make_pair(p, g);
	std::pair<Int, Group> public_key = std::make_pair(y, P_G);


#ifdef _DEBUG
	std::cout << "Safeprime p = " << p << std::endl;
	std::cout << "Generator g = " << g << std::endl;
	std::cout << "Public key y = " << y << std::endl;
	std::cout << "Private key x = " << private_key << std::endl;

#endif
	return std::make_pair(public_key, private_key);
}

std::pair<Int, Int> ElGamal::encrypt(std::pair<Int, Group> public_key, Int message){
	Int y = public_key.first;

	Group G_P = public_key.second;	
	Int p = G_P.first;
	Int g = G_P.second;

	//Генерація випалкового числа k
	Int k = 0;
	k = Algorithms::getRandomNumber(2, p - 1);

	//Розрахунок a по формулі a=g^k  mod p;                                           
	Int a = powm(g, k, p);
	
	//Розрахунок b по формулі b=y^k M mod p;
	Int b = powm(y, k, p);
	b =(b * message)%p;
	//Повернення шифротексту (а,b)
	return std::make_pair(a, b);	
}


Int ElGamal::decrypt(Int private_key,std::pair<Int, Group> public_key, std::pair<Int,Int> crypt_message){
	
	Int a = crypt_message.first;
	Int b = crypt_message.second;	
	
	Int y = public_key.first;
	Group G = public_key.second;
	Int p = G.first;
	Int g = G.second;

	
	//Int ainv = Algorithms::modInverse(B, p);

	//Int m = powm(B, ainv, p);
	//Розшифрування шифротексту m = ba^(p-1-x) mod p                                           
	Int m = powm(a, p-1-private_key, p);
	m = (m * b) % p;

	return m;
	
}
