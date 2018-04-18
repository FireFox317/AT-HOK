/*
 * security.cpp
 *
 *  Created on: Apr 12, 2018
 *      Author: timon & Ivo
 */

#include "security.h"

#include "Message.h"
#include "Socket/Sender.h"



security::security()
{
	keyTable["192.168.5.3"] = "ETM0x1E2mX/bwBaNpzJLCQAAAAAAAAAAQRIAAAAAAAA=";
	keyTable["192.168.5.1"] = "ETM0x1E2mX/bwBaNpzJLCQAAAAAAAAAAQRIAAAAAAAA=";
}

security::~security()
{

}

//void security::setInfo(std::string NAME, std::string ip, int _port, std::string _group)
//{
//	myName = NAME;
//	myIP = ip;
//	port = _port;
//	group = _group;
//}
//
//void security::setEncriptedMessage(std::string message)
//{
//	encriptedMessage = message;
//}
//
//std::string security::getEncriptedMessage()
//{
//	return encriptedMessage;
//}

//void security::generateKeyPair()
//{
//	CryptoPP::AutoSeededRandomPool rng;
//	CryptoPP::InvertibleRSAFunction privateKey;
//	privateKey.Initialize(rng, 1024);
//	CryptoPP::Base64Encoder privateKeySink(new CryptoPP::FileSink("/home/timon/Documents/myPrivateKey.txt"));
//	privateKey.DEREncode(privateKeySink);
//	privateKeySink.MessageEnd();
//
//	CryptoPP::RSAFunction publicKey(privateKey);
//	CryptoPP::Base64Encoder publicKeySink(new CryptoPP::FileSink("/home/timon/Documents/myPublicKey.txt"));
//	publicKey.DEREncode(publicKeySink);
//	publicKeySink.MessageEnd();
//}

void security::generateSessionKey()
{
	CryptoPP::AutoSeededRandomPool rnd;
	CryptoPP::SecByteBlock key (CryptoPP::AES::DEFAULT_KEYLENGTH);
	rnd.GenerateBlock(key, key.size());

	std::string sessionKey;
	CryptoPP::StringSource(key, sizeof(key), true,
			new CryptoPP::Base64Encoder(new CryptoPP::StringSink(sessionKey)));

	std::cout << "SessionKey: " << sessionKey << std::endl;

	//keyTable["test"] = sessionKey;

	//encriptMessage();
	//decriptMessage();
}



std::string security::encriptData(std::string data, std::string key)
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

std::string security::decriptData(std::string data, std::string key)
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

