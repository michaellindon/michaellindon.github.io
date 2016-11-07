---
id: 1097
title: C++ Merge Sort Algorithm
date: 2015-01-05T18:55:09+00:00
author: admin
layout: post
guid: http://www.lindonslog.com/?p=1097
permalink: /linux-unix/c-merge-sort-algorithm/
categories:
  - Linux/Unix
  - Programming
tags:
  - algorithms
  - c
  - C++11
  - merge
  - mergesort
  - sort
  - sorting
---
## C++ Merge Sort Algorithm

<pre class="brush: cpp; title: ; notranslate" title="">#include &lt;random&gt;
#include &lt;iostream&gt;
#include &lt;vector&gt;
#include &lt;algorithm&gt;

using namespace std;

vector&lt;int&gt; merge_sort(const vector&lt;int&gt;& input)
{
	if(input.size()&lt;=1) return input;
	vector&lt;int&gt; output(input.size());

	//Split Vector//
	int midpoint=0.5*input.size();
	vector&lt;int&gt; input_left(input.begin(),input.begin()+midpoint);
	vector&lt;int&gt; input_right(input.begin()+midpoint,input.end());

	input_left=merge_sort(input_left);
	input_right=merge_sort(input_right);
	merge(input_left.begin(),input_left.end(),input_right.begin(),input_right.end(),output.begin());

	return output;
}

int main(){

	//Create unsorted vector of ints
	vector&lt;int&gt; unsorted(40);
	iota(unsorted.begin(),unsorted.end(),-20);
	shuffle(unsorted.begin(),unsorted.end(),default_random_engine());

	//Perform merge_sort//
	vector&lt;int&gt; sorted=merge_sort(unsorted);

	//Display results//
	cout &lt;&lt; "Unsorted: " &lt;&lt;  endl;
	for(auto value:unsorted)  cout &lt;&lt; value &lt;&lt; " ";
	cout &lt;&lt;  endl;
	cout &lt;&lt; "Sorted: " &lt;&lt;  endl;
	for(auto value:sorted)  cout &lt;&lt; value &lt;&lt; " ";
	cout &lt;&lt;  endl;

}
</pre>

### stdout

Unsorted:
  
-2 14 7 -15 -9 -17 -8 -1 13 1 -10 -7 16 -19 6 2 -12 -11 8 -18 -14 10 5 4 17 12 15 -16 -5 18 -4 -3 -6 -20 0 3 9 -13 11 19
  
Sorted:
  
-20 -19 -18 -17 -16 -15 -14 -13 -12 -11 -10 -9 -8 -7 -6 -5 -4 -3 -2 -1 0 1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 16 17 18 19