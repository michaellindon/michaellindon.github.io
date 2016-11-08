---
id: 218
title: Multivariate Normal Conditional Distribution
date: 2013-05-15T23:19:37+00:00
author: admin
layout: post
guid: http://www.lindonslog.com/?p=218
permalink: /mathematics/multivariate-normal-conditional-distribution/
categories:
  - Mathematics
---
Previously I made a post about [diagonalizing a positive definite symmetric matrix](http://www.lindonslog.com/mathematics/diagonalize-positive-definite-symmetric-matrix-schur-complement-ldu-decomposition/ "Diagonalize a Positive-Definite Symmetric Matrix using the Schur Complement and LDU Decomposition") . One of the applications of this within statistics is to diagonalize the variance matrix of a multivariate normal in order to derive conditional distributions. Let

<img src="//s0.wp.com/latex.php?latex=Y%5Csim+N_%7Bp%7D%28m%2CV%29++&#038;bg=ffffff&#038;fg=000&#038;s=0" alt="Y&#92;sim N_{p}(m,V)  " title="Y&#92;sim N_{p}(m,V)  " class="latex" />
  
Consider multiplying Y by the following matrix
  
<img src="//s0.wp.com/latex.php?latex=%5Cbegin%7Bpmatrix%7D++++++++++1+%26+-V_%7B12%7DV_%7B22%7D%5E%7B-1%7D%5C%5C++++++++++0+%26+1%5C%5C++%5Cend%7Bpmatrix%7DY+%5Csim+N_%7Bp%7D%5Cleft%28+%5Cbegin%7Bpmatrix%7D+m_%7B1%7D+-+V_%7B12%7DV_%7B22%7D%5E%7B-1%7Dm_%7B2%7D+%5C%5C+m_%7B2%7D%5C%5C+%5Cend%7Bpmatrix%7D%2C%5Cbegin%7Bpmatrix%7D++++++++++V_%7B11%7D-V_%7B12%7DV_%7B22%7D%5E%7B-1%7DV_%7B21%7D+%26+0+%5C%5C++++++++++0+%26+V_%7B22%7D%5C%5C++%5Cend%7Bpmatrix%7D%5Cright%29++&#038;bg=ffffff&#038;fg=000&#038;s=0" alt="&#92;begin{pmatrix}          1 & -V_{12}V_{22}^{-1}&#92;&#92;          0 & 1&#92;&#92;  &#92;end{pmatrix}Y &#92;sim N_{p}&#92;left( &#92;begin{pmatrix} m_{1} - V_{12}V_{22}^{-1}m_{2} &#92;&#92; m_{2}&#92;&#92; &#92;end{pmatrix},&#92;begin{pmatrix}          V_{11}-V_{12}V_{22}^{-1}V_{21} & 0 &#92;&#92;          0 & V_{22}&#92;&#92;  &#92;end{pmatrix}&#92;right)  " title="&#92;begin{pmatrix}          1 & -V_{12}V_{22}^{-1}&#92;&#92;          0 & 1&#92;&#92;  &#92;end{pmatrix}Y &#92;sim N_{p}&#92;left( &#92;begin{pmatrix} m_{1} - V_{12}V_{22}^{-1}m_{2} &#92;&#92; m_{2}&#92;&#92; &#92;end{pmatrix},&#92;begin{pmatrix}          V_{11}-V_{12}V_{22}^{-1}V_{21} & 0 &#92;&#92;          0 & V_{22}&#92;&#92;  &#92;end{pmatrix}&#92;right)  " class="latex" />
  
i.e.
  
<img src="//s0.wp.com/latex.php?latex=%5Cbegin%7Bpmatrix%7D++++++++++Y_%7B1%7D-V_%7B12%7DV_%7B22%7D%5E%7B-1%7DY_%7B2%7D+%5C%5C++++++++++Y_%7B2%7D%5C%5C++%5Cend%7Bpmatrix%7D%5Csim++N%5Cleft%28++%5Cbegin%7Bpmatrix%7D+m_%7B1%7D+-+V_%7B12%7DV_%7B22%7D%5E%7B-1%7Dm_%7B2%7D+%5C%5C+m_%7B2%7D%5C%5C+%5Cend%7Bpmatrix%7D%2C%5Cbegin%7Bpmatrix%7D++++++++++V_%7B11%7D-V_%7B12%7DV_%7B22%7D%5E%7B-1%7DV_%7B21%7D+%26+0+%5C%5C++++++++++0+%26+V_%7B22%7D%5Cend%7Bpmatrix%7D+%5Cright%29%5C%5C++&#038;bg=ffffff&#038;fg=000&#038;s=0" alt="&#92;begin{pmatrix}          Y_{1}-V_{12}V_{22}^{-1}Y_{2} &#92;&#92;          Y_{2}&#92;&#92;  &#92;end{pmatrix}&#92;sim  N&#92;left(  &#92;begin{pmatrix} m_{1} - V_{12}V_{22}^{-1}m_{2} &#92;&#92; m_{2}&#92;&#92; &#92;end{pmatrix},&#92;begin{pmatrix}          V_{11}-V_{12}V_{22}^{-1}V_{21} & 0 &#92;&#92;          0 & V_{22}&#92;end{pmatrix} &#92;right)&#92;&#92;  " title="&#92;begin{pmatrix}          Y_{1}-V_{12}V_{22}^{-1}Y_{2} &#92;&#92;          Y_{2}&#92;&#92;  &#92;end{pmatrix}&#92;sim  N&#92;left(  &#92;begin{pmatrix} m_{1} - V_{12}V_{22}^{-1}m_{2} &#92;&#92; m_{2}&#92;&#92; &#92;end{pmatrix},&#92;begin{pmatrix}          V_{11}-V_{12}V_{22}^{-1}V_{21} & 0 &#92;&#92;          0 & V_{22}&#92;end{pmatrix} &#92;right)&#92;&#92;  " class="latex" />
  
The covariance matrix has now been diagonalized. This is useful because zero covariance implies independence for normally distributed random variables and so it follows that

<img src="//s0.wp.com/latex.php?latex=Y_%7B1%7D-V_%7B12%7DV_%7B22%7D%5E%7B-1%7DY_%7B2%7D+%5Csim+N%28+m_%7B1%7D+-+V_%7B12%7DV_%7B22%7D%5E%7B-1%7Dm_%7B2%7D+%2CV_%7B11%7D-V_%7B12%7DV_%7B22%7D%5E%7B-1%7DV_%7B21%7D%29++&#038;bg=ffffff&#038;fg=000&#038;s=0" alt="Y_{1}-V_{12}V_{22}^{-1}Y_{2} &#92;sim N( m_{1} - V_{12}V_{22}^{-1}m_{2} ,V_{11}-V_{12}V_{22}^{-1}V_{21})  " title="Y_{1}-V_{12}V_{22}^{-1}Y_{2} &#92;sim N( m_{1} - V_{12}V_{22}^{-1}m_{2} ,V_{11}-V_{12}V_{22}^{-1}V_{21})  " class="latex" />
  
Conditiong on Y2 it follows that the conditional distribution is
  
<img src="//s0.wp.com/latex.php?latex=Y_%7B1%7D%7CY_%7B2%7D+%5Csim+N%28+m_%7B1%7D+%2B+V_%7B12%7DV_%7B22%7D%5E%7B-1%7D%28Y_%7B2%7D-m_%7B2%7D+%29%2CV_%7B11%7D-V_%7B12%7DV_%7B22%7D%5E%7B-1%7DV_%7B21%7D%29++&#038;bg=ffffff&#038;fg=000&#038;s=0" alt="Y_{1}|Y_{2} &#92;sim N( m_{1} + V_{12}V_{22}^{-1}(Y_{2}-m_{2} ),V_{11}-V_{12}V_{22}^{-1}V_{21})  " title="Y_{1}|Y_{2} &#92;sim N( m_{1} + V_{12}V_{22}^{-1}(Y_{2}-m_{2} ),V_{11}-V_{12}V_{22}^{-1}V_{21})  " class="latex" />
  
Notice how the variance of the conditional normal distribution is the marginal variance of Y1 minus something. That is to say the variance of Y1 is reduced given the knowledge of Y2. The variance can be recognized as the Schur complement of the covariance matrix with respect to V22. A similar treatment yields
  
<img src="//s0.wp.com/latex.php?latex=Y_%7B2%7D%7CY_%7B1%7D+%5Csim+N%28+m_%7B2%7D+%2B+V_%7B21%7DV_%7B11%7D%5E%7B-1%7D%28Y_%7B1%7D-m_%7B1%7D+%29%2CV_%7B22%7D-V_%7B21%7DV_%7B11%7D%5E%7B-1%7DV_%7B12%7D%29++&#038;bg=ffffff&#038;fg=000&#038;s=0" alt="Y_{2}|Y_{1} &#92;sim N( m_{2} + V_{21}V_{11}^{-1}(Y_{1}-m_{1} ),V_{22}-V_{21}V_{11}^{-1}V_{12})  " title="Y_{2}|Y_{1} &#92;sim N( m_{2} + V_{21}V_{11}^{-1}(Y_{1}-m_{1} ),V_{22}-V_{21}V_{11}^{-1}V_{12})  " class="latex" />