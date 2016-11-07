---
id: 1035
title: EM Algorithm for Bayesian Lasso R Cpp Code
date: 2014-09-05T12:56:55+00:00
author: admin
layout: post
guid: http://www.lindonslog.com/?p=1035
permalink: /mathematics/em-algorithm-bayesian-lasso-r-cpp-code/
categories:
  - Mathematics
  - Programming
  - R
  - Statistics
---
# Bayesian Lasso 

$$\begin{align*}
	  
p(Y\_{o}|\beta,\phi)&=N(Y\_{o}|1\alpha+X\_{o}\beta,\phi^{-1} I\_{n{o}})\\
	  
\pi(\beta\_{i}|\phi,\tau\_{i}^{2})&=N(\beta\_{i}|0, \phi^{-1}\tau\_{i}^{2})\\
	  
\pi(\tau_{i}^{2})&=Exp \left( \frac{\lambda}{2} \right)\\
	  
\pi(\phi)&\propto \phi^{-1}\\
	  
\pi(\alpha)&\propto 1\\
  
\end{align*}$$

Marginalizing over \(\alpha\) equates to centering the observations and losing a degree of freedom and working with the centered \( Y_{o} \).
  
Mixing over \(\tau\_{i}^{2}\) leads to a Laplace or Double Exponential prior on \(\beta\_{i}\) with rate parameter \(\sqrt{\phi\lambda}\) as seen by considering the characteristic function
  
$$\begin{align*}
	  
\varphi\_{\beta\_{i}|\phi}(t)&=\int e^{jt\beta\_{i}}\pi(\beta\_{i}|\phi)d\beta_{i}\\
	  
&=\int \int e^{jt\beta\_{i}}\pi(\beta\_{i}|\phi,\tau\_{i}^{2})\pi(\tau\_{i}^{2})d\tau\_{i} d\beta\_{i}\\
	  
&=\frac{\lambda}{2} \int e^{-\frac{1}{2}\frac{t^{2}}{\phi}\tau\_{i}^{2}}e^{-\frac{\lambda}{2}\tau\_{i}^{2}}d\tau_{i}\\
	  
&=\frac{\lambda}{\frac{t^{2}}{\phi}+\lambda}=\frac{\lambda\phi}{t^{2}+\lambda\phi}
  
\end{align*}$$.

# EM Algorithm

The objective is to find the mode of the joint posterior \(\pi(\beta,\phi|Y\_{o})\). It is easier, however, to find the joint mode of \(\pi(\beta,\phi|Y\_{o},\tau^{2})\) and use EM to exploit the scale mixture representation.

$$\begin{align*}
	  
\log \pi(\beta,\phi|Y\_{o},\tau^{2})=c+ \frac{n\_o+p-3}{2}\log \phi -\frac{\phi}{2}||Y\_{o}-X\_{o}\beta||^{2}-\sum\_{i=1}^{p}\frac{\phi}{2}\frac{1}{\tau\_{i}^{2}}\beta^{2}_{i}
  
\end{align*}$$

## Expectation

The expecation w.r.t. \(\tau_{i}^{2}\) is handled as by
  
$$
  
\begin{align*}
	  
&\frac{\lambda}{2}\int \frac{1}{\tau\_{i}^{2}}\left( \frac{\phi}{2\pi\tau\_{i}^{2}} \right)^{\frac{1}{2}}e^{-\frac{1}{2}\phi\beta\_{i}^{2}\frac{1}{\tau\_{i}^{2}}}e^{-\frac{\lambda}{2}\tau\_{i}^{2}}d\tau\_{i}^{2}\\
	  
&\frac{\lambda}{2}\int \left( \frac{\phi}{2\pi[\tau\_{i}^{2}]^{3}} \right)^{\frac{1}{2}}e^{-\frac{1}{2}\phi\beta\_{i}^{2}\frac{1}{\tau\_{i}^{2}}}e^{-\frac{\lambda}{2}\tau\_{i}^{2}}d\tau_{i}^{2}\\
  
