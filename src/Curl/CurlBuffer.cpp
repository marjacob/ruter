#include "Curl/CurlBuffer.hpp"

using std::size_t;

namespace Curl
{
	CurlBuffer::CurlBuffer()
	{
		this->BufferCapacity = 0;
		this->BufferSize = 0;
	}
	
	CurlBuffer::~CurlBuffer()
	{
		
	}
}
