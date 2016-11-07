---
id: 663
title: Woodbury Matrix Inverse Identity
date: 2013-07-31T19:10:37+00:00
author: admin
layout: post
guid: http://www.lindonslog.com/?p=663
permalink: /mathematics/woodbury-matrix-inverse-multivariate-normal/
categories:
  - Linear Algebra
  - Mathematics
  - Statistics
---
# Application in Conditional Distribution of Multivariate Normal

The Sherman-Woodbury-Morrison matrix inverse identity can be regarded as a transform between Schur complements. That is, given  <img src="//s0.wp.com/latex.php?latex=V_%7B22.1%7D%5E%7B-1%7D&#038;bg=ffffff&#038;fg=000&#038;s=0" alt="V_{22.1}^{-1}" title="V_{22.1}^{-1}" class="latex" />one can obtain  <img src="//s0.wp.com/latex.php?latex=V_%7B11.2%7D%5E%7B-1%7D&#038;bg=ffffff&#038;fg=000&#038;s=0" alt="V_{11.2}^{-1}" title="V_{11.2}^{-1}" class="latex" />by using the Woodbury matrix identity and vice versa. Recall the <a title="Invert Matrix Woodbury Matrix Inverse Formula Identity" href="http://www.lindonslog.com/mathematics/invert-a-matrix-using-the-woodbury-matrix-inverse-formula-identity/" target="_blank">Woodbury Identity</a>:

<p align="center">
  <img src="//s0.wp.com/latex.php?latex=V_%7B11.2%7D%5E%7B-1%7D%3DV_%7B11%7D%5E%7B-1%7D%2BV_%7B11%7D%5E%7B-1%7DV_%7B12%7DV_%7B22.1%7D%5E%7B-1%7DV_%7B21%7DV_%7B11%7D%5E%7B-1%7D+&#038;bg=ffffff&#038;fg=000&#038;s=0" alt="V_{11.2}^{-1}=V_{11}^{-1}+V_{11}^{-1}V_{12}V_{22.1}^{-1}V_{21}V_{11}^{-1} " title="V_{11.2}^{-1}=V_{11}^{-1}+V_{11}^{-1}V_{12}V_{22.1}^{-1}V_{21}V_{11}^{-1} " class="latex" />
</p>

and

<p align="center">
  <img src="//s0.wp.com/latex.php?latex=V_%7B22.1%7D%5E%7B-1%7D%3DV_%7B22%7D%5E%7B-1%7D%2BV_%7B22%7D%5E%7B-1%7DV_%7B21%7DV_%7B11.2%7D%5E%7B-1%7DV_%7B12%7DV_%7B22%7D%5E%7B-1%7D+&#038;bg=ffffff&#038;fg=000&#038;s=0" alt="V_{22.1}^{-1}=V_{22}^{-1}+V_{22}^{-1}V_{21}V_{11.2}^{-1}V_{12}V_{22}^{-1} " title="V_{22.1}^{-1}=V_{22}^{-1}+V_{22}^{-1}V_{21}V_{11.2}^{-1}V_{12}V_{22}^{-1} " class="latex" />
</p>

