---
id: 262
title: Toy Exoplanet Change Point Light Curve Transit Detection
date: 2013-06-12T19:12:54+00:00
author: admin
layout: post
guid: http://www.lindonslog.com/?p=262
permalink: /mathematics/statistics/toy-exoplanet-change-point-detection/
categories:
  - Statistics
---
I&#8217;ve been attending an exoplanet data conference this week, a gathering between astrophysicists and statisticians. One way to look for exoplanets is by the [&#8220;Transit&#8221; method](http://www.nasa.gov/mission_pages/kepler/multimedia/images/kepler-transit-graph.html). Basically a dip in the flux from a star is observed as an orbiting planet passes across the line of sight between the observer and the star. There was a lot of talk about automated methods of detecting this dip in brightness. I decided to code this up for the lulz:

First I generated some fake data.
  
[<img src="http://www.lindonslog.com/wp-content/uploads/2013/06/transit.bmp" alt="fake exoplanet transit" title="fake exoplanet transit" class="alignnone size-medium wp-image-264" />](http://www.lindonslog.com/wp-content/uploads/2013/06/transit.bmp)

The dip in flux is meant to be as the exoplanet passes between the observer and the star. Interesting parameters are the dip in flux and the time interval of the reduced flux as these correspond to physical parameters of the planet-star system. I decided to model this as a simple changepoint problem:

<img src="//s0.wp.com/latex.php?latex=f%28t%29+%5Csim+N%28b_%7B1%7D+-+b_%7B2%7D1_%7B%5Cleft%5B+%5Ctau_%7B1%7D%2C%5Ctau_%7B2%7D+%5Cright%5D%7D%28t%29%2C+%5Csigma%5E%7B2%7D%29++&#038;bg=ffffff&#038;fg=000&#038;s=0" alt="f(t) &#92;sim N(b_{1} - b_{2}1_{&#92;left[ &#92;tau_{1},&#92;tau_{2} &#92;right]}(t), &#92;sigma^{2})  " title="f(t) &#92;sim N(b_{1} - b_{2}1_{&#92;left[ &#92;tau_{1},&#92;tau_{2} &#92;right]}(t), &#92;sigma^{2})  " class="latex" />

Parameters of interest are b\_1, b\_2, tau\_1 and tau\_2, which are the baseline, the drop, the time at which the drop occurs and the time at which the drop finishes respectively. I proceeded using the following JAGS code:

<pre class="brush: r; title: ; notranslate" title="">model
{
for(t in 1 : N)
{
D[t] ~ dnorm(mu[t],s2)
mu[t] &lt;- b[1] - step(t - tau1)*step(tau2-t)*step(tau2-tau1) * b[2]
}
b[1] ~ dnorm(0.0,1.0E-6)T(0,)
b[2] ~ dnorm(0.0,1.0E-6)T(0,b[1])
tau1 ~ dunif(1,N)
tau2 ~ dunif(tau1,N)
s2 ~ dunif(0,100)
}
</pre>

The script to generate the data and call JAGS from R can be found here. The posterior density estimates are displayed below:
  
[<img src="http://www.lindonslog.com/wp-content/uploads/2013/06/exo_post.bmp" alt="exoplanet posterior summaries" title="exoplanet posterior summaries" class="alignnone size-medium wp-image-273" />](http://www.lindonslog.com/wp-content/uploads/2013/06/exo_post.bmp)

The Bayesian change point model correctly identifies a drop of 40 and the times at which the transit begin and finishes. One might argue that such a large drop in flux is unphysical for detecting Earth like planets, so here is a further example to detect how robust this approach is. Consider the following light curve:
  
[<img src="http://www.lindonslog.com/wp-content/uploads/2013/06/exo_tiny_trace.bmp" alt="exoplanet light curve" title="exoplanet light curve" class="alignnone size-medium wp-image-278" />](http://www.lindonslog.com/wp-content/uploads/2013/06/exo_tiny_trace.bmp)
  
Can you tell by eye how deep the drop is, when it occurs and for how long? Here are the posterior density estimates:

[<img src="http://www.lindonslog.com/wp-content/uploads/2013/06/exo_post_b.bmp" alt="exoplanet posterior distribution" title="exoplanet posterior distribution"class="alignnone size-medium wp-image-280" />](http://www.lindonslog.com/wp-content/uploads/2013/06/exo_post_b.bmp)
  
[<img src="http://www.lindonslog.com/wp-content/uploads/2013/06/exo_post_tau.bmp" alt="exoplanet posterior distribution" title="exoplanet posterior distribution" class="alignnone size-medium wp-image-279" />](http://www.lindonslog.com/wp-content/uploads/2013/06/exo_post_tau.bmp)
  
The true valies with which the light curve was generated are
  
<img src="//s0.wp.com/latex.php?latex=b_%7B1%7D%3D100%5C%5C++b_%7B2%7D%3D0.5%5C%5C++%5Ctau_%7B1%7D%3D2000%5C%5C++%5Ctau_%7B2%7D%3D2400%5C%5C++%5Csigma%5E%7B2%7D%3D5++&#038;bg=ffffff&#038;fg=000&#038;s=0" alt="b_{1}=100&#92;&#92;  b_{2}=0.5&#92;&#92;  &#92;tau_{1}=2000&#92;&#92;  &#92;tau_{2}=2400&#92;&#92;  &#92;sigma^{2}=5  " title="b_{1}=100&#92;&#92;  b_{2}=0.5&#92;&#92;  &#92;tau_{1}=2000&#92;&#92;  &#92;tau_{2}=2400&#92;&#92;  &#92;sigma^{2}=5  " class="latex" />