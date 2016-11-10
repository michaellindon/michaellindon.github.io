---
title: A Dirichlet Process Implementation in Julia
date: 2016-11-09T13:07:09+00:00
author: admin
categories:
  - julia
tags:
  - nonparametric
  - dirichlet process
  - mcmc
---

The following code closely follows the lecture notes and examples found [here](http://www2.stat.duke.edu/~st118/sta941/ "Surya Tokdar") which provide excellent reading on Dirichlet Processes. This post is less about DP's themselves and more about what an implementation in Julia might look like. The example here is the "Traffic" dataset from the "MASS" library for which the description reads

>An experiment was performed in Sweden in 1961-2 to assess the effect of a speed
>limit on the motorway accident rate. The experiment was conducted on 92 days
>in each year, matched so that day j in 1962 was comparable to day j in 1961.
>On some days the speed limit was in effect and enforced, while on other days
>there was no speed limit and cars tended to be driven faster. The speed limit
>days tended to be in contiguous blocks.

Let's load the dataset into Julia and inspect histograms of the number of accidents each day when the speed limit is and is not enforced.


```julia
using RDatasets
using Gadfly
using Distributions
using Iterators

speed=dataset("MASS","Traffic")
limityes=speed[speed[:Limit].=="yes",:Y]
nyes=length(limityes)
limitno=speed[speed[:Limit].=="no",:Y]
nno=length(limitno)
plot(speed, x="Y", color="Limit", Geom.histogram(bincount=20,density=true))
```




<?xml version="1.0" encoding="UTF-8"?>
<svg xmlns="http://www.w3.org/2000/svg"
     xmlns:xlink="http://www.w3.org/1999/xlink"
     xmlns:gadfly="http://www.gadflyjl.org/ns"
     version="1.2"
     width="141.42mm" height="100mm" viewBox="0 0 141.42 100"
     stroke="none"
     fill="#000000"
     stroke-width="0.3"
     font-size="3.88"

     id="img-32380ac5">
<g class="plotroot xscalable yscalable" id="img-32380ac5-1">
  <g font-size="3.88" font-family="'PT Sans','Helvetica Neue','Helvetica',sans-serif" fill="#564A55" stroke="#000000" stroke-opacity="0.000" id="img-32380ac5-2">
    <text x="69.44" y="88.39" text-anchor="middle" dy="0.6em">Y</text>
  </g>
  <g class="guide xlabels" font-size="2.82" font-family="'PT Sans Caption','Helvetica Neue','Helvetica',sans-serif" fill="#6C606B" id="img-32380ac5-3">
    <text x="-128.04" y="84.39" text-anchor="middle" visibility="hidden" gadfly:scale="1.0">-100</text>
    <text x="-99.83" y="84.39" text-anchor="middle" visibility="hidden" gadfly:scale="1.0">-80</text>
    <text x="-71.62" y="84.39" text-anchor="middle" visibility="hidden" gadfly:scale="1.0">-60</text>
    <text x="-43.4" y="84.39" text-anchor="middle" visibility="hidden" gadfly:scale="1.0">-40</text>
    <text x="-15.19" y="84.39" text-anchor="middle" visibility="hidden" gadfly:scale="1.0">-20</text>
    <text x="13.02" y="84.39" text-anchor="middle" visibility="visible" gadfly:scale="1.0">0</text>
    <text x="41.23" y="84.39" text-anchor="middle" visibility="visible" gadfly:scale="1.0">20</text>
    <text x="69.44" y="84.39" text-anchor="middle" visibility="visible" gadfly:scale="1.0">40</text>
    <text x="97.66" y="84.39" text-anchor="middle" visibility="visible" gadfly:scale="1.0">60</text>
    <text x="125.87" y="84.39" text-anchor="middle" visibility="visible" gadfly:scale="1.0">80</text>
    <text x="154.08" y="84.39" text-anchor="middle" visibility="hidden" gadfly:scale="1.0">100</text>
    <text x="182.29" y="84.39" text-anchor="middle" visibility="hidden" gadfly:scale="1.0">120</text>
    <text x="210.51" y="84.39" text-anchor="middle" visibility="hidden" gadfly:scale="1.0">140</text>
    <text x="238.72" y="84.39" text-anchor="middle" visibility="hidden" gadfly:scale="1.0">160</text>
    <text x="266.93" y="84.39" text-anchor="middle" visibility="hidden" gadfly:scale="1.0">180</text>
    <text x="-99.83" y="84.39" text-anchor="middle" visibility="hidden" gadfly:scale="10.0">-80</text>
    <text x="-92.78" y="84.39" text-anchor="middle" visibility="hidden" gadfly:scale="10.0">-75</text>
    <text x="-85.72" y="84.39" text-anchor="middle" visibility="hidden" gadfly:scale="10.0">-70</text>
    <text x="-78.67" y="84.39" text-anchor="middle" visibility="hidden" gadfly:scale="10.0">-65</text>
    <text x="-71.62" y="84.39" text-anchor="middle" visibility="hidden" gadfly:scale="10.0">-60</text>
    <text x="-64.56" y="84.39" text-anchor="middle" visibility="hidden" gadfly:scale="10.0">-55</text>
    <text x="-57.51" y="84.39" text-anchor="middle" visibility="hidden" gadfly:scale="10.0">-50</text>
    <text x="-50.46" y="84.39" text-anchor="middle" visibility="hidden" gadfly:scale="10.0">-45</text>
    <text x="-43.4" y="84.39" text-anchor="middle" visibility="hidden" gadfly:scale="10.0">-40</text>
    <text x="-36.35" y="84.39" text-anchor="middle" visibility="hidden" gadfly:scale="10.0">-35</text>
    <text x="-29.3" y="84.39" text-anchor="middle" visibility="hidden" gadfly:scale="10.0">-30</text>
    <text x="-22.25" y="84.39" text-anchor="middle" visibility="hidden" gadfly:scale="10.0">-25</text>
    <text x="-15.19" y="84.39" text-anchor="middle" visibility="hidden" gadfly:scale="10.0">-20</text>
    <text x="-8.14" y="84.39" text-anchor="middle" visibility="hidden" gadfly:scale="10.0">-15</text>
    <text x="-1.09" y="84.39" text-anchor="middle" visibility="hidden" gadfly:scale="10.0">-10</text>
    <text x="5.97" y="84.39" text-anchor="middle" visibility="hidden" gadfly:scale="10.0">-5</text>
    <text x="13.02" y="84.39" text-anchor="middle" visibility="hidden" gadfly:scale="10.0">0</text>
    <text x="20.07" y="84.39" text-anchor="middle" visibility="hidden" gadfly:scale="10.0">5</text>
    <text x="27.13" y="84.39" text-anchor="middle" visibility="hidden" gadfly:scale="10.0">10</text>
    <text x="34.18" y="84.39" text-anchor="middle" visibility="hidden" gadfly:scale="10.0">15</text>
    <text x="41.23" y="84.39" text-anchor="middle" visibility="hidden" gadfly:scale="10.0">20</text>
    <text x="48.29" y="84.39" text-anchor="middle" visibility="hidden" gadfly:scale="10.0">25</text>
    <text x="55.34" y="84.39" text-anchor="middle" visibility="hidden" gadfly:scale="10.0">30</text>
    <text x="62.39" y="84.39" text-anchor="middle" visibility="hidden" gadfly:scale="10.0">35</text>
    <text x="69.44" y="84.39" text-anchor="middle" visibility="hidden" gadfly:scale="10.0">40</text>
    <text x="76.5" y="84.39" text-anchor="middle" visibility="hidden" gadfly:scale="10.0">45</text>
    <text x="83.55" y="84.39" text-anchor="middle" visibility="hidden" gadfly:scale="10.0">50</text>
    <text x="90.6" y="84.39" text-anchor="middle" visibility="hidden" gadfly:scale="10.0">55</text>
    <text x="97.66" y="84.39" text-anchor="middle" visibility="hidden" gadfly:scale="10.0">60</text>
    <text x="104.71" y="84.39" text-anchor="middle" visibility="hidden" gadfly:scale="10.0">65</text>
    <text x="111.76" y="84.39" text-anchor="middle" visibility="hidden" gadfly:scale="10.0">70</text>
    <text x="118.82" y="84.39" text-anchor="middle" visibility="hidden" gadfly:scale="10.0">75</text>
    <text x="125.87" y="84.39" text-anchor="middle" visibility="hidden" gadfly:scale="10.0">80</text>
    <text x="132.92" y="84.39" text-anchor="middle" visibility="hidden" gadfly:scale="10.0">85</text>
    <text x="139.97" y="84.39" text-anchor="middle" visibility="hidden" gadfly:scale="10.0">90</text>
    <text x="147.03" y="84.39" text-anchor="middle" visibility="hidden" gadfly:scale="10.0">95</text>
    <text x="154.08" y="84.39" text-anchor="middle" visibility="hidden" gadfly:scale="10.0">100</text>
    <text x="161.13" y="84.39" text-anchor="middle" visibility="hidden" gadfly:scale="10.0">105</text>
    <text x="168.19" y="84.39" text-anchor="middle" visibility="hidden" gadfly:scale="10.0">110</text>
    <text x="175.24" y="84.39" text-anchor="middle" visibility="hidden" gadfly:scale="10.0">115</text>
    <text x="182.29" y="84.39" text-anchor="middle" visibility="hidden" gadfly:scale="10.0">120</text>
    <text x="189.35" y="84.39" text-anchor="middle" visibility="hidden" gadfly:scale="10.0">125</text>
    <text x="196.4" y="84.39" text-anchor="middle" visibility="hidden" gadfly:scale="10.0">130</text>
    <text x="203.45" y="84.39" text-anchor="middle" visibility="hidden" gadfly:scale="10.0">135</text>
    <text x="210.51" y="84.39" text-anchor="middle" visibility="hidden" gadfly:scale="10.0">140</text>
    <text x="217.56" y="84.39" text-anchor="middle" visibility="hidden" gadfly:scale="10.0">145</text>
    <text x="224.61" y="84.39" text-anchor="middle" visibility="hidden" gadfly:scale="10.0">150</text>
    <text x="231.66" y="84.39" text-anchor="middle" visibility="hidden" gadfly:scale="10.0">155</text>
    <text x="238.72" y="84.39" text-anchor="middle" visibility="hidden" gadfly:scale="10.0">160</text>
    <text x="-128.04" y="84.39" text-anchor="middle" visibility="hidden" gadfly:scale="0.5">-100</text>
    <text x="13.02" y="84.39" text-anchor="middle" visibility="hidden" gadfly:scale="0.5">0</text>
    <text x="154.08" y="84.39" text-anchor="middle" visibility="hidden" gadfly:scale="0.5">100</text>
    <text x="295.14" y="84.39" text-anchor="middle" visibility="hidden" gadfly:scale="0.5">200</text>
    <text x="-99.83" y="84.39" text-anchor="middle" visibility="hidden" gadfly:scale="5.0">-80</text>
    <text x="-92.78" y="84.39" text-anchor="middle" visibility="hidden" gadfly:scale="5.0">-75</text>
    <text x="-85.72" y="84.39" text-anchor="middle" visibility="hidden" gadfly:scale="5.0">-70</text>
    <text x="-78.67" y="84.39" text-anchor="middle" visibility="hidden" gadfly:scale="5.0">-65</text>
    <text x="-71.62" y="84.39" text-anchor="middle" visibility="hidden" gadfly:scale="5.0">-60</text>
    <text x="-64.56" y="84.39" text-anchor="middle" visibility="hidden" gadfly:scale="5.0">-55</text>
    <text x="-57.51" y="84.39" text-anchor="middle" visibility="hidden" gadfly:scale="5.0">-50</text>
    <text x="-50.46" y="84.39" text-anchor="middle" visibility="hidden" gadfly:scale="5.0">-45</text>
    <text x="-43.4" y="84.39" text-anchor="middle" visibility="hidden" gadfly:scale="5.0">-40</text>
    <text x="-36.35" y="84.39" text-anchor="middle" visibility="hidden" gadfly:scale="5.0">-35</text>
    <text x="-29.3" y="84.39" text-anchor="middle" visibility="hidden" gadfly:scale="5.0">-30</text>
    <text x="-22.25" y="84.39" text-anchor="middle" visibility="hidden" gadfly:scale="5.0">-25</text>
    <text x="-15.19" y="84.39" text-anchor="middle" visibility="hidden" gadfly:scale="5.0">-20</text>
    <text x="-8.14" y="84.39" text-anchor="middle" visibility="hidden" gadfly:scale="5.0">-15</text>
    <text x="-1.09" y="84.39" text-anchor="middle" visibility="hidden" gadfly:scale="5.0">-10</text>
    <text x="5.97" y="84.39" text-anchor="middle" visibility="hidden" gadfly:scale="5.0">-5</text>
    <text x="13.02" y="84.39" text-anchor="middle" visibility="hidden" gadfly:scale="5.0">0</text>
    <text x="20.07" y="84.39" text-anchor="middle" visibility="hidden" gadfly:scale="5.0">5</text>
    <text x="27.13" y="84.39" text-anchor="middle" visibility="hidden" gadfly:scale="5.0">10</text>
    <text x="34.18" y="84.39" text-anchor="middle" visibility="hidden" gadfly:scale="5.0">15</text>
    <text x="41.23" y="84.39" text-anchor="middle" visibility="hidden" gadfly:scale="5.0">20</text>
    <text x="48.29" y="84.39" text-anchor="middle" visibility="hidden" gadfly:scale="5.0">25</text>
    <text x="55.34" y="84.39" text-anchor="middle" visibility="hidden" gadfly:scale="5.0">30</text>
    <text x="62.39" y="84.39" text-anchor="middle" visibility="hidden" gadfly:scale="5.0">35</text>
    <text x="69.44" y="84.39" text-anchor="middle" visibility="hidden" gadfly:scale="5.0">40</text>
    <text x="76.5" y="84.39" text-anchor="middle" visibility="hidden" gadfly:scale="5.0">45</text>
    <text x="83.55" y="84.39" text-anchor="middle" visibility="hidden" gadfly:scale="5.0">50</text>
    <text x="90.6" y="84.39" text-anchor="middle" visibility="hidden" gadfly:scale="5.0">55</text>
    <text x="97.66" y="84.39" text-anchor="middle" visibility="hidden" gadfly:scale="5.0">60</text>
    <text x="104.71" y="84.39" text-anchor="middle" visibility="hidden" gadfly:scale="5.0">65</text>
    <text x="111.76" y="84.39" text-anchor="middle" visibility="hidden" gadfly:scale="5.0">70</text>
    <text x="118.82" y="84.39" text-anchor="middle" visibility="hidden" gadfly:scale="5.0">75</text>
    <text x="125.87" y="84.39" text-anchor="middle" visibility="hidden" gadfly:scale="5.0">80</text>
    <text x="132.92" y="84.39" text-anchor="middle" visibility="hidden" gadfly:scale="5.0">85</text>
    <text x="139.97" y="84.39" text-anchor="middle" visibility="hidden" gadfly:scale="5.0">90</text>
    <text x="147.03" y="84.39" text-anchor="middle" visibility="hidden" gadfly:scale="5.0">95</text>
    <text x="154.08" y="84.39" text-anchor="middle" visibility="hidden" gadfly:scale="5.0">100</text>
    <text x="161.13" y="84.39" text-anchor="middle" visibility="hidden" gadfly:scale="5.0">105</text>
    <text x="168.19" y="84.39" text-anchor="middle" visibility="hidden" gadfly:scale="5.0">110</text>
    <text x="175.24" y="84.39" text-anchor="middle" visibility="hidden" gadfly:scale="5.0">115</text>
    <text x="182.29" y="84.39" text-anchor="middle" visibility="hidden" gadfly:scale="5.0">120</text>
    <text x="189.35" y="84.39" text-anchor="middle" visibility="hidden" gadfly:scale="5.0">125</text>
    <text x="196.4" y="84.39" text-anchor="middle" visibility="hidden" gadfly:scale="5.0">130</text>
    <text x="203.45" y="84.39" text-anchor="middle" visibility="hidden" gadfly:scale="5.0">135</text>
    <text x="210.51" y="84.39" text-anchor="middle" visibility="hidden" gadfly:scale="5.0">140</text>
    <text x="217.56" y="84.39" text-anchor="middle" visibility="hidden" gadfly:scale="5.0">145</text>
    <text x="224.61" y="84.39" text-anchor="middle" visibility="hidden" gadfly:scale="5.0">150</text>
    <text x="231.66" y="84.39" text-anchor="middle" visibility="hidden" gadfly:scale="5.0">155</text>
    <text x="238.72" y="84.39" text-anchor="middle" visibility="hidden" gadfly:scale="5.0">160</text>
  </g>
  <g class="guide colorkey" id="img-32380ac5-4">
    <g fill="#4C404B" font-size="2.82" font-family="'PT Sans','Helvetica Neue','Helvetica',sans-serif" id="img-32380ac5-5">
      <text x="131.68" y="42.86" dy="0.35em" id="img-32380ac5-6" class="color_no">no</text>
      <text x="131.68" y="46.48" dy="0.35em" id="img-32380ac5-7" class="color_yes">yes</text>
    </g>
    <g stroke="#000000" stroke-opacity="0.000" id="img-32380ac5-8">
      <rect x="128.87" y="41.95" width="1.81" height="1.81" id="img-32380ac5-9" fill="#00BFFF" class="color_no"/>
      <rect x="128.87" y="45.58" width="1.81" height="1.81" id="img-32380ac5-10" fill="#D4CA3A" class="color_yes"/>
    </g>
    <g fill="#362A35" font-size="3.88" font-family="'PT Sans','Helvetica Neue','Helvetica',sans-serif" stroke="#000000" stroke-opacity="0.000" id="img-32380ac5-11">
      <text x="128.87" y="39.04" id="img-32380ac5-12">Limit</text>
    </g>
  </g>
<g clip-path="url(#img-32380ac5-13)">
  <g id="img-32380ac5-14">
    <g pointer-events="visible" opacity="1" fill="#000000" fill-opacity="0.000" stroke="#000000" stroke-opacity="0.000" class="guide background" id="img-32380ac5-15">
      <rect x="11.02" y="5" width="116.85" height="75.72" id="img-32380ac5-16"/>
    </g>
    <g class="guide ygridlines xfixed" stroke-dasharray="0.5,0.5" stroke-width="0.2" stroke="#D0D0E0" id="img-32380ac5-17">
      <path fill="none" d="M11.02,174.33 L 127.87 174.33" id="img-32380ac5-18" visibility="hidden" gadfly:scale="1.0"/>
      <path fill="none" d="M11.02,150.43 L 127.87 150.43" id="img-32380ac5-19" visibility="hidden" gadfly:scale="1.0"/>
      <path fill="none" d="M11.02,126.52 L 127.87 126.52" id="img-32380ac5-20" visibility="hidden" gadfly:scale="1.0"/>
      <path fill="none" d="M11.02,102.62 L 127.87 102.62" id="img-32380ac5-21" visibility="hidden" gadfly:scale="1.0"/>
      <path fill="none" d="M11.02,78.71 L 127.87 78.71" id="img-32380ac5-22" visibility="visible" gadfly:scale="1.0"/>
      <path fill="none" d="M11.02,54.81 L 127.87 54.81" id="img-32380ac5-23" visibility="visible" gadfly:scale="1.0"/>
      <path fill="none" d="M11.02,30.9 L 127.87 30.9" id="img-32380ac5-24" visibility="visible" gadfly:scale="1.0"/>
      <path fill="none" d="M11.02,7 L 127.87 7" id="img-32380ac5-25" visibility="visible" gadfly:scale="1.0"/>
      <path fill="none" d="M11.02,-16.91 L 127.87 -16.91" id="img-32380ac5-26" visibility="hidden" gadfly:scale="1.0"/>
      <path fill="none" d="M11.02,-40.81 L 127.87 -40.81" id="img-32380ac5-27" visibility="hidden" gadfly:scale="1.0"/>
      <path fill="none" d="M11.02,-64.71 L 127.87 -64.71" id="img-32380ac5-28" visibility="hidden" gadfly:scale="1.0"/>
      <path fill="none" d="M11.02,-88.62 L 127.87 -88.62" id="img-32380ac5-29" visibility="hidden" gadfly:scale="1.0"/>
      <path fill="none" d="M11.02,150.43 L 127.87 150.43" id="img-32380ac5-30" visibility="hidden" gadfly:scale="10.0"/>
      <path fill="none" d="M11.02,148.04 L 127.87 148.04" id="img-32380ac5-31" visibility="hidden" gadfly:scale="10.0"/>
      <path fill="none" d="M11.02,145.65 L 127.87 145.65" id="img-32380ac5-32" visibility="hidden" gadfly:scale="10.0"/>
      <path fill="none" d="M11.02,143.26 L 127.87 143.26" id="img-32380ac5-33" visibility="hidden" gadfly:scale="10.0"/>
      <path fill="none" d="M11.02,140.87 L 127.87 140.87" id="img-32380ac5-34" visibility="hidden" gadfly:scale="10.0"/>
      <path fill="none" d="M11.02,138.48 L 127.87 138.48" id="img-32380ac5-35" visibility="hidden" gadfly:scale="10.0"/>
      <path fill="none" d="M11.02,136.09 L 127.87 136.09" id="img-32380ac5-36" visibility="hidden" gadfly:scale="10.0"/>
      <path fill="none" d="M11.02,133.7 L 127.87 133.7" id="img-32380ac5-37" visibility="hidden" gadfly:scale="10.0"/>
      <path fill="none" d="M11.02,131.31 L 127.87 131.31" id="img-32380ac5-38" visibility="hidden" gadfly:scale="10.0"/>
      <path fill="none" d="M11.02,128.92 L 127.87 128.92" id="img-32380ac5-39" visibility="hidden" gadfly:scale="10.0"/>
      <path fill="none" d="M11.02,126.52 L 127.87 126.52" id="img-32380ac5-40" visibility="hidden" gadfly:scale="10.0"/>
      <path fill="none" d="M11.02,124.13 L 127.87 124.13" id="img-32380ac5-41" visibility="hidden" gadfly:scale="10.0"/>
      <path fill="none" d="M11.02,121.74 L 127.87 121.74" id="img-32380ac5-42" visibility="hidden" gadfly:scale="10.0"/>
      <path fill="none" d="M11.02,119.35 L 127.87 119.35" id="img-32380ac5-43" visibility="hidden" gadfly:scale="10.0"/>
      <path fill="none" d="M11.02,116.96 L 127.87 116.96" id="img-32380ac5-44" visibility="hidden" gadfly:scale="10.0"/>
      <path fill="none" d="M11.02,114.57 L 127.87 114.57" id="img-32380ac5-45" visibility="hidden" gadfly:scale="10.0"/>
      <path fill="none" d="M11.02,112.18 L 127.87 112.18" id="img-32380ac5-46" visibility="hidden" gadfly:scale="10.0"/>
      <path fill="none" d="M11.02,109.79 L 127.87 109.79" id="img-32380ac5-47" visibility="hidden" gadfly:scale="10.0"/>
      <path fill="none" d="M11.02,107.4 L 127.87 107.4" id="img-32380ac5-48" visibility="hidden" gadfly:scale="10.0"/>
      <path fill="none" d="M11.02,105.01 L 127.87 105.01" id="img-32380ac5-49" visibility="hidden" gadfly:scale="10.0"/>
      <path fill="none" d="M11.02,102.62 L 127.87 102.62" id="img-32380ac5-50" visibility="hidden" gadfly:scale="10.0"/>
      <path fill="none" d="M11.02,100.23 L 127.87 100.23" id="img-32380ac5-51" visibility="hidden" gadfly:scale="10.0"/>
      <path fill="none" d="M11.02,97.84 L 127.87 97.84" id="img-32380ac5-52" visibility="hidden" gadfly:scale="10.0"/>
      <path fill="none" d="M11.02,95.45 L 127.87 95.45" id="img-32380ac5-53" visibility="hidden" gadfly:scale="10.0"/>
      <path fill="none" d="M11.02,93.06 L 127.87 93.06" id="img-32380ac5-54" visibility="hidden" gadfly:scale="10.0"/>
      <path fill="none" d="M11.02,90.67 L 127.87 90.67" id="img-32380ac5-55" visibility="hidden" gadfly:scale="10.0"/>
      <path fill="none" d="M11.02,88.28 L 127.87 88.28" id="img-32380ac5-56" visibility="hidden" gadfly:scale="10.0"/>
      <path fill="none" d="M11.02,85.89 L 127.87 85.89" id="img-32380ac5-57" visibility="hidden" gadfly:scale="10.0"/>
      <path fill="none" d="M11.02,83.5 L 127.87 83.5" id="img-32380ac5-58" visibility="hidden" gadfly:scale="10.0"/>
      <path fill="none" d="M11.02,81.11 L 127.87 81.11" id="img-32380ac5-59" visibility="hidden" gadfly:scale="10.0"/>
      <path fill="none" d="M11.02,78.71 L 127.87 78.71" id="img-32380ac5-60" visibility="hidden" gadfly:scale="10.0"/>
      <path fill="none" d="M11.02,76.32 L 127.87 76.32" id="img-32380ac5-61" visibility="hidden" gadfly:scale="10.0"/>
      <path fill="none" d="M11.02,73.93 L 127.87 73.93" id="img-32380ac5-62" visibility="hidden" gadfly:scale="10.0"/>
      <path fill="none" d="M11.02,71.54 L 127.87 71.54" id="img-32380ac5-63" visibility="hidden" gadfly:scale="10.0"/>
      <path fill="none" d="M11.02,69.15 L 127.87 69.15" id="img-32380ac5-64" visibility="hidden" gadfly:scale="10.0"/>
      <path fill="none" d="M11.02,66.76 L 127.87 66.76" id="img-32380ac5-65" visibility="hidden" gadfly:scale="10.0"/>
      <path fill="none" d="M11.02,64.37 L 127.87 64.37" id="img-32380ac5-66" visibility="hidden" gadfly:scale="10.0"/>
      <path fill="none" d="M11.02,61.98 L 127.87 61.98" id="img-32380ac5-67" visibility="hidden" gadfly:scale="10.0"/>
      <path fill="none" d="M11.02,59.59 L 127.87 59.59" id="img-32380ac5-68" visibility="hidden" gadfly:scale="10.0"/>
      <path fill="none" d="M11.02,57.2 L 127.87 57.2" id="img-32380ac5-69" visibility="hidden" gadfly:scale="10.0"/>
      <path fill="none" d="M11.02,54.81 L 127.87 54.81" id="img-32380ac5-70" visibility="hidden" gadfly:scale="10.0"/>
      <path fill="none" d="M11.02,52.42 L 127.87 52.42" id="img-32380ac5-71" visibility="hidden" gadfly:scale="10.0"/>
      <path fill="none" d="M11.02,50.03 L 127.87 50.03" id="img-32380ac5-72" visibility="hidden" gadfly:scale="10.0"/>
      <path fill="none" d="M11.02,47.64 L 127.87 47.64" id="img-32380ac5-73" visibility="hidden" gadfly:scale="10.0"/>
      <path fill="none" d="M11.02,45.25 L 127.87 45.25" id="img-32380ac5-74" visibility="hidden" gadfly:scale="10.0"/>
      <path fill="none" d="M11.02,42.86 L 127.87 42.86" id="img-32380ac5-75" visibility="hidden" gadfly:scale="10.0"/>
      <path fill="none" d="M11.02,40.47 L 127.87 40.47" id="img-32380ac5-76" visibility="hidden" gadfly:scale="10.0"/>
      <path fill="none" d="M11.02,38.08 L 127.87 38.08" id="img-32380ac5-77" visibility="hidden" gadfly:scale="10.0"/>
      <path fill="none" d="M11.02,35.69 L 127.87 35.69" id="img-32380ac5-78" visibility="hidden" gadfly:scale="10.0"/>
      <path fill="none" d="M11.02,33.3 L 127.87 33.3" id="img-32380ac5-79" visibility="hidden" gadfly:scale="10.0"/>
      <path fill="none" d="M11.02,30.9 L 127.87 30.9" id="img-32380ac5-80" visibility="hidden" gadfly:scale="10.0"/>
      <path fill="none" d="M11.02,28.51 L 127.87 28.51" id="img-32380ac5-81" visibility="hidden" gadfly:scale="10.0"/>
      <path fill="none" d="M11.02,26.12 L 127.87 26.12" id="img-32380ac5-82" visibility="hidden" gadfly:scale="10.0"/>
      <path fill="none" d="M11.02,23.73 L 127.87 23.73" id="img-32380ac5-83" visibility="hidden" gadfly:scale="10.0"/>
      <path fill="none" d="M11.02,21.34 L 127.87 21.34" id="img-32380ac5-84" visibility="hidden" gadfly:scale="10.0"/>
      <path fill="none" d="M11.02,18.95 L 127.87 18.95" id="img-32380ac5-85" visibility="hidden" gadfly:scale="10.0"/>
      <path fill="none" d="M11.02,16.56 L 127.87 16.56" id="img-32380ac5-86" visibility="hidden" gadfly:scale="10.0"/>
      <path fill="none" d="M11.02,14.17 L 127.87 14.17" id="img-32380ac5-87" visibility="hidden" gadfly:scale="10.0"/>
      <path fill="none" d="M11.02,11.78 L 127.87 11.78" id="img-32380ac5-88" visibility="hidden" gadfly:scale="10.0"/>
      <path fill="none" d="M11.02,9.39 L 127.87 9.39" id="img-32380ac5-89" visibility="hidden" gadfly:scale="10.0"/>
      <path fill="none" d="M11.02,7 L 127.87 7" id="img-32380ac5-90" visibility="hidden" gadfly:scale="10.0"/>
      <path fill="none" d="M11.02,4.61 L 127.87 4.61" id="img-32380ac5-91" visibility="hidden" gadfly:scale="10.0"/>
      <path fill="none" d="M11.02,2.22 L 127.87 2.22" id="img-32380ac5-92" visibility="hidden" gadfly:scale="10.0"/>
      <path fill="none" d="M11.02,-0.17 L 127.87 -0.17" id="img-32380ac5-93" visibility="hidden" gadfly:scale="10.0"/>
      <path fill="none" d="M11.02,-2.56 L 127.87 -2.56" id="img-32380ac5-94" visibility="hidden" gadfly:scale="10.0"/>
      <path fill="none" d="M11.02,-4.95 L 127.87 -4.95" id="img-32380ac5-95" visibility="hidden" gadfly:scale="10.0"/>
      <path fill="none" d="M11.02,-7.34 L 127.87 -7.34" id="img-32380ac5-96" visibility="hidden" gadfly:scale="10.0"/>
      <path fill="none" d="M11.02,-9.73 L 127.87 -9.73" id="img-32380ac5-97" visibility="hidden" gadfly:scale="10.0"/>
      <path fill="none" d="M11.02,-12.12 L 127.87 -12.12" id="img-32380ac5-98" visibility="hidden" gadfly:scale="10.0"/>
      <path fill="none" d="M11.02,-14.51 L 127.87 -14.51" id="img-32380ac5-99" visibility="hidden" gadfly:scale="10.0"/>
      <path fill="none" d="M11.02,-16.91 L 127.87 -16.91" id="img-32380ac5-100" visibility="hidden" gadfly:scale="10.0"/>
      <path fill="none" d="M11.02,-19.3 L 127.87 -19.3" id="img-32380ac5-101" visibility="hidden" gadfly:scale="10.0"/>
      <path fill="none" d="M11.02,-21.69 L 127.87 -21.69" id="img-32380ac5-102" visibility="hidden" gadfly:scale="10.0"/>
      <path fill="none" d="M11.02,-24.08 L 127.87 -24.08" id="img-32380ac5-103" visibility="hidden" gadfly:scale="10.0"/>
      <path fill="none" d="M11.02,-26.47 L 127.87 -26.47" id="img-32380ac5-104" visibility="hidden" gadfly:scale="10.0"/>
      <path fill="none" d="M11.02,-28.86 L 127.87 -28.86" id="img-32380ac5-105" visibility="hidden" gadfly:scale="10.0"/>
      <path fill="none" d="M11.02,-31.25 L 127.87 -31.25" id="img-32380ac5-106" visibility="hidden" gadfly:scale="10.0"/>
      <path fill="none" d="M11.02,-33.64 L 127.87 -33.64" id="img-32380ac5-107" visibility="hidden" gadfly:scale="10.0"/>
      <path fill="none" d="M11.02,-36.03 L 127.87 -36.03" id="img-32380ac5-108" visibility="hidden" gadfly:scale="10.0"/>
      <path fill="none" d="M11.02,-38.42 L 127.87 -38.42" id="img-32380ac5-109" visibility="hidden" gadfly:scale="10.0"/>
      <path fill="none" d="M11.02,-40.81 L 127.87 -40.81" id="img-32380ac5-110" visibility="hidden" gadfly:scale="10.0"/>
      <path fill="none" d="M11.02,-43.2 L 127.87 -43.2" id="img-32380ac5-111" visibility="hidden" gadfly:scale="10.0"/>
      <path fill="none" d="M11.02,-45.59 L 127.87 -45.59" id="img-32380ac5-112" visibility="hidden" gadfly:scale="10.0"/>
      <path fill="none" d="M11.02,-47.98 L 127.87 -47.98" id="img-32380ac5-113" visibility="hidden" gadfly:scale="10.0"/>
      <path fill="none" d="M11.02,-50.37 L 127.87 -50.37" id="img-32380ac5-114" visibility="hidden" gadfly:scale="10.0"/>
      <path fill="none" d="M11.02,-52.76 L 127.87 -52.76" id="img-32380ac5-115" visibility="hidden" gadfly:scale="10.0"/>
      <path fill="none" d="M11.02,-55.15 L 127.87 -55.15" id="img-32380ac5-116" visibility="hidden" gadfly:scale="10.0"/>
      <path fill="none" d="M11.02,-57.54 L 127.87 -57.54" id="img-32380ac5-117" visibility="hidden" gadfly:scale="10.0"/>
      <path fill="none" d="M11.02,-59.93 L 127.87 -59.93" id="img-32380ac5-118" visibility="hidden" gadfly:scale="10.0"/>
      <path fill="none" d="M11.02,-62.32 L 127.87 -62.32" id="img-32380ac5-119" visibility="hidden" gadfly:scale="10.0"/>
      <path fill="none" d="M11.02,-64.71 L 127.87 -64.71" id="img-32380ac5-120" visibility="hidden" gadfly:scale="10.0"/>
      <path fill="none" d="M11.02,174.33 L 127.87 174.33" id="img-32380ac5-121" visibility="hidden" gadfly:scale="0.5"/>
      <path fill="none" d="M11.02,78.71 L 127.87 78.71" id="img-32380ac5-122" visibility="hidden" gadfly:scale="0.5"/>
      <path fill="none" d="M11.02,-16.91 L 127.87 -16.91" id="img-32380ac5-123" visibility="hidden" gadfly:scale="0.5"/>
      <path fill="none" d="M11.02,-112.52 L 127.87 -112.52" id="img-32380ac5-124" visibility="hidden" gadfly:scale="0.5"/>
      <path fill="none" d="M11.02,150.43 L 127.87 150.43" id="img-32380ac5-125" visibility="hidden" gadfly:scale="5.0"/>
      <path fill="none" d="M11.02,145.65 L 127.87 145.65" id="img-32380ac5-126" visibility="hidden" gadfly:scale="5.0"/>
      <path fill="none" d="M11.02,140.87 L 127.87 140.87" id="img-32380ac5-127" visibility="hidden" gadfly:scale="5.0"/>
      <path fill="none" d="M11.02,136.09 L 127.87 136.09" id="img-32380ac5-128" visibility="hidden" gadfly:scale="5.0"/>
      <path fill="none" d="M11.02,131.31 L 127.87 131.31" id="img-32380ac5-129" visibility="hidden" gadfly:scale="5.0"/>
      <path fill="none" d="M11.02,126.52 L 127.87 126.52" id="img-32380ac5-130" visibility="hidden" gadfly:scale="5.0"/>
      <path fill="none" d="M11.02,121.74 L 127.87 121.74" id="img-32380ac5-131" visibility="hidden" gadfly:scale="5.0"/>
      <path fill="none" d="M11.02,116.96 L 127.87 116.96" id="img-32380ac5-132" visibility="hidden" gadfly:scale="5.0"/>
      <path fill="none" d="M11.02,112.18 L 127.87 112.18" id="img-32380ac5-133" visibility="hidden" gadfly:scale="5.0"/>
      <path fill="none" d="M11.02,107.4 L 127.87 107.4" id="img-32380ac5-134" visibility="hidden" gadfly:scale="5.0"/>
      <path fill="none" d="M11.02,102.62 L 127.87 102.62" id="img-32380ac5-135" visibility="hidden" gadfly:scale="5.0"/>
      <path fill="none" d="M11.02,97.84 L 127.87 97.84" id="img-32380ac5-136" visibility="hidden" gadfly:scale="5.0"/>
      <path fill="none" d="M11.02,93.06 L 127.87 93.06" id="img-32380ac5-137" visibility="hidden" gadfly:scale="5.0"/>
      <path fill="none" d="M11.02,88.28 L 127.87 88.28" id="img-32380ac5-138" visibility="hidden" gadfly:scale="5.0"/>
      <path fill="none" d="M11.02,83.5 L 127.87 83.5" id="img-32380ac5-139" visibility="hidden" gadfly:scale="5.0"/>
      <path fill="none" d="M11.02,78.71 L 127.87 78.71" id="img-32380ac5-140" visibility="hidden" gadfly:scale="5.0"/>
      <path fill="none" d="M11.02,73.93 L 127.87 73.93" id="img-32380ac5-141" visibility="hidden" gadfly:scale="5.0"/>
      <path fill="none" d="M11.02,69.15 L 127.87 69.15" id="img-32380ac5-142" visibility="hidden" gadfly:scale="5.0"/>
      <path fill="none" d="M11.02,64.37 L 127.87 64.37" id="img-32380ac5-143" visibility="hidden" gadfly:scale="5.0"/>
      <path fill="none" d="M11.02,59.59 L 127.87 59.59" id="img-32380ac5-144" visibility="hidden" gadfly:scale="5.0"/>
      <path fill="none" d="M11.02,54.81 L 127.87 54.81" id="img-32380ac5-145" visibility="hidden" gadfly:scale="5.0"/>
      <path fill="none" d="M11.02,50.03 L 127.87 50.03" id="img-32380ac5-146" visibility="hidden" gadfly:scale="5.0"/>
      <path fill="none" d="M11.02,45.25 L 127.87 45.25" id="img-32380ac5-147" visibility="hidden" gadfly:scale="5.0"/>
      <path fill="none" d="M11.02,40.47 L 127.87 40.47" id="img-32380ac5-148" visibility="hidden" gadfly:scale="5.0"/>
      <path fill="none" d="M11.02,35.69 L 127.87 35.69" id="img-32380ac5-149" visibility="hidden" gadfly:scale="5.0"/>
      <path fill="none" d="M11.02,30.9 L 127.87 30.9" id="img-32380ac5-150" visibility="hidden" gadfly:scale="5.0"/>
      <path fill="none" d="M11.02,26.12 L 127.87 26.12" id="img-32380ac5-151" visibility="hidden" gadfly:scale="5.0"/>
      <path fill="none" d="M11.02,21.34 L 127.87 21.34" id="img-32380ac5-152" visibility="hidden" gadfly:scale="5.0"/>
      <path fill="none" d="M11.02,16.56 L 127.87 16.56" id="img-32380ac5-153" visibility="hidden" gadfly:scale="5.0"/>
      <path fill="none" d="M11.02,11.78 L 127.87 11.78" id="img-32380ac5-154" visibility="hidden" gadfly:scale="5.0"/>
      <path fill="none" d="M11.02,7 L 127.87 7" id="img-32380ac5-155" visibility="hidden" gadfly:scale="5.0"/>
      <path fill="none" d="M11.02,2.22 L 127.87 2.22" id="img-32380ac5-156" visibility="hidden" gadfly:scale="5.0"/>
      <path fill="none" d="M11.02,-2.56 L 127.87 -2.56" id="img-32380ac5-157" visibility="hidden" gadfly:scale="5.0"/>
      <path fill="none" d="M11.02,-7.34 L 127.87 -7.34" id="img-32380ac5-158" visibility="hidden" gadfly:scale="5.0"/>
      <path fill="none" d="M11.02,-12.12 L 127.87 -12.12" id="img-32380ac5-159" visibility="hidden" gadfly:scale="5.0"/>
      <path fill="none" d="M11.02,-16.91 L 127.87 -16.91" id="img-32380ac5-160" visibility="hidden" gadfly:scale="5.0"/>
      <path fill="none" d="M11.02,-21.69 L 127.87 -21.69" id="img-32380ac5-161" visibility="hidden" gadfly:scale="5.0"/>
      <path fill="none" d="M11.02,-26.47 L 127.87 -26.47" id="img-32380ac5-162" visibility="hidden" gadfly:scale="5.0"/>
      <path fill="none" d="M11.02,-31.25 L 127.87 -31.25" id="img-32380ac5-163" visibility="hidden" gadfly:scale="5.0"/>
      <path fill="none" d="M11.02,-36.03 L 127.87 -36.03" id="img-32380ac5-164" visibility="hidden" gadfly:scale="5.0"/>
      <path fill="none" d="M11.02,-40.81 L 127.87 -40.81" id="img-32380ac5-165" visibility="hidden" gadfly:scale="5.0"/>
      <path fill="none" d="M11.02,-45.59 L 127.87 -45.59" id="img-32380ac5-166" visibility="hidden" gadfly:scale="5.0"/>
      <path fill="none" d="M11.02,-50.37 L 127.87 -50.37" id="img-32380ac5-167" visibility="hidden" gadfly:scale="5.0"/>
      <path fill="none" d="M11.02,-55.15 L 127.87 -55.15" id="img-32380ac5-168" visibility="hidden" gadfly:scale="5.0"/>
      <path fill="none" d="M11.02,-59.93 L 127.87 -59.93" id="img-32380ac5-169" visibility="hidden" gadfly:scale="5.0"/>
      <path fill="none" d="M11.02,-64.71 L 127.87 -64.71" id="img-32380ac5-170" visibility="hidden" gadfly:scale="5.0"/>
    </g>
    <g class="guide xgridlines yfixed" stroke-dasharray="0.5,0.5" stroke-width="0.2" stroke="#D0D0E0" id="img-32380ac5-171">
      <path fill="none" d="M-128.04,5 L -128.04 80.72" id="img-32380ac5-172" visibility="hidden" gadfly:scale="1.0"/>
      <path fill="none" d="M-99.83,5 L -99.83 80.72" id="img-32380ac5-173" visibility="hidden" gadfly:scale="1.0"/>
      <path fill="none" d="M-71.62,5 L -71.62 80.72" id="img-32380ac5-174" visibility="hidden" gadfly:scale="1.0"/>
      <path fill="none" d="M-43.4,5 L -43.4 80.72" id="img-32380ac5-175" visibility="hidden" gadfly:scale="1.0"/>
      <path fill="none" d="M-15.19,5 L -15.19 80.72" id="img-32380ac5-176" visibility="hidden" gadfly:scale="1.0"/>
      <path fill="none" d="M13.02,5 L 13.02 80.72" id="img-32380ac5-177" visibility="visible" gadfly:scale="1.0"/>
      <path fill="none" d="M41.23,5 L 41.23 80.72" id="img-32380ac5-178" visibility="visible" gadfly:scale="1.0"/>
      <path fill="none" d="M69.44,5 L 69.44 80.72" id="img-32380ac5-179" visibility="visible" gadfly:scale="1.0"/>
      <path fill="none" d="M97.66,5 L 97.66 80.72" id="img-32380ac5-180" visibility="visible" gadfly:scale="1.0"/>
      <path fill="none" d="M125.87,5 L 125.87 80.72" id="img-32380ac5-181" visibility="visible" gadfly:scale="1.0"/>
      <path fill="none" d="M154.08,5 L 154.08 80.72" id="img-32380ac5-182" visibility="hidden" gadfly:scale="1.0"/>
      <path fill="none" d="M182.29,5 L 182.29 80.72" id="img-32380ac5-183" visibility="hidden" gadfly:scale="1.0"/>
      <path fill="none" d="M210.51,5 L 210.51 80.72" id="img-32380ac5-184" visibility="hidden" gadfly:scale="1.0"/>
      <path fill="none" d="M238.72,5 L 238.72 80.72" id="img-32380ac5-185" visibility="hidden" gadfly:scale="1.0"/>
      <path fill="none" d="M266.93,5 L 266.93 80.72" id="img-32380ac5-186" visibility="hidden" gadfly:scale="1.0"/>
      <path fill="none" d="M-99.83,5 L -99.83 80.72" id="img-32380ac5-187" visibility="hidden" gadfly:scale="10.0"/>
      <path fill="none" d="M-92.78,5 L -92.78 80.72" id="img-32380ac5-188" visibility="hidden" gadfly:scale="10.0"/>
      <path fill="none" d="M-85.72,5 L -85.72 80.72" id="img-32380ac5-189" visibility="hidden" gadfly:scale="10.0"/>
      <path fill="none" d="M-78.67,5 L -78.67 80.72" id="img-32380ac5-190" visibility="hidden" gadfly:scale="10.0"/>
      <path fill="none" d="M-71.62,5 L -71.62 80.72" id="img-32380ac5-191" visibility="hidden" gadfly:scale="10.0"/>
      <path fill="none" d="M-64.56,5 L -64.56 80.72" id="img-32380ac5-192" visibility="hidden" gadfly:scale="10.0"/>
      <path fill="none" d="M-57.51,5 L -57.51 80.72" id="img-32380ac5-193" visibility="hidden" gadfly:scale="10.0"/>
      <path fill="none" d="M-50.46,5 L -50.46 80.72" id="img-32380ac5-194" visibility="hidden" gadfly:scale="10.0"/>
      <path fill="none" d="M-43.4,5 L -43.4 80.72" id="img-32380ac5-195" visibility="hidden" gadfly:scale="10.0"/>
      <path fill="none" d="M-36.35,5 L -36.35 80.72" id="img-32380ac5-196" visibility="hidden" gadfly:scale="10.0"/>
      <path fill="none" d="M-29.3,5 L -29.3 80.72" id="img-32380ac5-197" visibility="hidden" gadfly:scale="10.0"/>
      <path fill="none" d="M-22.25,5 L -22.25 80.72" id="img-32380ac5-198" visibility="hidden" gadfly:scale="10.0"/>
      <path fill="none" d="M-15.19,5 L -15.19 80.72" id="img-32380ac5-199" visibility="hidden" gadfly:scale="10.0"/>
      <path fill="none" d="M-8.14,5 L -8.14 80.72" id="img-32380ac5-200" visibility="hidden" gadfly:scale="10.0"/>
      <path fill="none" d="M-1.09,5 L -1.09 80.72" id="img-32380ac5-201" visibility="hidden" gadfly:scale="10.0"/>
      <path fill="none" d="M5.97,5 L 5.97 80.72" id="img-32380ac5-202" visibility="hidden" gadfly:scale="10.0"/>
      <path fill="none" d="M13.02,5 L 13.02 80.72" id="img-32380ac5-203" visibility="hidden" gadfly:scale="10.0"/>
      <path fill="none" d="M20.07,5 L 20.07 80.72" id="img-32380ac5-204" visibility="hidden" gadfly:scale="10.0"/>
      <path fill="none" d="M27.13,5 L 27.13 80.72" id="img-32380ac5-205" visibility="hidden" gadfly:scale="10.0"/>
      <path fill="none" d="M34.18,5 L 34.18 80.72" id="img-32380ac5-206" visibility="hidden" gadfly:scale="10.0"/>
      <path fill="none" d="M41.23,5 L 41.23 80.72" id="img-32380ac5-207" visibility="hidden" gadfly:scale="10.0"/>
      <path fill="none" d="M48.29,5 L 48.29 80.72" id="img-32380ac5-208" visibility="hidden" gadfly:scale="10.0"/>
      <path fill="none" d="M55.34,5 L 55.34 80.72" id="img-32380ac5-209" visibility="hidden" gadfly:scale="10.0"/>
      <path fill="none" d="M62.39,5 L 62.39 80.72" id="img-32380ac5-210" visibility="hidden" gadfly:scale="10.0"/>
      <path fill="none" d="M69.44,5 L 69.44 80.72" id="img-32380ac5-211" visibility="hidden" gadfly:scale="10.0"/>
      <path fill="none" d="M76.5,5 L 76.5 80.72" id="img-32380ac5-212" visibility="hidden" gadfly:scale="10.0"/>
      <path fill="none" d="M83.55,5 L 83.55 80.72" id="img-32380ac5-213" visibility="hidden" gadfly:scale="10.0"/>
      <path fill="none" d="M90.6,5 L 90.6 80.72" id="img-32380ac5-214" visibility="hidden" gadfly:scale="10.0"/>
      <path fill="none" d="M97.66,5 L 97.66 80.72" id="img-32380ac5-215" visibility="hidden" gadfly:scale="10.0"/>
      <path fill="none" d="M104.71,5 L 104.71 80.72" id="img-32380ac5-216" visibility="hidden" gadfly:scale="10.0"/>
      <path fill="none" d="M111.76,5 L 111.76 80.72" id="img-32380ac5-217" visibility="hidden" gadfly:scale="10.0"/>
      <path fill="none" d="M118.82,5 L 118.82 80.72" id="img-32380ac5-218" visibility="hidden" gadfly:scale="10.0"/>
      <path fill="none" d="M125.87,5 L 125.87 80.72" id="img-32380ac5-219" visibility="hidden" gadfly:scale="10.0"/>
      <path fill="none" d="M132.92,5 L 132.92 80.72" id="img-32380ac5-220" visibility="hidden" gadfly:scale="10.0"/>
      <path fill="none" d="M139.97,5 L 139.97 80.72" id="img-32380ac5-221" visibility="hidden" gadfly:scale="10.0"/>
      <path fill="none" d="M147.03,5 L 147.03 80.72" id="img-32380ac5-222" visibility="hidden" gadfly:scale="10.0"/>
      <path fill="none" d="M154.08,5 L 154.08 80.72" id="img-32380ac5-223" visibility="hidden" gadfly:scale="10.0"/>
      <path fill="none" d="M161.13,5 L 161.13 80.72" id="img-32380ac5-224" visibility="hidden" gadfly:scale="10.0"/>
      <path fill="none" d="M168.19,5 L 168.19 80.72" id="img-32380ac5-225" visibility="hidden" gadfly:scale="10.0"/>
      <path fill="none" d="M175.24,5 L 175.24 80.72" id="img-32380ac5-226" visibility="hidden" gadfly:scale="10.0"/>
      <path fill="none" d="M182.29,5 L 182.29 80.72" id="img-32380ac5-227" visibility="hidden" gadfly:scale="10.0"/>
      <path fill="none" d="M189.35,5 L 189.35 80.72" id="img-32380ac5-228" visibility="hidden" gadfly:scale="10.0"/>
      <path fill="none" d="M196.4,5 L 196.4 80.72" id="img-32380ac5-229" visibility="hidden" gadfly:scale="10.0"/>
      <path fill="none" d="M203.45,5 L 203.45 80.72" id="img-32380ac5-230" visibility="hidden" gadfly:scale="10.0"/>
      <path fill="none" d="M210.51,5 L 210.51 80.72" id="img-32380ac5-231" visibility="hidden" gadfly:scale="10.0"/>
      <path fill="none" d="M217.56,5 L 217.56 80.72" id="img-32380ac5-232" visibility="hidden" gadfly:scale="10.0"/>
      <path fill="none" d="M224.61,5 L 224.61 80.72" id="img-32380ac5-233" visibility="hidden" gadfly:scale="10.0"/>
      <path fill="none" d="M231.66,5 L 231.66 80.72" id="img-32380ac5-234" visibility="hidden" gadfly:scale="10.0"/>
      <path fill="none" d="M238.72,5 L 238.72 80.72" id="img-32380ac5-235" visibility="hidden" gadfly:scale="10.0"/>
      <path fill="none" d="M-128.04,5 L -128.04 80.72" id="img-32380ac5-236" visibility="hidden" gadfly:scale="0.5"/>
      <path fill="none" d="M13.02,5 L 13.02 80.72" id="img-32380ac5-237" visibility="hidden" gadfly:scale="0.5"/>
      <path fill="none" d="M154.08,5 L 154.08 80.72" id="img-32380ac5-238" visibility="hidden" gadfly:scale="0.5"/>
      <path fill="none" d="M295.14,5 L 295.14 80.72" id="img-32380ac5-239" visibility="hidden" gadfly:scale="0.5"/>
      <path fill="none" d="M-99.83,5 L -99.83 80.72" id="img-32380ac5-240" visibility="hidden" gadfly:scale="5.0"/>
      <path fill="none" d="M-92.78,5 L -92.78 80.72" id="img-32380ac5-241" visibility="hidden" gadfly:scale="5.0"/>
      <path fill="none" d="M-85.72,5 L -85.72 80.72" id="img-32380ac5-242" visibility="hidden" gadfly:scale="5.0"/>
      <path fill="none" d="M-78.67,5 L -78.67 80.72" id="img-32380ac5-243" visibility="hidden" gadfly:scale="5.0"/>
      <path fill="none" d="M-71.62,5 L -71.62 80.72" id="img-32380ac5-244" visibility="hidden" gadfly:scale="5.0"/>
      <path fill="none" d="M-64.56,5 L -64.56 80.72" id="img-32380ac5-245" visibility="hidden" gadfly:scale="5.0"/>
      <path fill="none" d="M-57.51,5 L -57.51 80.72" id="img-32380ac5-246" visibility="hidden" gadfly:scale="5.0"/>
      <path fill="none" d="M-50.46,5 L -50.46 80.72" id="img-32380ac5-247" visibility="hidden" gadfly:scale="5.0"/>
      <path fill="none" d="M-43.4,5 L -43.4 80.72" id="img-32380ac5-248" visibility="hidden" gadfly:scale="5.0"/>
      <path fill="none" d="M-36.35,5 L -36.35 80.72" id="img-32380ac5-249" visibility="hidden" gadfly:scale="5.0"/>
      <path fill="none" d="M-29.3,5 L -29.3 80.72" id="img-32380ac5-250" visibility="hidden" gadfly:scale="5.0"/>
      <path fill="none" d="M-22.25,5 L -22.25 80.72" id="img-32380ac5-251" visibility="hidden" gadfly:scale="5.0"/>
      <path fill="none" d="M-15.19,5 L -15.19 80.72" id="img-32380ac5-252" visibility="hidden" gadfly:scale="5.0"/>
      <path fill="none" d="M-8.14,5 L -8.14 80.72" id="img-32380ac5-253" visibility="hidden" gadfly:scale="5.0"/>
      <path fill="none" d="M-1.09,5 L -1.09 80.72" id="img-32380ac5-254" visibility="hidden" gadfly:scale="5.0"/>
      <path fill="none" d="M5.97,5 L 5.97 80.72" id="img-32380ac5-255" visibility="hidden" gadfly:scale="5.0"/>
      <path fill="none" d="M13.02,5 L 13.02 80.72" id="img-32380ac5-256" visibility="hidden" gadfly:scale="5.0"/>
      <path fill="none" d="M20.07,5 L 20.07 80.72" id="img-32380ac5-257" visibility="hidden" gadfly:scale="5.0"/>
      <path fill="none" d="M27.13,5 L 27.13 80.72" id="img-32380ac5-258" visibility="hidden" gadfly:scale="5.0"/>
      <path fill="none" d="M34.18,5 L 34.18 80.72" id="img-32380ac5-259" visibility="hidden" gadfly:scale="5.0"/>
      <path fill="none" d="M41.23,5 L 41.23 80.72" id="img-32380ac5-260" visibility="hidden" gadfly:scale="5.0"/>
      <path fill="none" d="M48.29,5 L 48.29 80.72" id="img-32380ac5-261" visibility="hidden" gadfly:scale="5.0"/>
      <path fill="none" d="M55.34,5 L 55.34 80.72" id="img-32380ac5-262" visibility="hidden" gadfly:scale="5.0"/>
      <path fill="none" d="M62.39,5 L 62.39 80.72" id="img-32380ac5-263" visibility="hidden" gadfly:scale="5.0"/>
      <path fill="none" d="M69.44,5 L 69.44 80.72" id="img-32380ac5-264" visibility="hidden" gadfly:scale="5.0"/>
      <path fill="none" d="M76.5,5 L 76.5 80.72" id="img-32380ac5-265" visibility="hidden" gadfly:scale="5.0"/>
      <path fill="none" d="M83.55,5 L 83.55 80.72" id="img-32380ac5-266" visibility="hidden" gadfly:scale="5.0"/>
      <path fill="none" d="M90.6,5 L 90.6 80.72" id="img-32380ac5-267" visibility="hidden" gadfly:scale="5.0"/>
      <path fill="none" d="M97.66,5 L 97.66 80.72" id="img-32380ac5-268" visibility="hidden" gadfly:scale="5.0"/>
      <path fill="none" d="M104.71,5 L 104.71 80.72" id="img-32380ac5-269" visibility="hidden" gadfly:scale="5.0"/>
      <path fill="none" d="M111.76,5 L 111.76 80.72" id="img-32380ac5-270" visibility="hidden" gadfly:scale="5.0"/>
      <path fill="none" d="M118.82,5 L 118.82 80.72" id="img-32380ac5-271" visibility="hidden" gadfly:scale="5.0"/>
      <path fill="none" d="M125.87,5 L 125.87 80.72" id="img-32380ac5-272" visibility="hidden" gadfly:scale="5.0"/>
      <path fill="none" d="M132.92,5 L 132.92 80.72" id="img-32380ac5-273" visibility="hidden" gadfly:scale="5.0"/>
      <path fill="none" d="M139.97,5 L 139.97 80.72" id="img-32380ac5-274" visibility="hidden" gadfly:scale="5.0"/>
      <path fill="none" d="M147.03,5 L 147.03 80.72" id="img-32380ac5-275" visibility="hidden" gadfly:scale="5.0"/>
      <path fill="none" d="M154.08,5 L 154.08 80.72" id="img-32380ac5-276" visibility="hidden" gadfly:scale="5.0"/>
      <path fill="none" d="M161.13,5 L 161.13 80.72" id="img-32380ac5-277" visibility="hidden" gadfly:scale="5.0"/>
      <path fill="none" d="M168.19,5 L 168.19 80.72" id="img-32380ac5-278" visibility="hidden" gadfly:scale="5.0"/>
      <path fill="none" d="M175.24,5 L 175.24 80.72" id="img-32380ac5-279" visibility="hidden" gadfly:scale="5.0"/>
      <path fill="none" d="M182.29,5 L 182.29 80.72" id="img-32380ac5-280" visibility="hidden" gadfly:scale="5.0"/>
      <path fill="none" d="M189.35,5 L 189.35 80.72" id="img-32380ac5-281" visibility="hidden" gadfly:scale="5.0"/>
      <path fill="none" d="M196.4,5 L 196.4 80.72" id="img-32380ac5-282" visibility="hidden" gadfly:scale="5.0"/>
      <path fill="none" d="M203.45,5 L 203.45 80.72" id="img-32380ac5-283" visibility="hidden" gadfly:scale="5.0"/>
      <path fill="none" d="M210.51,5 L 210.51 80.72" id="img-32380ac5-284" visibility="hidden" gadfly:scale="5.0"/>
      <path fill="none" d="M217.56,5 L 217.56 80.72" id="img-32380ac5-285" visibility="hidden" gadfly:scale="5.0"/>
      <path fill="none" d="M224.61,5 L 224.61 80.72" id="img-32380ac5-286" visibility="hidden" gadfly:scale="5.0"/>
      <path fill="none" d="M231.66,5 L 231.66 80.72" id="img-32380ac5-287" visibility="hidden" gadfly:scale="5.0"/>
      <path fill="none" d="M238.72,5 L 238.72 80.72" id="img-32380ac5-288" visibility="hidden" gadfly:scale="5.0"/>
    </g>
    <g class="plotpanel" id="img-32380ac5-289">
      <g shape-rendering="crispEdges" stroke-width="0.3" id="img-32380ac5-290">
        <g stroke="#000000" stroke-opacity="0.000" class="geometry" id="img-32380ac5-291">
          <rect x="22.87" y="71.23" width="4.28" height="7.48" id="img-32380ac5-292" fill="#00BFFF"/>
          <rect x="27.1" y="72.48" width="4.28" height="6.24" id="img-32380ac5-293" fill="#00BFFF"/>
          <rect x="31.33" y="57.51" width="4.28" height="21.2" id="img-32380ac5-294" fill="#00BFFF"/>
          <rect x="35.56" y="55.02" width="4.28" height="23.7" id="img-32380ac5-295" fill="#00BFFF"/>
          <rect x="39.8" y="56.27" width="4.28" height="22.45" id="img-32380ac5-296" fill="#00BFFF"/>
          <rect x="44.03" y="65" width="4.28" height="13.72" id="img-32380ac5-297" fill="#00BFFF"/>
          <rect x="48.26" y="67.49" width="4.28" height="11.22" id="img-32380ac5-298" fill="#00BFFF"/>
          <rect x="52.49" y="66.24" width="4.28" height="12.47" id="img-32380ac5-299" fill="#00BFFF"/>
          <rect x="56.72" y="72.48" width="4.28" height="6.24" id="img-32380ac5-300" fill="#00BFFF"/>
          <rect x="60.96" y="72.48" width="4.28" height="6.24" id="img-32380ac5-301" fill="#00BFFF"/>
          <rect x="65.19" y="72.48" width="4.28" height="6.24" id="img-32380ac5-302" fill="#00BFFF"/>
          <rect x="69.42" y="77.47" width="4.28" height="1.25" id="img-32380ac5-303" fill="#00BFFF"/>
          <rect x="73.65" y="77.47" width="4.28" height="1.25" id="img-32380ac5-304" fill="#00BFFF"/>
          <rect x="77.88" y="74.97" width="4.28" height="3.74" id="img-32380ac5-305" fill="#00BFFF"/>
          <rect x="82.11" y="NaN" width="4.28" height="0.01" id="img-32380ac5-306" fill="#00BFFF"/>
          <rect x="86.35" y="NaN" width="4.28" height="0.01" id="img-32380ac5-307" fill="#00BFFF"/>
          <rect x="90.58" y="NaN" width="4.28" height="0.01" id="img-32380ac5-308" fill="#00BFFF"/>
          <rect x="94.81" y="NaN" width="4.28" height="0.01" id="img-32380ac5-309" fill="#00BFFF"/>
          <rect x="99.04" y="NaN" width="4.28" height="0.01" id="img-32380ac5-310" fill="#00BFFF"/>
          <rect x="103.27" y="NaN" width="4.28" height="0.01" id="img-32380ac5-311" fill="#00BFFF"/>
          <rect x="22.87" y="56.68" width="4.28" height="14.55" id="img-32380ac5-312" fill="#D4CA3A"/>
          <rect x="27.1" y="51.69" width="4.28" height="20.79" id="img-32380ac5-313" fill="#D4CA3A"/>
          <rect x="31.33" y="28.41" width="4.28" height="29.1" id="img-32380ac5-314" fill="#D4CA3A"/>
          <rect x="35.56" y="34.23" width="4.28" height="20.79" id="img-32380ac5-315" fill="#D4CA3A"/>
          <rect x="39.8" y="37.56" width="4.28" height="18.71" id="img-32380ac5-316" fill="#D4CA3A"/>
          <rect x="44.03" y="44.21" width="4.28" height="20.79" id="img-32380ac5-317" fill="#D4CA3A"/>
          <rect x="48.26" y="61.25" width="4.28" height="6.24" id="img-32380ac5-318" fill="#D4CA3A"/>
          <rect x="52.49" y="62.09" width="4.28" height="4.16" id="img-32380ac5-319" fill="#D4CA3A"/>
          <rect x="56.72" y="70.4" width="4.28" height="2.08" id="img-32380ac5-320" fill="#D4CA3A"/>
          <rect x="60.96" y="72.48" width="4.28" height="0.01" id="img-32380ac5-321" fill="#D4CA3A"/>
          <rect x="65.19" y="72.48" width="4.28" height="0.01" id="img-32380ac5-322" fill="#D4CA3A"/>
          <rect x="69.42" y="71.23" width="4.28" height="6.24" id="img-32380ac5-323" fill="#D4CA3A"/>
          <rect x="73.65" y="77.47" width="4.28" height="0.01" id="img-32380ac5-324" fill="#D4CA3A"/>
          <rect x="77.88" y="74.97" width="4.28" height="0.01" id="img-32380ac5-325" fill="#D4CA3A"/>
          <rect x="82.11" y="NaN" width="4.28" height="0.01" id="img-32380ac5-326" fill="#D4CA3A"/>
          <rect x="86.35" y="NaN" width="4.28" height="0.01" id="img-32380ac5-327" fill="#D4CA3A"/>
          <rect x="90.58" y="NaN" width="4.28" height="0.01" id="img-32380ac5-328" fill="#D4CA3A"/>
          <rect x="94.81" y="NaN" width="4.28" height="0.01" id="img-32380ac5-329" fill="#D4CA3A"/>
          <rect x="99.04" y="NaN" width="4.28" height="0.01" id="img-32380ac5-330" fill="#D4CA3A"/>
          <rect x="103.27" y="NaN" width="4.28" height="0.01" id="img-32380ac5-331" fill="#D4CA3A"/>
        </g>
      </g>
    </g>
    <g opacity="0" class="guide zoomslider" stroke="#000000" stroke-opacity="0.000" id="img-32380ac5-332">
      <g fill="#EAEAEA" stroke-width="0.3" stroke-opacity="0" stroke="#6A6A6A" id="img-32380ac5-333">
        <rect x="120.87" y="8" width="4" height="4" id="img-32380ac5-334"/>
        <g class="button_logo" fill="#6A6A6A" id="img-32380ac5-335">
          <path d="M121.67,9.6 L 122.47 9.6 122.47 8.8 123.27 8.8 123.27 9.6 124.07 9.6 124.07 10.4 123.27 10.4 123.27 11.2 122.47 11.2 122.47 10.4 121.67 10.4 z" id="img-32380ac5-336"/>
        </g>
      </g>
      <g fill="#EAEAEA" id="img-32380ac5-337">
        <rect x="101.37" y="8" width="19" height="4" id="img-32380ac5-338"/>
      </g>
      <g class="zoomslider_thumb" fill="#6A6A6A" id="img-32380ac5-339">
        <rect x="109.87" y="8" width="2" height="4" id="img-32380ac5-340"/>
      </g>
      <g fill="#EAEAEA" stroke-width="0.3" stroke-opacity="0" stroke="#6A6A6A" id="img-32380ac5-341">
        <rect x="96.87" y="8" width="4" height="4" id="img-32380ac5-342"/>
        <g class="button_logo" fill="#6A6A6A" id="img-32380ac5-343">
          <path d="M97.67,9.6 L 100.07 9.6 100.07 10.4 97.67 10.4 z" id="img-32380ac5-344"/>
        </g>
      </g>
    </g>
  </g>
</g>
  <g class="guide ylabels" font-size="2.82" font-family="'PT Sans Caption','Helvetica Neue','Helvetica',sans-serif" fill="#6C606B" id="img-32380ac5-345">
    <text x="10.02" y="174.33" text-anchor="end" dy="0.35em" id="img-32380ac5-346" visibility="hidden" gadfly:scale="1.0">-2.0</text>
    <text x="10.02" y="150.43" text-anchor="end" dy="0.35em" id="img-32380ac5-347" visibility="hidden" gadfly:scale="1.0">-1.5</text>
    <text x="10.02" y="126.52" text-anchor="end" dy="0.35em" id="img-32380ac5-348" visibility="hidden" gadfly:scale="1.0">-1.0</text>
    <text x="10.02" y="102.62" text-anchor="end" dy="0.35em" id="img-32380ac5-349" visibility="hidden" gadfly:scale="1.0">-0.5</text>
    <text x="10.02" y="78.71" text-anchor="end" dy="0.35em" id="img-32380ac5-350" visibility="visible" gadfly:scale="1.0">0.0</text>
    <text x="10.02" y="54.81" text-anchor="end" dy="0.35em" id="img-32380ac5-351" visibility="visible" gadfly:scale="1.0">0.5</text>
    <text x="10.02" y="30.9" text-anchor="end" dy="0.35em" id="img-32380ac5-352" visibility="visible" gadfly:scale="1.0">1.0</text>
    <text x="10.02" y="7" text-anchor="end" dy="0.35em" id="img-32380ac5-353" visibility="visible" gadfly:scale="1.0">1.5</text>
    <text x="10.02" y="-16.91" text-anchor="end" dy="0.35em" id="img-32380ac5-354" visibility="hidden" gadfly:scale="1.0">2.0</text>
    <text x="10.02" y="-40.81" text-anchor="end" dy="0.35em" id="img-32380ac5-355" visibility="hidden" gadfly:scale="1.0">2.5</text>
    <text x="10.02" y="-64.71" text-anchor="end" dy="0.35em" id="img-32380ac5-356" visibility="hidden" gadfly:scale="1.0">3.0</text>
    <text x="10.02" y="-88.62" text-anchor="end" dy="0.35em" id="img-32380ac5-357" visibility="hidden" gadfly:scale="1.0">3.5</text>
    <text x="10.02" y="150.43" text-anchor="end" dy="0.35em" id="img-32380ac5-358" visibility="hidden" gadfly:scale="10.0">-1.50</text>
    <text x="10.02" y="148.04" text-anchor="end" dy="0.35em" id="img-32380ac5-359" visibility="hidden" gadfly:scale="10.0">-1.45</text>
    <text x="10.02" y="145.65" text-anchor="end" dy="0.35em" id="img-32380ac5-360" visibility="hidden" gadfly:scale="10.0">-1.40</text>
    <text x="10.02" y="143.26" text-anchor="end" dy="0.35em" id="img-32380ac5-361" visibility="hidden" gadfly:scale="10.0">-1.35</text>
    <text x="10.02" y="140.87" text-anchor="end" dy="0.35em" id="img-32380ac5-362" visibility="hidden" gadfly:scale="10.0">-1.30</text>
    <text x="10.02" y="138.48" text-anchor="end" dy="0.35em" id="img-32380ac5-363" visibility="hidden" gadfly:scale="10.0">-1.25</text>
    <text x="10.02" y="136.09" text-anchor="end" dy="0.35em" id="img-32380ac5-364" visibility="hidden" gadfly:scale="10.0">-1.20</text>
    <text x="10.02" y="133.7" text-anchor="end" dy="0.35em" id="img-32380ac5-365" visibility="hidden" gadfly:scale="10.0">-1.15</text>
    <text x="10.02" y="131.31" text-anchor="end" dy="0.35em" id="img-32380ac5-366" visibility="hidden" gadfly:scale="10.0">-1.10</text>
    <text x="10.02" y="128.92" text-anchor="end" dy="0.35em" id="img-32380ac5-367" visibility="hidden" gadfly:scale="10.0">-1.05</text>
    <text x="10.02" y="126.52" text-anchor="end" dy="0.35em" id="img-32380ac5-368" visibility="hidden" gadfly:scale="10.0">-1.00</text>
    <text x="10.02" y="124.13" text-anchor="end" dy="0.35em" id="img-32380ac5-369" visibility="hidden" gadfly:scale="10.0">-0.95</text>
    <text x="10.02" y="121.74" text-anchor="end" dy="0.35em" id="img-32380ac5-370" visibility="hidden" gadfly:scale="10.0">-0.90</text>
    <text x="10.02" y="119.35" text-anchor="end" dy="0.35em" id="img-32380ac5-371" visibility="hidden" gadfly:scale="10.0">-0.85</text>
    <text x="10.02" y="116.96" text-anchor="end" dy="0.35em" id="img-32380ac5-372" visibility="hidden" gadfly:scale="10.0">-0.80</text>
    <text x="10.02" y="114.57" text-anchor="end" dy="0.35em" id="img-32380ac5-373" visibility="hidden" gadfly:scale="10.0">-0.75</text>
    <text x="10.02" y="112.18" text-anchor="end" dy="0.35em" id="img-32380ac5-374" visibility="hidden" gadfly:scale="10.0">-0.70</text>
    <text x="10.02" y="109.79" text-anchor="end" dy="0.35em" id="img-32380ac5-375" visibility="hidden" gadfly:scale="10.0">-0.65</text>
    <text x="10.02" y="107.4" text-anchor="end" dy="0.35em" id="img-32380ac5-376" visibility="hidden" gadfly:scale="10.0">-0.60</text>
    <text x="10.02" y="105.01" text-anchor="end" dy="0.35em" id="img-32380ac5-377" visibility="hidden" gadfly:scale="10.0">-0.55</text>
    <text x="10.02" y="102.62" text-anchor="end" dy="0.35em" id="img-32380ac5-378" visibility="hidden" gadfly:scale="10.0">-0.50</text>
    <text x="10.02" y="100.23" text-anchor="end" dy="0.35em" id="img-32380ac5-379" visibility="hidden" gadfly:scale="10.0">-0.45</text>
    <text x="10.02" y="97.84" text-anchor="end" dy="0.35em" id="img-32380ac5-380" visibility="hidden" gadfly:scale="10.0">-0.40</text>
    <text x="10.02" y="95.45" text-anchor="end" dy="0.35em" id="img-32380ac5-381" visibility="hidden" gadfly:scale="10.0">-0.35</text>
    <text x="10.02" y="93.06" text-anchor="end" dy="0.35em" id="img-32380ac5-382" visibility="hidden" gadfly:scale="10.0">-0.30</text>
    <text x="10.02" y="90.67" text-anchor="end" dy="0.35em" id="img-32380ac5-383" visibility="hidden" gadfly:scale="10.0">-0.25</text>
    <text x="10.02" y="88.28" text-anchor="end" dy="0.35em" id="img-32380ac5-384" visibility="hidden" gadfly:scale="10.0">-0.20</text>
    <text x="10.02" y="85.89" text-anchor="end" dy="0.35em" id="img-32380ac5-385" visibility="hidden" gadfly:scale="10.0">-0.15</text>
    <text x="10.02" y="83.5" text-anchor="end" dy="0.35em" id="img-32380ac5-386" visibility="hidden" gadfly:scale="10.0">-0.10</text>
    <text x="10.02" y="81.11" text-anchor="end" dy="0.35em" id="img-32380ac5-387" visibility="hidden" gadfly:scale="10.0">-0.05</text>
    <text x="10.02" y="78.71" text-anchor="end" dy="0.35em" id="img-32380ac5-388" visibility="hidden" gadfly:scale="10.0">0.00</text>
    <text x="10.02" y="76.32" text-anchor="end" dy="0.35em" id="img-32380ac5-389" visibility="hidden" gadfly:scale="10.0">0.05</text>
    <text x="10.02" y="73.93" text-anchor="end" dy="0.35em" id="img-32380ac5-390" visibility="hidden" gadfly:scale="10.0">0.10</text>
    <text x="10.02" y="71.54" text-anchor="end" dy="0.35em" id="img-32380ac5-391" visibility="hidden" gadfly:scale="10.0">0.15</text>
    <text x="10.02" y="69.15" text-anchor="end" dy="0.35em" id="img-32380ac5-392" visibility="hidden" gadfly:scale="10.0">0.20</text>
    <text x="10.02" y="66.76" text-anchor="end" dy="0.35em" id="img-32380ac5-393" visibility="hidden" gadfly:scale="10.0">0.25</text>
    <text x="10.02" y="64.37" text-anchor="end" dy="0.35em" id="img-32380ac5-394" visibility="hidden" gadfly:scale="10.0">0.30</text>
    <text x="10.02" y="61.98" text-anchor="end" dy="0.35em" id="img-32380ac5-395" visibility="hidden" gadfly:scale="10.0">0.35</text>
    <text x="10.02" y="59.59" text-anchor="end" dy="0.35em" id="img-32380ac5-396" visibility="hidden" gadfly:scale="10.0">0.40</text>
    <text x="10.02" y="57.2" text-anchor="end" dy="0.35em" id="img-32380ac5-397" visibility="hidden" gadfly:scale="10.0">0.45</text>
    <text x="10.02" y="54.81" text-anchor="end" dy="0.35em" id="img-32380ac5-398" visibility="hidden" gadfly:scale="10.0">0.50</text>
    <text x="10.02" y="52.42" text-anchor="end" dy="0.35em" id="img-32380ac5-399" visibility="hidden" gadfly:scale="10.0">0.55</text>
    <text x="10.02" y="50.03" text-anchor="end" dy="0.35em" id="img-32380ac5-400" visibility="hidden" gadfly:scale="10.0">0.60</text>
    <text x="10.02" y="47.64" text-anchor="end" dy="0.35em" id="img-32380ac5-401" visibility="hidden" gadfly:scale="10.0">0.65</text>
    <text x="10.02" y="45.25" text-anchor="end" dy="0.35em" id="img-32380ac5-402" visibility="hidden" gadfly:scale="10.0">0.70</text>
    <text x="10.02" y="42.86" text-anchor="end" dy="0.35em" id="img-32380ac5-403" visibility="hidden" gadfly:scale="10.0">0.75</text>
    <text x="10.02" y="40.47" text-anchor="end" dy="0.35em" id="img-32380ac5-404" visibility="hidden" gadfly:scale="10.0">0.80</text>
    <text x="10.02" y="38.08" text-anchor="end" dy="0.35em" id="img-32380ac5-405" visibility="hidden" gadfly:scale="10.0">0.85</text>
    <text x="10.02" y="35.69" text-anchor="end" dy="0.35em" id="img-32380ac5-406" visibility="hidden" gadfly:scale="10.0">0.90</text>
    <text x="10.02" y="33.3" text-anchor="end" dy="0.35em" id="img-32380ac5-407" visibility="hidden" gadfly:scale="10.0">0.95</text>
    <text x="10.02" y="30.9" text-anchor="end" dy="0.35em" id="img-32380ac5-408" visibility="hidden" gadfly:scale="10.0">1.00</text>
    <text x="10.02" y="28.51" text-anchor="end" dy="0.35em" id="img-32380ac5-409" visibility="hidden" gadfly:scale="10.0">1.05</text>
    <text x="10.02" y="26.12" text-anchor="end" dy="0.35em" id="img-32380ac5-410" visibility="hidden" gadfly:scale="10.0">1.10</text>
    <text x="10.02" y="23.73" text-anchor="end" dy="0.35em" id="img-32380ac5-411" visibility="hidden" gadfly:scale="10.0">1.15</text>
    <text x="10.02" y="21.34" text-anchor="end" dy="0.35em" id="img-32380ac5-412" visibility="hidden" gadfly:scale="10.0">1.20</text>
    <text x="10.02" y="18.95" text-anchor="end" dy="0.35em" id="img-32380ac5-413" visibility="hidden" gadfly:scale="10.0">1.25</text>
    <text x="10.02" y="16.56" text-anchor="end" dy="0.35em" id="img-32380ac5-414" visibility="hidden" gadfly:scale="10.0">1.30</text>
    <text x="10.02" y="14.17" text-anchor="end" dy="0.35em" id="img-32380ac5-415" visibility="hidden" gadfly:scale="10.0">1.35</text>
    <text x="10.02" y="11.78" text-anchor="end" dy="0.35em" id="img-32380ac5-416" visibility="hidden" gadfly:scale="10.0">1.40</text>
    <text x="10.02" y="9.39" text-anchor="end" dy="0.35em" id="img-32380ac5-417" visibility="hidden" gadfly:scale="10.0">1.45</text>
    <text x="10.02" y="7" text-anchor="end" dy="0.35em" id="img-32380ac5-418" visibility="hidden" gadfly:scale="10.0">1.50</text>
    <text x="10.02" y="4.61" text-anchor="end" dy="0.35em" id="img-32380ac5-419" visibility="hidden" gadfly:scale="10.0">1.55</text>
    <text x="10.02" y="2.22" text-anchor="end" dy="0.35em" id="img-32380ac5-420" visibility="hidden" gadfly:scale="10.0">1.60</text>
    <text x="10.02" y="-0.17" text-anchor="end" dy="0.35em" id="img-32380ac5-421" visibility="hidden" gadfly:scale="10.0">1.65</text>
    <text x="10.02" y="-2.56" text-anchor="end" dy="0.35em" id="img-32380ac5-422" visibility="hidden" gadfly:scale="10.0">1.70</text>
    <text x="10.02" y="-4.95" text-anchor="end" dy="0.35em" id="img-32380ac5-423" visibility="hidden" gadfly:scale="10.0">1.75</text>
    <text x="10.02" y="-7.34" text-anchor="end" dy="0.35em" id="img-32380ac5-424" visibility="hidden" gadfly:scale="10.0">1.80</text>
    <text x="10.02" y="-9.73" text-anchor="end" dy="0.35em" id="img-32380ac5-425" visibility="hidden" gadfly:scale="10.0">1.85</text>
    <text x="10.02" y="-12.12" text-anchor="end" dy="0.35em" id="img-32380ac5-426" visibility="hidden" gadfly:scale="10.0">1.90</text>
    <text x="10.02" y="-14.51" text-anchor="end" dy="0.35em" id="img-32380ac5-427" visibility="hidden" gadfly:scale="10.0">1.95</text>
    <text x="10.02" y="-16.91" text-anchor="end" dy="0.35em" id="img-32380ac5-428" visibility="hidden" gadfly:scale="10.0">2.00</text>
    <text x="10.02" y="-19.3" text-anchor="end" dy="0.35em" id="img-32380ac5-429" visibility="hidden" gadfly:scale="10.0">2.05</text>
    <text x="10.02" y="-21.69" text-anchor="end" dy="0.35em" id="img-32380ac5-430" visibility="hidden" gadfly:scale="10.0">2.10</text>
    <text x="10.02" y="-24.08" text-anchor="end" dy="0.35em" id="img-32380ac5-431" visibility="hidden" gadfly:scale="10.0">2.15</text>
    <text x="10.02" y="-26.47" text-anchor="end" dy="0.35em" id="img-32380ac5-432" visibility="hidden" gadfly:scale="10.0">2.20</text>
    <text x="10.02" y="-28.86" text-anchor="end" dy="0.35em" id="img-32380ac5-433" visibility="hidden" gadfly:scale="10.0">2.25</text>
    <text x="10.02" y="-31.25" text-anchor="end" dy="0.35em" id="img-32380ac5-434" visibility="hidden" gadfly:scale="10.0">2.30</text>
    <text x="10.02" y="-33.64" text-anchor="end" dy="0.35em" id="img-32380ac5-435" visibility="hidden" gadfly:scale="10.0">2.35</text>
    <text x="10.02" y="-36.03" text-anchor="end" dy="0.35em" id="img-32380ac5-436" visibility="hidden" gadfly:scale="10.0">2.40</text>
    <text x="10.02" y="-38.42" text-anchor="end" dy="0.35em" id="img-32380ac5-437" visibility="hidden" gadfly:scale="10.0">2.45</text>
    <text x="10.02" y="-40.81" text-anchor="end" dy="0.35em" id="img-32380ac5-438" visibility="hidden" gadfly:scale="10.0">2.50</text>
    <text x="10.02" y="-43.2" text-anchor="end" dy="0.35em" id="img-32380ac5-439" visibility="hidden" gadfly:scale="10.0">2.55</text>
    <text x="10.02" y="-45.59" text-anchor="end" dy="0.35em" id="img-32380ac5-440" visibility="hidden" gadfly:scale="10.0">2.60</text>
    <text x="10.02" y="-47.98" text-anchor="end" dy="0.35em" id="img-32380ac5-441" visibility="hidden" gadfly:scale="10.0">2.65</text>
    <text x="10.02" y="-50.37" text-anchor="end" dy="0.35em" id="img-32380ac5-442" visibility="hidden" gadfly:scale="10.0">2.70</text>
    <text x="10.02" y="-52.76" text-anchor="end" dy="0.35em" id="img-32380ac5-443" visibility="hidden" gadfly:scale="10.0">2.75</text>
    <text x="10.02" y="-55.15" text-anchor="end" dy="0.35em" id="img-32380ac5-444" visibility="hidden" gadfly:scale="10.0">2.80</text>
    <text x="10.02" y="-57.54" text-anchor="end" dy="0.35em" id="img-32380ac5-445" visibility="hidden" gadfly:scale="10.0">2.85</text>
    <text x="10.02" y="-59.93" text-anchor="end" dy="0.35em" id="img-32380ac5-446" visibility="hidden" gadfly:scale="10.0">2.90</text>
    <text x="10.02" y="-62.32" text-anchor="end" dy="0.35em" id="img-32380ac5-447" visibility="hidden" gadfly:scale="10.0">2.95</text>
    <text x="10.02" y="-64.71" text-anchor="end" dy="0.35em" id="img-32380ac5-448" visibility="hidden" gadfly:scale="10.0">3.00</text>
    <text x="10.02" y="174.33" text-anchor="end" dy="0.35em" id="img-32380ac5-449" visibility="hidden" gadfly:scale="0.5">-2</text>
    <text x="10.02" y="78.71" text-anchor="end" dy="0.35em" id="img-32380ac5-450" visibility="hidden" gadfly:scale="0.5">0</text>
    <text x="10.02" y="-16.91" text-anchor="end" dy="0.35em" id="img-32380ac5-451" visibility="hidden" gadfly:scale="0.5">2</text>
    <text x="10.02" y="-112.52" text-anchor="end" dy="0.35em" id="img-32380ac5-452" visibility="hidden" gadfly:scale="0.5">4</text>
    <text x="10.02" y="150.43" text-anchor="end" dy="0.35em" id="img-32380ac5-453" visibility="hidden" gadfly:scale="5.0">-1.5</text>
    <text x="10.02" y="145.65" text-anchor="end" dy="0.35em" id="img-32380ac5-454" visibility="hidden" gadfly:scale="5.0">-1.4</text>
    <text x="10.02" y="140.87" text-anchor="end" dy="0.35em" id="img-32380ac5-455" visibility="hidden" gadfly:scale="5.0">-1.3</text>
    <text x="10.02" y="136.09" text-anchor="end" dy="0.35em" id="img-32380ac5-456" visibility="hidden" gadfly:scale="5.0">-1.2</text>
    <text x="10.02" y="131.31" text-anchor="end" dy="0.35em" id="img-32380ac5-457" visibility="hidden" gadfly:scale="5.0">-1.1</text>
    <text x="10.02" y="126.52" text-anchor="end" dy="0.35em" id="img-32380ac5-458" visibility="hidden" gadfly:scale="5.0">-1.0</text>
    <text x="10.02" y="121.74" text-anchor="end" dy="0.35em" id="img-32380ac5-459" visibility="hidden" gadfly:scale="5.0">-0.9</text>
    <text x="10.02" y="116.96" text-anchor="end" dy="0.35em" id="img-32380ac5-460" visibility="hidden" gadfly:scale="5.0">-0.8</text>
    <text x="10.02" y="112.18" text-anchor="end" dy="0.35em" id="img-32380ac5-461" visibility="hidden" gadfly:scale="5.0">-0.7</text>
    <text x="10.02" y="107.4" text-anchor="end" dy="0.35em" id="img-32380ac5-462" visibility="hidden" gadfly:scale="5.0">-0.6</text>
    <text x="10.02" y="102.62" text-anchor="end" dy="0.35em" id="img-32380ac5-463" visibility="hidden" gadfly:scale="5.0">-0.5</text>
    <text x="10.02" y="97.84" text-anchor="end" dy="0.35em" id="img-32380ac5-464" visibility="hidden" gadfly:scale="5.0">-0.4</text>
    <text x="10.02" y="93.06" text-anchor="end" dy="0.35em" id="img-32380ac5-465" visibility="hidden" gadfly:scale="5.0">-0.3</text>
    <text x="10.02" y="88.28" text-anchor="end" dy="0.35em" id="img-32380ac5-466" visibility="hidden" gadfly:scale="5.0">-0.2</text>
    <text x="10.02" y="83.5" text-anchor="end" dy="0.35em" id="img-32380ac5-467" visibility="hidden" gadfly:scale="5.0">-0.1</text>
    <text x="10.02" y="78.71" text-anchor="end" dy="0.35em" id="img-32380ac5-468" visibility="hidden" gadfly:scale="5.0">0.0</text>
    <text x="10.02" y="73.93" text-anchor="end" dy="0.35em" id="img-32380ac5-469" visibility="hidden" gadfly:scale="5.0">0.1</text>
    <text x="10.02" y="69.15" text-anchor="end" dy="0.35em" id="img-32380ac5-470" visibility="hidden" gadfly:scale="5.0">0.2</text>
    <text x="10.02" y="64.37" text-anchor="end" dy="0.35em" id="img-32380ac5-471" visibility="hidden" gadfly:scale="5.0">0.3</text>
    <text x="10.02" y="59.59" text-anchor="end" dy="0.35em" id="img-32380ac5-472" visibility="hidden" gadfly:scale="5.0">0.4</text>
    <text x="10.02" y="54.81" text-anchor="end" dy="0.35em" id="img-32380ac5-473" visibility="hidden" gadfly:scale="5.0">0.5</text>
    <text x="10.02" y="50.03" text-anchor="end" dy="0.35em" id="img-32380ac5-474" visibility="hidden" gadfly:scale="5.0">0.6</text>
    <text x="10.02" y="45.25" text-anchor="end" dy="0.35em" id="img-32380ac5-475" visibility="hidden" gadfly:scale="5.0">0.7</text>
    <text x="10.02" y="40.47" text-anchor="end" dy="0.35em" id="img-32380ac5-476" visibility="hidden" gadfly:scale="5.0">0.8</text>
    <text x="10.02" y="35.69" text-anchor="end" dy="0.35em" id="img-32380ac5-477" visibility="hidden" gadfly:scale="5.0">0.9</text>
    <text x="10.02" y="30.9" text-anchor="end" dy="0.35em" id="img-32380ac5-478" visibility="hidden" gadfly:scale="5.0">1.0</text>
    <text x="10.02" y="26.12" text-anchor="end" dy="0.35em" id="img-32380ac5-479" visibility="hidden" gadfly:scale="5.0">1.1</text>
    <text x="10.02" y="21.34" text-anchor="end" dy="0.35em" id="img-32380ac5-480" visibility="hidden" gadfly:scale="5.0">1.2</text>
    <text x="10.02" y="16.56" text-anchor="end" dy="0.35em" id="img-32380ac5-481" visibility="hidden" gadfly:scale="5.0">1.3</text>
    <text x="10.02" y="11.78" text-anchor="end" dy="0.35em" id="img-32380ac5-482" visibility="hidden" gadfly:scale="5.0">1.4</text>
    <text x="10.02" y="7" text-anchor="end" dy="0.35em" id="img-32380ac5-483" visibility="hidden" gadfly:scale="5.0">1.5</text>
    <text x="10.02" y="2.22" text-anchor="end" dy="0.35em" id="img-32380ac5-484" visibility="hidden" gadfly:scale="5.0">1.6</text>
    <text x="10.02" y="-2.56" text-anchor="end" dy="0.35em" id="img-32380ac5-485" visibility="hidden" gadfly:scale="5.0">1.7</text>
    <text x="10.02" y="-7.34" text-anchor="end" dy="0.35em" id="img-32380ac5-486" visibility="hidden" gadfly:scale="5.0">1.8</text>
    <text x="10.02" y="-12.12" text-anchor="end" dy="0.35em" id="img-32380ac5-487" visibility="hidden" gadfly:scale="5.0">1.9</text>
    <text x="10.02" y="-16.91" text-anchor="end" dy="0.35em" id="img-32380ac5-488" visibility="hidden" gadfly:scale="5.0">2.0</text>
    <text x="10.02" y="-21.69" text-anchor="end" dy="0.35em" id="img-32380ac5-489" visibility="hidden" gadfly:scale="5.0">2.1</text>
    <text x="10.02" y="-26.47" text-anchor="end" dy="0.35em" id="img-32380ac5-490" visibility="hidden" gadfly:scale="5.0">2.2</text>
    <text x="10.02" y="-31.25" text-anchor="end" dy="0.35em" id="img-32380ac5-491" visibility="hidden" gadfly:scale="5.0">2.3</text>
    <text x="10.02" y="-36.03" text-anchor="end" dy="0.35em" id="img-32380ac5-492" visibility="hidden" gadfly:scale="5.0">2.4</text>
    <text x="10.02" y="-40.81" text-anchor="end" dy="0.35em" id="img-32380ac5-493" visibility="hidden" gadfly:scale="5.0">2.5</text>
    <text x="10.02" y="-45.59" text-anchor="end" dy="0.35em" id="img-32380ac5-494" visibility="hidden" gadfly:scale="5.0">2.6</text>
    <text x="10.02" y="-50.37" text-anchor="end" dy="0.35em" id="img-32380ac5-495" visibility="hidden" gadfly:scale="5.0">2.7</text>
    <text x="10.02" y="-55.15" text-anchor="end" dy="0.35em" id="img-32380ac5-496" visibility="hidden" gadfly:scale="5.0">2.8</text>
    <text x="10.02" y="-59.93" text-anchor="end" dy="0.35em" id="img-32380ac5-497" visibility="hidden" gadfly:scale="5.0">2.9</text>
    <text x="10.02" y="-64.71" text-anchor="end" dy="0.35em" id="img-32380ac5-498" visibility="hidden" gadfly:scale="5.0">3.0</text>
  </g>
</g>
<defs>
  <clipPath id="img-32380ac5-13">
  <path d="M11.02,5 L 127.87 5 127.87 80.72 11.02 80.72" />
</clipPath>
</defs>
<script> <![CDATA[
(function(N){var k=/[\.\/]/,L=/\s*,\s*/,C=function(a,d){return a-d},a,v,y={n:{}},M=function(){for(var a=0,d=this.length;a<d;a++)if("undefined"!=typeof this[a])return this[a]},A=function(){for(var a=this.length;--a;)if("undefined"!=typeof this[a])return this[a]},w=function(k,d){k=String(k);var f=v,n=Array.prototype.slice.call(arguments,2),u=w.listeners(k),p=0,b,q=[],e={},l=[],r=a;l.firstDefined=M;l.lastDefined=A;a=k;for(var s=v=0,x=u.length;s<x;s++)"zIndex"in u[s]&&(q.push(u[s].zIndex),0>u[s].zIndex&&
(e[u[s].zIndex]=u[s]));for(q.sort(C);0>q[p];)if(b=e[q[p++] ],l.push(b.apply(d,n)),v)return v=f,l;for(s=0;s<x;s++)if(b=u[s],"zIndex"in b)if(b.zIndex==q[p]){l.push(b.apply(d,n));if(v)break;do if(p++,(b=e[q[p] ])&&l.push(b.apply(d,n)),v)break;while(b)}else e[b.zIndex]=b;else if(l.push(b.apply(d,n)),v)break;v=f;a=r;return l};w._events=y;w.listeners=function(a){a=a.split(k);var d=y,f,n,u,p,b,q,e,l=[d],r=[];u=0;for(p=a.length;u<p;u++){e=[];b=0;for(q=l.length;b<q;b++)for(d=l[b].n,f=[d[a[u] ],d["*"] ],n=2;n--;)if(d=
f[n])e.push(d),r=r.concat(d.f||[]);l=e}return r};w.on=function(a,d){a=String(a);if("function"!=typeof d)return function(){};for(var f=a.split(L),n=0,u=f.length;n<u;n++)(function(a){a=a.split(k);for(var b=y,f,e=0,l=a.length;e<l;e++)b=b.n,b=b.hasOwnProperty(a[e])&&b[a[e] ]||(b[a[e] ]={n:{}});b.f=b.f||[];e=0;for(l=b.f.length;e<l;e++)if(b.f[e]==d){f=!0;break}!f&&b.f.push(d)})(f[n]);return function(a){+a==+a&&(d.zIndex=+a)}};w.f=function(a){var d=[].slice.call(arguments,1);return function(){w.apply(null,
[a,null].concat(d).concat([].slice.call(arguments,0)))}};w.stop=function(){v=1};w.nt=function(k){return k?(new RegExp("(?:\\.|\\/|^)"+k+"(?:\\.|\\/|$)")).test(a):a};w.nts=function(){return a.split(k)};w.off=w.unbind=function(a,d){if(a){var f=a.split(L);if(1<f.length)for(var n=0,u=f.length;n<u;n++)w.off(f[n],d);else{for(var f=a.split(k),p,b,q,e,l=[y],n=0,u=f.length;n<u;n++)for(e=0;e<l.length;e+=q.length-2){q=[e,1];p=l[e].n;if("*"!=f[n])p[f[n] ]&&q.push(p[f[n] ]);else for(b in p)p.hasOwnProperty(b)&&
q.push(p[b]);l.splice.apply(l,q)}n=0;for(u=l.length;n<u;n++)for(p=l[n];p.n;){if(d){if(p.f){e=0;for(f=p.f.length;e<f;e++)if(p.f[e]==d){p.f.splice(e,1);break}!p.f.length&&delete p.f}for(b in p.n)if(p.n.hasOwnProperty(b)&&p.n[b].f){q=p.n[b].f;e=0;for(f=q.length;e<f;e++)if(q[e]==d){q.splice(e,1);break}!q.length&&delete p.n[b].f}}else for(b in delete p.f,p.n)p.n.hasOwnProperty(b)&&p.n[b].f&&delete p.n[b].f;p=p.n}}}else w._events=y={n:{}}};w.once=function(a,d){var f=function(){w.unbind(a,f);return d.apply(this,
arguments)};return w.on(a,f)};w.version="0.4.2";w.toString=function(){return"You are running Eve 0.4.2"};"undefined"!=typeof module&&module.exports?module.exports=w:"function"===typeof define&&define.amd?define("eve",[],function(){return w}):N.eve=w})(this);
(function(N,k){"function"===typeof define&&define.amd?define("Snap.svg",["eve"],function(L){return k(N,L)}):k(N,N.eve)})(this,function(N,k){var L=function(a){var k={},y=N.requestAnimationFrame||N.webkitRequestAnimationFrame||N.mozRequestAnimationFrame||N.oRequestAnimationFrame||N.msRequestAnimationFrame||function(a){setTimeout(a,16)},M=Array.isArray||function(a){return a instanceof Array||"[object Array]"==Object.prototype.toString.call(a)},A=0,w="M"+(+new Date).toString(36),z=function(a){if(null==
a)return this.s;var b=this.s-a;this.b+=this.dur*b;this.B+=this.dur*b;this.s=a},d=function(a){if(null==a)return this.spd;this.spd=a},f=function(a){if(null==a)return this.dur;this.s=this.s*a/this.dur;this.dur=a},n=function(){delete k[this.id];this.update();a("mina.stop."+this.id,this)},u=function(){this.pdif||(delete k[this.id],this.update(),this.pdif=this.get()-this.b)},p=function(){this.pdif&&(this.b=this.get()-this.pdif,delete this.pdif,k[this.id]=this)},b=function(){var a;if(M(this.start)){a=[];
for(var b=0,e=this.start.length;b<e;b++)a[b]=+this.start[b]+(this.end[b]-this.start[b])*this.easing(this.s)}else a=+this.start+(this.end-this.start)*this.easing(this.s);this.set(a)},q=function(){var l=0,b;for(b in k)if(k.hasOwnProperty(b)){var e=k[b],f=e.get();l++;e.s=(f-e.b)/(e.dur/e.spd);1<=e.s&&(delete k[b],e.s=1,l--,function(b){setTimeout(function(){a("mina.finish."+b.id,b)})}(e));e.update()}l&&y(q)},e=function(a,r,s,x,G,h,J){a={id:w+(A++).toString(36),start:a,end:r,b:s,s:0,dur:x-s,spd:1,get:G,
set:h,easing:J||e.linear,status:z,speed:d,duration:f,stop:n,pause:u,resume:p,update:b};k[a.id]=a;r=0;for(var K in k)if(k.hasOwnProperty(K)&&(r++,2==r))break;1==r&&y(q);return a};e.time=Date.now||function(){return+new Date};e.getById=function(a){return k[a]||null};e.linear=function(a){return a};e.easeout=function(a){return Math.pow(a,1.7)};e.easein=function(a){return Math.pow(a,0.48)};e.easeinout=function(a){if(1==a)return 1;if(0==a)return 0;var b=0.48-a/1.04,e=Math.sqrt(0.1734+b*b);a=e-b;a=Math.pow(Math.abs(a),
1/3)*(0>a?-1:1);b=-e-b;b=Math.pow(Math.abs(b),1/3)*(0>b?-1:1);a=a+b+0.5;return 3*(1-a)*a*a+a*a*a};e.backin=function(a){return 1==a?1:a*a*(2.70158*a-1.70158)};e.backout=function(a){if(0==a)return 0;a-=1;return a*a*(2.70158*a+1.70158)+1};e.elastic=function(a){return a==!!a?a:Math.pow(2,-10*a)*Math.sin(2*(a-0.075)*Math.PI/0.3)+1};e.bounce=function(a){a<1/2.75?a*=7.5625*a:a<2/2.75?(a-=1.5/2.75,a=7.5625*a*a+0.75):a<2.5/2.75?(a-=2.25/2.75,a=7.5625*a*a+0.9375):(a-=2.625/2.75,a=7.5625*a*a+0.984375);return a};
return N.mina=e}("undefined"==typeof k?function(){}:k),C=function(){function a(c,t){if(c){if(c.tagName)return x(c);if(y(c,"array")&&a.set)return a.set.apply(a,c);if(c instanceof e)return c;if(null==t)return c=G.doc.querySelector(c),x(c)}return new s(null==c?"100%":c,null==t?"100%":t)}function v(c,a){if(a){"#text"==c&&(c=G.doc.createTextNode(a.text||""));"string"==typeof c&&(c=v(c));if("string"==typeof a)return"xlink:"==a.substring(0,6)?c.getAttributeNS(m,a.substring(6)):"xml:"==a.substring(0,4)?c.getAttributeNS(la,
a.substring(4)):c.getAttribute(a);for(var da in a)if(a[h](da)){var b=J(a[da]);b?"xlink:"==da.substring(0,6)?c.setAttributeNS(m,da.substring(6),b):"xml:"==da.substring(0,4)?c.setAttributeNS(la,da.substring(4),b):c.setAttribute(da,b):c.removeAttribute(da)}}else c=G.doc.createElementNS(la,c);return c}function y(c,a){a=J.prototype.toLowerCase.call(a);return"finite"==a?isFinite(c):"array"==a&&(c instanceof Array||Array.isArray&&Array.isArray(c))?!0:"null"==a&&null===c||a==typeof c&&null!==c||"object"==
a&&c===Object(c)||$.call(c).slice(8,-1).toLowerCase()==a}function M(c){if("function"==typeof c||Object(c)!==c)return c;var a=new c.constructor,b;for(b in c)c[h](b)&&(a[b]=M(c[b]));return a}function A(c,a,b){function m(){var e=Array.prototype.slice.call(arguments,0),f=e.join("\u2400"),d=m.cache=m.cache||{},l=m.count=m.count||[];if(d[h](f)){a:for(var e=l,l=f,B=0,H=e.length;B<H;B++)if(e[B]===l){e.push(e.splice(B,1)[0]);break a}return b?b(d[f]):d[f]}1E3<=l.length&&delete d[l.shift()];l.push(f);d[f]=c.apply(a,
e);return b?b(d[f]):d[f]}return m}function w(c,a,b,m,e,f){return null==e?(c-=b,a-=m,c||a?(180*I.atan2(-a,-c)/C+540)%360:0):w(c,a,e,f)-w(b,m,e,f)}function z(c){return c%360*C/180}function d(c){var a=[];c=c.replace(/(?:^|\s)(\w+)\(([^)]+)\)/g,function(c,b,m){m=m.split(/\s*,\s*|\s+/);"rotate"==b&&1==m.length&&m.push(0,0);"scale"==b&&(2<m.length?m=m.slice(0,2):2==m.length&&m.push(0,0),1==m.length&&m.push(m[0],0,0));"skewX"==b?a.push(["m",1,0,I.tan(z(m[0])),1,0,0]):"skewY"==b?a.push(["m",1,I.tan(z(m[0])),
0,1,0,0]):a.push([b.charAt(0)].concat(m));return c});return a}function f(c,t){var b=O(c),m=new a.Matrix;if(b)for(var e=0,f=b.length;e<f;e++){var h=b[e],d=h.length,B=J(h[0]).toLowerCase(),H=h[0]!=B,l=H?m.invert():0,E;"t"==B&&2==d?m.translate(h[1],0):"t"==B&&3==d?H?(d=l.x(0,0),B=l.y(0,0),H=l.x(h[1],h[2]),l=l.y(h[1],h[2]),m.translate(H-d,l-B)):m.translate(h[1],h[2]):"r"==B?2==d?(E=E||t,m.rotate(h[1],E.x+E.width/2,E.y+E.height/2)):4==d&&(H?(H=l.x(h[2],h[3]),l=l.y(h[2],h[3]),m.rotate(h[1],H,l)):m.rotate(h[1],
h[2],h[3])):"s"==B?2==d||3==d?(E=E||t,m.scale(h[1],h[d-1],E.x+E.width/2,E.y+E.height/2)):4==d?H?(H=l.x(h[2],h[3]),l=l.y(h[2],h[3]),m.scale(h[1],h[1],H,l)):m.scale(h[1],h[1],h[2],h[3]):5==d&&(H?(H=l.x(h[3],h[4]),l=l.y(h[3],h[4]),m.scale(h[1],h[2],H,l)):m.scale(h[1],h[2],h[3],h[4])):"m"==B&&7==d&&m.add(h[1],h[2],h[3],h[4],h[5],h[6])}return m}function n(c,t){if(null==t){var m=!0;t="linearGradient"==c.type||"radialGradient"==c.type?c.node.getAttribute("gradientTransform"):"pattern"==c.type?c.node.getAttribute("patternTransform"):
c.node.getAttribute("transform");if(!t)return new a.Matrix;t=d(t)}else t=a._.rgTransform.test(t)?J(t).replace(/\.{3}|\u2026/g,c._.transform||aa):d(t),y(t,"array")&&(t=a.path?a.path.toString.call(t):J(t)),c._.transform=t;var b=f(t,c.getBBox(1));if(m)return b;c.matrix=b}function u(c){c=c.node.ownerSVGElement&&x(c.node.ownerSVGElement)||c.node.parentNode&&x(c.node.parentNode)||a.select("svg")||a(0,0);var t=c.select("defs"),t=null==t?!1:t.node;t||(t=r("defs",c.node).node);return t}function p(c){return c.node.ownerSVGElement&&
x(c.node.ownerSVGElement)||a.select("svg")}function b(c,a,m){function b(c){if(null==c)return aa;if(c==+c)return c;v(B,{width:c});try{return B.getBBox().width}catch(a){return 0}}function h(c){if(null==c)return aa;if(c==+c)return c;v(B,{height:c});try{return B.getBBox().height}catch(a){return 0}}function e(b,B){null==a?d[b]=B(c.attr(b)||0):b==a&&(d=B(null==m?c.attr(b)||0:m))}var f=p(c).node,d={},B=f.querySelector(".svg---mgr");B||(B=v("rect"),v(B,{x:-9E9,y:-9E9,width:10,height:10,"class":"svg---mgr",
fill:"none"}),f.appendChild(B));switch(c.type){case "rect":e("rx",b),e("ry",h);case "image":e("width",b),e("height",h);case "text":e("x",b);e("y",h);break;case "circle":e("cx",b);e("cy",h);e("r",b);break;case "ellipse":e("cx",b);e("cy",h);e("rx",b);e("ry",h);break;case "line":e("x1",b);e("x2",b);e("y1",h);e("y2",h);break;case "marker":e("refX",b);e("markerWidth",b);e("refY",h);e("markerHeight",h);break;case "radialGradient":e("fx",b);e("fy",h);break;case "tspan":e("dx",b);e("dy",h);break;default:e(a,
b)}f.removeChild(B);return d}function q(c){y(c,"array")||(c=Array.prototype.slice.call(arguments,0));for(var a=0,b=0,m=this.node;this[a];)delete this[a++];for(a=0;a<c.length;a++)"set"==c[a].type?c[a].forEach(function(c){m.appendChild(c.node)}):m.appendChild(c[a].node);for(var h=m.childNodes,a=0;a<h.length;a++)this[b++]=x(h[a]);return this}function e(c){if(c.snap in E)return E[c.snap];var a=this.id=V(),b;try{b=c.ownerSVGElement}catch(m){}this.node=c;b&&(this.paper=new s(b));this.type=c.tagName;this.anims=
{};this._={transform:[]};c.snap=a;E[a]=this;"g"==this.type&&(this.add=q);if(this.type in{g:1,mask:1,pattern:1})for(var e in s.prototype)s.prototype[h](e)&&(this[e]=s.prototype[e])}function l(c){this.node=c}function r(c,a){var b=v(c);a.appendChild(b);return x(b)}function s(c,a){var b,m,f,d=s.prototype;if(c&&"svg"==c.tagName){if(c.snap in E)return E[c.snap];var l=c.ownerDocument;b=new e(c);m=c.getElementsByTagName("desc")[0];f=c.getElementsByTagName("defs")[0];m||(m=v("desc"),m.appendChild(l.createTextNode("Created with Snap")),
b.node.appendChild(m));f||(f=v("defs"),b.node.appendChild(f));b.defs=f;for(var ca in d)d[h](ca)&&(b[ca]=d[ca]);b.paper=b.root=b}else b=r("svg",G.doc.body),v(b.node,{height:a,version:1.1,width:c,xmlns:la});return b}function x(c){return!c||c instanceof e||c instanceof l?c:c.tagName&&"svg"==c.tagName.toLowerCase()?new s(c):c.tagName&&"object"==c.tagName.toLowerCase()&&"image/svg+xml"==c.type?new s(c.contentDocument.getElementsByTagName("svg")[0]):new e(c)}a.version="0.3.0";a.toString=function(){return"Snap v"+
this.version};a._={};var G={win:N,doc:N.document};a._.glob=G;var h="hasOwnProperty",J=String,K=parseFloat,U=parseInt,I=Math,P=I.max,Q=I.min,Y=I.abs,C=I.PI,aa="",$=Object.prototype.toString,F=/^\s*((#[a-f\d]{6})|(#[a-f\d]{3})|rgba?\(\s*([\d\.]+%?\s*,\s*[\d\.]+%?\s*,\s*[\d\.]+%?(?:\s*,\s*[\d\.]+%?)?)\s*\)|hsba?\(\s*([\d\.]+(?:deg|\xb0|%)?\s*,\s*[\d\.]+%?\s*,\s*[\d\.]+(?:%?\s*,\s*[\d\.]+)?%?)\s*\)|hsla?\(\s*([\d\.]+(?:deg|\xb0|%)?\s*,\s*[\d\.]+%?\s*,\s*[\d\.]+(?:%?\s*,\s*[\d\.]+)?%?)\s*\))\s*$/i;a._.separator=
RegExp("[,\t\n\x0B\f\r \u00a0\u1680\u180e\u2000\u2001\u2002\u2003\u2004\u2005\u2006\u2007\u2008\u2009\u200a\u202f\u205f\u3000\u2028\u2029]+");var S=RegExp("[\t\n\x0B\f\r \u00a0\u1680\u180e\u2000\u2001\u2002\u2003\u2004\u2005\u2006\u2007\u2008\u2009\u200a\u202f\u205f\u3000\u2028\u2029]*,[\t\n\x0B\f\r \u00a0\u1680\u180e\u2000\u2001\u2002\u2003\u2004\u2005\u2006\u2007\u2008\u2009\u200a\u202f\u205f\u3000\u2028\u2029]*"),X={hs:1,rg:1},W=RegExp("([a-z])[\t\n\x0B\f\r \u00a0\u1680\u180e\u2000\u2001\u2002\u2003\u2004\u2005\u2006\u2007\u2008\u2009\u200a\u202f\u205f\u3000\u2028\u2029,]*((-?\\d*\\.?\\d*(?:e[\\-+]?\\d+)?[\t\n\x0B\f\r \u00a0\u1680\u180e\u2000\u2001\u2002\u2003\u2004\u2005\u2006\u2007\u2008\u2009\u200a\u202f\u205f\u3000\u2028\u2029]*,?[\t\n\x0B\f\r \u00a0\u1680\u180e\u2000\u2001\u2002\u2003\u2004\u2005\u2006\u2007\u2008\u2009\u200a\u202f\u205f\u3000\u2028\u2029]*)+)",
"ig"),ma=RegExp("([rstm])[\t\n\x0B\f\r \u00a0\u1680\u180e\u2000\u2001\u2002\u2003\u2004\u2005\u2006\u2007\u2008\u2009\u200a\u202f\u205f\u3000\u2028\u2029,]*((-?\\d*\\.?\\d*(?:e[\\-+]?\\d+)?[\t\n\x0B\f\r \u00a0\u1680\u180e\u2000\u2001\u2002\u2003\u2004\u2005\u2006\u2007\u2008\u2009\u200a\u202f\u205f\u3000\u2028\u2029]*,?[\t\n\x0B\f\r \u00a0\u1680\u180e\u2000\u2001\u2002\u2003\u2004\u2005\u2006\u2007\u2008\u2009\u200a\u202f\u205f\u3000\u2028\u2029]*)+)","ig"),Z=RegExp("(-?\\d*\\.?\\d*(?:e[\\-+]?\\d+)?)[\t\n\x0B\f\r \u00a0\u1680\u180e\u2000\u2001\u2002\u2003\u2004\u2005\u2006\u2007\u2008\u2009\u200a\u202f\u205f\u3000\u2028\u2029]*,?[\t\n\x0B\f\r \u00a0\u1680\u180e\u2000\u2001\u2002\u2003\u2004\u2005\u2006\u2007\u2008\u2009\u200a\u202f\u205f\u3000\u2028\u2029]*",
"ig"),na=0,ba="S"+(+new Date).toString(36),V=function(){return ba+(na++).toString(36)},m="http://www.w3.org/1999/xlink",la="http://www.w3.org/2000/svg",E={},ca=a.url=function(c){return"url('#"+c+"')"};a._.$=v;a._.id=V;a.format=function(){var c=/\{([^\}]+)\}/g,a=/(?:(?:^|\.)(.+?)(?=\[|\.|$|\()|\[('|")(.+?)\2\])(\(\))?/g,b=function(c,b,m){var h=m;b.replace(a,function(c,a,b,m,t){a=a||m;h&&(a in h&&(h=h[a]),"function"==typeof h&&t&&(h=h()))});return h=(null==h||h==m?c:h)+""};return function(a,m){return J(a).replace(c,
function(c,a){return b(c,a,m)})}}();a._.clone=M;a._.cacher=A;a.rad=z;a.deg=function(c){return 180*c/C%360};a.angle=w;a.is=y;a.snapTo=function(c,a,b){b=y(b,"finite")?b:10;if(y(c,"array"))for(var m=c.length;m--;){if(Y(c[m]-a)<=b)return c[m]}else{c=+c;m=a%c;if(m<b)return a-m;if(m>c-b)return a-m+c}return a};a.getRGB=A(function(c){if(!c||(c=J(c)).indexOf("-")+1)return{r:-1,g:-1,b:-1,hex:"none",error:1,toString:ka};if("none"==c)return{r:-1,g:-1,b:-1,hex:"none",toString:ka};!X[h](c.toLowerCase().substring(0,
2))&&"#"!=c.charAt()&&(c=T(c));if(!c)return{r:-1,g:-1,b:-1,hex:"none",error:1,toString:ka};var b,m,e,f,d;if(c=c.match(F)){c[2]&&(e=U(c[2].substring(5),16),m=U(c[2].substring(3,5),16),b=U(c[2].substring(1,3),16));c[3]&&(e=U((d=c[3].charAt(3))+d,16),m=U((d=c[3].charAt(2))+d,16),b=U((d=c[3].charAt(1))+d,16));c[4]&&(d=c[4].split(S),b=K(d[0]),"%"==d[0].slice(-1)&&(b*=2.55),m=K(d[1]),"%"==d[1].slice(-1)&&(m*=2.55),e=K(d[2]),"%"==d[2].slice(-1)&&(e*=2.55),"rgba"==c[1].toLowerCase().slice(0,4)&&(f=K(d[3])),
d[3]&&"%"==d[3].slice(-1)&&(f/=100));if(c[5])return d=c[5].split(S),b=K(d[0]),"%"==d[0].slice(-1)&&(b/=100),m=K(d[1]),"%"==d[1].slice(-1)&&(m/=100),e=K(d[2]),"%"==d[2].slice(-1)&&(e/=100),"deg"!=d[0].slice(-3)&&"\u00b0"!=d[0].slice(-1)||(b/=360),"hsba"==c[1].toLowerCase().slice(0,4)&&(f=K(d[3])),d[3]&&"%"==d[3].slice(-1)&&(f/=100),a.hsb2rgb(b,m,e,f);if(c[6])return d=c[6].split(S),b=K(d[0]),"%"==d[0].slice(-1)&&(b/=100),m=K(d[1]),"%"==d[1].slice(-1)&&(m/=100),e=K(d[2]),"%"==d[2].slice(-1)&&(e/=100),
"deg"!=d[0].slice(-3)&&"\u00b0"!=d[0].slice(-1)||(b/=360),"hsla"==c[1].toLowerCase().slice(0,4)&&(f=K(d[3])),d[3]&&"%"==d[3].slice(-1)&&(f/=100),a.hsl2rgb(b,m,e,f);b=Q(I.round(b),255);m=Q(I.round(m),255);e=Q(I.round(e),255);f=Q(P(f,0),1);c={r:b,g:m,b:e,toString:ka};c.hex="#"+(16777216|e|m<<8|b<<16).toString(16).slice(1);c.opacity=y(f,"finite")?f:1;return c}return{r:-1,g:-1,b:-1,hex:"none",error:1,toString:ka}},a);a.hsb=A(function(c,b,m){return a.hsb2rgb(c,b,m).hex});a.hsl=A(function(c,b,m){return a.hsl2rgb(c,
b,m).hex});a.rgb=A(function(c,a,b,m){if(y(m,"finite")){var e=I.round;return"rgba("+[e(c),e(a),e(b),+m.toFixed(2)]+")"}return"#"+(16777216|b|a<<8|c<<16).toString(16).slice(1)});var T=function(c){var a=G.doc.getElementsByTagName("head")[0]||G.doc.getElementsByTagName("svg")[0];T=A(function(c){if("red"==c.toLowerCase())return"rgb(255, 0, 0)";a.style.color="rgb(255, 0, 0)";a.style.color=c;c=G.doc.defaultView.getComputedStyle(a,aa).getPropertyValue("color");return"rgb(255, 0, 0)"==c?null:c});return T(c)},
qa=function(){return"hsb("+[this.h,this.s,this.b]+")"},ra=function(){return"hsl("+[this.h,this.s,this.l]+")"},ka=function(){return 1==this.opacity||null==this.opacity?this.hex:"rgba("+[this.r,this.g,this.b,this.opacity]+")"},D=function(c,b,m){null==b&&y(c,"object")&&"r"in c&&"g"in c&&"b"in c&&(m=c.b,b=c.g,c=c.r);null==b&&y(c,string)&&(m=a.getRGB(c),c=m.r,b=m.g,m=m.b);if(1<c||1<b||1<m)c/=255,b/=255,m/=255;return[c,b,m]},oa=function(c,b,m,e){c=I.round(255*c);b=I.round(255*b);m=I.round(255*m);c={r:c,
g:b,b:m,opacity:y(e,"finite")?e:1,hex:a.rgb(c,b,m),toString:ka};y(e,"finite")&&(c.opacity=e);return c};a.color=function(c){var b;y(c,"object")&&"h"in c&&"s"in c&&"b"in c?(b=a.hsb2rgb(c),c.r=b.r,c.g=b.g,c.b=b.b,c.opacity=1,c.hex=b.hex):y(c,"object")&&"h"in c&&"s"in c&&"l"in c?(b=a.hsl2rgb(c),c.r=b.r,c.g=b.g,c.b=b.b,c.opacity=1,c.hex=b.hex):(y(c,"string")&&(c=a.getRGB(c)),y(c,"object")&&"r"in c&&"g"in c&&"b"in c&&!("error"in c)?(b=a.rgb2hsl(c),c.h=b.h,c.s=b.s,c.l=b.l,b=a.rgb2hsb(c),c.v=b.b):(c={hex:"none"},
c.r=c.g=c.b=c.h=c.s=c.v=c.l=-1,c.error=1));c.toString=ka;return c};a.hsb2rgb=function(c,a,b,m){y(c,"object")&&"h"in c&&"s"in c&&"b"in c&&(b=c.b,a=c.s,c=c.h,m=c.o);var e,h,d;c=360*c%360/60;d=b*a;a=d*(1-Y(c%2-1));b=e=h=b-d;c=~~c;b+=[d,a,0,0,a,d][c];e+=[a,d,d,a,0,0][c];h+=[0,0,a,d,d,a][c];return oa(b,e,h,m)};a.hsl2rgb=function(c,a,b,m){y(c,"object")&&"h"in c&&"s"in c&&"l"in c&&(b=c.l,a=c.s,c=c.h);if(1<c||1<a||1<b)c/=360,a/=100,b/=100;var e,h,d;c=360*c%360/60;d=2*a*(0.5>b?b:1-b);a=d*(1-Y(c%2-1));b=e=
h=b-d/2;c=~~c;b+=[d,a,0,0,a,d][c];e+=[a,d,d,a,0,0][c];h+=[0,0,a,d,d,a][c];return oa(b,e,h,m)};a.rgb2hsb=function(c,a,b){b=D(c,a,b);c=b[0];a=b[1];b=b[2];var m,e;m=P(c,a,b);e=m-Q(c,a,b);c=((0==e?0:m==c?(a-b)/e:m==a?(b-c)/e+2:(c-a)/e+4)+360)%6*60/360;return{h:c,s:0==e?0:e/m,b:m,toString:qa}};a.rgb2hsl=function(c,a,b){b=D(c,a,b);c=b[0];a=b[1];b=b[2];var m,e,h;m=P(c,a,b);e=Q(c,a,b);h=m-e;c=((0==h?0:m==c?(a-b)/h:m==a?(b-c)/h+2:(c-a)/h+4)+360)%6*60/360;m=(m+e)/2;return{h:c,s:0==h?0:0.5>m?h/(2*m):h/(2-2*
m),l:m,toString:ra}};a.parsePathString=function(c){if(!c)return null;var b=a.path(c);if(b.arr)return a.path.clone(b.arr);var m={a:7,c:6,o:2,h:1,l:2,m:2,r:4,q:4,s:4,t:2,v:1,u:3,z:0},e=[];y(c,"array")&&y(c[0],"array")&&(e=a.path.clone(c));e.length||J(c).replace(W,function(c,a,b){var h=[];c=a.toLowerCase();b.replace(Z,function(c,a){a&&h.push(+a)});"m"==c&&2<h.length&&(e.push([a].concat(h.splice(0,2))),c="l",a="m"==a?"l":"L");"o"==c&&1==h.length&&e.push([a,h[0] ]);if("r"==c)e.push([a].concat(h));else for(;h.length>=
m[c]&&(e.push([a].concat(h.splice(0,m[c]))),m[c]););});e.toString=a.path.toString;b.arr=a.path.clone(e);return e};var O=a.parseTransformString=function(c){if(!c)return null;var b=[];y(c,"array")&&y(c[0],"array")&&(b=a.path.clone(c));b.length||J(c).replace(ma,function(c,a,m){var e=[];a.toLowerCase();m.replace(Z,function(c,a){a&&e.push(+a)});b.push([a].concat(e))});b.toString=a.path.toString;return b};a._.svgTransform2string=d;a._.rgTransform=RegExp("^[a-z][\t\n\x0B\f\r \u00a0\u1680\u180e\u2000\u2001\u2002\u2003\u2004\u2005\u2006\u2007\u2008\u2009\u200a\u202f\u205f\u3000\u2028\u2029]*-?\\.?\\d",
"i");a._.transform2matrix=f;a._unit2px=b;a._.getSomeDefs=u;a._.getSomeSVG=p;a.select=function(c){return x(G.doc.querySelector(c))};a.selectAll=function(c){c=G.doc.querySelectorAll(c);for(var b=(a.set||Array)(),m=0;m<c.length;m++)b.push(x(c[m]));return b};setInterval(function(){for(var c in E)if(E[h](c)){var a=E[c],b=a.node;("svg"!=a.type&&!b.ownerSVGElement||"svg"==a.type&&(!b.parentNode||"ownerSVGElement"in b.parentNode&&!b.ownerSVGElement))&&delete E[c]}},1E4);(function(c){function m(c){function a(c,
b){var m=v(c.node,b);(m=(m=m&&m.match(d))&&m[2])&&"#"==m.charAt()&&(m=m.substring(1))&&(f[m]=(f[m]||[]).concat(function(a){var m={};m[b]=ca(a);v(c.node,m)}))}function b(c){var a=v(c.node,"xlink:href");a&&"#"==a.charAt()&&(a=a.substring(1))&&(f[a]=(f[a]||[]).concat(function(a){c.attr("xlink:href","#"+a)}))}var e=c.selectAll("*"),h,d=/^\s*url\(("|'|)(.*)\1\)\s*$/;c=[];for(var f={},l=0,E=e.length;l<E;l++){h=e[l];a(h,"fill");a(h,"stroke");a(h,"filter");a(h,"mask");a(h,"clip-path");b(h);var t=v(h.node,
"id");t&&(v(h.node,{id:h.id}),c.push({old:t,id:h.id}))}l=0;for(E=c.length;l<E;l++)if(e=f[c[l].old])for(h=0,t=e.length;h<t;h++)e[h](c[l].id)}function e(c,a,b){return function(m){m=m.slice(c,a);1==m.length&&(m=m[0]);return b?b(m):m}}function d(c){return function(){var a=c?"<"+this.type:"",b=this.node.attributes,m=this.node.childNodes;if(c)for(var e=0,h=b.length;e<h;e++)a+=" "+b[e].name+'="'+b[e].value.replace(/"/g,'\\"')+'"';if(m.length){c&&(a+=">");e=0;for(h=m.length;e<h;e++)3==m[e].nodeType?a+=m[e].nodeValue:
1==m[e].nodeType&&(a+=x(m[e]).toString());c&&(a+="</"+this.type+">")}else c&&(a+="/>");return a}}c.attr=function(c,a){if(!c)return this;if(y(c,"string"))if(1<arguments.length){var b={};b[c]=a;c=b}else return k("snap.util.getattr."+c,this).firstDefined();for(var m in c)c[h](m)&&k("snap.util.attr."+m,this,c[m]);return this};c.getBBox=function(c){if(!a.Matrix||!a.path)return this.node.getBBox();var b=this,m=new a.Matrix;if(b.removed)return a._.box();for(;"use"==b.type;)if(c||(m=m.add(b.transform().localMatrix.translate(b.attr("x")||
0,b.attr("y")||0))),b.original)b=b.original;else var e=b.attr("xlink:href"),b=b.original=b.node.ownerDocument.getElementById(e.substring(e.indexOf("#")+1));var e=b._,h=a.path.get[b.type]||a.path.get.deflt;try{if(c)return e.bboxwt=h?a.path.getBBox(b.realPath=h(b)):a._.box(b.node.getBBox()),a._.box(e.bboxwt);b.realPath=h(b);b.matrix=b.transform().localMatrix;e.bbox=a.path.getBBox(a.path.map(b.realPath,m.add(b.matrix)));return a._.box(e.bbox)}catch(d){return a._.box()}};var f=function(){return this.string};
c.transform=function(c){var b=this._;if(null==c){var m=this;c=new a.Matrix(this.node.getCTM());for(var e=n(this),h=[e],d=new a.Matrix,l=e.toTransformString(),b=J(e)==J(this.matrix)?J(b.transform):l;"svg"!=m.type&&(m=m.parent());)h.push(n(m));for(m=h.length;m--;)d.add(h[m]);return{string:b,globalMatrix:c,totalMatrix:d,localMatrix:e,diffMatrix:c.clone().add(e.invert()),global:c.toTransformString(),total:d.toTransformString(),local:l,toString:f}}c instanceof a.Matrix?this.matrix=c:n(this,c);this.node&&
("linearGradient"==this.type||"radialGradient"==this.type?v(this.node,{gradientTransform:this.matrix}):"pattern"==this.type?v(this.node,{patternTransform:this.matrix}):v(this.node,{transform:this.matrix}));return this};c.parent=function(){return x(this.node.parentNode)};c.append=c.add=function(c){if(c){if("set"==c.type){var a=this;c.forEach(function(c){a.add(c)});return this}c=x(c);this.node.appendChild(c.node);c.paper=this.paper}return this};c.appendTo=function(c){c&&(c=x(c),c.append(this));return this};
c.prepend=function(c){if(c){if("set"==c.type){var a=this,b;c.forEach(function(c){b?b.after(c):a.prepend(c);b=c});return this}c=x(c);var m=c.parent();this.node.insertBefore(c.node,this.node.firstChild);this.add&&this.add();c.paper=this.paper;this.parent()&&this.parent().add();m&&m.add()}return this};c.prependTo=function(c){c=x(c);c.prepend(this);return this};c.before=function(c){if("set"==c.type){var a=this;c.forEach(function(c){var b=c.parent();a.node.parentNode.insertBefore(c.node,a.node);b&&b.add()});
this.parent().add();return this}c=x(c);var b=c.parent();this.node.parentNode.insertBefore(c.node,this.node);this.parent()&&this.parent().add();b&&b.add();c.paper=this.paper;return this};c.after=function(c){c=x(c);var a=c.parent();this.node.nextSibling?this.node.parentNode.insertBefore(c.node,this.node.nextSibling):this.node.parentNode.appendChild(c.node);this.parent()&&this.parent().add();a&&a.add();c.paper=this.paper;return this};c.insertBefore=function(c){c=x(c);var a=this.parent();c.node.parentNode.insertBefore(this.node,
c.node);this.paper=c.paper;a&&a.add();c.parent()&&c.parent().add();return this};c.insertAfter=function(c){c=x(c);var a=this.parent();c.node.parentNode.insertBefore(this.node,c.node.nextSibling);this.paper=c.paper;a&&a.add();c.parent()&&c.parent().add();return this};c.remove=function(){var c=this.parent();this.node.parentNode&&this.node.parentNode.removeChild(this.node);delete this.paper;this.removed=!0;c&&c.add();return this};c.select=function(c){return x(this.node.querySelector(c))};c.selectAll=
function(c){c=this.node.querySelectorAll(c);for(var b=(a.set||Array)(),m=0;m<c.length;m++)b.push(x(c[m]));return b};c.asPX=function(c,a){null==a&&(a=this.attr(c));return+b(this,c,a)};c.use=function(){var c,a=this.node.id;a||(a=this.id,v(this.node,{id:a}));c="linearGradient"==this.type||"radialGradient"==this.type||"pattern"==this.type?r(this.type,this.node.parentNode):r("use",this.node.parentNode);v(c.node,{"xlink:href":"#"+a});c.original=this;return c};var l=/\S+/g;c.addClass=function(c){var a=(c||
"").match(l)||[];c=this.node;var b=c.className.baseVal,m=b.match(l)||[],e,h,d;if(a.length){for(e=0;d=a[e++];)h=m.indexOf(d),~h||m.push(d);a=m.join(" ");b!=a&&(c.className.baseVal=a)}return this};c.removeClass=function(c){var a=(c||"").match(l)||[];c=this.node;var b=c.className.baseVal,m=b.match(l)||[],e,h;if(m.length){for(e=0;h=a[e++];)h=m.indexOf(h),~h&&m.splice(h,1);a=m.join(" ");b!=a&&(c.className.baseVal=a)}return this};c.hasClass=function(c){return!!~(this.node.className.baseVal.match(l)||[]).indexOf(c)};
c.toggleClass=function(c,a){if(null!=a)return a?this.addClass(c):this.removeClass(c);var b=(c||"").match(l)||[],m=this.node,e=m.className.baseVal,h=e.match(l)||[],d,f,E;for(d=0;E=b[d++];)f=h.indexOf(E),~f?h.splice(f,1):h.push(E);b=h.join(" ");e!=b&&(m.className.baseVal=b);return this};c.clone=function(){var c=x(this.node.cloneNode(!0));v(c.node,"id")&&v(c.node,{id:c.id});m(c);c.insertAfter(this);return c};c.toDefs=function(){u(this).appendChild(this.node);return this};c.pattern=c.toPattern=function(c,
a,b,m){var e=r("pattern",u(this));null==c&&(c=this.getBBox());y(c,"object")&&"x"in c&&(a=c.y,b=c.width,m=c.height,c=c.x);v(e.node,{x:c,y:a,width:b,height:m,patternUnits:"userSpaceOnUse",id:e.id,viewBox:[c,a,b,m].join(" ")});e.node.appendChild(this.node);return e};c.marker=function(c,a,b,m,e,h){var d=r("marker",u(this));null==c&&(c=this.getBBox());y(c,"object")&&"x"in c&&(a=c.y,b=c.width,m=c.height,e=c.refX||c.cx,h=c.refY||c.cy,c=c.x);v(d.node,{viewBox:[c,a,b,m].join(" "),markerWidth:b,markerHeight:m,
orient:"auto",refX:e||0,refY:h||0,id:d.id});d.node.appendChild(this.node);return d};var E=function(c,a,b,m){"function"!=typeof b||b.length||(m=b,b=L.linear);this.attr=c;this.dur=a;b&&(this.easing=b);m&&(this.callback=m)};a._.Animation=E;a.animation=function(c,a,b,m){return new E(c,a,b,m)};c.inAnim=function(){var c=[],a;for(a in this.anims)this.anims[h](a)&&function(a){c.push({anim:new E(a._attrs,a.dur,a.easing,a._callback),mina:a,curStatus:a.status(),status:function(c){return a.status(c)},stop:function(){a.stop()}})}(this.anims[a]);
return c};a.animate=function(c,a,b,m,e,h){"function"!=typeof e||e.length||(h=e,e=L.linear);var d=L.time();c=L(c,a,d,d+m,L.time,b,e);h&&k.once("mina.finish."+c.id,h);return c};c.stop=function(){for(var c=this.inAnim(),a=0,b=c.length;a<b;a++)c[a].stop();return this};c.animate=function(c,a,b,m){"function"!=typeof b||b.length||(m=b,b=L.linear);c instanceof E&&(m=c.callback,b=c.easing,a=b.dur,c=c.attr);var d=[],f=[],l={},t,ca,n,T=this,q;for(q in c)if(c[h](q)){T.equal?(n=T.equal(q,J(c[q])),t=n.from,ca=
n.to,n=n.f):(t=+T.attr(q),ca=+c[q]);var la=y(t,"array")?t.length:1;l[q]=e(d.length,d.length+la,n);d=d.concat(t);f=f.concat(ca)}t=L.time();var p=L(d,f,t,t+a,L.time,function(c){var a={},b;for(b in l)l[h](b)&&(a[b]=l[b](c));T.attr(a)},b);T.anims[p.id]=p;p._attrs=c;p._callback=m;k("snap.animcreated."+T.id,p);k.once("mina.finish."+p.id,function(){delete T.anims[p.id];m&&m.call(T)});k.once("mina.stop."+p.id,function(){delete T.anims[p.id]});return T};var T={};c.data=function(c,b){var m=T[this.id]=T[this.id]||
{};if(0==arguments.length)return k("snap.data.get."+this.id,this,m,null),m;if(1==arguments.length){if(a.is(c,"object")){for(var e in c)c[h](e)&&this.data(e,c[e]);return this}k("snap.data.get."+this.id,this,m[c],c);return m[c]}m[c]=b;k("snap.data.set."+this.id,this,b,c);return this};c.removeData=function(c){null==c?T[this.id]={}:T[this.id]&&delete T[this.id][c];return this};c.outerSVG=c.toString=d(1);c.innerSVG=d()})(e.prototype);a.parse=function(c){var a=G.doc.createDocumentFragment(),b=!0,m=G.doc.createElement("div");
c=J(c);c.match(/^\s*<\s*svg(?:\s|>)/)||(c="<svg>"+c+"</svg>",b=!1);m.innerHTML=c;if(c=m.getElementsByTagName("svg")[0])if(b)a=c;else for(;c.firstChild;)a.appendChild(c.firstChild);m.innerHTML=aa;return new l(a)};l.prototype.select=e.prototype.select;l.prototype.selectAll=e.prototype.selectAll;a.fragment=function(){for(var c=Array.prototype.slice.call(arguments,0),b=G.doc.createDocumentFragment(),m=0,e=c.length;m<e;m++){var h=c[m];h.node&&h.node.nodeType&&b.appendChild(h.node);h.nodeType&&b.appendChild(h);
"string"==typeof h&&b.appendChild(a.parse(h).node)}return new l(b)};a._.make=r;a._.wrap=x;s.prototype.el=function(c,a){var b=r(c,this.node);a&&b.attr(a);return b};k.on("snap.util.getattr",function(){var c=k.nt(),c=c.substring(c.lastIndexOf(".")+1),a=c.replace(/[A-Z]/g,function(c){return"-"+c.toLowerCase()});return pa[h](a)?this.node.ownerDocument.defaultView.getComputedStyle(this.node,null).getPropertyValue(a):v(this.node,c)});var pa={"alignment-baseline":0,"baseline-shift":0,clip:0,"clip-path":0,
"clip-rule":0,color:0,"color-interpolation":0,"color-interpolation-filters":0,"color-profile":0,"color-rendering":0,cursor:0,direction:0,display:0,"dominant-baseline":0,"enable-background":0,fill:0,"fill-opacity":0,"fill-rule":0,filter:0,"flood-color":0,"flood-opacity":0,font:0,"font-family":0,"font-size":0,"font-size-adjust":0,"font-stretch":0,"font-style":0,"font-variant":0,"font-weight":0,"glyph-orientation-horizontal":0,"glyph-orientation-vertical":0,"image-rendering":0,kerning:0,"letter-spacing":0,
"lighting-color":0,marker:0,"marker-end":0,"marker-mid":0,"marker-start":0,mask:0,opacity:0,overflow:0,"pointer-events":0,"shape-rendering":0,"stop-color":0,"stop-opacity":0,stroke:0,"stroke-dasharray":0,"stroke-dashoffset":0,"stroke-linecap":0,"stroke-linejoin":0,"stroke-miterlimit":0,"stroke-opacity":0,"stroke-width":0,"text-anchor":0,"text-decoration":0,"text-rendering":0,"unicode-bidi":0,visibility:0,"word-spacing":0,"writing-mode":0};k.on("snap.util.attr",function(c){var a=k.nt(),b={},a=a.substring(a.lastIndexOf(".")+
1);b[a]=c;var m=a.replace(/-(\w)/gi,function(c,a){return a.toUpperCase()}),a=a.replace(/[A-Z]/g,function(c){return"-"+c.toLowerCase()});pa[h](a)?this.node.style[m]=null==c?aa:c:v(this.node,b)});a.ajax=function(c,a,b,m){var e=new XMLHttpRequest,h=V();if(e){if(y(a,"function"))m=b,b=a,a=null;else if(y(a,"object")){var d=[],f;for(f in a)a.hasOwnProperty(f)&&d.push(encodeURIComponent(f)+"="+encodeURIComponent(a[f]));a=d.join("&")}e.open(a?"POST":"GET",c,!0);a&&(e.setRequestHeader("X-Requested-With","XMLHttpRequest"),
e.setRequestHeader("Content-type","application/x-www-form-urlencoded"));b&&(k.once("snap.ajax."+h+".0",b),k.once("snap.ajax."+h+".200",b),k.once("snap.ajax."+h+".304",b));e.onreadystatechange=function(){4==e.readyState&&k("snap.ajax."+h+"."+e.status,m,e)};if(4==e.readyState)return e;e.send(a);return e}};a.load=function(c,b,m){a.ajax(c,function(c){c=a.parse(c.responseText);m?b.call(m,c):b(c)})};a.getElementByPoint=function(c,a){var b,m,e=G.doc.elementFromPoint(c,a);if(G.win.opera&&"svg"==e.tagName){b=
e;m=b.getBoundingClientRect();b=b.ownerDocument;var h=b.body,d=b.documentElement;b=m.top+(g.win.pageYOffset||d.scrollTop||h.scrollTop)-(d.clientTop||h.clientTop||0);m=m.left+(g.win.pageXOffset||d.scrollLeft||h.scrollLeft)-(d.clientLeft||h.clientLeft||0);h=e.createSVGRect();h.x=c-m;h.y=a-b;h.width=h.height=1;b=e.getIntersectionList(h,null);b.length&&(e=b[b.length-1])}return e?x(e):null};a.plugin=function(c){c(a,e,s,G,l)};return G.win.Snap=a}();C.plugin(function(a,k,y,M,A){function w(a,d,f,b,q,e){null==
d&&"[object SVGMatrix]"==z.call(a)?(this.a=a.a,this.b=a.b,this.c=a.c,this.d=a.d,this.e=a.e,this.f=a.f):null!=a?(this.a=+a,this.b=+d,this.c=+f,this.d=+b,this.e=+q,this.f=+e):(this.a=1,this.c=this.b=0,this.d=1,this.f=this.e=0)}var z=Object.prototype.toString,d=String,f=Math;(function(n){function k(a){return a[0]*a[0]+a[1]*a[1]}function p(a){var d=f.sqrt(k(a));a[0]&&(a[0]/=d);a[1]&&(a[1]/=d)}n.add=function(a,d,e,f,n,p){var k=[[],[],[] ],u=[[this.a,this.c,this.e],[this.b,this.d,this.f],[0,0,1] ];d=[[a,
e,n],[d,f,p],[0,0,1] ];a&&a instanceof w&&(d=[[a.a,a.c,a.e],[a.b,a.d,a.f],[0,0,1] ]);for(a=0;3>a;a++)for(e=0;3>e;e++){for(f=n=0;3>f;f++)n+=u[a][f]*d[f][e];k[a][e]=n}this.a=k[0][0];this.b=k[1][0];this.c=k[0][1];this.d=k[1][1];this.e=k[0][2];this.f=k[1][2];return this};n.invert=function(){var a=this.a*this.d-this.b*this.c;return new w(this.d/a,-this.b/a,-this.c/a,this.a/a,(this.c*this.f-this.d*this.e)/a,(this.b*this.e-this.a*this.f)/a)};n.clone=function(){return new w(this.a,this.b,this.c,this.d,this.e,
this.f)};n.translate=function(a,d){return this.add(1,0,0,1,a,d)};n.scale=function(a,d,e,f){null==d&&(d=a);(e||f)&&this.add(1,0,0,1,e,f);this.add(a,0,0,d,0,0);(e||f)&&this.add(1,0,0,1,-e,-f);return this};n.rotate=function(b,d,e){b=a.rad(b);d=d||0;e=e||0;var l=+f.cos(b).toFixed(9);b=+f.sin(b).toFixed(9);this.add(l,b,-b,l,d,e);return this.add(1,0,0,1,-d,-e)};n.x=function(a,d){return a*this.a+d*this.c+this.e};n.y=function(a,d){return a*this.b+d*this.d+this.f};n.get=function(a){return+this[d.fromCharCode(97+
a)].toFixed(4)};n.toString=function(){return"matrix("+[this.get(0),this.get(1),this.get(2),this.get(3),this.get(4),this.get(5)].join()+")"};n.offset=function(){return[this.e.toFixed(4),this.f.toFixed(4)]};n.determinant=function(){return this.a*this.d-this.b*this.c};n.split=function(){var b={};b.dx=this.e;b.dy=this.f;var d=[[this.a,this.c],[this.b,this.d] ];b.scalex=f.sqrt(k(d[0]));p(d[0]);b.shear=d[0][0]*d[1][0]+d[0][1]*d[1][1];d[1]=[d[1][0]-d[0][0]*b.shear,d[1][1]-d[0][1]*b.shear];b.scaley=f.sqrt(k(d[1]));
p(d[1]);b.shear/=b.scaley;0>this.determinant()&&(b.scalex=-b.scalex);var e=-d[0][1],d=d[1][1];0>d?(b.rotate=a.deg(f.acos(d)),0>e&&(b.rotate=360-b.rotate)):b.rotate=a.deg(f.asin(e));b.isSimple=!+b.shear.toFixed(9)&&(b.scalex.toFixed(9)==b.scaley.toFixed(9)||!b.rotate);b.isSuperSimple=!+b.shear.toFixed(9)&&b.scalex.toFixed(9)==b.scaley.toFixed(9)&&!b.rotate;b.noRotation=!+b.shear.toFixed(9)&&!b.rotate;return b};n.toTransformString=function(a){a=a||this.split();if(+a.shear.toFixed(9))return"m"+[this.get(0),
this.get(1),this.get(2),this.get(3),this.get(4),this.get(5)];a.scalex=+a.scalex.toFixed(4);a.scaley=+a.scaley.toFixed(4);a.rotate=+a.rotate.toFixed(4);return(a.dx||a.dy?"t"+[+a.dx.toFixed(4),+a.dy.toFixed(4)]:"")+(1!=a.scalex||1!=a.scaley?"s"+[a.scalex,a.scaley,0,0]:"")+(a.rotate?"r"+[+a.rotate.toFixed(4),0,0]:"")}})(w.prototype);a.Matrix=w;a.matrix=function(a,d,f,b,k,e){return new w(a,d,f,b,k,e)}});C.plugin(function(a,v,y,M,A){function w(h){return function(d){k.stop();d instanceof A&&1==d.node.childNodes.length&&
("radialGradient"==d.node.firstChild.tagName||"linearGradient"==d.node.firstChild.tagName||"pattern"==d.node.firstChild.tagName)&&(d=d.node.firstChild,b(this).appendChild(d),d=u(d));if(d instanceof v)if("radialGradient"==d.type||"linearGradient"==d.type||"pattern"==d.type){d.node.id||e(d.node,{id:d.id});var f=l(d.node.id)}else f=d.attr(h);else f=a.color(d),f.error?(f=a(b(this).ownerSVGElement).gradient(d))?(f.node.id||e(f.node,{id:f.id}),f=l(f.node.id)):f=d:f=r(f);d={};d[h]=f;e(this.node,d);this.node.style[h]=
x}}function z(a){k.stop();a==+a&&(a+="px");this.node.style.fontSize=a}function d(a){var b=[];a=a.childNodes;for(var e=0,f=a.length;e<f;e++){var l=a[e];3==l.nodeType&&b.push(l.nodeValue);"tspan"==l.tagName&&(1==l.childNodes.length&&3==l.firstChild.nodeType?b.push(l.firstChild.nodeValue):b.push(d(l)))}return b}function f(){k.stop();return this.node.style.fontSize}var n=a._.make,u=a._.wrap,p=a.is,b=a._.getSomeDefs,q=/^url\(#?([^)]+)\)$/,e=a._.$,l=a.url,r=String,s=a._.separator,x="";k.on("snap.util.attr.mask",
function(a){if(a instanceof v||a instanceof A){k.stop();a instanceof A&&1==a.node.childNodes.length&&(a=a.node.firstChild,b(this).appendChild(a),a=u(a));if("mask"==a.type)var d=a;else d=n("mask",b(this)),d.node.appendChild(a.node);!d.node.id&&e(d.node,{id:d.id});e(this.node,{mask:l(d.id)})}});(function(a){k.on("snap.util.attr.clip",a);k.on("snap.util.attr.clip-path",a);k.on("snap.util.attr.clipPath",a)})(function(a){if(a instanceof v||a instanceof A){k.stop();if("clipPath"==a.type)var d=a;else d=
n("clipPath",b(this)),d.node.appendChild(a.node),!d.node.id&&e(d.node,{id:d.id});e(this.node,{"clip-path":l(d.id)})}});k.on("snap.util.attr.fill",w("fill"));k.on("snap.util.attr.stroke",w("stroke"));var G=/^([lr])(?:\(([^)]*)\))?(.*)$/i;k.on("snap.util.grad.parse",function(a){a=r(a);var b=a.match(G);if(!b)return null;a=b[1];var e=b[2],b=b[3],e=e.split(/\s*,\s*/).map(function(a){return+a==a?+a:a});1==e.length&&0==e[0]&&(e=[]);b=b.split("-");b=b.map(function(a){a=a.split(":");var b={color:a[0]};a[1]&&
(b.offset=parseFloat(a[1]));return b});return{type:a,params:e,stops:b}});k.on("snap.util.attr.d",function(b){k.stop();p(b,"array")&&p(b[0],"array")&&(b=a.path.toString.call(b));b=r(b);b.match(/[ruo]/i)&&(b=a.path.toAbsolute(b));e(this.node,{d:b})})(-1);k.on("snap.util.attr.#text",function(a){k.stop();a=r(a);for(a=M.doc.createTextNode(a);this.node.firstChild;)this.node.removeChild(this.node.firstChild);this.node.appendChild(a)})(-1);k.on("snap.util.attr.path",function(a){k.stop();this.attr({d:a})})(-1);
k.on("snap.util.attr.class",function(a){k.stop();this.node.className.baseVal=a})(-1);k.on("snap.util.attr.viewBox",function(a){a=p(a,"object")&&"x"in a?[a.x,a.y,a.width,a.height].join(" "):p(a,"array")?a.join(" "):a;e(this.node,{viewBox:a});k.stop()})(-1);k.on("snap.util.attr.transform",function(a){this.transform(a);k.stop()})(-1);k.on("snap.util.attr.r",function(a){"rect"==this.type&&(k.stop(),e(this.node,{rx:a,ry:a}))})(-1);k.on("snap.util.attr.textpath",function(a){k.stop();if("text"==this.type){var d,
f;if(!a&&this.textPath){for(a=this.textPath;a.node.firstChild;)this.node.appendChild(a.node.firstChild);a.remove();delete this.textPath}else if(p(a,"string")?(d=b(this),a=u(d.parentNode).path(a),d.appendChild(a.node),d=a.id,a.attr({id:d})):(a=u(a),a instanceof v&&(d=a.attr("id"),d||(d=a.id,a.attr({id:d})))),d)if(a=this.textPath,f=this.node,a)a.attr({"xlink:href":"#"+d});else{for(a=e("textPath",{"xlink:href":"#"+d});f.firstChild;)a.appendChild(f.firstChild);f.appendChild(a);this.textPath=u(a)}}})(-1);
k.on("snap.util.attr.text",function(a){if("text"==this.type){for(var b=this.node,d=function(a){var b=e("tspan");if(p(a,"array"))for(var f=0;f<a.length;f++)b.appendChild(d(a[f]));else b.appendChild(M.doc.createTextNode(a));b.normalize&&b.normalize();return b};b.firstChild;)b.removeChild(b.firstChild);for(a=d(a);a.firstChild;)b.appendChild(a.firstChild)}k.stop()})(-1);k.on("snap.util.attr.fontSize",z)(-1);k.on("snap.util.attr.font-size",z)(-1);k.on("snap.util.getattr.transform",function(){k.stop();
return this.transform()})(-1);k.on("snap.util.getattr.textpath",function(){k.stop();return this.textPath})(-1);(function(){function b(d){return function(){k.stop();var b=M.doc.defaultView.getComputedStyle(this.node,null).getPropertyValue("marker-"+d);return"none"==b?b:a(M.doc.getElementById(b.match(q)[1]))}}function d(a){return function(b){k.stop();var d="marker"+a.charAt(0).toUpperCase()+a.substring(1);if(""==b||!b)this.node.style[d]="none";else if("marker"==b.type){var f=b.node.id;f||e(b.node,{id:b.id});
this.node.style[d]=l(f)}}}k.on("snap.util.getattr.marker-end",b("end"))(-1);k.on("snap.util.getattr.markerEnd",b("end"))(-1);k.on("snap.util.getattr.marker-start",b("start"))(-1);k.on("snap.util.getattr.markerStart",b("start"))(-1);k.on("snap.util.getattr.marker-mid",b("mid"))(-1);k.on("snap.util.getattr.markerMid",b("mid"))(-1);k.on("snap.util.attr.marker-end",d("end"))(-1);k.on("snap.util.attr.markerEnd",d("end"))(-1);k.on("snap.util.attr.marker-start",d("start"))(-1);k.on("snap.util.attr.markerStart",
d("start"))(-1);k.on("snap.util.attr.marker-mid",d("mid"))(-1);k.on("snap.util.attr.markerMid",d("mid"))(-1)})();k.on("snap.util.getattr.r",function(){if("rect"==this.type&&e(this.node,"rx")==e(this.node,"ry"))return k.stop(),e(this.node,"rx")})(-1);k.on("snap.util.getattr.text",function(){if("text"==this.type||"tspan"==this.type){k.stop();var a=d(this.node);return 1==a.length?a[0]:a}})(-1);k.on("snap.util.getattr.#text",function(){return this.node.textContent})(-1);k.on("snap.util.getattr.viewBox",
function(){k.stop();var b=e(this.node,"viewBox");if(b)return b=b.split(s),a._.box(+b[0],+b[1],+b[2],+b[3])})(-1);k.on("snap.util.getattr.points",function(){var a=e(this.node,"points");k.stop();if(a)return a.split(s)})(-1);k.on("snap.util.getattr.path",function(){var a=e(this.node,"d");k.stop();return a})(-1);k.on("snap.util.getattr.class",function(){return this.node.className.baseVal})(-1);k.on("snap.util.getattr.fontSize",f)(-1);k.on("snap.util.getattr.font-size",f)(-1)});C.plugin(function(a,v,y,
M,A){function w(a){return a}function z(a){return function(b){return+b.toFixed(3)+a}}var d={"+":function(a,b){return a+b},"-":function(a,b){return a-b},"/":function(a,b){return a/b},"*":function(a,b){return a*b}},f=String,n=/[a-z]+$/i,u=/^\s*([+\-\/*])\s*=\s*([\d.eE+\-]+)\s*([^\d\s]+)?\s*$/;k.on("snap.util.attr",function(a){if(a=f(a).match(u)){var b=k.nt(),b=b.substring(b.lastIndexOf(".")+1),q=this.attr(b),e={};k.stop();var l=a[3]||"",r=q.match(n),s=d[a[1] ];r&&r==l?a=s(parseFloat(q),+a[2]):(q=this.asPX(b),
a=s(this.asPX(b),this.asPX(b,a[2]+l)));isNaN(q)||isNaN(a)||(e[b]=a,this.attr(e))}})(-10);k.on("snap.util.equal",function(a,b){var q=f(this.attr(a)||""),e=f(b).match(u);if(e){k.stop();var l=e[3]||"",r=q.match(n),s=d[e[1] ];if(r&&r==l)return{from:parseFloat(q),to:s(parseFloat(q),+e[2]),f:z(r)};q=this.asPX(a);return{from:q,to:s(q,this.asPX(a,e[2]+l)),f:w}}})(-10)});C.plugin(function(a,v,y,M,A){var w=y.prototype,z=a.is;w.rect=function(a,d,k,p,b,q){var e;null==q&&(q=b);z(a,"object")&&"[object Object]"==
a?e=a:null!=a&&(e={x:a,y:d,width:k,height:p},null!=b&&(e.rx=b,e.ry=q));return this.el("rect",e)};w.circle=function(a,d,k){var p;z(a,"object")&&"[object Object]"==a?p=a:null!=a&&(p={cx:a,cy:d,r:k});return this.el("circle",p)};var d=function(){function a(){this.parentNode.removeChild(this)}return function(d,k){var p=M.doc.createElement("img"),b=M.doc.body;p.style.cssText="position:absolute;left:-9999em;top:-9999em";p.onload=function(){k.call(p);p.onload=p.onerror=null;b.removeChild(p)};p.onerror=a;
b.appendChild(p);p.src=d}}();w.image=function(f,n,k,p,b){var q=this.el("image");if(z(f,"object")&&"src"in f)q.attr(f);else if(null!=f){var e={"xlink:href":f,preserveAspectRatio:"none"};null!=n&&null!=k&&(e.x=n,e.y=k);null!=p&&null!=b?(e.width=p,e.height=b):d(f,function(){a._.$(q.node,{width:this.offsetWidth,height:this.offsetHeight})});a._.$(q.node,e)}return q};w.ellipse=function(a,d,k,p){var b;z(a,"object")&&"[object Object]"==a?b=a:null!=a&&(b={cx:a,cy:d,rx:k,ry:p});return this.el("ellipse",b)};
w.path=function(a){var d;z(a,"object")&&!z(a,"array")?d=a:a&&(d={d:a});return this.el("path",d)};w.group=w.g=function(a){var d=this.el("g");1==arguments.length&&a&&!a.type?d.attr(a):arguments.length&&d.add(Array.prototype.slice.call(arguments,0));return d};w.svg=function(a,d,k,p,b,q,e,l){var r={};z(a,"object")&&null==d?r=a:(null!=a&&(r.x=a),null!=d&&(r.y=d),null!=k&&(r.width=k),null!=p&&(r.height=p),null!=b&&null!=q&&null!=e&&null!=l&&(r.viewBox=[b,q,e,l]));return this.el("svg",r)};w.mask=function(a){var d=
this.el("mask");1==arguments.length&&a&&!a.type?d.attr(a):arguments.length&&d.add(Array.prototype.slice.call(arguments,0));return d};w.ptrn=function(a,d,k,p,b,q,e,l){if(z(a,"object"))var r=a;else arguments.length?(r={},null!=a&&(r.x=a),null!=d&&(r.y=d),null!=k&&(r.width=k),null!=p&&(r.height=p),null!=b&&null!=q&&null!=e&&null!=l&&(r.viewBox=[b,q,e,l])):r={patternUnits:"userSpaceOnUse"};return this.el("pattern",r)};w.use=function(a){return null!=a?(make("use",this.node),a instanceof v&&(a.attr("id")||
a.attr({id:ID()}),a=a.attr("id")),this.el("use",{"xlink:href":a})):v.prototype.use.call(this)};w.text=function(a,d,k){var p={};z(a,"object")?p=a:null!=a&&(p={x:a,y:d,text:k||""});return this.el("text",p)};w.line=function(a,d,k,p){var b={};z(a,"object")?b=a:null!=a&&(b={x1:a,x2:k,y1:d,y2:p});return this.el("line",b)};w.polyline=function(a){1<arguments.length&&(a=Array.prototype.slice.call(arguments,0));var d={};z(a,"object")&&!z(a,"array")?d=a:null!=a&&(d={points:a});return this.el("polyline",d)};
w.polygon=function(a){1<arguments.length&&(a=Array.prototype.slice.call(arguments,0));var d={};z(a,"object")&&!z(a,"array")?d=a:null!=a&&(d={points:a});return this.el("polygon",d)};(function(){function d(){return this.selectAll("stop")}function n(b,d){var f=e("stop"),k={offset:+d+"%"};b=a.color(b);k["stop-color"]=b.hex;1>b.opacity&&(k["stop-opacity"]=b.opacity);e(f,k);this.node.appendChild(f);return this}function u(){if("linearGradient"==this.type){var b=e(this.node,"x1")||0,d=e(this.node,"x2")||
1,f=e(this.node,"y1")||0,k=e(this.node,"y2")||0;return a._.box(b,f,math.abs(d-b),math.abs(k-f))}b=this.node.r||0;return a._.box((this.node.cx||0.5)-b,(this.node.cy||0.5)-b,2*b,2*b)}function p(a,d){function f(a,b){for(var d=(b-u)/(a-w),e=w;e<a;e++)h[e].offset=+(+u+d*(e-w)).toFixed(2);w=a;u=b}var n=k("snap.util.grad.parse",null,d).firstDefined(),p;if(!n)return null;n.params.unshift(a);p="l"==n.type.toLowerCase()?b.apply(0,n.params):q.apply(0,n.params);n.type!=n.type.toLowerCase()&&e(p.node,{gradientUnits:"userSpaceOnUse"});
var h=n.stops,n=h.length,u=0,w=0;n--;for(var v=0;v<n;v++)"offset"in h[v]&&f(v,h[v].offset);h[n].offset=h[n].offset||100;f(n,h[n].offset);for(v=0;v<=n;v++){var y=h[v];p.addStop(y.color,y.offset)}return p}function b(b,k,p,q,w){b=a._.make("linearGradient",b);b.stops=d;b.addStop=n;b.getBBox=u;null!=k&&e(b.node,{x1:k,y1:p,x2:q,y2:w});return b}function q(b,k,p,q,w,h){b=a._.make("radialGradient",b);b.stops=d;b.addStop=n;b.getBBox=u;null!=k&&e(b.node,{cx:k,cy:p,r:q});null!=w&&null!=h&&e(b.node,{fx:w,fy:h});
return b}var e=a._.$;w.gradient=function(a){return p(this.defs,a)};w.gradientLinear=function(a,d,e,f){return b(this.defs,a,d,e,f)};w.gradientRadial=function(a,b,d,e,f){return q(this.defs,a,b,d,e,f)};w.toString=function(){var b=this.node.ownerDocument,d=b.createDocumentFragment(),b=b.createElement("div"),e=this.node.cloneNode(!0);d.appendChild(b);b.appendChild(e);a._.$(e,{xmlns:"http://www.w3.org/2000/svg"});b=b.innerHTML;d.removeChild(d.firstChild);return b};w.clear=function(){for(var a=this.node.firstChild,
b;a;)b=a.nextSibling,"defs"!=a.tagName?a.parentNode.removeChild(a):w.clear.call({node:a}),a=b}})()});C.plugin(function(a,k,y,M){function A(a){var b=A.ps=A.ps||{};b[a]?b[a].sleep=100:b[a]={sleep:100};setTimeout(function(){for(var d in b)b[L](d)&&d!=a&&(b[d].sleep--,!b[d].sleep&&delete b[d])});return b[a]}function w(a,b,d,e){null==a&&(a=b=d=e=0);null==b&&(b=a.y,d=a.width,e=a.height,a=a.x);return{x:a,y:b,width:d,w:d,height:e,h:e,x2:a+d,y2:b+e,cx:a+d/2,cy:b+e/2,r1:F.min(d,e)/2,r2:F.max(d,e)/2,r0:F.sqrt(d*
d+e*e)/2,path:s(a,b,d,e),vb:[a,b,d,e].join(" ")}}function z(){return this.join(",").replace(N,"$1")}function d(a){a=C(a);a.toString=z;return a}function f(a,b,d,h,f,k,l,n,p){if(null==p)return e(a,b,d,h,f,k,l,n);if(0>p||e(a,b,d,h,f,k,l,n)<p)p=void 0;else{var q=0.5,O=1-q,s;for(s=e(a,b,d,h,f,k,l,n,O);0.01<Z(s-p);)q/=2,O+=(s<p?1:-1)*q,s=e(a,b,d,h,f,k,l,n,O);p=O}return u(a,b,d,h,f,k,l,n,p)}function n(b,d){function e(a){return+(+a).toFixed(3)}return a._.cacher(function(a,h,l){a instanceof k&&(a=a.attr("d"));
a=I(a);for(var n,p,D,q,O="",s={},c=0,t=0,r=a.length;t<r;t++){D=a[t];if("M"==D[0])n=+D[1],p=+D[2];else{q=f(n,p,D[1],D[2],D[3],D[4],D[5],D[6]);if(c+q>h){if(d&&!s.start){n=f(n,p,D[1],D[2],D[3],D[4],D[5],D[6],h-c);O+=["C"+e(n.start.x),e(n.start.y),e(n.m.x),e(n.m.y),e(n.x),e(n.y)];if(l)return O;s.start=O;O=["M"+e(n.x),e(n.y)+"C"+e(n.n.x),e(n.n.y),e(n.end.x),e(n.end.y),e(D[5]),e(D[6])].join();c+=q;n=+D[5];p=+D[6];continue}if(!b&&!d)return n=f(n,p,D[1],D[2],D[3],D[4],D[5],D[6],h-c)}c+=q;n=+D[5];p=+D[6]}O+=
D.shift()+D}s.end=O;return n=b?c:d?s:u(n,p,D[0],D[1],D[2],D[3],D[4],D[5],1)},null,a._.clone)}function u(a,b,d,e,h,f,k,l,n){var p=1-n,q=ma(p,3),s=ma(p,2),c=n*n,t=c*n,r=q*a+3*s*n*d+3*p*n*n*h+t*k,q=q*b+3*s*n*e+3*p*n*n*f+t*l,s=a+2*n*(d-a)+c*(h-2*d+a),t=b+2*n*(e-b)+c*(f-2*e+b),x=d+2*n*(h-d)+c*(k-2*h+d),c=e+2*n*(f-e)+c*(l-2*f+e);a=p*a+n*d;b=p*b+n*e;h=p*h+n*k;f=p*f+n*l;l=90-180*F.atan2(s-x,t-c)/S;return{x:r,y:q,m:{x:s,y:t},n:{x:x,y:c},start:{x:a,y:b},end:{x:h,y:f},alpha:l}}function p(b,d,e,h,f,n,k,l){a.is(b,
"array")||(b=[b,d,e,h,f,n,k,l]);b=U.apply(null,b);return w(b.min.x,b.min.y,b.max.x-b.min.x,b.max.y-b.min.y)}function b(a,b,d){return b>=a.x&&b<=a.x+a.width&&d>=a.y&&d<=a.y+a.height}function q(a,d){a=w(a);d=w(d);return b(d,a.x,a.y)||b(d,a.x2,a.y)||b(d,a.x,a.y2)||b(d,a.x2,a.y2)||b(a,d.x,d.y)||b(a,d.x2,d.y)||b(a,d.x,d.y2)||b(a,d.x2,d.y2)||(a.x<d.x2&&a.x>d.x||d.x<a.x2&&d.x>a.x)&&(a.y<d.y2&&a.y>d.y||d.y<a.y2&&d.y>a.y)}function e(a,b,d,e,h,f,n,k,l){null==l&&(l=1);l=(1<l?1:0>l?0:l)/2;for(var p=[-0.1252,
0.1252,-0.3678,0.3678,-0.5873,0.5873,-0.7699,0.7699,-0.9041,0.9041,-0.9816,0.9816],q=[0.2491,0.2491,0.2335,0.2335,0.2032,0.2032,0.1601,0.1601,0.1069,0.1069,0.0472,0.0472],s=0,c=0;12>c;c++)var t=l*p[c]+l,r=t*(t*(-3*a+9*d-9*h+3*n)+6*a-12*d+6*h)-3*a+3*d,t=t*(t*(-3*b+9*e-9*f+3*k)+6*b-12*e+6*f)-3*b+3*e,s=s+q[c]*F.sqrt(r*r+t*t);return l*s}function l(a,b,d){a=I(a);b=I(b);for(var h,f,l,n,k,s,r,O,x,c,t=d?0:[],w=0,v=a.length;w<v;w++)if(x=a[w],"M"==x[0])h=k=x[1],f=s=x[2];else{"C"==x[0]?(x=[h,f].concat(x.slice(1)),
h=x[6],f=x[7]):(x=[h,f,h,f,k,s,k,s],h=k,f=s);for(var G=0,y=b.length;G<y;G++)if(c=b[G],"M"==c[0])l=r=c[1],n=O=c[2];else{"C"==c[0]?(c=[l,n].concat(c.slice(1)),l=c[6],n=c[7]):(c=[l,n,l,n,r,O,r,O],l=r,n=O);var z;var K=x,B=c;z=d;var H=p(K),J=p(B);if(q(H,J)){for(var H=e.apply(0,K),J=e.apply(0,B),H=~~(H/8),J=~~(J/8),U=[],A=[],F={},M=z?0:[],P=0;P<H+1;P++){var C=u.apply(0,K.concat(P/H));U.push({x:C.x,y:C.y,t:P/H})}for(P=0;P<J+1;P++)C=u.apply(0,B.concat(P/J)),A.push({x:C.x,y:C.y,t:P/J});for(P=0;P<H;P++)for(K=
0;K<J;K++){var Q=U[P],L=U[P+1],B=A[K],C=A[K+1],N=0.001>Z(L.x-Q.x)?"y":"x",S=0.001>Z(C.x-B.x)?"y":"x",R;R=Q.x;var Y=Q.y,V=L.x,ea=L.y,fa=B.x,ga=B.y,ha=C.x,ia=C.y;if(W(R,V)<X(fa,ha)||X(R,V)>W(fa,ha)||W(Y,ea)<X(ga,ia)||X(Y,ea)>W(ga,ia))R=void 0;else{var $=(R*ea-Y*V)*(fa-ha)-(R-V)*(fa*ia-ga*ha),aa=(R*ea-Y*V)*(ga-ia)-(Y-ea)*(fa*ia-ga*ha),ja=(R-V)*(ga-ia)-(Y-ea)*(fa-ha);if(ja){var $=$/ja,aa=aa/ja,ja=+$.toFixed(2),ba=+aa.toFixed(2);R=ja<+X(R,V).toFixed(2)||ja>+W(R,V).toFixed(2)||ja<+X(fa,ha).toFixed(2)||
ja>+W(fa,ha).toFixed(2)||ba<+X(Y,ea).toFixed(2)||ba>+W(Y,ea).toFixed(2)||ba<+X(ga,ia).toFixed(2)||ba>+W(ga,ia).toFixed(2)?void 0:{x:$,y:aa}}else R=void 0}R&&F[R.x.toFixed(4)]!=R.y.toFixed(4)&&(F[R.x.toFixed(4)]=R.y.toFixed(4),Q=Q.t+Z((R[N]-Q[N])/(L[N]-Q[N]))*(L.t-Q.t),B=B.t+Z((R[S]-B[S])/(C[S]-B[S]))*(C.t-B.t),0<=Q&&1>=Q&&0<=B&&1>=B&&(z?M++:M.push({x:R.x,y:R.y,t1:Q,t2:B})))}z=M}else z=z?0:[];if(d)t+=z;else{H=0;for(J=z.length;H<J;H++)z[H].segment1=w,z[H].segment2=G,z[H].bez1=x,z[H].bez2=c;t=t.concat(z)}}}return t}
function r(a){var b=A(a);if(b.bbox)return C(b.bbox);if(!a)return w();a=I(a);for(var d=0,e=0,h=[],f=[],l,n=0,k=a.length;n<k;n++)l=a[n],"M"==l[0]?(d=l[1],e=l[2],h.push(d),f.push(e)):(d=U(d,e,l[1],l[2],l[3],l[4],l[5],l[6]),h=h.concat(d.min.x,d.max.x),f=f.concat(d.min.y,d.max.y),d=l[5],e=l[6]);a=X.apply(0,h);l=X.apply(0,f);h=W.apply(0,h);f=W.apply(0,f);f=w(a,l,h-a,f-l);b.bbox=C(f);return f}function s(a,b,d,e,h){if(h)return[["M",+a+ +h,b],["l",d-2*h,0],["a",h,h,0,0,1,h,h],["l",0,e-2*h],["a",h,h,0,0,1,
-h,h],["l",2*h-d,0],["a",h,h,0,0,1,-h,-h],["l",0,2*h-e],["a",h,h,0,0,1,h,-h],["z"] ];a=[["M",a,b],["l",d,0],["l",0,e],["l",-d,0],["z"] ];a.toString=z;return a}function x(a,b,d,e,h){null==h&&null==e&&(e=d);a=+a;b=+b;d=+d;e=+e;if(null!=h){var f=Math.PI/180,l=a+d*Math.cos(-e*f);a+=d*Math.cos(-h*f);var n=b+d*Math.sin(-e*f);b+=d*Math.sin(-h*f);d=[["M",l,n],["A",d,d,0,+(180<h-e),0,a,b] ]}else d=[["M",a,b],["m",0,-e],["a",d,e,0,1,1,0,2*e],["a",d,e,0,1,1,0,-2*e],["z"] ];d.toString=z;return d}function G(b){var e=
A(b);if(e.abs)return d(e.abs);Q(b,"array")&&Q(b&&b[0],"array")||(b=a.parsePathString(b));if(!b||!b.length)return[["M",0,0] ];var h=[],f=0,l=0,n=0,k=0,p=0;"M"==b[0][0]&&(f=+b[0][1],l=+b[0][2],n=f,k=l,p++,h[0]=["M",f,l]);for(var q=3==b.length&&"M"==b[0][0]&&"R"==b[1][0].toUpperCase()&&"Z"==b[2][0].toUpperCase(),s,r,w=p,c=b.length;w<c;w++){h.push(s=[]);r=b[w];p=r[0];if(p!=p.toUpperCase())switch(s[0]=p.toUpperCase(),s[0]){case "A":s[1]=r[1];s[2]=r[2];s[3]=r[3];s[4]=r[4];s[5]=r[5];s[6]=+r[6]+f;s[7]=+r[7]+
l;break;case "V":s[1]=+r[1]+l;break;case "H":s[1]=+r[1]+f;break;case "R":for(var t=[f,l].concat(r.slice(1)),u=2,v=t.length;u<v;u++)t[u]=+t[u]+f,t[++u]=+t[u]+l;h.pop();h=h.concat(P(t,q));break;case "O":h.pop();t=x(f,l,r[1],r[2]);t.push(t[0]);h=h.concat(t);break;case "U":h.pop();h=h.concat(x(f,l,r[1],r[2],r[3]));s=["U"].concat(h[h.length-1].slice(-2));break;case "M":n=+r[1]+f,k=+r[2]+l;default:for(u=1,v=r.length;u<v;u++)s[u]=+r[u]+(u%2?f:l)}else if("R"==p)t=[f,l].concat(r.slice(1)),h.pop(),h=h.concat(P(t,
q)),s=["R"].concat(r.slice(-2));else if("O"==p)h.pop(),t=x(f,l,r[1],r[2]),t.push(t[0]),h=h.concat(t);else if("U"==p)h.pop(),h=h.concat(x(f,l,r[1],r[2],r[3])),s=["U"].concat(h[h.length-1].slice(-2));else for(t=0,u=r.length;t<u;t++)s[t]=r[t];p=p.toUpperCase();if("O"!=p)switch(s[0]){case "Z":f=+n;l=+k;break;case "H":f=s[1];break;case "V":l=s[1];break;case "M":n=s[s.length-2],k=s[s.length-1];default:f=s[s.length-2],l=s[s.length-1]}}h.toString=z;e.abs=d(h);return h}function h(a,b,d,e){return[a,b,d,e,d,
e]}function J(a,b,d,e,h,f){var l=1/3,n=2/3;return[l*a+n*d,l*b+n*e,l*h+n*d,l*f+n*e,h,f]}function K(b,d,e,h,f,l,n,k,p,s){var r=120*S/180,q=S/180*(+f||0),c=[],t,x=a._.cacher(function(a,b,c){var d=a*F.cos(c)-b*F.sin(c);a=a*F.sin(c)+b*F.cos(c);return{x:d,y:a}});if(s)v=s[0],t=s[1],l=s[2],u=s[3];else{t=x(b,d,-q);b=t.x;d=t.y;t=x(k,p,-q);k=t.x;p=t.y;F.cos(S/180*f);F.sin(S/180*f);t=(b-k)/2;v=(d-p)/2;u=t*t/(e*e)+v*v/(h*h);1<u&&(u=F.sqrt(u),e*=u,h*=u);var u=e*e,w=h*h,u=(l==n?-1:1)*F.sqrt(Z((u*w-u*v*v-w*t*t)/
(u*v*v+w*t*t)));l=u*e*v/h+(b+k)/2;var u=u*-h*t/e+(d+p)/2,v=F.asin(((d-u)/h).toFixed(9));t=F.asin(((p-u)/h).toFixed(9));v=b<l?S-v:v;t=k<l?S-t:t;0>v&&(v=2*S+v);0>t&&(t=2*S+t);n&&v>t&&(v-=2*S);!n&&t>v&&(t-=2*S)}if(Z(t-v)>r){var c=t,w=k,G=p;t=v+r*(n&&t>v?1:-1);k=l+e*F.cos(t);p=u+h*F.sin(t);c=K(k,p,e,h,f,0,n,w,G,[t,c,l,u])}l=t-v;f=F.cos(v);r=F.sin(v);n=F.cos(t);t=F.sin(t);l=F.tan(l/4);e=4/3*e*l;l*=4/3*h;h=[b,d];b=[b+e*r,d-l*f];d=[k+e*t,p-l*n];k=[k,p];b[0]=2*h[0]-b[0];b[1]=2*h[1]-b[1];if(s)return[b,d,k].concat(c);
c=[b,d,k].concat(c).join().split(",");s=[];k=0;for(p=c.length;k<p;k++)s[k]=k%2?x(c[k-1],c[k],q).y:x(c[k],c[k+1],q).x;return s}function U(a,b,d,e,h,f,l,k){for(var n=[],p=[[],[] ],s,r,c,t,q=0;2>q;++q)0==q?(r=6*a-12*d+6*h,s=-3*a+9*d-9*h+3*l,c=3*d-3*a):(r=6*b-12*e+6*f,s=-3*b+9*e-9*f+3*k,c=3*e-3*b),1E-12>Z(s)?1E-12>Z(r)||(s=-c/r,0<s&&1>s&&n.push(s)):(t=r*r-4*c*s,c=F.sqrt(t),0>t||(t=(-r+c)/(2*s),0<t&&1>t&&n.push(t),s=(-r-c)/(2*s),0<s&&1>s&&n.push(s)));for(r=q=n.length;q--;)s=n[q],c=1-s,p[0][q]=c*c*c*a+3*
c*c*s*d+3*c*s*s*h+s*s*s*l,p[1][q]=c*c*c*b+3*c*c*s*e+3*c*s*s*f+s*s*s*k;p[0][r]=a;p[1][r]=b;p[0][r+1]=l;p[1][r+1]=k;p[0].length=p[1].length=r+2;return{min:{x:X.apply(0,p[0]),y:X.apply(0,p[1])},max:{x:W.apply(0,p[0]),y:W.apply(0,p[1])}}}function I(a,b){var e=!b&&A(a);if(!b&&e.curve)return d(e.curve);var f=G(a),l=b&&G(b),n={x:0,y:0,bx:0,by:0,X:0,Y:0,qx:null,qy:null},k={x:0,y:0,bx:0,by:0,X:0,Y:0,qx:null,qy:null},p=function(a,b,c){if(!a)return["C",b.x,b.y,b.x,b.y,b.x,b.y];a[0]in{T:1,Q:1}||(b.qx=b.qy=null);
switch(a[0]){case "M":b.X=a[1];b.Y=a[2];break;case "A":a=["C"].concat(K.apply(0,[b.x,b.y].concat(a.slice(1))));break;case "S":"C"==c||"S"==c?(c=2*b.x-b.bx,b=2*b.y-b.by):(c=b.x,b=b.y);a=["C",c,b].concat(a.slice(1));break;case "T":"Q"==c||"T"==c?(b.qx=2*b.x-b.qx,b.qy=2*b.y-b.qy):(b.qx=b.x,b.qy=b.y);a=["C"].concat(J(b.x,b.y,b.qx,b.qy,a[1],a[2]));break;case "Q":b.qx=a[1];b.qy=a[2];a=["C"].concat(J(b.x,b.y,a[1],a[2],a[3],a[4]));break;case "L":a=["C"].concat(h(b.x,b.y,a[1],a[2]));break;case "H":a=["C"].concat(h(b.x,
b.y,a[1],b.y));break;case "V":a=["C"].concat(h(b.x,b.y,b.x,a[1]));break;case "Z":a=["C"].concat(h(b.x,b.y,b.X,b.Y))}return a},s=function(a,b){if(7<a[b].length){a[b].shift();for(var c=a[b];c.length;)q[b]="A",l&&(u[b]="A"),a.splice(b++,0,["C"].concat(c.splice(0,6)));a.splice(b,1);v=W(f.length,l&&l.length||0)}},r=function(a,b,c,d,e){a&&b&&"M"==a[e][0]&&"M"!=b[e][0]&&(b.splice(e,0,["M",d.x,d.y]),c.bx=0,c.by=0,c.x=a[e][1],c.y=a[e][2],v=W(f.length,l&&l.length||0))},q=[],u=[],c="",t="",x=0,v=W(f.length,
l&&l.length||0);for(;x<v;x++){f[x]&&(c=f[x][0]);"C"!=c&&(q[x]=c,x&&(t=q[x-1]));f[x]=p(f[x],n,t);"A"!=q[x]&&"C"==c&&(q[x]="C");s(f,x);l&&(l[x]&&(c=l[x][0]),"C"!=c&&(u[x]=c,x&&(t=u[x-1])),l[x]=p(l[x],k,t),"A"!=u[x]&&"C"==c&&(u[x]="C"),s(l,x));r(f,l,n,k,x);r(l,f,k,n,x);var w=f[x],z=l&&l[x],y=w.length,U=l&&z.length;n.x=w[y-2];n.y=w[y-1];n.bx=$(w[y-4])||n.x;n.by=$(w[y-3])||n.y;k.bx=l&&($(z[U-4])||k.x);k.by=l&&($(z[U-3])||k.y);k.x=l&&z[U-2];k.y=l&&z[U-1]}l||(e.curve=d(f));return l?[f,l]:f}function P(a,
b){for(var d=[],e=0,h=a.length;h-2*!b>e;e+=2){var f=[{x:+a[e-2],y:+a[e-1]},{x:+a[e],y:+a[e+1]},{x:+a[e+2],y:+a[e+3]},{x:+a[e+4],y:+a[e+5]}];b?e?h-4==e?f[3]={x:+a[0],y:+a[1]}:h-2==e&&(f[2]={x:+a[0],y:+a[1]},f[3]={x:+a[2],y:+a[3]}):f[0]={x:+a[h-2],y:+a[h-1]}:h-4==e?f[3]=f[2]:e||(f[0]={x:+a[e],y:+a[e+1]});d.push(["C",(-f[0].x+6*f[1].x+f[2].x)/6,(-f[0].y+6*f[1].y+f[2].y)/6,(f[1].x+6*f[2].x-f[3].x)/6,(f[1].y+6*f[2].y-f[3].y)/6,f[2].x,f[2].y])}return d}y=k.prototype;var Q=a.is,C=a._.clone,L="hasOwnProperty",
N=/,?([a-z]),?/gi,$=parseFloat,F=Math,S=F.PI,X=F.min,W=F.max,ma=F.pow,Z=F.abs;M=n(1);var na=n(),ba=n(0,1),V=a._unit2px;a.path=A;a.path.getTotalLength=M;a.path.getPointAtLength=na;a.path.getSubpath=function(a,b,d){if(1E-6>this.getTotalLength(a)-d)return ba(a,b).end;a=ba(a,d,1);return b?ba(a,b).end:a};y.getTotalLength=function(){if(this.node.getTotalLength)return this.node.getTotalLength()};y.getPointAtLength=function(a){return na(this.attr("d"),a)};y.getSubpath=function(b,d){return a.path.getSubpath(this.attr("d"),
b,d)};a._.box=w;a.path.findDotsAtSegment=u;a.path.bezierBBox=p;a.path.isPointInsideBBox=b;a.path.isBBoxIntersect=q;a.path.intersection=function(a,b){return l(a,b)};a.path.intersectionNumber=function(a,b){return l(a,b,1)};a.path.isPointInside=function(a,d,e){var h=r(a);return b(h,d,e)&&1==l(a,[["M",d,e],["H",h.x2+10] ],1)%2};a.path.getBBox=r;a.path.get={path:function(a){return a.attr("path")},circle:function(a){a=V(a);return x(a.cx,a.cy,a.r)},ellipse:function(a){a=V(a);return x(a.cx||0,a.cy||0,a.rx,
a.ry)},rect:function(a){a=V(a);return s(a.x||0,a.y||0,a.width,a.height,a.rx,a.ry)},image:function(a){a=V(a);return s(a.x||0,a.y||0,a.width,a.height)},line:function(a){return"M"+[a.attr("x1")||0,a.attr("y1")||0,a.attr("x2"),a.attr("y2")]},polyline:function(a){return"M"+a.attr("points")},polygon:function(a){return"M"+a.attr("points")+"z"},deflt:function(a){a=a.node.getBBox();return s(a.x,a.y,a.width,a.height)}};a.path.toRelative=function(b){var e=A(b),h=String.prototype.toLowerCase;if(e.rel)return d(e.rel);
a.is(b,"array")&&a.is(b&&b[0],"array")||(b=a.parsePathString(b));var f=[],l=0,n=0,k=0,p=0,s=0;"M"==b[0][0]&&(l=b[0][1],n=b[0][2],k=l,p=n,s++,f.push(["M",l,n]));for(var r=b.length;s<r;s++){var q=f[s]=[],x=b[s];if(x[0]!=h.call(x[0]))switch(q[0]=h.call(x[0]),q[0]){case "a":q[1]=x[1];q[2]=x[2];q[3]=x[3];q[4]=x[4];q[5]=x[5];q[6]=+(x[6]-l).toFixed(3);q[7]=+(x[7]-n).toFixed(3);break;case "v":q[1]=+(x[1]-n).toFixed(3);break;case "m":k=x[1],p=x[2];default:for(var c=1,t=x.length;c<t;c++)q[c]=+(x[c]-(c%2?l:
n)).toFixed(3)}else for(f[s]=[],"m"==x[0]&&(k=x[1]+l,p=x[2]+n),q=0,c=x.length;q<c;q++)f[s][q]=x[q];x=f[s].length;switch(f[s][0]){case "z":l=k;n=p;break;case "h":l+=+f[s][x-1];break;case "v":n+=+f[s][x-1];break;default:l+=+f[s][x-2],n+=+f[s][x-1]}}f.toString=z;e.rel=d(f);return f};a.path.toAbsolute=G;a.path.toCubic=I;a.path.map=function(a,b){if(!b)return a;var d,e,h,f,l,n,k;a=I(a);h=0;for(l=a.length;h<l;h++)for(k=a[h],f=1,n=k.length;f<n;f+=2)d=b.x(k[f],k[f+1]),e=b.y(k[f],k[f+1]),k[f]=d,k[f+1]=e;return a};
a.path.toString=z;a.path.clone=d});C.plugin(function(a,v,y,C){var A=Math.max,w=Math.min,z=function(a){this.items=[];this.bindings={};this.length=0;this.type="set";if(a)for(var f=0,n=a.length;f<n;f++)a[f]&&(this[this.items.length]=this.items[this.items.length]=a[f],this.length++)};v=z.prototype;v.push=function(){for(var a,f,n=0,k=arguments.length;n<k;n++)if(a=arguments[n])f=this.items.length,this[f]=this.items[f]=a,this.length++;return this};v.pop=function(){this.length&&delete this[this.length--];
return this.items.pop()};v.forEach=function(a,f){for(var n=0,k=this.items.length;n<k&&!1!==a.call(f,this.items[n],n);n++);return this};v.animate=function(d,f,n,u){"function"!=typeof n||n.length||(u=n,n=L.linear);d instanceof a._.Animation&&(u=d.callback,n=d.easing,f=n.dur,d=d.attr);var p=arguments;if(a.is(d,"array")&&a.is(p[p.length-1],"array"))var b=!0;var q,e=function(){q?this.b=q:q=this.b},l=0,r=u&&function(){l++==this.length&&u.call(this)};return this.forEach(function(a,l){k.once("snap.animcreated."+
a.id,e);b?p[l]&&a.animate.apply(a,p[l]):a.animate(d,f,n,r)})};v.remove=function(){for(;this.length;)this.pop().remove();return this};v.bind=function(a,f,k){var u={};if("function"==typeof f)this.bindings[a]=f;else{var p=k||a;this.bindings[a]=function(a){u[p]=a;f.attr(u)}}return this};v.attr=function(a){var f={},k;for(k in a)if(this.bindings[k])this.bindings[k](a[k]);else f[k]=a[k];a=0;for(k=this.items.length;a<k;a++)this.items[a].attr(f);return this};v.clear=function(){for(;this.length;)this.pop()};
v.splice=function(a,f,k){a=0>a?A(this.length+a,0):a;f=A(0,w(this.length-a,f));var u=[],p=[],b=[],q;for(q=2;q<arguments.length;q++)b.push(arguments[q]);for(q=0;q<f;q++)p.push(this[a+q]);for(;q<this.length-a;q++)u.push(this[a+q]);var e=b.length;for(q=0;q<e+u.length;q++)this.items[a+q]=this[a+q]=q<e?b[q]:u[q-e];for(q=this.items.length=this.length-=f-e;this[q];)delete this[q++];return new z(p)};v.exclude=function(a){for(var f=0,k=this.length;f<k;f++)if(this[f]==a)return this.splice(f,1),!0;return!1};
v.insertAfter=function(a){for(var f=this.items.length;f--;)this.items[f].insertAfter(a);return this};v.getBBox=function(){for(var a=[],f=[],k=[],u=[],p=this.items.length;p--;)if(!this.items[p].removed){var b=this.items[p].getBBox();a.push(b.x);f.push(b.y);k.push(b.x+b.width);u.push(b.y+b.height)}a=w.apply(0,a);f=w.apply(0,f);k=A.apply(0,k);u=A.apply(0,u);return{x:a,y:f,x2:k,y2:u,width:k-a,height:u-f,cx:a+(k-a)/2,cy:f+(u-f)/2}};v.clone=function(a){a=new z;for(var f=0,k=this.items.length;f<k;f++)a.push(this.items[f].clone());
return a};v.toString=function(){return"Snap\u2018s set"};v.type="set";a.set=function(){var a=new z;arguments.length&&a.push.apply(a,Array.prototype.slice.call(arguments,0));return a}});C.plugin(function(a,v,y,C){function A(a){var b=a[0];switch(b.toLowerCase()){case "t":return[b,0,0];case "m":return[b,1,0,0,1,0,0];case "r":return 4==a.length?[b,0,a[2],a[3] ]:[b,0];case "s":return 5==a.length?[b,1,1,a[3],a[4] ]:3==a.length?[b,1,1]:[b,1]}}function w(b,d,f){d=q(d).replace(/\.{3}|\u2026/g,b);b=a.parseTransformString(b)||
[];d=a.parseTransformString(d)||[];for(var k=Math.max(b.length,d.length),p=[],v=[],h=0,w,z,y,I;h<k;h++){y=b[h]||A(d[h]);I=d[h]||A(y);if(y[0]!=I[0]||"r"==y[0].toLowerCase()&&(y[2]!=I[2]||y[3]!=I[3])||"s"==y[0].toLowerCase()&&(y[3]!=I[3]||y[4]!=I[4])){b=a._.transform2matrix(b,f());d=a._.transform2matrix(d,f());p=[["m",b.a,b.b,b.c,b.d,b.e,b.f] ];v=[["m",d.a,d.b,d.c,d.d,d.e,d.f] ];break}p[h]=[];v[h]=[];w=0;for(z=Math.max(y.length,I.length);w<z;w++)w in y&&(p[h][w]=y[w]),w in I&&(v[h][w]=I[w])}return{from:u(p),
to:u(v),f:n(p)}}function z(a){return a}function d(a){return function(b){return+b.toFixed(3)+a}}function f(b){return a.rgb(b[0],b[1],b[2])}function n(a){var b=0,d,f,k,n,h,p,q=[];d=0;for(f=a.length;d<f;d++){h="[";p=['"'+a[d][0]+'"'];k=1;for(n=a[d].length;k<n;k++)p[k]="val["+b++ +"]";h+=p+"]";q[d]=h}return Function("val","return Snap.path.toString.call(["+q+"])")}function u(a){for(var b=[],d=0,f=a.length;d<f;d++)for(var k=1,n=a[d].length;k<n;k++)b.push(a[d][k]);return b}var p={},b=/[a-z]+$/i,q=String;
p.stroke=p.fill="colour";v.prototype.equal=function(a,b){return k("snap.util.equal",this,a,b).firstDefined()};k.on("snap.util.equal",function(e,k){var r,s;r=q(this.attr(e)||"");var x=this;if(r==+r&&k==+k)return{from:+r,to:+k,f:z};if("colour"==p[e])return r=a.color(r),s=a.color(k),{from:[r.r,r.g,r.b,r.opacity],to:[s.r,s.g,s.b,s.opacity],f:f};if("transform"==e||"gradientTransform"==e||"patternTransform"==e)return k instanceof a.Matrix&&(k=k.toTransformString()),a._.rgTransform.test(k)||(k=a._.svgTransform2string(k)),
w(r,k,function(){return x.getBBox(1)});if("d"==e||"path"==e)return r=a.path.toCubic(r,k),{from:u(r[0]),to:u(r[1]),f:n(r[0])};if("points"==e)return r=q(r).split(a._.separator),s=q(k).split(a._.separator),{from:r,to:s,f:function(a){return a}};aUnit=r.match(b);s=q(k).match(b);return aUnit&&aUnit==s?{from:parseFloat(r),to:parseFloat(k),f:d(aUnit)}:{from:this.asPX(e),to:this.asPX(e,k),f:z}})});C.plugin(function(a,v,y,C){var A=v.prototype,w="createTouch"in C.doc;v="click dblclick mousedown mousemove mouseout mouseover mouseup touchstart touchmove touchend touchcancel".split(" ");
var z={mousedown:"touchstart",mousemove:"touchmove",mouseup:"touchend"},d=function(a,b){var d="y"==a?"scrollTop":"scrollLeft",e=b&&b.node?b.node.ownerDocument:C.doc;return e[d in e.documentElement?"documentElement":"body"][d]},f=function(){this.returnValue=!1},n=function(){return this.originalEvent.preventDefault()},u=function(){this.cancelBubble=!0},p=function(){return this.originalEvent.stopPropagation()},b=function(){if(C.doc.addEventListener)return function(a,b,e,f){var k=w&&z[b]?z[b]:b,l=function(k){var l=
d("y",f),q=d("x",f);if(w&&z.hasOwnProperty(b))for(var r=0,u=k.targetTouches&&k.targetTouches.length;r<u;r++)if(k.targetTouches[r].target==a||a.contains(k.targetTouches[r].target)){u=k;k=k.targetTouches[r];k.originalEvent=u;k.preventDefault=n;k.stopPropagation=p;break}return e.call(f,k,k.clientX+q,k.clientY+l)};b!==k&&a.addEventListener(b,l,!1);a.addEventListener(k,l,!1);return function(){b!==k&&a.removeEventListener(b,l,!1);a.removeEventListener(k,l,!1);return!0}};if(C.doc.attachEvent)return function(a,
b,e,h){var k=function(a){a=a||h.node.ownerDocument.window.event;var b=d("y",h),k=d("x",h),k=a.clientX+k,b=a.clientY+b;a.preventDefault=a.preventDefault||f;a.stopPropagation=a.stopPropagation||u;return e.call(h,a,k,b)};a.attachEvent("on"+b,k);return function(){a.detachEvent("on"+b,k);return!0}}}(),q=[],e=function(a){for(var b=a.clientX,e=a.clientY,f=d("y"),l=d("x"),n,p=q.length;p--;){n=q[p];if(w)for(var r=a.touches&&a.touches.length,u;r--;){if(u=a.touches[r],u.identifier==n.el._drag.id||n.el.node.contains(u.target)){b=
u.clientX;e=u.clientY;(a.originalEvent?a.originalEvent:a).preventDefault();break}}else a.preventDefault();b+=l;e+=f;k("snap.drag.move."+n.el.id,n.move_scope||n.el,b-n.el._drag.x,e-n.el._drag.y,b,e,a)}},l=function(b){a.unmousemove(e).unmouseup(l);for(var d=q.length,f;d--;)f=q[d],f.el._drag={},k("snap.drag.end."+f.el.id,f.end_scope||f.start_scope||f.move_scope||f.el,b);q=[]};for(y=v.length;y--;)(function(d){a[d]=A[d]=function(e,f){a.is(e,"function")&&(this.events=this.events||[],this.events.push({name:d,
f:e,unbind:b(this.node||document,d,e,f||this)}));return this};a["un"+d]=A["un"+d]=function(a){for(var b=this.events||[],e=b.length;e--;)if(b[e].name==d&&(b[e].f==a||!a)){b[e].unbind();b.splice(e,1);!b.length&&delete this.events;break}return this}})(v[y]);A.hover=function(a,b,d,e){return this.mouseover(a,d).mouseout(b,e||d)};A.unhover=function(a,b){return this.unmouseover(a).unmouseout(b)};var r=[];A.drag=function(b,d,f,h,n,p){function u(r,v,w){(r.originalEvent||r).preventDefault();this._drag.x=v;
this._drag.y=w;this._drag.id=r.identifier;!q.length&&a.mousemove(e).mouseup(l);q.push({el:this,move_scope:h,start_scope:n,end_scope:p});d&&k.on("snap.drag.start."+this.id,d);b&&k.on("snap.drag.move."+this.id,b);f&&k.on("snap.drag.end."+this.id,f);k("snap.drag.start."+this.id,n||h||this,v,w,r)}if(!arguments.length){var v;return this.drag(function(a,b){this.attr({transform:v+(v?"T":"t")+[a,b]})},function(){v=this.transform().local})}this._drag={};r.push({el:this,start:u});this.mousedown(u);return this};
A.undrag=function(){for(var b=r.length;b--;)r[b].el==this&&(this.unmousedown(r[b].start),r.splice(b,1),k.unbind("snap.drag.*."+this.id));!r.length&&a.unmousemove(e).unmouseup(l);return this}});C.plugin(function(a,v,y,C){y=y.prototype;var A=/^\s*url\((.+)\)/,w=String,z=a._.$;a.filter={};y.filter=function(d){var f=this;"svg"!=f.type&&(f=f.paper);d=a.parse(w(d));var k=a._.id(),u=z("filter");z(u,{id:k,filterUnits:"userSpaceOnUse"});u.appendChild(d.node);f.defs.appendChild(u);return new v(u)};k.on("snap.util.getattr.filter",
function(){k.stop();var d=z(this.node,"filter");if(d)return(d=w(d).match(A))&&a.select(d[1])});k.on("snap.util.attr.filter",function(d){if(d instanceof v&&"filter"==d.type){k.stop();var f=d.node.id;f||(z(d.node,{id:d.id}),f=d.id);z(this.node,{filter:a.url(f)})}d&&"none"!=d||(k.stop(),this.node.removeAttribute("filter"))});a.filter.blur=function(d,f){null==d&&(d=2);return a.format('<feGaussianBlur stdDeviation="{def}"/>',{def:null==f?d:[d,f]})};a.filter.blur.toString=function(){return this()};a.filter.shadow=
function(d,f,k,u,p){"string"==typeof k&&(p=u=k,k=4);"string"!=typeof u&&(p=u,u="#000");null==k&&(k=4);null==p&&(p=1);null==d&&(d=0,f=2);null==f&&(f=d);u=a.color(u||"#000");return a.format('<feGaussianBlur in="SourceAlpha" stdDeviation="{blur}"/><feOffset dx="{dx}" dy="{dy}" result="offsetblur"/><feFlood flood-color="{color}"/><feComposite in2="offsetblur" operator="in"/><feComponentTransfer><feFuncA type="linear" slope="{opacity}"/></feComponentTransfer><feMerge><feMergeNode/><feMergeNode in="SourceGraphic"/></feMerge>',
{color:u,dx:d,dy:f,blur:k,opacity:p})};a.filter.shadow.toString=function(){return this()};a.filter.grayscale=function(d){null==d&&(d=1);return a.format('<feColorMatrix type="matrix" values="{a} {b} {c} 0 0 {d} {e} {f} 0 0 {g} {b} {h} 0 0 0 0 0 1 0"/>',{a:0.2126+0.7874*(1-d),b:0.7152-0.7152*(1-d),c:0.0722-0.0722*(1-d),d:0.2126-0.2126*(1-d),e:0.7152+0.2848*(1-d),f:0.0722-0.0722*(1-d),g:0.2126-0.2126*(1-d),h:0.0722+0.9278*(1-d)})};a.filter.grayscale.toString=function(){return this()};a.filter.sepia=
function(d){null==d&&(d=1);return a.format('<feColorMatrix type="matrix" values="{a} {b} {c} 0 0 {d} {e} {f} 0 0 {g} {h} {i} 0 0 0 0 0 1 0"/>',{a:0.393+0.607*(1-d),b:0.769-0.769*(1-d),c:0.189-0.189*(1-d),d:0.349-0.349*(1-d),e:0.686+0.314*(1-d),f:0.168-0.168*(1-d),g:0.272-0.272*(1-d),h:0.534-0.534*(1-d),i:0.131+0.869*(1-d)})};a.filter.sepia.toString=function(){return this()};a.filter.saturate=function(d){null==d&&(d=1);return a.format('<feColorMatrix type="saturate" values="{amount}"/>',{amount:1-
d})};a.filter.saturate.toString=function(){return this()};a.filter.hueRotate=function(d){return a.format('<feColorMatrix type="hueRotate" values="{angle}"/>',{angle:d||0})};a.filter.hueRotate.toString=function(){return this()};a.filter.invert=function(d){null==d&&(d=1);return a.format('<feComponentTransfer><feFuncR type="table" tableValues="{amount} {amount2}"/><feFuncG type="table" tableValues="{amount} {amount2}"/><feFuncB type="table" tableValues="{amount} {amount2}"/></feComponentTransfer>',{amount:d,
amount2:1-d})};a.filter.invert.toString=function(){return this()};a.filter.brightness=function(d){null==d&&(d=1);return a.format('<feComponentTransfer><feFuncR type="linear" slope="{amount}"/><feFuncG type="linear" slope="{amount}"/><feFuncB type="linear" slope="{amount}"/></feComponentTransfer>',{amount:d})};a.filter.brightness.toString=function(){return this()};a.filter.contrast=function(d){null==d&&(d=1);return a.format('<feComponentTransfer><feFuncR type="linear" slope="{amount}" intercept="{amount2}"/><feFuncG type="linear" slope="{amount}" intercept="{amount2}"/><feFuncB type="linear" slope="{amount}" intercept="{amount2}"/></feComponentTransfer>',
{amount:d,amount2:0.5-d/2})};a.filter.contrast.toString=function(){return this()}});return C});

]]> </script>
<script> <![CDATA[

(function (glob, factory) {
    // AMD support
    if (typeof define === "function" && define.amd) {
        // Define as an anonymous module
        define("Gadfly", ["Snap.svg"], function (Snap) {
            return factory(Snap);
        });
    } else {
        // Browser globals (glob is window)
        // Snap adds itself to window
        glob.Gadfly = factory(glob.Snap);
    }
}(this, function (Snap) {

var Gadfly = {};

// Get an x/y coordinate value in pixels
var xPX = function(fig, x) {
    var client_box = fig.node.getBoundingClientRect();
    return x * fig.node.viewBox.baseVal.width / client_box.width;
};

var yPX = function(fig, y) {
    var client_box = fig.node.getBoundingClientRect();
    return y * fig.node.viewBox.baseVal.height / client_box.height;
};


Snap.plugin(function (Snap, Element, Paper, global) {
    // Traverse upwards from a snap element to find and return the first
    // note with the "plotroot" class.
    Element.prototype.plotroot = function () {
        var element = this;
        while (!element.hasClass("plotroot") && element.parent() != null) {
            element = element.parent();
        }
        return element;
    };

    Element.prototype.svgroot = function () {
        var element = this;
        while (element.node.nodeName != "svg" && element.parent() != null) {
            element = element.parent();
        }
        return element;
    };

    Element.prototype.plotbounds = function () {
        var root = this.plotroot()
        var bbox = root.select(".guide.background").node.getBBox();
        return {
            x0: bbox.x,
            x1: bbox.x + bbox.width,
            y0: bbox.y,
            y1: bbox.y + bbox.height
        };
    };

    Element.prototype.plotcenter = function () {
        var root = this.plotroot()
        var bbox = root.select(".guide.background").node.getBBox();
        return {
            x: bbox.x + bbox.width / 2,
            y: bbox.y + bbox.height / 2
        };
    };

    // Emulate IE style mouseenter/mouseleave events, since Microsoft always
    // does everything right.
    // See: http://www.dynamic-tools.net/toolbox/isMouseLeaveOrEnter/
    var events = ["mouseenter", "mouseleave"];

    for (i in events) {
        (function (event_name) {
            var event_name = events[i];
            Element.prototype[event_name] = function (fn, scope) {
                if (Snap.is(fn, "function")) {
                    var fn2 = function (event) {
                        if (event.type != "mouseover" && event.type != "mouseout") {
                            return;
                        }

                        var reltg = event.relatedTarget ? event.relatedTarget :
                            event.type == "mouseout" ? event.toElement : event.fromElement;
                        while (reltg && reltg != this.node) reltg = reltg.parentNode;

                        if (reltg != this.node) {
                            return fn.apply(this, event);
                        }
                    };

                    if (event_name == "mouseenter") {
                        this.mouseover(fn2, scope);
                    } else {
                        this.mouseout(fn2, scope);
                    }
                }
                return this;
            };
        })(events[i]);
    }


    Element.prototype.mousewheel = function (fn, scope) {
        if (Snap.is(fn, "function")) {
            var el = this;
            var fn2 = function (event) {
                fn.apply(el, [event]);
            };
        }

        this.node.addEventListener(
            /Firefox/i.test(navigator.userAgent) ? "DOMMouseScroll" : "mousewheel",
            fn2);

        return this;
    };


    // Snap's attr function can be too slow for things like panning/zooming.
    // This is a function to directly update element attributes without going
    // through eve.
    Element.prototype.attribute = function(key, val) {
        if (val === undefined) {
            return this.node.getAttribute(key);
        } else {
            this.node.setAttribute(key, val);
            return this;
        }
    };

    Element.prototype.init_gadfly = function() {
        this.mouseenter(Gadfly.plot_mouseover)
            .mouseleave(Gadfly.plot_mouseout)
            .dblclick(Gadfly.plot_dblclick)
            .mousewheel(Gadfly.guide_background_scroll)
            .drag(Gadfly.guide_background_drag_onmove,
                  Gadfly.guide_background_drag_onstart,
                  Gadfly.guide_background_drag_onend);
        this.mouseenter(function (event) {
            init_pan_zoom(this.plotroot());
        });
        return this;
    };
});


// When the plot is moused over, emphasize the grid lines.
Gadfly.plot_mouseover = function(event) {
    var root = this.plotroot();

    var keyboard_zoom = function(event) {
        if (event.which == 187) { // plus
            increase_zoom_by_position(root, 0.1, true);
        } else if (event.which == 189) { // minus
            increase_zoom_by_position(root, -0.1, true);
        }
    };
    root.data("keyboard_zoom", keyboard_zoom);
    window.addEventListener("keyup", keyboard_zoom);

    var xgridlines = root.select(".xgridlines"),
        ygridlines = root.select(".ygridlines");

    xgridlines.data("unfocused_strokedash",
                    xgridlines.attribute("stroke-dasharray").replace(/(\d)(,|$)/g, "$1mm$2"));
    ygridlines.data("unfocused_strokedash",
                    ygridlines.attribute("stroke-dasharray").replace(/(\d)(,|$)/g, "$1mm$2"));

    // emphasize grid lines
    var destcolor = root.data("focused_xgrid_color");
    xgridlines.attribute("stroke-dasharray", "none")
              .selectAll("path")
              .animate({stroke: destcolor}, 250);

    destcolor = root.data("focused_ygrid_color");
    ygridlines.attribute("stroke-dasharray", "none")
              .selectAll("path")
              .animate({stroke: destcolor}, 250);

    // reveal zoom slider
    root.select(".zoomslider")
        .animate({opacity: 1.0}, 250);
};

// Reset pan and zoom on double click
Gadfly.plot_dblclick = function(event) {
  set_plot_pan_zoom(this.plotroot(), 0.0, 0.0, 1.0);
};

// Unemphasize grid lines on mouse out.
Gadfly.plot_mouseout = function(event) {
    var root = this.plotroot();

    window.removeEventListener("keyup", root.data("keyboard_zoom"));
    root.data("keyboard_zoom", undefined);

    var xgridlines = root.select(".xgridlines"),
        ygridlines = root.select(".ygridlines");

    var destcolor = root.data("unfocused_xgrid_color");

    xgridlines.attribute("stroke-dasharray", xgridlines.data("unfocused_strokedash"))
              .selectAll("path")
              .animate({stroke: destcolor}, 250);

    destcolor = root.data("unfocused_ygrid_color");
    ygridlines.attribute("stroke-dasharray", ygridlines.data("unfocused_strokedash"))
              .selectAll("path")
              .animate({stroke: destcolor}, 250);

    // hide zoom slider
    root.select(".zoomslider")
        .animate({opacity: 0.0}, 250);
};


var set_geometry_transform = function(root, tx, ty, scale) {
    var xscalable = root.hasClass("xscalable"),
        yscalable = root.hasClass("yscalable");

    var old_scale = root.data("scale");

    var xscale = xscalable ? scale : 1.0,
        yscale = yscalable ? scale : 1.0;

    tx = xscalable ? tx : 0.0;
    ty = yscalable ? ty : 0.0;

    var t = new Snap.Matrix().translate(tx, ty).scale(xscale, yscale);

    root.selectAll(".geometry, image")
        .forEach(function (element, i) {
            element.transform(t);
        });

    bounds = root.plotbounds();

    if (yscalable) {
        var xfixed_t = new Snap.Matrix().translate(0, ty).scale(1.0, yscale);
        root.selectAll(".xfixed")
            .forEach(function (element, i) {
                element.transform(xfixed_t);
            });

        root.select(".ylabels")
            .transform(xfixed_t)
            .selectAll("text")
            .forEach(function (element, i) {
                if (element.attribute("gadfly:inscale") == "true") {
                    var cx = element.asPX("x"),
                        cy = element.asPX("y");
                    var st = element.data("static_transform");
                    unscale_t = new Snap.Matrix();
                    unscale_t.scale(1, 1/scale, cx, cy).add(st);
                    element.transform(unscale_t);

                    var y = cy * scale + ty;
                    element.attr("visibility",
                        bounds.y0 <= y && y <= bounds.y1 ? "visible" : "hidden");
                }
            });
    }

    if (xscalable) {
        var yfixed_t = new Snap.Matrix().translate(tx, 0).scale(xscale, 1.0);
        var xtrans = new Snap.Matrix().translate(tx, 0);
        root.selectAll(".yfixed")
            .forEach(function (element, i) {
                element.transform(yfixed_t);
            });

        root.select(".xlabels")
            .transform(yfixed_t)
            .selectAll("text")
            .forEach(function (element, i) {
                if (element.attribute("gadfly:inscale") == "true") {
                    var cx = element.asPX("x"),
                        cy = element.asPX("y");
                    var st = element.data("static_transform");
                    unscale_t = new Snap.Matrix();
                    unscale_t.scale(1/scale, 1, cx, cy).add(st);

                    element.transform(unscale_t);

                    var x = cx * scale + tx;
                    element.attr("visibility",
                        bounds.x0 <= x && x <= bounds.x1 ? "visible" : "hidden");
                    }
            });
    }

    // we must unscale anything that is scale invariance: widths, raiduses, etc.
    var size_attribs = ["font-size"];
    var unscaled_selection = ".geometry, .geometry *";
    if (xscalable) {
        size_attribs.push("rx");
        unscaled_selection += ", .xgridlines";
    }
    if (yscalable) {
        size_attribs.push("ry");
        unscaled_selection += ", .ygridlines";
    }

    root.selectAll(unscaled_selection)
        .forEach(function (element, i) {
            // circle need special help
            if (element.node.nodeName == "circle") {
                var cx = element.attribute("cx"),
                    cy = element.attribute("cy");
                unscale_t = new Snap.Matrix().scale(1/xscale, 1/yscale,
                                                        cx, cy);
                element.transform(unscale_t);
                return;
            }

            for (i in size_attribs) {
                var key = size_attribs[i];
                var val = parseFloat(element.attribute(key));
                if (val !== undefined && val != 0 && !isNaN(val)) {
                    element.attribute(key, val * old_scale / scale);
                }
            }
        });
};


// Find the most appropriate tick scale and update label visibility.
var update_tickscale = function(root, scale, axis) {
    if (!root.hasClass(axis + "scalable")) return;

    var tickscales = root.data(axis + "tickscales");
    var best_tickscale = 1.0;
    var best_tickscale_dist = Infinity;
    for (tickscale in tickscales) {
        var dist = Math.abs(Math.log(tickscale) - Math.log(scale));
        if (dist < best_tickscale_dist) {
            best_tickscale_dist = dist;
            best_tickscale = tickscale;
        }
    }

    if (best_tickscale != root.data(axis + "tickscale")) {
        root.data(axis + "tickscale", best_tickscale);
        var mark_inscale_gridlines = function (element, i) {
            var inscale = element.attr("gadfly:scale") == best_tickscale;
            element.attribute("gadfly:inscale", inscale);
            element.attr("visibility", inscale ? "visible" : "hidden");
        };

        var mark_inscale_labels = function (element, i) {
            var inscale = element.attr("gadfly:scale") == best_tickscale;
            element.attribute("gadfly:inscale", inscale);
            element.attr("visibility", inscale ? "visible" : "hidden");
        };

        root.select("." + axis + "gridlines").selectAll("path").forEach(mark_inscale_gridlines);
        root.select("." + axis + "labels").selectAll("text").forEach(mark_inscale_labels);
    }
};


var set_plot_pan_zoom = function(root, tx, ty, scale) {
    var old_scale = root.data("scale");
    var bounds = root.plotbounds();

    var width = bounds.x1 - bounds.x0,
        height = bounds.y1 - bounds.y0;

    // compute the viewport derived from tx, ty, and scale
    var x_min = -width * scale - (scale * width - width),
        x_max = width * scale,
        y_min = -height * scale - (scale * height - height),
        y_max = height * scale;

    var x0 = bounds.x0 - scale * bounds.x0,
        y0 = bounds.y0 - scale * bounds.y0;

    var tx = Math.max(Math.min(tx - x0, x_max), x_min),
        ty = Math.max(Math.min(ty - y0, y_max), y_min);

    tx += x0;
    ty += y0;

    // when the scale change, we may need to alter which set of
    // ticks is being displayed
    if (scale != old_scale) {
        update_tickscale(root, scale, "x");
        update_tickscale(root, scale, "y");
    }

    set_geometry_transform(root, tx, ty, scale);

    root.data("scale", scale);
    root.data("tx", tx);
    root.data("ty", ty);
};


var scale_centered_translation = function(root, scale) {
    var bounds = root.plotbounds();

    var width = bounds.x1 - bounds.x0,
        height = bounds.y1 - bounds.y0;

    var tx0 = root.data("tx"),
        ty0 = root.data("ty");

    var scale0 = root.data("scale");

    // how off from center the current view is
    var xoff = tx0 - (bounds.x0 * (1 - scale0) + (width * (1 - scale0)) / 2),
        yoff = ty0 - (bounds.y0 * (1 - scale0) + (height * (1 - scale0)) / 2);

    // rescale offsets
    xoff = xoff * scale / scale0;
    yoff = yoff * scale / scale0;

    // adjust for the panel position being scaled
    var x_edge_adjust = bounds.x0 * (1 - scale),
        y_edge_adjust = bounds.y0 * (1 - scale);

    return {
        x: xoff + x_edge_adjust + (width - width * scale) / 2,
        y: yoff + y_edge_adjust + (height - height * scale) / 2
    };
};


// Initialize data for panning zooming if it isn't already.
var init_pan_zoom = function(root) {
    if (root.data("zoompan-ready")) {
        return;
    }

    // The non-scaling-stroke trick. Rather than try to correct for the
    // stroke-width when zooming, we force it to a fixed value.
    var px_per_mm = root.node.getCTM().a;

    // Drag events report deltas in pixels, which we'd like to convert to
    // millimeters.
    root.data("px_per_mm", px_per_mm);

    root.selectAll("path")
        .forEach(function (element, i) {
        sw = element.asPX("stroke-width") * px_per_mm;
        if (sw > 0) {
            element.attribute("stroke-width", sw);
            element.attribute("vector-effect", "non-scaling-stroke");
        }
    });

    // Store ticks labels original tranformation
    root.selectAll(".xlabels > text, .ylabels > text")
        .forEach(function (element, i) {
            var lm = element.transform().localMatrix;
            element.data("static_transform",
                new Snap.Matrix(lm.a, lm.b, lm.c, lm.d, lm.e, lm.f));
        });

    var xgridlines = root.select(".xgridlines");
    var ygridlines = root.select(".ygridlines");
    var xlabels = root.select(".xlabels");
    var ylabels = root.select(".ylabels");

    if (root.data("tx") === undefined) root.data("tx", 0);
    if (root.data("ty") === undefined) root.data("ty", 0);
    if (root.data("scale") === undefined) root.data("scale", 1.0);
    if (root.data("xtickscales") === undefined) {

        // index all the tick scales that are listed
        var xtickscales = {};
        var ytickscales = {};
        var add_x_tick_scales = function (element, i) {
            xtickscales[element.attribute("gadfly:scale")] = true;
        };
        var add_y_tick_scales = function (element, i) {
            ytickscales[element.attribute("gadfly:scale")] = true;
        };

        if (xgridlines) xgridlines.selectAll("path").forEach(add_x_tick_scales);
        if (ygridlines) ygridlines.selectAll("path").forEach(add_y_tick_scales);
        if (xlabels) xlabels.selectAll("text").forEach(add_x_tick_scales);
        if (ylabels) ylabels.selectAll("text").forEach(add_y_tick_scales);

        root.data("xtickscales", xtickscales);
        root.data("ytickscales", ytickscales);
        root.data("xtickscale", 1.0);
    }

    var min_scale = 1.0, max_scale = 1.0;
    for (scale in xtickscales) {
        min_scale = Math.min(min_scale, scale);
        max_scale = Math.max(max_scale, scale);
    }
    for (scale in ytickscales) {
        min_scale = Math.min(min_scale, scale);
        max_scale = Math.max(max_scale, scale);
    }
    root.data("min_scale", min_scale);
    root.data("max_scale", max_scale);

    // store the original positions of labels
    if (xlabels) {
        xlabels.selectAll("text")
               .forEach(function (element, i) {
                   element.data("x", element.asPX("x"));
               });
    }

    if (ylabels) {
        ylabels.selectAll("text")
               .forEach(function (element, i) {
                   element.data("y", element.asPX("y"));
               });
    }

    // mark grid lines and ticks as in or out of scale.
    var mark_inscale = function (element, i) {
        element.attribute("gadfly:inscale", element.attribute("gadfly:scale") == 1.0);
    };

    if (xgridlines) xgridlines.selectAll("path").forEach(mark_inscale);
    if (ygridlines) ygridlines.selectAll("path").forEach(mark_inscale);
    if (xlabels) xlabels.selectAll("text").forEach(mark_inscale);
    if (ylabels) ylabels.selectAll("text").forEach(mark_inscale);

    // figure out the upper ond lower bounds on panning using the maximum
    // and minum grid lines
    var bounds = root.plotbounds();
    var pan_bounds = {
        x0: 0.0,
        y0: 0.0,
        x1: 0.0,
        y1: 0.0
    };

    if (xgridlines) {
        xgridlines
            .selectAll("path")
            .forEach(function (element, i) {
                if (element.attribute("gadfly:inscale") == "true") {
                    var bbox = element.node.getBBox();
                    if (bounds.x1 - bbox.x < pan_bounds.x0) {
                        pan_bounds.x0 = bounds.x1 - bbox.x;
                    }
                    if (bounds.x0 - bbox.x > pan_bounds.x1) {
                        pan_bounds.x1 = bounds.x0 - bbox.x;
                    }
                    element.attr("visibility", "visible");
                }
            });
    }

    if (ygridlines) {
        ygridlines
            .selectAll("path")
            .forEach(function (element, i) {
                if (element.attribute("gadfly:inscale") == "true") {
                    var bbox = element.node.getBBox();
                    if (bounds.y1 - bbox.y < pan_bounds.y0) {
                        pan_bounds.y0 = bounds.y1 - bbox.y;
                    }
                    if (bounds.y0 - bbox.y > pan_bounds.y1) {
                        pan_bounds.y1 = bounds.y0 - bbox.y;
                    }
                    element.attr("visibility", "visible");
                }
            });
    }

    // nudge these values a little
    pan_bounds.x0 -= 5;
    pan_bounds.x1 += 5;
    pan_bounds.y0 -= 5;
    pan_bounds.y1 += 5;
    root.data("pan_bounds", pan_bounds);

    root.data("zoompan-ready", true)
};


// drag actions, i.e. zooming and panning
var pan_action = {
    start: function(root, x, y, event) {
        root.data("dx", 0);
        root.data("dy", 0);
        root.data("tx0", root.data("tx"));
        root.data("ty0", root.data("ty"));
    },
    update: function(root, dx, dy, x, y, event) {
        var px_per_mm = root.data("px_per_mm");
        dx /= px_per_mm;
        dy /= px_per_mm;

        var tx0 = root.data("tx"),
            ty0 = root.data("ty");

        var dx0 = root.data("dx"),
            dy0 = root.data("dy");

        root.data("dx", dx);
        root.data("dy", dy);

        dx = dx - dx0;
        dy = dy - dy0;

        var tx = tx0 + dx,
            ty = ty0 + dy;

        set_plot_pan_zoom(root, tx, ty, root.data("scale"));
    },
    end: function(root, event) {

    },
    cancel: function(root) {
        set_plot_pan_zoom(root, root.data("tx0"), root.data("ty0"), root.data("scale"));
    }
};

var zoom_box;
var zoom_action = {
    start: function(root, x, y, event) {
        var bounds = root.plotbounds();
        var width = bounds.x1 - bounds.x0,
            height = bounds.y1 - bounds.y0;
        var ratio = width / height;
        var xscalable = root.hasClass("xscalable"),
            yscalable = root.hasClass("yscalable");
        var px_per_mm = root.data("px_per_mm");
        x = xscalable ? x / px_per_mm : bounds.x0;
        y = yscalable ? y / px_per_mm : bounds.y0;
        var w = xscalable ? 0 : width;
        var h = yscalable ? 0 : height;
        zoom_box = root.rect(x, y, w, h).attr({
            "fill": "#000",
            "opacity": 0.25
        });
        zoom_box.data("ratio", ratio);
    },
    update: function(root, dx, dy, x, y, event) {
        var xscalable = root.hasClass("xscalable"),
            yscalable = root.hasClass("yscalable");
        var px_per_mm = root.data("px_per_mm");
        var bounds = root.plotbounds();
        if (yscalable) {
            y /= px_per_mm;
            y = Math.max(bounds.y0, y);
            y = Math.min(bounds.y1, y);
        } else {
            y = bounds.y1;
        }
        if (xscalable) {
            x /= px_per_mm;
            x = Math.max(bounds.x0, x);
            x = Math.min(bounds.x1, x);
        } else {
            x = bounds.x1;
        }

        dx = x - zoom_box.attr("x");
        dy = y - zoom_box.attr("y");
        if (xscalable && yscalable) {
            var ratio = zoom_box.data("ratio");
            var width = Math.min(Math.abs(dx), ratio * Math.abs(dy));
            var height = Math.min(Math.abs(dy), Math.abs(dx) / ratio);
            dx = width * dx / Math.abs(dx);
            dy = height * dy / Math.abs(dy);
        }
        var xoffset = 0,
            yoffset = 0;
        if (dx < 0) {
            xoffset = dx;
            dx = -1 * dx;
        }
        if (dy < 0) {
            yoffset = dy;
            dy = -1 * dy;
        }
        if (isNaN(dy)) {
            dy = 0.0;
        }
        if (isNaN(dx)) {
            dx = 0.0;
        }
        zoom_box.transform("T" + xoffset + "," + yoffset);
        zoom_box.attr("width", dx);
        zoom_box.attr("height", dy);
    },
    end: function(root, event) {
        var xscalable = root.hasClass("xscalable"),
            yscalable = root.hasClass("yscalable");
        var zoom_bounds = zoom_box.getBBox();
        if (zoom_bounds.width * zoom_bounds.height <= 0) {
            return;
        }
        var plot_bounds = root.plotbounds();
        var zoom_factor = 1.0;
        if (yscalable) {
            zoom_factor = (plot_bounds.y1 - plot_bounds.y0) / zoom_bounds.height;
        } else {
            zoom_factor = (plot_bounds.x1 - plot_bounds.x0) / zoom_bounds.width;
        }
        var tx = (root.data("tx") - zoom_bounds.x) * zoom_factor + plot_bounds.x0,
            ty = (root.data("ty") - zoom_bounds.y) * zoom_factor + plot_bounds.y0;
        set_plot_pan_zoom(root, tx, ty, root.data("scale") * zoom_factor);
        zoom_box.remove();
    },
    cancel: function(root) {
        zoom_box.remove();
    }
};


Gadfly.guide_background_drag_onstart = function(x, y, event) {
    var root = this.plotroot();
    var scalable = root.hasClass("xscalable") || root.hasClass("yscalable");
    var zoomable = !event.altKey && !event.ctrlKey && event.shiftKey && scalable;
    var panable = !event.altKey && !event.ctrlKey && !event.shiftKey && scalable;
    var drag_action = zoomable ? zoom_action :
                      panable  ? pan_action :
                                 undefined;
    root.data("drag_action", drag_action);
    if (drag_action) {
        var cancel_drag_action = function(event) {
            if (event.which == 27) { // esc key
                drag_action.cancel(root);
                root.data("drag_action", undefined);
            }
        };
        window.addEventListener("keyup", cancel_drag_action);
        root.data("cancel_drag_action", cancel_drag_action);
        drag_action.start(root, x, y, event);
    }
};


Gadfly.guide_background_drag_onmove = function(dx, dy, x, y, event) {
    var root = this.plotroot();
    var drag_action = root.data("drag_action");
    if (drag_action) {
        drag_action.update(root, dx, dy, x, y, event);
    }
};


Gadfly.guide_background_drag_onend = function(event) {
    var root = this.plotroot();
    window.removeEventListener("keyup", root.data("cancel_drag_action"));
    root.data("cancel_drag_action", undefined);
    var drag_action = root.data("drag_action");
    if (drag_action) {
        drag_action.end(root, event);
    }
    root.data("drag_action", undefined);
};


Gadfly.guide_background_scroll = function(event) {
    if (event.shiftKey) {
        increase_zoom_by_position(this.plotroot(), 0.001 * event.wheelDelta);
        event.preventDefault();
    }
};


Gadfly.zoomslider_button_mouseover = function(event) {
    this.select(".button_logo")
         .animate({fill: this.data("mouseover_color")}, 100);
};


Gadfly.zoomslider_button_mouseout = function(event) {
     this.select(".button_logo")
         .animate({fill: this.data("mouseout_color")}, 100);
};


Gadfly.zoomslider_zoomout_click = function(event) {
    increase_zoom_by_position(this.plotroot(), -0.1, true);
};


Gadfly.zoomslider_zoomin_click = function(event) {
    increase_zoom_by_position(this.plotroot(), 0.1, true);
};


Gadfly.zoomslider_track_click = function(event) {
    // TODO
};


// Map slider position x to scale y using the function y = a*exp(b*x)+c.
// The constants a, b, and c are solved using the constraint that the function
// should go through the points (0; min_scale), (0.5; 1), and (1; max_scale).
var scale_from_slider_position = function(position, min_scale, max_scale) {
    var a = (1 - 2 * min_scale + min_scale * min_scale) / (min_scale + max_scale - 2),
        b = 2 * Math.log((max_scale - 1) / (1 - min_scale)),
        c = (min_scale * max_scale - 1) / (min_scale + max_scale - 2);
    return a * Math.exp(b * position) + c;
}

// inverse of scale_from_slider_position
var slider_position_from_scale = function(scale, min_scale, max_scale) {
    var a = (1 - 2 * min_scale + min_scale * min_scale) / (min_scale + max_scale - 2),
        b = 2 * Math.log((max_scale - 1) / (1 - min_scale)),
        c = (min_scale * max_scale - 1) / (min_scale + max_scale - 2);
    return 1 / b * Math.log((scale - c) / a);
}

var increase_zoom_by_position = function(root, delta_position, animate) {
    var scale = root.data("scale"),
        min_scale = root.data("min_scale"),
        max_scale = root.data("max_scale");
    var position = slider_position_from_scale(scale, min_scale, max_scale);
    position += delta_position;
    scale = scale_from_slider_position(position, min_scale, max_scale);
    set_zoom(root, scale, animate);
}

var set_zoom = function(root, scale, animate) {
    var min_scale = root.data("min_scale"),
        max_scale = root.data("max_scale"),
        old_scale = root.data("scale");
    var new_scale = Math.max(min_scale, Math.min(scale, max_scale));
    if (animate) {
        Snap.animate(
            old_scale,
            new_scale,
            function (new_scale) {
                update_plot_scale(root, new_scale);
            },
            200);
    } else {
        update_plot_scale(root, new_scale);
    }
}


var update_plot_scale = function(root, new_scale) {
    var trans = scale_centered_translation(root, new_scale);
    set_plot_pan_zoom(root, trans.x, trans.y, new_scale);

    root.selectAll(".zoomslider_thumb")
        .forEach(function (element, i) {
            var min_pos = element.data("min_pos"),
                max_pos = element.data("max_pos"),
                min_scale = root.data("min_scale"),
                max_scale = root.data("max_scale");
            var xmid = (min_pos + max_pos) / 2;
            var xpos = slider_position_from_scale(new_scale, min_scale, max_scale);
            element.transform(new Snap.Matrix().translate(
                Math.max(min_pos, Math.min(
                         max_pos, min_pos + (max_pos - min_pos) * xpos)) - xmid, 0));
    });
};


Gadfly.zoomslider_thumb_dragmove = function(dx, dy, x, y, event) {
    var root = this.plotroot();
    var min_pos = this.data("min_pos"),
        max_pos = this.data("max_pos"),
        min_scale = root.data("min_scale"),
        max_scale = root.data("max_scale"),
        old_scale = root.data("old_scale");

    var px_per_mm = root.data("px_per_mm");
    dx /= px_per_mm;
    dy /= px_per_mm;

    var xmid = (min_pos + max_pos) / 2;
    var xpos = slider_position_from_scale(old_scale, min_scale, max_scale) +
                   dx / (max_pos - min_pos);

    // compute the new scale
    var new_scale = scale_from_slider_position(xpos, min_scale, max_scale);
    new_scale = Math.min(max_scale, Math.max(min_scale, new_scale));

    update_plot_scale(root, new_scale);
    event.stopPropagation();
};


Gadfly.zoomslider_thumb_dragstart = function(x, y, event) {
    this.animate({fill: this.data("mouseover_color")}, 100);
    var root = this.plotroot();

    // keep track of what the scale was when we started dragging
    root.data("old_scale", root.data("scale"));
    event.stopPropagation();
};


Gadfly.zoomslider_thumb_dragend = function(event) {
    this.animate({fill: this.data("mouseout_color")}, 100);
    event.stopPropagation();
};


var toggle_color_class = function(root, color_class, ison) {
    var guides = root.selectAll(".guide." + color_class + ",.guide ." + color_class);
    var geoms = root.selectAll(".geometry." + color_class + ",.geometry ." + color_class);
    if (ison) {
        guides.animate({opacity: 0.5}, 250);
        geoms.animate({opacity: 0.0}, 250);
    } else {
        guides.animate({opacity: 1.0}, 250);
        geoms.animate({opacity: 1.0}, 250);
    }
};


Gadfly.colorkey_swatch_click = function(event) {
    var root = this.plotroot();
    var color_class = this.data("color_class");

    if (event.shiftKey) {
        root.selectAll(".colorkey text")
            .forEach(function (element) {
                var other_color_class = element.data("color_class");
                if (other_color_class != color_class) {
                    toggle_color_class(root, other_color_class,
                                       element.attr("opacity") == 1.0);
                }
            });
    } else {
        toggle_color_class(root, color_class, this.attr("opacity") == 1.0);
    }
};


return Gadfly;

}));


//@ sourceURL=gadfly.js

(function (glob, factory) {
    // AMD support
      if (typeof require === "function" && typeof define === "function" && define.amd) {
        require(["Snap.svg", "Gadfly"], function (Snap, Gadfly) {
            factory(Snap, Gadfly);
        });
      } else {
          factory(glob.Snap, glob.Gadfly);
      }
})(window, function (Snap, Gadfly) {
    var fig = Snap("#img-32380ac5");
fig.select("#img-32380ac5-4")
   .drag(function() {}, function() {}, function() {});
fig.select("#img-32380ac5-6")
   .data("color_class", "color_no")
.click(Gadfly.colorkey_swatch_click)
;
fig.select("#img-32380ac5-7")
   .data("color_class", "color_yes")
.click(Gadfly.colorkey_swatch_click)
;
fig.select("#img-32380ac5-9")
   .data("color_class", "color_no")
.click(Gadfly.colorkey_swatch_click)
;
fig.select("#img-32380ac5-10")
   .data("color_class", "color_yes")
.click(Gadfly.colorkey_swatch_click)
;
fig.select("#img-32380ac5-14")
   .init_gadfly();
fig.select("#img-32380ac5-17")
   .plotroot().data("unfocused_ygrid_color", "#D0D0E0")
;
fig.select("#img-32380ac5-17")
   .plotroot().data("focused_ygrid_color", "#A0A0A0")
;
fig.select("#img-32380ac5-171")
   .plotroot().data("unfocused_xgrid_color", "#D0D0E0")
;
fig.select("#img-32380ac5-171")
   .plotroot().data("focused_xgrid_color", "#A0A0A0")
;
fig.select("#img-32380ac5-333")
   .data("mouseover_color", "#CD5C5C")
;
fig.select("#img-32380ac5-333")
   .data("mouseout_color", "#6A6A6A")
;
fig.select("#img-32380ac5-333")
   .click(Gadfly.zoomslider_zoomin_click)
.mouseenter(Gadfly.zoomslider_button_mouseover)
.mouseleave(Gadfly.zoomslider_button_mouseout)
;
fig.select("#img-32380ac5-337")
   .data("max_pos", 111.87)
;
fig.select("#img-32380ac5-337")
   .data("min_pos", 94.87)
;
fig.select("#img-32380ac5-337")
   .click(Gadfly.zoomslider_track_click);
fig.select("#img-32380ac5-339")
   .data("max_pos", 111.87)
;
fig.select("#img-32380ac5-339")
   .data("min_pos", 94.87)
;
fig.select("#img-32380ac5-339")
   .data("mouseover_color", "#CD5C5C")
;
fig.select("#img-32380ac5-339")
   .data("mouseout_color", "#6A6A6A")
;
fig.select("#img-32380ac5-339")
   .drag(Gadfly.zoomslider_thumb_dragmove,
     Gadfly.zoomslider_thumb_dragstart,
     Gadfly.zoomslider_thumb_dragend)
;
fig.select("#img-32380ac5-341")
   .data("mouseover_color", "#CD5C5C")
;
fig.select("#img-32380ac5-341")
   .data("mouseout_color", "#6A6A6A")
;
fig.select("#img-32380ac5-341")
   .click(Gadfly.zoomslider_zoomout_click)
.mouseenter(Gadfly.zoomslider_button_mouseover)
.mouseleave(Gadfly.zoomslider_button_mouseout)
;
    });
]]> </script>
</svg>




A natural question to ask is what is the posterior probability that the number of accidents is less on a day when the speed limit is enforced compared to when it is. To answer this one could build a statistical model using a Dirichlet process.

# Dirichlet Process Model Specification

![Model Specification](../../assets/images/latex/dplatex.gif)

and c a normalizing constant so that the weights sum to one.

A direct scan gibbs sampler could then be implemented as follows


```julia
function randDP(loglikelihood,logmarginallikelihood,posterior,Y,a,Θin)
    Θ=deepcopy(Θin)
    for i = 1:length(Y)
        weights = map(x -> loglikelihood(Y[i],x),Θ)
        weights[i] = logmarginallikelihood(Y[i])
        weights = exp(weights-maximum(weights))
        weights[i] = weights[i]*a
        weights = weights/sum(weights)
        select = rand(Categorical(weights))
        Θ[i] = (select == i ) ? rand(posterior(Y[i])) : Θ[select]
    end
    Θ
end
```







For the swedish speed limit dataset, natural assumptions would be that the function g is a Poisson pmf and the base measure is Gamma(b_{1},b_{2})

# Gibbs Sampling


```julia
function partial(f,a...)
    (b...) -> f(a...,b...)
end

b1=2
b2=0.1
a=1
PoissonGammaDP = partial(randDP,
                        (y,θ)->logpdf(Poisson(θ),y),
                        y->logpdf(NegativeBinomial(b1,b2/(b2+1)),y),
                        y->Gamma(b1+y,b2+1))

mcmcyes = iterate(partial(PoissonGammaDP,
                        limityes,a),
                    ones(length(limityes)))

mcmcno = iterate(partial(PoissonGammaDP,
                        limitno,a),
                    ones(length(limitno)))
```








```julia
drawsyes=collect(takenth(take(drop(mcmcyes,1000),10000),10))
drawsno=collect(takenth(take(drop(mcmcno,1000),10000),10))
distsyes=map( x -> MixtureModel([MixtureModel(map(y -> Poisson(y),x)),NegativeBinomial(b1,b2/(b2+1))],[nyes/(a+nyes), a/(a+nyes)]), drawsyes)
distsno=map( x -> MixtureModel([MixtureModel(map(y -> Poisson(y),x)),NegativeBinomial(b1,b2/(b2+1))],[nno/(a+nno), a/(a+nno)]), drawsno)
mean(x -> x[1]<x[2],zip(map(x->rand(x),distsyes),map(x->rand(x),distsno)))
```




    0.603



The iterate function from the Iterators.jl package forms a lazy evaluated list of MCMC iterates. In a functional style the first 1000 iterates can be dropped as a burnin. One can then "take" 10000 iterates from the chain (allbeit lazily) and therefrom take every 10'th iterate using the takenth function. The collect function then forces the evaluation of the lazily evaluated list. Each iterate consists of an $\theta$ array which can be used to form a posterior predictive distribution. Hence one can map a function that takes a $\theta$ array and returns a posterior predictive distribution over the mcmc draws to get an array of posterior predictive distributions, which is achieved in distsyes and distsno respectively. One can then generate random variates from each element of these arrays in turn, zip them together, and supply a predicate which evaluates true whenever the second element in a pair is greater than the first. Computing the mean of this boolean array then gives a monte carlo estimate of the probability that the number of accidents on a day when there is no speed limit is greater than the number of accidents on a day when there is no speed limit as 0.603.

# Visualization
Here's a function to plot the mean posterior predictive pmf's with 95% credible intervals.


```julia
function mcmcdataframe(draws,gitter,name)
  dists=map( x -> MixtureModel([MixtureModel(map(y -> Poisson(y),x)),NegativeBinomial(b1,b2/(b2+1))],[nyes/(a+nyes), a/(a+nyes)]), draws)
  gridmatrix=hcat(map(z -> pdf(z,gitter),dists)...)'
  meanfunction=map(grid -> mean(gridmatrix[:,grid]),gitter)
  upperfunction=map(grid -> quantile(gridmatrix[:,grid],0.975),gitter)
  lowerfunction=map(grid -> quantile(gridmatrix[:,grid],0.025),gitter)
  df_DP = DataFrame(
    x=gitter,
    y=meanfunction,
    ymin=lowerfunction,
    ymax=upperfunction,
    Speed_Limit=name
  )
end

df_mcmc=vcat(mcmcdataframe(drawsno,1:60,"No"),mcmcdataframe(drawsyes,1:60,"Yes"))

plot(df_mcmc, x=:x, y=:y, ymin=:ymin, ymax=:ymax, color=:Speed_Limit, Geom.line, Geom.ribbon)
```




<?xml version="1.0" encoding="UTF-8"?>
<svg xmlns="http://www.w3.org/2000/svg"
     xmlns:xlink="http://www.w3.org/1999/xlink"
     xmlns:gadfly="http://www.gadflyjl.org/ns"
     version="1.2"
     width="141.42mm" height="100mm" viewBox="0 0 141.42 100"
     stroke="none"
     fill="#000000"
     stroke-width="0.3"
     font-size="3.88"

     id="img-a6536837">
<g class="plotroot xscalable yscalable" id="img-a6536837-1">
  <g font-size="3.88" font-family="'PT Sans','Helvetica Neue','Helvetica',sans-serif" fill="#564A55" stroke="#000000" stroke-opacity="0.000" id="img-a6536837-2">
    <text x="67.29" y="88.39" text-anchor="middle" dy="0.6em">x</text>
  </g>
  <g class="guide xlabels" font-size="2.82" font-family="'PT Sans Caption','Helvetica Neue','Helvetica',sans-serif" fill="#6C606B" id="img-a6536837-3">
    <text x="-88.69" y="84.39" text-anchor="middle" visibility="hidden" gadfly:scale="1.0">-70</text>
    <text x="-73.09" y="84.39" text-anchor="middle" visibility="hidden" gadfly:scale="1.0">-60</text>
    <text x="-57.49" y="84.39" text-anchor="middle" visibility="hidden" gadfly:scale="1.0">-50</text>
    <text x="-41.89" y="84.39" text-anchor="middle" visibility="hidden" gadfly:scale="1.0">-40</text>
    <text x="-26.3" y="84.39" text-anchor="middle" visibility="hidden" gadfly:scale="1.0">-30</text>
    <text x="-10.7" y="84.39" text-anchor="middle" visibility="hidden" gadfly:scale="1.0">-20</text>
    <text x="4.9" y="84.39" text-anchor="middle" visibility="hidden" gadfly:scale="1.0">-10</text>
    <text x="20.5" y="84.39" text-anchor="middle" visibility="visible" gadfly:scale="1.0">0</text>
    <text x="36.1" y="84.39" text-anchor="middle" visibility="visible" gadfly:scale="1.0">10</text>
    <text x="51.7" y="84.39" text-anchor="middle" visibility="visible" gadfly:scale="1.0">20</text>
    <text x="67.29" y="84.39" text-anchor="middle" visibility="visible" gadfly:scale="1.0">30</text>
    <text x="82.89" y="84.39" text-anchor="middle" visibility="visible" gadfly:scale="1.0">40</text>
    <text x="98.49" y="84.39" text-anchor="middle" visibility="visible" gadfly:scale="1.0">50</text>
    <text x="114.09" y="84.39" text-anchor="middle" visibility="visible" gadfly:scale="1.0">60</text>
    <text x="129.69" y="84.39" text-anchor="middle" visibility="hidden" gadfly:scale="1.0">70</text>
    <text x="145.29" y="84.39" text-anchor="middle" visibility="hidden" gadfly:scale="1.0">80</text>
    <text x="160.88" y="84.39" text-anchor="middle" visibility="hidden" gadfly:scale="1.0">90</text>
    <text x="176.48" y="84.39" text-anchor="middle" visibility="hidden" gadfly:scale="1.0">100</text>
    <text x="192.08" y="84.39" text-anchor="middle" visibility="hidden" gadfly:scale="1.0">110</text>
    <text x="207.68" y="84.39" text-anchor="middle" visibility="hidden" gadfly:scale="1.0">120</text>
    <text x="223.28" y="84.39" text-anchor="middle" visibility="hidden" gadfly:scale="1.0">130</text>
    <text x="-73.09" y="84.39" text-anchor="middle" visibility="hidden" gadfly:scale="10.0">-60</text>
    <text x="-69.97" y="84.39" text-anchor="middle" visibility="hidden" gadfly:scale="10.0">-58</text>
    <text x="-66.85" y="84.39" text-anchor="middle" visibility="hidden" gadfly:scale="10.0">-56</text>
    <text x="-63.73" y="84.39" text-anchor="middle" visibility="hidden" gadfly:scale="10.0">-54</text>
    <text x="-60.61" y="84.39" text-anchor="middle" visibility="hidden" gadfly:scale="10.0">-52</text>
    <text x="-57.49" y="84.39" text-anchor="middle" visibility="hidden" gadfly:scale="10.0">-50</text>
    <text x="-54.37" y="84.39" text-anchor="middle" visibility="hidden" gadfly:scale="10.0">-48</text>
    <text x="-51.25" y="84.39" text-anchor="middle" visibility="hidden" gadfly:scale="10.0">-46</text>
    <text x="-48.13" y="84.39" text-anchor="middle" visibility="hidden" gadfly:scale="10.0">-44</text>
    <text x="-45.01" y="84.39" text-anchor="middle" visibility="hidden" gadfly:scale="10.0">-42</text>
    <text x="-41.89" y="84.39" text-anchor="middle" visibility="hidden" gadfly:scale="10.0">-40</text>
    <text x="-38.77" y="84.39" text-anchor="middle" visibility="hidden" gadfly:scale="10.0">-38</text>
    <text x="-35.66" y="84.39" text-anchor="middle" visibility="hidden" gadfly:scale="10.0">-36</text>
    <text x="-32.54" y="84.39" text-anchor="middle" visibility="hidden" gadfly:scale="10.0">-34</text>
    <text x="-29.42" y="84.39" text-anchor="middle" visibility="hidden" gadfly:scale="10.0">-32</text>
    <text x="-26.3" y="84.39" text-anchor="middle" visibility="hidden" gadfly:scale="10.0">-30</text>
    <text x="-23.18" y="84.39" text-anchor="middle" visibility="hidden" gadfly:scale="10.0">-28</text>
    <text x="-20.06" y="84.39" text-anchor="middle" visibility="hidden" gadfly:scale="10.0">-26</text>
    <text x="-16.94" y="84.39" text-anchor="middle" visibility="hidden" gadfly:scale="10.0">-24</text>
    <text x="-13.82" y="84.39" text-anchor="middle" visibility="hidden" gadfly:scale="10.0">-22</text>
    <text x="-10.7" y="84.39" text-anchor="middle" visibility="hidden" gadfly:scale="10.0">-20</text>
    <text x="-7.58" y="84.39" text-anchor="middle" visibility="hidden" gadfly:scale="10.0">-18</text>
    <text x="-4.46" y="84.39" text-anchor="middle" visibility="hidden" gadfly:scale="10.0">-16</text>
    <text x="-1.34" y="84.39" text-anchor="middle" visibility="hidden" gadfly:scale="10.0">-14</text>
    <text x="1.78" y="84.39" text-anchor="middle" visibility="hidden" gadfly:scale="10.0">-12</text>
    <text x="4.9" y="84.39" text-anchor="middle" visibility="hidden" gadfly:scale="10.0">-10</text>
    <text x="8.02" y="84.39" text-anchor="middle" visibility="hidden" gadfly:scale="10.0">-8</text>
    <text x="11.14" y="84.39" text-anchor="middle" visibility="hidden" gadfly:scale="10.0">-6</text>
    <text x="14.26" y="84.39" text-anchor="middle" visibility="hidden" gadfly:scale="10.0">-4</text>
    <text x="17.38" y="84.39" text-anchor="middle" visibility="hidden" gadfly:scale="10.0">-2</text>
    <text x="20.5" y="84.39" text-anchor="middle" visibility="hidden" gadfly:scale="10.0">0</text>
    <text x="23.62" y="84.39" text-anchor="middle" visibility="hidden" gadfly:scale="10.0">2</text>
    <text x="26.74" y="84.39" text-anchor="middle" visibility="hidden" gadfly:scale="10.0">4</text>
    <text x="29.86" y="84.39" text-anchor="middle" visibility="hidden" gadfly:scale="10.0">6</text>
    <text x="32.98" y="84.39" text-anchor="middle" visibility="hidden" gadfly:scale="10.0">8</text>
    <text x="36.1" y="84.39" text-anchor="middle" visibility="hidden" gadfly:scale="10.0">10</text>
    <text x="39.22" y="84.39" text-anchor="middle" visibility="hidden" gadfly:scale="10.0">12</text>
    <text x="42.34" y="84.39" text-anchor="middle" visibility="hidden" gadfly:scale="10.0">14</text>
    <text x="45.46" y="84.39" text-anchor="middle" visibility="hidden" gadfly:scale="10.0">16</text>
    <text x="48.58" y="84.39" text-anchor="middle" visibility="hidden" gadfly:scale="10.0">18</text>
    <text x="51.7" y="84.39" text-anchor="middle" visibility="hidden" gadfly:scale="10.0">20</text>
    <text x="54.82" y="84.39" text-anchor="middle" visibility="hidden" gadfly:scale="10.0">22</text>
    <text x="57.94" y="84.39" text-anchor="middle" visibility="hidden" gadfly:scale="10.0">24</text>
    <text x="61.06" y="84.39" text-anchor="middle" visibility="hidden" gadfly:scale="10.0">26</text>
    <text x="64.17" y="84.39" text-anchor="middle" visibility="hidden" gadfly:scale="10.0">28</text>
    <text x="67.29" y="84.39" text-anchor="middle" visibility="hidden" gadfly:scale="10.0">30</text>
    <text x="70.41" y="84.39" text-anchor="middle" visibility="hidden" gadfly:scale="10.0">32</text>
    <text x="73.53" y="84.39" text-anchor="middle" visibility="hidden" gadfly:scale="10.0">34</text>
    <text x="76.65" y="84.39" text-anchor="middle" visibility="hidden" gadfly:scale="10.0">36</text>
    <text x="79.77" y="84.39" text-anchor="middle" visibility="hidden" gadfly:scale="10.0">38</text>
    <text x="82.89" y="84.39" text-anchor="middle" visibility="hidden" gadfly:scale="10.0">40</text>
    <text x="86.01" y="84.39" text-anchor="middle" visibility="hidden" gadfly:scale="10.0">42</text>
    <text x="89.13" y="84.39" text-anchor="middle" visibility="hidden" gadfly:scale="10.0">44</text>
    <text x="92.25" y="84.39" text-anchor="middle" visibility="hidden" gadfly:scale="10.0">46</text>
    <text x="95.37" y="84.39" text-anchor="middle" visibility="hidden" gadfly:scale="10.0">48</text>
    <text x="98.49" y="84.39" text-anchor="middle" visibility="hidden" gadfly:scale="10.0">50</text>
    <text x="101.61" y="84.39" text-anchor="middle" visibility="hidden" gadfly:scale="10.0">52</text>
    <text x="104.73" y="84.39" text-anchor="middle" visibility="hidden" gadfly:scale="10.0">54</text>
    <text x="107.85" y="84.39" text-anchor="middle" visibility="hidden" gadfly:scale="10.0">56</text>
    <text x="110.97" y="84.39" text-anchor="middle" visibility="hidden" gadfly:scale="10.0">58</text>
    <text x="114.09" y="84.39" text-anchor="middle" visibility="hidden" gadfly:scale="10.0">60</text>
    <text x="117.21" y="84.39" text-anchor="middle" visibility="hidden" gadfly:scale="10.0">62</text>
    <text x="120.33" y="84.39" text-anchor="middle" visibility="hidden" gadfly:scale="10.0">64</text>
    <text x="123.45" y="84.39" text-anchor="middle" visibility="hidden" gadfly:scale="10.0">66</text>
    <text x="126.57" y="84.39" text-anchor="middle" visibility="hidden" gadfly:scale="10.0">68</text>
    <text x="129.69" y="84.39" text-anchor="middle" visibility="hidden" gadfly:scale="10.0">70</text>
    <text x="132.81" y="84.39" text-anchor="middle" visibility="hidden" gadfly:scale="10.0">72</text>
    <text x="135.93" y="84.39" text-anchor="middle" visibility="hidden" gadfly:scale="10.0">74</text>
    <text x="139.05" y="84.39" text-anchor="middle" visibility="hidden" gadfly:scale="10.0">76</text>
    <text x="142.17" y="84.39" text-anchor="middle" visibility="hidden" gadfly:scale="10.0">78</text>
    <text x="145.29" y="84.39" text-anchor="middle" visibility="hidden" gadfly:scale="10.0">80</text>
    <text x="148.41" y="84.39" text-anchor="middle" visibility="hidden" gadfly:scale="10.0">82</text>
    <text x="151.53" y="84.39" text-anchor="middle" visibility="hidden" gadfly:scale="10.0">84</text>
    <text x="154.65" y="84.39" text-anchor="middle" visibility="hidden" gadfly:scale="10.0">86</text>
    <text x="157.77" y="84.39" text-anchor="middle" visibility="hidden" gadfly:scale="10.0">88</text>
    <text x="160.88" y="84.39" text-anchor="middle" visibility="hidden" gadfly:scale="10.0">90</text>
    <text x="164" y="84.39" text-anchor="middle" visibility="hidden" gadfly:scale="10.0">92</text>
    <text x="167.12" y="84.39" text-anchor="middle" visibility="hidden" gadfly:scale="10.0">94</text>
    <text x="170.24" y="84.39" text-anchor="middle" visibility="hidden" gadfly:scale="10.0">96</text>
    <text x="173.36" y="84.39" text-anchor="middle" visibility="hidden" gadfly:scale="10.0">98</text>
    <text x="176.48" y="84.39" text-anchor="middle" visibility="hidden" gadfly:scale="10.0">100</text>
    <text x="179.6" y="84.39" text-anchor="middle" visibility="hidden" gadfly:scale="10.0">102</text>
    <text x="182.72" y="84.39" text-anchor="middle" visibility="hidden" gadfly:scale="10.0">104</text>
    <text x="185.84" y="84.39" text-anchor="middle" visibility="hidden" gadfly:scale="10.0">106</text>
    <text x="188.96" y="84.39" text-anchor="middle" visibility="hidden" gadfly:scale="10.0">108</text>
    <text x="192.08" y="84.39" text-anchor="middle" visibility="hidden" gadfly:scale="10.0">110</text>
    <text x="195.2" y="84.39" text-anchor="middle" visibility="hidden" gadfly:scale="10.0">112</text>
    <text x="198.32" y="84.39" text-anchor="middle" visibility="hidden" gadfly:scale="10.0">114</text>
    <text x="201.44" y="84.39" text-anchor="middle" visibility="hidden" gadfly:scale="10.0">116</text>
    <text x="204.56" y="84.39" text-anchor="middle" visibility="hidden" gadfly:scale="10.0">118</text>
    <text x="207.68" y="84.39" text-anchor="middle" visibility="hidden" gadfly:scale="10.0">120</text>
    <text x="-135.49" y="84.39" text-anchor="middle" visibility="hidden" gadfly:scale="0.5">-100</text>
    <text x="20.5" y="84.39" text-anchor="middle" visibility="hidden" gadfly:scale="0.5">0</text>
    <text x="176.48" y="84.39" text-anchor="middle" visibility="hidden" gadfly:scale="0.5">100</text>
    <text x="332.47" y="84.39" text-anchor="middle" visibility="hidden" gadfly:scale="0.5">200</text>
    <text x="-73.09" y="84.39" text-anchor="middle" visibility="hidden" gadfly:scale="5.0">-60</text>
    <text x="-65.29" y="84.39" text-anchor="middle" visibility="hidden" gadfly:scale="5.0">-55</text>
    <text x="-57.49" y="84.39" text-anchor="middle" visibility="hidden" gadfly:scale="5.0">-50</text>
    <text x="-49.69" y="84.39" text-anchor="middle" visibility="hidden" gadfly:scale="5.0">-45</text>
    <text x="-41.89" y="84.39" text-anchor="middle" visibility="hidden" gadfly:scale="5.0">-40</text>
    <text x="-34.1" y="84.39" text-anchor="middle" visibility="hidden" gadfly:scale="5.0">-35</text>
    <text x="-26.3" y="84.39" text-anchor="middle" visibility="hidden" gadfly:scale="5.0">-30</text>
    <text x="-18.5" y="84.39" text-anchor="middle" visibility="hidden" gadfly:scale="5.0">-25</text>
    <text x="-10.7" y="84.39" text-anchor="middle" visibility="hidden" gadfly:scale="5.0">-20</text>
    <text x="-2.9" y="84.39" text-anchor="middle" visibility="hidden" gadfly:scale="5.0">-15</text>
    <text x="4.9" y="84.39" text-anchor="middle" visibility="hidden" gadfly:scale="5.0">-10</text>
    <text x="12.7" y="84.39" text-anchor="middle" visibility="hidden" gadfly:scale="5.0">-5</text>
    <text x="20.5" y="84.39" text-anchor="middle" visibility="hidden" gadfly:scale="5.0">0</text>
    <text x="28.3" y="84.39" text-anchor="middle" visibility="hidden" gadfly:scale="5.0">5</text>
    <text x="36.1" y="84.39" text-anchor="middle" visibility="hidden" gadfly:scale="5.0">10</text>
    <text x="43.9" y="84.39" text-anchor="middle" visibility="hidden" gadfly:scale="5.0">15</text>
    <text x="51.7" y="84.39" text-anchor="middle" visibility="hidden" gadfly:scale="5.0">20</text>
    <text x="59.5" y="84.39" text-anchor="middle" visibility="hidden" gadfly:scale="5.0">25</text>
    <text x="67.29" y="84.39" text-anchor="middle" visibility="hidden" gadfly:scale="5.0">30</text>
    <text x="75.09" y="84.39" text-anchor="middle" visibility="hidden" gadfly:scale="5.0">35</text>
    <text x="82.89" y="84.39" text-anchor="middle" visibility="hidden" gadfly:scale="5.0">40</text>
    <text x="90.69" y="84.39" text-anchor="middle" visibility="hidden" gadfly:scale="5.0">45</text>
    <text x="98.49" y="84.39" text-anchor="middle" visibility="hidden" gadfly:scale="5.0">50</text>
    <text x="106.29" y="84.39" text-anchor="middle" visibility="hidden" gadfly:scale="5.0">55</text>
    <text x="114.09" y="84.39" text-anchor="middle" visibility="hidden" gadfly:scale="5.0">60</text>
    <text x="121.89" y="84.39" text-anchor="middle" visibility="hidden" gadfly:scale="5.0">65</text>
    <text x="129.69" y="84.39" text-anchor="middle" visibility="hidden" gadfly:scale="5.0">70</text>
    <text x="137.49" y="84.39" text-anchor="middle" visibility="hidden" gadfly:scale="5.0">75</text>
    <text x="145.29" y="84.39" text-anchor="middle" visibility="hidden" gadfly:scale="5.0">80</text>
    <text x="153.09" y="84.39" text-anchor="middle" visibility="hidden" gadfly:scale="5.0">85</text>
    <text x="160.88" y="84.39" text-anchor="middle" visibility="hidden" gadfly:scale="5.0">90</text>
    <text x="168.68" y="84.39" text-anchor="middle" visibility="hidden" gadfly:scale="5.0">95</text>
    <text x="176.48" y="84.39" text-anchor="middle" visibility="hidden" gadfly:scale="5.0">100</text>
    <text x="184.28" y="84.39" text-anchor="middle" visibility="hidden" gadfly:scale="5.0">105</text>
    <text x="192.08" y="84.39" text-anchor="middle" visibility="hidden" gadfly:scale="5.0">110</text>
    <text x="199.88" y="84.39" text-anchor="middle" visibility="hidden" gadfly:scale="5.0">115</text>
    <text x="207.68" y="84.39" text-anchor="middle" visibility="hidden" gadfly:scale="5.0">120</text>
  </g>
  <g class="guide colorkey" id="img-a6536837-4">
    <g fill="#4C404B" font-size="2.82" font-family="'PT Sans','Helvetica Neue','Helvetica',sans-serif" id="img-a6536837-5">
      <text x="119.9" y="42.86" dy="0.35em" id="img-a6536837-6" class="color_No">No</text>
      <text x="119.9" y="46.48" dy="0.35em" id="img-a6536837-7" class="color_Yes">Yes</text>
    </g>
    <g stroke="#000000" stroke-opacity="0.000" id="img-a6536837-8">
      <rect x="117.09" y="41.95" width="1.81" height="1.81" id="img-a6536837-9" fill="#00BFFF" class="color_No"/>
      <rect x="117.09" y="45.58" width="1.81" height="1.81" id="img-a6536837-10" fill="#D4CA3A" class="color_Yes"/>
    </g>
    <g fill="#362A35" font-size="3.88" font-family="'PT Sans','Helvetica Neue','Helvetica',sans-serif" stroke="#000000" stroke-opacity="0.000" id="img-a6536837-11">
      <text x="117.09" y="39.04" id="img-a6536837-12">Speed_Limit</text>
    </g>
  </g>
<g clip-path="url(#img-a6536837-13)">
  <g id="img-a6536837-14">
    <g pointer-events="visible" opacity="1" fill="#000000" fill-opacity="0.000" stroke="#000000" stroke-opacity="0.000" class="guide background" id="img-a6536837-15">
      <rect x="18.5" y="5" width="97.59" height="75.72" id="img-a6536837-16"/>
    </g>
    <g class="guide ygridlines xfixed" stroke-dasharray="0.5,0.5" stroke-width="0.2" stroke="#D0D0E0" id="img-a6536837-17">
      <path fill="none" d="M18.5,168.36 L 116.09 168.36" id="img-a6536837-18" visibility="hidden" gadfly:scale="1.0"/>
      <path fill="none" d="M18.5,150.43 L 116.09 150.43" id="img-a6536837-19" visibility="hidden" gadfly:scale="1.0"/>
      <path fill="none" d="M18.5,132.5 L 116.09 132.5" id="img-a6536837-20" visibility="hidden" gadfly:scale="1.0"/>
      <path fill="none" d="M18.5,114.57 L 116.09 114.57" id="img-a6536837-21" visibility="hidden" gadfly:scale="1.0"/>
      <path fill="none" d="M18.5,96.64 L 116.09 96.64" id="img-a6536837-22" visibility="hidden" gadfly:scale="1.0"/>
      <path fill="none" d="M18.5,78.72 L 116.09 78.72" id="img-a6536837-23" visibility="visible" gadfly:scale="1.0"/>
      <path fill="none" d="M18.5,60.79 L 116.09 60.79" id="img-a6536837-24" visibility="visible" gadfly:scale="1.0"/>
      <path fill="none" d="M18.5,42.86 L 116.09 42.86" id="img-a6536837-25" visibility="visible" gadfly:scale="1.0"/>
      <path fill="none" d="M18.5,24.93 L 116.09 24.93" id="img-a6536837-26" visibility="visible" gadfly:scale="1.0"/>
      <path fill="none" d="M18.5,7 L 116.09 7" id="img-a6536837-27" visibility="visible" gadfly:scale="1.0"/>
      <path fill="none" d="M18.5,-10.93 L 116.09 -10.93" id="img-a6536837-28" visibility="hidden" gadfly:scale="1.0"/>
      <path fill="none" d="M18.5,-28.86 L 116.09 -28.86" id="img-a6536837-29" visibility="hidden" gadfly:scale="1.0"/>
      <path fill="none" d="M18.5,-46.79 L 116.09 -46.79" id="img-a6536837-30" visibility="hidden" gadfly:scale="1.0"/>
      <path fill="none" d="M18.5,-64.72 L 116.09 -64.72" id="img-a6536837-31" visibility="hidden" gadfly:scale="1.0"/>
      <path fill="none" d="M18.5,-82.64 L 116.09 -82.64" id="img-a6536837-32" visibility="hidden" gadfly:scale="1.0"/>
      <path fill="none" d="M18.5,150.43 L 116.09 150.43" id="img-a6536837-33" visibility="hidden" gadfly:scale="10.0"/>
      <path fill="none" d="M18.5,145.95 L 116.09 145.95" id="img-a6536837-34" visibility="hidden" gadfly:scale="10.0"/>
      <path fill="none" d="M18.5,141.47 L 116.09 141.47" id="img-a6536837-35" visibility="hidden" gadfly:scale="10.0"/>
      <path fill="none" d="M18.5,136.98 L 116.09 136.98" id="img-a6536837-36" visibility="hidden" gadfly:scale="10.0"/>
      <path fill="none" d="M18.5,132.5 L 116.09 132.5" id="img-a6536837-37" visibility="hidden" gadfly:scale="10.0"/>
      <path fill="none" d="M18.5,128.02 L 116.09 128.02" id="img-a6536837-38" visibility="hidden" gadfly:scale="10.0"/>
      <path fill="none" d="M18.5,123.54 L 116.09 123.54" id="img-a6536837-39" visibility="hidden" gadfly:scale="10.0"/>
      <path fill="none" d="M18.5,119.05 L 116.09 119.05" id="img-a6536837-40" visibility="hidden" gadfly:scale="10.0"/>
      <path fill="none" d="M18.5,114.57 L 116.09 114.57" id="img-a6536837-41" visibility="hidden" gadfly:scale="10.0"/>
      <path fill="none" d="M18.5,110.09 L 116.09 110.09" id="img-a6536837-42" visibility="hidden" gadfly:scale="10.0"/>
      <path fill="none" d="M18.5,105.61 L 116.09 105.61" id="img-a6536837-43" visibility="hidden" gadfly:scale="10.0"/>
      <path fill="none" d="M18.5,101.13 L 116.09 101.13" id="img-a6536837-44" visibility="hidden" gadfly:scale="10.0"/>
      <path fill="none" d="M18.5,96.64 L 116.09 96.64" id="img-a6536837-45" visibility="hidden" gadfly:scale="10.0"/>
      <path fill="none" d="M18.5,92.16 L 116.09 92.16" id="img-a6536837-46" visibility="hidden" gadfly:scale="10.0"/>
      <path fill="none" d="M18.5,87.68 L 116.09 87.68" id="img-a6536837-47" visibility="hidden" gadfly:scale="10.0"/>
      <path fill="none" d="M18.5,83.2 L 116.09 83.2" id="img-a6536837-48" visibility="hidden" gadfly:scale="10.0"/>
      <path fill="none" d="M18.5,78.72 L 116.09 78.72" id="img-a6536837-49" visibility="hidden" gadfly:scale="10.0"/>
      <path fill="none" d="M18.5,74.23 L 116.09 74.23" id="img-a6536837-50" visibility="hidden" gadfly:scale="10.0"/>
      <path fill="none" d="M18.5,69.75 L 116.09 69.75" id="img-a6536837-51" visibility="hidden" gadfly:scale="10.0"/>
      <path fill="none" d="M18.5,65.27 L 116.09 65.27" id="img-a6536837-52" visibility="hidden" gadfly:scale="10.0"/>
      <path fill="none" d="M18.5,60.79 L 116.09 60.79" id="img-a6536837-53" visibility="hidden" gadfly:scale="10.0"/>
      <path fill="none" d="M18.5,56.3 L 116.09 56.3" id="img-a6536837-54" visibility="hidden" gadfly:scale="10.0"/>
      <path fill="none" d="M18.5,51.82 L 116.09 51.82" id="img-a6536837-55" visibility="hidden" gadfly:scale="10.0"/>
      <path fill="none" d="M18.5,47.34 L 116.09 47.34" id="img-a6536837-56" visibility="hidden" gadfly:scale="10.0"/>
      <path fill="none" d="M18.5,42.86 L 116.09 42.86" id="img-a6536837-57" visibility="hidden" gadfly:scale="10.0"/>
      <path fill="none" d="M18.5,38.38 L 116.09 38.38" id="img-a6536837-58" visibility="hidden" gadfly:scale="10.0"/>
      <path fill="none" d="M18.5,33.89 L 116.09 33.89" id="img-a6536837-59" visibility="hidden" gadfly:scale="10.0"/>
      <path fill="none" d="M18.5,29.41 L 116.09 29.41" id="img-a6536837-60" visibility="hidden" gadfly:scale="10.0"/>
      <path fill="none" d="M18.5,24.93 L 116.09 24.93" id="img-a6536837-61" visibility="hidden" gadfly:scale="10.0"/>
      <path fill="none" d="M18.5,20.45 L 116.09 20.45" id="img-a6536837-62" visibility="hidden" gadfly:scale="10.0"/>
      <path fill="none" d="M18.5,15.96 L 116.09 15.96" id="img-a6536837-63" visibility="hidden" gadfly:scale="10.0"/>
      <path fill="none" d="M18.5,11.48 L 116.09 11.48" id="img-a6536837-64" visibility="hidden" gadfly:scale="10.0"/>
      <path fill="none" d="M18.5,7 L 116.09 7" id="img-a6536837-65" visibility="hidden" gadfly:scale="10.0"/>
      <path fill="none" d="M18.5,2.52 L 116.09 2.52" id="img-a6536837-66" visibility="hidden" gadfly:scale="10.0"/>
      <path fill="none" d="M18.5,-1.96 L 116.09 -1.96" id="img-a6536837-67" visibility="hidden" gadfly:scale="10.0"/>
      <path fill="none" d="M18.5,-6.45 L 116.09 -6.45" id="img-a6536837-68" visibility="hidden" gadfly:scale="10.0"/>
      <path fill="none" d="M18.5,-10.93 L 116.09 -10.93" id="img-a6536837-69" visibility="hidden" gadfly:scale="10.0"/>
      <path fill="none" d="M18.5,-15.41 L 116.09 -15.41" id="img-a6536837-70" visibility="hidden" gadfly:scale="10.0"/>
      <path fill="none" d="M18.5,-19.89 L 116.09 -19.89" id="img-a6536837-71" visibility="hidden" gadfly:scale="10.0"/>
      <path fill="none" d="M18.5,-24.38 L 116.09 -24.38" id="img-a6536837-72" visibility="hidden" gadfly:scale="10.0"/>
      <path fill="none" d="M18.5,-28.86 L 116.09 -28.86" id="img-a6536837-73" visibility="hidden" gadfly:scale="10.0"/>
      <path fill="none" d="M18.5,-33.34 L 116.09 -33.34" id="img-a6536837-74" visibility="hidden" gadfly:scale="10.0"/>
      <path fill="none" d="M18.5,-37.82 L 116.09 -37.82" id="img-a6536837-75" visibility="hidden" gadfly:scale="10.0"/>
      <path fill="none" d="M18.5,-42.3 L 116.09 -42.3" id="img-a6536837-76" visibility="hidden" gadfly:scale="10.0"/>
      <path fill="none" d="M18.5,-46.79 L 116.09 -46.79" id="img-a6536837-77" visibility="hidden" gadfly:scale="10.0"/>
      <path fill="none" d="M18.5,-51.27 L 116.09 -51.27" id="img-a6536837-78" visibility="hidden" gadfly:scale="10.0"/>
      <path fill="none" d="M18.5,-55.75 L 116.09 -55.75" id="img-a6536837-79" visibility="hidden" gadfly:scale="10.0"/>
      <path fill="none" d="M18.5,-60.23 L 116.09 -60.23" id="img-a6536837-80" visibility="hidden" gadfly:scale="10.0"/>
      <path fill="none" d="M18.5,-64.72 L 116.09 -64.72" id="img-a6536837-81" visibility="hidden" gadfly:scale="10.0"/>
      <path fill="none" d="M18.5,-69.2 L 116.09 -69.2" id="img-a6536837-82" visibility="hidden" gadfly:scale="10.0"/>
      <path fill="none" d="M18.5,168.36 L 116.09 168.36" id="img-a6536837-83" visibility="hidden" gadfly:scale="0.5"/>
      <path fill="none" d="M18.5,78.72 L 116.09 78.72" id="img-a6536837-84" visibility="hidden" gadfly:scale="0.5"/>
      <path fill="none" d="M18.5,-10.93 L 116.09 -10.93" id="img-a6536837-85" visibility="hidden" gadfly:scale="0.5"/>
      <path fill="none" d="M18.5,-100.57 L 116.09 -100.57" id="img-a6536837-86" visibility="hidden" gadfly:scale="0.5"/>
      <path fill="none" d="M18.5,150.43 L 116.09 150.43" id="img-a6536837-87" visibility="hidden" gadfly:scale="5.0"/>
      <path fill="none" d="M18.5,145.95 L 116.09 145.95" id="img-a6536837-88" visibility="hidden" gadfly:scale="5.0"/>
      <path fill="none" d="M18.5,141.47 L 116.09 141.47" id="img-a6536837-89" visibility="hidden" gadfly:scale="5.0"/>
      <path fill="none" d="M18.5,136.98 L 116.09 136.98" id="img-a6536837-90" visibility="hidden" gadfly:scale="5.0"/>
      <path fill="none" d="M18.5,132.5 L 116.09 132.5" id="img-a6536837-91" visibility="hidden" gadfly:scale="5.0"/>
      <path fill="none" d="M18.5,128.02 L 116.09 128.02" id="img-a6536837-92" visibility="hidden" gadfly:scale="5.0"/>
      <path fill="none" d="M18.5,123.54 L 116.09 123.54" id="img-a6536837-93" visibility="hidden" gadfly:scale="5.0"/>
      <path fill="none" d="M18.5,119.05 L 116.09 119.05" id="img-a6536837-94" visibility="hidden" gadfly:scale="5.0"/>
      <path fill="none" d="M18.5,114.57 L 116.09 114.57" id="img-a6536837-95" visibility="hidden" gadfly:scale="5.0"/>
      <path fill="none" d="M18.5,110.09 L 116.09 110.09" id="img-a6536837-96" visibility="hidden" gadfly:scale="5.0"/>
      <path fill="none" d="M18.5,105.61 L 116.09 105.61" id="img-a6536837-97" visibility="hidden" gadfly:scale="5.0"/>
      <path fill="none" d="M18.5,101.13 L 116.09 101.13" id="img-a6536837-98" visibility="hidden" gadfly:scale="5.0"/>
      <path fill="none" d="M18.5,96.64 L 116.09 96.64" id="img-a6536837-99" visibility="hidden" gadfly:scale="5.0"/>
      <path fill="none" d="M18.5,92.16 L 116.09 92.16" id="img-a6536837-100" visibility="hidden" gadfly:scale="5.0"/>
      <path fill="none" d="M18.5,87.68 L 116.09 87.68" id="img-a6536837-101" visibility="hidden" gadfly:scale="5.0"/>
      <path fill="none" d="M18.5,83.2 L 116.09 83.2" id="img-a6536837-102" visibility="hidden" gadfly:scale="5.0"/>
      <path fill="none" d="M18.5,78.72 L 116.09 78.72" id="img-a6536837-103" visibility="hidden" gadfly:scale="5.0"/>
      <path fill="none" d="M18.5,74.23 L 116.09 74.23" id="img-a6536837-104" visibility="hidden" gadfly:scale="5.0"/>
      <path fill="none" d="M18.5,69.75 L 116.09 69.75" id="img-a6536837-105" visibility="hidden" gadfly:scale="5.0"/>
      <path fill="none" d="M18.5,65.27 L 116.09 65.27" id="img-a6536837-106" visibility="hidden" gadfly:scale="5.0"/>
      <path fill="none" d="M18.5,60.79 L 116.09 60.79" id="img-a6536837-107" visibility="hidden" gadfly:scale="5.0"/>
      <path fill="none" d="M18.5,56.3 L 116.09 56.3" id="img-a6536837-108" visibility="hidden" gadfly:scale="5.0"/>
      <path fill="none" d="M18.5,51.82 L 116.09 51.82" id="img-a6536837-109" visibility="hidden" gadfly:scale="5.0"/>
      <path fill="none" d="M18.5,47.34 L 116.09 47.34" id="img-a6536837-110" visibility="hidden" gadfly:scale="5.0"/>
      <path fill="none" d="M18.5,42.86 L 116.09 42.86" id="img-a6536837-111" visibility="hidden" gadfly:scale="5.0"/>
      <path fill="none" d="M18.5,38.38 L 116.09 38.38" id="img-a6536837-112" visibility="hidden" gadfly:scale="5.0"/>
      <path fill="none" d="M18.5,33.89 L 116.09 33.89" id="img-a6536837-113" visibility="hidden" gadfly:scale="5.0"/>
      <path fill="none" d="M18.5,29.41 L 116.09 29.41" id="img-a6536837-114" visibility="hidden" gadfly:scale="5.0"/>
      <path fill="none" d="M18.5,24.93 L 116.09 24.93" id="img-a6536837-115" visibility="hidden" gadfly:scale="5.0"/>
      <path fill="none" d="M18.5,20.45 L 116.09 20.45" id="img-a6536837-116" visibility="hidden" gadfly:scale="5.0"/>
      <path fill="none" d="M18.5,15.96 L 116.09 15.96" id="img-a6536837-117" visibility="hidden" gadfly:scale="5.0"/>
      <path fill="none" d="M18.5,11.48 L 116.09 11.48" id="img-a6536837-118" visibility="hidden" gadfly:scale="5.0"/>
      <path fill="none" d="M18.5,7 L 116.09 7" id="img-a6536837-119" visibility="hidden" gadfly:scale="5.0"/>
      <path fill="none" d="M18.5,2.52 L 116.09 2.52" id="img-a6536837-120" visibility="hidden" gadfly:scale="5.0"/>
      <path fill="none" d="M18.5,-1.96 L 116.09 -1.96" id="img-a6536837-121" visibility="hidden" gadfly:scale="5.0"/>
      <path fill="none" d="M18.5,-6.45 L 116.09 -6.45" id="img-a6536837-122" visibility="hidden" gadfly:scale="5.0"/>
      <path fill="none" d="M18.5,-10.93 L 116.09 -10.93" id="img-a6536837-123" visibility="hidden" gadfly:scale="5.0"/>
      <path fill="none" d="M18.5,-15.41 L 116.09 -15.41" id="img-a6536837-124" visibility="hidden" gadfly:scale="5.0"/>
      <path fill="none" d="M18.5,-19.89 L 116.09 -19.89" id="img-a6536837-125" visibility="hidden" gadfly:scale="5.0"/>
      <path fill="none" d="M18.5,-24.38 L 116.09 -24.38" id="img-a6536837-126" visibility="hidden" gadfly:scale="5.0"/>
      <path fill="none" d="M18.5,-28.86 L 116.09 -28.86" id="img-a6536837-127" visibility="hidden" gadfly:scale="5.0"/>
      <path fill="none" d="M18.5,-33.34 L 116.09 -33.34" id="img-a6536837-128" visibility="hidden" gadfly:scale="5.0"/>
      <path fill="none" d="M18.5,-37.82 L 116.09 -37.82" id="img-a6536837-129" visibility="hidden" gadfly:scale="5.0"/>
      <path fill="none" d="M18.5,-42.3 L 116.09 -42.3" id="img-a6536837-130" visibility="hidden" gadfly:scale="5.0"/>
      <path fill="none" d="M18.5,-46.79 L 116.09 -46.79" id="img-a6536837-131" visibility="hidden" gadfly:scale="5.0"/>
      <path fill="none" d="M18.5,-51.27 L 116.09 -51.27" id="img-a6536837-132" visibility="hidden" gadfly:scale="5.0"/>
      <path fill="none" d="M18.5,-55.75 L 116.09 -55.75" id="img-a6536837-133" visibility="hidden" gadfly:scale="5.0"/>
      <path fill="none" d="M18.5,-60.23 L 116.09 -60.23" id="img-a6536837-134" visibility="hidden" gadfly:scale="5.0"/>
      <path fill="none" d="M18.5,-64.72 L 116.09 -64.72" id="img-a6536837-135" visibility="hidden" gadfly:scale="5.0"/>
      <path fill="none" d="M18.5,-69.2 L 116.09 -69.2" id="img-a6536837-136" visibility="hidden" gadfly:scale="5.0"/>
    </g>
    <g class="guide xgridlines yfixed" stroke-dasharray="0.5,0.5" stroke-width="0.2" stroke="#D0D0E0" id="img-a6536837-137">
      <path fill="none" d="M-88.69,5 L -88.69 80.72" id="img-a6536837-138" visibility="hidden" gadfly:scale="1.0"/>
      <path fill="none" d="M-73.09,5 L -73.09 80.72" id="img-a6536837-139" visibility="hidden" gadfly:scale="1.0"/>
      <path fill="none" d="M-57.49,5 L -57.49 80.72" id="img-a6536837-140" visibility="hidden" gadfly:scale="1.0"/>
      <path fill="none" d="M-41.89,5 L -41.89 80.72" id="img-a6536837-141" visibility="hidden" gadfly:scale="1.0"/>
      <path fill="none" d="M-26.3,5 L -26.3 80.72" id="img-a6536837-142" visibility="hidden" gadfly:scale="1.0"/>
      <path fill="none" d="M-10.7,5 L -10.7 80.72" id="img-a6536837-143" visibility="hidden" gadfly:scale="1.0"/>
      <path fill="none" d="M4.9,5 L 4.9 80.72" id="img-a6536837-144" visibility="hidden" gadfly:scale="1.0"/>
      <path fill="none" d="M20.5,5 L 20.5 80.72" id="img-a6536837-145" visibility="visible" gadfly:scale="1.0"/>
      <path fill="none" d="M36.1,5 L 36.1 80.72" id="img-a6536837-146" visibility="visible" gadfly:scale="1.0"/>
      <path fill="none" d="M51.7,5 L 51.7 80.72" id="img-a6536837-147" visibility="visible" gadfly:scale="1.0"/>
      <path fill="none" d="M67.29,5 L 67.29 80.72" id="img-a6536837-148" visibility="visible" gadfly:scale="1.0"/>
      <path fill="none" d="M82.89,5 L 82.89 80.72" id="img-a6536837-149" visibility="visible" gadfly:scale="1.0"/>
      <path fill="none" d="M98.49,5 L 98.49 80.72" id="img-a6536837-150" visibility="visible" gadfly:scale="1.0"/>
      <path fill="none" d="M114.09,5 L 114.09 80.72" id="img-a6536837-151" visibility="visible" gadfly:scale="1.0"/>
      <path fill="none" d="M129.69,5 L 129.69 80.72" id="img-a6536837-152" visibility="hidden" gadfly:scale="1.0"/>
      <path fill="none" d="M145.29,5 L 145.29 80.72" id="img-a6536837-153" visibility="hidden" gadfly:scale="1.0"/>
      <path fill="none" d="M160.88,5 L 160.88 80.72" id="img-a6536837-154" visibility="hidden" gadfly:scale="1.0"/>
      <path fill="none" d="M176.48,5 L 176.48 80.72" id="img-a6536837-155" visibility="hidden" gadfly:scale="1.0"/>
      <path fill="none" d="M192.08,5 L 192.08 80.72" id="img-a6536837-156" visibility="hidden" gadfly:scale="1.0"/>
      <path fill="none" d="M207.68,5 L 207.68 80.72" id="img-a6536837-157" visibility="hidden" gadfly:scale="1.0"/>
      <path fill="none" d="M223.28,5 L 223.28 80.72" id="img-a6536837-158" visibility="hidden" gadfly:scale="1.0"/>
      <path fill="none" d="M-73.09,5 L -73.09 80.72" id="img-a6536837-159" visibility="hidden" gadfly:scale="10.0"/>
      <path fill="none" d="M-69.97,5 L -69.97 80.72" id="img-a6536837-160" visibility="hidden" gadfly:scale="10.0"/>
      <path fill="none" d="M-66.85,5 L -66.85 80.72" id="img-a6536837-161" visibility="hidden" gadfly:scale="10.0"/>
      <path fill="none" d="M-63.73,5 L -63.73 80.72" id="img-a6536837-162" visibility="hidden" gadfly:scale="10.0"/>
      <path fill="none" d="M-60.61,5 L -60.61 80.72" id="img-a6536837-163" visibility="hidden" gadfly:scale="10.0"/>
      <path fill="none" d="M-57.49,5 L -57.49 80.72" id="img-a6536837-164" visibility="hidden" gadfly:scale="10.0"/>
      <path fill="none" d="M-54.37,5 L -54.37 80.72" id="img-a6536837-165" visibility="hidden" gadfly:scale="10.0"/>
      <path fill="none" d="M-51.25,5 L -51.25 80.72" id="img-a6536837-166" visibility="hidden" gadfly:scale="10.0"/>
      <path fill="none" d="M-48.13,5 L -48.13 80.72" id="img-a6536837-167" visibility="hidden" gadfly:scale="10.0"/>
      <path fill="none" d="M-45.01,5 L -45.01 80.72" id="img-a6536837-168" visibility="hidden" gadfly:scale="10.0"/>
      <path fill="none" d="M-41.89,5 L -41.89 80.72" id="img-a6536837-169" visibility="hidden" gadfly:scale="10.0"/>
      <path fill="none" d="M-38.77,5 L -38.77 80.72" id="img-a6536837-170" visibility="hidden" gadfly:scale="10.0"/>
      <path fill="none" d="M-35.66,5 L -35.66 80.72" id="img-a6536837-171" visibility="hidden" gadfly:scale="10.0"/>
      <path fill="none" d="M-32.54,5 L -32.54 80.72" id="img-a6536837-172" visibility="hidden" gadfly:scale="10.0"/>
      <path fill="none" d="M-29.42,5 L -29.42 80.72" id="img-a6536837-173" visibility="hidden" gadfly:scale="10.0"/>
      <path fill="none" d="M-26.3,5 L -26.3 80.72" id="img-a6536837-174" visibility="hidden" gadfly:scale="10.0"/>
      <path fill="none" d="M-23.18,5 L -23.18 80.72" id="img-a6536837-175" visibility="hidden" gadfly:scale="10.0"/>
      <path fill="none" d="M-20.06,5 L -20.06 80.72" id="img-a6536837-176" visibility="hidden" gadfly:scale="10.0"/>
      <path fill="none" d="M-16.94,5 L -16.94 80.72" id="img-a6536837-177" visibility="hidden" gadfly:scale="10.0"/>
      <path fill="none" d="M-13.82,5 L -13.82 80.72" id="img-a6536837-178" visibility="hidden" gadfly:scale="10.0"/>
      <path fill="none" d="M-10.7,5 L -10.7 80.72" id="img-a6536837-179" visibility="hidden" gadfly:scale="10.0"/>
      <path fill="none" d="M-7.58,5 L -7.58 80.72" id="img-a6536837-180" visibility="hidden" gadfly:scale="10.0"/>
      <path fill="none" d="M-4.46,5 L -4.46 80.72" id="img-a6536837-181" visibility="hidden" gadfly:scale="10.0"/>
      <path fill="none" d="M-1.34,5 L -1.34 80.72" id="img-a6536837-182" visibility="hidden" gadfly:scale="10.0"/>
      <path fill="none" d="M1.78,5 L 1.78 80.72" id="img-a6536837-183" visibility="hidden" gadfly:scale="10.0"/>
      <path fill="none" d="M4.9,5 L 4.9 80.72" id="img-a6536837-184" visibility="hidden" gadfly:scale="10.0"/>
      <path fill="none" d="M8.02,5 L 8.02 80.72" id="img-a6536837-185" visibility="hidden" gadfly:scale="10.0"/>
      <path fill="none" d="M11.14,5 L 11.14 80.72" id="img-a6536837-186" visibility="hidden" gadfly:scale="10.0"/>
      <path fill="none" d="M14.26,5 L 14.26 80.72" id="img-a6536837-187" visibility="hidden" gadfly:scale="10.0"/>
      <path fill="none" d="M17.38,5 L 17.38 80.72" id="img-a6536837-188" visibility="hidden" gadfly:scale="10.0"/>
      <path fill="none" d="M20.5,5 L 20.5 80.72" id="img-a6536837-189" visibility="hidden" gadfly:scale="10.0"/>
      <path fill="none" d="M23.62,5 L 23.62 80.72" id="img-a6536837-190" visibility="hidden" gadfly:scale="10.0"/>
      <path fill="none" d="M26.74,5 L 26.74 80.72" id="img-a6536837-191" visibility="hidden" gadfly:scale="10.0"/>
      <path fill="none" d="M29.86,5 L 29.86 80.72" id="img-a6536837-192" visibility="hidden" gadfly:scale="10.0"/>
      <path fill="none" d="M32.98,5 L 32.98 80.72" id="img-a6536837-193" visibility="hidden" gadfly:scale="10.0"/>
      <path fill="none" d="M36.1,5 L 36.1 80.72" id="img-a6536837-194" visibility="hidden" gadfly:scale="10.0"/>
      <path fill="none" d="M39.22,5 L 39.22 80.72" id="img-a6536837-195" visibility="hidden" gadfly:scale="10.0"/>
      <path fill="none" d="M42.34,5 L 42.34 80.72" id="img-a6536837-196" visibility="hidden" gadfly:scale="10.0"/>
      <path fill="none" d="M45.46,5 L 45.46 80.72" id="img-a6536837-197" visibility="hidden" gadfly:scale="10.0"/>
      <path fill="none" d="M48.58,5 L 48.58 80.72" id="img-a6536837-198" visibility="hidden" gadfly:scale="10.0"/>
      <path fill="none" d="M51.7,5 L 51.7 80.72" id="img-a6536837-199" visibility="hidden" gadfly:scale="10.0"/>
      <path fill="none" d="M54.82,5 L 54.82 80.72" id="img-a6536837-200" visibility="hidden" gadfly:scale="10.0"/>
      <path fill="none" d="M57.94,5 L 57.94 80.72" id="img-a6536837-201" visibility="hidden" gadfly:scale="10.0"/>
      <path fill="none" d="M61.06,5 L 61.06 80.72" id="img-a6536837-202" visibility="hidden" gadfly:scale="10.0"/>
      <path fill="none" d="M64.17,5 L 64.17 80.72" id="img-a6536837-203" visibility="hidden" gadfly:scale="10.0"/>
      <path fill="none" d="M67.29,5 L 67.29 80.72" id="img-a6536837-204" visibility="hidden" gadfly:scale="10.0"/>
      <path fill="none" d="M70.41,5 L 70.41 80.72" id="img-a6536837-205" visibility="hidden" gadfly:scale="10.0"/>
      <path fill="none" d="M73.53,5 L 73.53 80.72" id="img-a6536837-206" visibility="hidden" gadfly:scale="10.0"/>
      <path fill="none" d="M76.65,5 L 76.65 80.72" id="img-a6536837-207" visibility="hidden" gadfly:scale="10.0"/>
      <path fill="none" d="M79.77,5 L 79.77 80.72" id="img-a6536837-208" visibility="hidden" gadfly:scale="10.0"/>
      <path fill="none" d="M82.89,5 L 82.89 80.72" id="img-a6536837-209" visibility="hidden" gadfly:scale="10.0"/>
      <path fill="none" d="M86.01,5 L 86.01 80.72" id="img-a6536837-210" visibility="hidden" gadfly:scale="10.0"/>
      <path fill="none" d="M89.13,5 L 89.13 80.72" id="img-a6536837-211" visibility="hidden" gadfly:scale="10.0"/>
      <path fill="none" d="M92.25,5 L 92.25 80.72" id="img-a6536837-212" visibility="hidden" gadfly:scale="10.0"/>
      <path fill="none" d="M95.37,5 L 95.37 80.72" id="img-a6536837-213" visibility="hidden" gadfly:scale="10.0"/>
      <path fill="none" d="M98.49,5 L 98.49 80.72" id="img-a6536837-214" visibility="hidden" gadfly:scale="10.0"/>
      <path fill="none" d="M101.61,5 L 101.61 80.72" id="img-a6536837-215" visibility="hidden" gadfly:scale="10.0"/>
      <path fill="none" d="M104.73,5 L 104.73 80.72" id="img-a6536837-216" visibility="hidden" gadfly:scale="10.0"/>
      <path fill="none" d="M107.85,5 L 107.85 80.72" id="img-a6536837-217" visibility="hidden" gadfly:scale="10.0"/>
      <path fill="none" d="M110.97,5 L 110.97 80.72" id="img-a6536837-218" visibility="hidden" gadfly:scale="10.0"/>
      <path fill="none" d="M114.09,5 L 114.09 80.72" id="img-a6536837-219" visibility="hidden" gadfly:scale="10.0"/>
      <path fill="none" d="M117.21,5 L 117.21 80.72" id="img-a6536837-220" visibility="hidden" gadfly:scale="10.0"/>
      <path fill="none" d="M120.33,5 L 120.33 80.72" id="img-a6536837-221" visibility="hidden" gadfly:scale="10.0"/>
      <path fill="none" d="M123.45,5 L 123.45 80.72" id="img-a6536837-222" visibility="hidden" gadfly:scale="10.0"/>
      <path fill="none" d="M126.57,5 L 126.57 80.72" id="img-a6536837-223" visibility="hidden" gadfly:scale="10.0"/>
      <path fill="none" d="M129.69,5 L 129.69 80.72" id="img-a6536837-224" visibility="hidden" gadfly:scale="10.0"/>
      <path fill="none" d="M132.81,5 L 132.81 80.72" id="img-a6536837-225" visibility="hidden" gadfly:scale="10.0"/>
      <path fill="none" d="M135.93,5 L 135.93 80.72" id="img-a6536837-226" visibility="hidden" gadfly:scale="10.0"/>
      <path fill="none" d="M139.05,5 L 139.05 80.72" id="img-a6536837-227" visibility="hidden" gadfly:scale="10.0"/>
      <path fill="none" d="M142.17,5 L 142.17 80.72" id="img-a6536837-228" visibility="hidden" gadfly:scale="10.0"/>
      <path fill="none" d="M145.29,5 L 145.29 80.72" id="img-a6536837-229" visibility="hidden" gadfly:scale="10.0"/>
      <path fill="none" d="M148.41,5 L 148.41 80.72" id="img-a6536837-230" visibility="hidden" gadfly:scale="10.0"/>
      <path fill="none" d="M151.53,5 L 151.53 80.72" id="img-a6536837-231" visibility="hidden" gadfly:scale="10.0"/>
      <path fill="none" d="M154.65,5 L 154.65 80.72" id="img-a6536837-232" visibility="hidden" gadfly:scale="10.0"/>
      <path fill="none" d="M157.77,5 L 157.77 80.72" id="img-a6536837-233" visibility="hidden" gadfly:scale="10.0"/>
      <path fill="none" d="M160.88,5 L 160.88 80.72" id="img-a6536837-234" visibility="hidden" gadfly:scale="10.0"/>
      <path fill="none" d="M164,5 L 164 80.72" id="img-a6536837-235" visibility="hidden" gadfly:scale="10.0"/>
      <path fill="none" d="M167.12,5 L 167.12 80.72" id="img-a6536837-236" visibility="hidden" gadfly:scale="10.0"/>
      <path fill="none" d="M170.24,5 L 170.24 80.72" id="img-a6536837-237" visibility="hidden" gadfly:scale="10.0"/>
      <path fill="none" d="M173.36,5 L 173.36 80.72" id="img-a6536837-238" visibility="hidden" gadfly:scale="10.0"/>
      <path fill="none" d="M176.48,5 L 176.48 80.72" id="img-a6536837-239" visibility="hidden" gadfly:scale="10.0"/>
      <path fill="none" d="M179.6,5 L 179.6 80.72" id="img-a6536837-240" visibility="hidden" gadfly:scale="10.0"/>
      <path fill="none" d="M182.72,5 L 182.72 80.72" id="img-a6536837-241" visibility="hidden" gadfly:scale="10.0"/>
      <path fill="none" d="M185.84,5 L 185.84 80.72" id="img-a6536837-242" visibility="hidden" gadfly:scale="10.0"/>
      <path fill="none" d="M188.96,5 L 188.96 80.72" id="img-a6536837-243" visibility="hidden" gadfly:scale="10.0"/>
      <path fill="none" d="M192.08,5 L 192.08 80.72" id="img-a6536837-244" visibility="hidden" gadfly:scale="10.0"/>
      <path fill="none" d="M195.2,5 L 195.2 80.72" id="img-a6536837-245" visibility="hidden" gadfly:scale="10.0"/>
      <path fill="none" d="M198.32,5 L 198.32 80.72" id="img-a6536837-246" visibility="hidden" gadfly:scale="10.0"/>
      <path fill="none" d="M201.44,5 L 201.44 80.72" id="img-a6536837-247" visibility="hidden" gadfly:scale="10.0"/>
      <path fill="none" d="M204.56,5 L 204.56 80.72" id="img-a6536837-248" visibility="hidden" gadfly:scale="10.0"/>
      <path fill="none" d="M207.68,5 L 207.68 80.72" id="img-a6536837-249" visibility="hidden" gadfly:scale="10.0"/>
      <path fill="none" d="M-135.49,5 L -135.49 80.72" id="img-a6536837-250" visibility="hidden" gadfly:scale="0.5"/>
      <path fill="none" d="M20.5,5 L 20.5 80.72" id="img-a6536837-251" visibility="hidden" gadfly:scale="0.5"/>
      <path fill="none" d="M176.48,5 L 176.48 80.72" id="img-a6536837-252" visibility="hidden" gadfly:scale="0.5"/>
      <path fill="none" d="M332.47,5 L 332.47 80.72" id="img-a6536837-253" visibility="hidden" gadfly:scale="0.5"/>
      <path fill="none" d="M-73.09,5 L -73.09 80.72" id="img-a6536837-254" visibility="hidden" gadfly:scale="5.0"/>
      <path fill="none" d="M-65.29,5 L -65.29 80.72" id="img-a6536837-255" visibility="hidden" gadfly:scale="5.0"/>
      <path fill="none" d="M-57.49,5 L -57.49 80.72" id="img-a6536837-256" visibility="hidden" gadfly:scale="5.0"/>
      <path fill="none" d="M-49.69,5 L -49.69 80.72" id="img-a6536837-257" visibility="hidden" gadfly:scale="5.0"/>
      <path fill="none" d="M-41.89,5 L -41.89 80.72" id="img-a6536837-258" visibility="hidden" gadfly:scale="5.0"/>
      <path fill="none" d="M-34.1,5 L -34.1 80.72" id="img-a6536837-259" visibility="hidden" gadfly:scale="5.0"/>
      <path fill="none" d="M-26.3,5 L -26.3 80.72" id="img-a6536837-260" visibility="hidden" gadfly:scale="5.0"/>
      <path fill="none" d="M-18.5,5 L -18.5 80.72" id="img-a6536837-261" visibility="hidden" gadfly:scale="5.0"/>
      <path fill="none" d="M-10.7,5 L -10.7 80.72" id="img-a6536837-262" visibility="hidden" gadfly:scale="5.0"/>
      <path fill="none" d="M-2.9,5 L -2.9 80.72" id="img-a6536837-263" visibility="hidden" gadfly:scale="5.0"/>
      <path fill="none" d="M4.9,5 L 4.9 80.72" id="img-a6536837-264" visibility="hidden" gadfly:scale="5.0"/>
      <path fill="none" d="M12.7,5 L 12.7 80.72" id="img-a6536837-265" visibility="hidden" gadfly:scale="5.0"/>
      <path fill="none" d="M20.5,5 L 20.5 80.72" id="img-a6536837-266" visibility="hidden" gadfly:scale="5.0"/>
      <path fill="none" d="M28.3,5 L 28.3 80.72" id="img-a6536837-267" visibility="hidden" gadfly:scale="5.0"/>
      <path fill="none" d="M36.1,5 L 36.1 80.72" id="img-a6536837-268" visibility="hidden" gadfly:scale="5.0"/>
      <path fill="none" d="M43.9,5 L 43.9 80.72" id="img-a6536837-269" visibility="hidden" gadfly:scale="5.0"/>
      <path fill="none" d="M51.7,5 L 51.7 80.72" id="img-a6536837-270" visibility="hidden" gadfly:scale="5.0"/>
      <path fill="none" d="M59.5,5 L 59.5 80.72" id="img-a6536837-271" visibility="hidden" gadfly:scale="5.0"/>
      <path fill="none" d="M67.29,5 L 67.29 80.72" id="img-a6536837-272" visibility="hidden" gadfly:scale="5.0"/>
      <path fill="none" d="M75.09,5 L 75.09 80.72" id="img-a6536837-273" visibility="hidden" gadfly:scale="5.0"/>
      <path fill="none" d="M82.89,5 L 82.89 80.72" id="img-a6536837-274" visibility="hidden" gadfly:scale="5.0"/>
      <path fill="none" d="M90.69,5 L 90.69 80.72" id="img-a6536837-275" visibility="hidden" gadfly:scale="5.0"/>
      <path fill="none" d="M98.49,5 L 98.49 80.72" id="img-a6536837-276" visibility="hidden" gadfly:scale="5.0"/>
      <path fill="none" d="M106.29,5 L 106.29 80.72" id="img-a6536837-277" visibility="hidden" gadfly:scale="5.0"/>
      <path fill="none" d="M114.09,5 L 114.09 80.72" id="img-a6536837-278" visibility="hidden" gadfly:scale="5.0"/>
      <path fill="none" d="M121.89,5 L 121.89 80.72" id="img-a6536837-279" visibility="hidden" gadfly:scale="5.0"/>
      <path fill="none" d="M129.69,5 L 129.69 80.72" id="img-a6536837-280" visibility="hidden" gadfly:scale="5.0"/>
      <path fill="none" d="M137.49,5 L 137.49 80.72" id="img-a6536837-281" visibility="hidden" gadfly:scale="5.0"/>
      <path fill="none" d="M145.29,5 L 145.29 80.72" id="img-a6536837-282" visibility="hidden" gadfly:scale="5.0"/>
      <path fill="none" d="M153.09,5 L 153.09 80.72" id="img-a6536837-283" visibility="hidden" gadfly:scale="5.0"/>
      <path fill="none" d="M160.88,5 L 160.88 80.72" id="img-a6536837-284" visibility="hidden" gadfly:scale="5.0"/>
      <path fill="none" d="M168.68,5 L 168.68 80.72" id="img-a6536837-285" visibility="hidden" gadfly:scale="5.0"/>
      <path fill="none" d="M176.48,5 L 176.48 80.72" id="img-a6536837-286" visibility="hidden" gadfly:scale="5.0"/>
      <path fill="none" d="M184.28,5 L 184.28 80.72" id="img-a6536837-287" visibility="hidden" gadfly:scale="5.0"/>
      <path fill="none" d="M192.08,5 L 192.08 80.72" id="img-a6536837-288" visibility="hidden" gadfly:scale="5.0"/>
      <path fill="none" d="M199.88,5 L 199.88 80.72" id="img-a6536837-289" visibility="hidden" gadfly:scale="5.0"/>
      <path fill="none" d="M207.68,5 L 207.68 80.72" id="img-a6536837-290" visibility="hidden" gadfly:scale="5.0"/>
    </g>
    <g class="plotpanel" id="img-a6536837-291">
      <g stroke-width="0.3" stroke="#000000" stroke-opacity="0.000" class="geometry" id="img-a6536837-292">
        <path d="M114.09,78.69 L 112.53 78.69 110.97 78.69 109.41 78.69 107.85 78.69 106.29 78.68 104.73 78.68 103.17 78.68 101.61 78.67 100.05 78.67 98.49 78.66 96.93 78.65 95.37 78.63 93.81 78.61 92.25 78.57 90.69 78.52 89.13 78.45 87.57 78.36 86.01 78.27 84.45 78.13 82.89 78 81.33 77.91 79.77 77.86 78.21 77.76 76.65 77.66 75.09 77.49 73.53 77.21 71.97 76.84 70.41 76.27 68.85 75.41 67.29 74.41 65.73 72.94 64.17 70.88 62.61 68.44 61.06 65.18 59.5 61.21 57.94 57.48 56.38 53.33 54.82 49.63 53.26 45.93 51.7 43.12 50.14 40.84 48.58 38.76 47.02 37.94 45.46 36.77 43.9 37.59 42.34 40.69 40.78 43.58 39.22 47.66 37.66 53.83 36.1 60.97 34.54 66.83 32.98 71.41 31.42 74.77 29.86 76.7 28.3 77.69 26.74 78.15 25.18 78.34 23.62 78.44 22.06 78.52 22.06 78.36 23.62 77.81 25.18 76.65 26.74 74.55 28.3 71.67 29.86 66.97 31.42 61.36 32.98 54.89 34.54 47.13 36.1 39.09 37.66 31.4 39.22 25.27 40.78 19.89 42.34 14.93 43.9 11.7 45.46 10.54 47.02 9.5 48.58 10.84 50.14 14.62 51.7 19.16 53.26 24.05 54.82 29.29 56.38 34.01 57.94 39.14 59.5 44.16 61.06 48.84 62.61 52.83 64.17 56.28 65.73 59.59 67.29 62.11 68.85 64.37 70.41 66.53 71.97 68.38 73.53 69.89 75.09 71.16 76.65 72.24 78.21 72.95 79.77 73.51 81.33 73.94 82.89 74.45 84.45 74.74 86.01 75.12 87.57 75.42 89.13 75.58 90.69 75.72 92.25 75.9 93.81 76.11 95.37 76.34 96.93 76.49 98.49 76.54 100.05 76.6 101.61 76.68 103.17 76.79 104.73 76.91 106.29 77.07 107.85 77.25 109.41 77.41 110.97 77.52 112.53 77.68 114.09 77.79 z" id="img-a6536837-293" fill="#EBE3BD"/>
        <path d="M114.09,78.69 L 112.53 78.69 110.97 78.68 109.41 78.68 107.85 78.67 106.29 78.66 104.73 78.63 103.17 78.6 101.61 78.55 100.05 78.48 98.49 78.37 96.93 78.21 95.37 78 93.81 77.74 92.25 77.35 90.69 76.91 89.13 76.38 87.57 75.65 86.01 74.81 84.45 73.68 82.89 72.71 81.33 71.78 79.77 71.09 78.21 70.56 76.65 69.91 75.09 68.99 73.53 68.06 71.97 67.2 70.41 66.47 68.85 65.7 67.29 64.92 65.73 63.77 64.17 62.37 62.61 61.14 61.06 59.36 59.5 57.62 57.94 55.7 56.38 52.63 54.82 50.06 53.26 45.99 51.7 43.75 50.14 42.03 48.58 41.45 47.02 42.63 45.46 46.01 43.9 50.89 42.34 54.48 40.78 58.84 39.22 62.93 37.66 66.97 36.1 70.64 34.54 73.63 32.98 75.8 31.42 77.12 29.86 77.83 28.3 78.16 26.74 78.31 25.18 78.39 23.62 78.45 22.06 78.52 22.06 78.42 23.62 78.08 25.18 77.45 26.74 76.5 28.3 75.01 29.86 72.97 31.42 70.55 32.98 68.01 34.54 64.28 36.1 59.47 37.66 53.39 39.22 47.06 40.78 40.48 42.34 34.41 43.9 29.5 45.46 25.69 47.02 23.93 48.58 22.98 50.14 22.96 51.7 24.17 53.26 26.22 54.82 29.03 56.38 31.7 57.94 35.35 59.5 39.3 61.06 42.85 62.61 46.19 64.17 48.42 65.73 50.34 67.29 51.99 68.85 53.9 70.41 55.04 71.97 56.35 73.53 57.88 75.09 59.18 76.65 60.48 78.21 61.84 79.77 63.18 81.33 64.75 82.89 66.32 84.45 67.69 86.01 69.1 87.57 70.31 89.13 71.35 90.69 72.27 92.25 73.15 93.81 73.81 95.37 74.46 96.93 75.08 98.49 75.73 100.05 76.19 101.61 76.54 103.17 76.88 104.73 77.18 106.29 77.39 107.85 77.6 109.41 77.75 110.97 77.92 112.53 78.02 114.09 78.13 z" id="img-a6536837-294" fill="#BDE8FF"/>
      </g>
      <g stroke-width="0.3" fill="#000000" fill-opacity="0.000" stroke-dasharray="none" id="img-a6536837-295">
        <path fill="none" d="M22.06,78.51 L 23.62 78.4 25.18 78.22 26.74 77.92 28.3 77.37 29.86 76.44 31.42 74.94 32.98 72.69 34.54 69.54 36.1 65.43 37.66 60.46 39.22 54.89 40.78 49.1 42.34 43.58 43.9 38.79 45.46 35.12 47.02 32.8 48.58 31.9 50.14 32.34 51.7 33.9 53.26 36.31 54.82 39.26 56.38 42.47 57.94 45.68 59.5 48.71 61.06 51.44 62.61 53.8 64.17 55.77 65.73 57.41 67.29 58.76 68.85 59.9 70.41 60.91 71.97 61.87 73.53 62.83 75.09 63.83 76.65 64.89 78.21 66.02 79.77 67.2 81.33 68.41 82.89 69.61 84.45 70.79 86.01 71.91 87.57 72.95 89.13 73.89 90.69 74.73 92.25 75.46 93.81 76.09 95.37 76.62 96.93 77.05 98.49 77.41 100.05 77.69 101.61 77.92 103.17 78.1 104.73 78.24 106.29 78.34 107.85 78.43 109.41 78.49 110.97 78.54 112.53 78.57 114.09 78.6" id="img-a6536837-296" stroke="#00BFFF" class="geometry color_No"/>
        <path fill="none" d="M22.06,78.5 L 23.62 78.33 25.18 77.97 26.74 77.2 28.3 75.69 29.86 73.1 31.42 69.16 32.98 63.78 34.54 57.21 36.1 49.93 37.66 42.62 39.22 35.96 40.78 30.5 42.34 26.54 43.9 24.18 45.46 23.31 47.02 23.76 48.58 25.31 50.14 27.78 51.7 31 53.26 34.83 54.82 39.1 56.38 43.63 57.94 48.23 59.5 52.71 61.06 56.91 62.61 60.71 64.17 64.02 65.73 66.8 67.29 69.08 68.85 70.9 70.41 72.3 71.97 73.37 73.53 74.18 75.09 74.78 76.65 75.24 78.21 75.59 79.77 75.87 81.33 76.1 82.89 76.3 84.45 76.49 86.01 76.66 87.57 76.83 89.13 76.99 90.69 77.15 92.25 77.3 93.81 77.44 95.37 77.58 96.93 77.71 98.49 77.83 100.05 77.95 101.61 78.05 103.17 78.14 104.73 78.23 106.29 78.3 107.85 78.37 109.41 78.42 110.97 78.47 112.53 78.51 114.09 78.55" id="img-a6536837-297" stroke="#D4CA3A" class="geometry color_Yes"/>
      </g>
    </g>
    <g opacity="0" class="guide zoomslider" stroke="#000000" stroke-opacity="0.000" id="img-a6536837-298">
      <g fill="#EAEAEA" stroke-width="0.3" stroke-opacity="0" stroke="#6A6A6A" id="img-a6536837-299">
        <rect x="109.09" y="8" width="4" height="4" id="img-a6536837-300"/>
        <g class="button_logo" fill="#6A6A6A" id="img-a6536837-301">
          <path d="M109.89,9.6 L 110.69 9.6 110.69 8.8 111.49 8.8 111.49 9.6 112.29 9.6 112.29 10.4 111.49 10.4 111.49 11.2 110.69 11.2 110.69 10.4 109.89 10.4 z" id="img-a6536837-302"/>
        </g>
      </g>
      <g fill="#EAEAEA" id="img-a6536837-303">
        <rect x="89.59" y="8" width="19" height="4" id="img-a6536837-304"/>
      </g>
      <g class="zoomslider_thumb" fill="#6A6A6A" id="img-a6536837-305">
        <rect x="98.09" y="8" width="2" height="4" id="img-a6536837-306"/>
      </g>
      <g fill="#EAEAEA" stroke-width="0.3" stroke-opacity="0" stroke="#6A6A6A" id="img-a6536837-307">
        <rect x="85.09" y="8" width="4" height="4" id="img-a6536837-308"/>
        <g class="button_logo" fill="#6A6A6A" id="img-a6536837-309">
          <path d="M85.89,9.6 L 88.29 9.6 88.29 10.4 85.89 10.4 z" id="img-a6536837-310"/>
        </g>
      </g>
    </g>
  </g>
</g>
  <g class="guide ylabels" font-size="2.82" font-family="'PT Sans Caption','Helvetica Neue','Helvetica',sans-serif" fill="#6C606B" id="img-a6536837-311">
    <text x="17.5" y="168.36" text-anchor="end" dy="0.35em" id="img-a6536837-312" visibility="hidden" gadfly:scale="1.0">-0.10</text>
    <text x="17.5" y="150.43" text-anchor="end" dy="0.35em" id="img-a6536837-313" visibility="hidden" gadfly:scale="1.0">-0.08</text>
    <text x="17.5" y="132.5" text-anchor="end" dy="0.35em" id="img-a6536837-314" visibility="hidden" gadfly:scale="1.0">-0.06</text>
    <text x="17.5" y="114.57" text-anchor="end" dy="0.35em" id="img-a6536837-315" visibility="hidden" gadfly:scale="1.0">-0.04</text>
    <text x="17.5" y="96.64" text-anchor="end" dy="0.35em" id="img-a6536837-316" visibility="hidden" gadfly:scale="1.0">-0.02</text>
    <text x="17.5" y="78.72" text-anchor="end" dy="0.35em" id="img-a6536837-317" visibility="visible" gadfly:scale="1.0">0.00</text>
    <text x="17.5" y="60.79" text-anchor="end" dy="0.35em" id="img-a6536837-318" visibility="visible" gadfly:scale="1.0">0.02</text>
    <text x="17.5" y="42.86" text-anchor="end" dy="0.35em" id="img-a6536837-319" visibility="visible" gadfly:scale="1.0">0.04</text>
    <text x="17.5" y="24.93" text-anchor="end" dy="0.35em" id="img-a6536837-320" visibility="visible" gadfly:scale="1.0">0.06</text>
    <text x="17.5" y="7" text-anchor="end" dy="0.35em" id="img-a6536837-321" visibility="visible" gadfly:scale="1.0">0.08</text>
    <text x="17.5" y="-10.93" text-anchor="end" dy="0.35em" id="img-a6536837-322" visibility="hidden" gadfly:scale="1.0">0.10</text>
    <text x="17.5" y="-28.86" text-anchor="end" dy="0.35em" id="img-a6536837-323" visibility="hidden" gadfly:scale="1.0">0.12</text>
    <text x="17.5" y="-46.79" text-anchor="end" dy="0.35em" id="img-a6536837-324" visibility="hidden" gadfly:scale="1.0">0.14</text>
    <text x="17.5" y="-64.72" text-anchor="end" dy="0.35em" id="img-a6536837-325" visibility="hidden" gadfly:scale="1.0">0.16</text>
    <text x="17.5" y="-82.64" text-anchor="end" dy="0.35em" id="img-a6536837-326" visibility="hidden" gadfly:scale="1.0">0.18</text>
    <text x="17.5" y="150.43" text-anchor="end" dy="0.35em" id="img-a6536837-327" visibility="hidden" gadfly:scale="10.0">-0.080</text>
    <text x="17.5" y="145.95" text-anchor="end" dy="0.35em" id="img-a6536837-328" visibility="hidden" gadfly:scale="10.0">-0.075</text>
    <text x="17.5" y="141.47" text-anchor="end" dy="0.35em" id="img-a6536837-329" visibility="hidden" gadfly:scale="10.0">-0.070</text>
    <text x="17.5" y="136.98" text-anchor="end" dy="0.35em" id="img-a6536837-330" visibility="hidden" gadfly:scale="10.0">-0.065</text>
    <text x="17.5" y="132.5" text-anchor="end" dy="0.35em" id="img-a6536837-331" visibility="hidden" gadfly:scale="10.0">-0.060</text>
    <text x="17.5" y="128.02" text-anchor="end" dy="0.35em" id="img-a6536837-332" visibility="hidden" gadfly:scale="10.0">-0.055</text>
    <text x="17.5" y="123.54" text-anchor="end" dy="0.35em" id="img-a6536837-333" visibility="hidden" gadfly:scale="10.0">-0.050</text>
    <text x="17.5" y="119.05" text-anchor="end" dy="0.35em" id="img-a6536837-334" visibility="hidden" gadfly:scale="10.0">-0.045</text>
    <text x="17.5" y="114.57" text-anchor="end" dy="0.35em" id="img-a6536837-335" visibility="hidden" gadfly:scale="10.0">-0.040</text>
    <text x="17.5" y="110.09" text-anchor="end" dy="0.35em" id="img-a6536837-336" visibility="hidden" gadfly:scale="10.0">-0.035</text>
    <text x="17.5" y="105.61" text-anchor="end" dy="0.35em" id="img-a6536837-337" visibility="hidden" gadfly:scale="10.0">-0.030</text>
    <text x="17.5" y="101.13" text-anchor="end" dy="0.35em" id="img-a6536837-338" visibility="hidden" gadfly:scale="10.0">-0.025</text>
    <text x="17.5" y="96.64" text-anchor="end" dy="0.35em" id="img-a6536837-339" visibility="hidden" gadfly:scale="10.0">-0.020</text>
    <text x="17.5" y="92.16" text-anchor="end" dy="0.35em" id="img-a6536837-340" visibility="hidden" gadfly:scale="10.0">-0.015</text>
    <text x="17.5" y="87.68" text-anchor="end" dy="0.35em" id="img-a6536837-341" visibility="hidden" gadfly:scale="10.0">-0.010</text>
    <text x="17.5" y="83.2" text-anchor="end" dy="0.35em" id="img-a6536837-342" visibility="hidden" gadfly:scale="10.0">-0.005</text>
    <text x="17.5" y="78.72" text-anchor="end" dy="0.35em" id="img-a6536837-343" visibility="hidden" gadfly:scale="10.0">0.000</text>
    <text x="17.5" y="74.23" text-anchor="end" dy="0.35em" id="img-a6536837-344" visibility="hidden" gadfly:scale="10.0">0.005</text>
    <text x="17.5" y="69.75" text-anchor="end" dy="0.35em" id="img-a6536837-345" visibility="hidden" gadfly:scale="10.0">0.010</text>
    <text x="17.5" y="65.27" text-anchor="end" dy="0.35em" id="img-a6536837-346" visibility="hidden" gadfly:scale="10.0">0.015</text>
    <text x="17.5" y="60.79" text-anchor="end" dy="0.35em" id="img-a6536837-347" visibility="hidden" gadfly:scale="10.0">0.020</text>
    <text x="17.5" y="56.3" text-anchor="end" dy="0.35em" id="img-a6536837-348" visibility="hidden" gadfly:scale="10.0">0.025</text>
    <text x="17.5" y="51.82" text-anchor="end" dy="0.35em" id="img-a6536837-349" visibility="hidden" gadfly:scale="10.0">0.030</text>
    <text x="17.5" y="47.34" text-anchor="end" dy="0.35em" id="img-a6536837-350" visibility="hidden" gadfly:scale="10.0">0.035</text>
    <text x="17.5" y="42.86" text-anchor="end" dy="0.35em" id="img-a6536837-351" visibility="hidden" gadfly:scale="10.0">0.040</text>
    <text x="17.5" y="38.38" text-anchor="end" dy="0.35em" id="img-a6536837-352" visibility="hidden" gadfly:scale="10.0">0.045</text>
    <text x="17.5" y="33.89" text-anchor="end" dy="0.35em" id="img-a6536837-353" visibility="hidden" gadfly:scale="10.0">0.050</text>
    <text x="17.5" y="29.41" text-anchor="end" dy="0.35em" id="img-a6536837-354" visibility="hidden" gadfly:scale="10.0">0.055</text>
    <text x="17.5" y="24.93" text-anchor="end" dy="0.35em" id="img-a6536837-355" visibility="hidden" gadfly:scale="10.0">0.060</text>
    <text x="17.5" y="20.45" text-anchor="end" dy="0.35em" id="img-a6536837-356" visibility="hidden" gadfly:scale="10.0">0.065</text>
    <text x="17.5" y="15.96" text-anchor="end" dy="0.35em" id="img-a6536837-357" visibility="hidden" gadfly:scale="10.0">0.070</text>
    <text x="17.5" y="11.48" text-anchor="end" dy="0.35em" id="img-a6536837-358" visibility="hidden" gadfly:scale="10.0">0.075</text>
    <text x="17.5" y="7" text-anchor="end" dy="0.35em" id="img-a6536837-359" visibility="hidden" gadfly:scale="10.0">0.080</text>
    <text x="17.5" y="2.52" text-anchor="end" dy="0.35em" id="img-a6536837-360" visibility="hidden" gadfly:scale="10.0">0.085</text>
    <text x="17.5" y="-1.96" text-anchor="end" dy="0.35em" id="img-a6536837-361" visibility="hidden" gadfly:scale="10.0">0.090</text>
    <text x="17.5" y="-6.45" text-anchor="end" dy="0.35em" id="img-a6536837-362" visibility="hidden" gadfly:scale="10.0">0.095</text>
    <text x="17.5" y="-10.93" text-anchor="end" dy="0.35em" id="img-a6536837-363" visibility="hidden" gadfly:scale="10.0">0.100</text>
    <text x="17.5" y="-15.41" text-anchor="end" dy="0.35em" id="img-a6536837-364" visibility="hidden" gadfly:scale="10.0">0.105</text>
    <text x="17.5" y="-19.89" text-anchor="end" dy="0.35em" id="img-a6536837-365" visibility="hidden" gadfly:scale="10.0">0.110</text>
    <text x="17.5" y="-24.38" text-anchor="end" dy="0.35em" id="img-a6536837-366" visibility="hidden" gadfly:scale="10.0">0.115</text>
    <text x="17.5" y="-28.86" text-anchor="end" dy="0.35em" id="img-a6536837-367" visibility="hidden" gadfly:scale="10.0">0.120</text>
    <text x="17.5" y="-33.34" text-anchor="end" dy="0.35em" id="img-a6536837-368" visibility="hidden" gadfly:scale="10.0">0.125</text>
    <text x="17.5" y="-37.82" text-anchor="end" dy="0.35em" id="img-a6536837-369" visibility="hidden" gadfly:scale="10.0">0.130</text>
    <text x="17.5" y="-42.3" text-anchor="end" dy="0.35em" id="img-a6536837-370" visibility="hidden" gadfly:scale="10.0">0.135</text>
    <text x="17.5" y="-46.79" text-anchor="end" dy="0.35em" id="img-a6536837-371" visibility="hidden" gadfly:scale="10.0">0.140</text>
    <text x="17.5" y="-51.27" text-anchor="end" dy="0.35em" id="img-a6536837-372" visibility="hidden" gadfly:scale="10.0">0.145</text>
    <text x="17.5" y="-55.75" text-anchor="end" dy="0.35em" id="img-a6536837-373" visibility="hidden" gadfly:scale="10.0">0.150</text>
    <text x="17.5" y="-60.23" text-anchor="end" dy="0.35em" id="img-a6536837-374" visibility="hidden" gadfly:scale="10.0">0.155</text>
    <text x="17.5" y="-64.72" text-anchor="end" dy="0.35em" id="img-a6536837-375" visibility="hidden" gadfly:scale="10.0">0.160</text>
    <text x="17.5" y="-69.2" text-anchor="end" dy="0.35em" id="img-a6536837-376" visibility="hidden" gadfly:scale="10.0">0.165</text>
    <text x="17.5" y="168.36" text-anchor="end" dy="0.35em" id="img-a6536837-377" visibility="hidden" gadfly:scale="0.5">-0.1</text>
    <text x="17.5" y="78.72" text-anchor="end" dy="0.35em" id="img-a6536837-378" visibility="hidden" gadfly:scale="0.5">0.0</text>
    <text x="17.5" y="-10.93" text-anchor="end" dy="0.35em" id="img-a6536837-379" visibility="hidden" gadfly:scale="0.5">0.1</text>
    <text x="17.5" y="-100.57" text-anchor="end" dy="0.35em" id="img-a6536837-380" visibility="hidden" gadfly:scale="0.5">0.2</text>
    <text x="17.5" y="150.43" text-anchor="end" dy="0.35em" id="img-a6536837-381" visibility="hidden" gadfly:scale="5.0">-0.080</text>
    <text x="17.5" y="145.95" text-anchor="end" dy="0.35em" id="img-a6536837-382" visibility="hidden" gadfly:scale="5.0">-0.075</text>
    <text x="17.5" y="141.47" text-anchor="end" dy="0.35em" id="img-a6536837-383" visibility="hidden" gadfly:scale="5.0">-0.070</text>
    <text x="17.5" y="136.98" text-anchor="end" dy="0.35em" id="img-a6536837-384" visibility="hidden" gadfly:scale="5.0">-0.065</text>
    <text x="17.5" y="132.5" text-anchor="end" dy="0.35em" id="img-a6536837-385" visibility="hidden" gadfly:scale="5.0">-0.060</text>
    <text x="17.5" y="128.02" text-anchor="end" dy="0.35em" id="img-a6536837-386" visibility="hidden" gadfly:scale="5.0">-0.055</text>
    <text x="17.5" y="123.54" text-anchor="end" dy="0.35em" id="img-a6536837-387" visibility="hidden" gadfly:scale="5.0">-0.050</text>
    <text x="17.5" y="119.05" text-anchor="end" dy="0.35em" id="img-a6536837-388" visibility="hidden" gadfly:scale="5.0">-0.045</text>
    <text x="17.5" y="114.57" text-anchor="end" dy="0.35em" id="img-a6536837-389" visibility="hidden" gadfly:scale="5.0">-0.040</text>
    <text x="17.5" y="110.09" text-anchor="end" dy="0.35em" id="img-a6536837-390" visibility="hidden" gadfly:scale="5.0">-0.035</text>
    <text x="17.5" y="105.61" text-anchor="end" dy="0.35em" id="img-a6536837-391" visibility="hidden" gadfly:scale="5.0">-0.030</text>
    <text x="17.5" y="101.13" text-anchor="end" dy="0.35em" id="img-a6536837-392" visibility="hidden" gadfly:scale="5.0">-0.025</text>
    <text x="17.5" y="96.64" text-anchor="end" dy="0.35em" id="img-a6536837-393" visibility="hidden" gadfly:scale="5.0">-0.020</text>
    <text x="17.5" y="92.16" text-anchor="end" dy="0.35em" id="img-a6536837-394" visibility="hidden" gadfly:scale="5.0">-0.015</text>
    <text x="17.5" y="87.68" text-anchor="end" dy="0.35em" id="img-a6536837-395" visibility="hidden" gadfly:scale="5.0">-0.010</text>
    <text x="17.5" y="83.2" text-anchor="end" dy="0.35em" id="img-a6536837-396" visibility="hidden" gadfly:scale="5.0">-0.005</text>
    <text x="17.5" y="78.72" text-anchor="end" dy="0.35em" id="img-a6536837-397" visibility="hidden" gadfly:scale="5.0">0.000</text>
    <text x="17.5" y="74.23" text-anchor="end" dy="0.35em" id="img-a6536837-398" visibility="hidden" gadfly:scale="5.0">0.005</text>
    <text x="17.5" y="69.75" text-anchor="end" dy="0.35em" id="img-a6536837-399" visibility="hidden" gadfly:scale="5.0">0.010</text>
    <text x="17.5" y="65.27" text-anchor="end" dy="0.35em" id="img-a6536837-400" visibility="hidden" gadfly:scale="5.0">0.015</text>
    <text x="17.5" y="60.79" text-anchor="end" dy="0.35em" id="img-a6536837-401" visibility="hidden" gadfly:scale="5.0">0.020</text>
    <text x="17.5" y="56.3" text-anchor="end" dy="0.35em" id="img-a6536837-402" visibility="hidden" gadfly:scale="5.0">0.025</text>
    <text x="17.5" y="51.82" text-anchor="end" dy="0.35em" id="img-a6536837-403" visibility="hidden" gadfly:scale="5.0">0.030</text>
    <text x="17.5" y="47.34" text-anchor="end" dy="0.35em" id="img-a6536837-404" visibility="hidden" gadfly:scale="5.0">0.035</text>
    <text x="17.5" y="42.86" text-anchor="end" dy="0.35em" id="img-a6536837-405" visibility="hidden" gadfly:scale="5.0">0.040</text>
    <text x="17.5" y="38.38" text-anchor="end" dy="0.35em" id="img-a6536837-406" visibility="hidden" gadfly:scale="5.0">0.045</text>
    <text x="17.5" y="33.89" text-anchor="end" dy="0.35em" id="img-a6536837-407" visibility="hidden" gadfly:scale="5.0">0.050</text>
    <text x="17.5" y="29.41" text-anchor="end" dy="0.35em" id="img-a6536837-408" visibility="hidden" gadfly:scale="5.0">0.055</text>
    <text x="17.5" y="24.93" text-anchor="end" dy="0.35em" id="img-a6536837-409" visibility="hidden" gadfly:scale="5.0">0.060</text>
    <text x="17.5" y="20.45" text-anchor="end" dy="0.35em" id="img-a6536837-410" visibility="hidden" gadfly:scale="5.0">0.065</text>
    <text x="17.5" y="15.96" text-anchor="end" dy="0.35em" id="img-a6536837-411" visibility="hidden" gadfly:scale="5.0">0.070</text>
    <text x="17.5" y="11.48" text-anchor="end" dy="0.35em" id="img-a6536837-412" visibility="hidden" gadfly:scale="5.0">0.075</text>
    <text x="17.5" y="7" text-anchor="end" dy="0.35em" id="img-a6536837-413" visibility="hidden" gadfly:scale="5.0">0.080</text>
    <text x="17.5" y="2.52" text-anchor="end" dy="0.35em" id="img-a6536837-414" visibility="hidden" gadfly:scale="5.0">0.085</text>
    <text x="17.5" y="-1.96" text-anchor="end" dy="0.35em" id="img-a6536837-415" visibility="hidden" gadfly:scale="5.0">0.090</text>
    <text x="17.5" y="-6.45" text-anchor="end" dy="0.35em" id="img-a6536837-416" visibility="hidden" gadfly:scale="5.0">0.095</text>
    <text x="17.5" y="-10.93" text-anchor="end" dy="0.35em" id="img-a6536837-417" visibility="hidden" gadfly:scale="5.0">0.100</text>
    <text x="17.5" y="-15.41" text-anchor="end" dy="0.35em" id="img-a6536837-418" visibility="hidden" gadfly:scale="5.0">0.105</text>
    <text x="17.5" y="-19.89" text-anchor="end" dy="0.35em" id="img-a6536837-419" visibility="hidden" gadfly:scale="5.0">0.110</text>
    <text x="17.5" y="-24.38" text-anchor="end" dy="0.35em" id="img-a6536837-420" visibility="hidden" gadfly:scale="5.0">0.115</text>
    <text x="17.5" y="-28.86" text-anchor="end" dy="0.35em" id="img-a6536837-421" visibility="hidden" gadfly:scale="5.0">0.120</text>
    <text x="17.5" y="-33.34" text-anchor="end" dy="0.35em" id="img-a6536837-422" visibility="hidden" gadfly:scale="5.0">0.125</text>
    <text x="17.5" y="-37.82" text-anchor="end" dy="0.35em" id="img-a6536837-423" visibility="hidden" gadfly:scale="5.0">0.130</text>
    <text x="17.5" y="-42.3" text-anchor="end" dy="0.35em" id="img-a6536837-424" visibility="hidden" gadfly:scale="5.0">0.135</text>
    <text x="17.5" y="-46.79" text-anchor="end" dy="0.35em" id="img-a6536837-425" visibility="hidden" gadfly:scale="5.0">0.140</text>
    <text x="17.5" y="-51.27" text-anchor="end" dy="0.35em" id="img-a6536837-426" visibility="hidden" gadfly:scale="5.0">0.145</text>
    <text x="17.5" y="-55.75" text-anchor="end" dy="0.35em" id="img-a6536837-427" visibility="hidden" gadfly:scale="5.0">0.150</text>
    <text x="17.5" y="-60.23" text-anchor="end" dy="0.35em" id="img-a6536837-428" visibility="hidden" gadfly:scale="5.0">0.155</text>
    <text x="17.5" y="-64.72" text-anchor="end" dy="0.35em" id="img-a6536837-429" visibility="hidden" gadfly:scale="5.0">0.160</text>
    <text x="17.5" y="-69.2" text-anchor="end" dy="0.35em" id="img-a6536837-430" visibility="hidden" gadfly:scale="5.0">0.165</text>
  </g>
  <g font-size="3.88" font-family="'PT Sans','Helvetica Neue','Helvetica',sans-serif" fill="#564A55" stroke="#000000" stroke-opacity="0.000" id="img-a6536837-431">
    <text x="8.81" y="42.86" text-anchor="end" dy="0.35em" id="img-a6536837-432">y</text>
  </g>
</g>
<defs>
  <clipPath id="img-a6536837-13">
  <path d="M18.5,5 L 116.09 5 116.09 80.72 18.5 80.72" />
</clipPath>
</defs>
<script> <![CDATA[
(function(N){var k=/[\.\/]/,L=/\s*,\s*/,C=function(a,d){return a-d},a,v,y={n:{}},M=function(){for(var a=0,d=this.length;a<d;a++)if("undefined"!=typeof this[a])return this[a]},A=function(){for(var a=this.length;--a;)if("undefined"!=typeof this[a])return this[a]},w=function(k,d){k=String(k);var f=v,n=Array.prototype.slice.call(arguments,2),u=w.listeners(k),p=0,b,q=[],e={},l=[],r=a;l.firstDefined=M;l.lastDefined=A;a=k;for(var s=v=0,x=u.length;s<x;s++)"zIndex"in u[s]&&(q.push(u[s].zIndex),0>u[s].zIndex&&
(e[u[s].zIndex]=u[s]));for(q.sort(C);0>q[p];)if(b=e[q[p++] ],l.push(b.apply(d,n)),v)return v=f,l;for(s=0;s<x;s++)if(b=u[s],"zIndex"in b)if(b.zIndex==q[p]){l.push(b.apply(d,n));if(v)break;do if(p++,(b=e[q[p] ])&&l.push(b.apply(d,n)),v)break;while(b)}else e[b.zIndex]=b;else if(l.push(b.apply(d,n)),v)break;v=f;a=r;return l};w._events=y;w.listeners=function(a){a=a.split(k);var d=y,f,n,u,p,b,q,e,l=[d],r=[];u=0;for(p=a.length;u<p;u++){e=[];b=0;for(q=l.length;b<q;b++)for(d=l[b].n,f=[d[a[u] ],d["*"] ],n=2;n--;)if(d=
f[n])e.push(d),r=r.concat(d.f||[]);l=e}return r};w.on=function(a,d){a=String(a);if("function"!=typeof d)return function(){};for(var f=a.split(L),n=0,u=f.length;n<u;n++)(function(a){a=a.split(k);for(var b=y,f,e=0,l=a.length;e<l;e++)b=b.n,b=b.hasOwnProperty(a[e])&&b[a[e] ]||(b[a[e] ]={n:{}});b.f=b.f||[];e=0;for(l=b.f.length;e<l;e++)if(b.f[e]==d){f=!0;break}!f&&b.f.push(d)})(f[n]);return function(a){+a==+a&&(d.zIndex=+a)}};w.f=function(a){var d=[].slice.call(arguments,1);return function(){w.apply(null,
[a,null].concat(d).concat([].slice.call(arguments,0)))}};w.stop=function(){v=1};w.nt=function(k){return k?(new RegExp("(?:\\.|\\/|^)"+k+"(?:\\.|\\/|$)")).test(a):a};w.nts=function(){return a.split(k)};w.off=w.unbind=function(a,d){if(a){var f=a.split(L);if(1<f.length)for(var n=0,u=f.length;n<u;n++)w.off(f[n],d);else{for(var f=a.split(k),p,b,q,e,l=[y],n=0,u=f.length;n<u;n++)for(e=0;e<l.length;e+=q.length-2){q=[e,1];p=l[e].n;if("*"!=f[n])p[f[n] ]&&q.push(p[f[n] ]);else for(b in p)p.hasOwnProperty(b)&&
q.push(p[b]);l.splice.apply(l,q)}n=0;for(u=l.length;n<u;n++)for(p=l[n];p.n;){if(d){if(p.f){e=0;for(f=p.f.length;e<f;e++)if(p.f[e]==d){p.f.splice(e,1);break}!p.f.length&&delete p.f}for(b in p.n)if(p.n.hasOwnProperty(b)&&p.n[b].f){q=p.n[b].f;e=0;for(f=q.length;e<f;e++)if(q[e]==d){q.splice(e,1);break}!q.length&&delete p.n[b].f}}else for(b in delete p.f,p.n)p.n.hasOwnProperty(b)&&p.n[b].f&&delete p.n[b].f;p=p.n}}}else w._events=y={n:{}}};w.once=function(a,d){var f=function(){w.unbind(a,f);return d.apply(this,
arguments)};return w.on(a,f)};w.version="0.4.2";w.toString=function(){return"You are running Eve 0.4.2"};"undefined"!=typeof module&&module.exports?module.exports=w:"function"===typeof define&&define.amd?define("eve",[],function(){return w}):N.eve=w})(this);
(function(N,k){"function"===typeof define&&define.amd?define("Snap.svg",["eve"],function(L){return k(N,L)}):k(N,N.eve)})(this,function(N,k){var L=function(a){var k={},y=N.requestAnimationFrame||N.webkitRequestAnimationFrame||N.mozRequestAnimationFrame||N.oRequestAnimationFrame||N.msRequestAnimationFrame||function(a){setTimeout(a,16)},M=Array.isArray||function(a){return a instanceof Array||"[object Array]"==Object.prototype.toString.call(a)},A=0,w="M"+(+new Date).toString(36),z=function(a){if(null==
a)return this.s;var b=this.s-a;this.b+=this.dur*b;this.B+=this.dur*b;this.s=a},d=function(a){if(null==a)return this.spd;this.spd=a},f=function(a){if(null==a)return this.dur;this.s=this.s*a/this.dur;this.dur=a},n=function(){delete k[this.id];this.update();a("mina.stop."+this.id,this)},u=function(){this.pdif||(delete k[this.id],this.update(),this.pdif=this.get()-this.b)},p=function(){this.pdif&&(this.b=this.get()-this.pdif,delete this.pdif,k[this.id]=this)},b=function(){var a;if(M(this.start)){a=[];
for(var b=0,e=this.start.length;b<e;b++)a[b]=+this.start[b]+(this.end[b]-this.start[b])*this.easing(this.s)}else a=+this.start+(this.end-this.start)*this.easing(this.s);this.set(a)},q=function(){var l=0,b;for(b in k)if(k.hasOwnProperty(b)){var e=k[b],f=e.get();l++;e.s=(f-e.b)/(e.dur/e.spd);1<=e.s&&(delete k[b],e.s=1,l--,function(b){setTimeout(function(){a("mina.finish."+b.id,b)})}(e));e.update()}l&&y(q)},e=function(a,r,s,x,G,h,J){a={id:w+(A++).toString(36),start:a,end:r,b:s,s:0,dur:x-s,spd:1,get:G,
set:h,easing:J||e.linear,status:z,speed:d,duration:f,stop:n,pause:u,resume:p,update:b};k[a.id]=a;r=0;for(var K in k)if(k.hasOwnProperty(K)&&(r++,2==r))break;1==r&&y(q);return a};e.time=Date.now||function(){return+new Date};e.getById=function(a){return k[a]||null};e.linear=function(a){return a};e.easeout=function(a){return Math.pow(a,1.7)};e.easein=function(a){return Math.pow(a,0.48)};e.easeinout=function(a){if(1==a)return 1;if(0==a)return 0;var b=0.48-a/1.04,e=Math.sqrt(0.1734+b*b);a=e-b;a=Math.pow(Math.abs(a),
1/3)*(0>a?-1:1);b=-e-b;b=Math.pow(Math.abs(b),1/3)*(0>b?-1:1);a=a+b+0.5;return 3*(1-a)*a*a+a*a*a};e.backin=function(a){return 1==a?1:a*a*(2.70158*a-1.70158)};e.backout=function(a){if(0==a)return 0;a-=1;return a*a*(2.70158*a+1.70158)+1};e.elastic=function(a){return a==!!a?a:Math.pow(2,-10*a)*Math.sin(2*(a-0.075)*Math.PI/0.3)+1};e.bounce=function(a){a<1/2.75?a*=7.5625*a:a<2/2.75?(a-=1.5/2.75,a=7.5625*a*a+0.75):a<2.5/2.75?(a-=2.25/2.75,a=7.5625*a*a+0.9375):(a-=2.625/2.75,a=7.5625*a*a+0.984375);return a};
return N.mina=e}("undefined"==typeof k?function(){}:k),C=function(){function a(c,t){if(c){if(c.tagName)return x(c);if(y(c,"array")&&a.set)return a.set.apply(a,c);if(c instanceof e)return c;if(null==t)return c=G.doc.querySelector(c),x(c)}return new s(null==c?"100%":c,null==t?"100%":t)}function v(c,a){if(a){"#text"==c&&(c=G.doc.createTextNode(a.text||""));"string"==typeof c&&(c=v(c));if("string"==typeof a)return"xlink:"==a.substring(0,6)?c.getAttributeNS(m,a.substring(6)):"xml:"==a.substring(0,4)?c.getAttributeNS(la,
a.substring(4)):c.getAttribute(a);for(var da in a)if(a[h](da)){var b=J(a[da]);b?"xlink:"==da.substring(0,6)?c.setAttributeNS(m,da.substring(6),b):"xml:"==da.substring(0,4)?c.setAttributeNS(la,da.substring(4),b):c.setAttribute(da,b):c.removeAttribute(da)}}else c=G.doc.createElementNS(la,c);return c}function y(c,a){a=J.prototype.toLowerCase.call(a);return"finite"==a?isFinite(c):"array"==a&&(c instanceof Array||Array.isArray&&Array.isArray(c))?!0:"null"==a&&null===c||a==typeof c&&null!==c||"object"==
a&&c===Object(c)||$.call(c).slice(8,-1).toLowerCase()==a}function M(c){if("function"==typeof c||Object(c)!==c)return c;var a=new c.constructor,b;for(b in c)c[h](b)&&(a[b]=M(c[b]));return a}function A(c,a,b){function m(){var e=Array.prototype.slice.call(arguments,0),f=e.join("\u2400"),d=m.cache=m.cache||{},l=m.count=m.count||[];if(d[h](f)){a:for(var e=l,l=f,B=0,H=e.length;B<H;B++)if(e[B]===l){e.push(e.splice(B,1)[0]);break a}return b?b(d[f]):d[f]}1E3<=l.length&&delete d[l.shift()];l.push(f);d[f]=c.apply(a,
e);return b?b(d[f]):d[f]}return m}function w(c,a,b,m,e,f){return null==e?(c-=b,a-=m,c||a?(180*I.atan2(-a,-c)/C+540)%360:0):w(c,a,e,f)-w(b,m,e,f)}function z(c){return c%360*C/180}function d(c){var a=[];c=c.replace(/(?:^|\s)(\w+)\(([^)]+)\)/g,function(c,b,m){m=m.split(/\s*,\s*|\s+/);"rotate"==b&&1==m.length&&m.push(0,0);"scale"==b&&(2<m.length?m=m.slice(0,2):2==m.length&&m.push(0,0),1==m.length&&m.push(m[0],0,0));"skewX"==b?a.push(["m",1,0,I.tan(z(m[0])),1,0,0]):"skewY"==b?a.push(["m",1,I.tan(z(m[0])),
0,1,0,0]):a.push([b.charAt(0)].concat(m));return c});return a}function f(c,t){var b=O(c),m=new a.Matrix;if(b)for(var e=0,f=b.length;e<f;e++){var h=b[e],d=h.length,B=J(h[0]).toLowerCase(),H=h[0]!=B,l=H?m.invert():0,E;"t"==B&&2==d?m.translate(h[1],0):"t"==B&&3==d?H?(d=l.x(0,0),B=l.y(0,0),H=l.x(h[1],h[2]),l=l.y(h[1],h[2]),m.translate(H-d,l-B)):m.translate(h[1],h[2]):"r"==B?2==d?(E=E||t,m.rotate(h[1],E.x+E.width/2,E.y+E.height/2)):4==d&&(H?(H=l.x(h[2],h[3]),l=l.y(h[2],h[3]),m.rotate(h[1],H,l)):m.rotate(h[1],
h[2],h[3])):"s"==B?2==d||3==d?(E=E||t,m.scale(h[1],h[d-1],E.x+E.width/2,E.y+E.height/2)):4==d?H?(H=l.x(h[2],h[3]),l=l.y(h[2],h[3]),m.scale(h[1],h[1],H,l)):m.scale(h[1],h[1],h[2],h[3]):5==d&&(H?(H=l.x(h[3],h[4]),l=l.y(h[3],h[4]),m.scale(h[1],h[2],H,l)):m.scale(h[1],h[2],h[3],h[4])):"m"==B&&7==d&&m.add(h[1],h[2],h[3],h[4],h[5],h[6])}return m}function n(c,t){if(null==t){var m=!0;t="linearGradient"==c.type||"radialGradient"==c.type?c.node.getAttribute("gradientTransform"):"pattern"==c.type?c.node.getAttribute("patternTransform"):
c.node.getAttribute("transform");if(!t)return new a.Matrix;t=d(t)}else t=a._.rgTransform.test(t)?J(t).replace(/\.{3}|\u2026/g,c._.transform||aa):d(t),y(t,"array")&&(t=a.path?a.path.toString.call(t):J(t)),c._.transform=t;var b=f(t,c.getBBox(1));if(m)return b;c.matrix=b}function u(c){c=c.node.ownerSVGElement&&x(c.node.ownerSVGElement)||c.node.parentNode&&x(c.node.parentNode)||a.select("svg")||a(0,0);var t=c.select("defs"),t=null==t?!1:t.node;t||(t=r("defs",c.node).node);return t}function p(c){return c.node.ownerSVGElement&&
x(c.node.ownerSVGElement)||a.select("svg")}function b(c,a,m){function b(c){if(null==c)return aa;if(c==+c)return c;v(B,{width:c});try{return B.getBBox().width}catch(a){return 0}}function h(c){if(null==c)return aa;if(c==+c)return c;v(B,{height:c});try{return B.getBBox().height}catch(a){return 0}}function e(b,B){null==a?d[b]=B(c.attr(b)||0):b==a&&(d=B(null==m?c.attr(b)||0:m))}var f=p(c).node,d={},B=f.querySelector(".svg---mgr");B||(B=v("rect"),v(B,{x:-9E9,y:-9E9,width:10,height:10,"class":"svg---mgr",
fill:"none"}),f.appendChild(B));switch(c.type){case "rect":e("rx",b),e("ry",h);case "image":e("width",b),e("height",h);case "text":e("x",b);e("y",h);break;case "circle":e("cx",b);e("cy",h);e("r",b);break;case "ellipse":e("cx",b);e("cy",h);e("rx",b);e("ry",h);break;case "line":e("x1",b);e("x2",b);e("y1",h);e("y2",h);break;case "marker":e("refX",b);e("markerWidth",b);e("refY",h);e("markerHeight",h);break;case "radialGradient":e("fx",b);e("fy",h);break;case "tspan":e("dx",b);e("dy",h);break;default:e(a,
b)}f.removeChild(B);return d}function q(c){y(c,"array")||(c=Array.prototype.slice.call(arguments,0));for(var a=0,b=0,m=this.node;this[a];)delete this[a++];for(a=0;a<c.length;a++)"set"==c[a].type?c[a].forEach(function(c){m.appendChild(c.node)}):m.appendChild(c[a].node);for(var h=m.childNodes,a=0;a<h.length;a++)this[b++]=x(h[a]);return this}function e(c){if(c.snap in E)return E[c.snap];var a=this.id=V(),b;try{b=c.ownerSVGElement}catch(m){}this.node=c;b&&(this.paper=new s(b));this.type=c.tagName;this.anims=
{};this._={transform:[]};c.snap=a;E[a]=this;"g"==this.type&&(this.add=q);if(this.type in{g:1,mask:1,pattern:1})for(var e in s.prototype)s.prototype[h](e)&&(this[e]=s.prototype[e])}function l(c){this.node=c}function r(c,a){var b=v(c);a.appendChild(b);return x(b)}function s(c,a){var b,m,f,d=s.prototype;if(c&&"svg"==c.tagName){if(c.snap in E)return E[c.snap];var l=c.ownerDocument;b=new e(c);m=c.getElementsByTagName("desc")[0];f=c.getElementsByTagName("defs")[0];m||(m=v("desc"),m.appendChild(l.createTextNode("Created with Snap")),
b.node.appendChild(m));f||(f=v("defs"),b.node.appendChild(f));b.defs=f;for(var ca in d)d[h](ca)&&(b[ca]=d[ca]);b.paper=b.root=b}else b=r("svg",G.doc.body),v(b.node,{height:a,version:1.1,width:c,xmlns:la});return b}function x(c){return!c||c instanceof e||c instanceof l?c:c.tagName&&"svg"==c.tagName.toLowerCase()?new s(c):c.tagName&&"object"==c.tagName.toLowerCase()&&"image/svg+xml"==c.type?new s(c.contentDocument.getElementsByTagName("svg")[0]):new e(c)}a.version="0.3.0";a.toString=function(){return"Snap v"+
this.version};a._={};var G={win:N,doc:N.document};a._.glob=G;var h="hasOwnProperty",J=String,K=parseFloat,U=parseInt,I=Math,P=I.max,Q=I.min,Y=I.abs,C=I.PI,aa="",$=Object.prototype.toString,F=/^\s*((#[a-f\d]{6})|(#[a-f\d]{3})|rgba?\(\s*([\d\.]+%?\s*,\s*[\d\.]+%?\s*,\s*[\d\.]+%?(?:\s*,\s*[\d\.]+%?)?)\s*\)|hsba?\(\s*([\d\.]+(?:deg|\xb0|%)?\s*,\s*[\d\.]+%?\s*,\s*[\d\.]+(?:%?\s*,\s*[\d\.]+)?%?)\s*\)|hsla?\(\s*([\d\.]+(?:deg|\xb0|%)?\s*,\s*[\d\.]+%?\s*,\s*[\d\.]+(?:%?\s*,\s*[\d\.]+)?%?)\s*\))\s*$/i;a._.separator=
RegExp("[,\t\n\x0B\f\r \u00a0\u1680\u180e\u2000\u2001\u2002\u2003\u2004\u2005\u2006\u2007\u2008\u2009\u200a\u202f\u205f\u3000\u2028\u2029]+");var S=RegExp("[\t\n\x0B\f\r \u00a0\u1680\u180e\u2000\u2001\u2002\u2003\u2004\u2005\u2006\u2007\u2008\u2009\u200a\u202f\u205f\u3000\u2028\u2029]*,[\t\n\x0B\f\r \u00a0\u1680\u180e\u2000\u2001\u2002\u2003\u2004\u2005\u2006\u2007\u2008\u2009\u200a\u202f\u205f\u3000\u2028\u2029]*"),X={hs:1,rg:1},W=RegExp("([a-z])[\t\n\x0B\f\r \u00a0\u1680\u180e\u2000\u2001\u2002\u2003\u2004\u2005\u2006\u2007\u2008\u2009\u200a\u202f\u205f\u3000\u2028\u2029,]*((-?\\d*\\.?\\d*(?:e[\\-+]?\\d+)?[\t\n\x0B\f\r \u00a0\u1680\u180e\u2000\u2001\u2002\u2003\u2004\u2005\u2006\u2007\u2008\u2009\u200a\u202f\u205f\u3000\u2028\u2029]*,?[\t\n\x0B\f\r \u00a0\u1680\u180e\u2000\u2001\u2002\u2003\u2004\u2005\u2006\u2007\u2008\u2009\u200a\u202f\u205f\u3000\u2028\u2029]*)+)",
"ig"),ma=RegExp("([rstm])[\t\n\x0B\f\r \u00a0\u1680\u180e\u2000\u2001\u2002\u2003\u2004\u2005\u2006\u2007\u2008\u2009\u200a\u202f\u205f\u3000\u2028\u2029,]*((-?\\d*\\.?\\d*(?:e[\\-+]?\\d+)?[\t\n\x0B\f\r \u00a0\u1680\u180e\u2000\u2001\u2002\u2003\u2004\u2005\u2006\u2007\u2008\u2009\u200a\u202f\u205f\u3000\u2028\u2029]*,?[\t\n\x0B\f\r \u00a0\u1680\u180e\u2000\u2001\u2002\u2003\u2004\u2005\u2006\u2007\u2008\u2009\u200a\u202f\u205f\u3000\u2028\u2029]*)+)","ig"),Z=RegExp("(-?\\d*\\.?\\d*(?:e[\\-+]?\\d+)?)[\t\n\x0B\f\r \u00a0\u1680\u180e\u2000\u2001\u2002\u2003\u2004\u2005\u2006\u2007\u2008\u2009\u200a\u202f\u205f\u3000\u2028\u2029]*,?[\t\n\x0B\f\r \u00a0\u1680\u180e\u2000\u2001\u2002\u2003\u2004\u2005\u2006\u2007\u2008\u2009\u200a\u202f\u205f\u3000\u2028\u2029]*",
"ig"),na=0,ba="S"+(+new Date).toString(36),V=function(){return ba+(na++).toString(36)},m="http://www.w3.org/1999/xlink",la="http://www.w3.org/2000/svg",E={},ca=a.url=function(c){return"url('#"+c+"')"};a._.$=v;a._.id=V;a.format=function(){var c=/\{([^\}]+)\}/g,a=/(?:(?:^|\.)(.+?)(?=\[|\.|$|\()|\[('|")(.+?)\2\])(\(\))?/g,b=function(c,b,m){var h=m;b.replace(a,function(c,a,b,m,t){a=a||m;h&&(a in h&&(h=h[a]),"function"==typeof h&&t&&(h=h()))});return h=(null==h||h==m?c:h)+""};return function(a,m){return J(a).replace(c,
function(c,a){return b(c,a,m)})}}();a._.clone=M;a._.cacher=A;a.rad=z;a.deg=function(c){return 180*c/C%360};a.angle=w;a.is=y;a.snapTo=function(c,a,b){b=y(b,"finite")?b:10;if(y(c,"array"))for(var m=c.length;m--;){if(Y(c[m]-a)<=b)return c[m]}else{c=+c;m=a%c;if(m<b)return a-m;if(m>c-b)return a-m+c}return a};a.getRGB=A(function(c){if(!c||(c=J(c)).indexOf("-")+1)return{r:-1,g:-1,b:-1,hex:"none",error:1,toString:ka};if("none"==c)return{r:-1,g:-1,b:-1,hex:"none",toString:ka};!X[h](c.toLowerCase().substring(0,
2))&&"#"!=c.charAt()&&(c=T(c));if(!c)return{r:-1,g:-1,b:-1,hex:"none",error:1,toString:ka};var b,m,e,f,d;if(c=c.match(F)){c[2]&&(e=U(c[2].substring(5),16),m=U(c[2].substring(3,5),16),b=U(c[2].substring(1,3),16));c[3]&&(e=U((d=c[3].charAt(3))+d,16),m=U((d=c[3].charAt(2))+d,16),b=U((d=c[3].charAt(1))+d,16));c[4]&&(d=c[4].split(S),b=K(d[0]),"%"==d[0].slice(-1)&&(b*=2.55),m=K(d[1]),"%"==d[1].slice(-1)&&(m*=2.55),e=K(d[2]),"%"==d[2].slice(-1)&&(e*=2.55),"rgba"==c[1].toLowerCase().slice(0,4)&&(f=K(d[3])),
d[3]&&"%"==d[3].slice(-1)&&(f/=100));if(c[5])return d=c[5].split(S),b=K(d[0]),"%"==d[0].slice(-1)&&(b/=100),m=K(d[1]),"%"==d[1].slice(-1)&&(m/=100),e=K(d[2]),"%"==d[2].slice(-1)&&(e/=100),"deg"!=d[0].slice(-3)&&"\u00b0"!=d[0].slice(-1)||(b/=360),"hsba"==c[1].toLowerCase().slice(0,4)&&(f=K(d[3])),d[3]&&"%"==d[3].slice(-1)&&(f/=100),a.hsb2rgb(b,m,e,f);if(c[6])return d=c[6].split(S),b=K(d[0]),"%"==d[0].slice(-1)&&(b/=100),m=K(d[1]),"%"==d[1].slice(-1)&&(m/=100),e=K(d[2]),"%"==d[2].slice(-1)&&(e/=100),
"deg"!=d[0].slice(-3)&&"\u00b0"!=d[0].slice(-1)||(b/=360),"hsla"==c[1].toLowerCase().slice(0,4)&&(f=K(d[3])),d[3]&&"%"==d[3].slice(-1)&&(f/=100),a.hsl2rgb(b,m,e,f);b=Q(I.round(b),255);m=Q(I.round(m),255);e=Q(I.round(e),255);f=Q(P(f,0),1);c={r:b,g:m,b:e,toString:ka};c.hex="#"+(16777216|e|m<<8|b<<16).toString(16).slice(1);c.opacity=y(f,"finite")?f:1;return c}return{r:-1,g:-1,b:-1,hex:"none",error:1,toString:ka}},a);a.hsb=A(function(c,b,m){return a.hsb2rgb(c,b,m).hex});a.hsl=A(function(c,b,m){return a.hsl2rgb(c,
b,m).hex});a.rgb=A(function(c,a,b,m){if(y(m,"finite")){var e=I.round;return"rgba("+[e(c),e(a),e(b),+m.toFixed(2)]+")"}return"#"+(16777216|b|a<<8|c<<16).toString(16).slice(1)});var T=function(c){var a=G.doc.getElementsByTagName("head")[0]||G.doc.getElementsByTagName("svg")[0];T=A(function(c){if("red"==c.toLowerCase())return"rgb(255, 0, 0)";a.style.color="rgb(255, 0, 0)";a.style.color=c;c=G.doc.defaultView.getComputedStyle(a,aa).getPropertyValue("color");return"rgb(255, 0, 0)"==c?null:c});return T(c)},
qa=function(){return"hsb("+[this.h,this.s,this.b]+")"},ra=function(){return"hsl("+[this.h,this.s,this.l]+")"},ka=function(){return 1==this.opacity||null==this.opacity?this.hex:"rgba("+[this.r,this.g,this.b,this.opacity]+")"},D=function(c,b,m){null==b&&y(c,"object")&&"r"in c&&"g"in c&&"b"in c&&(m=c.b,b=c.g,c=c.r);null==b&&y(c,string)&&(m=a.getRGB(c),c=m.r,b=m.g,m=m.b);if(1<c||1<b||1<m)c/=255,b/=255,m/=255;return[c,b,m]},oa=function(c,b,m,e){c=I.round(255*c);b=I.round(255*b);m=I.round(255*m);c={r:c,
g:b,b:m,opacity:y(e,"finite")?e:1,hex:a.rgb(c,b,m),toString:ka};y(e,"finite")&&(c.opacity=e);return c};a.color=function(c){var b;y(c,"object")&&"h"in c&&"s"in c&&"b"in c?(b=a.hsb2rgb(c),c.r=b.r,c.g=b.g,c.b=b.b,c.opacity=1,c.hex=b.hex):y(c,"object")&&"h"in c&&"s"in c&&"l"in c?(b=a.hsl2rgb(c),c.r=b.r,c.g=b.g,c.b=b.b,c.opacity=1,c.hex=b.hex):(y(c,"string")&&(c=a.getRGB(c)),y(c,"object")&&"r"in c&&"g"in c&&"b"in c&&!("error"in c)?(b=a.rgb2hsl(c),c.h=b.h,c.s=b.s,c.l=b.l,b=a.rgb2hsb(c),c.v=b.b):(c={hex:"none"},
c.r=c.g=c.b=c.h=c.s=c.v=c.l=-1,c.error=1));c.toString=ka;return c};a.hsb2rgb=function(c,a,b,m){y(c,"object")&&"h"in c&&"s"in c&&"b"in c&&(b=c.b,a=c.s,c=c.h,m=c.o);var e,h,d;c=360*c%360/60;d=b*a;a=d*(1-Y(c%2-1));b=e=h=b-d;c=~~c;b+=[d,a,0,0,a,d][c];e+=[a,d,d,a,0,0][c];h+=[0,0,a,d,d,a][c];return oa(b,e,h,m)};a.hsl2rgb=function(c,a,b,m){y(c,"object")&&"h"in c&&"s"in c&&"l"in c&&(b=c.l,a=c.s,c=c.h);if(1<c||1<a||1<b)c/=360,a/=100,b/=100;var e,h,d;c=360*c%360/60;d=2*a*(0.5>b?b:1-b);a=d*(1-Y(c%2-1));b=e=
h=b-d/2;c=~~c;b+=[d,a,0,0,a,d][c];e+=[a,d,d,a,0,0][c];h+=[0,0,a,d,d,a][c];return oa(b,e,h,m)};a.rgb2hsb=function(c,a,b){b=D(c,a,b);c=b[0];a=b[1];b=b[2];var m,e;m=P(c,a,b);e=m-Q(c,a,b);c=((0==e?0:m==c?(a-b)/e:m==a?(b-c)/e+2:(c-a)/e+4)+360)%6*60/360;return{h:c,s:0==e?0:e/m,b:m,toString:qa}};a.rgb2hsl=function(c,a,b){b=D(c,a,b);c=b[0];a=b[1];b=b[2];var m,e,h;m=P(c,a,b);e=Q(c,a,b);h=m-e;c=((0==h?0:m==c?(a-b)/h:m==a?(b-c)/h+2:(c-a)/h+4)+360)%6*60/360;m=(m+e)/2;return{h:c,s:0==h?0:0.5>m?h/(2*m):h/(2-2*
m),l:m,toString:ra}};a.parsePathString=function(c){if(!c)return null;var b=a.path(c);if(b.arr)return a.path.clone(b.arr);var m={a:7,c:6,o:2,h:1,l:2,m:2,r:4,q:4,s:4,t:2,v:1,u:3,z:0},e=[];y(c,"array")&&y(c[0],"array")&&(e=a.path.clone(c));e.length||J(c).replace(W,function(c,a,b){var h=[];c=a.toLowerCase();b.replace(Z,function(c,a){a&&h.push(+a)});"m"==c&&2<h.length&&(e.push([a].concat(h.splice(0,2))),c="l",a="m"==a?"l":"L");"o"==c&&1==h.length&&e.push([a,h[0] ]);if("r"==c)e.push([a].concat(h));else for(;h.length>=
m[c]&&(e.push([a].concat(h.splice(0,m[c]))),m[c]););});e.toString=a.path.toString;b.arr=a.path.clone(e);return e};var O=a.parseTransformString=function(c){if(!c)return null;var b=[];y(c,"array")&&y(c[0],"array")&&(b=a.path.clone(c));b.length||J(c).replace(ma,function(c,a,m){var e=[];a.toLowerCase();m.replace(Z,function(c,a){a&&e.push(+a)});b.push([a].concat(e))});b.toString=a.path.toString;return b};a._.svgTransform2string=d;a._.rgTransform=RegExp("^[a-z][\t\n\x0B\f\r \u00a0\u1680\u180e\u2000\u2001\u2002\u2003\u2004\u2005\u2006\u2007\u2008\u2009\u200a\u202f\u205f\u3000\u2028\u2029]*-?\\.?\\d",
"i");a._.transform2matrix=f;a._unit2px=b;a._.getSomeDefs=u;a._.getSomeSVG=p;a.select=function(c){return x(G.doc.querySelector(c))};a.selectAll=function(c){c=G.doc.querySelectorAll(c);for(var b=(a.set||Array)(),m=0;m<c.length;m++)b.push(x(c[m]));return b};setInterval(function(){for(var c in E)if(E[h](c)){var a=E[c],b=a.node;("svg"!=a.type&&!b.ownerSVGElement||"svg"==a.type&&(!b.parentNode||"ownerSVGElement"in b.parentNode&&!b.ownerSVGElement))&&delete E[c]}},1E4);(function(c){function m(c){function a(c,
b){var m=v(c.node,b);(m=(m=m&&m.match(d))&&m[2])&&"#"==m.charAt()&&(m=m.substring(1))&&(f[m]=(f[m]||[]).concat(function(a){var m={};m[b]=ca(a);v(c.node,m)}))}function b(c){var a=v(c.node,"xlink:href");a&&"#"==a.charAt()&&(a=a.substring(1))&&(f[a]=(f[a]||[]).concat(function(a){c.attr("xlink:href","#"+a)}))}var e=c.selectAll("*"),h,d=/^\s*url\(("|'|)(.*)\1\)\s*$/;c=[];for(var f={},l=0,E=e.length;l<E;l++){h=e[l];a(h,"fill");a(h,"stroke");a(h,"filter");a(h,"mask");a(h,"clip-path");b(h);var t=v(h.node,
"id");t&&(v(h.node,{id:h.id}),c.push({old:t,id:h.id}))}l=0;for(E=c.length;l<E;l++)if(e=f[c[l].old])for(h=0,t=e.length;h<t;h++)e[h](c[l].id)}function e(c,a,b){return function(m){m=m.slice(c,a);1==m.length&&(m=m[0]);return b?b(m):m}}function d(c){return function(){var a=c?"<"+this.type:"",b=this.node.attributes,m=this.node.childNodes;if(c)for(var e=0,h=b.length;e<h;e++)a+=" "+b[e].name+'="'+b[e].value.replace(/"/g,'\\"')+'"';if(m.length){c&&(a+=">");e=0;for(h=m.length;e<h;e++)3==m[e].nodeType?a+=m[e].nodeValue:
1==m[e].nodeType&&(a+=x(m[e]).toString());c&&(a+="</"+this.type+">")}else c&&(a+="/>");return a}}c.attr=function(c,a){if(!c)return this;if(y(c,"string"))if(1<arguments.length){var b={};b[c]=a;c=b}else return k("snap.util.getattr."+c,this).firstDefined();for(var m in c)c[h](m)&&k("snap.util.attr."+m,this,c[m]);return this};c.getBBox=function(c){if(!a.Matrix||!a.path)return this.node.getBBox();var b=this,m=new a.Matrix;if(b.removed)return a._.box();for(;"use"==b.type;)if(c||(m=m.add(b.transform().localMatrix.translate(b.attr("x")||
0,b.attr("y")||0))),b.original)b=b.original;else var e=b.attr("xlink:href"),b=b.original=b.node.ownerDocument.getElementById(e.substring(e.indexOf("#")+1));var e=b._,h=a.path.get[b.type]||a.path.get.deflt;try{if(c)return e.bboxwt=h?a.path.getBBox(b.realPath=h(b)):a._.box(b.node.getBBox()),a._.box(e.bboxwt);b.realPath=h(b);b.matrix=b.transform().localMatrix;e.bbox=a.path.getBBox(a.path.map(b.realPath,m.add(b.matrix)));return a._.box(e.bbox)}catch(d){return a._.box()}};var f=function(){return this.string};
c.transform=function(c){var b=this._;if(null==c){var m=this;c=new a.Matrix(this.node.getCTM());for(var e=n(this),h=[e],d=new a.Matrix,l=e.toTransformString(),b=J(e)==J(this.matrix)?J(b.transform):l;"svg"!=m.type&&(m=m.parent());)h.push(n(m));for(m=h.length;m--;)d.add(h[m]);return{string:b,globalMatrix:c,totalMatrix:d,localMatrix:e,diffMatrix:c.clone().add(e.invert()),global:c.toTransformString(),total:d.toTransformString(),local:l,toString:f}}c instanceof a.Matrix?this.matrix=c:n(this,c);this.node&&
("linearGradient"==this.type||"radialGradient"==this.type?v(this.node,{gradientTransform:this.matrix}):"pattern"==this.type?v(this.node,{patternTransform:this.matrix}):v(this.node,{transform:this.matrix}));return this};c.parent=function(){return x(this.node.parentNode)};c.append=c.add=function(c){if(c){if("set"==c.type){var a=this;c.forEach(function(c){a.add(c)});return this}c=x(c);this.node.appendChild(c.node);c.paper=this.paper}return this};c.appendTo=function(c){c&&(c=x(c),c.append(this));return this};
c.prepend=function(c){if(c){if("set"==c.type){var a=this,b;c.forEach(function(c){b?b.after(c):a.prepend(c);b=c});return this}c=x(c);var m=c.parent();this.node.insertBefore(c.node,this.node.firstChild);this.add&&this.add();c.paper=this.paper;this.parent()&&this.parent().add();m&&m.add()}return this};c.prependTo=function(c){c=x(c);c.prepend(this);return this};c.before=function(c){if("set"==c.type){var a=this;c.forEach(function(c){var b=c.parent();a.node.parentNode.insertBefore(c.node,a.node);b&&b.add()});
this.parent().add();return this}c=x(c);var b=c.parent();this.node.parentNode.insertBefore(c.node,this.node);this.parent()&&this.parent().add();b&&b.add();c.paper=this.paper;return this};c.after=function(c){c=x(c);var a=c.parent();this.node.nextSibling?this.node.parentNode.insertBefore(c.node,this.node.nextSibling):this.node.parentNode.appendChild(c.node);this.parent()&&this.parent().add();a&&a.add();c.paper=this.paper;return this};c.insertBefore=function(c){c=x(c);var a=this.parent();c.node.parentNode.insertBefore(this.node,
c.node);this.paper=c.paper;a&&a.add();c.parent()&&c.parent().add();return this};c.insertAfter=function(c){c=x(c);var a=this.parent();c.node.parentNode.insertBefore(this.node,c.node.nextSibling);this.paper=c.paper;a&&a.add();c.parent()&&c.parent().add();return this};c.remove=function(){var c=this.parent();this.node.parentNode&&this.node.parentNode.removeChild(this.node);delete this.paper;this.removed=!0;c&&c.add();return this};c.select=function(c){return x(this.node.querySelector(c))};c.selectAll=
function(c){c=this.node.querySelectorAll(c);for(var b=(a.set||Array)(),m=0;m<c.length;m++)b.push(x(c[m]));return b};c.asPX=function(c,a){null==a&&(a=this.attr(c));return+b(this,c,a)};c.use=function(){var c,a=this.node.id;a||(a=this.id,v(this.node,{id:a}));c="linearGradient"==this.type||"radialGradient"==this.type||"pattern"==this.type?r(this.type,this.node.parentNode):r("use",this.node.parentNode);v(c.node,{"xlink:href":"#"+a});c.original=this;return c};var l=/\S+/g;c.addClass=function(c){var a=(c||
"").match(l)||[];c=this.node;var b=c.className.baseVal,m=b.match(l)||[],e,h,d;if(a.length){for(e=0;d=a[e++];)h=m.indexOf(d),~h||m.push(d);a=m.join(" ");b!=a&&(c.className.baseVal=a)}return this};c.removeClass=function(c){var a=(c||"").match(l)||[];c=this.node;var b=c.className.baseVal,m=b.match(l)||[],e,h;if(m.length){for(e=0;h=a[e++];)h=m.indexOf(h),~h&&m.splice(h,1);a=m.join(" ");b!=a&&(c.className.baseVal=a)}return this};c.hasClass=function(c){return!!~(this.node.className.baseVal.match(l)||[]).indexOf(c)};
c.toggleClass=function(c,a){if(null!=a)return a?this.addClass(c):this.removeClass(c);var b=(c||"").match(l)||[],m=this.node,e=m.className.baseVal,h=e.match(l)||[],d,f,E;for(d=0;E=b[d++];)f=h.indexOf(E),~f?h.splice(f,1):h.push(E);b=h.join(" ");e!=b&&(m.className.baseVal=b);return this};c.clone=function(){var c=x(this.node.cloneNode(!0));v(c.node,"id")&&v(c.node,{id:c.id});m(c);c.insertAfter(this);return c};c.toDefs=function(){u(this).appendChild(this.node);return this};c.pattern=c.toPattern=function(c,
a,b,m){var e=r("pattern",u(this));null==c&&(c=this.getBBox());y(c,"object")&&"x"in c&&(a=c.y,b=c.width,m=c.height,c=c.x);v(e.node,{x:c,y:a,width:b,height:m,patternUnits:"userSpaceOnUse",id:e.id,viewBox:[c,a,b,m].join(" ")});e.node.appendChild(this.node);return e};c.marker=function(c,a,b,m,e,h){var d=r("marker",u(this));null==c&&(c=this.getBBox());y(c,"object")&&"x"in c&&(a=c.y,b=c.width,m=c.height,e=c.refX||c.cx,h=c.refY||c.cy,c=c.x);v(d.node,{viewBox:[c,a,b,m].join(" "),markerWidth:b,markerHeight:m,
orient:"auto",refX:e||0,refY:h||0,id:d.id});d.node.appendChild(this.node);return d};var E=function(c,a,b,m){"function"!=typeof b||b.length||(m=b,b=L.linear);this.attr=c;this.dur=a;b&&(this.easing=b);m&&(this.callback=m)};a._.Animation=E;a.animation=function(c,a,b,m){return new E(c,a,b,m)};c.inAnim=function(){var c=[],a;for(a in this.anims)this.anims[h](a)&&function(a){c.push({anim:new E(a._attrs,a.dur,a.easing,a._callback),mina:a,curStatus:a.status(),status:function(c){return a.status(c)},stop:function(){a.stop()}})}(this.anims[a]);
return c};a.animate=function(c,a,b,m,e,h){"function"!=typeof e||e.length||(h=e,e=L.linear);var d=L.time();c=L(c,a,d,d+m,L.time,b,e);h&&k.once("mina.finish."+c.id,h);return c};c.stop=function(){for(var c=this.inAnim(),a=0,b=c.length;a<b;a++)c[a].stop();return this};c.animate=function(c,a,b,m){"function"!=typeof b||b.length||(m=b,b=L.linear);c instanceof E&&(m=c.callback,b=c.easing,a=b.dur,c=c.attr);var d=[],f=[],l={},t,ca,n,T=this,q;for(q in c)if(c[h](q)){T.equal?(n=T.equal(q,J(c[q])),t=n.from,ca=
n.to,n=n.f):(t=+T.attr(q),ca=+c[q]);var la=y(t,"array")?t.length:1;l[q]=e(d.length,d.length+la,n);d=d.concat(t);f=f.concat(ca)}t=L.time();var p=L(d,f,t,t+a,L.time,function(c){var a={},b;for(b in l)l[h](b)&&(a[b]=l[b](c));T.attr(a)},b);T.anims[p.id]=p;p._attrs=c;p._callback=m;k("snap.animcreated."+T.id,p);k.once("mina.finish."+p.id,function(){delete T.anims[p.id];m&&m.call(T)});k.once("mina.stop."+p.id,function(){delete T.anims[p.id]});return T};var T={};c.data=function(c,b){var m=T[this.id]=T[this.id]||
{};if(0==arguments.length)return k("snap.data.get."+this.id,this,m,null),m;if(1==arguments.length){if(a.is(c,"object")){for(var e in c)c[h](e)&&this.data(e,c[e]);return this}k("snap.data.get."+this.id,this,m[c],c);return m[c]}m[c]=b;k("snap.data.set."+this.id,this,b,c);return this};c.removeData=function(c){null==c?T[this.id]={}:T[this.id]&&delete T[this.id][c];return this};c.outerSVG=c.toString=d(1);c.innerSVG=d()})(e.prototype);a.parse=function(c){var a=G.doc.createDocumentFragment(),b=!0,m=G.doc.createElement("div");
c=J(c);c.match(/^\s*<\s*svg(?:\s|>)/)||(c="<svg>"+c+"</svg>",b=!1);m.innerHTML=c;if(c=m.getElementsByTagName("svg")[0])if(b)a=c;else for(;c.firstChild;)a.appendChild(c.firstChild);m.innerHTML=aa;return new l(a)};l.prototype.select=e.prototype.select;l.prototype.selectAll=e.prototype.selectAll;a.fragment=function(){for(var c=Array.prototype.slice.call(arguments,0),b=G.doc.createDocumentFragment(),m=0,e=c.length;m<e;m++){var h=c[m];h.node&&h.node.nodeType&&b.appendChild(h.node);h.nodeType&&b.appendChild(h);
"string"==typeof h&&b.appendChild(a.parse(h).node)}return new l(b)};a._.make=r;a._.wrap=x;s.prototype.el=function(c,a){var b=r(c,this.node);a&&b.attr(a);return b};k.on("snap.util.getattr",function(){var c=k.nt(),c=c.substring(c.lastIndexOf(".")+1),a=c.replace(/[A-Z]/g,function(c){return"-"+c.toLowerCase()});return pa[h](a)?this.node.ownerDocument.defaultView.getComputedStyle(this.node,null).getPropertyValue(a):v(this.node,c)});var pa={"alignment-baseline":0,"baseline-shift":0,clip:0,"clip-path":0,
"clip-rule":0,color:0,"color-interpolation":0,"color-interpolation-filters":0,"color-profile":0,"color-rendering":0,cursor:0,direction:0,display:0,"dominant-baseline":0,"enable-background":0,fill:0,"fill-opacity":0,"fill-rule":0,filter:0,"flood-color":0,"flood-opacity":0,font:0,"font-family":0,"font-size":0,"font-size-adjust":0,"font-stretch":0,"font-style":0,"font-variant":0,"font-weight":0,"glyph-orientation-horizontal":0,"glyph-orientation-vertical":0,"image-rendering":0,kerning:0,"letter-spacing":0,
"lighting-color":0,marker:0,"marker-end":0,"marker-mid":0,"marker-start":0,mask:0,opacity:0,overflow:0,"pointer-events":0,"shape-rendering":0,"stop-color":0,"stop-opacity":0,stroke:0,"stroke-dasharray":0,"stroke-dashoffset":0,"stroke-linecap":0,"stroke-linejoin":0,"stroke-miterlimit":0,"stroke-opacity":0,"stroke-width":0,"text-anchor":0,"text-decoration":0,"text-rendering":0,"unicode-bidi":0,visibility:0,"word-spacing":0,"writing-mode":0};k.on("snap.util.attr",function(c){var a=k.nt(),b={},a=a.substring(a.lastIndexOf(".")+
1);b[a]=c;var m=a.replace(/-(\w)/gi,function(c,a){return a.toUpperCase()}),a=a.replace(/[A-Z]/g,function(c){return"-"+c.toLowerCase()});pa[h](a)?this.node.style[m]=null==c?aa:c:v(this.node,b)});a.ajax=function(c,a,b,m){var e=new XMLHttpRequest,h=V();if(e){if(y(a,"function"))m=b,b=a,a=null;else if(y(a,"object")){var d=[],f;for(f in a)a.hasOwnProperty(f)&&d.push(encodeURIComponent(f)+"="+encodeURIComponent(a[f]));a=d.join("&")}e.open(a?"POST":"GET",c,!0);a&&(e.setRequestHeader("X-Requested-With","XMLHttpRequest"),
e.setRequestHeader("Content-type","application/x-www-form-urlencoded"));b&&(k.once("snap.ajax."+h+".0",b),k.once("snap.ajax."+h+".200",b),k.once("snap.ajax."+h+".304",b));e.onreadystatechange=function(){4==e.readyState&&k("snap.ajax."+h+"."+e.status,m,e)};if(4==e.readyState)return e;e.send(a);return e}};a.load=function(c,b,m){a.ajax(c,function(c){c=a.parse(c.responseText);m?b.call(m,c):b(c)})};a.getElementByPoint=function(c,a){var b,m,e=G.doc.elementFromPoint(c,a);if(G.win.opera&&"svg"==e.tagName){b=
e;m=b.getBoundingClientRect();b=b.ownerDocument;var h=b.body,d=b.documentElement;b=m.top+(g.win.pageYOffset||d.scrollTop||h.scrollTop)-(d.clientTop||h.clientTop||0);m=m.left+(g.win.pageXOffset||d.scrollLeft||h.scrollLeft)-(d.clientLeft||h.clientLeft||0);h=e.createSVGRect();h.x=c-m;h.y=a-b;h.width=h.height=1;b=e.getIntersectionList(h,null);b.length&&(e=b[b.length-1])}return e?x(e):null};a.plugin=function(c){c(a,e,s,G,l)};return G.win.Snap=a}();C.plugin(function(a,k,y,M,A){function w(a,d,f,b,q,e){null==
d&&"[object SVGMatrix]"==z.call(a)?(this.a=a.a,this.b=a.b,this.c=a.c,this.d=a.d,this.e=a.e,this.f=a.f):null!=a?(this.a=+a,this.b=+d,this.c=+f,this.d=+b,this.e=+q,this.f=+e):(this.a=1,this.c=this.b=0,this.d=1,this.f=this.e=0)}var z=Object.prototype.toString,d=String,f=Math;(function(n){function k(a){return a[0]*a[0]+a[1]*a[1]}function p(a){var d=f.sqrt(k(a));a[0]&&(a[0]/=d);a[1]&&(a[1]/=d)}n.add=function(a,d,e,f,n,p){var k=[[],[],[] ],u=[[this.a,this.c,this.e],[this.b,this.d,this.f],[0,0,1] ];d=[[a,
e,n],[d,f,p],[0,0,1] ];a&&a instanceof w&&(d=[[a.a,a.c,a.e],[a.b,a.d,a.f],[0,0,1] ]);for(a=0;3>a;a++)for(e=0;3>e;e++){for(f=n=0;3>f;f++)n+=u[a][f]*d[f][e];k[a][e]=n}this.a=k[0][0];this.b=k[1][0];this.c=k[0][1];this.d=k[1][1];this.e=k[0][2];this.f=k[1][2];return this};n.invert=function(){var a=this.a*this.d-this.b*this.c;return new w(this.d/a,-this.b/a,-this.c/a,this.a/a,(this.c*this.f-this.d*this.e)/a,(this.b*this.e-this.a*this.f)/a)};n.clone=function(){return new w(this.a,this.b,this.c,this.d,this.e,
this.f)};n.translate=function(a,d){return this.add(1,0,0,1,a,d)};n.scale=function(a,d,e,f){null==d&&(d=a);(e||f)&&this.add(1,0,0,1,e,f);this.add(a,0,0,d,0,0);(e||f)&&this.add(1,0,0,1,-e,-f);return this};n.rotate=function(b,d,e){b=a.rad(b);d=d||0;e=e||0;var l=+f.cos(b).toFixed(9);b=+f.sin(b).toFixed(9);this.add(l,b,-b,l,d,e);return this.add(1,0,0,1,-d,-e)};n.x=function(a,d){return a*this.a+d*this.c+this.e};n.y=function(a,d){return a*this.b+d*this.d+this.f};n.get=function(a){return+this[d.fromCharCode(97+
a)].toFixed(4)};n.toString=function(){return"matrix("+[this.get(0),this.get(1),this.get(2),this.get(3),this.get(4),this.get(5)].join()+")"};n.offset=function(){return[this.e.toFixed(4),this.f.toFixed(4)]};n.determinant=function(){return this.a*this.d-this.b*this.c};n.split=function(){var b={};b.dx=this.e;b.dy=this.f;var d=[[this.a,this.c],[this.b,this.d] ];b.scalex=f.sqrt(k(d[0]));p(d[0]);b.shear=d[0][0]*d[1][0]+d[0][1]*d[1][1];d[1]=[d[1][0]-d[0][0]*b.shear,d[1][1]-d[0][1]*b.shear];b.scaley=f.sqrt(k(d[1]));
p(d[1]);b.shear/=b.scaley;0>this.determinant()&&(b.scalex=-b.scalex);var e=-d[0][1],d=d[1][1];0>d?(b.rotate=a.deg(f.acos(d)),0>e&&(b.rotate=360-b.rotate)):b.rotate=a.deg(f.asin(e));b.isSimple=!+b.shear.toFixed(9)&&(b.scalex.toFixed(9)==b.scaley.toFixed(9)||!b.rotate);b.isSuperSimple=!+b.shear.toFixed(9)&&b.scalex.toFixed(9)==b.scaley.toFixed(9)&&!b.rotate;b.noRotation=!+b.shear.toFixed(9)&&!b.rotate;return b};n.toTransformString=function(a){a=a||this.split();if(+a.shear.toFixed(9))return"m"+[this.get(0),
this.get(1),this.get(2),this.get(3),this.get(4),this.get(5)];a.scalex=+a.scalex.toFixed(4);a.scaley=+a.scaley.toFixed(4);a.rotate=+a.rotate.toFixed(4);return(a.dx||a.dy?"t"+[+a.dx.toFixed(4),+a.dy.toFixed(4)]:"")+(1!=a.scalex||1!=a.scaley?"s"+[a.scalex,a.scaley,0,0]:"")+(a.rotate?"r"+[+a.rotate.toFixed(4),0,0]:"")}})(w.prototype);a.Matrix=w;a.matrix=function(a,d,f,b,k,e){return new w(a,d,f,b,k,e)}});C.plugin(function(a,v,y,M,A){function w(h){return function(d){k.stop();d instanceof A&&1==d.node.childNodes.length&&
("radialGradient"==d.node.firstChild.tagName||"linearGradient"==d.node.firstChild.tagName||"pattern"==d.node.firstChild.tagName)&&(d=d.node.firstChild,b(this).appendChild(d),d=u(d));if(d instanceof v)if("radialGradient"==d.type||"linearGradient"==d.type||"pattern"==d.type){d.node.id||e(d.node,{id:d.id});var f=l(d.node.id)}else f=d.attr(h);else f=a.color(d),f.error?(f=a(b(this).ownerSVGElement).gradient(d))?(f.node.id||e(f.node,{id:f.id}),f=l(f.node.id)):f=d:f=r(f);d={};d[h]=f;e(this.node,d);this.node.style[h]=
x}}function z(a){k.stop();a==+a&&(a+="px");this.node.style.fontSize=a}function d(a){var b=[];a=a.childNodes;for(var e=0,f=a.length;e<f;e++){var l=a[e];3==l.nodeType&&b.push(l.nodeValue);"tspan"==l.tagName&&(1==l.childNodes.length&&3==l.firstChild.nodeType?b.push(l.firstChild.nodeValue):b.push(d(l)))}return b}function f(){k.stop();return this.node.style.fontSize}var n=a._.make,u=a._.wrap,p=a.is,b=a._.getSomeDefs,q=/^url\(#?([^)]+)\)$/,e=a._.$,l=a.url,r=String,s=a._.separator,x="";k.on("snap.util.attr.mask",
function(a){if(a instanceof v||a instanceof A){k.stop();a instanceof A&&1==a.node.childNodes.length&&(a=a.node.firstChild,b(this).appendChild(a),a=u(a));if("mask"==a.type)var d=a;else d=n("mask",b(this)),d.node.appendChild(a.node);!d.node.id&&e(d.node,{id:d.id});e(this.node,{mask:l(d.id)})}});(function(a){k.on("snap.util.attr.clip",a);k.on("snap.util.attr.clip-path",a);k.on("snap.util.attr.clipPath",a)})(function(a){if(a instanceof v||a instanceof A){k.stop();if("clipPath"==a.type)var d=a;else d=
n("clipPath",b(this)),d.node.appendChild(a.node),!d.node.id&&e(d.node,{id:d.id});e(this.node,{"clip-path":l(d.id)})}});k.on("snap.util.attr.fill",w("fill"));k.on("snap.util.attr.stroke",w("stroke"));var G=/^([lr])(?:\(([^)]*)\))?(.*)$/i;k.on("snap.util.grad.parse",function(a){a=r(a);var b=a.match(G);if(!b)return null;a=b[1];var e=b[2],b=b[3],e=e.split(/\s*,\s*/).map(function(a){return+a==a?+a:a});1==e.length&&0==e[0]&&(e=[]);b=b.split("-");b=b.map(function(a){a=a.split(":");var b={color:a[0]};a[1]&&
(b.offset=parseFloat(a[1]));return b});return{type:a,params:e,stops:b}});k.on("snap.util.attr.d",function(b){k.stop();p(b,"array")&&p(b[0],"array")&&(b=a.path.toString.call(b));b=r(b);b.match(/[ruo]/i)&&(b=a.path.toAbsolute(b));e(this.node,{d:b})})(-1);k.on("snap.util.attr.#text",function(a){k.stop();a=r(a);for(a=M.doc.createTextNode(a);this.node.firstChild;)this.node.removeChild(this.node.firstChild);this.node.appendChild(a)})(-1);k.on("snap.util.attr.path",function(a){k.stop();this.attr({d:a})})(-1);
k.on("snap.util.attr.class",function(a){k.stop();this.node.className.baseVal=a})(-1);k.on("snap.util.attr.viewBox",function(a){a=p(a,"object")&&"x"in a?[a.x,a.y,a.width,a.height].join(" "):p(a,"array")?a.join(" "):a;e(this.node,{viewBox:a});k.stop()})(-1);k.on("snap.util.attr.transform",function(a){this.transform(a);k.stop()})(-1);k.on("snap.util.attr.r",function(a){"rect"==this.type&&(k.stop(),e(this.node,{rx:a,ry:a}))})(-1);k.on("snap.util.attr.textpath",function(a){k.stop();if("text"==this.type){var d,
f;if(!a&&this.textPath){for(a=this.textPath;a.node.firstChild;)this.node.appendChild(a.node.firstChild);a.remove();delete this.textPath}else if(p(a,"string")?(d=b(this),a=u(d.parentNode).path(a),d.appendChild(a.node),d=a.id,a.attr({id:d})):(a=u(a),a instanceof v&&(d=a.attr("id"),d||(d=a.id,a.attr({id:d})))),d)if(a=this.textPath,f=this.node,a)a.attr({"xlink:href":"#"+d});else{for(a=e("textPath",{"xlink:href":"#"+d});f.firstChild;)a.appendChild(f.firstChild);f.appendChild(a);this.textPath=u(a)}}})(-1);
k.on("snap.util.attr.text",function(a){if("text"==this.type){for(var b=this.node,d=function(a){var b=e("tspan");if(p(a,"array"))for(var f=0;f<a.length;f++)b.appendChild(d(a[f]));else b.appendChild(M.doc.createTextNode(a));b.normalize&&b.normalize();return b};b.firstChild;)b.removeChild(b.firstChild);for(a=d(a);a.firstChild;)b.appendChild(a.firstChild)}k.stop()})(-1);k.on("snap.util.attr.fontSize",z)(-1);k.on("snap.util.attr.font-size",z)(-1);k.on("snap.util.getattr.transform",function(){k.stop();
return this.transform()})(-1);k.on("snap.util.getattr.textpath",function(){k.stop();return this.textPath})(-1);(function(){function b(d){return function(){k.stop();var b=M.doc.defaultView.getComputedStyle(this.node,null).getPropertyValue("marker-"+d);return"none"==b?b:a(M.doc.getElementById(b.match(q)[1]))}}function d(a){return function(b){k.stop();var d="marker"+a.charAt(0).toUpperCase()+a.substring(1);if(""==b||!b)this.node.style[d]="none";else if("marker"==b.type){var f=b.node.id;f||e(b.node,{id:b.id});
this.node.style[d]=l(f)}}}k.on("snap.util.getattr.marker-end",b("end"))(-1);k.on("snap.util.getattr.markerEnd",b("end"))(-1);k.on("snap.util.getattr.marker-start",b("start"))(-1);k.on("snap.util.getattr.markerStart",b("start"))(-1);k.on("snap.util.getattr.marker-mid",b("mid"))(-1);k.on("snap.util.getattr.markerMid",b("mid"))(-1);k.on("snap.util.attr.marker-end",d("end"))(-1);k.on("snap.util.attr.markerEnd",d("end"))(-1);k.on("snap.util.attr.marker-start",d("start"))(-1);k.on("snap.util.attr.markerStart",
d("start"))(-1);k.on("snap.util.attr.marker-mid",d("mid"))(-1);k.on("snap.util.attr.markerMid",d("mid"))(-1)})();k.on("snap.util.getattr.r",function(){if("rect"==this.type&&e(this.node,"rx")==e(this.node,"ry"))return k.stop(),e(this.node,"rx")})(-1);k.on("snap.util.getattr.text",function(){if("text"==this.type||"tspan"==this.type){k.stop();var a=d(this.node);return 1==a.length?a[0]:a}})(-1);k.on("snap.util.getattr.#text",function(){return this.node.textContent})(-1);k.on("snap.util.getattr.viewBox",
function(){k.stop();var b=e(this.node,"viewBox");if(b)return b=b.split(s),a._.box(+b[0],+b[1],+b[2],+b[3])})(-1);k.on("snap.util.getattr.points",function(){var a=e(this.node,"points");k.stop();if(a)return a.split(s)})(-1);k.on("snap.util.getattr.path",function(){var a=e(this.node,"d");k.stop();return a})(-1);k.on("snap.util.getattr.class",function(){return this.node.className.baseVal})(-1);k.on("snap.util.getattr.fontSize",f)(-1);k.on("snap.util.getattr.font-size",f)(-1)});C.plugin(function(a,v,y,
M,A){function w(a){return a}function z(a){return function(b){return+b.toFixed(3)+a}}var d={"+":function(a,b){return a+b},"-":function(a,b){return a-b},"/":function(a,b){return a/b},"*":function(a,b){return a*b}},f=String,n=/[a-z]+$/i,u=/^\s*([+\-\/*])\s*=\s*([\d.eE+\-]+)\s*([^\d\s]+)?\s*$/;k.on("snap.util.attr",function(a){if(a=f(a).match(u)){var b=k.nt(),b=b.substring(b.lastIndexOf(".")+1),q=this.attr(b),e={};k.stop();var l=a[3]||"",r=q.match(n),s=d[a[1] ];r&&r==l?a=s(parseFloat(q),+a[2]):(q=this.asPX(b),
a=s(this.asPX(b),this.asPX(b,a[2]+l)));isNaN(q)||isNaN(a)||(e[b]=a,this.attr(e))}})(-10);k.on("snap.util.equal",function(a,b){var q=f(this.attr(a)||""),e=f(b).match(u);if(e){k.stop();var l=e[3]||"",r=q.match(n),s=d[e[1] ];if(r&&r==l)return{from:parseFloat(q),to:s(parseFloat(q),+e[2]),f:z(r)};q=this.asPX(a);return{from:q,to:s(q,this.asPX(a,e[2]+l)),f:w}}})(-10)});C.plugin(function(a,v,y,M,A){var w=y.prototype,z=a.is;w.rect=function(a,d,k,p,b,q){var e;null==q&&(q=b);z(a,"object")&&"[object Object]"==
a?e=a:null!=a&&(e={x:a,y:d,width:k,height:p},null!=b&&(e.rx=b,e.ry=q));return this.el("rect",e)};w.circle=function(a,d,k){var p;z(a,"object")&&"[object Object]"==a?p=a:null!=a&&(p={cx:a,cy:d,r:k});return this.el("circle",p)};var d=function(){function a(){this.parentNode.removeChild(this)}return function(d,k){var p=M.doc.createElement("img"),b=M.doc.body;p.style.cssText="position:absolute;left:-9999em;top:-9999em";p.onload=function(){k.call(p);p.onload=p.onerror=null;b.removeChild(p)};p.onerror=a;
b.appendChild(p);p.src=d}}();w.image=function(f,n,k,p,b){var q=this.el("image");if(z(f,"object")&&"src"in f)q.attr(f);else if(null!=f){var e={"xlink:href":f,preserveAspectRatio:"none"};null!=n&&null!=k&&(e.x=n,e.y=k);null!=p&&null!=b?(e.width=p,e.height=b):d(f,function(){a._.$(q.node,{width:this.offsetWidth,height:this.offsetHeight})});a._.$(q.node,e)}return q};w.ellipse=function(a,d,k,p){var b;z(a,"object")&&"[object Object]"==a?b=a:null!=a&&(b={cx:a,cy:d,rx:k,ry:p});return this.el("ellipse",b)};
w.path=function(a){var d;z(a,"object")&&!z(a,"array")?d=a:a&&(d={d:a});return this.el("path",d)};w.group=w.g=function(a){var d=this.el("g");1==arguments.length&&a&&!a.type?d.attr(a):arguments.length&&d.add(Array.prototype.slice.call(arguments,0));return d};w.svg=function(a,d,k,p,b,q,e,l){var r={};z(a,"object")&&null==d?r=a:(null!=a&&(r.x=a),null!=d&&(r.y=d),null!=k&&(r.width=k),null!=p&&(r.height=p),null!=b&&null!=q&&null!=e&&null!=l&&(r.viewBox=[b,q,e,l]));return this.el("svg",r)};w.mask=function(a){var d=
this.el("mask");1==arguments.length&&a&&!a.type?d.attr(a):arguments.length&&d.add(Array.prototype.slice.call(arguments,0));return d};w.ptrn=function(a,d,k,p,b,q,e,l){if(z(a,"object"))var r=a;else arguments.length?(r={},null!=a&&(r.x=a),null!=d&&(r.y=d),null!=k&&(r.width=k),null!=p&&(r.height=p),null!=b&&null!=q&&null!=e&&null!=l&&(r.viewBox=[b,q,e,l])):r={patternUnits:"userSpaceOnUse"};return this.el("pattern",r)};w.use=function(a){return null!=a?(make("use",this.node),a instanceof v&&(a.attr("id")||
a.attr({id:ID()}),a=a.attr("id")),this.el("use",{"xlink:href":a})):v.prototype.use.call(this)};w.text=function(a,d,k){var p={};z(a,"object")?p=a:null!=a&&(p={x:a,y:d,text:k||""});return this.el("text",p)};w.line=function(a,d,k,p){var b={};z(a,"object")?b=a:null!=a&&(b={x1:a,x2:k,y1:d,y2:p});return this.el("line",b)};w.polyline=function(a){1<arguments.length&&(a=Array.prototype.slice.call(arguments,0));var d={};z(a,"object")&&!z(a,"array")?d=a:null!=a&&(d={points:a});return this.el("polyline",d)};
w.polygon=function(a){1<arguments.length&&(a=Array.prototype.slice.call(arguments,0));var d={};z(a,"object")&&!z(a,"array")?d=a:null!=a&&(d={points:a});return this.el("polygon",d)};(function(){function d(){return this.selectAll("stop")}function n(b,d){var f=e("stop"),k={offset:+d+"%"};b=a.color(b);k["stop-color"]=b.hex;1>b.opacity&&(k["stop-opacity"]=b.opacity);e(f,k);this.node.appendChild(f);return this}function u(){if("linearGradient"==this.type){var b=e(this.node,"x1")||0,d=e(this.node,"x2")||
1,f=e(this.node,"y1")||0,k=e(this.node,"y2")||0;return a._.box(b,f,math.abs(d-b),math.abs(k-f))}b=this.node.r||0;return a._.box((this.node.cx||0.5)-b,(this.node.cy||0.5)-b,2*b,2*b)}function p(a,d){function f(a,b){for(var d=(b-u)/(a-w),e=w;e<a;e++)h[e].offset=+(+u+d*(e-w)).toFixed(2);w=a;u=b}var n=k("snap.util.grad.parse",null,d).firstDefined(),p;if(!n)return null;n.params.unshift(a);p="l"==n.type.toLowerCase()?b.apply(0,n.params):q.apply(0,n.params);n.type!=n.type.toLowerCase()&&e(p.node,{gradientUnits:"userSpaceOnUse"});
var h=n.stops,n=h.length,u=0,w=0;n--;for(var v=0;v<n;v++)"offset"in h[v]&&f(v,h[v].offset);h[n].offset=h[n].offset||100;f(n,h[n].offset);for(v=0;v<=n;v++){var y=h[v];p.addStop(y.color,y.offset)}return p}function b(b,k,p,q,w){b=a._.make("linearGradient",b);b.stops=d;b.addStop=n;b.getBBox=u;null!=k&&e(b.node,{x1:k,y1:p,x2:q,y2:w});return b}function q(b,k,p,q,w,h){b=a._.make("radialGradient",b);b.stops=d;b.addStop=n;b.getBBox=u;null!=k&&e(b.node,{cx:k,cy:p,r:q});null!=w&&null!=h&&e(b.node,{fx:w,fy:h});
return b}var e=a._.$;w.gradient=function(a){return p(this.defs,a)};w.gradientLinear=function(a,d,e,f){return b(this.defs,a,d,e,f)};w.gradientRadial=function(a,b,d,e,f){return q(this.defs,a,b,d,e,f)};w.toString=function(){var b=this.node.ownerDocument,d=b.createDocumentFragment(),b=b.createElement("div"),e=this.node.cloneNode(!0);d.appendChild(b);b.appendChild(e);a._.$(e,{xmlns:"http://www.w3.org/2000/svg"});b=b.innerHTML;d.removeChild(d.firstChild);return b};w.clear=function(){for(var a=this.node.firstChild,
b;a;)b=a.nextSibling,"defs"!=a.tagName?a.parentNode.removeChild(a):w.clear.call({node:a}),a=b}})()});C.plugin(function(a,k,y,M){function A(a){var b=A.ps=A.ps||{};b[a]?b[a].sleep=100:b[a]={sleep:100};setTimeout(function(){for(var d in b)b[L](d)&&d!=a&&(b[d].sleep--,!b[d].sleep&&delete b[d])});return b[a]}function w(a,b,d,e){null==a&&(a=b=d=e=0);null==b&&(b=a.y,d=a.width,e=a.height,a=a.x);return{x:a,y:b,width:d,w:d,height:e,h:e,x2:a+d,y2:b+e,cx:a+d/2,cy:b+e/2,r1:F.min(d,e)/2,r2:F.max(d,e)/2,r0:F.sqrt(d*
d+e*e)/2,path:s(a,b,d,e),vb:[a,b,d,e].join(" ")}}function z(){return this.join(",").replace(N,"$1")}function d(a){a=C(a);a.toString=z;return a}function f(a,b,d,h,f,k,l,n,p){if(null==p)return e(a,b,d,h,f,k,l,n);if(0>p||e(a,b,d,h,f,k,l,n)<p)p=void 0;else{var q=0.5,O=1-q,s;for(s=e(a,b,d,h,f,k,l,n,O);0.01<Z(s-p);)q/=2,O+=(s<p?1:-1)*q,s=e(a,b,d,h,f,k,l,n,O);p=O}return u(a,b,d,h,f,k,l,n,p)}function n(b,d){function e(a){return+(+a).toFixed(3)}return a._.cacher(function(a,h,l){a instanceof k&&(a=a.attr("d"));
a=I(a);for(var n,p,D,q,O="",s={},c=0,t=0,r=a.length;t<r;t++){D=a[t];if("M"==D[0])n=+D[1],p=+D[2];else{q=f(n,p,D[1],D[2],D[3],D[4],D[5],D[6]);if(c+q>h){if(d&&!s.start){n=f(n,p,D[1],D[2],D[3],D[4],D[5],D[6],h-c);O+=["C"+e(n.start.x),e(n.start.y),e(n.m.x),e(n.m.y),e(n.x),e(n.y)];if(l)return O;s.start=O;O=["M"+e(n.x),e(n.y)+"C"+e(n.n.x),e(n.n.y),e(n.end.x),e(n.end.y),e(D[5]),e(D[6])].join();c+=q;n=+D[5];p=+D[6];continue}if(!b&&!d)return n=f(n,p,D[1],D[2],D[3],D[4],D[5],D[6],h-c)}c+=q;n=+D[5];p=+D[6]}O+=
D.shift()+D}s.end=O;return n=b?c:d?s:u(n,p,D[0],D[1],D[2],D[3],D[4],D[5],1)},null,a._.clone)}function u(a,b,d,e,h,f,k,l,n){var p=1-n,q=ma(p,3),s=ma(p,2),c=n*n,t=c*n,r=q*a+3*s*n*d+3*p*n*n*h+t*k,q=q*b+3*s*n*e+3*p*n*n*f+t*l,s=a+2*n*(d-a)+c*(h-2*d+a),t=b+2*n*(e-b)+c*(f-2*e+b),x=d+2*n*(h-d)+c*(k-2*h+d),c=e+2*n*(f-e)+c*(l-2*f+e);a=p*a+n*d;b=p*b+n*e;h=p*h+n*k;f=p*f+n*l;l=90-180*F.atan2(s-x,t-c)/S;return{x:r,y:q,m:{x:s,y:t},n:{x:x,y:c},start:{x:a,y:b},end:{x:h,y:f},alpha:l}}function p(b,d,e,h,f,n,k,l){a.is(b,
"array")||(b=[b,d,e,h,f,n,k,l]);b=U.apply(null,b);return w(b.min.x,b.min.y,b.max.x-b.min.x,b.max.y-b.min.y)}function b(a,b,d){return b>=a.x&&b<=a.x+a.width&&d>=a.y&&d<=a.y+a.height}function q(a,d){a=w(a);d=w(d);return b(d,a.x,a.y)||b(d,a.x2,a.y)||b(d,a.x,a.y2)||b(d,a.x2,a.y2)||b(a,d.x,d.y)||b(a,d.x2,d.y)||b(a,d.x,d.y2)||b(a,d.x2,d.y2)||(a.x<d.x2&&a.x>d.x||d.x<a.x2&&d.x>a.x)&&(a.y<d.y2&&a.y>d.y||d.y<a.y2&&d.y>a.y)}function e(a,b,d,e,h,f,n,k,l){null==l&&(l=1);l=(1<l?1:0>l?0:l)/2;for(var p=[-0.1252,
0.1252,-0.3678,0.3678,-0.5873,0.5873,-0.7699,0.7699,-0.9041,0.9041,-0.9816,0.9816],q=[0.2491,0.2491,0.2335,0.2335,0.2032,0.2032,0.1601,0.1601,0.1069,0.1069,0.0472,0.0472],s=0,c=0;12>c;c++)var t=l*p[c]+l,r=t*(t*(-3*a+9*d-9*h+3*n)+6*a-12*d+6*h)-3*a+3*d,t=t*(t*(-3*b+9*e-9*f+3*k)+6*b-12*e+6*f)-3*b+3*e,s=s+q[c]*F.sqrt(r*r+t*t);return l*s}function l(a,b,d){a=I(a);b=I(b);for(var h,f,l,n,k,s,r,O,x,c,t=d?0:[],w=0,v=a.length;w<v;w++)if(x=a[w],"M"==x[0])h=k=x[1],f=s=x[2];else{"C"==x[0]?(x=[h,f].concat(x.slice(1)),
h=x[6],f=x[7]):(x=[h,f,h,f,k,s,k,s],h=k,f=s);for(var G=0,y=b.length;G<y;G++)if(c=b[G],"M"==c[0])l=r=c[1],n=O=c[2];else{"C"==c[0]?(c=[l,n].concat(c.slice(1)),l=c[6],n=c[7]):(c=[l,n,l,n,r,O,r,O],l=r,n=O);var z;var K=x,B=c;z=d;var H=p(K),J=p(B);if(q(H,J)){for(var H=e.apply(0,K),J=e.apply(0,B),H=~~(H/8),J=~~(J/8),U=[],A=[],F={},M=z?0:[],P=0;P<H+1;P++){var C=u.apply(0,K.concat(P/H));U.push({x:C.x,y:C.y,t:P/H})}for(P=0;P<J+1;P++)C=u.apply(0,B.concat(P/J)),A.push({x:C.x,y:C.y,t:P/J});for(P=0;P<H;P++)for(K=
0;K<J;K++){var Q=U[P],L=U[P+1],B=A[K],C=A[K+1],N=0.001>Z(L.x-Q.x)?"y":"x",S=0.001>Z(C.x-B.x)?"y":"x",R;R=Q.x;var Y=Q.y,V=L.x,ea=L.y,fa=B.x,ga=B.y,ha=C.x,ia=C.y;if(W(R,V)<X(fa,ha)||X(R,V)>W(fa,ha)||W(Y,ea)<X(ga,ia)||X(Y,ea)>W(ga,ia))R=void 0;else{var $=(R*ea-Y*V)*(fa-ha)-(R-V)*(fa*ia-ga*ha),aa=(R*ea-Y*V)*(ga-ia)-(Y-ea)*(fa*ia-ga*ha),ja=(R-V)*(ga-ia)-(Y-ea)*(fa-ha);if(ja){var $=$/ja,aa=aa/ja,ja=+$.toFixed(2),ba=+aa.toFixed(2);R=ja<+X(R,V).toFixed(2)||ja>+W(R,V).toFixed(2)||ja<+X(fa,ha).toFixed(2)||
ja>+W(fa,ha).toFixed(2)||ba<+X(Y,ea).toFixed(2)||ba>+W(Y,ea).toFixed(2)||ba<+X(ga,ia).toFixed(2)||ba>+W(ga,ia).toFixed(2)?void 0:{x:$,y:aa}}else R=void 0}R&&F[R.x.toFixed(4)]!=R.y.toFixed(4)&&(F[R.x.toFixed(4)]=R.y.toFixed(4),Q=Q.t+Z((R[N]-Q[N])/(L[N]-Q[N]))*(L.t-Q.t),B=B.t+Z((R[S]-B[S])/(C[S]-B[S]))*(C.t-B.t),0<=Q&&1>=Q&&0<=B&&1>=B&&(z?M++:M.push({x:R.x,y:R.y,t1:Q,t2:B})))}z=M}else z=z?0:[];if(d)t+=z;else{H=0;for(J=z.length;H<J;H++)z[H].segment1=w,z[H].segment2=G,z[H].bez1=x,z[H].bez2=c;t=t.concat(z)}}}return t}
function r(a){var b=A(a);if(b.bbox)return C(b.bbox);if(!a)return w();a=I(a);for(var d=0,e=0,h=[],f=[],l,n=0,k=a.length;n<k;n++)l=a[n],"M"==l[0]?(d=l[1],e=l[2],h.push(d),f.push(e)):(d=U(d,e,l[1],l[2],l[3],l[4],l[5],l[6]),h=h.concat(d.min.x,d.max.x),f=f.concat(d.min.y,d.max.y),d=l[5],e=l[6]);a=X.apply(0,h);l=X.apply(0,f);h=W.apply(0,h);f=W.apply(0,f);f=w(a,l,h-a,f-l);b.bbox=C(f);return f}function s(a,b,d,e,h){if(h)return[["M",+a+ +h,b],["l",d-2*h,0],["a",h,h,0,0,1,h,h],["l",0,e-2*h],["a",h,h,0,0,1,
-h,h],["l",2*h-d,0],["a",h,h,0,0,1,-h,-h],["l",0,2*h-e],["a",h,h,0,0,1,h,-h],["z"] ];a=[["M",a,b],["l",d,0],["l",0,e],["l",-d,0],["z"] ];a.toString=z;return a}function x(a,b,d,e,h){null==h&&null==e&&(e=d);a=+a;b=+b;d=+d;e=+e;if(null!=h){var f=Math.PI/180,l=a+d*Math.cos(-e*f);a+=d*Math.cos(-h*f);var n=b+d*Math.sin(-e*f);b+=d*Math.sin(-h*f);d=[["M",l,n],["A",d,d,0,+(180<h-e),0,a,b] ]}else d=[["M",a,b],["m",0,-e],["a",d,e,0,1,1,0,2*e],["a",d,e,0,1,1,0,-2*e],["z"] ];d.toString=z;return d}function G(b){var e=
A(b);if(e.abs)return d(e.abs);Q(b,"array")&&Q(b&&b[0],"array")||(b=a.parsePathString(b));if(!b||!b.length)return[["M",0,0] ];var h=[],f=0,l=0,n=0,k=0,p=0;"M"==b[0][0]&&(f=+b[0][1],l=+b[0][2],n=f,k=l,p++,h[0]=["M",f,l]);for(var q=3==b.length&&"M"==b[0][0]&&"R"==b[1][0].toUpperCase()&&"Z"==b[2][0].toUpperCase(),s,r,w=p,c=b.length;w<c;w++){h.push(s=[]);r=b[w];p=r[0];if(p!=p.toUpperCase())switch(s[0]=p.toUpperCase(),s[0]){case "A":s[1]=r[1];s[2]=r[2];s[3]=r[3];s[4]=r[4];s[5]=r[5];s[6]=+r[6]+f;s[7]=+r[7]+
l;break;case "V":s[1]=+r[1]+l;break;case "H":s[1]=+r[1]+f;break;case "R":for(var t=[f,l].concat(r.slice(1)),u=2,v=t.length;u<v;u++)t[u]=+t[u]+f,t[++u]=+t[u]+l;h.pop();h=h.concat(P(t,q));break;case "O":h.pop();t=x(f,l,r[1],r[2]);t.push(t[0]);h=h.concat(t);break;case "U":h.pop();h=h.concat(x(f,l,r[1],r[2],r[3]));s=["U"].concat(h[h.length-1].slice(-2));break;case "M":n=+r[1]+f,k=+r[2]+l;default:for(u=1,v=r.length;u<v;u++)s[u]=+r[u]+(u%2?f:l)}else if("R"==p)t=[f,l].concat(r.slice(1)),h.pop(),h=h.concat(P(t,
q)),s=["R"].concat(r.slice(-2));else if("O"==p)h.pop(),t=x(f,l,r[1],r[2]),t.push(t[0]),h=h.concat(t);else if("U"==p)h.pop(),h=h.concat(x(f,l,r[1],r[2],r[3])),s=["U"].concat(h[h.length-1].slice(-2));else for(t=0,u=r.length;t<u;t++)s[t]=r[t];p=p.toUpperCase();if("O"!=p)switch(s[0]){case "Z":f=+n;l=+k;break;case "H":f=s[1];break;case "V":l=s[1];break;case "M":n=s[s.length-2],k=s[s.length-1];default:f=s[s.length-2],l=s[s.length-1]}}h.toString=z;e.abs=d(h);return h}function h(a,b,d,e){return[a,b,d,e,d,
e]}function J(a,b,d,e,h,f){var l=1/3,n=2/3;return[l*a+n*d,l*b+n*e,l*h+n*d,l*f+n*e,h,f]}function K(b,d,e,h,f,l,n,k,p,s){var r=120*S/180,q=S/180*(+f||0),c=[],t,x=a._.cacher(function(a,b,c){var d=a*F.cos(c)-b*F.sin(c);a=a*F.sin(c)+b*F.cos(c);return{x:d,y:a}});if(s)v=s[0],t=s[1],l=s[2],u=s[3];else{t=x(b,d,-q);b=t.x;d=t.y;t=x(k,p,-q);k=t.x;p=t.y;F.cos(S/180*f);F.sin(S/180*f);t=(b-k)/2;v=(d-p)/2;u=t*t/(e*e)+v*v/(h*h);1<u&&(u=F.sqrt(u),e*=u,h*=u);var u=e*e,w=h*h,u=(l==n?-1:1)*F.sqrt(Z((u*w-u*v*v-w*t*t)/
(u*v*v+w*t*t)));l=u*e*v/h+(b+k)/2;var u=u*-h*t/e+(d+p)/2,v=F.asin(((d-u)/h).toFixed(9));t=F.asin(((p-u)/h).toFixed(9));v=b<l?S-v:v;t=k<l?S-t:t;0>v&&(v=2*S+v);0>t&&(t=2*S+t);n&&v>t&&(v-=2*S);!n&&t>v&&(t-=2*S)}if(Z(t-v)>r){var c=t,w=k,G=p;t=v+r*(n&&t>v?1:-1);k=l+e*F.cos(t);p=u+h*F.sin(t);c=K(k,p,e,h,f,0,n,w,G,[t,c,l,u])}l=t-v;f=F.cos(v);r=F.sin(v);n=F.cos(t);t=F.sin(t);l=F.tan(l/4);e=4/3*e*l;l*=4/3*h;h=[b,d];b=[b+e*r,d-l*f];d=[k+e*t,p-l*n];k=[k,p];b[0]=2*h[0]-b[0];b[1]=2*h[1]-b[1];if(s)return[b,d,k].concat(c);
c=[b,d,k].concat(c).join().split(",");s=[];k=0;for(p=c.length;k<p;k++)s[k]=k%2?x(c[k-1],c[k],q).y:x(c[k],c[k+1],q).x;return s}function U(a,b,d,e,h,f,l,k){for(var n=[],p=[[],[] ],s,r,c,t,q=0;2>q;++q)0==q?(r=6*a-12*d+6*h,s=-3*a+9*d-9*h+3*l,c=3*d-3*a):(r=6*b-12*e+6*f,s=-3*b+9*e-9*f+3*k,c=3*e-3*b),1E-12>Z(s)?1E-12>Z(r)||(s=-c/r,0<s&&1>s&&n.push(s)):(t=r*r-4*c*s,c=F.sqrt(t),0>t||(t=(-r+c)/(2*s),0<t&&1>t&&n.push(t),s=(-r-c)/(2*s),0<s&&1>s&&n.push(s)));for(r=q=n.length;q--;)s=n[q],c=1-s,p[0][q]=c*c*c*a+3*
c*c*s*d+3*c*s*s*h+s*s*s*l,p[1][q]=c*c*c*b+3*c*c*s*e+3*c*s*s*f+s*s*s*k;p[0][r]=a;p[1][r]=b;p[0][r+1]=l;p[1][r+1]=k;p[0].length=p[1].length=r+2;return{min:{x:X.apply(0,p[0]),y:X.apply(0,p[1])},max:{x:W.apply(0,p[0]),y:W.apply(0,p[1])}}}function I(a,b){var e=!b&&A(a);if(!b&&e.curve)return d(e.curve);var f=G(a),l=b&&G(b),n={x:0,y:0,bx:0,by:0,X:0,Y:0,qx:null,qy:null},k={x:0,y:0,bx:0,by:0,X:0,Y:0,qx:null,qy:null},p=function(a,b,c){if(!a)return["C",b.x,b.y,b.x,b.y,b.x,b.y];a[0]in{T:1,Q:1}||(b.qx=b.qy=null);
switch(a[0]){case "M":b.X=a[1];b.Y=a[2];break;case "A":a=["C"].concat(K.apply(0,[b.x,b.y].concat(a.slice(1))));break;case "S":"C"==c||"S"==c?(c=2*b.x-b.bx,b=2*b.y-b.by):(c=b.x,b=b.y);a=["C",c,b].concat(a.slice(1));break;case "T":"Q"==c||"T"==c?(b.qx=2*b.x-b.qx,b.qy=2*b.y-b.qy):(b.qx=b.x,b.qy=b.y);a=["C"].concat(J(b.x,b.y,b.qx,b.qy,a[1],a[2]));break;case "Q":b.qx=a[1];b.qy=a[2];a=["C"].concat(J(b.x,b.y,a[1],a[2],a[3],a[4]));break;case "L":a=["C"].concat(h(b.x,b.y,a[1],a[2]));break;case "H":a=["C"].concat(h(b.x,
b.y,a[1],b.y));break;case "V":a=["C"].concat(h(b.x,b.y,b.x,a[1]));break;case "Z":a=["C"].concat(h(b.x,b.y,b.X,b.Y))}return a},s=function(a,b){if(7<a[b].length){a[b].shift();for(var c=a[b];c.length;)q[b]="A",l&&(u[b]="A"),a.splice(b++,0,["C"].concat(c.splice(0,6)));a.splice(b,1);v=W(f.length,l&&l.length||0)}},r=function(a,b,c,d,e){a&&b&&"M"==a[e][0]&&"M"!=b[e][0]&&(b.splice(e,0,["M",d.x,d.y]),c.bx=0,c.by=0,c.x=a[e][1],c.y=a[e][2],v=W(f.length,l&&l.length||0))},q=[],u=[],c="",t="",x=0,v=W(f.length,
l&&l.length||0);for(;x<v;x++){f[x]&&(c=f[x][0]);"C"!=c&&(q[x]=c,x&&(t=q[x-1]));f[x]=p(f[x],n,t);"A"!=q[x]&&"C"==c&&(q[x]="C");s(f,x);l&&(l[x]&&(c=l[x][0]),"C"!=c&&(u[x]=c,x&&(t=u[x-1])),l[x]=p(l[x],k,t),"A"!=u[x]&&"C"==c&&(u[x]="C"),s(l,x));r(f,l,n,k,x);r(l,f,k,n,x);var w=f[x],z=l&&l[x],y=w.length,U=l&&z.length;n.x=w[y-2];n.y=w[y-1];n.bx=$(w[y-4])||n.x;n.by=$(w[y-3])||n.y;k.bx=l&&($(z[U-4])||k.x);k.by=l&&($(z[U-3])||k.y);k.x=l&&z[U-2];k.y=l&&z[U-1]}l||(e.curve=d(f));return l?[f,l]:f}function P(a,
b){for(var d=[],e=0,h=a.length;h-2*!b>e;e+=2){var f=[{x:+a[e-2],y:+a[e-1]},{x:+a[e],y:+a[e+1]},{x:+a[e+2],y:+a[e+3]},{x:+a[e+4],y:+a[e+5]}];b?e?h-4==e?f[3]={x:+a[0],y:+a[1]}:h-2==e&&(f[2]={x:+a[0],y:+a[1]},f[3]={x:+a[2],y:+a[3]}):f[0]={x:+a[h-2],y:+a[h-1]}:h-4==e?f[3]=f[2]:e||(f[0]={x:+a[e],y:+a[e+1]});d.push(["C",(-f[0].x+6*f[1].x+f[2].x)/6,(-f[0].y+6*f[1].y+f[2].y)/6,(f[1].x+6*f[2].x-f[3].x)/6,(f[1].y+6*f[2].y-f[3].y)/6,f[2].x,f[2].y])}return d}y=k.prototype;var Q=a.is,C=a._.clone,L="hasOwnProperty",
N=/,?([a-z]),?/gi,$=parseFloat,F=Math,S=F.PI,X=F.min,W=F.max,ma=F.pow,Z=F.abs;M=n(1);var na=n(),ba=n(0,1),V=a._unit2px;a.path=A;a.path.getTotalLength=M;a.path.getPointAtLength=na;a.path.getSubpath=function(a,b,d){if(1E-6>this.getTotalLength(a)-d)return ba(a,b).end;a=ba(a,d,1);return b?ba(a,b).end:a};y.getTotalLength=function(){if(this.node.getTotalLength)return this.node.getTotalLength()};y.getPointAtLength=function(a){return na(this.attr("d"),a)};y.getSubpath=function(b,d){return a.path.getSubpath(this.attr("d"),
b,d)};a._.box=w;a.path.findDotsAtSegment=u;a.path.bezierBBox=p;a.path.isPointInsideBBox=b;a.path.isBBoxIntersect=q;a.path.intersection=function(a,b){return l(a,b)};a.path.intersectionNumber=function(a,b){return l(a,b,1)};a.path.isPointInside=function(a,d,e){var h=r(a);return b(h,d,e)&&1==l(a,[["M",d,e],["H",h.x2+10] ],1)%2};a.path.getBBox=r;a.path.get={path:function(a){return a.attr("path")},circle:function(a){a=V(a);return x(a.cx,a.cy,a.r)},ellipse:function(a){a=V(a);return x(a.cx||0,a.cy||0,a.rx,
a.ry)},rect:function(a){a=V(a);return s(a.x||0,a.y||0,a.width,a.height,a.rx,a.ry)},image:function(a){a=V(a);return s(a.x||0,a.y||0,a.width,a.height)},line:function(a){return"M"+[a.attr("x1")||0,a.attr("y1")||0,a.attr("x2"),a.attr("y2")]},polyline:function(a){return"M"+a.attr("points")},polygon:function(a){return"M"+a.attr("points")+"z"},deflt:function(a){a=a.node.getBBox();return s(a.x,a.y,a.width,a.height)}};a.path.toRelative=function(b){var e=A(b),h=String.prototype.toLowerCase;if(e.rel)return d(e.rel);
a.is(b,"array")&&a.is(b&&b[0],"array")||(b=a.parsePathString(b));var f=[],l=0,n=0,k=0,p=0,s=0;"M"==b[0][0]&&(l=b[0][1],n=b[0][2],k=l,p=n,s++,f.push(["M",l,n]));for(var r=b.length;s<r;s++){var q=f[s]=[],x=b[s];if(x[0]!=h.call(x[0]))switch(q[0]=h.call(x[0]),q[0]){case "a":q[1]=x[1];q[2]=x[2];q[3]=x[3];q[4]=x[4];q[5]=x[5];q[6]=+(x[6]-l).toFixed(3);q[7]=+(x[7]-n).toFixed(3);break;case "v":q[1]=+(x[1]-n).toFixed(3);break;case "m":k=x[1],p=x[2];default:for(var c=1,t=x.length;c<t;c++)q[c]=+(x[c]-(c%2?l:
n)).toFixed(3)}else for(f[s]=[],"m"==x[0]&&(k=x[1]+l,p=x[2]+n),q=0,c=x.length;q<c;q++)f[s][q]=x[q];x=f[s].length;switch(f[s][0]){case "z":l=k;n=p;break;case "h":l+=+f[s][x-1];break;case "v":n+=+f[s][x-1];break;default:l+=+f[s][x-2],n+=+f[s][x-1]}}f.toString=z;e.rel=d(f);return f};a.path.toAbsolute=G;a.path.toCubic=I;a.path.map=function(a,b){if(!b)return a;var d,e,h,f,l,n,k;a=I(a);h=0;for(l=a.length;h<l;h++)for(k=a[h],f=1,n=k.length;f<n;f+=2)d=b.x(k[f],k[f+1]),e=b.y(k[f],k[f+1]),k[f]=d,k[f+1]=e;return a};
a.path.toString=z;a.path.clone=d});C.plugin(function(a,v,y,C){var A=Math.max,w=Math.min,z=function(a){this.items=[];this.bindings={};this.length=0;this.type="set";if(a)for(var f=0,n=a.length;f<n;f++)a[f]&&(this[this.items.length]=this.items[this.items.length]=a[f],this.length++)};v=z.prototype;v.push=function(){for(var a,f,n=0,k=arguments.length;n<k;n++)if(a=arguments[n])f=this.items.length,this[f]=this.items[f]=a,this.length++;return this};v.pop=function(){this.length&&delete this[this.length--];
return this.items.pop()};v.forEach=function(a,f){for(var n=0,k=this.items.length;n<k&&!1!==a.call(f,this.items[n],n);n++);return this};v.animate=function(d,f,n,u){"function"!=typeof n||n.length||(u=n,n=L.linear);d instanceof a._.Animation&&(u=d.callback,n=d.easing,f=n.dur,d=d.attr);var p=arguments;if(a.is(d,"array")&&a.is(p[p.length-1],"array"))var b=!0;var q,e=function(){q?this.b=q:q=this.b},l=0,r=u&&function(){l++==this.length&&u.call(this)};return this.forEach(function(a,l){k.once("snap.animcreated."+
a.id,e);b?p[l]&&a.animate.apply(a,p[l]):a.animate(d,f,n,r)})};v.remove=function(){for(;this.length;)this.pop().remove();return this};v.bind=function(a,f,k){var u={};if("function"==typeof f)this.bindings[a]=f;else{var p=k||a;this.bindings[a]=function(a){u[p]=a;f.attr(u)}}return this};v.attr=function(a){var f={},k;for(k in a)if(this.bindings[k])this.bindings[k](a[k]);else f[k]=a[k];a=0;for(k=this.items.length;a<k;a++)this.items[a].attr(f);return this};v.clear=function(){for(;this.length;)this.pop()};
v.splice=function(a,f,k){a=0>a?A(this.length+a,0):a;f=A(0,w(this.length-a,f));var u=[],p=[],b=[],q;for(q=2;q<arguments.length;q++)b.push(arguments[q]);for(q=0;q<f;q++)p.push(this[a+q]);for(;q<this.length-a;q++)u.push(this[a+q]);var e=b.length;for(q=0;q<e+u.length;q++)this.items[a+q]=this[a+q]=q<e?b[q]:u[q-e];for(q=this.items.length=this.length-=f-e;this[q];)delete this[q++];return new z(p)};v.exclude=function(a){for(var f=0,k=this.length;f<k;f++)if(this[f]==a)return this.splice(f,1),!0;return!1};
v.insertAfter=function(a){for(var f=this.items.length;f--;)this.items[f].insertAfter(a);return this};v.getBBox=function(){for(var a=[],f=[],k=[],u=[],p=this.items.length;p--;)if(!this.items[p].removed){var b=this.items[p].getBBox();a.push(b.x);f.push(b.y);k.push(b.x+b.width);u.push(b.y+b.height)}a=w.apply(0,a);f=w.apply(0,f);k=A.apply(0,k);u=A.apply(0,u);return{x:a,y:f,x2:k,y2:u,width:k-a,height:u-f,cx:a+(k-a)/2,cy:f+(u-f)/2}};v.clone=function(a){a=new z;for(var f=0,k=this.items.length;f<k;f++)a.push(this.items[f].clone());
return a};v.toString=function(){return"Snap\u2018s set"};v.type="set";a.set=function(){var a=new z;arguments.length&&a.push.apply(a,Array.prototype.slice.call(arguments,0));return a}});C.plugin(function(a,v,y,C){function A(a){var b=a[0];switch(b.toLowerCase()){case "t":return[b,0,0];case "m":return[b,1,0,0,1,0,0];case "r":return 4==a.length?[b,0,a[2],a[3] ]:[b,0];case "s":return 5==a.length?[b,1,1,a[3],a[4] ]:3==a.length?[b,1,1]:[b,1]}}function w(b,d,f){d=q(d).replace(/\.{3}|\u2026/g,b);b=a.parseTransformString(b)||
[];d=a.parseTransformString(d)||[];for(var k=Math.max(b.length,d.length),p=[],v=[],h=0,w,z,y,I;h<k;h++){y=b[h]||A(d[h]);I=d[h]||A(y);if(y[0]!=I[0]||"r"==y[0].toLowerCase()&&(y[2]!=I[2]||y[3]!=I[3])||"s"==y[0].toLowerCase()&&(y[3]!=I[3]||y[4]!=I[4])){b=a._.transform2matrix(b,f());d=a._.transform2matrix(d,f());p=[["m",b.a,b.b,b.c,b.d,b.e,b.f] ];v=[["m",d.a,d.b,d.c,d.d,d.e,d.f] ];break}p[h]=[];v[h]=[];w=0;for(z=Math.max(y.length,I.length);w<z;w++)w in y&&(p[h][w]=y[w]),w in I&&(v[h][w]=I[w])}return{from:u(p),
to:u(v),f:n(p)}}function z(a){return a}function d(a){return function(b){return+b.toFixed(3)+a}}function f(b){return a.rgb(b[0],b[1],b[2])}function n(a){var b=0,d,f,k,n,h,p,q=[];d=0;for(f=a.length;d<f;d++){h="[";p=['"'+a[d][0]+'"'];k=1;for(n=a[d].length;k<n;k++)p[k]="val["+b++ +"]";h+=p+"]";q[d]=h}return Function("val","return Snap.path.toString.call(["+q+"])")}function u(a){for(var b=[],d=0,f=a.length;d<f;d++)for(var k=1,n=a[d].length;k<n;k++)b.push(a[d][k]);return b}var p={},b=/[a-z]+$/i,q=String;
p.stroke=p.fill="colour";v.prototype.equal=function(a,b){return k("snap.util.equal",this,a,b).firstDefined()};k.on("snap.util.equal",function(e,k){var r,s;r=q(this.attr(e)||"");var x=this;if(r==+r&&k==+k)return{from:+r,to:+k,f:z};if("colour"==p[e])return r=a.color(r),s=a.color(k),{from:[r.r,r.g,r.b,r.opacity],to:[s.r,s.g,s.b,s.opacity],f:f};if("transform"==e||"gradientTransform"==e||"patternTransform"==e)return k instanceof a.Matrix&&(k=k.toTransformString()),a._.rgTransform.test(k)||(k=a._.svgTransform2string(k)),
w(r,k,function(){return x.getBBox(1)});if("d"==e||"path"==e)return r=a.path.toCubic(r,k),{from:u(r[0]),to:u(r[1]),f:n(r[0])};if("points"==e)return r=q(r).split(a._.separator),s=q(k).split(a._.separator),{from:r,to:s,f:function(a){return a}};aUnit=r.match(b);s=q(k).match(b);return aUnit&&aUnit==s?{from:parseFloat(r),to:parseFloat(k),f:d(aUnit)}:{from:this.asPX(e),to:this.asPX(e,k),f:z}})});C.plugin(function(a,v,y,C){var A=v.prototype,w="createTouch"in C.doc;v="click dblclick mousedown mousemove mouseout mouseover mouseup touchstart touchmove touchend touchcancel".split(" ");
var z={mousedown:"touchstart",mousemove:"touchmove",mouseup:"touchend"},d=function(a,b){var d="y"==a?"scrollTop":"scrollLeft",e=b&&b.node?b.node.ownerDocument:C.doc;return e[d in e.documentElement?"documentElement":"body"][d]},f=function(){this.returnValue=!1},n=function(){return this.originalEvent.preventDefault()},u=function(){this.cancelBubble=!0},p=function(){return this.originalEvent.stopPropagation()},b=function(){if(C.doc.addEventListener)return function(a,b,e,f){var k=w&&z[b]?z[b]:b,l=function(k){var l=
d("y",f),q=d("x",f);if(w&&z.hasOwnProperty(b))for(var r=0,u=k.targetTouches&&k.targetTouches.length;r<u;r++)if(k.targetTouches[r].target==a||a.contains(k.targetTouches[r].target)){u=k;k=k.targetTouches[r];k.originalEvent=u;k.preventDefault=n;k.stopPropagation=p;break}return e.call(f,k,k.clientX+q,k.clientY+l)};b!==k&&a.addEventListener(b,l,!1);a.addEventListener(k,l,!1);return function(){b!==k&&a.removeEventListener(b,l,!1);a.removeEventListener(k,l,!1);return!0}};if(C.doc.attachEvent)return function(a,
b,e,h){var k=function(a){a=a||h.node.ownerDocument.window.event;var b=d("y",h),k=d("x",h),k=a.clientX+k,b=a.clientY+b;a.preventDefault=a.preventDefault||f;a.stopPropagation=a.stopPropagation||u;return e.call(h,a,k,b)};a.attachEvent("on"+b,k);return function(){a.detachEvent("on"+b,k);return!0}}}(),q=[],e=function(a){for(var b=a.clientX,e=a.clientY,f=d("y"),l=d("x"),n,p=q.length;p--;){n=q[p];if(w)for(var r=a.touches&&a.touches.length,u;r--;){if(u=a.touches[r],u.identifier==n.el._drag.id||n.el.node.contains(u.target)){b=
u.clientX;e=u.clientY;(a.originalEvent?a.originalEvent:a).preventDefault();break}}else a.preventDefault();b+=l;e+=f;k("snap.drag.move."+n.el.id,n.move_scope||n.el,b-n.el._drag.x,e-n.el._drag.y,b,e,a)}},l=function(b){a.unmousemove(e).unmouseup(l);for(var d=q.length,f;d--;)f=q[d],f.el._drag={},k("snap.drag.end."+f.el.id,f.end_scope||f.start_scope||f.move_scope||f.el,b);q=[]};for(y=v.length;y--;)(function(d){a[d]=A[d]=function(e,f){a.is(e,"function")&&(this.events=this.events||[],this.events.push({name:d,
f:e,unbind:b(this.node||document,d,e,f||this)}));return this};a["un"+d]=A["un"+d]=function(a){for(var b=this.events||[],e=b.length;e--;)if(b[e].name==d&&(b[e].f==a||!a)){b[e].unbind();b.splice(e,1);!b.length&&delete this.events;break}return this}})(v[y]);A.hover=function(a,b,d,e){return this.mouseover(a,d).mouseout(b,e||d)};A.unhover=function(a,b){return this.unmouseover(a).unmouseout(b)};var r=[];A.drag=function(b,d,f,h,n,p){function u(r,v,w){(r.originalEvent||r).preventDefault();this._drag.x=v;
this._drag.y=w;this._drag.id=r.identifier;!q.length&&a.mousemove(e).mouseup(l);q.push({el:this,move_scope:h,start_scope:n,end_scope:p});d&&k.on("snap.drag.start."+this.id,d);b&&k.on("snap.drag.move."+this.id,b);f&&k.on("snap.drag.end."+this.id,f);k("snap.drag.start."+this.id,n||h||this,v,w,r)}if(!arguments.length){var v;return this.drag(function(a,b){this.attr({transform:v+(v?"T":"t")+[a,b]})},function(){v=this.transform().local})}this._drag={};r.push({el:this,start:u});this.mousedown(u);return this};
A.undrag=function(){for(var b=r.length;b--;)r[b].el==this&&(this.unmousedown(r[b].start),r.splice(b,1),k.unbind("snap.drag.*."+this.id));!r.length&&a.unmousemove(e).unmouseup(l);return this}});C.plugin(function(a,v,y,C){y=y.prototype;var A=/^\s*url\((.+)\)/,w=String,z=a._.$;a.filter={};y.filter=function(d){var f=this;"svg"!=f.type&&(f=f.paper);d=a.parse(w(d));var k=a._.id(),u=z("filter");z(u,{id:k,filterUnits:"userSpaceOnUse"});u.appendChild(d.node);f.defs.appendChild(u);return new v(u)};k.on("snap.util.getattr.filter",
function(){k.stop();var d=z(this.node,"filter");if(d)return(d=w(d).match(A))&&a.select(d[1])});k.on("snap.util.attr.filter",function(d){if(d instanceof v&&"filter"==d.type){k.stop();var f=d.node.id;f||(z(d.node,{id:d.id}),f=d.id);z(this.node,{filter:a.url(f)})}d&&"none"!=d||(k.stop(),this.node.removeAttribute("filter"))});a.filter.blur=function(d,f){null==d&&(d=2);return a.format('<feGaussianBlur stdDeviation="{def}"/>',{def:null==f?d:[d,f]})};a.filter.blur.toString=function(){return this()};a.filter.shadow=
function(d,f,k,u,p){"string"==typeof k&&(p=u=k,k=4);"string"!=typeof u&&(p=u,u="#000");null==k&&(k=4);null==p&&(p=1);null==d&&(d=0,f=2);null==f&&(f=d);u=a.color(u||"#000");return a.format('<feGaussianBlur in="SourceAlpha" stdDeviation="{blur}"/><feOffset dx="{dx}" dy="{dy}" result="offsetblur"/><feFlood flood-color="{color}"/><feComposite in2="offsetblur" operator="in"/><feComponentTransfer><feFuncA type="linear" slope="{opacity}"/></feComponentTransfer><feMerge><feMergeNode/><feMergeNode in="SourceGraphic"/></feMerge>',
{color:u,dx:d,dy:f,blur:k,opacity:p})};a.filter.shadow.toString=function(){return this()};a.filter.grayscale=function(d){null==d&&(d=1);return a.format('<feColorMatrix type="matrix" values="{a} {b} {c} 0 0 {d} {e} {f} 0 0 {g} {b} {h} 0 0 0 0 0 1 0"/>',{a:0.2126+0.7874*(1-d),b:0.7152-0.7152*(1-d),c:0.0722-0.0722*(1-d),d:0.2126-0.2126*(1-d),e:0.7152+0.2848*(1-d),f:0.0722-0.0722*(1-d),g:0.2126-0.2126*(1-d),h:0.0722+0.9278*(1-d)})};a.filter.grayscale.toString=function(){return this()};a.filter.sepia=
function(d){null==d&&(d=1);return a.format('<feColorMatrix type="matrix" values="{a} {b} {c} 0 0 {d} {e} {f} 0 0 {g} {h} {i} 0 0 0 0 0 1 0"/>',{a:0.393+0.607*(1-d),b:0.769-0.769*(1-d),c:0.189-0.189*(1-d),d:0.349-0.349*(1-d),e:0.686+0.314*(1-d),f:0.168-0.168*(1-d),g:0.272-0.272*(1-d),h:0.534-0.534*(1-d),i:0.131+0.869*(1-d)})};a.filter.sepia.toString=function(){return this()};a.filter.saturate=function(d){null==d&&(d=1);return a.format('<feColorMatrix type="saturate" values="{amount}"/>',{amount:1-
d})};a.filter.saturate.toString=function(){return this()};a.filter.hueRotate=function(d){return a.format('<feColorMatrix type="hueRotate" values="{angle}"/>',{angle:d||0})};a.filter.hueRotate.toString=function(){return this()};a.filter.invert=function(d){null==d&&(d=1);return a.format('<feComponentTransfer><feFuncR type="table" tableValues="{amount} {amount2}"/><feFuncG type="table" tableValues="{amount} {amount2}"/><feFuncB type="table" tableValues="{amount} {amount2}"/></feComponentTransfer>',{amount:d,
amount2:1-d})};a.filter.invert.toString=function(){return this()};a.filter.brightness=function(d){null==d&&(d=1);return a.format('<feComponentTransfer><feFuncR type="linear" slope="{amount}"/><feFuncG type="linear" slope="{amount}"/><feFuncB type="linear" slope="{amount}"/></feComponentTransfer>',{amount:d})};a.filter.brightness.toString=function(){return this()};a.filter.contrast=function(d){null==d&&(d=1);return a.format('<feComponentTransfer><feFuncR type="linear" slope="{amount}" intercept="{amount2}"/><feFuncG type="linear" slope="{amount}" intercept="{amount2}"/><feFuncB type="linear" slope="{amount}" intercept="{amount2}"/></feComponentTransfer>',
{amount:d,amount2:0.5-d/2})};a.filter.contrast.toString=function(){return this()}});return C});

]]> </script>
<script> <![CDATA[

(function (glob, factory) {
    // AMD support
    if (typeof define === "function" && define.amd) {
        // Define as an anonymous module
        define("Gadfly", ["Snap.svg"], function (Snap) {
            return factory(Snap);
        });
    } else {
        // Browser globals (glob is window)
        // Snap adds itself to window
        glob.Gadfly = factory(glob.Snap);
    }
}(this, function (Snap) {

var Gadfly = {};

// Get an x/y coordinate value in pixels
var xPX = function(fig, x) {
    var client_box = fig.node.getBoundingClientRect();
    return x * fig.node.viewBox.baseVal.width / client_box.width;
};

var yPX = function(fig, y) {
    var client_box = fig.node.getBoundingClientRect();
    return y * fig.node.viewBox.baseVal.height / client_box.height;
};


Snap.plugin(function (Snap, Element, Paper, global) {
    // Traverse upwards from a snap element to find and return the first
    // note with the "plotroot" class.
    Element.prototype.plotroot = function () {
        var element = this;
        while (!element.hasClass("plotroot") && element.parent() != null) {
            element = element.parent();
        }
        return element;
    };

    Element.prototype.svgroot = function () {
        var element = this;
        while (element.node.nodeName != "svg" && element.parent() != null) {
            element = element.parent();
        }
        return element;
    };

    Element.prototype.plotbounds = function () {
        var root = this.plotroot()
        var bbox = root.select(".guide.background").node.getBBox();
        return {
            x0: bbox.x,
            x1: bbox.x + bbox.width,
            y0: bbox.y,
            y1: bbox.y + bbox.height
        };
    };

    Element.prototype.plotcenter = function () {
        var root = this.plotroot()
        var bbox = root.select(".guide.background").node.getBBox();
        return {
            x: bbox.x + bbox.width / 2,
            y: bbox.y + bbox.height / 2
        };
    };

    // Emulate IE style mouseenter/mouseleave events, since Microsoft always
    // does everything right.
    // See: http://www.dynamic-tools.net/toolbox/isMouseLeaveOrEnter/
    var events = ["mouseenter", "mouseleave"];

    for (i in events) {
        (function (event_name) {
            var event_name = events[i];
            Element.prototype[event_name] = function (fn, scope) {
                if (Snap.is(fn, "function")) {
                    var fn2 = function (event) {
                        if (event.type != "mouseover" && event.type != "mouseout") {
                            return;
                        }

                        var reltg = event.relatedTarget ? event.relatedTarget :
                            event.type == "mouseout" ? event.toElement : event.fromElement;
                        while (reltg && reltg != this.node) reltg = reltg.parentNode;

                        if (reltg != this.node) {
                            return fn.apply(this, event);
                        }
                    };

                    if (event_name == "mouseenter") {
                        this.mouseover(fn2, scope);
                    } else {
                        this.mouseout(fn2, scope);
                    }
                }
                return this;
            };
        })(events[i]);
    }


    Element.prototype.mousewheel = function (fn, scope) {
        if (Snap.is(fn, "function")) {
            var el = this;
            var fn2 = function (event) {
                fn.apply(el, [event]);
            };
        }

        this.node.addEventListener(
            /Firefox/i.test(navigator.userAgent) ? "DOMMouseScroll" : "mousewheel",
            fn2);

        return this;
    };


    // Snap's attr function can be too slow for things like panning/zooming.
    // This is a function to directly update element attributes without going
    // through eve.
    Element.prototype.attribute = function(key, val) {
        if (val === undefined) {
            return this.node.getAttribute(key);
        } else {
            this.node.setAttribute(key, val);
            return this;
        }
    };

    Element.prototype.init_gadfly = function() {
        this.mouseenter(Gadfly.plot_mouseover)
            .mouseleave(Gadfly.plot_mouseout)
            .dblclick(Gadfly.plot_dblclick)
            .mousewheel(Gadfly.guide_background_scroll)
            .drag(Gadfly.guide_background_drag_onmove,
                  Gadfly.guide_background_drag_onstart,
                  Gadfly.guide_background_drag_onend);
        this.mouseenter(function (event) {
            init_pan_zoom(this.plotroot());
        });
        return this;
    };
});


// When the plot is moused over, emphasize the grid lines.
Gadfly.plot_mouseover = function(event) {
    var root = this.plotroot();

    var keyboard_zoom = function(event) {
        if (event.which == 187) { // plus
            increase_zoom_by_position(root, 0.1, true);
        } else if (event.which == 189) { // minus
            increase_zoom_by_position(root, -0.1, true);
        }
    };
    root.data("keyboard_zoom", keyboard_zoom);
    window.addEventListener("keyup", keyboard_zoom);

    var xgridlines = root.select(".xgridlines"),
        ygridlines = root.select(".ygridlines");

    xgridlines.data("unfocused_strokedash",
                    xgridlines.attribute("stroke-dasharray").replace(/(\d)(,|$)/g, "$1mm$2"));
    ygridlines.data("unfocused_strokedash",
                    ygridlines.attribute("stroke-dasharray").replace(/(\d)(,|$)/g, "$1mm$2"));

    // emphasize grid lines
    var destcolor = root.data("focused_xgrid_color");
    xgridlines.attribute("stroke-dasharray", "none")
              .selectAll("path")
              .animate({stroke: destcolor}, 250);

    destcolor = root.data("focused_ygrid_color");
    ygridlines.attribute("stroke-dasharray", "none")
              .selectAll("path")
              .animate({stroke: destcolor}, 250);

    // reveal zoom slider
    root.select(".zoomslider")
        .animate({opacity: 1.0}, 250);
};

// Reset pan and zoom on double click
Gadfly.plot_dblclick = function(event) {
  set_plot_pan_zoom(this.plotroot(), 0.0, 0.0, 1.0);
};

// Unemphasize grid lines on mouse out.
Gadfly.plot_mouseout = function(event) {
    var root = this.plotroot();

    window.removeEventListener("keyup", root.data("keyboard_zoom"));
    root.data("keyboard_zoom", undefined);

    var xgridlines = root.select(".xgridlines"),
        ygridlines = root.select(".ygridlines");

    var destcolor = root.data("unfocused_xgrid_color");

    xgridlines.attribute("stroke-dasharray", xgridlines.data("unfocused_strokedash"))
              .selectAll("path")
              .animate({stroke: destcolor}, 250);

    destcolor = root.data("unfocused_ygrid_color");
    ygridlines.attribute("stroke-dasharray", ygridlines.data("unfocused_strokedash"))
              .selectAll("path")
              .animate({stroke: destcolor}, 250);

    // hide zoom slider
    root.select(".zoomslider")
        .animate({opacity: 0.0}, 250);
};


var set_geometry_transform = function(root, tx, ty, scale) {
    var xscalable = root.hasClass("xscalable"),
        yscalable = root.hasClass("yscalable");

    var old_scale = root.data("scale");

    var xscale = xscalable ? scale : 1.0,
        yscale = yscalable ? scale : 1.0;

    tx = xscalable ? tx : 0.0;
    ty = yscalable ? ty : 0.0;

    var t = new Snap.Matrix().translate(tx, ty).scale(xscale, yscale);

    root.selectAll(".geometry, image")
        .forEach(function (element, i) {
            element.transform(t);
        });

    bounds = root.plotbounds();

    if (yscalable) {
        var xfixed_t = new Snap.Matrix().translate(0, ty).scale(1.0, yscale);
        root.selectAll(".xfixed")
            .forEach(function (element, i) {
                element.transform(xfixed_t);
            });

        root.select(".ylabels")
            .transform(xfixed_t)
            .selectAll("text")
            .forEach(function (element, i) {
                if (element.attribute("gadfly:inscale") == "true") {
                    var cx = element.asPX("x"),
                        cy = element.asPX("y");
                    var st = element.data("static_transform");
                    unscale_t = new Snap.Matrix();
                    unscale_t.scale(1, 1/scale, cx, cy).add(st);
                    element.transform(unscale_t);

                    var y = cy * scale + ty;
                    element.attr("visibility",
                        bounds.y0 <= y && y <= bounds.y1 ? "visible" : "hidden");
                }
            });
    }

    if (xscalable) {
        var yfixed_t = new Snap.Matrix().translate(tx, 0).scale(xscale, 1.0);
        var xtrans = new Snap.Matrix().translate(tx, 0);
        root.selectAll(".yfixed")
            .forEach(function (element, i) {
                element.transform(yfixed_t);
            });

        root.select(".xlabels")
            .transform(yfixed_t)
            .selectAll("text")
            .forEach(function (element, i) {
                if (element.attribute("gadfly:inscale") == "true") {
                    var cx = element.asPX("x"),
                        cy = element.asPX("y");
                    var st = element.data("static_transform");
                    unscale_t = new Snap.Matrix();
                    unscale_t.scale(1/scale, 1, cx, cy).add(st);

                    element.transform(unscale_t);

                    var x = cx * scale + tx;
                    element.attr("visibility",
                        bounds.x0 <= x && x <= bounds.x1 ? "visible" : "hidden");
                    }
            });
    }

    // we must unscale anything that is scale invariance: widths, raiduses, etc.
    var size_attribs = ["font-size"];
    var unscaled_selection = ".geometry, .geometry *";
    if (xscalable) {
        size_attribs.push("rx");
        unscaled_selection += ", .xgridlines";
    }
    if (yscalable) {
        size_attribs.push("ry");
        unscaled_selection += ", .ygridlines";
    }

    root.selectAll(unscaled_selection)
        .forEach(function (element, i) {
            // circle need special help
            if (element.node.nodeName == "circle") {
                var cx = element.attribute("cx"),
                    cy = element.attribute("cy");
                unscale_t = new Snap.Matrix().scale(1/xscale, 1/yscale,
                                                        cx, cy);
                element.transform(unscale_t);
                return;
            }

            for (i in size_attribs) {
                var key = size_attribs[i];
                var val = parseFloat(element.attribute(key));
                if (val !== undefined && val != 0 && !isNaN(val)) {
                    element.attribute(key, val * old_scale / scale);
                }
            }
        });
};


// Find the most appropriate tick scale and update label visibility.
var update_tickscale = function(root, scale, axis) {
    if (!root.hasClass(axis + "scalable")) return;

    var tickscales = root.data(axis + "tickscales");
    var best_tickscale = 1.0;
    var best_tickscale_dist = Infinity;
    for (tickscale in tickscales) {
        var dist = Math.abs(Math.log(tickscale) - Math.log(scale));
        if (dist < best_tickscale_dist) {
            best_tickscale_dist = dist;
            best_tickscale = tickscale;
        }
    }

    if (best_tickscale != root.data(axis + "tickscale")) {
        root.data(axis + "tickscale", best_tickscale);
        var mark_inscale_gridlines = function (element, i) {
            var inscale = element.attr("gadfly:scale") == best_tickscale;
            element.attribute("gadfly:inscale", inscale);
            element.attr("visibility", inscale ? "visible" : "hidden");
        };

        var mark_inscale_labels = function (element, i) {
            var inscale = element.attr("gadfly:scale") == best_tickscale;
            element.attribute("gadfly:inscale", inscale);
            element.attr("visibility", inscale ? "visible" : "hidden");
        };

        root.select("." + axis + "gridlines").selectAll("path").forEach(mark_inscale_gridlines);
        root.select("." + axis + "labels").selectAll("text").forEach(mark_inscale_labels);
    }
};


var set_plot_pan_zoom = function(root, tx, ty, scale) {
    var old_scale = root.data("scale");
    var bounds = root.plotbounds();

    var width = bounds.x1 - bounds.x0,
        height = bounds.y1 - bounds.y0;

    // compute the viewport derived from tx, ty, and scale
    var x_min = -width * scale - (scale * width - width),
        x_max = width * scale,
        y_min = -height * scale - (scale * height - height),
        y_max = height * scale;

    var x0 = bounds.x0 - scale * bounds.x0,
        y0 = bounds.y0 - scale * bounds.y0;

    var tx = Math.max(Math.min(tx - x0, x_max), x_min),
        ty = Math.max(Math.min(ty - y0, y_max), y_min);

    tx += x0;
    ty += y0;

    // when the scale change, we may need to alter which set of
    // ticks is being displayed
    if (scale != old_scale) {
        update_tickscale(root, scale, "x");
        update_tickscale(root, scale, "y");
    }

    set_geometry_transform(root, tx, ty, scale);

    root.data("scale", scale);
    root.data("tx", tx);
    root.data("ty", ty);
};


var scale_centered_translation = function(root, scale) {
    var bounds = root.plotbounds();

    var width = bounds.x1 - bounds.x0,
        height = bounds.y1 - bounds.y0;

    var tx0 = root.data("tx"),
        ty0 = root.data("ty");

    var scale0 = root.data("scale");

    // how off from center the current view is
    var xoff = tx0 - (bounds.x0 * (1 - scale0) + (width * (1 - scale0)) / 2),
        yoff = ty0 - (bounds.y0 * (1 - scale0) + (height * (1 - scale0)) / 2);

    // rescale offsets
    xoff = xoff * scale / scale0;
    yoff = yoff * scale / scale0;

    // adjust for the panel position being scaled
    var x_edge_adjust = bounds.x0 * (1 - scale),
        y_edge_adjust = bounds.y0 * (1 - scale);

    return {
        x: xoff + x_edge_adjust + (width - width * scale) / 2,
        y: yoff + y_edge_adjust + (height - height * scale) / 2
    };
};


// Initialize data for panning zooming if it isn't already.
var init_pan_zoom = function(root) {
    if (root.data("zoompan-ready")) {
        return;
    }

    // The non-scaling-stroke trick. Rather than try to correct for the
    // stroke-width when zooming, we force it to a fixed value.
    var px_per_mm = root.node.getCTM().a;

    // Drag events report deltas in pixels, which we'd like to convert to
    // millimeters.
    root.data("px_per_mm", px_per_mm);

    root.selectAll("path")
        .forEach(function (element, i) {
        sw = element.asPX("stroke-width") * px_per_mm;
        if (sw > 0) {
            element.attribute("stroke-width", sw);
            element.attribute("vector-effect", "non-scaling-stroke");
        }
    });

    // Store ticks labels original tranformation
    root.selectAll(".xlabels > text, .ylabels > text")
        .forEach(function (element, i) {
            var lm = element.transform().localMatrix;
            element.data("static_transform",
                new Snap.Matrix(lm.a, lm.b, lm.c, lm.d, lm.e, lm.f));
        });

    var xgridlines = root.select(".xgridlines");
    var ygridlines = root.select(".ygridlines");
    var xlabels = root.select(".xlabels");
    var ylabels = root.select(".ylabels");

    if (root.data("tx") === undefined) root.data("tx", 0);
    if (root.data("ty") === undefined) root.data("ty", 0);
    if (root.data("scale") === undefined) root.data("scale", 1.0);
    if (root.data("xtickscales") === undefined) {

        // index all the tick scales that are listed
        var xtickscales = {};
        var ytickscales = {};
        var add_x_tick_scales = function (element, i) {
            xtickscales[element.attribute("gadfly:scale")] = true;
        };
        var add_y_tick_scales = function (element, i) {
            ytickscales[element.attribute("gadfly:scale")] = true;
        };

        if (xgridlines) xgridlines.selectAll("path").forEach(add_x_tick_scales);
        if (ygridlines) ygridlines.selectAll("path").forEach(add_y_tick_scales);
        if (xlabels) xlabels.selectAll("text").forEach(add_x_tick_scales);
        if (ylabels) ylabels.selectAll("text").forEach(add_y_tick_scales);

        root.data("xtickscales", xtickscales);
        root.data("ytickscales", ytickscales);
        root.data("xtickscale", 1.0);
    }

    var min_scale = 1.0, max_scale = 1.0;
    for (scale in xtickscales) {
        min_scale = Math.min(min_scale, scale);
        max_scale = Math.max(max_scale, scale);
    }
    for (scale in ytickscales) {
        min_scale = Math.min(min_scale, scale);
        max_scale = Math.max(max_scale, scale);
    }
    root.data("min_scale", min_scale);
    root.data("max_scale", max_scale);

    // store the original positions of labels
    if (xlabels) {
        xlabels.selectAll("text")
               .forEach(function (element, i) {
                   element.data("x", element.asPX("x"));
               });
    }

    if (ylabels) {
        ylabels.selectAll("text")
               .forEach(function (element, i) {
                   element.data("y", element.asPX("y"));
               });
    }

    // mark grid lines and ticks as in or out of scale.
    var mark_inscale = function (element, i) {
        element.attribute("gadfly:inscale", element.attribute("gadfly:scale") == 1.0);
    };

    if (xgridlines) xgridlines.selectAll("path").forEach(mark_inscale);
    if (ygridlines) ygridlines.selectAll("path").forEach(mark_inscale);
    if (xlabels) xlabels.selectAll("text").forEach(mark_inscale);
    if (ylabels) ylabels.selectAll("text").forEach(mark_inscale);

    // figure out the upper ond lower bounds on panning using the maximum
    // and minum grid lines
    var bounds = root.plotbounds();
    var pan_bounds = {
        x0: 0.0,
        y0: 0.0,
        x1: 0.0,
        y1: 0.0
    };

    if (xgridlines) {
        xgridlines
            .selectAll("path")
            .forEach(function (element, i) {
                if (element.attribute("gadfly:inscale") == "true") {
                    var bbox = element.node.getBBox();
                    if (bounds.x1 - bbox.x < pan_bounds.x0) {
                        pan_bounds.x0 = bounds.x1 - bbox.x;
                    }
                    if (bounds.x0 - bbox.x > pan_bounds.x1) {
                        pan_bounds.x1 = bounds.x0 - bbox.x;
                    }
                    element.attr("visibility", "visible");
                }
            });
    }

    if (ygridlines) {
        ygridlines
            .selectAll("path")
            .forEach(function (element, i) {
                if (element.attribute("gadfly:inscale") == "true") {
                    var bbox = element.node.getBBox();
                    if (bounds.y1 - bbox.y < pan_bounds.y0) {
                        pan_bounds.y0 = bounds.y1 - bbox.y;
                    }
                    if (bounds.y0 - bbox.y > pan_bounds.y1) {
                        pan_bounds.y1 = bounds.y0 - bbox.y;
                    }
                    element.attr("visibility", "visible");
                }
            });
    }

    // nudge these values a little
    pan_bounds.x0 -= 5;
    pan_bounds.x1 += 5;
    pan_bounds.y0 -= 5;
    pan_bounds.y1 += 5;
    root.data("pan_bounds", pan_bounds);

    root.data("zoompan-ready", true)
};


// drag actions, i.e. zooming and panning
var pan_action = {
    start: function(root, x, y, event) {
        root.data("dx", 0);
        root.data("dy", 0);
        root.data("tx0", root.data("tx"));
        root.data("ty0", root.data("ty"));
    },
    update: function(root, dx, dy, x, y, event) {
        var px_per_mm = root.data("px_per_mm");
        dx /= px_per_mm;
        dy /= px_per_mm;

        var tx0 = root.data("tx"),
            ty0 = root.data("ty");

        var dx0 = root.data("dx"),
            dy0 = root.data("dy");

        root.data("dx", dx);
        root.data("dy", dy);

        dx = dx - dx0;
        dy = dy - dy0;

        var tx = tx0 + dx,
            ty = ty0 + dy;

        set_plot_pan_zoom(root, tx, ty, root.data("scale"));
    },
    end: function(root, event) {

    },
    cancel: function(root) {
        set_plot_pan_zoom(root, root.data("tx0"), root.data("ty0"), root.data("scale"));
    }
};

var zoom_box;
var zoom_action = {
    start: function(root, x, y, event) {
        var bounds = root.plotbounds();
        var width = bounds.x1 - bounds.x0,
            height = bounds.y1 - bounds.y0;
        var ratio = width / height;
        var xscalable = root.hasClass("xscalable"),
            yscalable = root.hasClass("yscalable");
        var px_per_mm = root.data("px_per_mm");
        x = xscalable ? x / px_per_mm : bounds.x0;
        y = yscalable ? y / px_per_mm : bounds.y0;
        var w = xscalable ? 0 : width;
        var h = yscalable ? 0 : height;
        zoom_box = root.rect(x, y, w, h).attr({
            "fill": "#000",
            "opacity": 0.25
        });
        zoom_box.data("ratio", ratio);
    },
    update: function(root, dx, dy, x, y, event) {
        var xscalable = root.hasClass("xscalable"),
            yscalable = root.hasClass("yscalable");
        var px_per_mm = root.data("px_per_mm");
        var bounds = root.plotbounds();
        if (yscalable) {
            y /= px_per_mm;
            y = Math.max(bounds.y0, y);
            y = Math.min(bounds.y1, y);
        } else {
            y = bounds.y1;
        }
        if (xscalable) {
            x /= px_per_mm;
            x = Math.max(bounds.x0, x);
            x = Math.min(bounds.x1, x);
        } else {
            x = bounds.x1;
        }

        dx = x - zoom_box.attr("x");
        dy = y - zoom_box.attr("y");
        if (xscalable && yscalable) {
            var ratio = zoom_box.data("ratio");
            var width = Math.min(Math.abs(dx), ratio * Math.abs(dy));
            var height = Math.min(Math.abs(dy), Math.abs(dx) / ratio);
            dx = width * dx / Math.abs(dx);
            dy = height * dy / Math.abs(dy);
        }
        var xoffset = 0,
            yoffset = 0;
        if (dx < 0) {
            xoffset = dx;
            dx = -1 * dx;
        }
        if (dy < 0) {
            yoffset = dy;
            dy = -1 * dy;
        }
        if (isNaN(dy)) {
            dy = 0.0;
        }
        if (isNaN(dx)) {
            dx = 0.0;
        }
        zoom_box.transform("T" + xoffset + "," + yoffset);
        zoom_box.attr("width", dx);
        zoom_box.attr("height", dy);
    },
    end: function(root, event) {
        var xscalable = root.hasClass("xscalable"),
            yscalable = root.hasClass("yscalable");
        var zoom_bounds = zoom_box.getBBox();
        if (zoom_bounds.width * zoom_bounds.height <= 0) {
            return;
        }
        var plot_bounds = root.plotbounds();
        var zoom_factor = 1.0;
        if (yscalable) {
            zoom_factor = (plot_bounds.y1 - plot_bounds.y0) / zoom_bounds.height;
        } else {
            zoom_factor = (plot_bounds.x1 - plot_bounds.x0) / zoom_bounds.width;
        }
        var tx = (root.data("tx") - zoom_bounds.x) * zoom_factor + plot_bounds.x0,
            ty = (root.data("ty") - zoom_bounds.y) * zoom_factor + plot_bounds.y0;
        set_plot_pan_zoom(root, tx, ty, root.data("scale") * zoom_factor);
        zoom_box.remove();
    },
    cancel: function(root) {
        zoom_box.remove();
    }
};


Gadfly.guide_background_drag_onstart = function(x, y, event) {
    var root = this.plotroot();
    var scalable = root.hasClass("xscalable") || root.hasClass("yscalable");
    var zoomable = !event.altKey && !event.ctrlKey && event.shiftKey && scalable;
    var panable = !event.altKey && !event.ctrlKey && !event.shiftKey && scalable;
    var drag_action = zoomable ? zoom_action :
                      panable  ? pan_action :
                                 undefined;
    root.data("drag_action", drag_action);
    if (drag_action) {
        var cancel_drag_action = function(event) {
            if (event.which == 27) { // esc key
                drag_action.cancel(root);
                root.data("drag_action", undefined);
            }
        };
        window.addEventListener("keyup", cancel_drag_action);
        root.data("cancel_drag_action", cancel_drag_action);
        drag_action.start(root, x, y, event);
    }
};


Gadfly.guide_background_drag_onmove = function(dx, dy, x, y, event) {
    var root = this.plotroot();
    var drag_action = root.data("drag_action");
    if (drag_action) {
        drag_action.update(root, dx, dy, x, y, event);
    }
};


Gadfly.guide_background_drag_onend = function(event) {
    var root = this.plotroot();
    window.removeEventListener("keyup", root.data("cancel_drag_action"));
    root.data("cancel_drag_action", undefined);
    var drag_action = root.data("drag_action");
    if (drag_action) {
        drag_action.end(root, event);
    }
    root.data("drag_action", undefined);
};


Gadfly.guide_background_scroll = function(event) {
    if (event.shiftKey) {
        increase_zoom_by_position(this.plotroot(), 0.001 * event.wheelDelta);
        event.preventDefault();
    }
};


Gadfly.zoomslider_button_mouseover = function(event) {
    this.select(".button_logo")
         .animate({fill: this.data("mouseover_color")}, 100);
};


Gadfly.zoomslider_button_mouseout = function(event) {
     this.select(".button_logo")
         .animate({fill: this.data("mouseout_color")}, 100);
};


Gadfly.zoomslider_zoomout_click = function(event) {
    increase_zoom_by_position(this.plotroot(), -0.1, true);
};


Gadfly.zoomslider_zoomin_click = function(event) {
    increase_zoom_by_position(this.plotroot(), 0.1, true);
};


Gadfly.zoomslider_track_click = function(event) {
    // TODO
};


// Map slider position x to scale y using the function y = a*exp(b*x)+c.
// The constants a, b, and c are solved using the constraint that the function
// should go through the points (0; min_scale), (0.5; 1), and (1; max_scale).
var scale_from_slider_position = function(position, min_scale, max_scale) {
    var a = (1 - 2 * min_scale + min_scale * min_scale) / (min_scale + max_scale - 2),
        b = 2 * Math.log((max_scale - 1) / (1 - min_scale)),
        c = (min_scale * max_scale - 1) / (min_scale + max_scale - 2);
    return a * Math.exp(b * position) + c;
}

// inverse of scale_from_slider_position
var slider_position_from_scale = function(scale, min_scale, max_scale) {
    var a = (1 - 2 * min_scale + min_scale * min_scale) / (min_scale + max_scale - 2),
        b = 2 * Math.log((max_scale - 1) / (1 - min_scale)),
        c = (min_scale * max_scale - 1) / (min_scale + max_scale - 2);
    return 1 / b * Math.log((scale - c) / a);
}

var increase_zoom_by_position = function(root, delta_position, animate) {
    var scale = root.data("scale"),
        min_scale = root.data("min_scale"),
        max_scale = root.data("max_scale");
    var position = slider_position_from_scale(scale, min_scale, max_scale);
    position += delta_position;
    scale = scale_from_slider_position(position, min_scale, max_scale);
    set_zoom(root, scale, animate);
}

var set_zoom = function(root, scale, animate) {
    var min_scale = root.data("min_scale"),
        max_scale = root.data("max_scale"),
        old_scale = root.data("scale");
    var new_scale = Math.max(min_scale, Math.min(scale, max_scale));
    if (animate) {
        Snap.animate(
            old_scale,
            new_scale,
            function (new_scale) {
                update_plot_scale(root, new_scale);
            },
            200);
    } else {
        update_plot_scale(root, new_scale);
    }
}


var update_plot_scale = function(root, new_scale) {
    var trans = scale_centered_translation(root, new_scale);
    set_plot_pan_zoom(root, trans.x, trans.y, new_scale);

    root.selectAll(".zoomslider_thumb")
        .forEach(function (element, i) {
            var min_pos = element.data("min_pos"),
                max_pos = element.data("max_pos"),
                min_scale = root.data("min_scale"),
                max_scale = root.data("max_scale");
            var xmid = (min_pos + max_pos) / 2;
            var xpos = slider_position_from_scale(new_scale, min_scale, max_scale);
            element.transform(new Snap.Matrix().translate(
                Math.max(min_pos, Math.min(
                         max_pos, min_pos + (max_pos - min_pos) * xpos)) - xmid, 0));
    });
};


Gadfly.zoomslider_thumb_dragmove = function(dx, dy, x, y, event) {
    var root = this.plotroot();
    var min_pos = this.data("min_pos"),
        max_pos = this.data("max_pos"),
        min_scale = root.data("min_scale"),
        max_scale = root.data("max_scale"),
        old_scale = root.data("old_scale");

    var px_per_mm = root.data("px_per_mm");
    dx /= px_per_mm;
    dy /= px_per_mm;

    var xmid = (min_pos + max_pos) / 2;
    var xpos = slider_position_from_scale(old_scale, min_scale, max_scale) +
                   dx / (max_pos - min_pos);

    // compute the new scale
    var new_scale = scale_from_slider_position(xpos, min_scale, max_scale);
    new_scale = Math.min(max_scale, Math.max(min_scale, new_scale));

    update_plot_scale(root, new_scale);
    event.stopPropagation();
};


Gadfly.zoomslider_thumb_dragstart = function(x, y, event) {
    this.animate({fill: this.data("mouseover_color")}, 100);
    var root = this.plotroot();

    // keep track of what the scale was when we started dragging
    root.data("old_scale", root.data("scale"));
    event.stopPropagation();
};


Gadfly.zoomslider_thumb_dragend = function(event) {
    this.animate({fill: this.data("mouseout_color")}, 100);
    event.stopPropagation();
};


var toggle_color_class = function(root, color_class, ison) {
    var guides = root.selectAll(".guide." + color_class + ",.guide ." + color_class);
    var geoms = root.selectAll(".geometry." + color_class + ",.geometry ." + color_class);
    if (ison) {
        guides.animate({opacity: 0.5}, 250);
        geoms.animate({opacity: 0.0}, 250);
    } else {
        guides.animate({opacity: 1.0}, 250);
        geoms.animate({opacity: 1.0}, 250);
    }
};


Gadfly.colorkey_swatch_click = function(event) {
    var root = this.plotroot();
    var color_class = this.data("color_class");

    if (event.shiftKey) {
        root.selectAll(".colorkey text")
            .forEach(function (element) {
                var other_color_class = element.data("color_class");
                if (other_color_class != color_class) {
                    toggle_color_class(root, other_color_class,
                                       element.attr("opacity") == 1.0);
                }
            });
    } else {
        toggle_color_class(root, color_class, this.attr("opacity") == 1.0);
    }
};


return Gadfly;

}));


//@ sourceURL=gadfly.js

(function (glob, factory) {
    // AMD support
      if (typeof require === "function" && typeof define === "function" && define.amd) {
        require(["Snap.svg", "Gadfly"], function (Snap, Gadfly) {
            factory(Snap, Gadfly);
        });
      } else {
          factory(glob.Snap, glob.Gadfly);
      }
})(window, function (Snap, Gadfly) {
    var fig = Snap("#img-a6536837");
fig.select("#img-a6536837-4")
   .drag(function() {}, function() {}, function() {});
fig.select("#img-a6536837-6")
   .data("color_class", "color_No")
.click(Gadfly.colorkey_swatch_click)
;
fig.select("#img-a6536837-7")
   .data("color_class", "color_Yes")
.click(Gadfly.colorkey_swatch_click)
;
fig.select("#img-a6536837-9")
   .data("color_class", "color_No")
.click(Gadfly.colorkey_swatch_click)
;
fig.select("#img-a6536837-10")
   .data("color_class", "color_Yes")
.click(Gadfly.colorkey_swatch_click)
;
fig.select("#img-a6536837-14")
   .init_gadfly();
fig.select("#img-a6536837-17")
   .plotroot().data("unfocused_ygrid_color", "#D0D0E0")
;
fig.select("#img-a6536837-17")
   .plotroot().data("focused_ygrid_color", "#A0A0A0")
;
fig.select("#img-a6536837-137")
   .plotroot().data("unfocused_xgrid_color", "#D0D0E0")
;
fig.select("#img-a6536837-137")
   .plotroot().data("focused_xgrid_color", "#A0A0A0")
;
fig.select("#img-a6536837-299")
   .data("mouseover_color", "#CD5C5C")
;
fig.select("#img-a6536837-299")
   .data("mouseout_color", "#6A6A6A")
;
fig.select("#img-a6536837-299")
   .click(Gadfly.zoomslider_zoomin_click)
.mouseenter(Gadfly.zoomslider_button_mouseover)
.mouseleave(Gadfly.zoomslider_button_mouseout)
;
fig.select("#img-a6536837-303")
   .data("max_pos", 100.09)
;
fig.select("#img-a6536837-303")
   .data("min_pos", 83.09)
;
fig.select("#img-a6536837-303")
   .click(Gadfly.zoomslider_track_click);
fig.select("#img-a6536837-305")
   .data("max_pos", 100.09)
;
fig.select("#img-a6536837-305")
   .data("min_pos", 83.09)
;
fig.select("#img-a6536837-305")
   .data("mouseover_color", "#CD5C5C")
;
fig.select("#img-a6536837-305")
   .data("mouseout_color", "#6A6A6A")
;
fig.select("#img-a6536837-305")
   .drag(Gadfly.zoomslider_thumb_dragmove,
     Gadfly.zoomslider_thumb_dragstart,
     Gadfly.zoomslider_thumb_dragend)
;
fig.select("#img-a6536837-307")
   .data("mouseover_color", "#CD5C5C")
;
fig.select("#img-a6536837-307")
   .data("mouseout_color", "#6A6A6A")
;
fig.select("#img-a6536837-307")
   .click(Gadfly.zoomslider_zoomout_click)
.mouseenter(Gadfly.zoomslider_button_mouseover)
.mouseleave(Gadfly.zoomslider_button_mouseout)
;
    });
]]> </script>
</svg>



