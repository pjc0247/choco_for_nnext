#pragma once

#include "choco/error/errno.h"

#include <queue>

namespace choco{
namespace mem{

	class pool{
	public:
		pool();
		virtual ~pool();

		error initialize();
		void cleanup();

		void *malloc(size_t size);
		void free(void *ptr);

	private:
		std::queue<void *> m32, m64,
			m128, m256, m512;
	};
};};