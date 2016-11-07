---
id: 859
title: Easy 3-Minute Guide to Making apply() Parallel over Distributed Grids and Clusters in R
date: 2013-09-02T01:00:13+00:00
author: admin
layout: post
guid: http://www.lindonslog.com/?p=859
permalink: /programming/apply-parallel-distributed-grid-cluster-r/
image: /wp-content/uploads/2013/08/boscosmall.png
categories:
  - Programming
  - R
  - Statistics
---
Last week I attended a workshop on how to run highly parallel distributed jobs on the Open Science Grid (osg). There I met [Derek Weitzel](http://derekweitzel.blogspot.com/ "BoscoR") who has made an excellent contribution to advancing R as a high performance computing language by developing BoscoR. BoscoR greatly facilitates the use of the already existing package &#8220;GridR&#8221; by allowing the R user to use [Bosco](http://bosco.opensciencegrid.org/about/) to manage the submission of jobs. It seems no matter how many kinds of queue-submission system I become familiar with (torque,sge,condor), the current cluster I&#8217;m working on uses something foreign and so I have to relearn how to write a job submission file. One of the two major selling points of Bosco is that it allows the user to write one job submission file locally (based on HTCondor) and use it to submit jobs on various remote clusters all using different interfaces. The second major selling point is that Bosco will manage work sharing if you have access to more than one cluster, that is it will submit jobs to each cluster proportional to how unburdened that cluster is, which is great if you have access to 3 clusters. It means the users apply jobs will get through the queue as quickly as possible by cleverly distributing the work over all available clusters. Hopefully that will have convinced you that Bosco is worth having, now lets proceed with how to use it. I will illustrate the process by using Duke University&#8217;s cluster, the DSCR. There are three steps: 1) Installing Bosco 2) Installing GridR 3) Running a test job.
  
<br \>

## Installing Bosco

First go ahead and download <a href="http://bosco.opensciencegrid.org/download-form/?package=1.2/bosco_quickstart.tar.gz" rel="external nofollow" title="boscor download">Bosco</a>, the sign-up is only for the developers to get an idea of how many people are using it. Detailed install instructions can be found [here](https://confluence.grid.iu.edu/pages/viewpage.action?pageId=10944561) but I will also go through the steps.

<pre class="brush: bash; title: ; notranslate" title="">[lindon@laptop Downloads]$ tar xvzf ./bosco_quickstart.tar.gz
[lindon@laptop Downloads]$ ./bosco_quickstart
</pre>

The executable will then ask some questions:

Do you want to install Bosco? Select y/n and press [ENTER]: y
  
Type the cluster name and press [ENTER]: dscr-login-01.oit.duke.edu
  
When prompted &#8220;Type your name at dscr-login-01.oit.duke.edu (default YOUR_USER) and press [ENTER]: NetID
  
When prompted &#8220;Type the queue manager for login01.osgconnect.net (pbs, condor, lsf, sge, slurm) and press [ENTER]: sge
  
Then when prompted &#8220;NetID@dscr-login-01.oit.duke.edu&#8217;s password: XXXXXXX

For duke users, the HostName of the DCSR is dscr-login-01.oit.duke.edu. You login with your NetID and the queue submission system is the Sun Grid Engine, so type sge. If you already have <a href="http://www.lindonslog.com/linux-unix/ssh-keygen-keys/" title="Creating SSH Keys with ssh-keygen and ssh-copy-id" target="_blank">SSH-Keys</a> set up then I think the last question gets skipped. That takes care of the installation. You can now try submitting on the remote cluster locally from your laptop. Download this test [executable](http://www.lindonslog.com/example_code/short.sh) and this [submission file](http://www.lindonslog.com/example_code/bosco01.sub). Start Bosco and try submitting a job.

<pre class="brush: bash; title: ; notranslate" title="">[msl33@hotel ~/tutorial-bosco]$ source ~/bosco/bosco_setenv
[msl33@hotel ~/tutorial-bosco]$ bosco_start
BOSCO Started
[msl33@hotel ~/tutorial-bosco]$ condor_submit bosco01.sub 
Submitting job(s).
1 job(s) submitted to cluster 70.
[msl33@hotel ~/tutorial-bosco]$ condor_q


-- Submitter: hotel.stat.duke.edu : &lt;127.0.0.1:11000?sock=21707_cbb6_3&gt; : hotel.stat.duke.edu
 ID      OWNER            SUBMITTED     RUN_TIME ST PRI SIZE CMD               
  70.0   msl33           8/31 12:08   0+00:00:00 I  0   0.0  short.sh          

1 jobs; 0 completed, 0 removed, 1 idle, 0 running, 0 held, 0 suspended
</pre>

This is the result if all has worked well. Note that you need to start Bosco by the above two lines.

<br \>

## Installing GridR

The current version of GridR on CRAN is an older version doesn&#8217;t support job submission by bosco. It will when CRAN gets the latest version of GridR but until then you need to install GridR from source so download it <a href="https://github.com/osg-bosco/GridR/releases/download/v0.9.7/GridR_0.9.7.tar.gz" rel="external nofollow">here</a> and install it:

<pre class="brush: r; title: ; notranslate" title="">install.packages("~/Downloads/GridR_0.9.7.tar.gz", repos=NULL, type="source")
</pre>

<br \>

## Running a Parallel Apply on the Cluster

Consider a toy example which approximates pi by monte-carlo.

<pre class="brush: r; title: ; notranslate" title="">montecarloPi &lt;- function(trials, inst) {
  count = 0
  for(i in 1:trials) {
    if((runif(1,0,1)^2 + runif(1,0,1)^2)&lt;1) {
      count = count + 1
    }
  }
  return((count*4)/trials)
}
</pre>

One can now use grid.apply from the GridR package combined with Bosco to submit jobs on the remote cluster from within the users local R session.

<pre class="brush: plain; title: ; notranslate" title=""># load the GridR library
library("GridR")
grid.init(service="bosco.direct", localTmpDir="tmp")
# Send 10 instances of the montecarloPi
grid.apply("pi_estimate", montecarloPi, 10000000, c(1:10), batch=c(2))
</pre>

You can then see how your jobs are getting on by the &#8220;grid.printJobs()&#8221; command.
  
[<img src="http://www.lindonslog.com/wp-content/uploads/2013/08/parallel_grid_apply-1024x358.png" alt="parallel grid apply" width="640" height="223" class="aligncenter size-large wp-image-876" srcset="http://www.lindonslog.com/wp-content/uploads/2013/08/parallel_grid_apply-1024x358.png 1024w, http://www.lindonslog.com/wp-content/uploads/2013/08/parallel_grid_apply-300x105.png 300w, http://www.lindonslog.com/wp-content/uploads/2013/08/parallel_grid_apply.png 1325w" sizes="(max-width: 640px) 100vw, 640px" />](http://www.lindonslog.com/wp-content/uploads/2013/08/parallel_grid_apply.png)
  
When it completes, &#8220;pi\_estimate&#8221; will be a list object with 10 elements containing approximations to pi. Obviously, there is an overhead with submitting jobs and also a lag time while these jobs get through the queue. One must balance this overhead with the computational time required to complete a single iteration of the apply function. Bosco will create and submit a job for every iteration of the apply function. If each iteration does not take too long but there exists a great many of them to perform, one could consider blocking these operations into, say, 10 jobs so that the queue lag and submission overhead is negligible in comparison to the time taken to complete no\_apply\_iteraions/10 computations, which also saves creating a large number of jobs on the cluster which might aggravate other users. One can also add clusters to bosco using the &#8220;bosco\_cluster &#8211;add&#8221; command, so that jobs are submitted to whichever cluster has the most free cores available. All in all this is a great aid for those doing computationally intensive tasks and makes parallel work-sharing very easy indeed.