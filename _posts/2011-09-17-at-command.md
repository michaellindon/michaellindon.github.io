---
id: 39
title: at command
date: 2011-09-17T16:01:06+00:00
author: admin
layout: post
guid: http://www.lindonslog.com/?p=39
permalink: /linux-unix/at-command/
categories:
  - Linux/Unix
---
Scheduling a process to run automatically at a certain date and time can be quite useful. This is achieved with the at command. The at command reads a series of commands from the standard input and lumps them together into one single at-job to be executed at some point in the future.

`Syntax:   at [-V] [-q queue] [-f file] [-mkdv] [-t time]`

You can `man at` to find out about all the options. 

Here is an example:

<pre>michael@michael-laptop:~$ at -mv 4:44 sep 21
Wed Sep 21 04:44:00 2011

warning: commands will be executed using /bin/sh
at> g++ main.cpp -o test
at> nohup ./test &
at> &lt;EOT>
job 13 at Wed Sep 21 04:44:00 2011
michael@michael-laptop:~$ 
</pre>

-m option mails the user when the at-job has been executed. The -v option simply produces the first line i.e. displays when the job will be executed. One does not write <EOT>, instead, this is achieved by entering `CTRL+D` here on a new line.

To see whats at-jobs have been scheduled, enter **atq** (alternatively **at -l**

<pre>michael@michael-laptop:~$ atq
13	Wed Sep 21 04:44:00 2011 a michael
michael@michael-laptop:~$ at -l
13	Wed Sep 21 04:44:00 2011 a michael
michael@michael-laptop:~$ 
</pre>

To remove this job, enter **atrm** and the ID:

<pre>michael@michael-laptop:~$ atrm 13
michael@michael-laptop:~$ atq
michael@michael-laptop:~$
</pre>

The at-job inherits the environment of the terminal that schedules it and hence contains the same working directory and environment variables. If at some point in the future you forget what commands an at-job contains, you can view them with `at -c job`. This lists the environment variables of the at-job and contains the commands at the bottom.