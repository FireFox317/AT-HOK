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

#include "Socket/Sender.h"

Security::Security() {
	// TODO Auto-generated constructor stub

}

Security::~Security() {
	// TODO Auto-generated destructor stub
}

void Security::generateKeyPair(){
	std::vector<std::string> ipAddresses = {"192.168.5.1","192.168.5.2","192.168.5.3","192.168.5.4"};
	for(auto ip: ipAddresses){
		CryptoPP::AutoSeededRandomPool rng;
			CryptoPP::InvertibleRSAFunction params;
			params.GenerateRandomWithKeySize(rng, 3072);

			CryptoPP::RSA::PrivateKey privateKey1(params);
			CryptoPP::RSA::PublicKey publicKey1(params);

			std::string privateKeytoPrint;
			std::string publicKeytoPrint;

			CryptoPP::Base64Encoder privateKeySink(new CryptoPP::FileSink(std::string("privateKey" + ip + ".txt").c_str()));
			privateKey1.DEREncode(privateKeySink);
			privateKeySink.MessageEnd();

			CryptoPP::Base64Encoder publicKeySink(new CryptoPP::FileSink(std::string("publicKey" + ip + ".txt").c_str()));
			publicKey1.DEREncode(publicKeySink);
			publicKeySink.MessageEnd();


	}

}


void Security::loadKeys(){
	std::vector<std::string> ipAddresses = {"192.168.5.1","192.168.5.2","192.168.5.3","192.168.5.4"};
	for(auto ip : ipAddresses){
		CryptoPP::ByteQueue bytes;
		CryptoPP::FileSource file(std::string("Keys/publicKey" + ip + ".txt").c_str(), true, new CryptoPP::Base64Decoder);
		file.TransferTo(bytes);
		bytes.MessageEnd();
		CryptoPP::RSA::PublicKey publicKey;
		publicKey.Load(bytes);
		publicKeyList[ip] = publicKey;
	}

	CryptoPP::ByteQueue bytes;
	CryptoPP::FileSource file(std::string("Keys/privateKey" + sourceIP + ".txt").c_str(), true, new CryptoPP::Base64Decoder);
	file.TransferTo(bytes);
	bytes.MessageEnd();
	privateKey.Load(bytes);
}

void Security::setupConnection(std::string ip){
	{
		if(sessionKeyList.find(ip) != sessionKeyList.end()){
			return;
		}
		using namespace CryptoPP;

		AutoSeededRandomPool rng;
		RSAES_OAEP_SHA_Encryptor encryptor(publicKeyList[ip]);

		CryptoPP::SecByteBlock key (CryptoPP::AES::DEFAULT_KEYLENGTH);
		rng.GenerateBlock(key, key.size());

		std::string sessionKey;
		CryptoPP::StringSource(key, sizeof(key), true,
				new CryptoPP::StringSink(sessionKey));

		std::cout << "SessionKey: " << sessionKey << std::endl;
		sessionKeyList[ip] = sessionKey;

		std::string cipherSessionKey;

		StringSource ss1(sessionKey, true,
			new PK_EncryptorFilter(rng, encryptor,
				new StringSink(cipherSessionKey)
		   ) // PK_EncryptorFilter
		); // StringSource

 		////////////////////////////////////////////////
		// Sign and Encode
		RSASSA_PKCS1v15_SHA_Signer signer(privateKey);

		std::string to_send;

		StringSource ss2(cipherSessionKey, true,
			new SignerFilter(rng, signer,
				new Base64Encoder(new StringSink(to_send)), true
		   ) // SignerFilter
		); // StringSource

		std::cout << "To send: " << to_send << std::endl;
		Sender::sendMessage(Message(ip,"SessionKey:" + to_send));
	}
}