I recently stumbled across a neat application of this whilst deriving full conditionals for a multivariate normal. Recall that if the data are partitioned into two blocks, <img src="//s0.wp.com/latex.php?latex=Y_%7B1%7D%2CY_%7B2%7D&#038;bg=ffffff&#038;fg=000&#038;s=0" alt="Y_{1},Y_{2}" title="Y_{1},Y_{2}" class="latex" />, then the variance of the conditional distribution  <img src="//s0.wp.com/latex.php?latex=Y_%7B1%7D%7CY_%7B2%7D%2C-&#038;bg=ffffff&#038;fg=000&#038;s=0" alt="Y_{1}|Y_{2},-" title="Y_{1}|Y_{2},-" class="latex" />is the Schur complement of the block  <img src="//s0.wp.com/latex.php?latex=V_%7B22%7D&#038;bg=ffffff&#038;fg=000&#038;s=0" alt="V_{22}" title="V_{22}" class="latex" />of total variance matrix <img src="//s0.wp.com/latex.php?latex=V&#038;bg=ffffff&#038;fg=000&#038;s=0" alt="V" title="V" class="latex" />, that is, the variance of the conditional distribution is  <img src="//s0.wp.com/latex.php?latex=V_%7B11.2%7D%3DV_%7B11%7D-V_%7B12%7DV_%7B22%7D%5E%7B-1%7DV_%7B21%7D&#038;bg=ffffff&#038;fg=000&#038;s=0" alt="V_{11.2}=V_{11}-V_{12}V_{22}^{-1}V_{21}" title="V_{11.2}=V_{11}-V_{12}V_{22}^{-1}V_{21}" class="latex" />which is the variance of  <img src="//s0.wp.com/latex.php?latex=Y_%7B1%7D&#038;bg=ffffff&#038;fg=000&#038;s=0" alt="Y_{1}" title="Y_{1}" class="latex" />subtracted by something corresponding to the reduction in uncertainty about  <img src="//s0.wp.com/latex.php?latex=Y_%7B1%7D&#038;bg=ffffff&#038;fg=000&#038;s=0" alt="Y_{1}" title="Y_{1}" class="latex" />gained from the knowledge about <img src="//s0.wp.com/latex.php?latex=Y_%7B2%7D&#038;bg=ffffff&#038;fg=000&#038;s=0" alt="Y_{2}" title="Y_{2}" class="latex" />. If, however,  <img src="//s0.wp.com/latex.php?latex=V_%7B22%7D&#038;bg=ffffff&#038;fg=000&#038;s=0" alt="V_{22}" title="V_{22}" class="latex" />has the form of a Schur complement itself, then it may be possible to exploit the Woodbury identity above to considerably simplify the variance term. I came across this when I derived two very different-looking expressions for the conditional distribution and found them equivalent by the Woodbury identity. Consider the model

<p align="center">
  <img src="//s0.wp.com/latex.php?latex=%5Cbegin%7Bbmatrix%7D+++Y_%7B1%7D%5C%5C+++Y_%7B2%7D++%5Cend%7Bbmatrix%7D++%3D++%5Cbegin%7Bbmatrix%7D+++X_%7B1%7D%5C%5C+++X_%7B2%7D++%5Cend%7Bbmatrix%7D%5Cbeta_%7B+%7D+%2B+%5Cvarepsilon++&#038;bg=ffffff&#038;fg=000&#038;s=0" alt="&#92;begin{bmatrix}   Y_{1}&#92;&#92;   Y_{2}  &#92;end{bmatrix}  =  &#92;begin{bmatrix}   X_{1}&#92;&#92;   X_{2}  &#92;end{bmatrix}&#92;beta_{ } + &#92;varepsilon  " title="&#92;begin{bmatrix}   Y_{1}&#92;&#92;   Y_{2}  &#92;end{bmatrix}  =  &#92;begin{bmatrix}   X_{1}&#92;&#92;   X_{2}  &#92;end{bmatrix}&#92;beta_{ } + &#92;varepsilon  " class="latex" />
</p>

where

<p align="center">
  <img src="//s0.wp.com/latex.php?latex=%5Cvarepsilon+%5Csim+N%5Cleft%28+%5Cbegin%7Bbmatrix%7D0%5C%5C+0%5Cend%7Bbmatrix%7D%2C+%5Csigma%5E%7B2%7D+%5Cbegin%7Bbmatrix%7DI_%7B1%7D+%26+0+%5C%5C+0+%26+I_%7B2%7D%5Cend%7Bbmatrix%7D+%5Cright%29+&#038;bg=ffffff&#038;fg=000&#038;s=0" alt="&#92;varepsilon &#92;sim N&#92;left( &#92;begin{bmatrix}0&#92;&#92; 0&#92;end{bmatrix}, &#92;sigma^{2} &#92;begin{bmatrix}I_{1} & 0 &#92;&#92; 0 & I_{2}&#92;end{bmatrix} &#92;right) " title="&#92;varepsilon &#92;sim N&#92;left( &#92;begin{bmatrix}0&#92;&#92; 0&#92;end{bmatrix}, &#92;sigma^{2} &#92;begin{bmatrix}I_{1} & 0 &#92;&#92; 0 & I_{2}&#92;end{bmatrix} &#92;right) " class="latex" />