//void security::receiverHandshake(std::string data)
//{
//	flag = false;
//	encriptedMessage = data;
//	encriptedMessage = encriptedMessage.substr(5, encriptedMessage.size());
//	std::string receivedPublicKey = encriptedMessage.substr(0, encriptedMessage.find("/endofkey/"));
//	encriptedMessage = encriptedMessage.substr((encriptedMessage.find("/endofkey/") + 10), encriptedMessage.size());
//
//	std::string receivedTimestamp = encriptedMessage.substr(0, 19);
//
//	std::ofstream file("/home/timon/Documents/receivedPublicKey.txt");
//	file << receivedPublicKey;
//	file.close();
//
//	message.clear();
//
//	decodeMessage();
//
//	if (message == receivedTimestamp)
//	{
//		char accept;
//		while (accept != 'y' && accept != 'n')
//		{
//			std::cout << "System: Accept a new connection? (y/n)" << std::endl;
//			std::cin >> accept;
//			if (accept == 'n')
//			{
//				std::cout << "System: Connection refused." << std::endl;
//				return;
//			}
//			else if ( accept == 'y')
//			{
//				std::cout << "Sytem: Creating secure connection..." << std::endl;
//			}
//			else
//			{
//				std::cout << "System: Invalid input." << std::endl;
//			}
//		}
//	}
//
//	generateKeyPair();
//
//	message.clear();
//	encriptedMessage.clear();
//	int64_t timestamp = std::chrono::system_clock::now().time_since_epoch().count();
//	message = std::to_string(timestamp);
//	encodeMessage();
//
//	std::string publicKey;
//	CryptoPP::FileSource("/home/timon/Documents/myPublicKey.txt", true, new CryptoPP::StringSink(publicKey));
//
//	encriptedMessage.insert(0, "HandResp/" + publicKey + "/endofkey/");
//	encriptedMessage.append("/data/" + receivedTimestamp);
//
//
//	Sender::sendMessage(Message(group, encriptedMessage));
//
//	//waiting for sessionkey
//}
//
//void security::senderHandshake()
//{
//	flag = false;
//	int64_t timestamp = std::chrono::system_clock::now().time_since_epoch().count();
//	message = std::to_string(timestamp);
//
//	encodeMessage();
//
//	std::string publicKey;
//	CryptoPP::FileSource("/home/timon/Documents/myPublicKey.txt", true, new CryptoPP::StringSink(publicKey));
//
//	encriptedMessage.insert(0, "Hand/" + publicKey + "/endofkey/");
//
//	std::cout << encriptedMessage << std::endl;
//
//	Sender::sendMessage(Message("192.168.5.3",encriptedMessage));
//
//	message.clear();
//	encriptedMessage.clear();
//
//	std::cout << "Waiting for response" << std::endl;
//
//	while(!flag){
//
//	}
//
////	BlockingQueue< std::string > q;
////	std::thread receiver(receivePacket, myIP, port, group, &q);
////	while (1)
////	{
////		encriptedMessage = q.pop();
////		if (encriptedMessage.substr(0, 9) == "HandResp/")
////		{
////			break;
////		}
////	}
////	closeSocket();
////	receiver.detach();
//
//	encriptedMessage = encriptedMessage.substr(9, encriptedMessage.size());
//
//	std::string receivedPublicKey = encriptedMessage.substr(0, encriptedMessage.find("/endofkey/"));
//	encriptedMessage = encriptedMessage.substr((encriptedMessage.find("/endofkey/") + 10), encriptedMessage.size());
//
//	std::string receivedTimestamp = encriptedMessage.substr(0, 19);
//
//	std::ofstream file("/home/timon/Documents/receivedPublicKey.txt");
//	file << receivedPublicKey;
//	file.close();
//
//	std::string returnedTimestamp = encriptedMessage.substr(encriptedMessage.find("/data/") + 6, encriptedMessage.size());
//	encriptedMessage = encriptedMessage.substr(0, encriptedMessage.find("/data/"));
//
//	message.clear();
//
//
//	decodeMessage();
//
//	if (message == receivedTimestamp && std::to_string(timestamp) == returnedTimestamp)
//	{
//		std::cout << "Correct message received, generating session key..." << std::endl;
//		generateSessionKey();
//		message = keyTable["test"];
//
//		CryptoPP::ByteQueue bytes;
//		CryptoPP::FileSource file("/home/timon/Documents/receivedPublicKey.txt", true, new CryptoPP::Base64Decoder);
//		file.TransferTo(bytes);
//		bytes.MessageEnd();
//		CryptoPP::RSA::PublicKey publicKey;
//		publicKey.Load(bytes);
//
//		encriptedMessage.clear();
//
//		CryptoPP::AutoSeededRandomPool rng;
//
//		CryptoPP::RSAES_OAEP_SHA_Encryptor e(publicKey);
//
//		CryptoPP::StringSource ss1(message, true,
//		    new CryptoPP::PK_EncryptorFilter(rng, e,
//		        new CryptoPP::StringSink(encriptedMessage)
//		   ) // PK_EncryptorFilter
//		); // StringSource
//
//		std::cout << "message = " << message << std::endl << std::endl;
//		std::cout << "encriptedMessage = " << encriptedMessage << std::endl;
//
//		Sender::sendMessage(Message("192.168.5.3","SessionKey/" + encriptedMessage));
//	}
//	else
//	{
//		std::cout << "Received an unexpected message, breaking the connection..." << std::endl;
//	}
//
//
//}
//
//void security::decodeMessage()
//{
//	CryptoPP::ByteQueue bytes;
//	CryptoPP::FileSource file("/home/timon/Documents/receivedPublicKey.txt", true, new CryptoPP::Base64Decoder);
//	file.TransferTo(bytes);
//	bytes.MessageEnd();
//	CryptoPP::RSA::PublicKey publicKey;
//	publicKey.Load(bytes);
//
//	std::cout << encriptedMessage << std::endl;
//
//	try
//	{
//		CryptoPP::RSASS<CryptoPP::PSSR, CryptoPP::SHA1>::Verifier verifier(publicKey);
//
//		CryptoPP::StringSource ss2(encriptedMessage, true,
//					new CryptoPP::SignatureVerificationFilter(
//									verifier,
//									new CryptoPP::StringSink(message),
//									CryptoPP::SignatureVerificationFilter::THROW_EXCEPTION | CryptoPP::SignatureVerificationFilter::PUT_MESSAGE
//									)
//							);
//	}
//	catch (std::exception const& e)
//	{
//		std::cout << "Exception: " << e.what() << std::endl;
//	}
//}
//
//void security::encodeMessage()
//{
//	CryptoPP::AutoSeededRandomPool rng;
//
//	CryptoPP::ByteQueue bytes;
//	CryptoPP::FileSource file("/home/timon/Documents/myPrivateKey.txt", true, new CryptoPP::Base64Decoder);
//	file.TransferTo(bytes);
//	bytes.MessageEnd();
//	CryptoPP::RSA::PrivateKey privateKey;
//	privateKey.Load(bytes);
//
//	CryptoPP::RSASS<CryptoPP::PSSR, CryptoPP::SHA1>::Signer signer(privateKey);
//
//	CryptoPP::StringSource ss1(message, true,
//			new CryptoPP::SignerFilter(rng, signer,
//							new CryptoPP::StringSink(encriptedMessage),
//							true
//							)
//					);
//}

void security::encriptMessage(Message& message){
	std::string sessionKey = keyTable[message.getDestinationIP()];
	message.setData(encriptData(message.getData(),sessionKey));
}

void security::decriptMessage(Message& message){
	std::string sessionKey = keyTable[message.getSourceIP()];
	message.setData(decriptData(message.getData(),sessionKey));
}

security chatsecurity;

