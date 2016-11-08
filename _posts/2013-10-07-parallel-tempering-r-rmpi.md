---
title: Parallel Tempering in R with Rmpi
date: 2013-10-07T01:00:54+00:00
image: /assets/images/parallel_tempering_mixing.jpeg
categories:
  - R
tags:
  - mcmc
  - parallel
  - R
---
My office computer recently got a really nice upgrade and now I have 8 cores on my desktop to play with. I also at the same time received some code for a Gibbs sampler written in R from my adviser. I wanted to try a metropolis-coupled markov chain monte carlo, <img src="//s0.wp.com/latex.php?latex=MC%5E%7B3%7D&#038;bg=ffffff&#038;fg=000&#038;s=0" alt="MC^{3}" title="MC^{3}" class="latex" />, algorithm on it to try and improve the mixing but the problem was that it was written in R and I&#8217;m used to writing parallel code in C/C++ with OpenMP or MPI.
{% include toc title="Slice Sampling" %}
Previously I wrote about a parallel tempering algorithm with an implementation in C++ using OpenMP and so I thought I would try and code up the same sort of thing in R as a warm-up exercise before I started with the full  <img src="//s0.wp.com/latex.php?latex=MC%5E%7B3%7D&#038;bg=ffffff&#038;fg=000&#038;s=0" alt="MC^{3}" title="MC^{3}" class="latex" />algorithm. Sadly I don&#8217;t think there is any facility in R for OpenMP style parallelism. There are packages such as _snow_ and _multicore_ but these are very high level packages and don&#8217;t really allow one to control the finer details. There is, however, Rmpi. It is a little bit different from regular C/Fortran MPI implementations and I once had a very bad experience getting some Rmpi code to work for a project deadline, it wasn&#8217;t pretty, so I was a little reluctant to reconsider this package but if you look at the changelogs it is still being actively maintained and in the end I&#8217;m very happy with the outcome of this experiment. I tried to write the below code as generally as possible, so that it is easily adapted by myself, or others, in the future.

## Target Density

First one needs to write a density one wishes to sample from

{% highlight R linenos %}
logdensity<-function(theta){
  #Distribution one wishes to sample from here.
  #It may be more convinient to pass a theta as a list
  sigma2=0.001;
  Sigma=matrix(0,2,2);
  Sigma[1,1]=sigma2;
  Sigma[2,2]=sigma2;
  density=dmvnorm(theta,c(0,0),Sigma)+dmvnorm(theta,c(-2,0.8),Sigma)+dmvnorm(theta,c(-1,1),Sigma)+dmvnorm(theta,c(1,1),Sigma)+dmvnorm(theta,c(0.5,0.5),Sigma);
  return(log(density))
}
{% endhighlight %}



The density I chose was a mixture of 5 well-separated bi-variate Normals. One should note that it is probably cleanest to pass all the arguments to this function as a list theta. It wasn&#8217;t really necessary in this case but if you have a posterior distribution with a number of parameters of varying dimension then it would be much nicer as a list. In a future blog post I may change the target density to be the energy distribution of a Lennard-Jones cluster. 

## Parallel Tempering Algorithm

This too is written as a function because Rmpi allows you to pass the function to all slaves and execute it. It was basically the easiest way of writing it for Rmpi.

