#include "mt.h"

#include <Windows.h>

using namespace std;

namespace choco{
namespace mt{	

	error initialize(){
		ConvertThreadToFiber(0);

		return errorno::none;
	}
	void cleanup(){
		ConvertFiberToThread();
	}
};};