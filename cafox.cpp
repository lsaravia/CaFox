//
//	CAFox 
//
//	Clase Base : CABase
//
//	Clase Asociada : SpecieFox
//
//
#pragma implementation

#include "cafox.hpp"
#include "fortify.h"
#include <strstrea.h>
#include <iostream.h>
#include <fstream.h>
#include <iomanip.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <conio.h>
#include <ctype.h>

#ifdef __GNUG__
#include <pc.h>
#endif

void CAFox::Init( unsigned numSp, int dimX, int dimY, int rr )
	{
	//if(C==NULL)
	CABase::Init(numSp, dimX, dimY,rr);

	C.resize(dimX,dimY);

	ActualSpNum = 0;
	Season = 0;
	Stage = 0;
	MeanDistDisp=0;
   CantDispersers=0;

	Sp	= new SpecieFox[NumSpecies];

	//ActualSp = Sp[0];
	ActualCell = C(0,0);

	}

//	Lee los parametros para el CA
//	La estructura del archivo es
//
//	Dimension
//	nroEval   :  Nro. de iteraciones del CA
//	nEsp	  :  Nro. de especies
//
//	Spe  est  sur	vgr  dde
//	...  ...  ...	...  ...
//
int CAFox::ReadParms( int &nEval, int &nRuns, char * file )
	{
	FILE *in;
	char buff[255];

	int sp=0;


	if ((in = fopen(file, "rt")) == NULL)
		{
		fprintf(stderr, "Cannot open Parms file.\n");
		return 1;
		}

	fgets(buff,80,in);
	sscanf(buff,"%i %i",&DimX,&DimY);
	fgets(buff,80,in);
	sscanf(buff,"%i %i",&nEval,&nRuns);
	fgets(buff,80,in);
	sscanf(buff,"%i",&NumSpecies);

	Init( NumSpecies, DimX, DimY);

	for(int i=0;i<NumSpecies;i++)
		{
		fgets(buff,255,in);
		sp = Sp[i].Scan(buff);
		if( sp>=NumSpecies || sp!=i )
			{
			fprintf(stderr, "Error reading Parms file.\n");
			return 1;
			}

		}
	return 0;
	}


//	AgeToStage : Pasa la edad en trimestres a Stages
//
//
//
inline unsigned CAFox::AgeToStage(unsigned age)
	{
	if(age <5)
		age =0; 		// Juvenil
	else
		if(age <13)
			age=1;		// Adulto Joven
		else
			age=2;		// Adulto

	return age;
	}


void CAFox::Evaluate()
	{
	Season = (T % 4) + 1 ;
	CABase::Evaluate();

	}

void CAFox::EvalCell(int x,int y)
	{
	ActualSpNum = C(x,y).Elem(A);
	if( ActualSpNum>0 )
		{
		ActualCell = C(x,y);
		ActualSp = &Sp[ActualSpNum-1];	 // Deberia usar aritmetica de punteros
		Stage=AgeToStage( ActualCell.Age );

		Survival(x,y);
		Growth(x,y);
		Dispersal(x,y);
		}
	}

