#ifndef _BASEAFF_HXX_
#define _BASEAFF_HXX_

class Affix
{
public: 
	virtual ~Affix() {}

protected:
	int numents;
	struct affentry *first;
#if 0
	virtual char* check(...) {};
#endif
};

#endif
