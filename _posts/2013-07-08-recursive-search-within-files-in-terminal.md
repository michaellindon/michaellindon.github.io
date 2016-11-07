---
id: 560
title: Recursive Search Within Files in Terminal
date: 2013-07-08T18:54:54+00:00
author: admin
layout: post
guid: http://www.lindonslog.com/?p=560
permalink: /linux-unix/recursive-search-within-files-in-terminal/
categories:
  - Linux/Unix
---
When I inherit load of code from people, often I like to see what files call certain functions. A way to do this is to use grep recursively by using the -R option. Say I want to find all the files in which &#8216;rgamma&#8217; appears.

<pre class="brush: bash; title: ; notranslate" title="">msl33@hotel Documents]$ grep -R 'rgamma' *
601/lab/4/script4.R:lamda=rgamma(n,shape=5,rate=6)
601/lab/9/9.R:#  X[i,]=rgamma
601/lab/9/9.R:#   X=matrix(rgamma(length(a)*k,a,1),k,length(a),byrow=T)
601/lab/9/9.R:# theta=rgamma(10000,7,1000)
msl33@hotel Documents]$
</pre>

This will look recursively within files to find appearances of &#8216;rgamma&#8217;. As it turns out rgamma appears once in &#8220;script4.R&#8221; and thrice in &#8220;9.R&#8221;.