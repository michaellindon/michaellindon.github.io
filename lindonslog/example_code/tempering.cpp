#include <cstdlib>
#include <iostream>
#include <vector>
#include <omp.h>
#include <cmath>
#include <trng/yarn2.hpp>
#include <trng/uniform01_dist.hpp>
#include <trng/normal_dist.hpp>
#include <algorithm>
#include <fstream>

double lLikelihood(double t1, double t2);
double lprior(double t1, double t2);

using namespace std;
int main(int argc, char *argv[])
{
	int nmc, size, rank,rank_partner;
	double lalpha, t1new, t2new,temp;
	int exchange_interval=5;
	int i=0;
	int acc=0;
	vector<double> B;
	vector<double> lpost_new;
	vector<double> lpost;
	vector<double> t1;
	vector<double> t2;
	vector<double> swapt1;
	vector<double> swapt2;
	trng::uniform01_dist<> u;
	trng::normal_dist<> normal(0.0,0.1);
	ofstream outmaster("master.txt");
	ofstream outsize("size.txt");
	ofstream outone("one.txt");
	ofstream outtwo("two.txt");
	ofstream outthree("three.txt");
	ofstream outfour("four.txt");
	ofstream outfive("five.txt");
	ofstream outsix("six.txt");

	if(argc==2){
		nmc=atoi(argv[1]);
	}else{
		cout << "Incorrect number of arguments supplied" << endl;
		return(EXIT_FAILURE);
	}

	//Queery maximum number of threads supported by system
	size=omp_get_max_threads();
	size=8;
	//Set number of threads to this maximum
	omp_set_num_threads(size);
	//Resize vectors accordingly
	B.resize(size);
	lpost_new.resize(size);
	lpost.resize(size);
	t1.resize(size*nmc);
	t2.resize(size*nmc);
	swapt1.resize(size);
	swapt2.resize(size);



	//Generate "size" numbers of independent PRN streams
	trng::yarn2 stream[size];
	for (i = 0; i < size; i++)
	{
		stream[i].split(size,i);
	}

	//Randomize Initial Starting Positions of each chain
	for (rank = 0; rank < size; ++rank)
	{
		t1[rank*nmc+0]=(u(stream[rank])-0.5)*3;
		t2[rank*nmc+0]=(u(stream[rank])-0.5)*3;
	}


	for (i = 0; i < size; ++i)
	{
		temp=(double)(i)/(size-1);
		B[i]=pow(0.005,temp);
		cout << "Thread " << i << " has fractional power " << B[i] << endl;
	}




#pragma omp parallel private(i,t1new,t2new,rank,lalpha,rank_partner) shared(B, lpost, lpost_new,t1,t2,swapt1,swapt2)
	{
		//Identify Each Thread
		rank=omp_get_thread_num();

		for (i = 1; i < nmc; ++i)
		{

			//***Intra-Thread Metropolis Part***//

			//Propose Candidate Position//
			t1new=t1[rank*nmc+i-1] + normal(stream[rank]);
			t2new=t2[rank*nmc+i-1] + normal(stream[rank]);

			//Calculate Density at Newly-Proposed and Current Position//
			lpost_new[rank]=lLikelihood(t1new,t2new) + lprior(t1new,t2new);
			lpost[rank]=lLikelihood(t1[rank*nmc+i-1],t2[rank*nmc+i-1]) + lprior(t1[rank*nmc+i-1],t2[rank*nmc+i-1]);

			//Melt Density and Calculate log Acceptance Probability//
			lalpha=B[rank]*(lpost_new[rank]-lpost[rank]);

			//Perform Metropolis Accept-Reject Step//
			if( log(u(stream[rank])) < lalpha ){
				//Accept
				//Proposed as Current Position
				t1[rank*nmc+i]=t1new;
				t2[rank*nmc+i]=t2new;
			}else{
				//Do not Accept
				//Propogate Current Position
				t1[rank*nmc+i]=t1[rank*nmc+i-1];
				t2[rank*nmc+i]=t2[rank*nmc+i-1];
			}


			//***Inter-Thread Parallel Tempering Part***//

#pragma omp barrier      //Synchronise Threads
#pragma omp critical     //Executed Critical Code Block Oney Thread at a Time. 
			{
				if(u(stream[rank]) < 0.5){
					rank_partner=rank+1;
					if(rank_partner < size){
						//Inter-Thread Metropolis-Hastings Part
						lalpha = (B[rank]-B[rank_partner])*(lpost[rank_partner]-lpost[rank]);
						if(log(u(stream[rank])) < lalpha){
							//accept swap
							swap(t1[rank*nmc+i],t1[rank_partner*nmc+i]);
							swap(t2[rank*nmc+i],t2[rank_partner*nmc+i]);
						}

					}
				}



			}
#pragma omp barrier   //Synchronise Threads
		}
	}

	cout << acc << " " << acc << endl;

	for (i = 1000; i < nmc; ++i)
	{
		outmaster << t1[i] << " " << t2[i] << endl;
		outsize << t1[(size-1)*nmc +i] << " " << t2[(size-1)*nmc+i] << endl;
		outone << t1[nmc+i] << " " << t2[nmc+i] << endl;
		outtwo << t1[nmc*2+i] << " " << t2[nmc*2+i] << endl;
		outthree << t1[nmc*3+i] << " " <<t2[nmc*3+i] << endl;
		outfour << t1[nmc*4+i] << " " << t2[nmc*4+i] << endl;
		outfive << t1[nmc*5+i] << " " << t2[nmc*5+i] << endl;
		outsix << t1[nmc*6+i] << " " << t2[nmc*6+i] << endl;
	}

	return 0;
}



