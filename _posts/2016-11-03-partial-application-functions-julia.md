---
id: 1181
title: Partial Application for Functions in Julia
date: 2016-11-03T13:07:09+00:00
author: admin
layout: single
author_profile: true 
guid: http://www.lindonslog.com/?p=1181
permalink: /programming/partial-application-functions-julia/
categories:
  - julia
tags:
  - functional programming
  - julia
---
Julia is purportedly a multi-paradigm language but I find their support for functional paradigms to be lacking. One feature that I looked for was <a href="https://en.wikipedia.org/wiki/Currying" target="_blank">Currying</a> or <a href="https://en.wikipedia.org/wiki/Partial_application" target="_blank">Partial Application</a> which corresponds to converting a function of multiple arguments into a sequence of single argument functions and taking a multiple argument function and fixing some of the
arguments to produce a new function of lesser arity respectively. Currying does not play nicely with Julias multiple dispatch model, and the creators have seemingly no intention of supporting it in the future if the discussion forums are anything to go by. In Clojure one has the <a href="https://clojuredocs.org/clojure.core/partial" target="_blank">partial function</a> for the latter. Here is my attempt to emulate this behaviour in Julia. 

## Partial Application

{% highlight julia %}
function partial(f,a...)
  (b...) -> f(a...,b...)
end
{% endhighlight %}

The function returns a lambda where the a&#8230; parameters are fixed. If you don&#8217;t know what the ellipsis do, check the <a href="https://en.wikibooks.org/wiki/Introducing_Julia/Functions" target="_blank">documentation</a> for &#8220;splat&#8221;. One can check the behavour is as expected

{% highlight julia %}
julia> function testf(x,y,z)
         2*x+y*y+x*y*z
       end
testf (generic function with 1 method)
julia> testf(2,3,4)
37
julia> partial(testf,2)(3,4)
37
julia> partial(testf,2,3)(4)
37
{% endhighlight %}


Of course you could just use a lambda.
