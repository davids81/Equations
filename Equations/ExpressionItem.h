#pragma once

#include <string>
#include <list>

using std::string;

class ExpressionItem
{
public:
	ExpressionItem();
	~ExpressionItem();

	void* m_compiledLua;
	int m_compiledLuaLen;

	string m_Name;
	double m_value;
	string m_eqUsrInput;

	//
	void UpdateReferenced(string &VarName, double Value);
	void Evaluate();

};