void Security::acceptConnection(Message message){
	{
		using namespace CryptoPP;

		AutoSeededRandomPool rng;
		////////////////////////////////////////////////
		// Verify and Recover
		RSASSA_PKCS1v15_SHA_Verifier verifier(publicKeyList[message.getSourceIP()]);

		std::string decoded;
		StringSource ss(message.getData().substr(11,message.getData().size()), true,
			new Base64Decoder(
				new StringSink(decoded)
			) // Base64Decoder
		); // StringSource

		std::string encryptedSessionKey;

		StringSource ss3(decoded, true,
			new SignatureVerificationFilter(
				verifier, new StringSink(encryptedSessionKey),
				SignatureVerificationFilter::THROW_EXCEPTION | SignatureVerificationFilter::PUT_MESSAGE
		   ) // SignatureVerificationFilter
		); // StringSource

		RSAES_OAEP_SHA_Decryptor decryptor(privateKey);

		std::string sessionKey;
		StringSource ss4(encryptedSessionKey, true,
			new PK_DecryptorFilter(rng, decryptor,
				new StringSink(sessionKey)
		   ) // PK_DecryptorFilter
		); // StringSource

		std::cout << "SessionKey: " << sessionKey << std::endl;
		sessionKeyList[message.getSourceIP()] = sessionKey;
	}
}

void Security::encriptMessage(Message& message){
	std::string sessionKey = sessionKeyList[message.getDestinationIP()];
	message.setData(encriptData(message.getData(),sessionKey));
}

void Security::decriptMessage(Message& message){
	std::string sessionKey = sessionKeyList[message.getSourceIP()];
	message.setData(decriptData(message.getData(),sessionKey));
}

void Security::endConnection(){
	Sender::sendMessage(Message(MULTIGROUP,"/LEAVING/"));
}

void Security::deleteEntry(std::string ip){
	sessionKeyList.erase(sessionKeyList.find(ip));
}


std::string Security::encriptData(std::string data, std::string key)
{
	std::string encriptedMessage;

	CryptoPP::AutoSeededRandomPool rnd;
	CryptoPP::byte iv[CryptoPP::AES::DEFAULT_BLOCKSIZE];
	rnd.GenerateBlock(iv, sizeof(iv));

	std::string ivString;
	CryptoPP::StringSource(iv, sizeof(iv), true,
			new CryptoPP::StringSink(ivString));

	CryptoPP::byte sessionKeyByte[CryptoPP::AES::DEFAULT_KEYLENGTH];
	CryptoPP::StringSource(key, true,
				new CryptoPP::ArraySink(sessionKeyByte, CryptoPP::AES::DEFAULT_KEYLENGTH));

	CryptoPP::CFB_Mode<CryptoPP::AES>::Encryption cfbEncryption;
	cfbEncryption.SetKeyWithIV(sessionKeyByte, sizeof(sessionKeyByte), iv);
	CryptoPP::StringSource ss1( data, true,
	        new CryptoPP::StreamTransformationFilter( cfbEncryption,
	             new CryptoPP::StringSink( encriptedMessage )
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
			new CryptoPP::ArraySink(sessionKeyByte, CryptoPP::AES::DEFAULT_KEYLENGTH));

	CryptoPP::byte ivByte[CryptoPP::AES::DEFAULT_BLOCKSIZE];
	CryptoPP::StringSource(ivString, true,
			new CryptoPP::ArraySink(ivByte, CryptoPP::AES::DEFAULT_KEYLENGTH));

	CryptoPP::CFB_Mode<CryptoPP::AES>::Decryption cfbDecryption;
	cfbDecryption.SetKeyWithIV(sessionKeyByte, sizeof(sessionKeyByte), ivByte);


	std::string recovered;
	CryptoPP::StringSource ss3(encriptedMessage , true,
        new CryptoPP::StreamTransformationFilter( cfbDecryption,
            new CryptoPP::StringSink( recovered )
        ) // StreamTransformationFilter
    ); // StringSource


//	std::cout << "Message: " << recovered << std::endl;
	return recovered;


}


Security chatsecurity;

