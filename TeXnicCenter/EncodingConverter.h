#pragma once

#include <vector>
#include <memory>
#include <stdexcept>
#include <cstddef>

void ANSItoUTF8(const char* text, std::size_t n, std::vector<char>& data, UINT codepage = ::GetACP());
void ANSItoUTF16(const char* text, std::size_t n, std::vector<char>& data, UINT codepage = ::GetACP(), bool little_endian = true);
void ANSItoUTF16(const char* text, std::size_t n, std::vector<wchar_t>& data, UINT codepage = ::GetACP(), bool little_endian = true);
void UTF8toANSI(const char* text, std::size_t n, std::vector<char>& data, UINT codepage = ::GetACP(), char dfault = '?', bool throw_on_invalid_sequence = false);
void UTF8toUTF16(const char* text, std::size_t n, std::vector<char>& data, bool little_endian = true);
void UTF8toUTF16(const char* text, std::size_t n, std::vector<wchar_t>& data, bool little_endian = true);
void UTF16toANSI(const char* text, std::size_t n, std::vector<char>& data, UINT codepage = ::GetACP(), bool little_endian = true);
void UTF16toUTF8(const wchar_t* text, std::size_t n, std::vector<char>& data, bool little_endian = true);
void UTF16toUTF8(const char* text, std::size_t n, std::vector<char>& data, bool little_endian = true);
void UTF32toUTF8(const char* text, std::size_t n, std::vector<char>& data, bool little_endian = true);
void UTF8toUTF32(const char* text, std::size_t n, std::vector<char>& data, bool little_endian = true);
void UTF32toUTF16(const char* text, std::size_t n, std::vector<char>& data, bool in_little_endian = true, bool out_little_endian = true);
void UTF32toUTF16(const char* text, std::size_t n, std::vector<wchar_t>& data, bool in_little_endian = true, bool out_little_endian = true);
void FromUTF8(const char* tocode, const char* text, std::size_t n, std::vector<char>& data);
void FromUTF16(const char* tocode, const wchar_t* text, std::size_t n, std::vector<char>& data, bool little_endian = true);
void FromANSI(const char* tocode, const char* text, std::size_t n, std::vector<char>& data, UINT codepage = ::GetACP());
void ToUTF16(const char* fromcode, const char* text, std::size_t n, std::vector<wchar_t>& data, bool little_endian = true);
bool GetUTF8CharBytes(unsigned char byte, std::size_t& n);
bool IsEncodingAvailable(const char* code);

class EncodingConverterError :
	public std::runtime_error
{
public:
	explicit EncodingConverterError(const std::string& message = "")
	: runtime_error(message)
	{
	}
};

class InvalidEncodingConverterArgument :
	public std::invalid_argument
{
public:
	explicit InvalidEncodingConverterArgument(const std::string& message = "")
	: invalid_argument(message)
	{
	}
};

/// Represents an exception thrown when an invalid character is encountered.
class InvalidCharacterEncoutered :
	public EncodingConverterError
{
public:
	explicit InvalidCharacterEncoutered(const std::string& message = "")
	: EncodingConverterError(message)
	{
	}
};

typedef std::vector<std::vector<CString> > EncodingCodeContainer;

class EncodingConverter
{
	struct Impl;
	
	// Non copyable
	EncodingConverter(const EncodingConverter&);
	EncodingConverter& operator=(const EncodingConverter&);

	Impl* impl_;
	static std::auto_ptr<EncodingCodeContainer> codes_;

public:
	EncodingConverter();
	~EncodingConverter();

	/// Initializes the instance by opening an encoding converter
	/// with the specified \a tocode and \a fromcode. If the initialization
	/// fails, \ref EncodingConverterError is thrown.
	///
	/// \param tocode	Target encoding.
	/// \param fromcode Source encoding.
	EncodingConverter(const char* tocode, const char* fromcode);

	bool Open(const char* tocode, const char* fromcode);
	bool IsOpen() const;
	void Close();
	std::size_t Convert(const char** in, std::size_t* in_left, char** out, std::size_t* out_left);
	void Reset();
	bool IsConversionTrivial() const;
	void SetTransliterate(bool set = true);
	bool GetTransliterate() const;
	bool GetDiscardIllegalSequence() const;
	void SetDiscardIllegalSequence(bool discard = true);

	static const EncodingCodeContainer& GetSupportedCodes();
};
