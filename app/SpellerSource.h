#pragma once

class Speller;

class SpellerSource
{
public:
	virtual ~SpellerSource();

	/**
	 * @brief Gets the speller.
	 * 
	 * @return Initialized and configured spell checker.
	 */
	virtual Speller* GetSpeller() = 0;
};
