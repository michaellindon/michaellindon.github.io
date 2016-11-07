---
id: 973
title: Monotonicity of EM Algorithm Proof
date: 2014-04-19T16:25:41+00:00
author: admin
layout: post
guid: http://www.lindonslog.com/?p=973
permalink: /mathematics/monotonicity-em-algorithm-proof/
categories:
  - Mathematics
  - Statistics
---

  
Here the monotonicity of the EM algorithm is established.
  
$$ f\_{o}(Y\_{o}|\theta)=f\_{o,m}(Y\_{o},Y\_{m}|\theta)/f\_{m|o}(Y\_{m}|Y\_{o},\theta)$$
  
$$ \log L\_{o}(\theta)=\log L\_{o,m}(\theta)-\log f\_{m|o}(Y\_{m}|Y_{o},\theta) \label{eq:loglikelihood} $$
  
where \( L\_{o}(\theta)\) is the likelihood under the observed data and \(L\_{o,m}(\theta)\) is the likelihood under the complete data. Taking the expectation of the second line with respect to the conditional distribution of \(Y\_{m}\) given \(Y\_{o}\) and the current parameters \(\theta^{k}\) yields
  
$$\log L\_{o}(\theta)= \mathbb{E}\_{Y\_{m}}\left[\log L\_{o,m}(\theta)|Y\_{o},\theta^{k}\right]-\mathbb{E}\_{Y\_{m}}\left[\log f\_{m|o}(Y\_{m}|Y\_{o},\theta)|Y_{o},\theta^{k} \right]$$
  
which is used to construct the difference between the log-likelihood of a new value of \(\theta\) and the current value \(\theta^{k}\) as
  
\begin{equation}
	  
\begin{split}
		  
\log L\_{o}(\theta)-&\log L\_{o}(\theta^{k})=\mathbb{E}\_{Y\_{m}}\left[ \log L\_{o,m}(\theta)|Y\_{o},\theta^{k}\right]-\mathbb{E}\_{Y\_{m}}\left[ \log L\_{o,m}(\theta^{k})|Y\_{o},\theta^{k}\right] \\
		  
+&\mathbb{E}\_{Y\_{m}}\left[ \log f\_{m|o}(Y\_{m}|Y\_{o},\theta^{k})|Y\_{o},\theta^{k} \right]-\mathbb{E}\_{Y\_{m}}\left[ \log f\_{m|o}(Y\_{m}|Y\_{o},\theta)|Y\_{o},\theta^{k} \right],\\
  
\end{split}
  
\end{equation}
  
or by adopting common notation as
  
\begin{equation}
		  
\log L\_{o}(\theta)-\log L\_{o}(\theta^{k})=Q(\theta;\theta^{k})-Q(\theta^{k};\theta^{k})+H(\theta^{k};\theta^{k})-H(\theta;\theta^{k}).\\
  
\end{equation}
  
Consider the last two &#8220;\( H\)&#8221; terms, then by Jensen&#8217;s inequality

\begin{align*}
	         
&-\mathbb{E}\_{Y\_{m}}\left[ \log f\_{m|o}(Y\_{m}|Y\_{o},\theta)- \log f\_{m|o}(Y\_{m}|Y\_{o},\theta^{k})|Y_{o},\theta^{k} \right]\\
	         
&=-\mathbb{E}\_{Y\_{m}}\left[\log \frac{ f\_{m|o}(Y\_{m}|Y\_{o},\theta)}{ f\_{m|o}(Y\_{m}|Y\_{o},\theta^{k})}|Y_{o},\theta^{k} \right]\\
	         
&\geq-\log \mathbb{E}\_{Y\_{m}}\left[ \frac{ f\_{m|o}(Y\_{m}|Y\_{o},\theta)}{ f\_{m|o}(Y\_{m}|Y\_{o},\theta^{k})}|Y_{o},\theta^{k} \right]\\
	         
&=-\log \int f\_{m|o}(Y\_{m}|Y\_{o},\theta)dY\_{m}\\
	         
&=0 \; \; \; \; \; \; \;\forall \theta\in \Theta.
  
