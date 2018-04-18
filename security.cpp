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
	CryptoPP::AutoSeededRandomPool rnd;
	CryptoPP::SecByteBlock key (0x00, CryptoPP::AES::DEFAULT_KEYLENGTH);
	rnd.GenerateBlock(key, key.size());
	std::string sessionKey = std::string(reinterpret_cast<const char*>(key.data()));
	std::vector<std::string> destination = {"user", sessionKey};
	keyTable.push_back(destination);

	std::cout << sessionKey << std::endl << std::endl;

	//encriptMessage();
	//decriptMessage();
}

void security::encriptMessage()
{
	message = "Hallo allemaal, wat fijn dat je er bent... :)D)D))Dkjkfda hoi hoi ho ih oinklfd;ak;lfdsan k;dsaj kfjdsakjk djkf jsak djfkla ;jk;lfdsa jkflds want een onzin";
	CryptoPP::byte bytemessage[message.size()];
	std::cout << "message = " << message << std::endl << std::endl;
	for (unsigned i = 0; i < message.size(); i++)
	{
		bytemessage[i] = message[i];
	}

	CryptoPP::SecByteBlock iv(CryptoPP::AES::BLOCKSIZE);
	ivtemp = std::string(reinterpret_cast<const char*>(iv.data()));
	std::string sessionKey = keyTable[0][1];

	CryptoPP::StringSource ss(sessionKey, true, new CryptoPP::HexEncoder(new CryptoPP::StringSink(sessionKey)));
	const CryptoPP::byte* result = (const CryptoPP::byte*) sessionKey.data();
	CryptoPP::SecByteBlock key(result, CryptoPP::AES::DEFAULT_KEYLENGTH);

	CryptoPP::CFB_Mode<CryptoPP::AES>::Encryption cfbEncryption(key, key.size(), iv);
	cfbEncryption.ProcessData(bytemessage, bytemessage, message.size());

	encriptedMessage = std::string(reinterpret_cast<const char*>(bytemessage));
	std::cout << "encriptedMessage = " << encriptedMessage << std::endl << std::endl;
}

void security::decriptMessage()
{
	CryptoPP::byte bytemessage[encriptedMessage.size()];
	for (unsigned i = 0; i < encriptedMessage.size(); i++)
	{
		bytemessage[i] = encriptedMessage[i];
	}

	std::string sessionKey = keyTable[0][1];
	CryptoPP::StringSource ss(sessionKey, true, new CryptoPP::HexEncoder(new CryptoPP::StringSink(sessionKey)));
	const CryptoPP::byte* result = (const CryptoPP::byte*) sessionKey.data();
	CryptoPP::SecByteBlock key(result, CryptoPP::AES::DEFAULT_KEYLENGTH);
	CryptoPP::StringSource ss2(ivtemp, true, new CryptoPP::HexEncoder(new CryptoPP::StringSink(ivtemp)));
	const CryptoPP::byte* result2 = (const CryptoPP::byte*) (ivtemp.substr(0, 6)).data();
	CryptoPP::SecByteBlock iv(result2, ivtemp.size());

	std::string divtemp = std::string(reinterpret_cast<const char*>(iv.data()));

	CryptoPP::CFB_Mode<CryptoPP::AES>::Decryption cfbDecryption(key, key.size(), iv);
	cfbDecryption.ProcessData(bytemessage, bytemessage, encriptedMessage.size());
	message = std::string(reinterpret_cast<const char*>(bytemessage));
	std::cout << "decriptedMessage = " << message << std::endl << std::endl;
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
	CryptoPP::FileSource("/home/niek/Documents/myPublicKey.txt", true, new CryptoPP::StringSink(publicKey));

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

	std::ofstream file("/home/niek/Documents/receivedPublicKey.txt");
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