double lLikelihood(double t1, double t2){
	double result;
	double sigma2=0.001;
	result=   exp(-(1/(2*sigma2))*((t1-1)*(t1-1) + (t2-1)*(t2-1) ))
		+ exp(-(1/(2*sigma2))*((t1-0)*(t1-0) + (t2-0)*(t2-0) ))
		+ exp(-(1/(2*sigma2))*((t1-1)*(t1-1) + (t2+1)*(t2+1) ))
		+ exp(-(1/(2*sigma2))*((t1+1)*(t1+1) + (t2-1)*(t2-1) ))
		+ exp(-(1/(2*sigma2))*((t1+1)*(t1+1) + (t2+1)*(t2+1) ));
	return (log(result));
}

double lprior(double t1, double t2){
	double result;
	double sigma2=1000;

	result=exp((-1/(2*sigma2))*(t1*t1+t2*t2));

	return(log(result));
}


/*			if(i % exchange_interval == 0){
//Compute Energy: E[rank]=-lpost[rank]
//Wait for all threads to reach the barrier
#pragma omp barrier
//Continue execution in parallel: proc even uses proc even+1 as proposal
{
if((rank+2)%2 ==0)
{
rank_partner=rank+1;
if(rank_partner>=0 & rank_partner < size){
lalpha = (B[rank]-B[rank_partner])*(lpost[rank_partner]-lpost[rank]);
if(log(u(stream[rank])) < lalpha){
//accept swap
swap(t1[rank*nmc+i],t1[rank_partner*nmc+i]);
swap(t2[rank*nmc+i],t1[rank_partner*nmc+i]);
}

}
}
}


//Wait for all threads to reach the barrier
#pragma omp barrier
//Continue execution in parallel: proc odd uses proc odd+1 as proposal
{
if((rank+2)%2 !=0)
{
rank_partner=rank+1;
if(rank_partner>=0 & rank_partner < size){
lalpha = (B[rank]-B[rank_partner])*(lpost[rank_partner]-lpost[rank]);
if(log(u(stream[rank])) < lalpha){
//accept swap
swap(t1[rank*nmc+i],t1[rank_partner*nmc+i]);
swap(t2[rank*nmc+i],t1[rank_partner*nmc+i]);
}

}
}
} 


}*/
