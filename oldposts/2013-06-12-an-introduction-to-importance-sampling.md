---
id: 244
title: An Introduction to Importance Sampling
date: 2013-06-12T13:44:21+00:00
author: admin
layout: post
guid: http://www.lindonslog.com/?p=244
permalink: /mathematics/statistics/an-introduction-to-importance-sampling/
categories:
  - Statistics
---
Importance Sampling is a Monte Carlo integration technique for getting (very accurate) approximations to integrals. Consider the integral
  
<img src="//s0.wp.com/latex.php?latex=%5Cint+e%5E%7B-%5Cfrac%7B1%7D%7B2%7Dx%5E%7B2%7D%7Ddx%2C++&#038;bg=ffffff&#038;fg=000&#038;s=0" alt="&#92;int e^{-&#92;frac{1}{2}x^{2}}dx,  " title="&#92;int e^{-&#92;frac{1}{2}x^{2}}dx,  " class="latex" />
  
and suppose we wish to approximate this without doing any calculus. Statistically speaking we want to compute the normalizing constant for a standard normal, which we know to be
  
<img src="//s0.wp.com/latex.php?latex=%5Csqrt%7B2+%5Cpi%7D+%5Capprox+2.506628.++&#038;bg=ffffff&#038;fg=000&#038;s=0" alt="&#92;sqrt{2 &#92;pi} &#92;approx 2.506628.  " title="&#92;sqrt{2 &#92;pi} &#92;approx 2.506628.  " class="latex" />
  
We can rewrite the above integral as
  
<img src="//s0.wp.com/latex.php?latex=%5Cint+%5Cfrac%7Be%5E%7B-%5Cfrac%7B1%7D%7B2%7Dx%5E%7B2%7D%7D%7D%7Bg%28x%29%7Dg%28x%29dx%2C++&#038;bg=ffffff&#038;fg=000&#038;s=0" alt="&#92;int &#92;frac{e^{-&#92;frac{1}{2}x^{2}}}{g(x)}g(x)dx,  " title="&#92;int &#92;frac{e^{-&#92;frac{1}{2}x^{2}}}{g(x)}g(x)dx,  " class="latex" />
  
because this is just multiplying the integrand by 1. One can now make the observation that
  
<img src="//s0.wp.com/latex.php?latex=%5Cint+%5Cfrac%7Be%5E%7B-%5Cfrac%7B1%7D%7B2%7Dx%5E%7B2%7D%7D%7D%7Bg%28x%29%7Dg%28x%29dx%3D%5Cmathbb%7BE%7D%5Cleft%5B%5Cfrac%7Be%5E%7B-%5Cfrac%7B1%7D%7B2%7Dx%5E%7B2%7D%7D%7D%7Bg%28x%29%7D+%5Cright%5D%2C++&#038;bg=ffffff&#038;fg=000&#038;s=0" alt="&#92;int &#92;frac{e^{-&#92;frac{1}{2}x^{2}}}{g(x)}g(x)dx=&#92;mathbb{E}&#92;left[&#92;frac{e^{-&#92;frac{1}{2}x^{2}}}{g(x)} &#92;right],  " title="&#92;int &#92;frac{e^{-&#92;frac{1}{2}x^{2}}}{g(x)}g(x)dx=&#92;mathbb{E}&#92;left[&#92;frac{e^{-&#92;frac{1}{2}x^{2}}}{g(x)} &#92;right],  " class="latex" />
  
and realise that
  
<img src="//s0.wp.com/latex.php?latex=%5Cmathbb%7BE%7D%5Cleft%5B%5Cfrac%7Be%5E%7B-%5Cfrac%7B1%7D%7B2%7Dx%5E%7B2%7D%7D%7D%7Bg%28x%29%7D+%5Cright%5D+%5Capprox+%5Cfrac%7B1%7D%7Bn%7D%5Csum_%7Bi%3D1%7D%5E%7Bn%7D+%5Cfrac%7Be%5E%7B-%5Cfrac%7B1%7D%7B2%7Dx_%7Bi%7D%5E%7B2%7D%7D%7D%7Bg%28x_%7Bi%7D%29%7D%2C++&#038;bg=ffffff&#038;fg=000&#038;s=0" alt="&#92;mathbb{E}&#92;left[&#92;frac{e^{-&#92;frac{1}{2}x^{2}}}{g(x)} &#92;right] &#92;approx &#92;frac{1}{n}&#92;sum_{i=1}^{n} &#92;frac{e^{-&#92;frac{1}{2}x_{i}^{2}}}{g(x_{i})},  " title="&#92;mathbb{E}&#92;left[&#92;frac{e^{-&#92;frac{1}{2}x^{2}}}{g(x)} &#92;right] &#92;approx &#92;frac{1}{n}&#92;sum_{i=1}^{n} &#92;frac{e^{-&#92;frac{1}{2}x_{i}^{2}}}{g(x_{i})},  " class="latex" />
  
where
  
<img src="//s0.wp.com/latex.php?latex=x_%7Bi%7D+%5Csim+g%28x%29&#038;bg=ffffff&#038;fg=000&#038;s=0" alt="x_{i} &#92;sim g(x)" title="x_{i} &#92;sim g(x)" class="latex" />.
  
The code below in R performs this summation.

<pre class="brush: r; title: ; notranslate" title="">N=10000
cumsum=rep(0,N)
for(i in 2:N){
x=rt(1,3)
cumsum[i]=cumsum[i-1]+((exp((-0.5)*(x^2)))/dt(x,3))
}

for(i in 1:N) cumsum[i]=cumsum[i]/i
plot(cumsum[50:N],type="l")
</pre>

[<img src="http://www.lindonslog.com/wp-content/uploads/2013/06/nconstant.bmp" alt="Importance Sampling Normalizing Constant" title="Importance Sampling Normalizing Constant" class="alignnone size-medium wp-image-258" />](http://www.lindonslog.com/wp-content/uploads/2013/06/nconstant.bmp)
  
At n=10000 the normalizing constant given by importance sampling is 2.516303. Increasing n to n=100000 the normalizing constant is 2.505983