//
// No coloniza si alguna celda adyacente crecio por crec. vegetativo.
// y ocupo el lugar
//
void CAFox::Dispersal(int x,int y)
	{
	if(ActualSpNum>0)
		{
		if(Season == 1 || Season == 2)
			{
			double ang=0.0,pDis=0.0,dis=0.0,rnd=0.0;
			int maxDis=0;
			pDis = ActualSp->PDispersal(Stage,ActualCell.Site);
			if( Rand() < pDis )
				{
				int dx,dy,actDis;
				do {
					while( (rnd = Rand()) == 0.0 );
					actDis = ActualSp->MinDistEstablishment; // Uso este como distancia media
					dis = ceil( - actDis * log( rnd ) );
					maxDis = ActualSp->DispersalDistance(Stage,C(x,y).Site);
					if( dis > maxDis )
						dis = maxDis;
						
	//				rnd = Rand();
					ang = Rand() * Pi2;
					dx = cos( ang ) * dis ;
					dy = sin( ang ) * dis ;
					dx = (x+dx+DimX) % DimX;
					dy = (y+dy+DimY) % DimY;
					}
				while( dx==0 && dy==0 );

				// Se asegura que encuentre un celda vacia en los
				// alrededores de el lugar a donde se disperso
				while(1)
					{
					if( C(dx,dy).Elem(A)==0 )
						if( C(dx,dy).Elem(N)==0 )
							{
							C(dx,dy).Elem(N) = ActualSpNum;
							C(dx,dy).Age = C(x,y).Age;
							C(x,y).Age = 0;
							C(x,y).Elem(0,0);

							actDis = sqrt( (x-dx)*(x-dx) + (y-dy)*(y-dy) );
							MeanDistDisp += actDis;
							CantDispersers++;
							break;
							}
					switch( Rand(4) )
						{
						case 3:
							dx = (dx+1+DimX) % DimX;
							dy = (dy+1+DimY) % DimY;
							break;
						case 0:
							dx = (dx-1+DimX) % DimX;
							dy = (dy+1+DimY) % DimY;
							break;
						case 1:
							dx = (dx-1+DimX) % DimX;
							dy = (dy-1+DimY) % DimY;
							break;
						case 2:
							dx = (dx+1+DimX) % DimX;
							dy = (dy-1+DimY) % DimY;
						}
					}
				}
			}
		}
	}

/*
int CAFox::HistDispersal(int x, int y, float angl)
	{
	int dh= ActualSp->DistHistDispersal,n=0;
	int dx,dy;

	while(dh--)
		{
		dx = x - cos(angl) * (dh+1);
		dy = y - sin(angl) * (dh+1);
		dx = (DimX + dx) % DimX;
		dy = (DimY + dy) % DimY;
		n += C(dx,dy).Elem(A);
		}
	return n;
	}
*/

void CAFox::Growth(int x,int y)
	{
	int dx,dy,dd,dis,bs;

	if(ActualSpNum>0)
		{
		if( ActualCell.AlreadyReproduced>0 && (Season == 1 || Season == 2) )
			{
			C(x,y).AlreadyReproduced = 0;

			double ang=0.0,pDis=0.0,dis=0.0,rnd=0.0,pMort=0;
			int maxDis=0,i=0;
			pMort = ActualSp->PMortality(0,ActualCell.Site);
			pDis = ActualSp->PDispersal(0,ActualCell.Site);
			for(i=ActualCell.AlreadyReproduced; i>0 ;i-- )
			if( Rand() >= pMort )
				{
				int dx,dy,actDis;
				do {
					while( (rnd = Rand()) == 0.0 );
					actDis = ActualSp->MinDistEstablishment; // Uso este como distancia media
					dis = ceil( - actDis * log( rnd ) );
					maxDis = ActualSp->DispersalDistance(Stage,C(x,y).Site);
					if( dis > maxDis )
						dis = maxDis;
						
					ang = Rand() * Pi2;
					dx = cos( ang ) * dis ;
					dy = sin( ang ) * dis ;
					dx = (x+dx+DimX) % DimX;
					dy = (y+dy+DimY) % DimY;
					}
				while( dx==0 && dy==0 );

				// Se asegura que encuentre un celda vacia en los
				// alrededores de el lugar a donde se disperso
				i=ActualSp->BreedDispersal;
				dd= (i==0);
				while(dd)
//				while((i--)>0)
					{
					if( C(dx,dy).Elem(A)==0 )
						if( C(dx,dy).Elem(N)==0 )
							{
							C(dx,dy).Elem(N) = ActualSpNum;
							C(dx,dy).Age = 1;
							actDis = sqrt( (x-dx)*(x-dx) + (y-dy)*(y-dy) );
							MeanDistDisp += actDis;
							CantDispersers++;
							break;
							}
					switch( Rand(4) )
						{
						case 3:
							dx = (dx+1+DimX) % DimX;
							dy = (dy+1+DimY) % DimY;
							break;
						case 0:
							dx = (dx-1+DimX) % DimX;
							dy = (dy+1+DimY) % DimY;
							break;
						case 1:
							dx = (dx-1+DimX) % DimX;
							dy = (dy-1+DimY) % DimY;
							break;
						case 2:
							dx = (dx+1+DimX) % DimX;
							dy = (dy-1+DimY) % DimY;
						}
					}
				}
			}

		if( (Season == 1 && ActualCell.AlreadyReproduced==0)
			|| Season == 4 )
			{
			if( Rand() < ActualSp->PFecundity(Stage,ActualCell.Site ) )
				{
				// Tiene los cachorros que se distribuyen al azar
				// en los alrededores de la madre a una distancia no
				// mayor que BreedDispersal
				//
				dd = ActualSp->BreedDispersal;
				bs = ActualSp->BreedSize(Stage,ActualCell.Site);
				C(x,y).AlreadyReproduced = bs;
				/*
				for(int i=0; i<bs; i++)
					{
					do {
						dx = Rand( dd*2+1 ) - dd;
						dis = sqrt( dd * dd - dx * dx );
						dy = Rand(dis*2+1) - dis ;
						}
					while( dx==0 && dy==0 );

					Colonize(x+dx,y+dy);

					}
				*/
				}
			else
				C(x,y).AlreadyReproduced = 0;
			}
		}
	}

