---
id: 1177
title: Newtons Iteration in Scala, Clojure and Haskell Comparison
date: 2016-10-16T09:19:28+00:00
author: admin
layout: single
guid: http://www.lindonslog.com/?p=1177
permalink: /programming/newtons-iteration-scala-clojure-haskell/
tags:
  - scala
  - clojure
  - haskell
  - functional programming
categories:
  - functional programming
---
Newton&#8217;s iterations is an algorithm for computing the square root of a number n. It also makes a nice &#8220;Hello World&#8221; program if you find yourself doing a lot of optimization. Here is how it looks in Haskell, Clojure and Scala

## Haskell

{% highlight haskell linenos %}
update y x = 0.5*(x+y/x)
foo = (iterate (update 100) 1000)
foobar = zip foo (tail foo)
notconverged tol x = if (abs(a-b)/abs(b) > tol) then True else False
  where
    a = fst x
    b = snd x
map (\x -> fst x) $ takeWhile (notconverged (0.000001)) foobar
{% endhighlight %}

## Clojure

{% highlight clojure linenos %}
(use '(incanter core stats charts io))
  
(defn sqroot [y x] (* 0.5 (+ x (/ y x))))
(def updater (partial sqroot 16))
(defn converged? [x] (> (abs (- (first x) (last x))) 0.0000001))
(map (fn [x] (first x)) (take-while converged? (partition 2 1 (iterate updater 100))))

{% endhighlight %}

## Scala

 
{% highlight scala linenos %}
import breeze.numerics.abs
  
def UpdateCurry(r: Double)(x: Double) = 0.5*(x+r/x)
  
val tolerance=0.0000001
val desiredSQRoot=16
val Update=UpdateCurry(desiredSQRoot)_
  
val iterates = Iterator.iterate(1.0) { x => Update(x) }
  
def ConvergedCurry(tol: Double)(x: Seq[Double]): Boolean = abs(x(0)-x(1))/abs(x(0))<tol
val Converged=ConvergedCurry(tolerance)_
  
val trace=iterates.sliding(2).takeWhile( x=> !Converged(x)).toList

{% endhighlight %}