{% highlight R linenos %}
temper<-function(niter,Bmin,swap.interval){
  rank=mpi.comm.rank();
  size=mpi.comm.size();
  swap=0;
  swaps.attempted=0;
  swaps.accepted=0;
  #Higher ranks run the higher "temperatures" (~smaller fractional powers)
  B=rep(0,size-1);
  for(r in 1:size-1){
    temp=(r-1)/(size-2);
    B[r]=Bmin^temp;
  }
  #Create a list for proposal moves
  prop=rep(0,2);
  theta=matrix(0,niter,2)
  for(t in 2:niter){
    for(c in 1:length(prop))   prop[c]=theta[t-1,c]+rnorm(1,0,0.1);
    #Calculate Log-Density at proposed and current position
    logdensity.current=logdensity(theta[t-1,])
    logdensity.prop=logdensity(prop);
    #Calculate log acceptance probability
    lalpha=B[rank]*(logdensity.prop-logdensity.current)
    if(log(runif(1))<lalpha){
      #Accept proposed move
      theta[t,]=prop;
      logdensity.current=logdensity.prop;
    }else{
      #Otherwise do not move
      theta[t,]=theta[t-1,];
    } 
    if(t%%swap.interval ==0){
      for(evenodd in 0:1){
        swap=0;
        logdensity.partner=0;
        if(rank%%2 == evenodd%%2){
          rank.partner=rank + 1;
          #ranks range from 1:size-1. Cannot have a partner rank == size
          if(0<rank.partner && rank.partner<size){
            #On first iteration, evens receive from above odd
            #On second iteration, odds receive from above evens
            logdensity.partner<-mpi.recv.Robj(rank.partner,rank.partner);
            lalpha = (B[rank]-B[rank.partner])*(logdensity.partner-logdensity.current);
            swaps.attempted=swaps.attempted+1;
            if(log(runif(1))<lalpha){
              swap=1;
              swaps.accepted=swaps.accepted+1;
            }
            mpi.send.Robj(swap,dest=rank.partner,tag=rank)
          }
          if(swap==1){
            thetaswap=theta[t,];
            mpi.send.Robj(thetaswap,dest=rank.partner,tag=rank)
            theta[t,]=mpi.recv.Robj(rank.partner,rank.partner)
          }
        }else{
          rank.partner=rank-1;
          #ranks range from 1:size-1. Cannot have a partner rank ==0
          if(0<rank.partner && rank.partner<size){
            #On first iteration, odds send to evens below
            #On second iteration, evens sent to odds below
            mpi.send.Robj(logdensity.current,dest=rank.partner,tag=rank);
            swap=mpi.recv.Robj(rank.partner,rank.partner);
          }
          if(swap==1){
            thetaswap=theta[t,];
            theta[t,]=mpi.recv.Robj(rank.partner,rank.partner);
            mpi.send.Robj(thetaswap,dest=rank.partner,tag=rank);
          }
        }
      }
    }
  }
  return(theta)
}
{% endhighlight %}