void CAFox::Colonize(int x,int y)
	{
	x = (x+DimX) % DimX;
	y = (y+DimY) % DimY;
	//if( !( xx<0 || yy<0 || xx>=DimX || yy>=DimY ) )
	if( C(x,y).Elem(A)==0 )
		if( C(x,y).Elem(N)==0 )
			{
			C(x,y).Elem(N) = ActualSpNum;
			C(x,y).Age = 0;
			C(x,y).AlreadyReproduced = 0;
			}
	}


void CAFox::Survival(int x, int y)
	{
	//
	// Agregar Edad Maxima !!!!!!!!!!!!!!!!!!
	//
	if(ActualSpNum>0)
		{
		if( ActualCell.Age > ActualSp->MaxAge )
			{
			//C(x,y).Age = 0;
			//C(x,y).AlreadyReproduced = 0;
			ActualSpNum=0;
			}
		else
			{
			float ps;
			ps = ActualSp->PMortality(Stage,ActualCell.Site);
			if( Rand() < ps )
				{
				//C(x,y).Age = 0;
				//C(x,y).AlreadyReproduced = 0;
				//C(x,y).Elem(0,0);
				ActualSpNum=0;
				}
			else
				{
				if(Season == 2 || Season == 3)
					{
					ps = ActualSp->PHunt(Stage,ActualCell.Site);
					if(Rand() < ps)
						ActualSpNum=0;
					else
						{
						C(x,y).CopySp(N,A);
						C(x,y).Age++;
						}
					}
				else
					{
					// C(x,y).Elem(N)= C(x,y).Elem(A);
					C(x,y).CopySp(N,A);
					C(x,y).Age++;
					}
				}
			}
		}
	}




void CAFox::Reset()
	{
	rndSeed= time(NULL);
	r250_init(rndSeed);
	C.fill( ActualCell.Elem(0,0) );
	A = 0;
	N = 1;
	T = 0;
	ActualSpNum = 0;
	MeanDistDisp=0;
    CantDispersers=0;
	}


void CAFox::InitGraph()
	{
	CABase::InitGraph();
	char buff[20];

	for( int i=1; i<=NumSpecies; i++)
		{
		for(int j=1; j<=3; j++)
			{
			sprintf(buff,"%dS%d",i,j);
			GLabel(buff,i+j);
			}
		}
	}

