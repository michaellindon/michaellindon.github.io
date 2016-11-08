---
id: 43
title: Libraries
date: 2011-09-26T18:45:44+00:00
author: admin
layout: post
guid: http://www.lindonslog.com/?p=43
permalink: /linux-unix/libraries/
categories:
  - Linux/Unix
---
Compiling converts your source code into object or machine code, which the processor can understand. So the compiler produces an object file (.o) from your source code. The linker then pieces the object files together and herefrom produces an executable. If you wish to &#8220;compile only&#8221; i.e. to obtain merely the object file, you can add the &#8220;-c&#8221; flag at compilation:

<pre class="brush: bash; title: ; notranslate" title="">michael@michael-laptop:~$ gcc -c test.c</pre>

This produces the object file &#8220;test.o&#8221;. You can inspect this object file with the **nm** command. It basically lists all the symbols defined in the object file.

## Static Libraries

A static library is an archive of object files. All we do is include this archive in the compile line just like we would do for the .o files. Any executable created by linking with a static library contains its own copy of all the binary object files found inside, so only the executable need be distributed. This archive is created with the **ar** command e.g.

<pre class="brush: bash; title: ; notranslate" title="">michael@michael-laptop:~$ ar r library.a file1.o file2.o file3.o </pre>

&#8221; r Insert the files member into archive (with replacement).&#8221;
  
You can then display what files are in an archive with the t option.

<pre class="brush: bash; title: ; notranslate" title="">michael@michael-laptop:~$ ar t library.a</pre>

## Shared Libraries

It is a better idea to use shared libraries over static libraries. This is because modules that a program requires are loaded into the memory from shared objects at run time or load time, whereas static libraries are all put together at compile time. This has the advantage that I can change the object files in the libraries and not need to recompile my executable. If the library was static and I made a change, then I would need to recompile all the executables which depend on that library. Shared libraries have 3 names:

### soname

has the prefix &#8220;lib&#8221; and the suffix .so followed by a full stop and then the major version number i.e. libtest.so.1
  
We would only increment the major version number if we make a change which breaks backward compatibility e.g. changing the number of arguments that a function has

### realname

is the actual filename which contains the actual library code. It gains a minor version number plus a realase number in addition to the soname i.e. libtest.so.1.0.1

### linker name

is the name of the library which the linker refers to at compilation. It is the same as the soname, just without the version number i.e. libtest.so
  
The linker name is basically a symbolic link to the soname, which is itself a symbolic link to the real name.

To create a shared library, we need to compile our source code as follows:

<pre class="brush: bash; title: ; notranslate" title="">michael@michael-laptop:~$ gcc -fPIC -c test.c</pre>

The &#8220;-fPIC&#8221; option tells the compiler to produce _Position Independent Code_, which means the code can function regardless of where in the memory it is loaded. We can then proceed by using the &#8220;-shared&#8221; option at gcc and passing the soname as an option to the linker with the -Wl command.

<pre class="brush: bash; title: ; notranslate" title="">michael@michael-laptop:~$ gcc -shared -Wl,-soname,libtest.so.1 -o libtest.1.0.1 test.o</pre>

The -shared option tells the compiler that the output file should be a shared library.
  
-Wl,option
  
Pass option as an option to the linker. If option contains commas, it is split into multiple options at the commas. You must not include whitespaces.
  
The -soname option specifies the soname, duh.
  
The -o option specifies the real name.

Now that the shared library has been created, we need to install it, namely with ldconfig. The ldconfig program generates a symbolic link, named as the soname, to the realname. The -n option specifies the directory where the shared library is found.
  
Finally, we need to create a new symbolic link with a filename as the linker name, to the soname. 

<pre class="brush: bash; title: ; notranslate" title="">michael@michael-laptop:~$ ln -s libtest.so.1 libtest.so</pre>

where -s option stands for symbolic.