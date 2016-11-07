---
id: 78
title: 'OpenMP Tutorial &#8211; Critical, Atomic and Reduction'
date: 2011-10-23T09:32:04+00:00
author: admin
layout: post
guid: http://www.lindonslog.com/?p=78
permalink: /programming/openmp/openmp-tutorial-critical-atomic-and-reduction/
categories:
  - OpenMP
  - Programming
---
## Atomic and Critical

>   * _critical_: the enclosed code block will be executed by only one thread at a time, and not simultaneously executed by multiple threads. It is often used to protect shared data fromrace conditions.
>   * _atomic_: the memory update (write, or read-modify-write) in the next instruction will be performed atomically. It does not make the entire statement atomic; only the memory update is atomic. A compiler might use special hardware instructions for better performance than when using _critical_.

Consider this code which numerically approximates pi:

<pre class="brush: cpp; title: ; notranslate" title="">int main(void){
double pi,x;
int i,N;
pi=0.0;
N=1000;
#pragma omp parallel for
for(i=0;i x=(double)i/N;
pi+=4/(1+x*x);
}
pi=pi/N;
printf("Pi is %f\n",pi);
}
</pre>

We compile this with gcc main.c -o test, ignoring the -fopenmp options, this means that the #pragma omp parallel for will be interpreted as a comment i.e. ignored. We run it and this is the result:
  
< Now compile with the -fopenmp option and run: Oh dear... Let's examine what went wrong. Well, by default and as we have not specified it as private, the variable x is shared. This means all threads have the same memory address of the variable x. Therefore, thread i will compute some value at x and store it at memory address &x, thread j will then compute its value of x and store it at &x **BEFORE** thread i has used its value to make its contribution to pi. The threads are all over writing each others values of x because they all have the same memory address for x. Our first correction is that x must be made private:
  
`#pragma omp parallel for private(x)`

Secondly, we have a **&#8220;Race Condition&#8221;** for pi. Let me illustrate this with a simple example. Here is what would ideally happen:

&nbsp;

  * Thread 1 reads the current value of pi : 0
  * Thread 1 increments the value of pi : 1
  * Thread 1 stores the new value of pi: 1
  * Thread 2 reads the current value of pi: 1
  * Thread 2 increments the value of pi: 2
  * Thread 2 stores the value of pi: 2

What is actually happening is more like this:

  * Thread 1 reads the current value of pi: 0
  * Thread 2 reads the current value of pi: 0
  * Thread 1 increments pi: 1
  * Thread 2 increments pi: 1
  * Thread 1 stores its value of pi: 1
  * Thread 2 stores its value of pi: 1

The way to correct this is to tell the code to execute the read/write of pi only one thread at a time. This can be achieved with critical or atomic. Add
  
`#pragma omp atomic` Just before pi get&#8217;s updated and you&#8217;ll see that it works.

This scenario crops up time and time again where you are updating some value inside a parallel loop so in the end it had its own clause made for it. All the above can be achieved by simply making pi a** reduction variable**. 

## Reduction

To make pi a reduction variable the code is changed as follows:

&nbsp;

<pre class="brush: cpp; title: ; notranslate" title="">int main(void){
double pi,x;
int i,N;
pi=0.0;
N=1000;
#pragma omp parallel for private(x) reduction(+:pi)
for(i=0;i&lt;N;i++){
x=(double)i/N;
pi+=4/(1+x*x);
}
pi=pi/N;
printf("Pi is %f\n",pi);
}
</pre>

This is simply the quick and neat way of achieving all what we did above.