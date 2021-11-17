# include <cstdlib>
# include <sstream>
# include <string>
# include <fstream>
# include <iostream>
# include <iomanip>
# include <mpi.h>
#include<cmath>

int main( int nargs, char* argv[] )
{
	// On initialise le contexte MPI qui va s'occuper :
	//    1. Créer un communicateur global, COMM_WORLD qui permet de gérer
	//       et assurer la cohésion de l'ensemble des processus créés par MPI;
	//    2. d'attribuer à chaque processus un identifiant ( entier ) unique pour
	//       le communicateur COMM_WORLD
	//    3. etc...
	MPI_Init( &nargs, &argv );
	// Pour des raisons de portabilité qui débordent largement du cadre
	// de ce cours, on préfère toujours cloner le communicateur global
	// MPI_COMM_WORLD qui gère l'ensemble des processus lancés par MPI.
	MPI_Comm globComm;
	MPI_Comm_dup(MPI_COMM_WORLD, &globComm);
	// On interroge le communicateur global pour connaître le nombre de processus
	// qui ont été lancés par l'utilisateur :
	int nbp;
	MPI_Comm_size(globComm, &nbp);
	// On interroge le communicateur global pour connaître l'identifiant qui
	// m'a été attribué ( en tant que processus ). Cet identifiant est compris
	// entre 0 et nbp-1 ( nbp étant le nombre de processus qui ont été lancés par
	// l'utilisateur )
	int rank;
	MPI_Comm_rank(globComm, &rank);
	// Création d'un fichier pour ma propre sortie en écriture :
	std::stringstream fileName;
	fileName << "Output" << std::setfill('0') << std::setw(5) << rank << ".txt";
	std::ofstream output( fileName.str().c_str() );
	int val = 999;
	int d =2; // dimension de l'espace de travail !! on peut changer au choix l'espace ! 
// il faut une certain condition : IL FAUT que nombre_de_processurs = 2^d
	std::cout<<"Le rank est : " <<rank<<std::endl;
    	if (rank == 0) 	
    	{	
    	       int j=1;
    	       while(j<d+1)
    	       {
    	        	MPI_Send(&val, 1, MPI_DOUBLE, j, 0, MPI_COMM_WORLD);
    	        	output << "reception de la source : " << rank << " valeur " <<  val << " vers la source : 1 " << std::endl;
        		j++;
        	}
 /*       	MPI_Request request;
		for(int i=2;i<d-1;i++)
        	{
            		
            		//MPI_Isend(&val, 1, MPI_DOUBLE, rank+i, 0, MPI_COMM_WORLD,&request);
            		
            		MPI_Send(&val, 1, MPI_DOUBLE, i, 0, MPI_COMM_WORLD);
            		std::cout << "reception de la source : " << rank << " valeur " << val << std::endl;
    	    	}*/
    	}
	else 
    	{
        	MPI_Status status;
		MPI_Request request;
        	MPI_Irecv(&val, 1, MPI_DOUBLE, rank-1, 0, MPI_COMM_WORLD, &request);
               MPI_Irecv(&val, 1, MPI_DOUBLE, rank+1, 0, MPI_COMM_WORLD, &request);
		//MPI_Recv(&val, 1, MPI_DOUBLE, rank-1, 0, MPI_COMM_WORLD, &status);
		int i=rank;
		int comp=1;
		while(comp<d)
		{
            		//MPI_Isend(&val, 1, MPI_DOUBLE, rank+comp, 0, MPI_COMM_WORLD,&request);
			//MPI_Send(&val, 1, MPI_DOUBLE, rank+comp, 0, MPI_COMM_WORLD);
            		output << "envoie vers la source : " << rank+comp << " la valeur = ( " <<  status.MPI_SOURCE << " ) de la source : "<<rank<<std::endl;
			MPI_Send(&val, 1, MPI_DOUBLE, rank+comp, 0, MPI_COMM_WORLD);
            		//MPI_Isend(&val, 1, MPI_DOUBLE, rank-comp, 0, MPI_COMM_WORLD,&request);
            		output << "envoie vers la source : " << rank-comp << " la valeur = ( " <<  status.MPI_SOURCE << " ) de la source : "<<rank<<std::endl;
            		comp++;
            		
		}
        }
	
	output.close();
	// A la fin du programme, on doit synchroniser une dernière fois tous les processus
	// afin qu'aucun processus ne se termine pendant que d'autres processus continue à
	// tourner. Si on oublie cet instruction, on aura une plantage assuré des processus
	// qui ne seront pas encore terminés.
	MPI_Finalize();
	return EXIT_SUCCESS;
}