</p>

<p align="center">
  <img src="//s0.wp.com/latex.php?latex=%5Cbeta_%7B+%7D%7C+%2C%5Csigma%5E%7B2%7D+%5Csim+N%280%2C+%5Csigma%5E%7B2%7D%5CLambda%5E%7B-1%7D%29&#038;bg=ffffff&#038;fg=000&#038;s=0" alt="&#92;beta_{ }| ,&#92;sigma^{2} &#92;sim N(0, &#92;sigma^{2}&#92;Lambda^{-1})" title="&#92;beta_{ }| ,&#92;sigma^{2} &#92;sim N(0, &#92;sigma^{2}&#92;Lambda^{-1})" class="latex" />
</p>

.
  
I was seeking the distribution  <img src="//s0.wp.com/latex.php?latex=Y_%7B1%7D%7C+Y_%7B2%7D%2C%5Csigma%5E%7B2%7D+&#038;bg=ffffff&#038;fg=000&#038;s=0" alt="Y_{1}| Y_{2},&#92;sigma^{2} " title="Y_{1}| Y_{2},&#92;sigma^{2} " class="latex" />and arrived there through two different paths. The distributions derived looked very different, but they turned out to be equivalent upon considering the Woodbury identity.

## Method 1

This simply manipulates properties of the multivariate normal. Marginalizing over  <img src="//s0.wp.com/latex.php?latex=%5Cbeta&#038;bg=ffffff&#038;fg=000&#038;s=0" alt="&#92;beta" title="&#92;beta" class="latex" />one gets

<p align="center">
  <img src="//s0.wp.com/latex.php?latex=Cov+%5Cbegin%7Bbmatrix%7D+++Y_%7B1%7D+%5C%5C++Y_%7B2%7D++%5Cend%7Bbmatrix%7D+%3D+%5Cbegin%7Bbmatrix%7D+++X_%7B1%7D+%5C%5C++X_%7B2%7D++%5Cend%7Bbmatrix%7D+Cov+%28%5Cbeta_%7B+%7D%29++%5Cbegin%7Bbmatrix%7D+++X_%7B1%7D%5E%7BT%7D+%26++X_%7B2%7D%5E%7BT%7D++%5Cend%7Bbmatrix%7D+%2B+Cov%28%5Cvarepsilon%29++&#038;bg=ffffff&#038;fg=000&#038;s=0" alt="Cov &#92;begin{bmatrix}   Y_{1} &#92;&#92;  Y_{2}  &#92;end{bmatrix} = &#92;begin{bmatrix}   X_{1} &#92;&#92;  X_{2}  &#92;end{bmatrix} Cov (&#92;beta_{ })  &#92;begin{bmatrix}   X_{1}^{T} &  X_{2}^{T}  &#92;end{bmatrix} + Cov(&#92;varepsilon)  " title="Cov &#92;begin{bmatrix}   Y_{1} &#92;&#92;  Y_{2}  &#92;end{bmatrix} = &#92;begin{bmatrix}   X_{1} &#92;&#92;  X_{2}  &#92;end{bmatrix} Cov (&#92;beta_{ })  &#92;begin{bmatrix}   X_{1}^{T} &  X_{2}^{T}  &#92;end{bmatrix} + Cov(&#92;varepsilon)  " class="latex" />
</p>

