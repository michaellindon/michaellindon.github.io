---
id: 306
title: Polynomial Regression Linear Model in R
date: 2013-06-14T16:26:01+00:00
author: admin
layout: post
guid: http://www.lindonslog.com/?p=306
permalink: /mathematics/polynomial-regression-linear-model-in-r/
categories:
  - Mathematics
---
This post is just a brief example of how linear model theory can be used to perform polynomial regression. Consider getting some bivariate data that looks like this [(downloadable here)](http://www.lindonslog.com/example_code/polydata.dat)
  
[<img src="http://www.lindonslog.com/wp-content/uploads/2013/06/polyreg.bmp" alt="Polynomial Regression" title="Polynomial Regression" class="alignnone size-medium wp-image-307" />](http://www.lindonslog.com/wp-content/uploads/2013/06/polyreg.bmp)

and suppose we wish to fit a 3rd degree polynomial to this data. We can write it as a linear model
  
<img src="//s0.wp.com/latex.php?latex=y_%7Bi%7D%3DB_%7B0%7D%2BB_%7B1%7Dx_%7Bi%7D%2BB_%7B2%7Dx_%7Bi%7D%5E%7B2%7D%2BB_%7B3%7Dx_%7Bi%7D%5E%7B3%7D+%2B+%5Cepsilon_%7Bi%7D%5C%5C++Y%3DXB+%2B+%5Cepsilon%2C++&#038;bg=ffffff&#038;fg=000&#038;s=0" alt="y_{i}=B_{0}+B_{1}x_{i}+B_{2}x_{i}^{2}+B_{3}x_{i}^{3} + &#92;epsilon_{i}&#92;&#92;  Y=XB + &#92;epsilon,  " title="y_{i}=B_{0}+B_{1}x_{i}+B_{2}x_{i}^{2}+B_{3}x_{i}^{3} + &#92;epsilon_{i}&#92;&#92;  Y=XB + &#92;epsilon,  " class="latex" />
  
which is a linear model because it is linear in the regression coefficients. The ordinary least squares estimator of the regression coefficients is then
  
<img src="//s0.wp.com/latex.php?latex=%5Chat%7BB%7D_%7BOLS%7D%3D%28X%5E%7B%27%7DX%29%5E%7B-1%7DX%27Y.++&#038;bg=ffffff&#038;fg=000&#038;s=0" alt="&#92;hat{B}_{OLS}=(X^{&#039;}X)^{-1}X&#039;Y.  " title="&#92;hat{B}_{OLS}=(X^{&#039;}X)^{-1}X&#039;Y.  " class="latex" />

This is implemented in the below R code

<pre class="brush: r; title: ; notranslate" title="">plot(x,y)
n=length(y)
int=rep(1,n)
X=cbind(x,x^2,x^3,int)
B=solve(t(X)%*%X)%*%t(X)%*%y
u=seq(-100,100,1)
v=B[4]+B[1]*u+B[2]*u^2+B[3]*u^3
lines(u,v,col="red")
</pre>

The array B contains the OLS estimates of the regression coefficients. The fitted polynomial is fitted below in red:
  
[<img src="http://www.lindonslog.com/wp-content/uploads/2013/06/polyregfit.bmp" alt="Fitted Polynomial" title="Fitted Polynomial" class="alignnone size-medium wp-image-313" />](http://www.lindonslog.com/wp-content/uploads/2013/06/polyregfit.bmp)

The extension to higher degree polynomials is simple, just add columns to the design matrix X.