#ifndef CRYPTOPP_OAEP_H
#define CRYPTOPP_OAEP_H

#include "pubkey.h"
#include "sha.h"

NAMESPACE_BEGIN(CryptoPP)

//! <a href="http://www.weidai.com/scan-mirror/ca.html#cem_OAEP-MGF1">EME-OAEP</a>, for use with RSAES
class CRYPTOPP_DLL OAEP_Base : public PK_EncryptionMessageEncodingMethod
{
public:
	bool ParameterSupported(const char *name) const {return strcmp(name, Name::EncodingParameters()) == 0;}
	unsigned int MaxUnpaddedLength(unsigned int paddedLength) const;
	void Pad(RandomNumberGenerator &rng, const byte *raw, unsigned int inputLength, byte *padded, unsigned int paddedLength, const NameValuePairs &parameters) const;
	DecodingResult Unpad(const byte *padded, unsigned int paddedLength, byte *raw, const NameValuePairs &parameters) const;

protected:
	virtual unsigned int DigestSize() const =0;
	virtual HashTransformation * NewHash() const =0;
	virtual MaskGeneratingFunction * NewMGF() const =0;
};

template <class H, class MGF=P1363_MGF1>
class OAEP : public OAEP_Base, public EncryptionStandard
{
public:
	static std::string StaticAlgorithmName() {return std::string("OAEP-") + MGF::StaticAlgorithmName() + "(" + H::StaticAlgorithmName() + ")";}
	typedef OAEP<H, MGF> EncryptionMessageEncodingMethod;

protected:
	unsigned int DigestSize() const {return H::DIGESTSIZE;}
	HashTransformation * NewHash() const {return new H;}
	MaskGeneratingFunction * NewMGF() const {return new MGF;}
};

CRYPTOPP_DLL_TEMPLATE_CLASS OAEP<SHA>;

NAMESPACE_END

#endif