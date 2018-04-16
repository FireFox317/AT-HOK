/*
 * security.cpp
 *
 *  Created on: Apr 12, 2018
 *      Author: Niek & Ivo
 */

#include "security.h"

security::security()
{

}

security::~security()
{

}

void security::setInfo(std::string NAME, std::string IP, int PORT, std::string GROUP)
{
	myName = NAME;
	myIP = IP;
	port = PORT;
	group = GROUP;
}

void security::setEncriptedMessage(std::string message)
{
	encriptedMessage = message;
}

std::string security::getEncriptedMessage()
{
	return encriptedMessage;
}

void security::generateKeyPair()
{
	CryptoPP::AutoSeededRandomPool rng;
	CryptoPP::InvertibleRSAFunction privateKey;
	privateKey.Initialize(rng, 1024);
	CryptoPP::Base64Encoder privateKeySink(new CryptoPP::FileSink("/home/niek/Documents/myPrivateKey.txt"));
	privateKey.DEREncode(privateKeySink);
	privateKeySink.MessageEnd();

	CryptoPP::RSAFunction publicKey(privateKey);
	CryptoPP::Base64Encoder publicKeySink(new CryptoPP::FileSink("/home/niek/Documents/myPublicKey.txt"));
	publicKey.DEREncode(publicKeySink);
	publicKeySink.MessageEnd();
}

void security::generateSessionKey()
{
	std::cout << "Het lukte :)  -Dora" << std::endl;

	CryptoPP::AutoSeededRandomPool rnd;
	CryptoPP::SecByteBlock iv(CryptoPP::AES::BLOCKSIZE);
	CryptoPP::SecByteBlock key (0x00, CryptoPP::AES::DEFAULT_KEYLENGTH);
	rnd.GenerateBlock(key, key.size());
	std::string sessionKey = std::string(reinterpret_cast<const char*>(key.data()));
	std::cout << "session key = " << sessionKey << std::endl;

	//encryption
	message = "hoiIvoIkHebGeenZinIn Linux.";
	CryptoPP::byte bytemessage[] = "hoiIvoIkHebGeenZinIn Linux.";
	CryptoPP::CFB_Mode<CryptoPP::AES>::Encryption cfbEncryption(key, key.size(), iv);
	cfbEncryption.ProcessData(bytemessage, bytemessage, message.size());
	std::cout << "bytemessage = " << bytemessage << std::endl;

	//decryption
	CryptoPP::CFB_Mode<CryptoPP::AES>::Decryption cfbDecryption(key, key.size(), iv);
	cfbDecryption.ProcessData(bytemessage, bytemessage, message.size());
	std::cout << "Decrypted = " << bytemessage << std::endl;
}

//void security::encriptMessage()
//{
//	message = "hoiIvoIkHebGeenZinIn Linux.";
//	CryptoPP::byte bytemessage[] = "hoiIvoIkHebGeenZinIn Linux.";
//	CryptoPP::CFB_Mode<CryptoPP::AES>::Encryption cfbEncryption(key, key.size(), iv);
//	cfbEncryption.ProcessData(bytemessage, bytemessage, message.size());
//	std::cout << bytemessage << std::endl;
//}
//
//void security::encriptMessage()
//{
//	CryptoPP::CFB_Mode<CryptoPP::AES>::Decryption cfbDecryption(key, key.size(), iv);
//	cfbDecryption(bytemessage, bytemessage, message.size());
//}

void security::receiverHandshake()
{
	encriptedMessage = encriptedMessage.substr(5, encriptedMessage.size());
	std::string receivedPublicKey = encriptedMessage.substr(0, encriptedMessage.find("/endofkey/"));
	encriptedMessage = encriptedMessage.substr((encriptedMessage.find("/endofkey/") + 10), encriptedMessage.size());

	std::string receivedTimestamp = encriptedMessage.substr(0, 19);

	std::ofstream file("/home/niek/Documents/receivedPublicKey.txt");
	file << receivedPublicKey;
	file.close();

	message.clear();

	decodeMessage();

	if (message == receivedTimestamp)
	{
		char accept;
		while (accept != 'y' && accept != 'n')
		{
			std::cout << "System: Accept a new connection? (y/n)" << std::endl;
			std::cin >> accept;
			if (accept == 'n')
			{
				std::cout << "System: Connection refused." << std::endl;
				return;
			}
			else if ( accept == 'y')
			{
				std::cout << "Sytem: Creating secure connection..." << std::endl;
			}
			else
			{
				std::cout << "System: Invalid input." << std::endl;
			}
		}
	}

	generateKeyPair();

	message.clear();
	encriptedMessage.clear();
	int64_t timestamp = std::chrono::system_clock::now().time_since_epoch().count();
	message = std::to_string(timestamp);
	encodeMessage();

	std::string publicKey;
	CryptoPP::FileSource("/home/niek/Documents/myPublicKey.txt", true, new CryptoPP::StringSink(publicKey));

	encriptedMessage.insert(0, "HandResp/" + publicKey + "/endofkey/");
	encriptedMessage.append("/data/" + receivedTimestamp);

	sendPacket(myIP, port, group, encriptedMessage);
}

