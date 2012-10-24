#ifndef __CAFOX_HPP
#define __CAFOX_HPP

#include "cabase.hpp"
#include "spfox.hpp"
#include "smattpl.hpp"
#include "cellfox.hpp"


//
//	CAFox : CA Discreto.
//


class CAFox : public CABase
	{
	protected:
		SpecieFox * Sp;
		SpecieFox * ActualSp;

		simplmat <CellFox> C;
		CellFox ActualCell ;

		int ActualSpNum;
		int Season;
		unsigned Stage;
		unsigned long MeanDistDisp;
		unsigned long CantDispersers;


	public:

	CAFox( unsigned numSp, int dimX, int dimY, int rr=0 ) : CABase(numSp,dimX,dimY,rr) {Init(numSp,dimX,dimY,rr); };
	CAFox() {};
	~CAFox() {};
	void Init(unsigned numSp, int dimX, int dimY, int rr=0 );
/*
	int & Elem(int x, int y, unsigned O) {
								if( x>=DimX )
									x=0;
								if( y>=DimY )
									y=0;
								if( x<0 )
									x=DimX-1;
								if( y<0 )
									y=DimY-1;

								return C(x,y).Elem(O);
								}
*/

	void Dispersal(int x,int y);
//	int HistDispersal(int xx, int yy, float angl);
	void Colonize(int xx,int yy);
	void Growth(int x,int y);
	void Survival(int x,int y);
	virtual int  ReadParms( int &nEval, int &nRuns, char * file );
	void Reset();
	void Evaluate();
	void SetN( unsigned n ){ N= n;};
	void SetA( unsigned n ){ A= n;};
	void SetT( unsigned n ){ T= n;};

	void EvalCell(int x,int y);
	void EmptyPrevious(){ for(int x=0;x<DimX;x++)
									for(int y=0;y<DimY;y++)
										C(x,y).Elem(A)=0;
									};

	unsigned AgeToStage(unsigned age);

	void PrintGraph();
	void InitGraph();

	int ReadSeed( char * file);
	int SaveSeed( char * fname);
	void ReadSetSeed(char * file);
	void RandomSetSeed(int sp,unsigned age, int no, int minX);

	void PrintDensStage();
	void PrintMap();
	void PrintMapXY();
	void PrintMapXYSp();


	};


#endif