<p align="center">
  <img src="//s0.wp.com/latex.php?latex=Cov+%5Cbegin%7Bbmatrix%7D+++Y_%7B1%7D+%5C%5C++Y_%7B2%7D++%5Cend%7Bbmatrix%7D+%3D+%5Csigma%5E%7B2%7D%5Cbegin%7Bbmatrix%7D+++X_%7B1%7D%5CLambda%5E%7B-1%7D+X_%7B1%7D%5E%7BT%7D+%26++X_%7B1%7D%5CLambda%5E%7B-1%7D+X_%7B2%7D%5E%7BT%7D+%5C%5C++X_%7B2%7D%5CLambda%5E%7B-1%7D+X_%7B1%7D%5E%7BT%7D+%26++X_%7B2%7D%5CLambda%5E%7B-1%7D+X_%7B2%7D%5E%7BT%7D++%5Cend%7Bbmatrix%7D++%2B+%5Csigma%5E%7B2%7D++%5Cbegin%7Bbmatrix%7D++I_%7B1%7D+%26+0+%5C%5C+0+%26+I_%7B2%7D++%5Cend%7Bbmatrix%7D++&#038;bg=ffffff&#038;fg=000&#038;s=0" alt="Cov &#92;begin{bmatrix}   Y_{1} &#92;&#92;  Y_{2}  &#92;end{bmatrix} = &#92;sigma^{2}&#92;begin{bmatrix}   X_{1}&#92;Lambda^{-1} X_{1}^{T} &  X_{1}&#92;Lambda^{-1} X_{2}^{T} &#92;&#92;  X_{2}&#92;Lambda^{-1} X_{1}^{T} &  X_{2}&#92;Lambda^{-1} X_{2}^{T}  &#92;end{bmatrix}  + &#92;sigma^{2}  &#92;begin{bmatrix}  I_{1} & 0 &#92;&#92; 0 & I_{2}  &#92;end{bmatrix}  " title="Cov &#92;begin{bmatrix}   Y_{1} &#92;&#92;  Y_{2}  &#92;end{bmatrix} = &#92;sigma^{2}&#92;begin{bmatrix}   X_{1}&#92;Lambda^{-1} X_{1}^{T} &  X_{1}&#92;Lambda^{-1} X_{2}^{T} &#92;&#92;  X_{2}&#92;Lambda^{-1} X_{1}^{T} &  X_{2}&#92;Lambda^{-1} X_{2}^{T}  &#92;end{bmatrix}  + &#92;sigma^{2}  &#92;begin{bmatrix}  I_{1} & 0 &#92;&#92; 0 & I_{2}  &#92;end{bmatrix}  " class="latex" />
</p>

.
  
Such that the distribution

<p align="center">
  <img src="//s0.wp.com/latex.php?latex=%5Cbegin%7Bbmatrix%7D+++Y_%7B1%7D%5C%5C+++Y_%7B2%7D++%5Cend%7Bbmatrix%7D%7C+%2C%5Csigma%5E%7B2%7D+%5Csim+N+%5Cleft%28++%5Cbegin%7Bbmatrix%7D++0%5C%5C++0++%5Cend%7Bbmatrix%7D%2C++%5Csigma%5E%7B2%7D+%5Cleft%28+%5Cbegin%7Bbmatrix%7D++I_%7B1%7D%2B++X_%7B1%7D%5CLambda%5E%7B-1%7D+X_%7B1%7D%5E%7BT%7D+%26++X_%7B1%7D%5CLambda%5E%7B-1%7D+X_%7B2%7D%5E%7BT%7D+%5C%5C++X_%7B2%7D%5CLambda%5E%7B-1%7D+X_%7B1%7D%5E%7BT%7D+%26+I_%7B2%7D%2B+X_%7B2%7D%5CLambda%5E%7B-1%7D+X_%7B2%7D%5E%7BT%7D++%5Cend%7Bbmatrix%7D++%5Cright%29+%5Cright%29++&#038;bg=ffffff&#038;fg=000&#038;s=0" alt="&#92;begin{bmatrix}   Y_{1}&#92;&#92;   Y_{2}  &#92;end{bmatrix}| ,&#92;sigma^{2} &#92;sim N &#92;left(  &#92;begin{bmatrix}  0&#92;&#92;  0  &#92;end{bmatrix},  &#92;sigma^{2} &#92;left( &#92;begin{bmatrix}  I_{1}+  X_{1}&#92;Lambda^{-1} X_{1}^{T} &  X_{1}&#92;Lambda^{-1} X_{2}^{T} &#92;&#92;  X_{2}&#92;Lambda^{-1} X_{1}^{T} & I_{2}+ X_{2}&#92;Lambda^{-1} X_{2}^{T}  &#92;end{bmatrix}  &#92;right) &#92;right)  " title="&#92;begin{bmatrix}   Y_{1}&#92;&#92;   Y_{2}  &#92;end{bmatrix}| ,&#92;sigma^{2} &#92;sim N &#92;left(  &#92;begin{bmatrix}  0&#92;&#92;  0  &#92;end{bmatrix},  &#92;sigma^{2} &#92;left( &#92;begin{bmatrix}  I_{1}+  X_{1}&#92;Lambda^{-1} X_{1}^{T} &  X_{1}&#92;Lambda^{-1} X_{2}^{T} &#92;&#92;  X_{2}&#92;Lambda^{-1} X_{1}^{T} & I_{2}+ X_{2}&#92;Lambda^{-1} X_{2}^{T}  &#92;end{bmatrix}  &#92;right) &#92;right)  " class="latex" />
