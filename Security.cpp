/*
 * Security.cpp
 *
 *  Created on: Apr 18, 2018
 *      Author: timon
 */

#include "Security.h"

#include <cryptopp/rsa.h>
#include <cryptopp/osrng.h>
#include <cryptopp/base64.h>
#include <cryptopp/files.h>
#include <cryptopp/pssr.h>
#include <cryptopp/modes.h>
#include <cryptopp/aes.h>
#include <cryptopp/hex.h>

Security::Security() {
	// TODO Auto-generated constructor stub
	keyTable["192.168.5.3"] = "ETM0x1E2mX/bwBaNpzJLCQAAAAAAAAAAQRIAAAAAAAA=";
	keyTable["192.168.5.1"] = "ETM0x1E2mX/bwBaNpzJLCQAAAAAAAAAAQRIAAAAAAAA=";
}

Security::~Security() {
	// TODO Auto-generated destructor stub
}

void Security::encriptMessage(Message& message){
	std::string sessionKey = keyTable[message.getDestinationIP()];
	message.setData(encriptData(message.getData(),sessionKey));
}

void Security::decriptMessage(Message& message){
	std::string sessionKey = keyTable[message.getSourceIP()];
	message.setData(decriptData(message.getData(),sessionKey));
}



void Security::generateSessionKey()
{
	CryptoPP::AutoSeededRandomPool rnd;
	CryptoPP::SecByteBlock key (CryptoPP::AES::DEFAULT_KEYLENGTH);
	rnd.GenerateBlock(key, key.size());

	std::string sessionKey;
	CryptoPP::StringSource(key, sizeof(key), true,
			new CryptoPP::Base64Encoder(new CryptoPP::StringSink(sessionKey)));

	std::cout << "SessionKey: " << sessionKey << std::endl;

}



std::string Security::encriptData(std::string data, std::string key)
{
	std::string encriptedMessage;

	CryptoPP::AutoSeededRandomPool rnd;
	CryptoPP::byte iv[CryptoPP::AES::DEFAULT_BLOCKSIZE];
	rnd.GenerateBlock(iv, sizeof(iv));

	std::string ivString;
	CryptoPP::StringSource(iv, sizeof(iv), true,
			new CryptoPP::Base64Encoder(new CryptoPP::StringSink(ivString)));

	CryptoPP::byte sessionKeyByte[CryptoPP::AES::DEFAULT_KEYLENGTH];
	CryptoPP::StringSource(key, true,
				new CryptoPP::Base64Decoder(new CryptoPP::ArraySink(sessionKeyByte, CryptoPP::AES::DEFAULT_KEYLENGTH)));

	CryptoPP::CFB_Mode<CryptoPP::AES>::Encryption cfbEncryption;
	cfbEncryption.SetKeyWithIV(sessionKeyByte, sizeof(sessionKeyByte), iv);
	CryptoPP::StringSource ss1( data, true,
	        new CryptoPP::StreamTransformationFilter( cfbEncryption,
	            new CryptoPP::Base64Encoder( new CryptoPP::StringSink( encriptedMessage ))
	        ) // StreamTransformationFilter
	    ); // StringSource

	std::cout << "Encrypted message: " << encriptedMessage << std::endl;

	return ivString + "/endIV/" + encriptedMessage;
}

std::string Security::decriptData(std::string data, std::string key)
{
	std::string ivString = data.substr(0, data.find("/endIV/"));
	std::string encriptedMessage = data.substr(data.find("/endIV/") + 7, data.size() );

	CryptoPP::byte sessionKeyByte[CryptoPP::AES::DEFAULT_KEYLENGTH];
	CryptoPP::StringSource(key, true,
			new CryptoPP::Base64Decoder(new CryptoPP::ArraySink(sessionKeyByte, CryptoPP::AES::DEFAULT_KEYLENGTH)));

	CryptoPP::byte ivByte[CryptoPP::AES::DEFAULT_BLOCKSIZE];
	CryptoPP::StringSource(ivString, true,
			new CryptoPP::Base64Decoder(new CryptoPP::ArraySink(ivByte, CryptoPP::AES::DEFAULT_KEYLENGTH)));

	CryptoPP::CFB_Mode<CryptoPP::AES>::Decryption cfbDecryption;
	cfbDecryption.SetKeyWithIV(sessionKeyByte, sizeof(sessionKeyByte), ivByte);

	std::string decoded;
	CryptoPP::StringSource ss(encriptedMessage, true,
	    new CryptoPP::Base64Decoder(
	        new CryptoPP::StringSink(decoded)
	    ) // Base64Decoder
	); // StringSource

	std::string recovered;
	CryptoPP::StringSource ss3(decoded , true,
        new CryptoPP::StreamTransformationFilter( cfbDecryption,
            new CryptoPP::StringSink( recovered )
        ) // StreamTransformationFilter
    ); // StringSource

	return recovered;
//	std::cout << "Message: " << recovered << std::endl;

}


Security chatsecurity;

