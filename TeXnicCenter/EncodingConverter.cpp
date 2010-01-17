#include "stdafx.h"
#include "EncodingConverter.h"

#include <sstream>
#include <cstdlib>

#include "../libiconv/include/iconv.h"

bool GetUTF8CharBytes(unsigned char byte, std::size_t& n)
{
	bool result = true;

	if ((byte & 0xf0) == 0xf0)
		n = 4;
	else if ((byte & 0xf0) == 0xe0)
		n = 3;
	else if ((byte & 0xe0) == 0xc0)
		n = 2;
	else if ((byte & 0x80) == 0x00)
		n = 1;
	else
		result = false;

	return result;
}

namespace {
	const iconv_t invalid_handle = reinterpret_cast<iconv_t>(-1);
	const std::size_t ConversionBufferSize = 256;

	template<class ChIn, class ChOut>
	struct ConversionTraits;

	template<class ChOut>
	struct ConversionTraits<char,ChOut>
	{
		static unsigned char ToUnsigned(char ch)
		{
			return static_cast<unsigned char>(ch);
		}

		static const wchar_t* NextChar(const wchar_t* p, std::size_t& s)
		{
			--s;
			return ++p;
		}

		static const char* NextChar(const char* p, std::size_t& s)
		{
			--s;
			return ++p;
		}
	};

	template<class ChOut>
	struct ConversionTraits<wchar_t,ChOut>
	{
		static wchar_t ToUnsigned(wchar_t ch)
		{
			return ch;
		}

		static const wchar_t* NextChar(const wchar_t* p, std::size_t& s)
		{
			--s;
			return ++p;
		}

		static const char* NextChar(const char* p, std::size_t& s)
		{
			--s;
			return ++p;
		}
	};

	template<class ChIn, class ChOut>
	struct UTF8ConversionTraits;

	template<class ChOut>
	struct UTF8ConversionTraits<char,ChOut>
	{
		static unsigned char ToUnsigned(char ch)
		{
			return static_cast<unsigned char>(ch);
		}

		static const char* NextChar(const char* p, std::size_t& s)
		{
			std::size_t advance;
			const unsigned char byte = static_cast<unsigned char>(*p);

			if (!GetUTF8CharBytes(byte,advance)) { // p doesn't point to the beginning of a character
				int skipped = 0;

				// Skip the bytes that are part of a multi byte sequence
				for ( ; s && (static_cast<unsigned char>(*p) & 0xc0) == 0x80; ) {
					++p; --s; ++skipped;
				}

				if (skipped > 0) // Some bytes skipped, done.
					advance = 0;
				else
					advance = 1; // No bytes skipped, increment to prevent an infinite loop
			}

			if (advance > s) {
				if (s >= 1)
					advance = 1;
				else {
					s = 0; p = 0;
				}
			}
			
			s -= advance;
			p += advance;

			return p;
		}
	};

