#pragma once
#ifndef __LIB_HPP_2015_11_03
#define __LIB_HPP_2015_11_03

#pragma comment(lib, "comctl32.lib")   

# ifdef _DEBUG
#	pragma comment(lib, "../Lib/DuiLib2_U_StaticD.lib")
# else
#	pragma comment(lib, "../Lib/DuiLib2_U_Static.lib")
# endif

#endif