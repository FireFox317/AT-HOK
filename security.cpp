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

void security::generateKeyPair()
{
	CryptoPP::AutoSeededRandomPool rng;
	CryptoPP::InvertibleRSAFunction privateKey;
	privateKey.Initialize(rng, 1024);
	CryptoPP::Base64Encoder privateKeySink(new CryptoPP::FileSink("/home/niek/Documents/privateKey.txt"));
	privateKey.DEREncode(privateKeySink);
	privateKeySink.MessageEnd();

	CryptoPP::RSAFunction publicKey(privateKey);
	CryptoPP::Base64Encoder publicKeySink(new CryptoPP::FileSink("/home/niek/Documents/publicKey.txt"));
	publicKey.DEREncode(publicKeySink);
	publicKeySink.MessageEnd();
}

void security::generateSessionKey()
{

}

void security::handshake()
{
	int64_t timestamp = std::chrono::system_clock::now().time_since_epoch().count();
	message = std::to_string(timestamp);
	std::cout << message << std::endl;

	encodeMessage();

	std::string publicKey;
	CryptoPP::FileSource("/home/niek/Documents/publicKey.txt", true, new CryptoPP::StringSink(publicKey));

	decodeMessage();

	//sendPacket(myIP, port, group, message);


//	std::string finalMessage;
//	CryptoPP::StringStore SSM(finalMessage);
//	SSM.Put ((CryptoPP::byte const*) message.data(), message.size());
//	std::string finalSig;
//	CryptoPP::StringStore SSS(finalSig);
//	SSS.Put (sbbSignature, sbbSignature.size());
//
//	std::cout << "finalMessage = " << finalMessage << std::endl << "finalSig = " << finalSig << std::endl;

}

void security::decodeMessage()
{
	CryptoPP::ByteQueue bytes;
	CryptoPP::FileSource file("/home/niek/Documents/publicKey.txt", true, new CryptoPP::Base64Decoder);
	file.TransferTo(bytes);
	bytes.MessageEnd();
	CryptoPP::RSA::PublicKey publicKey;
	publicKey.Load(bytes);

	CryptoPP::RSASS<CryptoPP::PSSR, CryptoPP::SHA1>::Verifier verifier(publicKey);

	CryptoPP::StringSource ss2(signature, true,
				new CryptoPP::SignatureVerificationFilter(
								verifier,
								new CryptoPP::StringSink(recovered),
								CryptoPP::SignatureVerificationFilter::THROW_EXCEPTION | CryptoPP::SignatureVerificationFilter::PUT_MESSAGE
								)
						);
	std::cout << recovered << std::endl;
}

void security::encodeMessage()
{
	CryptoPP::AutoSeededRandomPool rng;

	CryptoPP::ByteQueue bytes;
	CryptoPP::FileSource file("/home/niek/Documents/privateKey.txt", true, new CryptoPP::Base64Decoder);
	file.TransferTo(bytes);
	bytes.MessageEnd();
	CryptoPP::RSA::PrivateKey privateKey;
	privateKey.Load(bytes);

	CryptoPP::RSASS<CryptoPP::PSSR, CryptoPP::SHA1>::Signer signer(privateKey);

	CryptoPP::StringSource ss1(message, true,
			new CryptoPP::SignerFilter(rng, signer,
							new CryptoPP::StringSink(signature),
							true
							)
					);
	std::cout << "signature = " << signature << std::endl;
}
