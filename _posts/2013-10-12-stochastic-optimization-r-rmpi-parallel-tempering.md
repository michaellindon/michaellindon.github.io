---
id: 910
title: Stochastic Optimization in R by Parallel Tempering
date: 2013-10-12T17:24:04+00:00
author: admin
layout: post
guid: http://www.lindonslog.com/?p=910
permalink: /programming/stochastic-optimization-r-rmpi-parallel-tempering/
categories:
  - Programming
  - R
  - Statistics
tags:
  - optimization
  - parallel
  - stochastic
  - tempering
---

  
I&#8217;ve written a few posts now about using [parallel tempering](http://www.lindonslog.com/programming/openmp/parallel-tempering-algorithm-c/ "Parallel Tempering Algorithm with OpenMP / C++") to sample from complicated multi-modal target distributions but there are also other benefits and uses to this algorithm. There is a nice post on Darren Wilkinson&#8217;s blog about using [tempered posteriors for marginal likelihood](http://darrenjw.wordpress.com/2013/10/01/marginal-likelihood-from-tempered-bayesian-posteriors/) calculations. There is also another area where parallel tempering finds application, namely in stochastic optimization. I first encountered parallel tempering whilst doing my MPhys degree at the University of Warwick but at that time it was employed as a stochastic optimization algorithm to find the minimum energy configuration of a Lennard-Jones cluster as opposed to a sampling algorithm. All that is required is one observation to turn this sampling algorithm into a stochastic optimization algorithm. Lets break this observation down into a few steps.
  
Consider sampling from a simple exponential distribution $$f(E)\propto e^{-\beta E}1_{(0,\infty )}(E),$$
  