void security::senderHandshake()
{
	generateSessionKey();
	return;
	int64_t timestamp = std::chrono::system_clock::now().time_since_epoch().count();
	message = std::to_string(timestamp);

	encodeMessage();

	std::string publicKey;
	CryptoPP::FileSource("/home/niek/Documents/myPublicKey.txt", true, new CryptoPP::StringSink(publicKey));

	encriptedMessage.insert(0, "Hand/" + publicKey + "/endofkey/");

	std::cout << encriptedMessage << std::endl;

	sendPacket(myIP, port, group, encriptedMessage);

	message.clear();
	encriptedMessage.clear();

	BlockingQueue< std::string > q;
	std::thread receiver(receivePacket, myIP, port, group, &q);
	while (1)
	{
		encriptedMessage = q.pop();
		if (encriptedMessage.substr(0, 9) == "HandResp/")
		{
			break;
		}
	}
	receiver.detach();
	encriptedMessage = encriptedMessage.substr(9, encriptedMessage.size());

	std::string receivedPublicKey = encriptedMessage.substr(0, encriptedMessage.find("/endofkey/"));
	encriptedMessage = encriptedMessage.substr((encriptedMessage.find("/endofkey/") + 10), encriptedMessage.size());

	std::string receivedTimestamp = encriptedMessage.substr(0, 19);

	std::ofstream file("/home/niek/Documents/receivedPublicKey.txt");
	file << receivedPublicKey;
	file.close();

	std::string returnedTimestamp = encriptedMessage.substr(encriptedMessage.find("/data/") + 6, encriptedMessage.size());
	encriptedMessage = encriptedMessage.substr(0, encriptedMessage.find("/data/"));

	message.clear();

	decodeMessage();

	if (message == receivedTimestamp && std::to_string(timestamp) == returnedTimestamp)
	{
		generateSessionKey();
	}
}

void security::decodeMessage()
{
	CryptoPP::ByteQueue bytes;
	CryptoPP::FileSource file("/home/niek/Documents/receivedPublicKey.txt", true, new CryptoPP::Base64Decoder);
	file.TransferTo(bytes);
	bytes.MessageEnd();
	CryptoPP::RSA::PublicKey publicKey;
	publicKey.Load(bytes);

	CryptoPP::RSASS<CryptoPP::PSSR, CryptoPP::SHA1>::Verifier verifier(publicKey);

	CryptoPP::StringSource ss2(encriptedMessage, true,
				new CryptoPP::SignatureVerificationFilter(
								verifier,
								new CryptoPP::StringSink(message),
								CryptoPP::SignatureVerificationFilter::THROW_EXCEPTION | CryptoPP::SignatureVerificationFilter::PUT_MESSAGE
								)
						);
}

void security::encodeMessage()
{
	CryptoPP::AutoSeededRandomPool rng;

	CryptoPP::ByteQueue bytes;
	CryptoPP::FileSource file("/home/niek/Documents/myPrivateKey.txt", true, new CryptoPP::Base64Decoder);
	file.TransferTo(bytes);
	bytes.MessageEnd();
	CryptoPP::RSA::PrivateKey privateKey;
	privateKey.Load(bytes);

	CryptoPP::RSASS<CryptoPP::PSSR, CryptoPP::SHA1>::Signer signer(privateKey);

	CryptoPP::StringSource ss1(message, true,
			new CryptoPP::SignerFilter(rng, signer,
							new CryptoPP::StringSink(encriptedMessage),
							true
							)
					);
}
