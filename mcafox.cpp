//
// CADis : CA Discreto con bordes continuos
//
#include <strstrea.h>
#include <iomanip.h>
#include <ctype.h>
#include "cafox.hpp"
#include "fortify.h"
int ReadParameters(int argc, char * argv[], CAFox &ca, int &nEval, int &nRuns );



int main(int argc, char * argv[])
	{
	int nEval=0,nRuns=0;
	CAFox ca;
	char gr,de,sa,baseName[12];
	int inter=0,argi;

//	ca.SetA(0);
	argi = ReadParameters(argc,argv,ca,nEval,nRuns);

	if( argc>=7 )
		{
		gr = toupper( *argv[argi] );
		inter = atoi(  argv[argi+1] );
		de = toupper( *argv[argi+2] );
		sa = toupper( *argv[argi+3] );
		if( sa=='S' && argc>argi+4 )
			strcpy(baseName, argv[argi+4]);
		else
			strcpy(baseName, "sed");

		goto NoPide;
		}

	cerr << "Graficos ? :";
	cin >> gr;
	gr = toupper(gr);

	cerr << "Cantidad de pasos para salida : ";
	cin >> inter;

	cerr << "Imprime densidades de las especies :";
	cin >> de;
	de = toupper( de );

	cerr << "Salvar al final :";
	cin >> sa;

	sa = toupper( sa );
	if( sa=='S' )
		{
		cerr << "Nombre de archivo base :";
		cin >> setw(6) >> baseName;
		}

	NoPide:;

	for(int r=0; r<nRuns; r++)
		{
		if( gr=='S' )
			ca.InitGraph();
		ca.SetT(2);

		for(int i=0; i<nEval; i++)
			{
			if( i == 0 )
				{
				ca.SetN(0);
				if( gr=='S' )
					ca.PrintGraph();
				else
					cerr << "Evaluaci¢n " << i << endl;
				if( de=='S')
					ca.PrintDensStage();
				ca.SetN(1);
   			}
			ca.Evaluate();
			if( ((i+1) % inter)==0 || i==0 )
				{
				if( gr=='S' )
					ca.PrintGraph();
				else
					cerr << "Evaluaci¢n " << (i+1) << endl;

				if( de=='S')
					ca.PrintDensStage();
				}
			}
		if( gr=='S' )
			ca.EndGraph();

		if( sa=='S' )
			{
			ostrstream name;
			name << baseName << (i+1) << ".sed" << ends;
			ca.SaveSeed( name.str() );
			}

		ca.Reset();
		ReadParameters(argc,argv,ca,nEval,nRuns);
		}
	return 0;
	}



int ReadParameters(int argc, char * argv[], CAFox &ca, int &nEval, int &nRuns )
	{
	int argi=3;
	static int privez=1;
	if( argc > 1 )
		{
		if( privez )
			{
			ca.ReadParms( nEval,nRuns,argv[1] );
			privez=0;
			}
		if( argc>2 )
			{
			if( strstr(argv[2],"sed")!=NULL )
				ca.ReadSeed(argv[2]);	// Lee mapa ASCII de las especies y edades
			else if( strstr(argv[2],"set")!=NULL )
				ca.ReadSetSeed(argv[2]);
			}
		if( argc>3 )
			{
			if( strstr(argv[3],"set")!=NULL )
				{
				ca.ReadSetSeed(argv[3]);
				argi = 4;
				}
			}

		}
	else
		cerr << "Parameters file missing!!!\n";
	return argi;
	}