The bulk of the above code is the communication of each processor with its next nearest neighbors. Metropolis moves will be attempted every _swap.interval_ iterations, an argument one can pass to the function. When this code block is entered, even rank processors will partner with their higher ranked odd neighbours (they have a high rank so higher temperature i.e. smaller fractional power &#8211; a more &#8220;melted down&#8221; target density). The higher odd partners will send their lower even partners the value of their density and then the lower even partners will calculate an acceptance probabilty. If the move succeeds the lower rank even processors send their higher rank odd processors a binary swap=1 telling the higher rank odd processors that a send/receive procedure will occur. The lower even rank sends the higher odd rank its parameters and then subsequently the higher odd rank sends its lower even rank its parameters. In this way a metropolis move between processors is achieved. Next, odd rank processors form partners with their higher even ranked neighbours (because we need to swap with processor rank 1, the target density). The same procedure occurs as before but swapping odd for even. More visually, first swaps are attempted between 2-3, 4-5, 6-7 etc and then swaps are attempted between 1-2, 3-4, 5-6. This is almost like a merge-sort style algorithm. One can see how the parameters could be passed from 3 down to 2 and then from 2 down to 1. The main point is that each processor attempts a swap with its nearest-neighbours, the one above and the one below, every _swap.interval_ iterations.
  
With these functions defined one can now proceed to set up the mpi communicator/world.

## Rmpi


First spawn some slaves.
{% highlight R linenos %}
library(Rmpi)
mpi.spawn.Rslaves(nslaves=6)
{% endhighlight %}


If it worked, you should see something like this:
{% highlight R %}
> mpi.spawn.Rslaves(nslaves=6)
	6 slaves are spawned successfully. 0 failed.
master (rank 0, comm 1) of size 7 is running on: cabbage 
slave1 (rank 1, comm 1) of size 7 is running on: cabbage 
slave2 (rank 2, comm 1) of size 7 is running on: cabbage 
slave3 (rank 3, comm 1) of size 7 is running on: cabbage 
slave4 (rank 4, comm 1) of size 7 is running on: cabbage 
slave5 (rank 5, comm 1) of size 7 is running on: cabbage 
slave6 (rank 6, comm 1) of size 7 is running on: cabbage 
{% endhighlight %}

One can then send the function definitions to the slave processors.

{% highlight R linenos %}
niter=3000
Bmin=0.005
swap.interval=3
#Send to slaves some required data
mpi.bcast.Robj2slave(niter)
mpi.bcast.Robj2slave(Bmin)
mpi.bcast.Robj2slave(swap.interval)
#Send to slaves the logdensity function
mpi.bcast.Robj2slave(logdensity)
#Send to slaves the temper function
mpi.bcast.Robj2slave(temper) 
#Send to slaves the dmvnorm function
mpi.bcast.Robj2slave(dmvnorm) 
{% endhighlight %}

If you want to make sure that the slaves have the correct function definition, one can execute the command _mpi.remote.exec(temper)_ and this will return the function definition. That is all, now it can be run.

{% highlight R linenos %}
mcmc=mpi.remote.exec(temper(niter,Bmin,swap.interval))
{% endhighlight %}

This returns a list object containing the mcmc draws for each slave.

## Results

The end product is something that looks like this
  
[<img src="http://www.lindonslog.com/wp-content/uploads/2013/10/parallel_tempering_mixing-300x300.jpeg" alt="parallel tempering mixing" width="300" height="300" class="aligncenter size-medium wp-image-889" srcset="http://www.lindonslog.com/wp-content/uploads/2013/10/parallel_tempering_mixing-300x300.jpeg 300w, http://www.lindonslog.com/wp-content/uploads/2013/10/parallel_tempering_mixing-150x150.jpeg 150w, http://www.lindonslog.com/wp-content/uploads/2013/10/parallel_tempering_mixing.jpeg 1000w" sizes="(max-width: 300px) 100vw, 300px" />](http://www.lindonslog.com/wp-content/uploads/2013/10/parallel_tempering_mixing.jpeg)
  
Which are the draws (in black) from the target distribution. It is also useful to build up intuition for parallel tempering to look at what is happening on the other processors. The draws for all processors are shown below:
  
[<img src="http://www.lindonslog.com/wp-content/uploads/2013/10/parallel_tempering-682x1024.jpeg" alt="parallel tempering draws for each processor" width="640" height="960" class="aligncenter size-large wp-image-891" srcset="http://www.lindonslog.com/wp-content/uploads/2013/10/parallel_tempering-682x1024.jpeg 682w, http://www.lindonslog.com/wp-content/uploads/2013/10/parallel_tempering-200x300.jpeg 200w, http://www.lindonslog.com/wp-content/uploads/2013/10/parallel_tempering.jpeg 1000w" sizes="(max-width: 640px) 100vw, 640px" />](http://www.lindonslog.com/wp-content/uploads/2013/10/parallel_tempering.jpeg)

N.B. Although my computer only has 8 cores I tried running the code 12 slaves. At first I was concerned that the MPI communications would enter a deadlock and the code would hang but it didn&#8217;t, so it seems you can scale up the number of slaves above the number of cores.

## Temperature Set

Notice that the temperature set used in the code has the property that <img src="//s0.wp.com/latex.php?latex=%5Cfrac%7B%5Cbeta_%7Bn%7D%7D%7B%5Cbeta_%7Bn%2B1%7D%7D%3Dc&#038;bg=ffffff&#038;fg=000&#038;s=0" alt="&#92;frac{&#92;beta_{n}}{&#92;beta_{n+1}}=c" title="&#92;frac{&#92;beta_{n}}{&#92;beta_{n+1}}=c" class="latex" />, for c a constant. There is a paper by Kofke(2002) that justifies this temperature set as it yields a constant acceptance ratio between cores under certain conditions. Indeed, the acceptance ratio (the fraction of metropolis moves that succeeded between cores) are roughly constant, as shown below:

{% highlight R linenos %}
[1] 0.7227723
[1] 0.7926793
[1] 0.710171
[1] 0.8037804
[1] 0.7191719
[1] 0.7974797
[1] 0.729673
[1] 0.8223822
[1] 0.8184818
[1] 0.8445845
{% endhighlight %}

<span class="Z3988" title="ctx_ver=Z39.88-2004&#038;rft_val_fmt=info%3Aofi%2Ffmt%3Akev%3Amtx%3Ajournal&#038;rft_id=info%3Adoi%2F10.1039%2Fb509983h&#038;rft.atitle=Parallel+tempering%3A+Theory%2C+applications%2C+and+new+perspectives&#038;rft.jtitle=Physical+Chemistry+Chemical+Physics&#038;rft.artnum=http%3A%2F%2Fxlink.rsc.org%2F%3FDOI%3Db509983h&#038;rft.volume=7&#038;rft.issue=23&#038;rft.issn=1463-9076&#038;rft.spage=3910&#038;rft.date=2005&#038;rfr_id=info%3Asid%2Fscienceseeker.org&#038;rft.au=Earl+David+J.&#038;rft.aulast=Earl&#038;rft.aufirst=David+J.&#038;rft.au=Deem+Michael+W.&#038;rft.aulast=Deem&#038;rft.aufirst=Michael+W.&#038;rfs_dat=ss.included=1&#038;rfe_dat=bpr3.included=1;bpr3.tags=Chemistry%2CComputer+Science+%2F+Engineering%2CMathematics%2CPhysics">Earl D.J. & Deem M.W. (2005). Parallel tempering: Theory, applications, and new perspectives, <span style="font-style:italic;">Physical Chemistry Chemical Physics, 7</span> (23) 3910. DOI: <a rel="author" href="http://dx.doi.org/10.1039%2Fb509983h">10.1039/b509983h</a></span>
  
<span class="Z3988" title="ctx_ver=Z39.88-2004&#038;rft_val_fmt=info%3Aofi%2Ffmt%3Akev%3Amtx%3Ajournal&#038;rft_id=info%3Adoi%2F10.1063%2F1.1507776&#038;rft.atitle=On+the+acceptance+probability+of+replica-exchange+Monte+Carlo+trials&#038;rft.jtitle=The+Journal+of+Chemical+Physics&#038;rft.artnum=http%3A%2F%2Flink.aip.org%2Flink%2FJCPSA6%2Fv117%2Fi15%2Fp6911%2Fs1%26Agg%3Ddoi&#038;rft.volume=117&#038;rft.issue=15&#038;rft.issn=00219606&#038;rft.spage=6911&#038;rft.date=2002&#038;rfr_id=info%3Asid%2Fscienceseeker.org&#038;rft.au=Kofke+David+A.&#038;rft.aulast=Kofke&#038;rft.aufirst=David+A.&#038;rfs_dat=ss.included=1&#038;rfe_dat=bpr3.included=1;bpr3.tags=Chemistry%2CComputer+Science+%2F+Engineering%2CPhysics">Kofke D.A. (2002). On the acceptance probability of replica-exchange Monte Carlo trials, <span style="font-style:italic;">The Journal of Chemical Physics, 117</span> (15) 6911. DOI: <a rel="author" href="http://dx.doi.org/10.1063%2F1.1507776">10.1063/1.1507776</a></span>
