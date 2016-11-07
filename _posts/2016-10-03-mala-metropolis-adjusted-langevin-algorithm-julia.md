---
title: Metropolis Adjusted Langevin Algorithm in Julia
date: 2016-10-05T13:07:09+00:00
author: admin
layout: single
author_profile: true 
categories:
  - julia
tags:
  - julia
  - mcmc
---


# Generic Julia Implementation
The Metropolis Adjusted Langevin Algorithm is a more general purpose markov chain monte carlo algorithm for sampling from a differentiable target density. Here is an implementation in Julia using an optional preconditioning matrix. The function arguments are a function to evaluate the target logdensity, a function to evaluate the gradient, a step size h, a preconditioning matrix M (use an Identity matrix if preconditioning is not desired), number of iterations and an initial parameter value.


```julia
using Distributions

function mala(logdensity,gradient,h,M,niter,θinit)                                                                                                                                                                 
        function gradientStep(θ,t)                                                                                                                                                                                 
                θ-t*M*gradient(θ)                                                                                                                                                                                  
        end                                                                                                                                                                                                        
        θtrace=Array{Float64}(length(θinit),niter)                                                                                                                                                                 
        θ=θinit                                                                                                                                                                                                    
        θtrace[:,1]=θinit                                                                                                                                                                                          
        for i=2:niter                                                                                                                                                                                              
                θold=θ                                                                                                                                                                                             
                θ=rand(MvNormal(gradientStep(θ,0.5*h),h*M))                                                                                                                                                        
                d=logdensity(θ) - logdensity(θold) + logpdf(MvNormal(gradientStep(θ,0.5*h),h*M),θold) - logpdf(MvNormal(gradientStep(θold,0.5*h),h*M),θ)                                                           
                if(!(log(rand(Uniform(0,1)))<d))                                                                                                                                                                   
                        θ=θold                                                                                                                                                                                     
                end                                                                                                                                                                                                
                θtrace[:,i]=θ                                                                                                                                                                                      
        end                                                                                                                                                                                                        
        θtrace                                                                                                                                                                                                     
end  

```







## Bivariate Normal Example
Let's illustrate the above code by using it to sample a bivariate normal


```julia
ρ²=0.8                                                                                                                                                                                                             
Σ=[1 ρ²;ρ² 1]                                                                                                                                                                                                      
                                                                                                                                                                                                                   
function logdensity(θ)                                                                                                                                                                                             
        logpdf(MvNormal(Σ),θ)                                                                                                                                                                                      
end                                                                                                                                                                                                                
                                                                                                                                                                                                                   
function gradient(θ)                                                                                                                                                                                               
        Σ\θ                                                                                                                                                                                                        
end  

niter=1000                                                                                                                                                                                                         
h=1/eigs(inv(Σ),nev=1)[1][1]                                                                                                                                                                                       
draws=mala(logdensity,gradient,h,eye(2),niter,[5,50]);   #No preconditioning                                                                                                                                                                                                                                                                                                    
pdraws=mala(logdensity,gradient,h,Σ,niter,[5,50]);       #With Preconditioning                                                                                                                                                    
```



## Visualization
pdraws uses the covariance matrix Σ as the preconditioning matrix, whereas the first uses an identity matrix, resulting in the original MALA algorithm. The traceplot of draws from MALA and preconditioned MALA are shown in blue and green respectively…


```julia
using PyPlot
function logdensity2d(x,y)                                                                                                                                                                                         
        logdensity([x,y])                                                                                                                                                                                          
end                                                                                                                                                                                                                
x = -30:0.1:30                                                                                                                                                                                                     
y = -30:0.1:50                                                                                                                                                                                                     
X = repmat(x',length(y),1)                                                                                                                                                                                         
Y = repmat(y,1,length(x))                                                                                                                                                                                          
Z = map(logdensity2d,Y,X)                                                                                                                                                                                          
p1 = contour(x,y,Z,200)                                                                                                                                                                                            
plot(vec(draws[1,:]),vec(draws[2,:]))                                                                                                                                                                              
plot(vec(pdraws[1,:]),vec(pdraws[2,:])) 
```


![png](../../assets/images/mala.png)



## Effective Sample Sizes in R

We can use the julia “RCall” package to switch over to R and use the coda library to evaluate the minimum effective sample size for both of these MCMC algorithms.

julia> library(coda)

R> library(coda)

R> min(effectiveSize($(draws')))
[1] 22.02418

R> min(effectiveSize($(pdraws')))
[1] 50.85163

I didn’t tune the step size h in this example at all (you should).


