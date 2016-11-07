---
id: 762
title: Model Scale Parameterization for MCMC Efficiency
date: 2013-08-01T17:44:01+00:00
author: admin
layout: post
guid: http://www.lindonslog.com/?p=762
permalink: /mathematics/mcmc-interweaving-parameterization-efficiency/
categories:
  - Mathematics
  - R
  - Statistics
---
I recently came across a very interesting paper by Y. Yu and X. Meng[[1](#paperkey_0){.papercite_bibcite}] who present an interweaving strategy between different model parameterizations to improve mixing. It is well known that different model parameterizations can perform better than others under certain conditions. Papaspiliopoulos, Roberts and Sköld [[2](#paperkey_1){.papercite_bibcite}] present a general framework for how to parameterize hierarchical models and provide insights into the conditions under which centered and non-centered parameterizations outperform each other. One isn&#8217;t, however, restricted to reperameterizations of location parameters only, as outlined in the aforementioned paper, and so I decided to experiment with reparameterizations of the scale parameter in a simple hierarchical model with improper priors on the parameters.

## Centered Parameterization

Papaspiliopoulos gave a general definition of the centered parameterization to be when  <img src="//s0.wp.com/latex.php?latex=Y_%7Bi%7D&#038;bg=ffffff&#038;fg=000&#038;s=0" alt="Y_{i}" title="Y_{i}" class="latex" />is independent of  <img src="//s0.wp.com/latex.php?latex=%5Clambda&#038;bg=ffffff&#038;fg=000&#038;s=0" alt="&#92;lambda" title="&#92;lambda" class="latex" />given <img src="//s0.wp.com/latex.php?latex=X_%7Bi%7D&#038;bg=ffffff&#038;fg=000&#038;s=0" alt="X_{i}" title="X_{i}" class="latex" />

<p align="center">
  <img src="//s0.wp.com/latex.php?latex=%5Cdisplaystyle+Y_%7Bi%7D%7CX_%7Bi%7D%2C%5Csigma%5E%7B2%7D+%5Csim+N%28X_%7Bi%7D%2C%5Csigma%5E%7B2%7D%29+%5C+%5C+%5C+%5C+%5C+%281%29&#038;bg=ffffff&#038;fg=000000&#038;s=0" alt="&#92;displaystyle Y_{i}|X_{i},&#92;sigma^{2} &#92;sim N(X_{i},&#92;sigma^{2}) &#92; &#92; &#92; &#92; &#92; (1)" title="&#92;displaystyle Y_{i}|X_{i},&#92;sigma^{2} &#92;sim N(X_{i},&#92;sigma^{2}) &#92; &#92; &#92; &#92; &#92; (1)" class="latex" />
</p>

<p align="center">
  <img src="//s0.wp.com/latex.php?latex=%5Cdisplaystyle+X_%7Bi%7D%7C%5Csigma%5E%7B2%7D%2C%5Clambda%5E%7B2%7D+%5Csim+N%280%2C%5Clambda%5E%7B2%7D%5Csigma%5E%7B2%7D%29+%5C+%5C+%5C+%5C+%5C+%282%29&#038;bg=ffffff&#038;fg=000000&#038;s=0" alt="&#92;displaystyle X_{i}|&#92;sigma^{2},&#92;lambda^{2} &#92;sim N(0,&#92;lambda^{2}&#92;sigma^{2}) &#92; &#92; &#92; &#92; &#92; (2)" title="&#92;displaystyle X_{i}|&#92;sigma^{2},&#92;lambda^{2} &#92;sim N(0,&#92;lambda^{2}&#92;sigma^{2}) &#92; &#92; &#92; &#92; &#92; (2)" class="latex" />
</p>

<p align="center">
  <img src="//s0.wp.com/latex.php?latex=%5Cdisplaystyle+p%28+%5Clambda%5E%7B2%7D+%29+%5Cpropto+%5Cfrac%7B1%7D%7B%5Clambda%5E%7B2%7D%7D+%5C+%5C+%5C+%5C+%5C+%283%29&#038;bg=ffffff&#038;fg=000000&#038;s=0" alt="&#92;displaystyle p( &#92;lambda^{2} ) &#92;propto &#92;frac{1}{&#92;lambda^{2}} &#92; &#92; &#92; &#92; &#92; (3)" title="&#92;displaystyle p( &#92;lambda^{2} ) &#92;propto &#92;frac{1}{&#92;lambda^{2}} &#92; &#92; &#92; &#92; &#92; (3)" class="latex" />
</p>

### Full Conditionals

<p align="center">
  <img src="//s0.wp.com/latex.php?latex=%5Cdisplaystyle+%5Clambda%5E%7B2%7D%7CY_%7B1%3An%7D%2CX_%7B1%3An%7D%2C%5Csigma%5E%7B2%7D+%5Csim+%5CGamma%5E%7B-1%7D%5Cleft%28+%5Cfrac%7Bn%7D%7B2%7D%2C+%5Cfrac%7B%5Csum_%7Bi%7D%5E%7Bn%7D+X_%7Bi%7D%5E%7B2%7D%7D%7B2%5Csigma%5E%7B2%7D%7D%5Cright%29+%5C+%5C+%5C+%5C+%5C+%284%29&#038;bg=ffffff&#038;fg=000000&#038;s=0" alt="&#92;displaystyle &#92;lambda^{2}|Y_{1:n},X_{1:n},&#92;sigma^{2} &#92;sim &#92;Gamma^{-1}&#92;left( &#92;frac{n}{2}, &#92;frac{&#92;sum_{i}^{n} X_{i}^{2}}{2&#92;sigma^{2}}&#92;right) &#92; &#92; &#92; &#92; &#92; (4)" title="&#92;displaystyle &#92;lambda^{2}|Y_{1:n},X_{1:n},&#92;sigma^{2} &#92;sim &#92;Gamma^{-1}&#92;left( &#92;frac{n}{2}, &#92;frac{&#92;sum_{i}^{n} X_{i}^{2}}{2&#92;sigma^{2}}&#92;right) &#92; &#92; &#92; &#92; &#92; (4)" class="latex" />
</p>

<p align="center">
  <img src="//s0.wp.com/latex.php?latex=%5Cdisplaystyle+X_%7Bi%7D%7CY_%7Bi%7D%2C%5Csigma%5E%7B2%7D%2C%5Clambda%5E%7B2%7D+%5Csim+N%5Cleft%28+%5Cfrac%7B%5Cfrac%7BY_%7Bi%7D%7D%7B%5Csigma%5E%7B2%7D%7D%7D%7B%5Cfrac%7B1%7D%7B%5Csigma%5E%7B2%7D%7D%2B%5Cfrac%7B1%7D%7B%5Clambda%5E%7B2%7D%5Csigma%5E%7B2%7D%7D%7D%2C+%5Cfrac%7B1%7D%7B%5Cfrac%7B1%7D%7B%5Csigma%5E%7B2%7D%7D%2B%5Cfrac%7B1%7D%7B%5Clambda%5E%7B2%7D%5Csigma%5E%7B2%7D%7D%7D+%5Cright%29+%5C+%5C+%5C+%5C+%5C+%285%29&#038;bg=ffffff&#038;fg=000000&#038;s=0" alt="&#92;displaystyle X_{i}|Y_{i},&#92;sigma^{2},&#92;lambda^{2} &#92;sim N&#92;left( &#92;frac{&#92;frac{Y_{i}}{&#92;sigma^{2}}}{&#92;frac{1}{&#92;sigma^{2}}+&#92;frac{1}{&#92;lambda^{2}&#92;sigma^{2}}}, &#92;frac{1}{&#92;frac{1}{&#92;sigma^{2}}+&#92;frac{1}{&#92;lambda^{2}&#92;sigma^{2}}} &#92;right) &#92; &#92; &#92; &#92; &#92; (5)" title="&#92;displaystyle X_{i}|Y_{i},&#92;sigma^{2},&#92;lambda^{2} &#92;sim N&#92;left( &#92;frac{&#92;frac{Y_{i}}{&#92;sigma^{2}}}{&#92;frac{1}{&#92;sigma^{2}}+&#92;frac{1}{&#92;lambda^{2}&#92;sigma^{2}}}, &#92;frac{1}{&#92;frac{1}{&#92;sigma^{2}}+&#92;frac{1}{&#92;lambda^{2}&#92;sigma^{2}}} &#92;right) &#92; &#92; &#92; &#92; &#92; (5)" class="latex" />
</p>

## Non-Centered Parameterization

Papaspiliopoulos gave a general definition of the non-centered parameterization to be when  <img src="//s0.wp.com/latex.php?latex=%5Ctilde%7BX%7D_%7Bi%7D&#038;bg=ffffff&#038;fg=000&#038;s=0" alt="&#92;tilde{X}_{i}" title="&#92;tilde{X}_{i}" class="latex" />and  <img src="//s0.wp.com/latex.php?latex=%5Clambda&#038;bg=ffffff&#038;fg=000&#038;s=0" alt="&#92;lambda" title="&#92;lambda" class="latex" />are a priori independent.

<p align="center">
  <img src="//s0.wp.com/latex.php?latex=%5Cdisplaystyle+Y_%7Bi%7D%7C%5Ctilde%7BX%7D_%7Bi%7D%2C%5Csigma%5E%7B2%7D%2C%5Clambda+%5Csim+N%28%5Clambda+%5Ctilde%7BX%7D_%7Bi%7D%2C%5Csigma%5E%7B2%7D%29+%5C+%5C+%5C+%5C+%5C+%286%29&#038;bg=ffffff&#038;fg=000000&#038;s=0" alt="&#92;displaystyle Y_{i}|&#92;tilde{X}_{i},&#92;sigma^{2},&#92;lambda &#92;sim N(&#92;lambda &#92;tilde{X}_{i},&#92;sigma^{2}) &#92; &#92; &#92; &#92; &#92; (6)" title="&#92;displaystyle Y_{i}|&#92;tilde{X}_{i},&#92;sigma^{2},&#92;lambda &#92;sim N(&#92;lambda &#92;tilde{X}_{i},&#92;sigma^{2}) &#92; &#92; &#92; &#92; &#92; (6)" class="latex" />
</p>

<p align="center">
  <img src="//s0.wp.com/latex.php?latex=%5Cdisplaystyle+%5Ctilde%7BX%7D_%7Bi%7D%7C%5Csigma%5E%7B2%7D+%5Csim+N%280%2C%5Csigma%5E%7B2%7D%29+%5C+%5C+%5C+%5C+%5C+%287%29&#038;bg=ffffff&#038;fg=000000&#038;s=0" alt="&#92;displaystyle &#92;tilde{X}_{i}|&#92;sigma^{2} &#92;sim N(0,&#92;sigma^{2}) &#92; &#92; &#92; &#92; &#92; (7)" title="&#92;displaystyle &#92;tilde{X}_{i}|&#92;sigma^{2} &#92;sim N(0,&#92;sigma^{2}) &#92; &#92; &#92; &#92; &#92; (7)" class="latex" />
</p>

<p align="center">
  <img src="//s0.wp.com/latex.php?latex=%5Cdisplaystyle+p%28%5Clambda%29+%5Cpropto+1+%5C+%5C+%5C+%5C+%5C+%288%29&#038;bg=ffffff&#038;fg=000000&#038;s=0" alt="&#92;displaystyle p(&#92;lambda) &#92;propto 1 &#92; &#92; &#92; &#92; &#92; (8)" title="&#92;displaystyle p(&#92;lambda) &#92;propto 1 &#92; &#92; &#92; &#92; &#92; (8)" class="latex" />
</p>

### Full Conditionals

<p align="center">
  <img src="//s0.wp.com/latex.php?latex=%5Cdisplaystyle+%5Clambda%7CY_%7B1%3An%7D%2CX_%7B1%3An%7D%2C%5Csigma%5E%7B2%7D+%5Csim+N+%5Cleft%28+%5Cfrac%7B%5Csum_%7Bi%3D1%7D%5E%7Bn%7D%5Ctilde%7BX%7D_%7Bi%7DY_%7Bi%7D%7D%7B%5Csum_%7Bi%3D1%7D%5E%7Bn%7D%5Ctilde%7BX%7D_%7Bi%7D%5E%7B2%7D%7D%2C+%5Cfrac%7B%5Csigma%5E%7B2%7D%7D%7B%5Csum_%7Bi%3D1%7D%5E%7Bn%7D%5Ctilde%7BX%7D_%7Bi%7D%5E%7B2%7D%7D+%5Cright%29+%5C+%5C+%5C+%5C+%5C+%289%29&#038;bg=ffffff&#038;fg=000000&#038;s=0" alt="&#92;displaystyle &#92;lambda|Y_{1:n},X_{1:n},&#92;sigma^{2} &#92;sim N &#92;left( &#92;frac{&#92;sum_{i=1}^{n}&#92;tilde{X}_{i}Y_{i}}{&#92;sum_{i=1}^{n}&#92;tilde{X}_{i}^{2}}, &#92;frac{&#92;sigma^{2}}{&#92;sum_{i=1}^{n}&#92;tilde{X}_{i}^{2}} &#92;right) &#92; &#92; &#92; &#92; &#92; (9)" title="&#92;displaystyle &#92;lambda|Y_{1:n},X_{1:n},&#92;sigma^{2} &#92;sim N &#92;left( &#92;frac{&#92;sum_{i=1}^{n}&#92;tilde{X}_{i}Y_{i}}{&#92;sum_{i=1}^{n}&#92;tilde{X}_{i}^{2}}, &#92;frac{&#92;sigma^{2}}{&#92;sum_{i=1}^{n}&#92;tilde{X}_{i}^{2}} &#92;right) &#92; &#92; &#92; &#92; &#92; (9)" class="latex" />
</p>

<p align="center">
  <img src="//s0.wp.com/latex.php?latex=%5Cdisplaystyle+%5Ctilde%7BX%7D_%7Bi%7D%7CY_%7Bi%7D%2C%5Csigma%5E%7B2%7D%2C%5Clambda%5E%7B2%7D+%5Csim+N%5Cleft%28+%5Cfrac%7B%5Cfrac%7B%5Clambda+Y_%7Bi%7D%7D%7B%5Csigma%5E%7B2%7D%7D%7D%7B%5Cfrac%7B%5Clambda%5E%7B2%7D%7D%7B%5Csigma%5E%7B2%7D%7D%2B%5Cfrac%7B1%7D%7B%5Csigma%5E%7B2%7D%7D%7D%2C+%5Cfrac%7B1%7D%7B%5Cfrac%7B%5Clambda%5E%7B2%7D%7D%7B%5Csigma%5E%7B2%7D%7D%2B%5Cfrac%7B1%7D%7B%5Csigma%5E%7B2%7D%7D%7D+%5Cright%29+%5C+%5C+%5C+%5C+%5C+%2810%29&#038;bg=ffffff&#038;fg=000000&#038;s=0" alt="&#92;displaystyle &#92;tilde{X}_{i}|Y_{i},&#92;sigma^{2},&#92;lambda^{2} &#92;sim N&#92;left( &#92;frac{&#92;frac{&#92;lambda Y_{i}}{&#92;sigma^{2}}}{&#92;frac{&#92;lambda^{2}}{&#92;sigma^{2}}+&#92;frac{1}{&#92;sigma^{2}}}, &#92;frac{1}{&#92;frac{&#92;lambda^{2}}{&#92;sigma^{2}}+&#92;frac{1}{&#92;sigma^{2}}} &#92;right) &#92; &#92; &#92; &#92; &#92; (10)" title="&#92;displaystyle &#92;tilde{X}_{i}|Y_{i},&#92;sigma^{2},&#92;lambda^{2} &#92;sim N&#92;left( &#92;frac{&#92;frac{&#92;lambda Y_{i}}{&#92;sigma^{2}}}{&#92;frac{&#92;lambda^{2}}{&#92;sigma^{2}}+&#92;frac{1}{&#92;sigma^{2}}}, &#92;frac{1}{&#92;frac{&#92;lambda^{2}}{&#92;sigma^{2}}+&#92;frac{1}{&#92;sigma^{2}}} &#92;right) &#92; &#92; &#92; &#92; &#92; (10)" class="latex" />
</p>

## Interweaving Strategy

Generally when the CP works well, the NCP works poorly and vice versa. Yaming Yu and Xiao-Li Meng[[1](#paperkey_0){.papercite_bibcite}] present a way of combining both strategies by interweaving the Gibbs steps of both parameterizations at each iteration. The details can be read in their paper. I decided to test all three Gibbs samplers with the following R code:

<pre class="brush: r; title: ; notranslate" title="">#Generate Data
lam2=0.5
lam=sqrt(lam2)
sig2=1
n=1000
Xt=rnorm(n,0,sqrt(lam2*sig2))
Y=rnorm(n,Xt,sqrt(sig2))
nmc=2000
X=Xt

#Centered Parameterization
cp_lam2=rep(0,nmc)
cp_X=matrix(0,nmc,n)
for(i in 1:nmc){
	inv_lam2=rgamma(1,(n)/2,rate=(t(X)%*%X)/(2*sig2))
	lam2=1/inv_lam2
	X=rnorm(n,(1/(1/sig2 + 1/(sig2*lam2)))*Y/sig2, sqrt(1/(1/sig2 + 1/(sig2*lam2))))
	cp_lam2[i]=lam2
	cp_X[i,]=X
}
mean_cp_X=apply(cp_X,2,mean)

#Non-Centered Parameterization
X=Xt
ncp_lam2=rep(0,nmc)
ncp_X=matrix(0,nmc,n)
for(i in 1:nmc){
	lam=rnorm(1,t(X)%*%Y/(t(X)%*%X), sqrt(sig2/(t(X)%*%X)))
	lam2=lam*lam;
	X=rnorm(n, (1/(1/sig2 + lam2/sig2))*lam*Y/sig2, sqrt(1/(1/sig2+lam2/sig2))  )
	ncp_lam2[i]=lam2
	ncp_X[i,]=X
}
mean_ncp_X=apply(ncp_X,2,mean)

#Interweaving Strategy
int_lam2=rep(0,nmc)
int_X=matrix(0,nmc,n)
for(i in 1:nmc){
	X=rnorm(n,(1/(1/sig2 + 1/(sig2*lam2)))*Y/sig2, sqrt(1/(1/sig2 + 1/(sig2*lam2))))
	inv_lam2=rgamma(1,(n)/2,rate=(t(X)%*%X)/(2*sig2))
	half_lam2=1/inv_lam2
	X=X/sqrt(half_lam2)    #Transform to Xtilde
	lam=rnorm(1,t(X)%*%Y/(t(X)%*%X), sqrt(sig2/(t(X)%*%X)))
	lam2=lam*lam;
	int_lam2[i]=lam2
	int_X[i,]=X
}
mean_cp_X=apply(cp_X,2,mean)

#Remove Burnin
cp_lam2=cp_lam2[-(1:1000)]
ncp_lam2=ncp_lam2[-(1:1000)]
int_lam2=int_lam2[-(1:1000)]

#Plot Results
par(mfrow=c(3,3))
acf(cp_lam2)
plot(cp_lam2,type="l")
plot(cp_lam2[1:nmc-1],cp_lam2[2:nmc])
acf(ncp_lam2)
plot(ncp_lam2,type="l")
plot(ncp_lam2[1:nmc-1],ncp_lam2[2:nmc])
acf(int_lam2)
plot(int_lam2,type="l")
plot(int_lam2[1:nmc-1],int_lam2[2:nmc])
</pre>

## Results

### <img src="//s0.wp.com/latex.php?latex=%5Clambda%3D0.3&#038;bg=ffffff&#038;fg=000&#038;s=0" alt="&#92;lambda=0.3" title="&#92;lambda=0.3" class="latex" />

<div id="attachment_798" style="width: 910px" class="wp-caption aligncenter">
  <a href="http://www.lindonslog.com/wp-content/uploads/2013/08/lam0.3.png"><img class="size-full wp-image-798" alt="mcmc parameterization" src="http://www.lindonslog.com/wp-content/uploads/2013/08/lam0.3.png" width="900" height="900" srcset="http://www.lindonslog.com/wp-content/uploads/2013/08/lam0.3.png 900w, http://www.lindonslog.com/wp-content/uploads/2013/08/lam0.3-150x150.png 150w, http://www.lindonslog.com/wp-content/uploads/2013/08/lam0.3-300x300.png 300w" sizes="(max-width: 900px) 100vw, 900px" /></a>
  
  <p class="wp-caption-text">
    Interweaving outperforms non-centered outperforms centered
  </p>
</div>

### <img src="//s0.wp.com/latex.php?latex=%5Clambda%3D6&#038;bg=ffffff&#038;fg=000&#038;s=0" alt="&#92;lambda=6" title="&#92;lambda=6" class="latex" />

<div id="attachment_802" style="width: 910px" class="wp-caption aligncenter">
  <a href="http://www.lindonslog.com/wp-content/uploads/2013/08/lam6.png"><img class="size-full wp-image-802" alt="ncp poorly mixing" src="http://www.lindonslog.com/wp-content/uploads/2013/08/lam6.png" width="900" height="900" srcset="http://www.lindonslog.com/wp-content/uploads/2013/08/lam6.png 900w, http://www.lindonslog.com/wp-content/uploads/2013/08/lam6-150x150.png 150w, http://www.lindonslog.com/wp-content/uploads/2013/08/lam6-300x300.png 300w" sizes="(max-width: 900px) 100vw, 900px" /></a>
  
  <p class="wp-caption-text">
    Interweaving outperforms centered outperforms non-centered
  </p>
</div>

## Discussion

As lambda gets small the centered parameterization becomes ever more autocorrelated and poorly mixing. When lambda becomes large the non-centered parameterization becomes ever more autocorrelated and poorly mixing. The interweaved Gibbs sampler exhibits great mixing in all cases.

<div id="paperkey_0" class="papercite_entry">
  [1] <a href='http://dx.doi.org/10.1198/jcgs.2011.203main' class='papercite_doi' title='View document in publisher site'><img src='http://www.lindonslog.com/wp-content/plugins/papercite/img/external.png' width='10' height='10' alt='[doi]' /></a> Y. Yu and X. Meng, &#8220;To Center or Not to Center: That Is Not the Question&#8211;An Ancillarity-Sufficiency Interweaving Strategy (ASIS) for Boosting MCMC Efficiency,&#8221; <span style="font-style: italic">Journal of computational and graphical statistics</span>, vol. 20, iss. 3, pp. 531-570, 2011. <br /> <a href="javascript:void(0)" id="papercite_0" class="papercite_toggle">[Bibtex]</a>
</div>

<div class="papercite_bibtex" id="papercite_0_block">
  <pre><code class="tex bibtex">@article{Yu11,
author = {Yu, Yaming and Meng, Xiao-Li},
citeulike-article-id = {10408757},
citeulike-linkout-0 = {http://amstat.tandfonline.com/doi/abs/10.1198/jcgs.2011.203main},
citeulike-linkout-1 = {http://pubs.amstat.org/doi/abs/10.1198/jcgs.2011.203main},
citeulike-linkout-2 = {http://dx.doi.org/10.1198/jcgs.2011.203main},
doi = {10.1198/jcgs.2011.203main},
journal = {Journal of Computational and Graphical Statistics},
number = {3},
pages = {531--570},
posted-at = {2012-03-03 18:10:07},
priority = {2},
title = {{To Center or Not to Center: That Is Not the Question--An Ancillarity-Sufficiency Interweaving Strategy (ASIS) for Boosting MCMC Efficiency}},
url = {http://amstat.tandfonline.com/doi/abs/10.1198/jcgs.2011.203main},
volume = {20},
year = {2011}
}</code></pre>
</div>

<div id="paperkey_1" class="papercite_entry">
  [2] O. Papaspiliopoulos, G. O. Roberts, and M. Sköld, &#8220;A general framework for the parametrization of hierarchical models,&#8221; <span style="font-style: italic">Statistical science</span>, vol. 22, iss. 1, pp. 59-73, 2007. <br /> <a href="javascript:void(0)" id="papercite_1" class="papercite_toggle">[Bibtex]</a>
</div>

<div class="papercite_bibtex" id="papercite_1_block">
  <pre><code class="tex bibtex">@article{Papaspiliopoulos07,
abstract = {{In this paper, we describe centering and noncentering methodology as complementary techniques for use in parametrization of broad classes of hierarchical models, with a view to the construction of effective MCMC algorithms for exploring posterior distributions from these models. We give a clear qualitative understanding as to when centering and noncentering work well, and introduce theory concerning the convergence time complexity of Gibbs samplers using centered and noncentered parametrizations. We give general recipes for the construction of noncentered parametrizations, including an auxiliary variable technique called the state-space expansion technique. We also describe partially noncentered methods, and demonstrate their use in constructing robust Gibbs sampler algorithms whose convergence properties are not overly sensitive to the data.}},
author = {Papaspiliopoulos, Omiros and Roberts, Gareth O. and Sk\"{o}ld, Martin},
citeulike-article-id = {8977350},
citeulike-linkout-0 = {http://www.jstor.org/stable/27645805},
journal = {Statistical Science},
number = {1},
pages = {59--73},
posted-at = {2011-03-10 18:55:50},
priority = {2},
publisher = {Institute of Mathematical Statistics},
title = {{A general framework for the parametrization of hierarchical models}},
url = {http://www.jstor.org/stable/27645805},
volume = {22},
year = {2007}
}</code></pre>
</div>

<span class="Z3988" title="ctx_ver=Z39.88-2004&#038;rft_val_fmt=info%3Aofi%2Ffmt%3Akev%3Amtx%3Ajournal&#038;rft_id=info%3Adoi%2F10.1198%2Fjcgs.2011.203main&#038;rft.atitle=To+Center+or+Not+to+Center%3A+That+Is+Not+the+Question%E2%80%94An+Ancillarity%E2%80%93Sufficiency+Interweaving+Strategy+%28ASIS%29+for+Boosting+MCMC+Efficiency&#038;rft.jtitle=Journal+of+Computational+and+Graphical+Statistics&#038;rft.artnum=http%3A%2F%2Fwww.tandfonline.com%2Fdoi%2Fabs%2F10.1198%2Fjcgs.2011.203main&#038;rft.volume=20&#038;rft.issue=3&#038;rft.issn=1061-8600&#038;rft.spage=531&#038;rft.epage=570&#038;rft.date=2011&#038;rfr_id=info%3Asid%2Fscienceseeker.org&#038;rft.au=Yu+Yaming&#038;rft.aulast=Yu&#038;rft.aufirst=Yaming&#038;rft.au=Meng+Xiao-Li&#038;rft.aulast=Meng&#038;rft.aufirst=Xiao-Li&#038;rfs_dat=ss.included=1&#038;rfe_dat=bpr3.included=1;bpr3.tags=Computer+Science+%2F+Engineering%2CMathematics%2COther">Yu Y. & Meng X.L. (2011). To Center or Not to Center: That Is Not the Question—An Ancillarity–Sufficiency Interweaving Strategy (ASIS) for Boosting MCMC Efficiency, <span style="font-style:italic;">Journal of Computational and Graphical Statistics, 20</span> (3) 531-570. DOI: <a rel="author" href="http://dx.doi.org/10.1198%2Fjcgs.2011.203main">10.1198/jcgs.2011.203main</a></span>