</p>

It follows that the conditional distribution is
  
<img src="//s0.wp.com/latex.php?latex=Y_%7B1%7D%7C+Y_%7B2%7D+%2C%5Csigma%5E%7B2%7D+%5Csim+N+%5Cleft%28++X_%7B1%7D%5CLambda%5E%7B-1%7D+X_%7B2%7D%5E%7BT%7D+%5Cleft%5B++X_%7B2%7D%5CLambda%5E%7B-1%7D+X_%7B2%7D%5E%7BT%7D+%2B+I_%7B2%7D%5Cright%5D%5E%7B-1%7D+Y_%7B2%7D%2C+I_%7B1%7D+%2B++X_%7B1%7D%5CLambda%5E%7B-1%7D+X_%7B1%7D%5E%7BT%7D+-++X_%7B1%7D%5CLambda%5E%7B-1%7D+X_%7B2%7D%5E%7BT%7D+%5Cleft%5B+I_%7B2%7D+%2B++X_%7B2%7D%5CLambda%5E%7B-1%7D+X_%7B2%7D%5E%7BT%7D+%5Cright%5D%5E%7B-1%7D++X_%7B2%7D%5CLambda%5E%7B-1%7D+X_%7B1%7D%5E%7BT%7D%5Cright%29.++&#038;bg=ffffff&#038;fg=000&#038;s=0" alt="Y_{1}| Y_{2} ,&#92;sigma^{2} &#92;sim N &#92;left(  X_{1}&#92;Lambda^{-1} X_{2}^{T} &#92;left[  X_{2}&#92;Lambda^{-1} X_{2}^{T} + I_{2}&#92;right]^{-1} Y_{2}, I_{1} +  X_{1}&#92;Lambda^{-1} X_{1}^{T} -  X_{1}&#92;Lambda^{-1} X_{2}^{T} &#92;left[ I_{2} +  X_{2}&#92;Lambda^{-1} X_{2}^{T} &#92;right]^{-1}  X_{2}&#92;Lambda^{-1} X_{1}^{T}&#92;right).  " title="Y_{1}| Y_{2} ,&#92;sigma^{2} &#92;sim N &#92;left(  X_{1}&#92;Lambda^{-1} X_{2}^{T} &#92;left[  X_{2}&#92;Lambda^{-1} X_{2}^{T} + I_{2}&#92;right]^{-1} Y_{2}, I_{1} +  X_{1}&#92;Lambda^{-1} X_{1}^{T} -  X_{1}&#92;Lambda^{-1} X_{2}^{T} &#92;left[ I_{2} +  X_{2}&#92;Lambda^{-1} X_{2}^{T} &#92;right]^{-1}  X_{2}&#92;Lambda^{-1} X_{1}^{T}&#92;right).  " class="latex" />
  
This looks a bit nasty, but notice that  <img src="//s0.wp.com/latex.php?latex=V_%7B22%7D%5E%7B-1%7D&#038;bg=ffffff&#038;fg=000&#038;s=0" alt="V_{22}^{-1}" title="V_{22}^{-1}" class="latex" />looks like it too could be a Schur complement of some matrix.

## Method 2

An alternative route to this distribution is

