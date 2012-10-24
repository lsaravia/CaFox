#include <strstrea.h>
#include "spfox.hpp"
#include "fortify.h"

SpecieFox & SpecieFox::operator=(const SpecieFox& src)
	{
	DispMinNumEmptyCells = src.DispMinNumEmptyCells;
	DistHistDispersal    = src.DistHistDispersal;
	PFecundity = src.PFecundity;
	PDispersal = src.PDispersal;
	DispersalDistance = src.DispersalDistance;
	PHunt = src.PHunt;
	NumberStages = src.NumberStages;
	NumberSites = src.NumberSites ;
	MinDistEstablishment = src.MinDistEstablishment;
	DistEvaluationInterval = src.DistEvaluationInterval;
	BreedSize = src.BreedSize;
	BreedDispersal = src.BreedDispersal;
	MaxAge = src.MaxAge;
	}

SpecieFox::SpecieFox(const SpecieFox &src)
	{
	DispMinNumEmptyCells = src.DispMinNumEmptyCells;
	DistHistDispersal    = src.DistHistDispersal;
	PMortality = src.PMortality;
	PFecundity = src.PFecundity;
	PDispersal = src.PDispersal;
	DispersalDistance = src.DispersalDistance;
	PHunt = src.PHunt;
	NumberStages = src.NumberStages;
	NumberSites = src.NumberSites ;
	MinDistEstablishment = src.MinDistEstablishment;
	DistEvaluationInterval = src.DistEvaluationInterval;
	BreedSize = src.BreedSize;
	BreedDispersal = src.BreedDispersal;
	MaxAge = src.MaxAge;
	}


void SpecieFox::Init(unsigned nStages, unsigned nSites)
	{
	NumberStages = nStages;
	NumberSites  = nSites;
	Init();
	}

void SpecieFox::Init()
	{
	PMortality.resize(NumberStages,NumberSites);
	PFecundity.resize(NumberStages,NumberSites);
	DispersalDistance.resize(NumberStages,NumberSites);
	PDispersal.resize(NumberStages,NumberSites);
	PHunt.resize(NumberStages,NumberSites);
	BreedSize.resize(NumberStages,NumberSites);
	}

SpecieFox::~SpecieFox()
	{
	}

int SpecieFox::Scan(char * buff)
	{
	int sp=0,i,j;
	istrstream ins(buff,strlen(buff));
	ins >> sp >> NumberStages >> NumberSites >> MinDistEstablishment
		>> DistEvaluationInterval >> DistHistDispersal >> DispMinNumEmptyCells
		>> BreedDispersal >> MaxAge;

	Init();
	for(i=0; i<NumberSites; i++)
		for(j=0; j<NumberStages; j++)
			ins >> BreedSize(j,i);

	for(i=0; i<NumberSites; i++)
		for(j=0; j<NumberStages; j++)
			ins >> PMortality(j,i);

	for(i=0; i<NumberSites; i++)
		for(j=0; j<NumberStages; j++)
			ins >> PFecundity(j,i);

	for(i=0; i<NumberSites; i++)
		for(j=0; j<NumberStages; j++)
			ins >> DispersalDistance(j,i);

	for(i=0; i<NumberSites; i++)
		for(j=0; j<NumberStages; j++)
			ins >> PDispersal(j,i);

	for(i=0; i<NumberSites; i++)
		for(j=0; j<NumberStages; j++)
			ins >> PHunt(j,i);

	return sp;
	}

