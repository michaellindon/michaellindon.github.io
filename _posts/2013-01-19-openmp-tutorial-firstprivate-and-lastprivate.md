---
id: 87
title: 'OpenMP Tutorial &#8211; firstprivate and lastprivate'
date: 2013-01-19T23:39:18+00:00
author: admin
layout: post
guid: http://www.lindonslog.com/?p=87
permalink: /programming/openmp/openmp-tutorial-firstprivate-and-lastprivate/
categories:
  - OpenMP
---
Here I will consider firstprivate and lastprivate. Recall one of the earlier entries about private variables. When a variable is declared as private, each thread gets a unique memory address of where to store values for that variable while in the parallel region. When the parallel region ends, the memory is freed and these variables no longer exist. Consider the following bit of code as an example:

<pre class="brush: cpp; title: ; notranslate" title="">#include &lt;stdio.h&gt;
#include &lt;stdlib.h&gt;
#include &lt;omp.h&gt;


int main(void){
int i;
int x;
x=44;
#pragma omp parallel for private(x) 
for(i=0;i&lt;=10;i++){
x=i;
printf("Thread number: %d     x: %d\n",omp_get_thread_num(),x);
}
printf("x is %d\n", x);


}
</pre>

Yields&#8230;

<pre class="brush: bash; title: ; notranslate" title="">Thread number: 0     x: 0
Thread number: 0     x: 1
Thread number: 0     x: 2
Thread number: 3     x: 9
Thread number: 3     x: 10
Thread number: 2     x: 6
Thread number: 2     x: 7
Thread number: 2     x: 8
Thread number: 1     x: 3
Thread number: 1     x: 4
Thread number: 1     x: 5
x is 44
</pre>

You&#8217;ll notice that x is exactly the value it was before the parallel region.

Suppose we wanted to keep the last value of x after the parallel region. This can be achieved with lastprivate. Replace private(x) with lastprivate(x) and this is the result:

<pre class="brush: bash; title: ; notranslate" title="">Thread number: 3     x: 9
Thread number: 3     x: 10
Thread number: 1     x: 3
Thread number: 1     x: 4
Thread number: 1     x: 5
Thread number: 0     x: 0
Thread number: 0     x: 1
Thread number: 0     x: 2
Thread number: 2     x: 6
Thread number: 2     x: 7
Thread number: 2     x: 8
x is 10
</pre>

Notice that it is 10 and not 8. That is to say, it is the last iteration which is kept, not the last operation. Now what if we replace lastprivate(x) with firstprivate(x). What do you think it will do? This:

<pre class="brush: bash; title: ; notranslate" title="">Thread number: 3     x: 9
Thread number: 3     x: 10
Thread number: 1     x: 3
Thread number: 1     x: 4
Thread number: 1     x: 5
Thread number: 0     x: 0
Thread number: 0     x: 1
Thread number: 0     x: 2
Thread number: 2     x: 6
Thread number: 2     x: 7
Thread number: 2     x: 8
x is 44
</pre>

If you were like me, you were expecting to get the value 0 i.e. the value of x on the first iteration. **NO** 

> firstprivate Specifies that each thread should have its own instance of a variable, and that the variable should be initialized with the value of the variable, because it exists before the parallel construct.

That is, every thread gets its own instance of x and that instance equals 44.