with rate parameter beta. For now lets fix beta=5. One could sample from this distribution using the same [Rmpi parallel tempering code](http://www.lindonslog.com/mathematics/parallel-tempering-r-rmpi/ "Parallel Tempering Code in R Rmpi") given in my previous post by simply changing the target distribution to the exponential above. The histograms of mcmc draws from four tempered distribution would then look something like this:
  
[<img src="http://www.lindonslog.com/wp-content/uploads/2013/10/exphist-300x300.jpeg" alt="Histogram of mcmc draws" width="300" height="300" class="aligncenter size-medium wp-image-920" srcset="http://www.lindonslog.com/wp-content/uploads/2013/10/exphist-300x300.jpeg 300w, http://www.lindonslog.com/wp-content/uploads/2013/10/exphist-150x150.jpeg 150w, http://www.lindonslog.com/wp-content/uploads/2013/10/exphist.jpeg 1000w" sizes="(max-width: 300px) 100vw, 300px" />](http://www.lindonslog.com/wp-content/uploads/2013/10/exphist.jpeg)
  
Note the scale on the x-axis. The two important observations mentioned earlier are 

  * The minimum value of E occurs most frequently as it the mode of the target distribution 
  * The greater the rate parameter, the more concentrated the distribution is around E-min
The second point is important because although the sampling algorithm is creating draws that are not the minimum value of E, by increasing the rate parameter one can force these draws to be arbitrarily close to E-min.

## A Uni-modal Optimization Function

How does this relate to optimization? Consider setting $$E(\theta)=(\theta-40)^2$$ Whereas before where using the Metropolis algorithm one would propose a new value of E, say E&#8217;, now the proposal is made in θ, and θ&#8217; is accepted based on u < f(E(θ')) / f(E(θ)). By construction the algorithm gives draws close to E-min, which occurs when θ=40. The traceplot of θ is shown below: [<img src="http://www.lindonslog.com/wp-content/uploads/2013/10/unioptim-300x300.jpeg" alt="Stochastic Optimization mcmc traceplot" width="350" height="350" class="aligncenter size-medium wp-image-929" srcset="http://www.lindonslog.com/wp-content/uploads/2013/10/unioptim-300x300.jpeg 300w, http://www.lindonslog.com/wp-content/uploads/2013/10/unioptim-150x150.jpeg 150w, http://www.lindonslog.com/wp-content/uploads/2013/10/unioptim.jpeg 600w" sizes="(max-width: 350px) 100vw, 350px" />](http://www.lindonslog.com/wp-content/uploads/2013/10/unioptim.jpeg)
  
Click here for the [code](http://www.lindonslog.com/example_code/unioptim.R).

## A Harder Optimization Function

The above quadratic was an easy uni-modal example. Let&#8217;s try a harder function. Consider the minimum of $$ E(\theta)=3sin(\theta)+(0.1\theta-3)^2,$$ which looks like this:
  
[<img src="http://www.lindonslog.com/wp-content/uploads/2013/10/optimtest-300x185.jpeg" alt="Optimization Function" width="300" height="185" class="aligncenter size-medium wp-image-932" srcset="http://www.lindonslog.com/wp-content/uploads/2013/10/optimtest-300x185.jpeg 300w, http://www.lindonslog.com/wp-content/uploads/2013/10/optimtest.jpeg 894w" sizes="(max-width: 300px) 100vw, 300px" />](http://www.lindonslog.com/wp-content/uploads/2013/10/optimtest.jpeg)
  
This function has infinitely many local minima but one global minimum around 30. Local minima make optimization challenging and many optimization algorithms get stuck in these regions as locally it appears the minimum has been reached. This is where the parallel tempering really helps. The traceplots of theta are shown for six tempered distributions below:
  
[<img src="http://www.lindonslog.com/wp-content/uploads/2013/10/hardoptim-682x1024.jpeg" alt="optimization traceplots mcmc" width="640" height="960" class="aligncenter size-large wp-image-938" srcset="http://www.lindonslog.com/wp-content/uploads/2013/10/hardoptim-682x1024.jpeg 682w, http://www.lindonslog.com/wp-content/uploads/2013/10/hardoptim-200x300.jpeg 200w, http://www.lindonslog.com/wp-content/uploads/2013/10/hardoptim.jpeg 1000w" sizes="(max-width: 640px) 100vw, 640px" />](http://www.lindonslog.com/wp-content/uploads/2013/10/hardoptim.jpeg)
  
Click here for the [code](http://www.lindonslog.com/example_code/multioptim.R).

I&#8217;m currently working on another example just for fun, namely finding the lowest energy configuration of an n-particle Lennard-Jones cluster. This is a nice example because one can visualize the process using <a href="http://www.ks.uiuc.edu/Research/vmd/" rel="external nofollow">vmd</a> and it also provides some insight into the origins of such terminology as &#8220;tempering&#8221;, &#8220;annealing&#8221; and &#8220;temperature&#8221; which always look somewhat out of place in the statistics literature. 

## An Even Harder Function

Consider the function
  
$$ E(\theta)=10\sin(0.3\theta)\sin(1.3\theta^2) + 0.00001\theta^4 + 0.2\theta+80, $$
  
which is shown below.
  
[<img src="http://www.lindonslog.com/wp-content/uploads/2013/10/wildfunction.jpeg" alt="Optimization Function" width="600" height="500" class="aligncenter size-full wp-image-945" srcset="http://www.lindonslog.com/wp-content/uploads/2013/10/wildfunction.jpeg 600w, http://www.lindonslog.com/wp-content/uploads/2013/10/wildfunction-300x250.jpeg 300w" sizes="(max-width: 600px) 100vw, 600px" />](http://www.lindonslog.com/wp-content/uploads/2013/10/wildfunction.jpeg)

The trace-plots for the parallel tempering optimization are shown below
  
[<img src="http://www.lindonslog.com/wp-content/uploads/2013/10/goodwild-682x1024.jpeg" alt="Parallel Tempering Optimization" width="640" height="960" class="aligncenter size-large wp-image-946" srcset="http://www.lindonslog.com/wp-content/uploads/2013/10/goodwild-682x1024.jpeg 682w, http://www.lindonslog.com/wp-content/uploads/2013/10/goodwild-200x300.jpeg 200w, http://www.lindonslog.com/wp-content/uploads/2013/10/goodwild.jpeg 1000w" sizes="(max-width: 640px) 100vw, 640px" />](http://www.lindonslog.com/wp-content/uploads/2013/10/goodwild.jpeg)

Examining the mcmc draws the minimum is obtained at theta=-15.81515.

<span class="Z3988" title="ctx_ver=Z39.88-2004&#038;rft_val_fmt=info%3Aofi%2Ffmt%3Akev%3Amtx%3Ajournal&#038;rft_id=info%3Adoi%2F10.1016%2Fj.amc.2009.02.023&#038;rft.atitle=Hybrid+parallel+tempering+and+simulated+annealing+method&#038;rft.jtitle=Applied+Mathematics+and+Computation&#038;rft.artnum=http%3A%2F%2Flinkinghub.elsevier.com%2Fretrieve%2Fpii%2FS0096300309001362&#038;rft.volume=212&#038;rft.issue=1&#038;rft.issn=00963003&#038;rft.spage=216&#038;rft.epage=228&#038;rft.date=2009&#038;rfr_id=info%3Asid%2Fscienceseeker.org&#038;rft.au=Li+Yaohang&#038;rft.aulast=Li&#038;rft.aufirst=Yaohang&#038;rft.au=Protopopescu+Vladimir+A.&#038;rft.aulast=Protopopescu&#038;rft.aufirst=Vladimir+A.&#038;rft.au=Arnold+Nikita&#038;rft.aulast=Arnold&#038;rft.aufirst=Nikita&#038;rft.au=Zhang+Xinyu&#038;rft.aulast=Zhang&#038;rft.aufirst=Xinyu&#038;rft.au=Gorin+Andrey&#038;rft.aulast=Gorin&#038;rft.aufirst=Andrey&#038;rfs_dat=ss.included=1&#038;rfe_dat=bpr3.included=1;bpr3.tags=Chemistry%2CComputer+Science+%2F+Engineering%2CMathematics%2CPhysics">Li Y., Protopopescu V.A., Arnold N., Zhang X. & Gorin A. (2009). Hybrid parallel tempering and simulated annealing method, <span style="font-style:italic;">Applied Mathematics and Computation, 212</span> (1) 216-228. DOI: <a rel="author" href="http://dx.doi.org/10.1016%2Fj.amc.2009.02.023">10.1016/j.amc.2009.02.023</a></span>