	template<class Tr, class ChIn, class ChOut>
	inline bool Convert(const ChIn* text, std::size_t n, std::vector<ChOut>& data, EncodingConverter& converter, ChOut dfault = 0, bool throw_on_invalid_sequence = false)
	{
		converter.IsConversionTrivial();
		data.reserve(static_cast<std::size_t>(n * sizeof(ChOut) * 1.5)); // Reserve some space
		char buffer[ConversionBufferSize];

		bool stop = false;
		const std::size_t inbytes = n * sizeof(ChIn);
		std::size_t inbytesleft = inbytes, outbytesleft, result, size;

		const char* in = reinterpret_cast<const char*>(text);
		char *out;

		bool copy;

		while (inbytesleft && !stop) {
			ASSERT(inbytesleft <= inbytes);
			out = buffer; outbytesleft = sizeof(buffer);	

			result = converter.Convert(&in,&inbytesleft,&out,&outbytesleft);
			// Stream conversion may fail although some characters will be converted
			// make sure converted characters are copied to the buffer
			size = sizeof(buffer) - outbytesleft;
			copy = size > 0; //result != static_cast<std::size_t>(-1);			

			if (!copy) {
				if (errno == EILSEQ) { // Invalid sequence encountered
					std::size_t ilseqbytes = sizeof(ChIn);
					ASSERT(inbytesleft % sizeof(ChIn) == 0);

					if (throw_on_invalid_sequence)
						throw InvalidCharacterEncoutered();

					while (inbytesleft && ilseqbytes--) {
						data.push_back(dfault ? dfault : static_cast<ChOut>(Tr::ToUnsigned(*in)));
						in = Tr::NextChar(in,inbytesleft);
					}

					if (!in) // Stream might be corrupted
						stop = true;
				}
				else {
					if (errno == E2BIG && size > 0)
						copy = true; // Not enough room, copy only converted bytes
					else
						stop = true; // No chance, stop conversion
				}
			}
			
			if (copy)
				data.insert(data.end(),reinterpret_cast<const ChOut*>(buffer),
					reinterpret_cast<const ChOut*>(buffer + size));
		}

		return !stop; // stop only set, if an error has occurred
	}

	const std::string FormatEndianCode(const char* base, bool little_endian)
	{
		std::string code(base);

		if (little_endian)
			code += "LE";
		else
			code += "BE";

		return code;
	}

	const std::string FormatCodePage(UINT codepage)
	{
		if (!::IsValidCodePage(codepage))
			throw std::invalid_argument("Invalid code page");

		std::ostringstream oss;
		oss << "CP" << codepage;

		return oss.str();
	}

	template<class ChOut>
	inline void ConvertANSItoUTF16(const char* text, std::size_t n, std::vector<ChOut>& data, UINT codepage, bool little_endian)
	{
		data.clear();

		if (n > 0) {
			EncodingConverter converter(FormatEndianCode("UTF-16",little_endian).c_str(),
				FormatCodePage(codepage).c_str());
			Convert<ConversionTraits<char,ChOut> >(text,n,data,converter);
		}
	}

	template<class ChOut>
	inline void ConvertUTF8toUTF16(const char* text, std::size_t n, std::vector<ChOut>& data, bool little_endian)
	{
		data.clear();

		if (n > 0) {
			EncodingConverter converter(FormatEndianCode("UTF-16",little_endian).c_str(),"UTF-8");
			Convert<ConversionTraits<char,ChOut> >(text,n,data,converter);
		}
	}

	template<class ChIn>
	inline void ConvertUTF16toUTF8(const ChIn* text, std::size_t n, std::vector<char>& data, bool little_endian)
	{
		data.clear();

		if (n > 0) {
			EncodingConverter converter("UTF-8",FormatEndianCode("UTF-16",little_endian).c_str());
			Convert<ConversionTraits<ChIn,char> >(text,n,data,converter);
		}
	}

	template<class ChOut>
	inline void ConvertUTF32toUTF16(const char* text, std::size_t n, std::vector<ChOut>& data, bool in_little_endian, bool out_little_endian)
	{
		data.clear();

		if (n > 0) {
			EncodingConverter converter(FormatEndianCode("UTF-16",out_little_endian).c_str(),
				FormatEndianCode("UTF-32",in_little_endian).c_str());
			Convert<ConversionTraits<char,ChOut> >(text,n,data,converter);
		}
	}
}

void ANSItoUTF8(const char* text, std::size_t n, std::vector<char>& data, UINT codepage)
{
	data.clear();

	if (n > 0) {
		EncodingConverter converter("UTF-8",FormatCodePage(codepage).c_str());
		Convert<ConversionTraits<char,char> >(text,n,data,converter);
	}
}

void ANSItoUTF16(const char* text, std::size_t n, std::vector<wchar_t>& data, UINT codepage, bool little_endian)
{
	ConvertANSItoUTF16(text,n,data,codepage,little_endian);
}