<p align="center">
  <img src="//s0.wp.com/latex.php?latex=f%28+Y_%7B1%7D%7C+Y_%7B2%7D%2C%5Csigma%5E%7B2%7D+%29%3D%5Cint+f%28+Y_%7B1%7D%7C%5Csigma%5E%7B2%7D+%2C%5Cbeta_%7B+%7D%29%5Cpi%28%5Cbeta_%7B+%7D%7C+Y_%7B2%7D%2C%5Csigma%5E%7B2%7D+%29d%5Cbeta_%7B+%7D++&#038;bg=ffffff&#038;fg=000&#038;s=0" alt="f( Y_{1}| Y_{2},&#92;sigma^{2} )=&#92;int f( Y_{1}|&#92;sigma^{2} ,&#92;beta_{ })&#92;pi(&#92;beta_{ }| Y_{2},&#92;sigma^{2} )d&#92;beta_{ }  " title="f( Y_{1}| Y_{2},&#92;sigma^{2} )=&#92;int f( Y_{1}|&#92;sigma^{2} ,&#92;beta_{ })&#92;pi(&#92;beta_{ }| Y_{2},&#92;sigma^{2} )d&#92;beta_{ }  " class="latex" />
</p>

where

<p align="center">
  <img src="//s0.wp.com/latex.php?latex=%5Cbeta_%7B+%7D%7C+Y_%7B2%7D+%2C%5Csigma%5E%7B2%7D%5Csim+N+%5Cleft%28+%28+X_%7B2%7D%5E%7BT%7D+X_%7B2%7D%2B%5CLambda%29%5E%7B-1%7D+X_%7B2%7D%5E%7BT%7D+Y_%7B2%7D%2C+%5Csigma%5E%7B2%7D%28+X_%7B2%7D%5E%7BT%7D+X_%7B2%7D%2B%5CLambda%29%5E%7B-1%7D+%5Cright%29.++&#038;bg=ffffff&#038;fg=000&#038;s=0" alt="&#92;beta_{ }| Y_{2} ,&#92;sigma^{2}&#92;sim N &#92;left( ( X_{2}^{T} X_{2}+&#92;Lambda)^{-1} X_{2}^{T} Y_{2}, &#92;sigma^{2}( X_{2}^{T} X_{2}+&#92;Lambda)^{-1} &#92;right).  " title="&#92;beta_{ }| Y_{2} ,&#92;sigma^{2}&#92;sim N &#92;left( ( X_{2}^{T} X_{2}+&#92;Lambda)^{-1} X_{2}^{T} Y_{2}, &#92;sigma^{2}( X_{2}^{T} X_{2}+&#92;Lambda)^{-1} &#92;right).  " class="latex" />
</p>

It follows that

<p align="center">
  <img src="//s0.wp.com/latex.php?latex=Y_%7B1%7D%7C+Y_%7B2%7D+%2C%5Csigma%5E%7B2%7D+%5Csim+N%5Cleft%28++X_%7B1%7D%28+X_%7B2%7D%5E%7BT%7D+X_%7B2%7D%2B%5CLambda%29%5E%7B-1%7D+X_%7B2%7D%5E%7BT%7D+Y_%7B2%7D%2C+%5Csigma%5E%7B2%7D+%28I_%7B1%7D+%2B++X_%7B1%7D+%28+X_%7B2%7D%5E%7BT%7D+X_%7B2%7D%2B%5CLambda%29%5E%7B-1%7D+X_%7B1%7D%5E%7BT%7D%29+%5Cright%29++&#038;bg=ffffff&#038;fg=000&#038;s=0" alt="Y_{1}| Y_{2} ,&#92;sigma^{2} &#92;sim N&#92;left(  X_{1}( X_{2}^{T} X_{2}+&#92;Lambda)^{-1} X_{2}^{T} Y_{2}, &#92;sigma^{2} (I_{1} +  X_{1} ( X_{2}^{T} X_{2}+&#92;Lambda)^{-1} X_{1}^{T}) &#92;right)  " title="Y_{1}| Y_{2} ,&#92;sigma^{2} &#92;sim N&#92;left(  X_{1}( X_{2}^{T} X_{2}+&#92;Lambda)^{-1} X_{2}^{T} Y_{2}, &#92;sigma^{2} (I_{1} +  X_{1} ( X_{2}^{T} X_{2}+&#92;Lambda)^{-1} X_{1}^{T}) &#92;right)  " class="latex" />
</p>

which looks different from the distribution obtained through method 1. The expression for the variance is a lot neater. They are in fact identical by the Woodbury identity.

