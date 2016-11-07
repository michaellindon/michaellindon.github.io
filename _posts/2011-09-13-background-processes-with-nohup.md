---
id: 22
title: Background Processes and the jobs Command
date: 2011-09-13T18:22:13+00:00
author: admin
layout: post
guid: http://www.lindonslog.com/?p=22
permalink: /linux-unix/background-processes-with-nohup/
categories:
  - Linux/Unix
---
If your code takes a long time to run after execution, you may consider running it in the background. Consider the executable &#8220;test&#8221;, I run it as follows

**1. Running Processes in the Background**

<pre>michael@michael-laptop:~$ ./test</pre>

while it is running, you do much with the terminal. You can abort it with  `CTRL + C` or you can pause it with `CTRL + Z`

<pre>michael@michael-laptop:~$ ./test
^Z
[1]+  Stopped                 ./test
</pre>

The last line is exactly what you would get if you typed the `job` command. You can now send this process to run in the background by typing `bg`

<pre>michael@michael-laptop:~$ bg
[1]+ ./test &
michael@michael-laptop:~$ 
</pre>

Typing `jobs` again will now show you that the process is running in the background

<pre>michael@michael-laptop:~$ jobs
[1]+  Running                 ./test &
michael@michael-laptop:~$
</pre>

When the process finished, a confirmation will be displayed in the terminal like this

<pre>[1]+  Done                    ./test
michael@michael-laptop:~$ 
</pre>

In addition, you can bring processes back to the foreground by entering `fg<br />
job_id`, where the job id is found from the `jobs` command (it is the number before the process e.g. 1 in this example).

You may now also wish to know how to [kill background processes](http://www.lindonslog.com/2011/09/13/kill-killing-processes-and-the-top-command/)