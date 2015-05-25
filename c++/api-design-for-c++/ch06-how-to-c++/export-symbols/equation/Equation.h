/**
	@file	
*/

#ifndef INCLUDE_APIDES_EQUATION_EQUATION_H
#define INCLUDE_APIDES_EQUATION_EQUATION_H

#include <dll-export.h>

#include <memory>
#include <vector>

namespace equation {

DLL_PUBLIC
class Equation {
	class Impl;
	std::unique_ptr<Equation::Impl> impl;

public:
	
};

} // namespace equation

#endif //INCLUDE_APIDES_EQUATION_EQUATION_H