## Comparison

### Mean (Submitted by Michelle Leigh)

<p align="center">
  <img src="//s0.wp.com/latex.php?latex=%5Cleft%5B%5CLambda%2B++X_%7B2%7D%5ETI_%7B2%7D+X_%7B2%7D%5Cright%5D%5E%7B-1%7D+X_%7B2%7D%5ET%5C%5C++%3D%5C%7B%5CLambda%5E%7B-1%7D-%5CLambda%5E%7B-1%7D+X_%7B2%7D%5ET%5Cleft%5BI_%7B2%7D%2B++X_%7B2%7D%5CLambda%5E%7B-1%7D+X_%7B2%7D%5ET%5Cright%5D%5E%7B-1%7D+X_%7B2%7D%5CLambda%5E%7B-1%7D%5C%7D+X_%7B2%7D%5ET%5C%5C++%3D%5CLambda%5E%7B-1%7D+X_%7B2%7D%5ET%5Cleft%5BI_%7B2%7D%2B++X_%7B2%7D%5CLambda%5E%7B-1%7D+X_%7B2%7D%5ET%5Cright%5D%5E%7B-1%7D%5Cleft%5BI_%7B2%7D%2B++X_%7B2%7D%5CLambda%5E%7B-1%7D+X_%7B2%7D%5ET%5Cright%5D-%5CLambda%5E%7B-1%7D+X_%7B2%7D%5ET%5Cleft%5BI_%7B2%7D%2B++X_%7B2%7D%5CLambda%5E%7B-1%7D+X_%7B2%7D%5ET%5Cright%5D%5E%7B-1%7D+X_%7B2%7D%5CLambda%5E%7B-1%7D+X_%7B2%7D%5ET%5C%5C++%3D%5CLambda%5E%7B-1%7D+X_%7B2%7D%5ET%5Cleft%5BI_%7B2%7D%2B++X_%7B2%7D%5CLambda%5E%7B-1%7D+X_%7B2%7D%5ET%5Cright%5D%5E%7B-1%7DI_%7B2%7D&#038;bg=ffffff&#038;fg=000&#038;s=0" alt="&#92;left[&#92;Lambda+  X_{2}^TI_{2} X_{2}&#92;right]^{-1} X_{2}^T&#92;&#92;  =&#92;{&#92;Lambda^{-1}-&#92;Lambda^{-1} X_{2}^T&#92;left[I_{2}+  X_{2}&#92;Lambda^{-1} X_{2}^T&#92;right]^{-1} X_{2}&#92;Lambda^{-1}&#92;} X_{2}^T&#92;&#92;  =&#92;Lambda^{-1} X_{2}^T&#92;left[I_{2}+  X_{2}&#92;Lambda^{-1} X_{2}^T&#92;right]^{-1}&#92;left[I_{2}+  X_{2}&#92;Lambda^{-1} X_{2}^T&#92;right]-&#92;Lambda^{-1} X_{2}^T&#92;left[I_{2}+  X_{2}&#92;Lambda^{-1} X_{2}^T&#92;right]^{-1} X_{2}&#92;Lambda^{-1} X_{2}^T&#92;&#92;  =&#92;Lambda^{-1} X_{2}^T&#92;left[I_{2}+  X_{2}&#92;Lambda^{-1} X_{2}^T&#92;right]^{-1}I_{2}" title="&#92;left[&#92;Lambda+  X_{2}^TI_{2} X_{2}&#92;right]^{-1} X_{2}^T&#92;&#92;  =&#92;{&#92;Lambda^{-1}-&#92;Lambda^{-1} X_{2}^T&#92;left[I_{2}+  X_{2}&#92;Lambda^{-1} X_{2}^T&#92;right]^{-1} X_{2}&#92;Lambda^{-1}&#92;} X_{2}^T&#92;&#92;  =&#92;Lambda^{-1} X_{2}^T&#92;left[I_{2}+  X_{2}&#92;Lambda^{-1} X_{2}^T&#92;right]^{-1}&#92;left[I_{2}+  X_{2}&#92;Lambda^{-1} X_{2}^T&#92;right]-&#92;Lambda^{-1} X_{2}^T&#92;left[I_{2}+  X_{2}&#92;Lambda^{-1} X_{2}^T&#92;right]^{-1} X_{2}&#92;Lambda^{-1} X_{2}^T&#92;&#92;  =&#92;Lambda^{-1} X_{2}^T&#92;left[I_{2}+  X_{2}&#92;Lambda^{-1} X_{2}^T&#92;right]^{-1}I_{2}" class="latex" />
