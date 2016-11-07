---
id: 1090
title: Generate Random Inverse Gaussian in R
date: 2014-09-20T17:57:19+00:00
author: admin
layout: post
guid: http://www.lindonslog.com/?p=1090
permalink: /programming/generate-random-inverse-gaussian-r/
categories:
  - Programming
  - R
  - Statistics
---
Needed to generate draws from an inverse Gaussian today, so I wrote the following Rcpp code:

<pre class="brush: cpp; title: ; notranslate" title="">#include &lt;RcppArmadillo.h&gt;
// [[Rcpp::depends(RcppArmadillo)]]

using namespace Rcpp;
using namespace arma;

// [[Rcpp::export]]
Col&lt;double&gt; rrinvgauss(int n, double mu, double lambda){

	Col&lt;double&gt; random_vector(n);
	double z,y,x,u;

	for(int i=0; i&lt;n; ++i){
		z=R::rnorm(0,1);
		y=z*z;
		x=mu+0.5*mu*mu*y/lambda - 0.5*(mu/lambda)*sqrt(4*mu*lambda*y+mu*mu*y*y);
		u=R::runif(0,1);
		if(u &lt;= mu/(mu+x)){
			random_vector(i)=x;
		}else{
			random_vector(i)=mu*mu/x;
		};
	}
	return(random_vector);
}
</pre>

It seems to be faster than existing implementations such as rig from [mgcv](http://stat.ethz.ch/R-manual/R-devel/library/mgcv/html/rig.html) and rinvgauss from [statmod](http://ugrad.stat.ubc.ca/R/library/statmod/html/invgauss.html) packages.

<pre class="brush: r; title: ; notranslate" title="">library(Rcpp) 
library(RcppArmadillo)
library(rbenchmark)
library(statmod)
library(mgcv)
sourceCpp("rrinvgauss.cpp")
n=10000 
benchmark(rig(n,1,1),rinvgauss(n,1,1),rrinvgauss(n,1,1),replications=100)
</pre>

[<img src="http://www.lindonslog.com/wp-content/uploads/2014/09/inversegaussian.png" alt="inverse gaussian" width="923" height="97" class="aligncenter size-full wp-image-1092" srcset="http://www.lindonslog.com/wp-content/uploads/2014/09/inversegaussian.png 923w, http://www.lindonslog.com/wp-content/uploads/2014/09/inversegaussian-300x31.png 300w" sizes="(max-width: 923px) 100vw, 923px" />](http://www.lindonslog.com/wp-content/uploads/2014/09/inversegaussian.png)

rename rrinvgauss as desired.