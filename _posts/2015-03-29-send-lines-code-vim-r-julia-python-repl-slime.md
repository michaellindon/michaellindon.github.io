---
id: 1108
title: Send Lines of Code from Vim to R/Julia/Python REPL
date: 2015-03-29T10:36:42+00:00
author: admin
layout: post
guid: http://www.lindonslog.com/?p=1108
permalink: /linux-unix/send-lines-code-vim-r-julia-python-repl-slime/
categories:
  - Linux/Unix
---
# Vim + Tmux + Slime Workflow

Editing code in anything other than Vim seems to drain my coding stamina rapidly, but when working with a language such as R that has an interactive REPL (read evaluate print loop) it is often useful to send lines of code thereto for purposes of debugging. I have used the [Vim-R-Plugin](https://github.com/vim-scripts/Vim-R-plugin) for some time but recently my differences with R have led me to Julia, and so I have been looking for a way to send code from Vim to Julia. My search has resulted in [vim-slime](https://github.com/jpalardy/vim-slime).

## Vim Slime

vim-slime is absolutely great, it&#8217;s a plugin for vim that lets you send text to screen or tmux. This is perfect for programming any language that has an REPL, simply load a session up in a tmux, open up vim, tell it what tmux window and session you want to send text to and away you go. This will work for any language!
  
A great post can be found [here](https://technotales.wordpress.com/2007/10/03/like-slime-for-vim/).

## Edit Locally, Execute Remotely

I&#8217;ve long since desired to be able to do this. It would be great if you could edit the file locally, but have the Julia session execute on a remote machine, say my office computer, so that all the heavy lifting is done by the remote machine allowing me to get on with my own personal things on my local machine. Vim-slime provides an elegant solution. Simply start a tmux session, ssh to your remote machine, start up an interactive REPL of the language of your choice, open another terminal locally and start vim on your code and now you can send lines to your remote R/Julia/Python session! This has dramatically improved my workflow as my office computer is much more powerful than my laptop.
  


<div id="attachment_1112" style="width: 650px" class="wp-caption aligncenter">
  <a href="http://www.lindonslog.com/wp-content/uploads/2015/03/vimslime.png"><img src="http://www.lindonslog.com/wp-content/uploads/2015/03/vimslime-1024x576.png" alt="vimslime" width="640" height="360" class="size-large wp-image-1112" srcset="http://www.lindonslog.com/wp-content/uploads/2015/03/vimslime-1024x576.png 1024w, http://www.lindonslog.com/wp-content/uploads/2015/03/vimslime-300x169.png 300w" sizes="(max-width: 640px) 100vw, 640px" /></a>
  
  <p class="wp-caption-text">
    send code from vim to r python julia etc repl
  </p>
</div>