\end{align*}$$

This has the kernel of an Inverse Gaussian distribution with shape parameter \(\phi \beta\_{i}^{2}\) and mean \(\sqrt{\frac{\phi}{\lambda}}|\beta\_{i}|\)

$$\begin{align*}
	  
&\frac{{\lambda}}{2|\beta\_{i}|}\int \left( \frac{\beta\_{i}^{2}\phi}{2\pi[\tau\_{i}^{2}]^{3}} \right)^{\frac{1}{2}}e^{-\frac{1}{2}\phi\beta\_{i}^{2}\frac{1}{\tau\_{i}^{2}}}e^{-\frac{\lambda}{2}\tau\_{i}^{2}}d\tau_{i}^{2}\\
	  
&\frac{\lambda}{2|\beta\_{i}|}e^{-\sqrt{\lambda\phi\beta\_{i}^{2}}}\int \left( \frac{\beta\_{i}^{2}\phi}{2\pi[\tau\_{i}^{2}]^{3}} \right)^{\frac{1}{2}}e^{-\frac{1}{2}\phi\beta\_{i}^{2}\frac{1}{\tau\_{i}^{2}}}e^{-\frac{\lambda}{2}\tau\_{i}^{2}}e^{\sqrt{\lambda\phi\beta\_{i}^{2}}}d\tau_{i}^{2}\\
	  
&\frac{\lambda}{2|\beta\_{i}|}e^{-\sqrt{\lambda\phi\beta\_{i}^{2}}}\\
  
\end{align*}$$

Normalization as follows

$$\begin{align*}
	  
&\frac{\lambda}{2}\int \left( \frac{\phi}{2\pi\tau\_{i}^{2}} \right)^{\frac{1}{2}}e^{-\frac{1}{2}\phi\beta\_{i}^{2}\frac{1}{\tau\_{i}^{2}}}e^{-\frac{\lambda}{2}\tau\_{i}^{2}}d\tau_{i}^{2}\\
	  
&\frac{\lambda}{2}\int \tau\_{i}^{2}\left( \frac{\phi}{2\pi[\tau\_{i}^{2}]^{3}} \right)^{\frac{1}{2}}e^{-\frac{1}{2}\phi\beta\_{i}^{2}\frac{1}{\tau\_{i}^{2}}}e^{-\frac{\lambda}{2}\tau\_{i}^{2}}d\tau\_{i}^{2}\\
  
\end{align*}$$
  
$$\begin{align*}
	  
&\frac{\lambda}{2|\beta\_{i}|}e^{-\sqrt{\lambda\phi\beta\_{i}^{2}}}\sqrt{\frac{\phi}{\lambda}}|\beta_{i}|\\
  
\end{align*}$$

\( \Rightarrow \mathbb{E}\left[ \frac{1}{\tau\_{i}^{2}} \right]=\sqrt{\frac{\lambda}{\phi^{t}}}\frac{1}{|\beta\_{i}^{t}|}\).
   
Let \(\Lambda^{t}=diag(\sqrt{\frac{\lambda}{\phi^{t}}}\frac{1}{|\beta\_{1}^{t}|}, \dots, \sqrt{\frac{\lambda}{\phi^{t}}}\frac{1}{|\beta\_{p}^{t}|})\).

## Maximization

$$\begin{align*}
	  
&Q(\beta,\phi||\beta^{t},\phi^{t})=c+ \frac{n\_o+p-3}{2}\log \phi -\frac{\phi}{2}||Y\_{o}-X_{o}\beta||^{2} &#8211; \frac{\phi}{2}\beta^{T}\Lambda^{t}\beta\\
	  
&=c+ \frac{n\_o+p-3}{2}\log \phi -\frac{\phi}{2}||\beta-(X\_{o}^{T}X\_{o}+\Lambda^{t})^{-1}X\_{o}^{T}Y\_{o}||^{2}\_{(X\_{o}^{T}X\_{o}+\Lambda^{t})}-\frac{\phi}{2}Y\_{o}^{T}(I\_{n\_{o}}-X\_{o}^{T}(X\_{o}^{T}X\_{o}+\Lambda^{t})^{-1}X\_{o})Y\_{o}\\
  