void CAFox::PrintGraph()
	{
	char sa, baseName[9];
	ostrstream name;
	int stage=0;

	for(int i=0; i<DimX; i++)
		{
		for(int j=0;j<DimY;j++)
			{
			//PPix(i,j,C(i,j).Elem(N));
			stage = C(i,j).Elem(N);
			if(stage!=0)
				stage = AgeToStage( C(i,j).Age ) + C(i,j).Elem(N);
			PPix(i,j,stage);
			}
		}
	if( kbhit() )
		{
		sa=toupper(getch());
		if( sa=='S')
			{
			EndGraph();
			cerr << "Tiempo (en trimestres):" << T << endl;
			cerr << "Ingrese nombre BASE de archivo : ";
			cin.width(6);
			cin >> baseName;
			name << baseName << T << ".sed" << ends;
			SaveSeed( name.str() );
			InitGraph();
			}
		else
			getch();
		}
	//getch();
	}


void CAFox::PrintDensStage()
	{
	static int privez=1;
	unsigned long tot=0;  // OJO esto funciona para 1 sp y 2 Sitios!
	int e,a,s, nSites,nStages;

	nStages = Sp[0].NumberStages;
	nSites  = Sp[0].NumberSites;

	simplmat <unsigned long>dens(nStages,NumSpecies);
	simplmat <unsigned long>denSit(nStages,nSites);

	dens.fill( 0 );
	denSit.fill( 0 );

	for( int i=0; i<DimY; i++)
		for(int j=0; j<DimX; j++)
			{
			e = C(j,i).Elem(N) - 1;
			if( e >= 0 )
				{
				a = C(j,i).Age;
				a = AgeToStage( a );
				dens.elem(a,e)++;
				denSit(a,C(j,i).Site)++;
				}
			}

	if( privez )
		{
		privez=0;
		cout << "Sp  Age";
		for( a=0; a<nStages; a++)
			{
			cout.width(6);
			cout << a;
			}
		cout << "  Tot sp" << " ";

		for( s=0; s<nSites; s++ )
			{
			for( a=0; a<nStages; a++)
				{
				cout.width(6);
				cout << a;
				}
			cout << "  T.Sit" << setw(2) << s << "  ";
			}
		cout << "Disp.Med.  Cant.Disp." << endl;

		}

	for(e=0; e<NumSpecies; e++)
		{
		cout << setw(2) << e << setw(5) << T;
		tot = 0 ;
		for( a=0; a<Sp[e].NumberStages; a++)
			{
			cout.width(6);
			cout << dens.elem(a,e);
			tot += dens.elem(a,e);
			}

		cout << setw(8) << tot << " ";

		for( s=0; s<nSites; s++)
			{
			tot = 0 ;
			for( a=0; a<Sp[e].NumberStages; a++)
				{
				cout.width(6);
				cout << denSit(a,s);
				tot += denSit(a,s);
				}
			cout << setw(8) << tot << "   ";
			}
		}
	if( CantDispersers!=0)
		cout << "  " << setw(10) <<  MeanDistDisp/CantDispersers << "  " << setw(10) << CantDispersers;
	else
		cout << "  " << setw(10) <<  MeanDistDisp << "  " << setw(10) << CantDispersers;
	cout << endl;

	MeanDistDisp = 0;
	CantDispersers = 0;
	}



void CAFox::PrintMap()
	{
	ostrstream name;
	name << "map" << T << ".map" << ends;
	//name.freeze();
	ofstream map( name.str() );
	for(int i=0; i<DimY; i++)
		{
		for(int j=0;j<DimX;j++)
			{
			map.width(2);
			map << C(j,i).Elem(N) ;
			}
		map << endl;
		}
	map << endl << endl;
	//name.freeze(0);
	}

void CAFox::PrintMapXY()
	{
	ostrstream name;
	name << "xy" << T << ".map" << ends;
	//name.freeze();
	ofstream xymap( name.str() );
	int sp;
	for(int i=0; i<DimY; i++)
		for(int j=0;j<DimX;j++)
			{
			sp = C(j,i).Elem(N);
			if( sp > 0 )
				xymap << setw(5) << j << setw(5) << i << setw(3) << sp << endl;
			}
	//name.freeze(0);
	}

