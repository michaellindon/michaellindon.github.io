---
id: 91
title: GNU Wget Tutorial
date: 2012-07-06T14:15:56+00:00
author: admin
layout: post
guid: http://www.lindonslog.com/?p=91
permalink: /linux-unix/gnu-wget-tutorial/
categories:
  - Linux/Unix
---
As a student, you may find yourself wanting to download lots of lecture slides and other materials off a module homepage, which can become quite an arduous task. Thankfully, GNU created Wget which is already on most linux machines. It is best demonstrated by example:

<pre>wget -r -l5 -np -k -nH --cut-dirs=5 --load-cookies cookies.txt http://www2.warwick.ac.uk/fac/sci/physics/current/teach/module_home/px421/
</pre>

-r
  
Means wget acts _recursively_ i.e. it follows links found on the current page (much like a search engine spider.

-l
  
Specifies the _depth_, which means how many of these links it can follow. If you imagine all the links on the current page forming branches away from it, then the links on those pages forming branches away from those, then -l5 sets the maximum branch distance away from the current page.

-np
  
_No Parent_, means wget will only progress down the directory tree i.e. it will not work its way back into http://www2.warwick.ac.uk/fac/sci/physics/current/teach/module_home/

-k
  
_Convert Hyperlinks_. When wget downloads a page, say index.html, there will be links on that page just like viewed in your browser but -k will convert them to local links, so that you can navigate your way through the pages on your local machine.

-nH
  
_No host directories_. Basically wget would otherwise create a folder named &#8220;http://www2.warwick.ac.uk/&#8221; and all the downloaded stuff would get stored in there, which is normally undesirable.

&#8211;cut-dirs=5
  
Otherwise wget would create 5 directories
  
`<br />
http://www2.warwick.ac.uk/fac/<br />
sci/<br />
physics/<br />
current/<br />
teach/<br />
module_home/<br />
` 
  
in a directory tree which you don&#8217;t want to have to click through&#8230;

_&#8211;load-cookies_
  
Normally content is restricted and you need to login, so you need to supply wget with some cookies. If you are a firefox user then there is an extension called &#8216;cookie exporter&#8217;, which you can use to output your cookies to a file called cookies.txt.

That&#8217;s it!