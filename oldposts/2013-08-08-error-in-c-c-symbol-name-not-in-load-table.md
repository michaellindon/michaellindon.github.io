---
id: 822
title: 'Error in .C(&#8221; *** &#8220;) : C symbol name &#8221; *** &#8221; not in load table'
date: 2013-08-08T01:05:27+00:00
author: admin
layout: post
guid: http://www.lindonslog.com/?p=822
permalink: /programming/error-in-c-c-symbol-name-not-in-load-table/
categories:
  - Programming
---
If you are getting this error, as I was, then you are probably trying to write an extension for R in _C++_ and not C. I was just writing a function &#8220;[linsgp](https://github.com/michaellindon/linsgp)&#8221; in C++. See if the following scenario is familiar to you

<pre class="brush: r; title: ; notranslate" title="">&gt; dyn.load("main.so")
&gt; .C("linsgp")
Error in .C("linsgp") : C symbol name "linsgp" not in load table
</pre>

My C++ code looked like this

<pre class="brush: cpp; title: ; notranslate" title="">...
void linsgp(){
...
</pre>

What is missing is extern &#8220;C&#8221;, so it should look like this:

<pre class="brush: cpp; title: ; notranslate" title="">...
extern "C" void linsgp(){
...
</pre>

The reason is that C++ supports overloading of function names and so the compiler mangles the name with information about the arguments. C, however, does not support this and doesn&#8217;t mangle the name. Inserting extern &#8220;C&#8221; tells the compiler not to mangle the name such that the name used for linkage is C-compatible.