void ANSItoUTF16(const char* text, std::size_t n, std::vector<char>& data, UINT codepage, bool little_endian)
{
	ConvertANSItoUTF16(text,n,data,codepage,little_endian);
}

void UTF8toUTF16(const char* text, std::size_t n, std::vector<wchar_t>& data, bool little_endian)
{
	ConvertUTF8toUTF16(text,n,data,little_endian);
}

void UTF8toUTF16(const char* text, std::size_t n, std::vector<char>& data, bool little_endian)
{
	ConvertUTF8toUTF16(text,n,data,little_endian);
}

void UTF16toUTF8(const wchar_t* text, std::size_t n, std::vector<char>& data, bool little_endian)
{
	ConvertUTF16toUTF8(text,n,data,little_endian);
}

void UTF16toUTF8(const char* text, std::size_t n, std::vector<char>& data, bool little_endian)
{
	ConvertUTF16toUTF8(text,n,data,little_endian);
}

void UTF8toANSI(const char* text, std::size_t n, std::vector<char>& data, UINT codepage /*= ::GetACP()*/, 
				char dfault /*= '?'*/, bool throw_on_invalid_sequence /*= false*/)
{
	data.clear();

	if (n > 0) {
		EncodingConverter converter(FormatCodePage(codepage).c_str(),"UTF-8");
		Convert<UTF8ConversionTraits<char,char> >(text,n,data,converter,dfault,throw_on_invalid_sequence);
	}
}

void UTF32toUTF8(const char* text, std::size_t n, std::vector<char>& data, bool little_endian)
{
	data.clear();

	if (n > 0) {
		EncodingConverter converter("UTF-8",FormatEndianCode("UTF-32",little_endian).c_str());
		Convert<ConversionTraits<char,char> >(text,n,data,converter);
	}
}

void UTF8toUTF32(const char* text, std::size_t n, std::vector<char>& data, bool little_endian)
{
	data.clear();

	if (n > 0) {
		EncodingConverter converter(FormatEndianCode("UTF-32",little_endian).c_str(),"UTF-8");
		Convert<ConversionTraits<char,char> >(text,n,data,converter);
	}
}

void UTF32toUTF16(const char* text, std::size_t n, std::vector<wchar_t>& data, bool in_little_endian, bool out_little_endian)
{
	ConvertUTF32toUTF16(text,n,data,in_little_endian,out_little_endian);
}

void UTF32toUTF16(const char* text, std::size_t n, std::vector<char>& data, bool in_little_endian, bool out_little_endian)
{
	ConvertUTF32toUTF16(text,n,data,in_little_endian,out_little_endian);
}

void FromUTF8(const char* tocode, const char* text, std::size_t n, std::vector<char>& data)
{
	EncodingConverter converter(tocode,"UTF-8");
	Convert<UTF8ConversionTraits<char,char> >(text,n,data,converter);
}

void ToUTF16(const char* fromcode, const char* text, std::size_t n, std::vector<wchar_t>& data, bool little_endian)
{
	EncodingConverter converter(FormatEndianCode("UTF-16",little_endian).c_str(),fromcode);
	Convert<ConversionTraits<char,wchar_t> >(text,n,data,converter);
}

void FromUTF16(const char* tocode, const wchar_t* text, std::size_t n, std::vector<char>& data, bool little_endian)
{
	EncodingConverter converter(tocode,FormatEndianCode("UTF-16",little_endian).c_str());
	Convert<ConversionTraits<wchar_t,char> >(text,n,data,converter);
}

void FromANSI(const char* tocode, const char* text, std::size_t n, std::vector<char>& data, UINT codepage)
{
	EncodingConverter converter(tocode,FormatCodePage(codepage).c_str());
	Convert<ConversionTraits<char,char> >(text,n,data,converter);
}

