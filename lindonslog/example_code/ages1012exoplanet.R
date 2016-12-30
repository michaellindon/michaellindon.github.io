rm(list=ls())
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

jags <- jags.model(file.path('/home/grad/msl33/Dropbox/samsi/cp.bugs'),data = datalist, n.chains = 1, n.adapt = 500)
mcmc.samples=coda.samples(jags,c('b','tau1','tau2','amp','omega'),2000)
plot(mcmc.samples)

bmp('per_per.bpm')
par(mfrow=c(2,1))
plot(density(mcmc.samples[[1]][,1]),col="red", xlab="Flux", main='Posterior Density amp')
plot(density(mcmc.samples[[1]][,4]),col="red",xlab="Rads/Time", main='Posterior Density omega')
dev.off()

bmp('per_b.bpm')
par(mfrow=c(2,1))
plot(density(mcmc.samples[[1]][,2]),col="red", xlab="Flux", main='Posterior Density b_1')
plot(density(mcmc.samples[[1]][,3]),col="red",xlab="Time t", main='Posterior Density b_2')
dev.off()

bmp('per_tau.bpm')
par(mfrow=c(2,1))
plot(density(mcmc.samples[[1]][,5]),col="red",xlab="Time t", main='Posterior Density tau_1')
plot(density(mcmc.samples[[1]][,6]),col="red",xlab="Time t", main='Posterior Density tau_2')
dev.off()