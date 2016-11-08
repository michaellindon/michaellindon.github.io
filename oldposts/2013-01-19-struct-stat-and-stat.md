---
id: 52
title: Struct stat and stat()
date: 2013-01-19T23:40:06+00:00
author: admin
layout: post
guid: http://www.lindonslog.com/?p=52
permalink: /programming/struct-stat-and-stat/
categories:
  - Programming
---
I came across this useful bit of code when wanting to read in some data from a text file. Suppose you do not know in advance how much data there is in a text file, how long should you make your buffer array to read the data into? The problem is you don&#8217;t know. You don&#8217;t want to make your buffer array neither too short and miss data, nor needlessly large.

You can use the stat() function to populate a structure containing information about a file, including its size, which you can then use to dynamically allocate memory. Here&#8217;s some example code:

<pre>#include &lt;stdio.h>
#include &lt;stdlib.h>
#include &lt;sys/stat.h>
#include &lt;sys/types.h>


int main(void){
    FILE *fp;
    char *buffer;
    struct stat statistics;
    fp = fopen("test.txt", "rb");
    stat("test.txt", &statistics);
    buffer = (char *) malloc(statistics.st_size);
    fread(buffer, 1, statistics.st_size, fp);
    fclose(fp);  
}
</pre>

First, you want to include the last two header files. Next you declare the struct stat structure where you want to store all the data. The stat() function takes two arguements. Number 1 being the file in question, number 2 the memory address of the structure where you want to store the information. So stat() will populate your structure. You can then call one of the members of this strucutre, statistics.st\_size (which is of type off\_t) to see how big in bytes the file is. Now a char is ALWAYS 1 byte, the standard says so, so you then allocate this number of bytes(==characters) for your array. fread() reads statistics.st_size objects, each of size 1, from fp and stores them in buffer. It advances the file position indicator by the number of bytes read.