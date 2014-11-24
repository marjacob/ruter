#ifndef __H_D1D0EBB7324C4D07B7C97E2EBD126AF8__
#define __H_D1D0EBB7324C4D07B7C97E2EBD126AF8__

#include <cstddef>
#include <vector>

namespace Curl
{
	class CurlBuffer {
	public:
		std::size_t BufferCapacity;
		std::size_t BufferSize;
		std::vector<char> Buffer;
		
		CurlBuffer();
		~CurlBuffer();
	};
}

#endif
