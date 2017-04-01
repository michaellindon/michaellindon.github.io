---
title: Clojure/Java and R Communication via Sockets
date: 2017-04-01T13:07:09+00:00
author: admin
categories:
  - clojure
  - r
tags:
  - clojure
  - r
---

## Motivation
Sometimes it is necessary for two applications to communicate with each other. 
{% include toc title="Sockets" %}
For instance, I like working in Clojure but my collaborator likes working in R. 
Originally I just bundled my clojure code into a standalone .jar file which my collaborator would execute from the command line. 
It would read in structured R data from a JSON file, perform some computation, serialize the output data to JSON which would get subsequently read back into R. 
This was a quick fix at the time, but by far not the most elegant solution. 
In particular my collaborator needed to execute this code many times in their application, and the .jar that I had distributed from the command line had quite a high startup cost, even with ahead of time compilation.
There are other tools like rJava which allow communication between R and Java, but I could not get these to work well with clojure and seemed a bit overkill. 
Moreover, these methods are very specific to both R and Java, whereas a more general approach exists.
The new solution introduces a client-server architecture whereby the clojure code (acting as a server) listens on a port for incoming data from R (the client). 
The idea is that clojure performs some expensive computation and then writes the output back to the socket. 
For an illustration lets just consider an "expensive" function that converts a string to upper case. 

## R Client Code
```r
capitalize <- function(inputstring){
	con <- socketConnection("localhost",port=6011)
	writeLines(inputstring,con)
	outputstring=character(0)
	while(length(outputstring)==0){
		outputstring=readLines(con)
	}
	close(con)
	outputstring
}
```

## Clojure Server Code
```clojure
(loop []
  (let [con (new java.net.ServerSocket 6011)
        servicesocket (.accept con)
        input (new java.io.DataInputStream (.getInputStream servicesocket))
        output (new java.io.DataOutputStream (.getOutputStream servicesocket))
        message-in (.readLine input)
        capitals (str (.toUpperCase message-in) "\n")]
    (.writeBytes output capitals)
    (.close output)
    (.close input)
    (.close servicesocket)
    (.close con)
    (recur)))
```
As you can see the clojure code relies mainly on native Java code existing for sockets. Load the clojure code into the clojure repl first, then R.

## Result
```r
> capitalize("hello")
[1] "HELLO"
```

Obviously this was a small example. In reality I was using this for some Shotgun Stochastic Search code I wrote [here](https://github.com/michaellindon/sss4clj)
