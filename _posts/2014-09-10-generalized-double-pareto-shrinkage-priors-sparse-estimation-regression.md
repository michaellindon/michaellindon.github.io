---
id: 1067
title: Generalized Double Pareto Priors for Regression
date: 2014-09-10T21:45:58+00:00
author: admin
layout: post
guid: http://www.lindonslog.com/?p=1067
permalink: /mathematics/statistics/generalized-double-pareto-shrinkage-priors-sparse-estimation-regression/
categories:
  - R
  - Statistics
---
This post is a review of the &#8220;GENERALIZED DOUBLE PARETO SHRINKAGE&#8221; Statistica Sinica (2012) paper by Armagan, Dunson and Lee. 

Consider the regression model \(Y=X\beta+\varepsilon\) where we put a generalized double pareto distribution as the prior on the regression coefficients \(\beta\). The GDP distribution has density
  
$$\begin{equation}
	  
f(\beta|\xi,\alpha)=\frac{1}{2\xi}\left( 1+\frac{|\beta|}{\alpha\xi} \right)^{-(\alpha+1)}.
	  
\label{}
  
\end{equation}$$

## GDP as Scale Mixture of Normals

The GDP distribution can be conveniently represented as a scale mixture of normals. Let
  
$$\begin{align*}
  
\beta\_{i}|\phi,\tau\_{i} &\sim N(0,\phi^{-1}\tau_{i})\\
  
\tau\_{i}|\lambda\_{i}&\sim Exp(\frac{\lambda_{i}^{2}}{2})\\
  
\lambda_{i}&\sim Ga(\alpha,\eta)\\
  
\end{align*}$$
   
then \(\beta|\phi \sim GDP(\xi=\frac{\eta}{\sqrt{\phi}\alpha},\alpha)\).
  
To see this first note that \(\beta\_{i}|\phi,\lambda\_{i}\) has a Laplace or Double Exponential distribution with rate parameter \(\sqrt{\phi}\lambda_{i}\).
  
$$\begin{align*}
	  
p(\beta\_{i}|\phi,\lambda\_{i})&=\int p(\beta\_{i}|\phi,\tau\_{i})p(\tau\_{i}|\lambda\_{i})d\tau_{i}\\
	  
\psi(t)&=\int e^{it\beta\_{i}} \int p(\beta\_{i}|\phi,\tau\_{i})p(\tau\_{i}|\lambda\_{i})d\tau\_{i} d\beta_{i}\\
	  
&=\int \int e^{it\beta\_{i}}p(\beta\_{i}|\phi,\tau\_{i})d\beta\_{i}p(\tau\_{i}|\lambda\_{i})d\tau_{i}\\
	  
&=\int e^{-\frac{1}{2}\frac{\tau\_{i}}{\phi}t^{2}}p(\tau\_{i}|\lambda\_{i})d\tau\_{i}\\
	  
&=\frac{\lambda\_{i}^{2}}{2} \int e^{-\frac{1}{2}(\frac{t^{2}}{\phi}+\frac{\lambda\_{i}^{2}}{2})\tau\_{i}}d\tau\_{i}\\
	  
&=\frac{\phi\lambda\_{i}^{2}}{t^{2}+\phi\lambda\_{i}^{2}},
  
\end{align*}$$
  
which is the characteristic function of a Double Exponential distribution with rate parameter \(\sqrt{\phi}\lambda_{i}\).
  
Lastly
  
$$\begin{align*}
	  
p(\beta\_{i}|\phi)&=\int p(\beta\_{i}|\phi,\lambda\_{i})p(\lambda\_{i})d\lambda_{i}\\
	  
&=\frac{1}{2}\sqrt{\phi}\frac{\eta^{\alpha}}{\Gamma(\alpha)}\frac{\Gamma(\alpha+1)}{(\eta+\sqrt{\phi}|\beta_{i}|)^{\alpha+1}}\\
	  
