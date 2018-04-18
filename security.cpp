/*
 * security.cpp
 *
 *  Created on: Apr 12, 2018
 *      Author: timon & Ivo
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
	CryptoPP::Base64Encoder privateKeySink(new CryptoPP::FileSink("/home/timon/Documents/myPrivateKey.txt"));
	privateKey.DEREncode(privateKeySink);
	privateKeySink.MessageEnd();

	CryptoPP::RSAFunction publicKey(privateKey);
	CryptoPP::Base64Encoder publicKeySink(new CryptoPP::FileSink("/home/timon/Documents/myPublicKey.txt"));
	publicKey.DEREncode(publicKeySink);
	publicKeySink.MessageEnd();
}

void security::generateSessionKey()
{
	CryptoPP::AutoSeededRandomPool rnd;
	CryptoPP::SecByteBlock key (CryptoPP::AES::DEFAULT_KEYLENGTH);
	rnd.GenerateBlock(key, key.size());

	std::string sessionKey;
	CryptoPP::StringSource(key, sizeof(key), true,
			new CryptoPP::Base64Encoder(new CryptoPP::StringSink(sessionKey)));

	std::cout << "SessionKey: " << sessionKey << std::endl;

	keyTable["test"] = sessionKey;

	//encriptMessage();
	//decriptMessage();
}



void security::encriptMessage()
{
	message = "Werkt t nou ofwa";

	CryptoPP::AutoSeededRandomPool rnd;
	CryptoPP::byte iv[CryptoPP::AES::DEFAULT_BLOCKSIZE];
	rnd.GenerateBlock(iv, sizeof(iv));

	CryptoPP::StringSource(iv, sizeof(iv), true,
			new CryptoPP::Base64Encoder(new CryptoPP::StringSink(ivtemp)));

	std::cout << "IV: " << ivtemp << std::endl;

	CryptoPP::byte sessionKeyByte[CryptoPP::AES::DEFAULT_KEYLENGTH];
	CryptoPP::StringSource(keyTable["test"], true,
				new CryptoPP::Base64Decoder(new CryptoPP::ArraySink(sessionKeyByte, CryptoPP::AES::DEFAULT_KEYLENGTH)));

	CryptoPP::CFB_Mode<CryptoPP::AES>::Encryption cfbEncryption;
	cfbEncryption.SetKeyWithIV(sessionKeyByte, sizeof(sessionKeyByte), iv);
	CryptoPP::StringSource ss1( message, true,
	        new CryptoPP::StreamTransformationFilter( cfbEncryption,
	            new CryptoPP::Base64Encoder( new CryptoPP::StringSink( encriptedMessage ))
	        ) // StreamTransformationFilter
	    ); // StringSource

	std::cout << "Encrypted message: " << encriptedMessage << std::endl;

}

void security::decriptMessage()
{

	CryptoPP::byte sessionKeyByte[CryptoPP::AES::DEFAULT_KEYLENGTH];
	CryptoPP::StringSource(keyTable["test"], true,
			new CryptoPP::Base64Decoder(new CryptoPP::ArraySink(sessionKeyByte, CryptoPP::AES::DEFAULT_KEYLENGTH)));

	CryptoPP::byte ivByte[CryptoPP::AES::DEFAULT_BLOCKSIZE];
	CryptoPP::StringSource(ivtemp, true,
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

	std::cout << "Message: " << recovered << std::endl;

}

void security::receiverHandshake()
{
	encriptedMessage = encriptedMessage.substr(5, encriptedMessage.size());
	std::string receivedPublicKey = encriptedMessage.substr(0, encriptedMessage.find("/endofkey/"));
	encriptedMessage = encriptedMessage.substr((encriptedMessage.find("/endofkey/") + 10), encriptedMessage.size());

	std::string receivedTimestamp = encriptedMessage.substr(0, 19);

	std::ofstream file("/home/timon/Documents/receivedPublicKey.txt");
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
	CryptoPP::FileSource("/home/timon/Documents/myPublicKey.txt", true, new CryptoPP::StringSink(publicKey));

	encriptedMessage.insert(0, "HandResp/" + publicKey + "/endofkey/");
	encriptedMessage.append("/data/" + receivedTimestamp);

	sendPacket(myIP, port, group, encriptedMessage);

	//waiting for sessionkey
}

void security::senderHandshake()
{
	int64_t timestamp = std::chrono::system_clock::now().time_since_epoch().count();
	message = std::to_string(timestamp);

	encodeMessage();

	std::string publicKey;
	CryptoPP::FileSource("/home/timon/Documents/myPublicKey.txt", true, new CryptoPP::StringSink(publicKey));

	encriptedMessage.insert(0, "Hand/" + publicKey + "/endofkey/");

	std::cout << encriptedMessage << std::endl;

	sendPacket(myIP, port, group, encriptedMessage);

	message.clear();
	encriptedMessage.clear();

	std::cout << "Waiting for response" << std::endl;
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

	std::ofstream file("/home/timon/Documents/receivedPublicKey.txt");
	file << receivedPublicKey;
	file.close();

	std::string returnedTimestamp = encriptedMessage.substr(encriptedMessage.find("/data/") + 6, encriptedMessage.size());
	encriptedMessage = encriptedMessage.substr(0, encriptedMessage.find("/data/"));

	message.clear();

	decodeMessage();

	if (message == receivedTimestamp && std::to_string(timestamp) == returnedTimestamp)
	{
		std::cout << "check" << std::endl;
		generateSessionKey();
		message.clear();
		message = keyTable[0][1];

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
		std::cout << "message = " << message << std::endl;
		std::cout << "encriptedMessage = " << encriptedMessage << std::endl;
	}
	else
	{
		std::cout << "Received an unexpected message..." << std::endl;
	}
}

void security::decodeMessage()
{
	CryptoPP::ByteQueue bytes;
	CryptoPP::FileSource file("/home/timon/Documents/receivedPublicKey.txt", true, new CryptoPP::Base64Decoder);
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
	CryptoPP::FileSource file("/home/timon/Documents/myPrivateKey.txt", true, new CryptoPP::Base64Decoder);
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