</p>

So mean1=mean2.

### Variance

By the Woodbury Identity it follows that

<p align="center">
  <img src="//s0.wp.com/latex.php?latex=%5CLambda%5E%7B-1%7D+-+%5CLambda%5E%7B-1%7D+X_%7B2%7D%5E%7BT%7D+%5Cleft%5B+I_%7B2%7D+%2B++X_%7B2%7D%5CLambda%5E%7B-1%7D+X_%7B2%7D%5E%7BT%7D+%5Cright%5D%5E%7B-1%7D++X_%7B2%7D%5CLambda%5E%7B-1%7D+%3D+%28+X_%7B2%7D%5E%7BT%7DI_%7B2%7D+X_%7B2%7D%2B%5CLambda%29%5E%7B-1%7D.++&#038;bg=ffffff&#038;fg=000&#038;s=0" alt="&#92;Lambda^{-1} - &#92;Lambda^{-1} X_{2}^{T} &#92;left[ I_{2} +  X_{2}&#92;Lambda^{-1} X_{2}^{T} &#92;right]^{-1}  X_{2}&#92;Lambda^{-1} = ( X_{2}^{T}I_{2} X_{2}+&#92;Lambda)^{-1}.  " title="&#92;Lambda^{-1} - &#92;Lambda^{-1} X_{2}^{T} &#92;left[ I_{2} +  X_{2}&#92;Lambda^{-1} X_{2}^{T} &#92;right]^{-1}  X_{2}&#92;Lambda^{-1} = ( X_{2}^{T}I_{2} X_{2}+&#92;Lambda)^{-1}.  " class="latex" />
</p>

Therefore

<p align="center">
  <img src="//s0.wp.com/latex.php?latex=X_%7B1%7D%5CLambda%5E%7B-1%7D+X_%7B1%7D%5E%7BT%7D-+X_%7B1%7D%5CLambda%5E%7B-1%7D+X_%7B2%7D%5E%7BT%7D+%5Cleft%5B+I_%7B2%7D%2B+X_%7B2%7D%5CLambda%5E%7B-1%7D+X_%7B2%7D%5E%7BT%7D+%5Cright%5D%5E%7B-1%7D++X_%7B2%7D%5CLambda%5E%7B-1%7D+X_%7B1%7D%5E%7BT%7D%3D%7B+X_%7B1%7D+%28+X_%7B2%7D%5E%7BT%7D+X_%7B2%7D%2B%5CLambda%29%5E%7B-1%7D+X_%7B1%7D%5E%7BT%7D%7D%5C%5C++&#038;bg=ffffff&#038;fg=000&#038;s=0" alt="X_{1}&#92;Lambda^{-1} X_{1}^{T}- X_{1}&#92;Lambda^{-1} X_{2}^{T} &#92;left[ I_{2}+ X_{2}&#92;Lambda^{-1} X_{2}^{T} &#92;right]^{-1}  X_{2}&#92;Lambda^{-1} X_{1}^{T}={ X_{1} ( X_{2}^{T} X_{2}+&#92;Lambda)^{-1} X_{1}^{T}}&#92;&#92;  " title="X_{1}&#92;Lambda^{-1} X_{1}^{T}- X_{1}&#92;Lambda^{-1} X_{2}^{T} &#92;left[ I_{2}+ X_{2}&#92;Lambda^{-1} X_{2}^{T} &#92;right]^{-1}  X_{2}&#92;Lambda^{-1} X_{1}^{T}={ X_{1} ( X_{2}^{T} X_{2}+&#92;Lambda)^{-1} X_{1}^{T}}&#92;&#92;  " class="latex" />
</p>

and so variance1=variance2. The trick is recognizing the form of the formulas at the top of the page, then one can write the variance as a much neater expression.