&=\frac{1}{2}\frac{\sqrt{\phi}\alpha}{\eta}\left( 1+\frac{\sqrt{\phi}\alpha}{\eta}\frac{|\beta_{i}|}{\alpha} \right)^{-(\alpha+1)},
  
\end{align*}$$
  
which is the density of a \(GDP(\xi=\frac{\eta}{\sqrt{\phi}\alpha},\alpha)\).

## EM Algorithm

\(\tau\_{i}\) and \(\lambda\_{i}\) are treated as missing data for each \(i\).
  
\begin{align*}
	  
Q(\beta,\phi||\beta^{(t)},\phi^{(t)})&=c+\mathbb{E}_{\tau,\lambda}\left[ \log p(\beta,\phi|Y,\tau,\lambda)|\beta^{(t)},\phi^{(t)} \right]\\
	  
&=\frac{n+p-3}{2}\log\phi &#8211; \frac{\phi}{2}||Y-X\beta||^{2}-\frac{\phi}{2}\sum\_{i=1}^{p}\beta\_{i}^{2}\mathbb{E}\left[ \frac{1}{\tau_{i}} \right]\\
  
\end{align*}

## Expectation

For the iterated expectation one needs the distribution \(\tau\_{i}|\lambda\_{i},\beta\_{i},\phi\) and \(\lambda\_{i}|\beta_{i},\phi\).
  
\begin{align*}
	  
p(\tau\_{i}|\beta\_{i},\lambda\_{i},\phi)&\propto p(\beta\_{i}|\phi,\tau\_{i})p(\tau\_{i}|\lambda_{i})\\
	  
&\propto \left( \frac{1}{\tau\_{i}} \right)^{\frac{1}{2}}e^{-\frac{1}{2}(\frac{\phi \beta\_{i}^{2}}{\tau\_{i}}+\lambda\_{i}^{2}\tau_{i})}
  
\end{align*}
  
This is the kernel of a Generalized Inverse Gaussian distribution, specifically \(p(\tau\_{i}|\beta\_{i},\lambda\_{i},\phi)=GIG(\tau\_{i}:\lambda\_{i}^{2},\phi \beta\_{i}^{2},\frac{1}{2})\).
   
By a standard change of variables it follows that \(p(\frac{1}{\tau\_{i}}|\beta\_{i},\lambda\_{i},\phi)=IG(\frac{1}{\tau\_{i}}:\sqrt{\frac{\lambda\_{i}^{2}}{\phi \beta\_{i}^{2}}},\lambda\_{i}^{2})\) and so \(\mathbb{E}\left[ \frac{1}{\tau\_{i}}|\lambda\_{i},\beta^{(t)},\phi^{(t)} \right]=\frac{\lambda\_{i}}{\sqrt{\phi^{(t)}}|\beta_{i}^{(t)}|}\).

Recall that \(p(\beta\_{i}|\phi,\lambda\_{i})\) has a double exponential distribution with rate \(\sqrt{\phi}\lambda_{i}\).
   
Hence from \(p(\lambda\_{i}|\beta\_{i},\phi)\propto p(\beta\_{i}|\lambda\_{i},\phi)p(\lambda\_{i})\) it follows that \(\lambda\_{i}|\beta\_{i},\phi \sim Ga(\alpha+1,\eta+\sqrt{\phi}|\beta\_{i}|)\), then performing the expectation with respect to \(\lambda_{i}\) yields
  
\begin{align*}
	  
\mathbb{E}\left[ \frac{1}{\tau\_{i}}|\beta^{(t)},\phi^{(t)} \right]=\left( \frac{\alpha+1}{\eta+\sqrt{\phi^{t}}|\beta\_{i}^{(t)}|} \right)\left( \frac{1}{\sqrt{\phi^{(t)}}|\beta_{i}^{(t)}|} \right)
  
\end{align*}

## Maximization

Writing \(D^{(t)}=diag(\mathbb{E}[\frac{1}{\tau\_{1}}],\dots,\mathbb{E}[\frac{1}{\tau\_{p}}])\) the function to maximize is
  
\begin{align*}
	  
