---
id: 97
title: How to crop a pdf when using the ps2pdf converter via -dEPSCrop
date: 2013-01-19T14:48:49+00:00
author: admin
layout: post
guid: http://www.lindonslog.com/?p=97
permalink: /linux-unix/ps2pdf-how-to-crop-the-white-border/
categories:
  - Linux/Unix
---
Often I find myself converting eps or ps to pdf files for inclusion in a latex document using the ps2pdf converter. The problem is that often when using ps2pdf I get a large white border around the figure of interest in my pdf file which was not included in the eps file. Obviously this is annoying when trying to include the graphic in a latex file surrounded by text. The solution comes with using the -dEPSCrop option. 

The following flags will remedy the problem.

<pre class="brush: bash; title: ; notranslate" title="">lindon$ ps2pdf -dPDFSETTINGS=/prepress -dEPSCrop test.eps
</pre>

Now the pdf doesn&#8217;t included the unnecessary white space border.