\end{align*}
  
It follows that \(\log L\_{o}(\theta)-\log L\_{o}(\theta^{k})\geq 0\) by choosing \(\theta\) such that \(Q(\theta;\theta^{k})-Q(\theta^{k};\theta^{k})\geq 0\).

<span class="Z3988" title="ctx_ver=Z39.88-2004&#038;rft_val_fmt=info%3Aofi%2Ffmt%3Akev%3Amtx%3Ajournal&#038;rft.jtitle=arXiv&#038;rft_id=info%3Aarxiv%2F1212.2490v1&#038;rfr_id=info%3Asid%2Fresearchblogging.org&#038;rft.atitle=On+the+Convergence+of+Bound+Optimization+Algorithms&#038;rft.issn=&#038;rft.date=2012&#038;rft.volume=&#038;rft.issue=&#038;rft.spage=&#038;rft.epage=&#038;rft.artnum=&#038;rft.au=Ruslan+R+Salakhutdinov&#038;rft.au=Sam+T+Roweis&#038;rft.au=Zoubin+Ghahramani&#038;rfe_dat=bpr3.included=1;bpr3.tags=Mathematics%2CApplied+Mathematics%2C+Probability+and+Statistics%2C+Parallel+and+Distributed+Computing%2C+Algorithms%2C+Operating+Systems">Ruslan R Salakhutdinov, Sam T Roweis, & Zoubin Ghahramani (2012). On the Convergence of Bound Optimization Algorithms <span style="font-style: italic;">arXiv</span> arXiv: <a rev="review" href="http://arxiv.org/abs/1212.2490v1">1212.2490v1</a></span>
  
<span class="Z3988" title="ctx_ver=Z39.88-2004&#038;rft_val_fmt=info%3Aofi%2Ffmt%3Akev%3Amtx%3Ajournal&#038;rft_id=info%3Adoi%2F10.1214%2Faos%2F1176346060&#038;rft.atitle=On+the+Convergence+Properties+of+the+EM+Algorithm&#038;rft.jtitle=The+Annals+of+Statistics&#038;rft.artnum=http%3A%2F%2Fprojecteuclid.org%2Feuclid.aos%2F1176346060&#038;rft.volume=11&#038;rft.issue=1&#038;rft.issn=0090-5364&#038;rft.spage=95&#038;rft.epage=103&#038;rft.date=1983&#038;rfr_id=info%3Asid%2Fscienceseeker.org&#038;rft.au=Wu+C.+F.+Jeff&#038;rft.aulast=Wu&#038;rft.aufirst=C.+F.+Jeff&#038;rfs_dat=ss.included=1&#038;rfe_dat=bpr3.included=1;bpr3.tags=Mathematics">Wu C.F.J. (1983). On the Convergence Properties of the EM Algorithm, <span style="font-style:italic;">The Annals of Statistics, 11</span> (1) 95-103. DOI: <a rel="author" href="http://dx.doi.org/10.1214%2Faos%2F1176346060">10.1214/aos/1176346060</a></span>
  
<span class="Z3988" title="ctx_ver=Z39.88-2004&#038;rft_val_fmt=info%3Aofi%2Ffmt%3Akev%3Amtx%3Ajournal&#038;rft_id=info%3Adoi%2F10.1002%2F0471721182&#038;rft.artnum=http%3A%2F%2Fdoi.wiley.com%2F10.1002%2F0471721182&#038;rfr_id=info%3Asid%2Fmathblogging.org&#038;rft.au=McLachlan+Geoffrey&#038;rft.aulast=McLachlan&#038;rft.aufirst=Geoffrey&#038;rft.au=Peel+David&#038;rft.aulast=Peel&#038;rft.aufirst=David&#038;rfs_dat=ss.included=1&#038;rfe_dat=bpr3.included=1;bpr3.tags=Mathematics">McLachlan G. & Peel D. <span style="font-style:italic;"> </span> DOI: <a rel="author" href="http://dx.doi.org/10.1002%2F0471721182">10.1002/0471721182</a></span>