---
layout: frontpage
title: A Verilog to C/C++ Compiler
---

{% assign glfwversion = site.tags.changelog.first.title %}
{% assign releasedate = site.tags.changelog.first.date %}

{% row %}

{% col 2-3 %}

**CModelGen** is a free and open source compiler that compiles synthesizable
Verilog to C or C++ code.

Using a cycle-based simulation technique, the
generated C or C++ code simulates the behavior of the Verilog design in a
much higher speed compared with common-seen Verilog simulators.

The most straight-forward usage of CModelGen is to build cycle-accurate
simulators. Developing a cycle-accurate simulator manually is often tedious
and error-prone. CModelGen allows generating a C/C++ simulation model
automatically that has both a cycle-level accuracy and a high speed. When
a Verilog design is modified, the simulation model can be generated again
to immediately follow the updates.

CModelGen can also be used in other scenarios, such as accelarating hardware
developments and enabling secure distribution of evaluating IP.

CModelGen supports Windows, macOS and Linux.

CModelGen consists of a frontend and a backend. It uses a library called
[CModelSimLib](https://cmodelgen.github.io/cmodelsimlib_www)
to facilitate Verilog simulation. The frontend of CModelGen is extended from
the ICarus Verilog Simulator and is licensed under the [GNU Public License v3](gplv3.html).
The backend is licensed under the [MIT License](mitlic.html).
{% endcol %}

{% col 1-3 %}

{% button download.html %}
Getting Started
{% endbutton %}

{% button download.html %}
Documentation
{% endbutton %}

{% button download.html %}
Download CModelGen
{% endbutton %}

{% button download.html %}
Get Source Code
{% endbutton %}

{% button https://github.com/glfw/glfw/releases/download/{{ glfwversion }}/glfw-{{ glfwversion }}.zip %}
Download GLFW {{ glfwversion }}
<br>
<small>Released on {% include time.html date=releasedate %}</small>
{% endbutton %}

{% include milestone.html %}

{% endcol %}

{% endrow %}

<br/>

{% row %}

{% col 2-3 %}

{% include features.html %}

No library can be perfect for everyone.  If GLFW isn't what you're looking for,
there are
[alternatives](https://www.opengl.org/wiki/Related_toolkits_and_APIs).

{% endcol %}

{% col 1-3 %}

{% for post in site.tags.news limit:3 %}
<article>
<header>

<h3>{{ post.title }}</h3>
<small>
Posted on {% include time.html date=post.date %}
</small>

</header>

{{ post.content }}

</article>
{% endfor %}

See the [news archive](news.html) for older posts.

{% endcol %}

{% endrow %}
