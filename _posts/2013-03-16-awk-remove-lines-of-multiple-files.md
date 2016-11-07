---
id: 133
title: AWK Remove Lines of Multiple Files
date: 2013-03-16T19:22:34+00:00
author: admin
layout: post
guid: http://www.lindonslog.com/?p=133
permalink: /linux-unix/awk-remove-lines-of-multiple-files/
categories:
  - Linux/Unix
---
My girlfriend is a statistician and acquired some data in the form of 200 text files. The problem is that each file had two lines of non-informative description which needed removing. The goal was to process the data to remove the descriptive header. Instead of removing the lines from every file, I wrote this for her

`<br />
mkdir news<br />
for file in *;<br />
do awk '{if (NR!=1 && NR!=2) {print}}' "$file" > new_"$file";<br />
mv new_"$file" news/"$file";<br />
done </p>
<p>cd news;<br />
rm script;<br />
rm news;<br />
` 

This uses awk to remove lines 1 and 2 from all of the files in the current directory and store the reduced files in the newly created directory &#8220;news&#8221;. Just run the shell script in the directory containing all the files.