\end{align*}$$

$$\begin{align*}
	  
\beta^{t+1}&=(X\_{o}^{T}X\_{o}+\Lambda^{t})^{-1}X\_{o}^{T}Y\_{o}\\
  
\end{align*}$$

$$\begin{align*}
	  
\phi^{t+1}=\frac{n\_{o}+p-3}{Y\_{o}^{T}(I\_{n\_{o}}-X\_{o}^{T}(X\_{o}^{T}X\_{o}+\Lambda^{t})^{-1}X\_{o})Y_{o}}
  
\end{align*}$$

## RCpp C++ Code

<pre class="brush: cpp; title: ; notranslate" title="">#include &lt;RcppArmadillo.h&gt;
// [[Rcpp::depends(RcppArmadillo)]]

using namespace Rcpp;
using namespace arma;

double or_log_posterior_density(int no, int p, double lasso, const Col&lt;double&gt;& yo, const Mat&lt;double&gt;& xo, const Col&lt;double&gt;& B,double phi);

// [[Rcpp::export]]
List or_lasso_em(NumericVector ryo, NumericMatrix rxo, SEXP rlasso){

	//Define Variables//
	int p=rxo.ncol();
	int no=rxo.nrow();
	double lasso=Rcpp::as&lt;double &gt;(rlasso);

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
	lpd_trace(t)=or_log_posterior_density(no,p,lasso,yo,xo,B,phi);
	do{
		t=t+1;

		lp=sqrt(lasso/phi);

		Babs=abs(B);
		D=diagmat(sqrt(Babs));
		B=D*solve(D*xoxo*D+lp*Ip,D*xoyo);

		phi=(no+p-3)/(yoyo-dot(xoyo,B));

		//Store Values//
		B_trace.col(t)=B;
		phi_trace(t)=phi;
		lpd_trace(t)=or_log_posterior_density(no,p,lasso,yo,xo,B,phi);

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




double or_log_posterior_density(int no, int p, double lasso, const Col&lt;double&gt;& yo, const Mat&lt;double&gt;& xo, const Col&lt;double&gt;& B,double phi){

	double lpd;
	lpd=(double)0.5*((double)no-1)*log(phi/(2*M_PI))-0.5*phi*dot(yo-xo*B,yo-xo*B)+0.5*(double)p*log(phi*lasso)-sqrt(phi*lasso)*sum(abs(B))-log(phi);
	return(lpd);

}
</pre>

## An Example in R

<pre class="brush: r; title: ; notranslate" title="">rm(list=ls())

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

#Run Lasso
or_lasso=or_lasso_em(yo,xo,100)

#Posterior Density Increasing at Every Iteration?
or_lasso$lpd_trace[2:dim(or_lasso$lpd_trace)[1],1]-or_lasso$lpd_trace[1:(dim(or_lasso$lpd_trace)[1]-1),1]&gt;=0
mean(or_lasso$lpd_trace[2:dim(or_lasso$lpd_trace)[1],1]-or_lasso$lpd_trace[1:(dim(or_lasso$lpd_trace)[1]-1),1]&gt;=0)

#Plot Results
plot(or_lasso$B,ylab=expression(beta[lasso]),main="Lasso MAP Estimate of Regression Coefficients")
</pre>

[<img src="http://www.lindonslog.com/wp-content/uploads/2014/09/lasso.jpeg" alt="MAP regression coefficients" width="815" height="566" class="aligncenter size-full wp-image-1062" srcset="http://www.lindonslog.com/wp-content/uploads/2014/09/lasso.jpeg 815w, http://www.lindonslog.com/wp-content/uploads/2014/09/lasso-300x208.jpeg 300w" sizes="(max-width: 815px) 100vw, 815px" />](http://www.lindonslog.com/wp-content/uploads/2014/09/lasso.jpeg)

<span class="Z3988" title="ctx_ver=Z39.88-2004&#038;rft_val_fmt=info%3Aofi%2Ffmt%3Akev%3Amtx%3Ajournal&#038;rft.jtitle=Journal+of+the+American+Statistical+Association&#038;rft_id=info%3Adoi%2F10.1198%2F016214508000000337&#038;rfr_id=info%3Asid%2Fresearchblogging.org&#038;rft.atitle=The+Bayesian+Lasso&#038;rft.issn=0162-1459&#038;rft.date=2008&#038;rft.volume=103&#038;rft.issue=482&#038;rft.spage=681&#038;rft.epage=686&#038;rft.artnum=http%3A%2F%2Fwww.tandfonline.com%2Fdoi%2Fabs%2F10.1198%2F016214508000000337&#038;rft.au=Park%2C+T.&#038;rft.au=Casella%2C+G.&#038;rfe_dat=bpr3.included=1;bpr3.tags=Mathematics%2CApplied+Mathematics%2C+Probability+and+Statistics%2C+Algorithms">Park, T., & Casella, G. (2008). The Bayesian Lasso <span style="font-style: italic;">Journal of the American Statistical Association, 103</span> (482), 681-686 DOI: <a rev="review" href="http://dx.doi.org/10.1198/016214508000000337">10.1198/016214508000000337</a></span>
  
<span class="Z3988" title="ctx_ver=Z39.88-2004&#038;rft_val_fmt=info%3Aofi%2Ffmt%3Akev%3Amtx%3Ajournal&#038;rft_id=info%3Adoi%2Fhttp%3A%2F%2Fdx.doi.org%2F10.1109%2Ftpami.2003.1227989&#038;rft.atitle=Adaptive+sparseness+for+supervised+learning&#038;rft.jtitle=IEEE+Transactions+on+Pattern+Analysis+and+Machine+Intelligence&#038;rft.volume=25&#038;rft.issue=9&#038;rft.spage=1150&#038;rft.epage=1159&#038;rft.date=2003&#038;rfr_id=info%3Asid%2Fscienceseeker.org&#038;rft.au=Figueiredo+M.A.T.&#038;rft.aulast=Figueiredo&#038;rft.aufirst=M.A.T.&#038;rfs_dat=ss.included=1&#038;rfe_dat=bpr3.included=1">Figueiredo M.A.T. (2003). Adaptive sparseness for supervised learning, <span style="font-style:italic;">IEEE Transactions on Pattern Analysis and Machine Intelligence, 25</span> (9) 1150-1159. DOI: <a rev="review" href="http://dx.doi.org/10.1109/tpami.2003.1227989">http://dx.doi.org/10.1109/tpami.2003.1227989</a></span>
  
Better Shrinkage Priors:
  
<span class="Z3988" title="ctx_ver=Z39.88-2004&#038;rft_val_fmt=info%3Aofi%2Ffmt%3Akev%3Amtx%3Ajournal&#038;rft_id=info%3Apmid%2F24478567&#038;rft.atitle=GENERALIZED+DOUBLE+PARETO+SHRINKAGE.&#038;rft.jtitle=Statistica+Sinica&#038;rft.issn=1017-0405&#038;rfr_id=info%3Asid%2Fmathblogging.org&#038;rft.au=Armagan+Artin&#038;rft.aulast=Armagan&#038;rft.aufirst=Artin&#038;rft.au=Dunson+David+B&#038;rft.aulast=Dunson&#038;rft.aufirst=David+B&#038;rft.au=Lee+Jaeyong&#038;rft.aulast=Lee&#038;rft.aufirst=Jaeyong&#038;rfs_dat=ss.included=1&#038;rfe_dat=bpr3.included=1">Armagan A., Dunson D.B. & Lee J. GENERALIZED DOUBLE PARETO SHRINKAGE., <span style="font-style:italic;">Statistica Sinica, </span> PMID: <a rel="author" href="http://www.ncbi.nlm.nih.gov/pubmed/24478567">24478567</a></span>