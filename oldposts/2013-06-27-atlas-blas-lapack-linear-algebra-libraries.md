---
id: 539
title: Getting started with ATLAS, BLAS and LAPACK
date: 2013-06-27T18:23:48+00:00
author: admin
layout: post
guid: http://www.lindonslog.com/?p=539
permalink: /programming/atlas-blas-lapack-linear-algebra-libraries/
categories:
  - Programming
---
I decided to experiment with Atlas (Automatically Tuned Linear Algebra Software) because it contains a parallel BLAS library. For those that don&#8217;t have access to the Intel math kernel library Atlas is a good choice for obtaining an automatically optimized BLAS library. It also provides a nice C interface to BLAS, hence there are two ways to go about using the library. On my Fedora 18 the Atlas build is found under &#8220;/usr/lib64/atlas/&#8221;. I&#8217;ll start with a short example of how to scale a vector.

## Without cblas.h

<pre class="brush: cpp; title: ; notranslate" title="">#include &lt;stdio.h&gt;

int
main ()
{
  int i, j, n, one;
  double coefficient;
  double x[] = { 1, 1, 1 };

  coefficient = 4.323;
  one = 1;
  n = 3;

  dscal_(&n, &coefficient, &x[0], &one);

  for (i = 0; i &lt; 3; ++i)
    printf ("%f\n", x[i]);

  return 0;
}
</pre>

### Calling Fortran from C

The Fortran compiler appends an underscore after subroutinenames (subroutine is Fortran-speak for &#8220;function&#8221;). So bearing in mind that BLAS is written in Fortran, to call the subroutine &#8220;dscal()&#8221; from C one must write &#8220;dscal\_()&#8221;. Similarly if a C function is named &#8220;myfunc\_(&#8230;)&#8221;, it may be called from Fortran using &#8220;myfunc(&#8230;)&#8217;. In addition, C passes arguments by value whereas Fortran passes arguments by reference, so to call a Fortran subroutine from C one must pass the memory address of the arguments by using the ampersand and not something like &#8220;dscal_(3, 4.323, &x[0], 1);&#8221;.
  
To compile we must link with the atlas libraries.

<pre class="brush: bash; title: ; notranslate" title="">[michael@michael lin]$ gcc example.c -o example -L /usr/lib64/atlas -lf77blas 
[michael@michael lin]$ ./testblas 
4.323000
4.323000
4.323000
</pre>

## With cblas.h

<pre class="brush: cpp; title: ; notranslate" title="">#include &lt;stdio.h&gt;
#include &lt;cblas.h&gt;

int
main ()
{
  int i;
  double x[] = { 1, 1, 1 };


  cblas_dscal(3, 4.323, x, 1);

  for (i = 0; i &lt; 3; ++i)
    printf ("%f\n", x[i]);

  return 0;
}
</pre>

The main difference here is that instead of an &#8220;\_&#8221; suffix there is a &#8220;cblas\_&#8221; prefix, the cblas header has been included and that the arguments are passed by value than by reference. To compile this we link against the cblas library instead of the blas library.

<pre class="brush: bash; title: ; notranslate" title="">[michael@michael lin]$ gcc testblasc.c -o testblas -L /usr/lib64/atlas -lcblas -latlas
[michael@michael lin]$ ./testblas 
4.323000
4.323000
4.323000
</pre>

## Compiling with C++

There are further modifications to the code if you want to write in C++ rather than C. The first is instead of &#8220;#include <cblas.h>&#8221; one must write

<pre class="brush: cpp; title: ; notranslate" title="">extern "C"
{
	   #include &lt;cblas.h&gt;
}
</pre>

I&#8217;ve now rewritten the above example using the vectors library and making use of cpp&#8217;s input/output.

<pre class="brush: cpp; title: ; notranslate" title="">#include &lt;cstdlib&gt;
#include &lt;vector&gt;
#include &lt;iostream&gt;

extern "C"
{
	   #include &lt;cblas.h&gt;
}

using namespace std;
int main ()
{
  int i;
  vector&lt;double&gt; x(3,1);

  cblas_dscal (x.size(), 4.323, &x[0], 1);

  for (i = 0; i &lt; x.size(); ++i)
	cout &lt;&lt; x[i] &lt;&lt; endl;
  return 0;
}
</pre>

The advantages of the vector class instead of a C array is that now one doesn&#8217;t need an extra variable to store the length of the array. Link against -lcblas this time than -lf77blas. All atlas libraries can be found <a href="http://math-atlas.sourceforge.net/errata.html#LINK" title="atlas libraries" rel="nofollow">here</a>.