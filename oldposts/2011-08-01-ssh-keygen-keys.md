---
id: 1
title: Creating SSH Keys with ssh-keygen and ssh-copy-id
date: 2011-08-01T16:36:16+00:00
author: admin
layout: post
guid: http://www.lindonslog.com/?p=1
permalink: /linux-unix/ssh-keygen-keys/
categories:
  - Linux/Unix
---
Have you ever gotten tired of constantly entering your password whenever ssh&#8217;ing a remote computer? You don&#8217;t need to! You can create a pair of SSH keys, namely a private and public one, which will save you from re-entering your password in the future. The private key you keep secret in your home folder and the public key you copy to every server or remote computer which you wish to SSH. The authentication then proceeds as follows: when you wish to SSH a remote computer, their public key is then compared with your private key and if they match, then the authentication succeeds. Let&#8217;s proceed with how we create SSH keys

&nbsp;

**1. Create SSH Keys with ssh-keygen**

Open up a terminal and type _ssh-keygen_

<pre>lindon@michael-laptop:~$ ssh-keygen
Generating public/private rsa key pair.
Enter file in which to save the key (/home/lindon/.ssh/id_rsa):  
Created directory '/home/lindon/.ssh'.
Enter passphrase (empty for no passphrase):
Enter same passphrase again:
Your identification has been saved in /home/lindon/.ssh/id_rsa.
Your public key has been saved in /home/lindon/.ssh/id_rsa.pub.
</pre>

When prompted to enter a file, passphrase, same passphrase, simply press return &#8211; we don&#8217;t want to enter anything here. The last line will then be succeeded  by the key&#8217;s _fingerprint_ and _random image_. You&#8217;ll notice that the public and private key are found in the /home/user/.ssh    folder (it&#8217;s hidden so make sure you can view hidden folders). The next step is to copy the public key over to the remote computer&#8230;

&nbsp;

**2.Transfer the public key with ssh-copy-id command**

Syntax: ssh-copy-id [-i [identity_file]] _[user@]machine_

<pre>lindon@michael-laptop:~$ ssh-copy-id -i lindon@remotecomputer.com
The authenticity of host 'remotecomputer (xx.xxx.xxx.xx)' can't be established.
RSA key fingerprint is <del>  :  :  :  :  :  :  :  :  :  :  :  :  :  :  :  :</del>.
Are you sure you want to continue connecting (yes/no)? yes
Warning: Permanently added 'remotecomputer,xx.xxx.xxx.xx' (RSA) to the list of known hosts.
lindon@remotecomputer's password:
Now try logging into the machine, with "ssh 'lindon@remotecomputer.com'", and check in:

~/.ssh/authorized_keys

to make sure we haven't added extra keys that you weren't expecting.

lindon@michael-laptop:~$
</pre>

The -i option means that the default public key, i.e. ~/.ssh/id_rsa.pub, is used.

&nbsp;

**3. Login without Password**

<pre>lindon@michael-laptop:~$ ssh lindon@remotecomputer.com
Welcome to Ubuntu 11.04 (GNU/Linux 2.6.38-11-generic x86_64)

* Documentation:  https://help.ubuntu.com/

Last login: Sun Sep 11 15:57:14 2011 from xx.xxx.xxx.xx
lindon@remotecomputer:~$</pre>

And there we go, login without password!