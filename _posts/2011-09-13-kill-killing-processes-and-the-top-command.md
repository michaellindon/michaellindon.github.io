---
id: 29
title: 'Kill &#8211; Killing Processes and the top Command'
date: 2011-09-13T19:15:36+00:00
author: admin
layout: post
guid: http://www.lindonslog.com/?p=29
permalink: /linux-unix/kill-killing-processes-and-the-top-command/
categories:
  - Linux/Unix
---
Sooner or later you will want to kill a process, whether it be some code executed in the background that is taking too long or simply a program that is misbehaving.

If the code has been executed by the user in the background, then one can use the **jobs** command with the **kill** command:

<pre class="brush: bash; title: ; notranslate" title="">michael@michael-laptop:~$ jobs
[1]+  Running                 ./test &
michael@michael-laptop:~$ kill -9 %1
michael@michael-laptop:~$ 
[1]+  Killed                  ./test
</pre>

The second term specifies which signal to send, as specified in <signal.h>. As an exemplary aside, `CTRL + C` sends the `SIGINT` interrupt signal to the program and `CTRL + Z` sends the `SIGSTOP` pause signal. The -9 sends the `SIGKILL` signal, which is slightly more forceful than the default `SIGTERM` which is obtained by -15 or omitting this field. `SIGKILL` forces the program to end immediately whereas `SIGTERM` can be intercepted or ignored by the program. The latter is the gentler approach as it can allow the program to clean itself up before finishing. ?using the latter would have produced this line of code `[1]+  Terminated                  ./test` as opposed to `Killed`

## top command

Alternatively, you can use the `top` command to display processes in order of CPU demand. N.B. It is called &#8220;top&#8221; because only the top most demanding processes are shown. 

<pre class="brush: r; title: ; notranslate" title="">top - 12:26:34 up 23 days, 12:39,  2 users,  load average: 8.00, 8.01, 8.05
Tasks: 152 total,  10 running, 142 sleeping,   0 stopped,   0 zombie
Cpu(s): 64.4%us,  0.2%sy,  0.0%ni, 35.4%id,  0.0%wa,  0.0%hi,  0.1%si,  0.0%st
Mem:  24732896k total, 24581112k used,   151784k free,   531980k buffers
Swap:  3999676k total,     9080k used,  3990596k free, 21636732k cached

  PID  USER      PR  NI  VIRT  RES  SHR S %CPU %MEM    TIME+  COMMAND            
 2447  laemmel   20   0  122m 111m  972 R  100  0.5   9880:20 BD-concn-semifl    
 2486  lindon    20   0 58676  40m 1492 R  100  0.2   9875:24 hing                
 4053  laemmel   20   0  122m 111m  972 R  100  0.5   9672:33 BD-concn-semifl    
 7782  sturm     20   0 19496 7100 1220 R  100  0.0   1520:41 tfpulling          
 20031 sturm     20   0 23384  10m 1220 R  100  0.0   5330:51 tfpulling          
 26339 sturm     20   0 22616  10m 1220 R  100  0.0   4374:08 tfpulling          
 30841 glaser    20   0 44.5g 473m 344m R  100  2.0  10974:30 hoomd              
 1093  sturm     20   0 58764  45m 1184 R  100  0.2  10176:04 tfpulling          
</pre>

Within top, you can press k (for kill) and then enter the PID (process ID) of the process you would like to kill. To exit top, press q. If you know the PID, which you can get from top, you can use the `kill` command in a regular terminal by entering  `kill -9 PID`