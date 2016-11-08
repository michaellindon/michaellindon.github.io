---
id: 562
title: Parallel Random Number Generation using TRNG
date: 2013-07-10T17:46:39+00:00
author: admin
layout: post
guid: http://www.lindonslog.com/?p=562
permalink: /programming/parallel-random-number-generation-trng/
categories:
  - Programming
  - R
---
To my surprise and disappointment, popular scientific libraries like [Boost](http://www.boost.org/doc/libs/1_54_0/doc/html/boost_random.html "Random Number Libraries") or [GSL](http://www.gnu.org/software/gsl/ "GNU Scientific Library") provide no native support for parallel random number generation. Recently I came across TRNG, an excellent random number generation library for C++ built specifically with parallel architectures in mind. Over the last few days I&#8217;ve been trawling internet forums and reading discussions about the best parallel random number generation libraries. Given the trend in CPU architectures whereby each contains an ever increasing number of cores, it makes sense to start a project by considering what libraries are available to best make use of this technology. The first libraries I came across were <a href="http://statmath.wu.ac.at/software/RngStreams/doc/rngstreams.html" rel="nofollow">RngStream</a> and <a href="http://www.sprng.org/" title="SPRNG" rel="nofollow">SPRNG</a>. It seems SPRNG was built specifically with MPI, i.e. for distributed memory architectures, in mind and there are some excellent examples and resources of how to get started with <a href="http://darrenjw.wordpress.com/2010/12/14/getting-started-with-parallel-mcmc/" rel="nofollow">parallel MCMC on Darren Wilkinson&#8217;s blog</a>. As a result, it seems a bit contrived to get SPRNG to work with OpenMP, i.e. for shared memory architectures. Moreover, I specifically wanted to use OpenMP because I wanted to write an extension for R for use on personal computers. RngStream is written by the man himself, Pierre L&#8217;Ecuyer, and is much more OpenMP amenable. Both of these, however, only generate uniform pseudo random numbers. This isn&#8217;t a fault, but it means you need to code up transformations and samplers to generate non-uniform pseudo random numbers. While this would be a good exercise, life is short, and I&#8217;d rather leave this sort of thing to the professionals (I don&#8217;t want to code up my own Ziggurat algorithm). Also, the generators or engines are of defined types and I found it hard to convert them into the corresponding types of other libraries like Boost or GSL so that I could use their non-uniform generation code. That probably says more about my ability rather than the actual difficulty of the problem and Darren Wilkinson provides some of his own code for getting the RNG engine of SPRNG into the correct datatype to be compatible with GSL. 

## TRNG

At this point I was quite discouraged but then I came across <a href="http://numbercrunch.de/trng/" title="Tina's Random Number Generator" rel="nofollow">TRNG</a>, written by Heiko Bauke. At first glance TRNG is an excellently documented C++ PRNG (which stands for pseudo random number generator, not parallel, that would be PPRNG) library built specifically with parallel architectures in mind. Not only does it provide non-uniform distributions, but it can be used easily with MPI, OpenMP, CUDA and TBB, for which many examples are supplied. The documentation is excellent and the many examples of the same problem coded with each of the aforementioned parallelization methods are enlightening. If that weren&#8217;t enough, TRNG can be used in combination and interchangeably with the Boost random as well as the C++11 TR1 random libraries, that is, the engines/generators from TRNG can be used with the distribution functions of Boost and C++11 TR1, which was a problem I encountered with RngStream and SPRNG. The way TRNG and RngStream work are slightly different. Whereas RngStream generates multiple independent streams, TRNG uses a single stream and either divides it into blocks, or interleaves it between different processors by a leap-frog type scheme, much like dealing out cards round a table. The point of all this is that the streams of different processors never overlap, otherwise one would get the same draws on processor A as processor B. While purists might argue that L&#8217;Ecuyer&#8217;s method is more rigorous, I&#8217;m happy enough with the way Heiko has done it, especially given TRNG&#8217;s out-of-box easy of use and compatibility.

### Installation of TRNG

Clone the repository off Github.

<pre class="brush: bash; title: ; notranslate" title="">[michael@michael$git clone https://github.com/rabauke/trng4
[michael@michael$cd trng4/
[michael@michael trng4]$./configure --prefix=/usr
[michael@michael trng4]$make
[michael@michael trng4]$make inst
[michael@michael trng4]$ sudo bash
[sudo] password for michael: 
[root@michael trng4]# ldconfig
[root@michael trng4]#
</pre>

the &#8220;&#8211;prefix=&#8221; argument just sets where I want the files to be installed and is not necessary. If omitted the default case is /usr/local. After make install, run ldconfig as root in order to update the dynamic linker/loader with the presence of the new library. 

Basically there exists a cache /etc/ld.so.cache which is used by the dynamic linker/loader at run-time as a cross-reference for a library&#8217;s soname with its full file path. ldconfig is normally run during booting but can also be run anytime to update the cache with the locations of new libraries. Here is what happens if you don&#8217;t run ldconfig, as I did the first time.

<pre class="brush: bash; title: ; notranslate" title="">[michael@michael ~]$ g++ hello_world.cc -L /usr/lib -ltrng4
[michael@michael ~]$ ./a.out
./a.out: error while loading shared libraries: libtrng4.so.0: cannot open shared object file: No such file or directory
</pre>

It compiled fine, but at run-time the loader couldn&#8217;t find the library.

## Parallel Random Number Generation in C++

**Nachtrag:** I think instead of using trng::yarn2 gen[max] it is better to do:

<pre class="brush: cpp; title: ; notranslate" title="">trng::yarn2 * gen;
gen=new trng::yarn2[max];
</pre>

The approach will be to generate the the PRNGs in C++ and call it from R using [Rcpp](http://www.lindonslog.com/programming/r/rcpp/ "Calling C++ from R using Rcpp"). First lets consider the C++ code to generate some random uniforms.

<pre class="brush: cpp; title: ; notranslate" title="">#include &lt;cstdlib&gt;
#include &lt;iostream&gt;
#include &lt;omp.h&gt;
#include &lt;trng/yarn2.hpp&gt;
#include &lt;trng/uniform01_dist.hpp&gt;

int main() {

	int max=omp_get_max_threads();
	omp_set_num_threads(max);
	int rank;
	trng::yarn2 gen[max];
	trng::uniform01_dist&lt;&gt; u;
	std::cout &lt;&lt; max &lt;&lt; " =max num of threads" &lt;&lt; std::endl;


	for (int i = 0; i &lt; max; i++)
	{
		gen[i].split(max,i);
	}


#pragma omp parallel for private(rank)
	for (int i = 0; i &lt; max; ++i)
	{
		rank=omp_get_thread_num();
#pragma omp critical
	std::cout &lt;&lt; u(gen[rank]) &lt;&lt; " from thread " &lt;&lt; rank &lt;&lt;  std::endl;
	}
	return EXIT_SUCCESS;
}

</pre>

which returns

<pre class="brush: bash; title: ; notranslate" title="">[michael@michael ~]$ g++ omprng.cpp -o omprng -fopenmp -ltrng4
[michael@michael ~]$ ./omprng 
4 =max num of threads
0.919233 from thread 0
0.408994 from thread 1
0.943502 from thread 2
0.401236 from thread 3
[michael@michael ~]$ 
</pre>

The salient feature of this code is the leapfrog process by calling split. There exists a sequence of random uniforms and &#8220;.split(max,i)&#8221; divides it into _max_ subsequences, leap-frogging each other, and grab the _i&#8217;th_ subsequence. You can think of this as _max_ players sitting around a poker table and the .split() as continuously dealing out random uniforms to each of the players. The code says let processor i be &#8220;player&#8221; i and use the sequence of random uniforms dealt to it.

## Parallel Random Number Generation in R using Rcpp

Thanks to Rcpp the above C++ code can be trivially changed so that it can be used from R. Just include the Rcpp header and change the function return type.

<pre class="brush: bash; title: ; notranslate" title="">#include &lt;cstdlib&gt;
#include &lt;iostream&gt;
#include &lt;omp.h&gt;
#include &lt;trng/yarn2.hpp&gt;
#include &lt;trng/uniform01_dist.hpp&gt;
#include &lt;Rcpp.h&gt;

// [[Rcpp::export]]
Rcpp::NumericVector prunif(int n) {

	int max=omp_get_max_threads();
	omp_set_num_threads(max);
	int rank;
	trng::yarn2 gen[max];
	trng::uniform01_dist&lt;&gt; u;
	Rcpp::NumericVector draws(n);

	for (int i = 0; i &lt; max; i++)
	{
		gen[i].split(max,i);
	}


#pragma omp parallel for private(rank)
	for (int i = 0; i &lt; n; ++i)
	{
		rank=omp_get_thread_num();
		draws[i]=u(gen[rank]);
	}

	return draws;
}
</pre>

This code can be compiled and loaded into R on the fly, so lets test it.

### Speedup Performance

<pre class="brush: r; title: ; notranslate" title="">&gt; library(Rcpp)
&gt; library(rbenchmark) 
&gt; Sys.setenv("PKG_CXXFLAGS"="-fopenmp")
&gt; Sys.setenv("PKG_LIBS"="-ltrng4")
&gt; sourceCpp("prunif.cpp")
&gt; benchmark(replications=rep(100,0,1),runif(1000000),prunif(1000000))
           test replications elapsed relative user.self sys.self user.child
2 prunif(1e+06)          100   0.611     1.00     2.227    0.114          0
1  runif(1e+06)          100   3.837     6.28     3.745    0.086          0
</pre>

<div id="attachment_583" style="width: 490px" class="wp-caption aligncenter">
  <a href="http://www.lindonslog.com/wp-content/uploads/2013/07/speedup_prunif.png"><img src="http://www.lindonslog.com/wp-content/uploads/2013/07/speedup_prunif.png" alt="Speedup" width="480" height="480" class="size-full wp-image-583" srcset="http://www.lindonslog.com/wp-content/uploads/2013/07/speedup_prunif.png 480w, http://www.lindonslog.com/wp-content/uploads/2013/07/speedup_prunif-150x150.png 150w, http://www.lindonslog.com/wp-content/uploads/2013/07/speedup_prunif-300x300.png 300w" sizes="(max-width: 480px) 100vw, 480px" /></a>
  
  <p class="wp-caption-text">
    Parallel RNG speedup
  </p>
</div>


  
There are a few things to note. Spawning threads incurs its own overhead, so it will obviously be slower for very few draws. As the number of draws becomes larger the time taken to spawn new threads is dwarfed by the time taken to create the draws and so it is worthwhile to do it in parallel. One caveat is that prunif and runif did not in this case use the same generating algorithm. R&#8217;s algorithm can be changed with RNG.kind and the TRNG algorithm can be changed by using an alternative to yarn in &#8220;trng::yarn2&#8221;. Even if they were the same though I would expect the same qualitative behaviour.

## Discussion

Generating large samples of random numbers in one hit quickly is not the reason why I started looking for a good parallel random number generator. Rarely is it important to me to generate large amount of draws in one go but it certainly is important to me to have independent streams. Generally I will port expensive parts of my R code, usually for loops, to C++ and inevitably I will somewhere within these for loops or other expensive parts of code need to draw some random numbers. Since these expensive pieces of code are self-evidently _expensive_, I will want to compute them in parallel in C++ if I can and so it is very important to me to have independent streams from which to draw random numbers.