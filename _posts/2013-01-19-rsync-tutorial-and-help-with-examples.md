---
id: 103
title: rsync Tutorial and Help with Examples
date: 2013-01-19T16:11:57+00:00
author: admin
layout: post
guid: http://www.lindonslog.com/?p=103
permalink: /linux-unix/rsync-tutorial-and-help-with-examples/
categories:
  - Linux/Unix
---
rsync is what people used to use before Dropbox. It is used to sync remote folders with local ones. Say for example tha I have some work on my office computer in folder &#8216;foo&#8217;. It is the weekend and I need to work on this project but I don&#8217;t want to go into the office. rsync can be used to pull the folder from the remote (office) machine onto my local (home) computer. Then, when I am done with the changes, rsync can push all the modified files back to the office computer. rsync has some really great features so that it will only transfer the files which have been modified. 

<pre class="brush: bash; title: ; notranslate" title="">Home$ rsync -avzu office:.matlab/myfunctions/figs/ .
receiving incremental file list
./
1_a_0_neg.eps
1_a_0_pos.eps
1_a_10_neg.eps
1_a_10_pos.eps

sent 90 bytes  received 12464 bytes  2789.78 bytes/sec
total size is 36238  speedup is 2.89
</pre>

**-a &#8211;archive**
  
Stands for archive mode. Basically this means that the structure from the office machine is kept on the local machine (i.e. same symbolic links etc.)

**-v, &#8211;verbose**
  
Increases verbosity

**-z, &#8211;compress**
  
Compresses the files for transfer purposes and then uncompresses them on the local machine. Really great if transferring massive data.

**-u, &#8211;update**
  
This means that if the destination folder has files with a timestamp which is newer than the files in the source folder, then the files in the destination folder will not be overwritten with the older source ones.

These are just a few most common arguments. Lastly, a good argument to remember

**-n, &#8211;dry-run**
  
This won&#8217;t actually do any transfer of data, it merely shows you what rsync WOULD do if you removed this argument. A good choice if you want to be careful.