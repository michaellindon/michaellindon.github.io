---
id: 1146
title: Passing Julia Type to C Function as Struct
date: 2016-02-24T14:28:06+00:00
author: admin
layout: post
guid: http://www.lindonslog.com/?p=1146
permalink: /programming/passing-julia-type-to-c-function-as-struct/
categories:
  - julia
  - Programming
---
There seems to be hardly any examples to be found about calling C from Julia (perhaps because people feel no need to do so). Moreover the docs are, at least to me, quite cryptic. So if anyone wants to get started here is a minimal example.

## Julia Code:

<pre class="brush: python; title: ; notranslate" title="">type mystruct                                                                                                                                                                                                      
        n::Int32                                                                                                                                                                                                   
end 
</pre>

## C Code:

<pre class="brush: cpp; title: ; notranslate" title="">struct mystruct{
	int n;
};
void structure(struct mystruct * input);
void sructure(struct mystruct * input){
	(*input).n=99;
}
</pre>

_Compilation:_
  
`gcc -fPIC -shared rosetta.c -o rosetta.so`

_Execution:_
  
`julia> test=mystruct(10)<br />
mystruct(10)</p>
<p>julia> ccall((:structure, "rosetta.so"), Void, (Ref{mystruct},), test);</p>
<p>julia> test<br />
mystruct(99)`