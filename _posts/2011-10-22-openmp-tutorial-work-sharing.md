---
id: 65
title: OpenMP Parallel For
date: 2011-10-22T20:41:12+00:00
author: admin
layout: post
guid: http://www.lindonslog.com/?p=65
permalink: /programming/openmp/openmp-tutorial-work-sharing/
categories:
  - OpenMP
---
The parallel directive `#pragma omp parallel` makes the code parallel, that is, it forks the master thread into a number of parallel threads, but it doesn&#8217;t actually share out the work.
  
What we are really after is the parallel for directive, which we call a **work-sharing** construct. Consider

<pre class="brush: cpp; title: ; notranslate" title="">#include &lt;iostream&gt;
#include &lt;omp.h&gt;

using namespace std;
main (void)
{
	int i;
	int foo;
#pragma omp parallel for
  for(i=1;i&lt;10;i++){
#pragma omp critical
{
foo=omp_get_thread_num();
	  cout &lt;&lt; "Loop number: "&lt;&lt; i &lt;&lt; " " &lt;&lt; "Thread number: " &lt;&lt; foo &lt;&lt; endl;
}
}
}
</pre>

The for directive applies to the for loop immediately preceding it. Notice how we don&#8217;t have to outline a parallel region with curly braces {} following this directive in contrast to before. This program yields:

<pre class="brush: bash; title: ; notranslate" title="">[michael@michael lindonslog]$ ./openmp 
Loop number: 1 Thread number: 0
Loop number: 8 Thread number: 3
Loop number: 2 Thread number: 0
Loop number: 3 Thread number: 0
Loop number: 9 Thread number: 3
Loop number: 6 Thread number: 2
Loop number: 4 Thread number: 1
Loop number: 7 Thread number: 2
Loop number: 5 Thread number: 1
[michael@michael lindonslog]$ 
</pre>

Notice what I said about the order. By default, the loop index i.e. &#8220;i&#8221; in this context, is made private by the for directive.

At the end of the parallel for loop, there is an implicit barrier where all threads wait until they have all finished. There are however some rules for the parallel for directive

  1. The loop index, i, is incremented by a fixed amount each iteration e.g. i++ or i+=step.
  2. The start and end values must not change during the loop.
  3. There must be no &#8220;breaks&#8221; in the loop where the code steps out of that code block. Functions are, however, permitted and run as you would expect.
  4. The comparison operators may be < <= => >

There may be times when you want to perform some operation in the order of the iterations. This can be achieved with an ordered directive and an ordered clause. Each thread will wait until the previous iteration has finished it&#8217;s ordered section before proceeding with its own.

<pre class="brush: cpp; title: ; notranslate" title="">int main(void){
int i,a[10];
#pragma omp parallel for ordered 
for(i=0;i&lt;10;i++){
a[i]=expensive_function(i);
#pragma omp ordered
printf("Thread ID: %d    Hello World %d\n",omp_get_thread_num(),i);
}
}
</pre>

Will now print out the Hello Worlds in order. N.B. There is a penalty for this. The threads have to wait until the preceding iteration has finished with its ordered section of code. Only if the expensive_function() in this case were expensive, would this be worthwhile.