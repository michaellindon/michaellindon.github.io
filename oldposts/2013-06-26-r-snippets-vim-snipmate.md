---
id: 527
title: R snippets for vim-SnipMate
date: 2013-06-26T22:37:16+00:00
author: admin
layout: post
guid: http://www.lindonslog.com/?p=527
permalink: /linux-unix/r-snippets-vim-snipmate/
categories:
  - Linux/Unix
  - R
---
Vim is my editor of choice, reasonable so, whether it be for coding C++, LaTeX or even R. I&#8217;ve used RStudio, which even has a Vim-Mode, but I still prefer to use Vim. Vim has it&#8217;s own R plugin, namely Vim-R-plugin, but this post is about snippets. SnipMate is an awesome auto-completion plugin for Vim which is fully customizable. One simply writes a string, rnorm for example, and presses tab to autocomplete the code to rnorm(n=,mean=,sd=), where repeated press of tab cycles through the placeholders at the function parameters. The strings to recognize, referred to as snippets, are stored in a snippets file &#8220;languagetype.snippets&#8221; along with the corresponding code to auto-complete. These can be user defined for any language, not just R. It&#8217;s usually not necessary to write these snippets files yourself, as there are already existing snippets files within the vim community, including one for R. <a href="https://github.com/honza/vim-snippets/tree/master/snippets" rel="nofollow" title="Vim R snippets">Here</a> is a github repository containing snippets files for a great many languages, including R. Simply put this into your vim-SnipMate snippets directory. The last thing to do is to tell Vim to recognize an r filetype. If you open an R file and type,

<pre class="brush: bash; title: ; notranslate" title="">:set ft=r
</pre>

then this will tell Vim that the file is an R file. Obviously you don&#8217;t want to do this all the time, so to get Vim to automatically recognize &#8220;.r&#8221; and &#8220;.R&#8221; extensions as R files simply append your .vimrc file with:

<pre class="brush: bash; title: ; notranslate" title="">au BufNewFile,BufRead *.r set filetype=r
au BufNewFile,BufRead *.R set filetype=r
</pre>

## Writing Custom Snippets

This is an example of how to complete a for loop having written only &#8220;for&#8221;. Append the r.snippets file with the following code.

<pre class="brush: bash; title: ; notranslate" title="">snippet for
    for(${1:i} in ${2:begin}:${3:end}){
    ${4}
    }${5}
</pre>

This defines &#8220;for&#8221; as the snippet, the string after which we will want to press tab. The dollar signs define the place holders. &#8220;${x:text}&#8221; defines the x&#8217;th placeholder and the highlighted text which will be replaced.

<pre class="brush: r; title: ; notranslate" title="">for&lt;tab&gt;
</pre>

becomes

<pre class="brush: r; title: ; notranslate" title="">for(i in begin:end){

}
</pre>

.

Repeated pressing of tab cycles between i, begin, end, body of foor loop and then breaks out of the for loop.