bool IsEncodingAvailable(const char* code)
{
	EncodingConverter converter;
	return converter.Open(code,"");
}

void UTF16toANSI(const char* text, std::size_t n, std::vector<char>& data, UINT codepage, bool little_endian)
{
	EncodingConverter converter(FormatCodePage(codepage).c_str(),FormatEndianCode("UTF-16",little_endian).c_str());
	Convert<ConversionTraits<char,char> >(text,n,data,converter);
}

std::auto_ptr<EncodingCodeContainer> EncodingConverter::codes_;

struct EncodingConverter::Impl
{
	Impl(iconv_t cd = invalid_handle)
	: cd(cd)
	{
	}

	iconv_t cd;
};

EncodingConverter::EncodingConverter()
: impl_(0)
{
}

EncodingConverter::EncodingConverter( const char* tocode, const char* fromcode ) 
: impl_(0)
{
	if (!Open(tocode,fromcode))
		throw EncodingConverterError("Invalid encoding");
}

EncodingConverter::~EncodingConverter()
{
	if (IsOpen())
		Close();

	delete impl_;
}

bool EncodingConverter::Open(const char* tocode, const char* fromcode)
{
	iconv_t cd = iconv_open(tocode,fromcode);
	bool result = cd != invalid_handle;

	if (result) {
		if (!impl_)
			impl_ = new Impl(cd);
		else
			impl_->cd = cd;
	}

	return result;
}

bool EncodingConverter::IsOpen() const
{
	return impl_ && impl_->cd != invalid_handle;
}

void EncodingConverter::Close()
{
	if (impl_ && impl_->cd != invalid_handle) {
		iconv_close(impl_->cd);
		impl_->cd = invalid_handle;
	}
}

std::size_t EncodingConverter::Convert(const char** in, std::size_t* in_left, char** out, std::size_t* out_left)
{
	ASSERT(IsOpen());
	return iconv(impl_->cd,in,in_left,out,out_left);
}

void EncodingConverter::Reset()
{
	ASSERT(IsOpen());
	Convert(0,0,0,0);
}

bool EncodingConverter::IsConversionTrivial() const
{
	ASSERT(IsOpen());
	int result;
	VERIFY(iconvctl(impl_->cd,ICONV_TRIVIALP,&result) == 0);

	return result == 1;
}

void EncodingConverter::SetTransliterate( bool set /*= true*/ )
{
	int value = set;
	VERIFY(iconvctl(impl_->cd,ICONV_SET_TRANSLITERATE,&value) == 0);
}

bool EncodingConverter::GetTransliterate() const
{
	ASSERT(IsOpen());

	int result;
	VERIFY(iconvctl(impl_->cd,ICONV_GET_TRANSLITERATE,&result) == 0);

	return result == 1;
}

bool EncodingConverter::GetDiscardIllegalSequence() const
{
	ASSERT(IsOpen());
	int result;

	VERIFY(iconvctl(impl_->cd,ICONV_GET_DISCARD_ILSEQ,&result) == 0);
	return result == 1;
}

void EncodingConverter::SetDiscardIllegalSequence( bool discard /*= true*/ )
{
	ASSERT(IsOpen());
	int value = discard;
	VERIFY(iconvctl(impl_->cd,ICONV_SET_DISCARD_ILSEQ,&value) == 0);
}

namespace {
	int EnumerateCodes(unsigned count, const char* const* codes, void* data)
	{
		EncodingCodeContainer* container = static_cast<EncodingCodeContainer*>(data);
		EncodingCodeContainer::value_type c;

		for ( ; count; --count, ++codes) {
			c.push_back(CString(*codes));
		}

		container->push_back(c);

		return 0;
	}
}

const EncodingCodeContainer& EncodingConverter::GetSupportedCodes()
{
	if (!codes_.get()) {
		codes_.reset(new EncodingCodeContainer());
		iconvlist(EnumerateCodes, codes_.get());
	}

	return *codes_;
}