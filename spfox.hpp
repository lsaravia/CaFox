#ifndef __SPFOX_HPP
#define __SPFOX_HPP

#include "spbase.hpp"
#include "smattpl.hpp"

class SpecieFox : public SpecieBase
	{
    protected:


	public:

	unsigned NumberStages;
    unsigned NumberSites;

	SpecieFox(){NumberStages=0; NumberSites=0;};
	SpecieFox(unsigned nStages, unsigned nSites) { Init(nStages, nSites); };

	~SpecieFox();
	SpecieFox & operator=(const SpecieFox& src);
	SpecieFox(const SpecieFox &src);

	virtual void Init(unsigned nStages, unsigned nSites);
	virtual void Init();
	virtual int Scan(char * buff);

	simplmat <unsigned> BreedSize;
	simplmat <float> PMortality;
	simplmat <float> PFecundity;
	simplmat <unsigned> DispersalDistance;
	simplmat <float> PDispersal;
	simplmat <float> PHunt;
	unsigned MinDistEstablishment;
	unsigned DistEvaluationInterval;
	unsigned BreedDispersal;
	unsigned DispMinNumEmptyCells;
	unsigned DistHistDispersal;
	unsigned MaxAge;
	};

#endif