Q(\beta,\phi||\beta^{(t)},\phi^{(t)})&=c+\mathbb{E}_{\tau,\lambda}\left[ \log p(\beta,\phi|Y,\tau,\lambda)|\beta^{(t)},\phi^{(t)} \right]\\
	  
&=\frac{n+p-3}{2}\log\phi &#8211; \frac{\phi}{2}||Y-X\beta||^{2}-\frac{\phi}{2}\beta^{&#8216;}D^{(t)}\beta,\\
  
\end{align*}
  
which is maximized by letting
  
\begin{align*}
	  
\beta^{(t+1)}&=(X^{&#8216;}X+D^{(t)})^{-1}X^{&#8216;}Y\\
	  
\phi^{(t+1)}&=\frac{n+p-3}{Y^{&#8216;}(I-X(X^{&#8216;}X+D^{(t)})^{-1}X^{&#8216;})Y}\\
	  
&=\frac{n+p-3}{||Y-X\beta^{(t+1)}||^{2}+\beta^{(t+1)&#8217;}D^(t)\beta^{(t+1)}}\\
  
\end{align*}

## R CPP Code

<pre class="brush: cpp; title: ; notranslate" title="">#include &lt;RcppArmadillo.h&gt;
// [[Rcpp::depends(RcppArmadillo)]]

using namespace Rcpp;
using namespace arma;

double gdp_log_posterior_density(int no, int p, double alpha, double eta, const Col&lt;double&gt;& yo, const Mat&lt;double&gt;& xo, const Col&lt;double&gt;& B,double phi);

// [[Rcpp::export]]
List gdp_em(NumericVector ryo, NumericMatrix rxo, SEXP ralpha, SEXP reta){

	//Define Variables//
	int p=rxo.ncol();
	int no=rxo.nrow();
	double eta=Rcpp::as&lt;double &gt;(reta);
	double alpha=Rcpp::as&lt;double &gt;(ralpha);

	//Create Data//
	arma::mat xo(rxo.begin(), no, p, false);
	arma::colvec yo(ryo.begin(), ryo.size(), false);
	yo-=mean(yo);

	//Pre-Processing//
	Col&lt;double&gt; xoyo=xo.t()*yo;
	Col&lt;double&gt; B=xoyo/no;
	Col&lt;double&gt; Babs=abs(B);
	Mat&lt;double&gt; xoxo=xo.t()*xo;
	Mat&lt;double&gt; D=eye(p,p);
	Mat&lt;double&gt; Ip=eye(p,p);
	double yoyo=dot(yo,yo);
	double deltaB;
	double deltaphi;
	double phi=no/dot(yo-xo*B,yo-xo*B);
	double lp;

	//Create Trace Matrices
	Mat&lt;double&gt; B_trace(p,20000);
	Col&lt;double&gt; phi_trace(20000);
	Col&lt;double&gt; lpd_trace(20000);

	//Run EM Algorithm//
	cout &lt;&lt; "Beginning EM Algorithm" &lt;&lt; endl;
	int t=0;
	B_trace.col(t)=B;
	phi_trace(t)=phi;
	lpd_trace(t)=gdp_log_posterior_density(no,p,alpha,eta,yo,xo,B,phi);
	do{
		t=t+1;


		Babs=abs(B);
		D=diagmat(sqrt(((eta+sqrt(phi)*Babs)%(sqrt(phi)*Babs))/(alpha+1)));
		B=D*solve(D*xoxo*D+Ip,D*xoyo);

		phi=(no+p-3)/(yoyo-dot(xoyo,B));

		//Store Values//
		B_trace.col(t)=B;
		phi_trace(t)=phi;
		lpd_trace(t)=gdp_log_posterior_density(no,p,alpha,eta,yo,xo,B,phi);

		deltaB=dot(B_trace.col(t)-B_trace.col(t-1),B_trace.col(t)-B_trace.col(t-1));
		deltaphi=phi_trace(t)-phi_trace(t-1);
	} while((deltaB&gt;0.00001 || deltaphi&gt;0.00001) && t&lt;19999);
	cout &lt;&lt; "EM Algorithm Converged in " &lt;&lt; t &lt;&lt; " Iterations" &lt;&lt; endl;

	//Resize Trace Matrices//
	B_trace.resize(p,t);
	phi_trace.resize(t);
	lpd_trace.resize(t);

	return Rcpp::List::create(
			Rcpp::Named("B") = B,
			Rcpp::Named("B_trace") = B_trace,
			Rcpp::Named("phi") = phi,
			Rcpp::Named("phi_trace") = phi_trace,
			Rcpp::Named("lpd_trace") = lpd_trace
			) ;

}



double gdp_log_posterior_density(int no, int p, double alpha, double eta, const Col&lt;double&gt;& yo, const Mat&lt;double&gt;& xo, const Col&lt;double&gt;& B,double phi){

	double lpd;
	double xi=eta/(sqrt(phi)*alpha);
	lpd=(double)0.5*((double)no-1)*log(phi/(2*M_PI))-p*log(2*xi)-(alpha+1)*sum(log(1+abs(B)/(alpha*xi)))-0.5*phi*dot(yo-xo*B,yo-xo*B)-log(phi);
	return(lpd);

}
</pre>

## An Example in R

<pre class="brush: r; title: ; notranslate" title="">rm(list=ls())
library(Rcpp)
library(RcppArmadillo)
sourceCpp("src/gdp_em.cpp")

#Generate Design Matrix
set.seed(3)
no=100
foo=rnorm(no,0,1)
sd=4
xo=cbind(foo+rnorm(no,0,sd),foo+rnorm(no,0,sd),foo+rnorm(no,0,sd),foo+rnorm(no,0,sd),foo+rnorm(no,0,sd),foo+rnorm(no,0,sd),foo+rnorm(no,0,sd),foo+rnorm(no,0,sd))
for(i in 1:40) xo=cbind(xo,foo+rnorm(no,0,sd),foo+rnorm(no,0,sd),foo+rnorm(no,0,sd),foo+rnorm(no,0,sd),foo+rnorm(no,0,sd),foo+rnorm(no,0,sd),foo+rnorm(no,0,sd))

#Scale and Center Design Matrix
xo=scale(xo,center=T,scale=F)
var=apply(xo^2,2,sum)
xo=scale(xo,center=F,scale=sqrt(var/no))

#Generate Data under True Model
p=dim(xo)[2]
b=rep(0,p)
b[1]=1
b[2]=2
b[3]=3
b[4]=4
b[5]=5
xo%*%b
yo=xo%*%b+rnorm(no,0,1)
yo=yo-mean(yo)

#Run GDP
gdp=gdp_em(yo,xo,100,100)

#Posterior Density Increasing at Every Iteration?
gdp$lpd_trace[2:dim(gdp$lpd_trace)[1],1]-gdp$lpd_trace[1:(dim(gdp$lpd_trace)[1]-1),1]&gt;=0
mean(gdp$lpd_trace[2:dim(gdp$lpd_trace)[1],1]-gdp$lpd_trace[1:(dim(gdp$lpd_trace)[1]-1),1]&gt;=0)

#Plot Results
plot(gdp$B,ylab=expression(beta[GDP]),main="GDP MAP Estimate of Regression Coefficients")
</pre>

[<img src="http://www.lindonslog.com/wp-content/uploads/2014/09/gdp.jpeg" alt="Generalized Double Pareto Estimated Coefficients" width="815" height="534" class="aligncenter size-full wp-image-1081" srcset="http://www.lindonslog.com/wp-content/uploads/2014/09/gdp.jpeg 815w, http://www.lindonslog.com/wp-content/uploads/2014/09/gdp-300x196.jpeg 300w" sizes="(max-width: 815px) 100vw, 815px" />](http://www.lindonslog.com/wp-content/uploads/2014/09/gdp.jpeg)

<span class="Z3988" title="ctx_ver=Z39.88-2004&#038;rft_val_fmt=info%3Aofi%2Ffmt%3Akev%3Amtx%3Ajournal&#038;rft_id=info%3Adoi%2Fhttp%3A%2F%2Fdx.doi.org%2F10.1093%2Fbiomet%2F74.3.646&#038;rft.atitle=On+scale+mixtures+of+normal+distributions&#038;rft.jtitle=Biometrika&#038;rft.volume=74&#038;rft.issue=3&#038;rft.spage=646&#038;rft.epage=648&#038;rft.date=1987&#038;rfr_id=info%3Asid%2Fscienceseeker.org&#038;rft.au=WEST+MIKE&#038;rft.aulast=WEST&#038;rft.aufirst=MIKE&#038;rfs_dat=ss.included=1&#038;rfe_dat=bpr3.included=1">WEST M. (1987). On scale mixtures of normal distributions, <span style="font-style:italic;">Biometrika, 74</span> (3) 646-648. DOI: <a rev="review" href="http://dx.doi.org/10.1093/biomet/74.3.646">http://dx.doi.org/10.1093/biomet/74.3.646</a></span>

<span class="Z3988" title="ctx_ver=Z39.88-2004&#038;rft_val_fmt=info%3Aofi%2Ffmt%3Akev%3Amtx%3Ajournal&#038;rft.jtitle=Statistica+Sinica+23+%282013%29%2C+119-143&#038;rft_id=info%3Aarxiv%2F1104.0861v4&#038;rfr_id=info%3Asid%2Fresearchblogging.org&#038;rft.atitle=Generalized+double+Pareto+shrinkage&#038;rft.issn=&#038;rft.date=2011&#038;rft.volume=&#038;rft.issue=&#038;rft.spage=&#038;rft.epage=&#038;rft.artnum=&#038;rft.au=Artin+Armagan&#038;rft.au=David+Dunson&#038;rft.au=Jaeyong+Lee&#038;rfe_dat=bpr3.included=1;bpr3.tags=Mathematics%2CApplied+Mathematics%2C+Probability+and+Statistics%2C+Parallel+and+Distributed+Computing%2C+Algorithms%2C+Operating+Systems">Artin Armagan, David Dunson, & Jaeyong Lee (2011). Generalized double Pareto shrinkage <span style="font-style: italic;">Statistica Sinica 23 (2013), 119-143</span> arXiv: <a rev="review" href="http://arxiv.org/abs/1104.0861v4">1104.0861v4</a></span>

<span class="Z3988" title="ctx_ver=Z39.88-2004&#038;rft_val_fmt=info%3Aofi%2Ffmt%3Akev%3Amtx%3Ajournal&#038;rft_id=info%3Adoi%2Fhttp%3A%2F%2Fdx.doi.org%2F10.1109%2Ftpami.2003.1227989&#038;rft.atitle=Adaptive+sparseness+for+supervised+learning&#038;rft.jtitle=IEEE+Transactions+on+Pattern+Analysis+and+Machine+Intelligence&#038;rft.volume=25&#038;rft.issue=9&#038;rft.spage=1150&#038;rft.epage=1159&#038;rft.date=2003&#038;rfr_id=info%3Asid%2Fscienceseeker.org&#038;rft.au=Figueiredo+M.A.T.&#038;rft.aulast=Figueiredo&#038;rft.aufirst=M.A.T.&#038;rfs_dat=ss.included=1&#038;rfe_dat=bpr3.included=1">Figueiredo M.A.T. (2003). Adaptive sparseness for supervised learning, <span style="font-style:italic;">IEEE Transactions on Pattern Analysis and Machine Intelligence, 25</span> (9) 1150-1159. DOI: <a rev="review" href="http://dx.doi.org/10.1109/tpami.2003.1227989">http://dx.doi.org/10.1109/tpami.2003.1227989</a></span>

Also see this <a href="http://www.lindonslog.com/mathematics/em-algorithm-bayesian-lasso-r-cpp-code/" title="lasso" target="_blank">similar post on the Bayesian lasso</a>.