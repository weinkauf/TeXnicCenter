#pragma once

class CBuildView;
class CParseOutputView;
class CGrepView;

class OutputViewVisitor
{
public:
	virtual void Accept(CBuildView*) = 0;
	virtual void Accept(CParseOutputView*) = 0;
	virtual void Accept(CGrepView*) = 0;

	virtual ~OutputViewVisitor() { }
};