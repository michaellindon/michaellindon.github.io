---
id: 605
title: Parallel Tempering Algorithm with OpenMP / C++
date: 2013-07-14T21:00:35+00:00
author: admin
layout: post
guid: http://www.lindonslog.com/?p=605
permalink: /programming/openmp/parallel-tempering-algorithm-c/
categories:
  - OpenMP
  - Programming
  - Statistics
tags:
  - c
  - mcmc
  - openmp
  - parallel tempering
---
[1.1. Parallel Tempering Theory](#theory)
  
[1.2. Physics Origins](#physics)
  
[2.1 Intra-Thread Metropolis Move](#intra)
  
[2.2. Inter-Thread Parallel Tempering](#inter)
  
[2.3. OpenMP Parallelization](#openmp)
  
[3. Full Code](#fullcode)4. Simulation Study
  
[5. On the Future use of Parallel Tempering with OpenMP](#futureuse)</p> 

Parallel tempering is one of my favourite sampling algorithms to improve MCMC mixing times. This algorithm seems to be used _exclusively_ on distributed memory architectures using MPI and remains unexploited on shared memory architectures such as our office computers, which have up to eight cores. I&#8217;ve written parallel tempering algorithms in MPI and Rmpi but never in OpenMP. It turns out that the latter has substantial advantages. I guess when people think of parallel tempering they think of processors communicating with each other via MPI and swapping parameters directly. If you are on a shared memory device, however, you can have processor A simply write to a shared array and have processor B read therefrom, which really saves a lot of aggro fiddling around with message numbers, blocking/non-blocking calls and deadlocks etc. Moreover, with OpenMP you can spawn more threads than you have processors, which translates to more parallel MCMC chains in the present context, whereas this becomes troublesome with MPI due to the danger of deadlocks. OpenMP is also much easier to use than MPI, with one line you can fork a serial thread into a desired and hardware-independent number of parallel threads. The code looks as follows:

<a name="theory"></a>
  


## Parallel Tempering Theory

Each thread simulates an MCMC trajectory from the posterior raised to a fractional power, B. When B=1, the MCMC draws are from the posterior from which we wish to sample. When B=0, the MCMC trajectory is just a realization of a Brownian motion random walk. To see this, consider the acceptance probability of the metropolis move. The density evaluated at the proposed parameters over the density evaluated at the current parameters all raised to the power of zero is unity, whatever the densities are, so the moves always get accepted. Similarly if B is close to zero, then the acceptance probability is near unity and the distribution from which this MCMC is sampling is quite uniform over the parameter space, so the trajectory explores a relatively larger part of the parameter space. As B is increased toward one, the features of the distribution from which we wish to sample start to become more prominent. In the other direction from B=1 to 0 one commonly says that the posterior is &#8220;melted down&#8221; and spreading out its mass. The terminology has remained from its origins in statistical physics where one would simulated particles at a hotter temperature, so that they would jostle around more and escape wells in the potential energy. The key to parallel tempering is to use the more diffuse, hotter or melted down MCMC chains as proposal distributions for the actual cold distribution we wish to sample from. One proceeds by performing a Metropolis-Hastings move because the proposal distributions are not symmetric. For illustration, thread j uses the hotter thread j+1 as its partner and as proposal distribution. Let theta j+1 be the proposed new position for thread j, being the current position of thread j+1.
  
<img src="//s0.wp.com/latex.php?latex=%5Calpha%3Dmin%281%2C%5Cfrac%7B++p_%7Bj%7D+%28%5Ctheta_%7Bj%2B1%7D+%29%7D++%7B+++p_%7Bj%7D%28%5Ctheta_%7Bj%7D+%29+%7D++%5Cfrac%7B++p_%7Bj%2B1%7D+%28%5Ctheta_%7Bj%7D+%29%7D++%7B+++p_%7Bj%2B1%7D%28%5Ctheta_%7Bj%2B1%7D+%29+%7D++++%29++&#038;bg=ffffff&#038;fg=000&#038;s=0" alt="&#92;alpha=min(1,&#92;frac{  p_{j} (&#92;theta_{j+1} )}  {   p_{j}(&#92;theta_{j} ) }  &#92;frac{  p_{j+1} (&#92;theta_{j} )}  {   p_{j+1}(&#92;theta_{j+1} ) }    )  " title="&#92;alpha=min(1,&#92;frac{  p_{j} (&#92;theta_{j+1} )}  {   p_{j}(&#92;theta_{j} ) }  &#92;frac{  p_{j+1} (&#92;theta_{j} )}  {   p_{j+1}(&#92;theta_{j+1} ) }    )  " class="latex" />
  
The second fraction is the Hastings addition to the Metropolis algorithm and is required to satisfy detailed balance for an unsymmetrical proposal distribution. Now realise that
  
<img src="//s0.wp.com/latex.php?latex=p_%7Bj%7D%3D%5Cpi%28%5Ctheta%7CY%29%5E%7BB_%7Bj%7D%7D%5C%5C++p_%7Bj%2B1%7D%3D%5Cpi%28%5Ctheta%7CY%29%5E%7BB_%7Bj%2B1%7D%7D++&#038;bg=ffffff&#038;fg=000&#038;s=0" alt="p_{j}=&#92;pi(&#92;theta|Y)^{B_{j}}&#92;&#92;  p_{j+1}=&#92;pi(&#92;theta|Y)^{B_{j+1}}  " title="p_{j}=&#92;pi(&#92;theta|Y)^{B_{j}}&#92;&#92;  p_{j+1}=&#92;pi(&#92;theta|Y)^{B_{j+1}}  " class="latex" />
  
i.e. they are the same distribution raised to different fractional powers. Working now on the log scale, it can be shown that
  
<img src="//s0.wp.com/latex.php?latex=log+%5Cleft%28+%5Cfrac%7B++p_%7Bj%7D+%28%5Ctheta_%7Bj%2B1%7D+%29%7D++%7B+++p_%7Bj%7D%28%5Ctheta_%7Bj%7D+%29+%7D++%5Cfrac%7B++p_%7Bj%2B1%7D+%28%5Ctheta_%7Bj%7D+%29%7D++%7B+++p_%7Bj%2B1%7D%28%5Ctheta_%7Bj%2B1%7D+%29+%7D++++%5Cright%29+%3D++%28B_%7Bj%7D-B_%7Bj%2B1%7D%29+%5Cleft%28+log%28%5Cpi%5B%5Ctheta_%7Bj%2B1%7D%7CY%5D%29+-+log%28%5Cpi%5B%5Ctheta_%7Bj%7D%7CY%5D%29+%5Cright%29++&#038;bg=ffffff&#038;fg=000&#038;s=0" alt="log &#92;left( &#92;frac{  p_{j} (&#92;theta_{j+1} )}  {   p_{j}(&#92;theta_{j} ) }  &#92;frac{  p_{j+1} (&#92;theta_{j} )}  {   p_{j+1}(&#92;theta_{j+1} ) }    &#92;right) =  (B_{j}-B_{j+1}) &#92;left( log(&#92;pi[&#92;theta_{j+1}|Y]) - log(&#92;pi[&#92;theta_{j}|Y]) &#92;right)  " title="log &#92;left( &#92;frac{  p_{j} (&#92;theta_{j+1} )}  {   p_{j}(&#92;theta_{j} ) }  &#92;frac{  p_{j+1} (&#92;theta_{j} )}  {   p_{j+1}(&#92;theta_{j+1} ) }    &#92;right) =  (B_{j}-B_{j+1}) &#92;left( log(&#92;pi[&#92;theta_{j+1}|Y]) - log(&#92;pi[&#92;theta_{j}|Y]) &#92;right)  " class="latex" />
  
<a name="physics"></a>
  


### Physics Origins

It is at this point where sometimes, in order to make things correspond to the earlier physics literature, one defines the &#8220;Energy&#8221; as
   
<img src="//s0.wp.com/latex.php?latex=E_%7Bj%7D%3D-log%28%5Cpi%5B%5Ctheta_%7Bj%7D%7CY%5D%29.&#038;bg=ffffff&#038;fg=000&#038;s=0" alt="E_{j}=-log(&#92;pi[&#92;theta_{j}|Y])." title="E_{j}=-log(&#92;pi[&#92;theta_{j}|Y])." class="latex" />
  
So that the acceptance probability becomes
  
<img src="//s0.wp.com/latex.php?latex=%5Calpha%3Dmin%281%2Ce%5E%7B-%28B_%7Bj%7D-B_%7Bj%2B1%7D%29%28E_%7Bj%2B1%7D+-+E_%7Bj%7D%29++%7D%29.++&#038;bg=ffffff&#038;fg=000&#038;s=0" alt="&#92;alpha=min(1,e^{-(B_{j}-B_{j+1})(E_{j+1} - E_{j})  }).  " title="&#92;alpha=min(1,e^{-(B_{j}-B_{j+1})(E_{j+1} - E_{j})  }).  " class="latex" />
  
It&#8217;s not necessary to define this energy, it only defines an equivalence mapping between statistics and physics. In physics particles get stuck in the local minima of the energy landscape and in statistics the MCMC gets stuck in the local peaks of the posterior. The reason for this is that in a canonical ensemble lower energy states are more probable (recall that nature tries to minimize the potential energy and that force is the negative gradient of the potential energy), so regions of the parameter space with low potential energy, physically, correspond to regions of high probability density, statistically. To be more precise, a result from statistical physics is that the distribution of energy is exponential with scale parameter kT, where k is Boltzmann&#8217;s constant and T is temperature (this condition holds only for a canonical ensemble). An exponential distribution with this scale parameter is called the Boltzmann distribution by physicists. As the temperature increases, higher energy states become more probable and the particle jumps out of the minima more. If you are a statistician you don&#8217;t need to worry about this, but sometimes this notation crops up in the literature. Its also the same acceptance probability now as in physics when sampling energies from a Boltzmann distribution. I have decided not to adopt the physics notation for this post.

<a name="intra"></a>
  


## Intra-Thread Metropolis Move

Each thread, within itself, performs a normal vanilla metropolis move:

<pre class="brush: cpp; title: ; notranslate" title="">//Propose Candidate Position//
			t1new=t1[rank*nmc+i-1] + normal(stream[rank]);
			t2new=t2[rank*nmc+i-1] + normal(stream[rank]);

			//Calculate log-Density at Newly-Proposed and Current Position//
			lpost_new[rank]=lLikelihood(t1new,t2new) + lprior(t1new,t2new);
			lpost[rank]=lLikelihood(t1[rank*nmc+i-1],t2[rank*nmc+i-1]) + lprior(t1[rank*nmc+i-1],t2[rank*nmc+i-1]);

			//Melt Density and Calculate log-Acceptance Probability//
			lalpha=B[rank]*(lpost_new[rank]-lpost[rank]);

			//Perform Metropolis Accept-Reject Step//
			if( log(u(stream[rank])) &lt; lalpha ){
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
</pre>

A few comments about the variables. &#8220;nmc&#8221; is the number of mcmc draws I wish to generate. I have two parameters which I have denoted t1 and t2, because t is closest to theta. Moreover, each processor stores its _nmc_ draws of t1 and t2 in a contiguous array in the memory of length nmc times number of threads. &#8220;Rank&#8221; Identifies the thread and &#8220;lpost&#8221; and &#8220;B&#8221; are arrays of length equal to the number of threads in which to store the log posterior density at the current position and the fractional melting power. All of these variables are defined at the top of the code.

<a name="inter"></a>
  


## Inter-Thread Metropolis-Hastings Move

<pre class="brush: cpp; title: ; notranslate" title="">if(u(stream[rank]) &lt; 0.5){
					rank_partner=rank+1;
					if(rank_partner &lt; size){
						//Inter-Thread Metropolis-Hastings Part
						lalpha = (B[rank]-B[rank_partner])*(lpost[rank_partner]-lpost[rank]);
						if(log(u(stream[rank])) &lt; lalpha){
							//accept swap
							swap(t1[rank*nmc+i],t1[rank_partner*nmc+i]);
							swap(t2[rank*nmc+i],t2[rank_partner*nmc+i]);
						}

					}
				}
</pre>

The only additional thing to add is that each chain attempts a swap with its neighbour at each iteration with probability 1/2. There is nothing special about 1/2, you could choose what you like, but there are pros and cons. How this made parallel in OpenMP is shown below.

<a name="openmp"></a>
  


## OpenMP Parallelization

The OpenMP parallel implementation of the above algorithm is very simple!

<pre class="brush: plain; title: ; notranslate" title="">#pragma omp parallel private(i,t1new,t2new,rank,lalpha,rank_partner) shared(B, lpost, lpost_new,t1,t2,swapt1,swapt2)
	{
		//Identify Each Thread
		rank=omp_get_thread_num();

		for (i = 1; i &lt; nmc; ++i)
		{

                 //***Intra-Thread Metropolis Part***//
	
#pragma omp barrier      //Synchronise Threads
#pragma omp critical     //Executed Critical Code Block Oney Thread at a Time. 
			{

                 //***Inter-Thread Parallel Tempering Part***//

			}
#pragma omp barrier   //Synchronise Threads
		}
	}
</pre>

The first parallel pragma simply forks the master thread into a number of threads whereby each thread executes the following code block independently i.e. a number of independent parallel mcmcs. Specifying variables as private means that each thread gets a copy of that variable in its own seperate location in the memory. Shared is the opposite, although I think variables are shared by default. The barrier pragma means that each thread halts until all threads have reached this point. The critical pragma means the following code block is executed by one thread at a time only. This prevents thread j swapping with thread j+1 whilst thread j+1 is attempting a swap with thread j+2, nasty things such as race conditions can occur. The last pragma barrier waits for all threads to have reached the end and then the next iteration of the for loop proceeds.

<a name="fullcode"></a>
  


## Full code

The full code can be found [here](http://www.lindonslog.com/example_code/tempering.cpp). It depends on [OpenMP](http://www.lindonslog.com/category/programming/openmp/) and the [TRNG](http://www.lindonslog.com/programming/parallel-random-number-generation-trng/ "Parallel Random Number Generation using TRNG") library in order to generate multiple independent streams of random numbers. It takes the number of mcmc draws as a command-line argument.

<pre class="brush: bash; title: ; notranslate" title="">[michael@michael tempering]$ wget http://www.lindonslog.com/example_code/tempering.cpp
[michael@michael tempering]$ g++ tempering.cpp -fopenmp -o tempering  -ltrng4 -lm
[michael@michael tempering]$ ./tempering 10000
Thread 0 has fractional power 1
Thread 1 has fractional power 0.469117
Thread 2 has fractional power 0.220071
Thread 3 has fractional power 0.103239
Thread 4 has fractional power 0.0484313
Thread 5 has fractional power 0.0227199
Thread 6 has fractional power 0.0106583
Thread 7 has fractional power 0.005
[michael@michael tempering]$
</pre>

<a name="simstudy"></a>
  


## Simulation Study

I chose the likelihood to be 5 sharply peaked normal distributions located at the corners of a sort-of unit square plus one at the origin with variances of 0.001. The prior was a normal of variance 1000 centered at the origin. The parallel tempering algorithm was run with 8 threads. The posterior draws and mixing results are below:
  


<div id="attachment_632" style="width: 490px" class="wp-caption aligncenter">
  <a href="http://www.lindonslog.com/wp-content/uploads/2013/07/partempdraws.png"><img src="http://www.lindonslog.com/wp-content/uploads/2013/07/partempdraws.png" alt="Posterior Draws" width="480" height="480" class="size-full wp-image-632" srcset="http://www.lindonslog.com/wp-content/uploads/2013/07/partempdraws.png 480w, http://www.lindonslog.com/wp-content/uploads/2013/07/partempdraws-150x150.png 150w, http://www.lindonslog.com/wp-content/uploads/2013/07/partempdraws-300x300.png 300w" sizes="(max-width: 480px) 100vw, 480px" /></a>
  
  <p class="wp-caption-text">
    Posterior Draws from Parallel Tempering
  </p>
</div>


  


<div id="attachment_630" style="width: 610px" class="wp-caption aligncenter">
  <a href="http://www.lindonslog.com/wp-content/uploads/2013/07/parallel_tempering.png"><img src="http://www.lindonslog.com/wp-content/uploads/2013/07/parallel_tempering.png" alt="parallel tempering mixing" width="600" height="900" class="size-full wp-image-630" srcset="http://www.lindonslog.com/wp-content/uploads/2013/07/parallel_tempering.png 600w, http://www.lindonslog.com/wp-content/uploads/2013/07/parallel_tempering-200x300.png 200w" sizes="(max-width: 600px) 100vw, 600px" /></a>
  
  <p class="wp-caption-text">
    Mixing of parallel tempering algorithm
  </p>
</div>

<a name="futureuse"></a>
  


## On the Future use of Parallel Tempering with OpenMP

I hope the code exemplifies how easy it is to run parallel MCMC chains with OpenMP. I would argue that the metropolis moves are the hardest part. If you can write them for a single serial chain, then it is only a few extra steps to run parallel chains and imlement that parallel tempering algorithm. My laptop has four cores and my office computer has eight. Given the trajectory of technology that shared memory devices have an ever increasing number of cores, it seems to me that parallel tempering is becoming an ever-more valuable algorithm to improve mixing times of MCMC runs. Afterall, had I not used the extra 3 cores on my laptop, they would have remained idle. If you have extra cores, why not use them! Moreover with OpenMP you can spawn as many parallel MCMCs as you desire, avoiding the pitalls of MPI.

<span class="Z3988" title="ctx_ver=Z39.88-2004&#038;rft_val_fmt=info%3Aofi%2Ffmt%3Akev%3Amtx%3Ajournal&#038;rft_id=info%3Adoi%2F10.1039%2Fb509983h&#038;rft.atitle=Parallel+tempering%3A+Theory%2C+applications%2C+and+new+perspectives&#038;rft.jtitle=Physical+Chemistry+Chemical+Physics&#038;rft.artnum=http%3A%2F%2Fxlink.rsc.org%2F%3FDOI%3Db509983h&#038;rft.volume=7&#038;rft.issue=23&#038;rft.issn=1463-9076&#038;rft.spage=3910&#038;rft.date=2005&#038;rfr_id=info%3Asid%2Fscienceseeker.org&#038;rft.au=Earl+David+J.&#038;rft.aulast=Earl&#038;rft.aufirst=David+J.&#038;rft.au=Deem+Michael+W.&#038;rft.aulast=Deem&#038;rft.aufirst=Michael+W.&#038;rfs_dat=ss.included=1&#038;rfe_dat=bpr3.included=1;bpr3.tags=Chemistry%2CComputer+Science+%2F+Engineering%2CMathematics%2CPhysics">Earl D.J. & Deem M.W. (2005). Parallel tempering: Theory, applications, and new perspectives, <span style="font-style:italic;">Physical Chemistry Chemical Physics, 7</span> (23) 3910. DOI: <a rel="author" href="http://dx.doi.org/10.1039%2Fb509983h">10.1039/b509983h</a></span>