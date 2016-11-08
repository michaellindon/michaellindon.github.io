---
id: 56
title: Parallel Programming in C with OpenMP
date: 2011-10-22T19:56:12+00:00
author: admin
layout: post
guid: http://www.lindonslog.com/?p=56
permalink: /programming/openmp/openmp-tutorial-thinking-parallel/
categories:
  - OpenMP
tags:
  - c
  - openmp
  - parallel
  - programming
---
## OpenMP &#8211; Open Specifications for Multi Processing

The central theme of parallel code is that of threads. A serial code starts off as one thread. As soon as the first parallel directive(Fortran)/pragma(C) is encountered, the master thread forks into a number of threads. The proceeding code is then executed in parallel in a manner which can be adjusted using certain options.

To get started with OpenMP. You will need to include the omp header file with
   
`#include <omp.h>`
   
and you will need to add the -fopenmp option when compiling.

To fork the master thread into a number of parallel threads, one writes the following line of code:
  
`#pragma omp parallel`
  
This directive will apply to the following block of code, {&#8230;}, only and must be structured as such. By default, all variables previously declared are shared i.e. all threads have the same memory address of a shared variable. This can, however, be declared explicitly by adding `shared(var_name)`. Conversely, you may want to make variables private, that is, each thread gets allocated a unique location in the memory to store this variable. Private variables are only accessed by the threads they are in and all the additional copies of the variable created for parallisation are destroyed when the threads merge. There are also reduction variables. More on that later&#8230;

Lets try an example. When you execute your code, it will inherit the OMP\_NUM\_THREADS environment variable of your terminal. Suppose we want to set the number of threads to 4. We write

<pre class="brush: bash; title: ; notranslate" title="">prog@michael-laptop:~$ export OMP_NUM_THREADS=4
prog@michael-laptop:~$ echo $OMP_NUM_THREADS
4
prog@michael-laptop:~$ 
</pre>

You can also specify the number of threads during run time with the omp\_set\_num_threads() function defined in omp.h

Good. Now here&#8217;s our sample code:

<pre class="brush: cpp; title: ; notranslate" title="">#include &lt;stdio.h&gt;
#include &lt;stdlib.h&gt;
#include &lt;omp.h&gt;

int main(void){
printf("Number of threads before parallisation: %d\n",omp_get_num_threads());
#pragma omp parallel 
{
printf("Current thread number: %d\n",omp_get_thread_num());
if(omp_get_thread_num()==0)
{
printf("Number of threads after parallisation: %d\n",omp_get_num_threads());
}
}
printf("Number of threads after merging threads %d\n",omp_get_num_threads());
}
</pre>

compile and run:

<pre class="brush: bash; title: ; notranslate" title="">prog@michael-laptop:~$ g++ openmp.cpp -o test -fopenmp
prog@michael-laptop:~$ ./test
Number of threads before parallisation: 1
Current thread number: 3
Current thread number: 1
Current thread number: 0
Number of threads after parallisation: 4
Current thread number: 2
Number of threads after merging threads 1
</pre>

Next I&#8217;ll talk about work sharing&#8230;