---
id: 960
title: C++11 versus R Standalone Random Number Generation Performance Comparison
date: 2014-03-01T16:00:25+00:00
author: admin
layout: post
guid: http://www.lindonslog.com/?p=960
permalink: /linux-unix/c11-r-standalone-random-number-generation-performance-comparison/
categories:
  - Linux/Unix
  - Programming
  - R
  - Statistics
tags:
  - C++11
  - PRNG
  - pseudo random number generation
  - R
  - RNG
  - standalone
---
If you are writing some C++ code with the intent of calling it from R or even developing it into a package you might wonder whether it is better to use the pseudo random number library native to C++11 or the R standalone library. On the one hand users of your package might have an outdated compiler which doesn&#8217;t support C++11 but on the other hand perhaps there are potential speedups to be won by using the <random> library native to C++11. I decided to compare the performance of these two libraries.

<pre class="brush: cpp; title: ; notranslate" title="">#define MATHLIB_STANDALONE
#include &lt;iostream&gt;
#include &lt;vector&gt;
#include &lt;random&gt;
#include &lt;chrono&gt;
#include "Rmath.h"

int main(int argc, char *argv[])
{
        int ndraws=100000000;
        std::vector&lt;double&gt; Z(ndraws);
        std::mt19937 engine;
        std::normal_distribution&lt;double&gt; N(0,1);

        auto start = std::chrono::steady_clock::now();
        for(auto & z : Z ) {
                z=N(engine);
        }
        auto end = std::chrono::steady_clock::now();
        std::chrono::duration&lt;double&gt; elapsed=end-start;

        std::cout &lt;&lt;  elapsed.count() &lt;&lt; " seconds - C++11" &lt;&lt; std::endl;

        start = std::chrono::steady_clock::now();
        GetRNGstate();
        for(auto & z : Z ) {
                z=rnorm(0,1);
        }
        PutRNGstate();
        end = std::chrono::steady_clock::now();
        elapsed=end-start;

        std::cout &lt;&lt;  elapsed.count() &lt;&lt; " seconds - R Standalone" &lt;&lt; std::endl;

        return 0;
}
</pre>

Compiling and run with:

<pre class="brush: bash; title: ; notranslate" title="">[michael@michael coda]$ g++ normal.cpp -o normal -std=c++11 -O3 -lRmath
[michael@michael coda]$ ./normal 
</pre>

## Normal Generation

<pre class="brush: bash; title: ; notranslate" title="">5.2252 seconds - C++11
6.0679 seconds - R Standalone
</pre>

## Gamma Generation

<pre class="brush: bash; title: ; notranslate" title="">11.2132 seconds - C++11
12.4486 seconds - R Standalone
</pre>

## Cauchy

<pre class="brush: bash; title: ; notranslate" title="">6.31157 seconds - C++11
6.35053 seconds - R Standalone
</pre>

As expected the C++11 implementation is faster but not by a huge amount. As the computational cost of my code is dominated by other linear algebra procedures of O(n^3) I&#8217;d actually be willing to use the R standalone library because the syntax is more user friendly.