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

}

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

	encriptedMessage.insert(0, publicKey + "/endofkey/");
	encriptedMessage.append("/" + receivedTimestamp);

	sendPacket(myIP, port, group, encriptedMessage);
}

void security::senderHandshake()
{
	int64_t timestamp = std::chrono::system_clock::now().time_since_epoch().count();
	message = std::to_string(timestamp);
	std::cout << message << std::endl;

	encodeMessage();

	std::string publicKey;
	CryptoPP::FileSource("/home/niek/Documents/myPublicKey.txt", true, new CryptoPP::StringSink(publicKey));

	encriptedMessage.insert(0, "Hand/" + publicKey + "/endofkey/");

	sendPacket(myIP, port, group, encriptedMessage);

	receiverHandshake();


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
