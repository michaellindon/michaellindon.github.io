---
id: 291
title: Ages 10-12 Toy Exoplanet Detection
date: 2013-06-13T15:55:38+00:00
author: admin
layout: post
guid: http://www.lindonslog.com/?p=291
permalink: /mathematics/statistics/ages-10-12-toy-exoplanet-detection/
categories:
  - Statistics
---
A major objection with the previous simulated light curves is that the baseline is rarely constnat. Instead, from what I have learned, it is a horrible mess of discontinuities and curves due to the telescope rotating and instruments heating up. I spoke to someone who said that there is some periodicity in the curve. It is easy to adapt the previous code to generalize it for a periodic background flux. Consider the model:
  
<img src="//s0.wp.com/latex.php?latex=f_%7Bt%7D+%5Csim+N%28b_%7B1%7D+%2B+a%5Ctext%7Bsin%7D%28%5Comega+t%29-+b_%7B2%7D1_%7B+%5Cleft%5B+%5Ctau_%7B1%7D%2C%5Ctau_%7B2%7D+%5Cright%5D+%7D%28t%29%2C+%5Csigma%5E%7B2%7D%29++&#038;bg=ffffff&#038;fg=000&#038;s=0" alt="f_{t} &#92;sim N(b_{1} + a&#92;text{sin}(&#92;omega t)- b_{2}1_{ &#92;left[ &#92;tau_{1},&#92;tau_{2} &#92;right] }(t), &#92;sigma^{2})  " title="f_{t} &#92;sim N(b_{1} + a&#92;text{sin}(&#92;omega t)- b_{2}1_{ &#92;left[ &#92;tau_{1},&#92;tau_{2} &#92;right] }(t), &#92;sigma^{2})  " class="latex" />
  
which has two additional parameters, the amplitude a and the angular frequency omega, upon which I put priors:
  
<img src="//s0.wp.com/latex.php?latex=a+%5Csim+unif%280%2Cb_%7B1%7D%29%5C%5C++%5Comega+%5Csim+unif%280%2C1%29%5C%5C++&#038;bg=ffffff&#038;fg=000&#038;s=0" alt="a &#92;sim unif(0,b_{1})&#92;&#92;  &#92;omega &#92;sim unif(0,1)&#92;&#92;  " title="a &#92;sim unif(0,b_{1})&#92;&#92;  &#92;omega &#92;sim unif(0,1)&#92;&#92;  " class="latex" />
  
I&#8217;m not sure if the last one is justified, I&#8217;d need to talk to an exoplanet expert, but hopefully the background frequency is a lot less than the frequency at which measurements are being made.
  
The JAGS code looks like this:

<pre class="brush: r; title: ; notranslate" title="">model
{
for(t in 1 : N)
{
D[t] ~ dnorm(mu[t],s2)
mu[t] &lt;- b[1]+ amp*sin(omega*t) - step(t - tau1)*step(tau2-t)*step(tau2-tau1) * b[2]
}
b[1] ~ dnorm(0.0,1.0E-6)T(0,)
b[2] ~ dnorm(0.0,1.0E-6)T(0,b[1])
tau1 ~ dunif(1,N)
tau2 ~ dunif(tau1,N)
s2 ~ dunif(0,100)
amp ~ dunif(0,b[1])
omega ~ dunif(0,1)
}
</pre>

The code to call JAGS from R and make plots can be found [here](http://www.lindonslog.com/example_code/ages1012exoplanet.R).

<pre class="brush: r; title: ; notranslate" title="">rm(list=ls())
library('rjags')

s2=5
bn=1000
dn=400
amp=1
omega=0.005
base=rnorm(bn,100,sqrt(s2))
drop=rnorm(dn,97,sqrt(s2))
t=seq(1,length(c(base,drop,base,base)),1)
sin=amp*sin(omega*t)
trace=c(base,drop,base,base)
trace=trace+sin

plot(trace,type="l",col="red",xlab="Time t",ylab="Flux")

datalist=list(
  D=trace,
  N=length(trace)
  )

jags &lt;- jags.model(file.path('/home/grad/msl33/Dropbox/samsi/cp.bugs'),data = datalist, n.chains = 1, n.adapt = 500)
mcmc.samples=coda.samples(jags,c('b','tau1','tau2','amp','omega'),2000)
plot(mcmc.samples)
</pre>

and I generated some fake data that looks like this:
  
[<img src="http://www.lindonslog.com/wp-content/uploads/2013/06/per_flux.bmp" alt="Periodic Background Flux with Exoplanet Transit Light Curve" class="alignnone size-medium wp-image-297" />](http://www.lindonslog.com/wp-content/uploads/2013/06/per_flux.bmp)
  
based on the parameters:
  
<img src="//s0.wp.com/latex.php?latex=b_%7B1%7D%3D100%5C%5C++b_%7B2%7D%3D3%5C%5C++a%3D1%5C%5C++%5Ctau_%7B1%7D%3D1000%5C%5C++%5Ctau_%7B2%7D%3D1400%5C%5C++&#038;bg=ffffff&#038;fg=000&#038;s=0" alt="b_{1}=100&#92;&#92;  b_{2}=3&#92;&#92;  a=1&#92;&#92;  &#92;tau_{1}=1000&#92;&#92;  &#92;tau_{2}=1400&#92;&#92;  " title="b_{1}=100&#92;&#92;  b_{2}=3&#92;&#92;  a=1&#92;&#92;  &#92;tau_{1}=1000&#92;&#92;  &#92;tau_{2}=1400&#92;&#92;  " class="latex" />

The posterior density estimates are below:
  
[<img src="http://www.lindonslog.com/wp-content/uploads/2013/06/per_per.bmp" alt="Posterior Angular Frequency and Amplitude" title="Posterior Angular Frequency and Amplitude" class="alignnone size-medium wp-image-299" />](http://www.lindonslog.com/wp-content/uploads/2013/06/per_per.bmp)
  
[<img src="http://www.lindonslog.com/wp-content/uploads/2013/06/per_tau.bmp" alt="Posterior Exoplanet Transit Times" class="alignnone size-medium wp-image-300" />](http://www.lindonslog.com/wp-content/uploads/2013/06/per_tau.bmp)
  
[<img src="http://www.lindonslog.com/wp-content/uploads/2013/06/per_b.bmp" alt="Background Flux and Dip in Flux" class="alignnone size-medium wp-image-301" />](http://www.lindonslog.com/wp-content/uploads/2013/06/per_b.bmp)