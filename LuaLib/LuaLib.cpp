
#include "LuaSrc\lua.hpp"

int __declspec(dllexport) MyModuleName(lua_State* L) { 
	return 1;
}