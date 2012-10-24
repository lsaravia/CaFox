#ifndef __CELLFOX_HPP
#define __CELLFOX_HPP
#include "fortify.h"

class CellFox
	{
	public:
	int NumSpecie[2];
	unsigned char Age;
	unsigned char Site;
	unsigned char AlreadyReproduced;

	CellFox() { NumSpecie[0]=0;  NumSpecie[1]=0; Age=0; Site=0; AlreadyReproduced=0; };
	virtual int & Elem( int idx ) { return NumSpecie[idx]; };
	const CellFox & Elem( int nsp0, int nsp1 ) { NumSpecie[0]=nsp0;NumSpecie[1]=nsp1; return *this; };

	CellFox & operator=(const CellFox &src){ NumSpecie[0] = src.NumSpecie[0];
												NumSpecie[1] = src.NumSpecie[1];
												Age = src.Age; Site = src.Site;
												AlreadyReproduced = src.AlreadyReproduced;
												return *this;};

	CellFox(const CellFox &src){ NumSpecie[0] = src.NumSpecie[0];
								NumSpecie[1] = src.NumSpecie[1];
								Age = src.Age; Site = src.Site;
								AlreadyReproduced = src.AlreadyReproduced;
								};

	void CopySp(int N,int A) { NumSpecie[N] = NumSpecie[A]; };

	};


#endif __CELLFOX_HPP