void CAFox::PrintMapXYSp()
	{
	int sp,s;
	unsigned long k;
	for(s=0; s<NumSpecies; s++)
		{
		k=0;
		ostrstream name;
		name << "xy" << T << "sp" << s << ".map" << ends;
		//name.freeze();
		ofstream xymap( name.str() );
		//cout << "Age " << setw(5) << T << "Specie " << setw(3) << s << endl;
		sp = s+1;
		for(int i=0; i<DimY; i++)
			for(int j=0;j<DimX;j++)
				{
				if( sp == C(j,i).Elem(N) )
					{
					xymap<< setw(5) << j << setw(5) << i << endl;
					k++;
					}
				}
		//name.freeze(0);
		xymap << k << endl;
		}
	}

void CAFox::RandomSetSeed(int sp,unsigned age, int no, int minX)
	{
	int rx,ry;
	int i;
	for( i=0; i<no; i++)
		{
		while( 1 )
			{
			rx=minX + Rand(DimX-minX);
			ry=Rand(DimY);
			if( C(rx,ry).Elem(A) == 0 )
				{
				C(rx,ry).Elem(A) = sp+1;
				C(rx,ry).Age = age;
				break;
				}
			}
		}
	}

void CAFox::ReadSetSeed(char * file)
	{
	FILE *in;
	char buff[255];

	int sp=0,age=0,cant=0,minx=0;


	if ((in = fopen(file, "rt")) == NULL)
		{
		fprintf(stderr, "Cannot open Parms file.\n");
		exit(1);
		}

	fgets(buff,80,in);
	do
		{
		sscanf(buff,"%i %i %i %i",&sp,&age,&cant,&minx);
		RandomSetSeed(sp,age,cant,minx);
		}
	while( fgets(buff,80,in)!=NULL );
	fclose(in);

	}

int CAFox::ReadSeed( char * fname)
	{
	FILE *in;
	char buff[255];
	int dx,dy;
	int spe,tipo;

	if ((in = fopen(fname, "rt")) == NULL)
		{
		fprintf(stderr, "Cannot open Seed file.\n");
		return 1;
		}
	fgets(buff,255,in);
	sscanf(buff,"%d %d",&dx,&dy);
	if( dx!=DimX || dy!=DimY )
		{
		fprintf(stderr, "Wrong dimension for Seed file.\n");
		exit(1);
		}

	while( fgets(buff,4,in)!= NULL )
		{
		if( strncmp(buff,"SP",2)==0 )
			tipo = 0;
		else if(strncmp(buff,"AG",2)==0 )
			tipo = 1;
		else if(strncmp(buff,"SI",2)==0 )
			tipo = 2;
		else
			continue;

		for(dy=0;dy<DimY; dy++)
			for(dx=0;dx<DimX; dx++)
				{
				int ret = fscanf(in,"%i",&spe);
				if( ret == 0 || ret ==EOF )
					{
					cerr << "Seed File invalid.\n";
					exit(1);
					}

				switch (tipo) {
					case 0:
						C(dx,dy).Elem(A) = spe;
						break;
					case 1:
						C(dx,dy).Age = spe;
						break;
					case 2:
						C(dx,dy).Site= spe;
						break;
					default:
						cerr << "Seed File invalid.\n";
						exit(1);
					}

				}
		}

	fclose(in);
	return 0;

	}


int CAFox::SaveSeed( char * fname)
	{
	int i,j;
	ofstream sav( fname );
	if(!sav)
		{
		cerr << "Cannot open Seed file.\n";
		return 1;
		}

	sav << DimX << "\t" << DimY << endl;
	sav << "SP" << endl;
	for(i=0; i<DimY; i++)
		{
		for(j=0;j<DimX;j++)
			{
			sav<< setw(2) << C(j,i).Elem(N);
			}
		sav << endl;
		}
	sav << endl;

	sav << "AG" << endl;
	for(i=0; i<DimY; i++)
		{
		for(j=0;j<DimX;j++)
			{
			sav<< setw(4) << C(j,i).Age;
			}
		sav << endl;
		}
	sav << endl;

	sav << "SI" << endl;
	for(i=0; i<DimY; i++)
		{
		for(j=0;j<DimX;j++)
			{
			sav<< setw(2) << C(j,i).Site;
			}
		sav << endl;
		}
	sav << endl;

	return 0;
	};
