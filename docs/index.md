---
layout: frontpage
title: A Verilog to C/C++ Compiler
---

{% assign glfwversion = site.tags.changelog.first.title %}
{% assign releasedate = site.tags.changelog.first.date %}

{% row %}

{% col 3-4 %}

**CModelGen** is a free and open source compiler that compiles
[synthesizable](synthesizable.html) Verilog to C or C++ code.

Using a cycle-based simulation technique, the
generated C or C++ code simulates the behavior of the Verilog design in a
much higher speed compared with common-seen Verilog simulators.

The most straight-forward usage of CModelGen is to build cycle-accurate
simulation models. Developing a cycle-accurate simulation models
manually is often tedious and error-prone.
CModelGen enables automatically generating C/C++ simulation models
that are both accuracy and ultra fast.
Especially, when a Verilog design is subject to modifications,
its simulation model can be regenerated to immediately catch up the updates.

CModelGen can also be used in other scenarios, such as accelarating hardware
developments and enabling secure distribution of evaluating IP.

CModelGen supports Windows, macOS and Linux.

{% endcol %}

{% col 1-4 %}

<br>

{% button https://github.com/glfw/glfw/releases/download/{{ glfwversion }}/glfw-{{ glfwversion }}.zip %}
Current Stable Release
<br>
<small>Version {{ glfwversion }}</small>
<br>
<small>Released on {% include time.html date=releasedate %}</small>
{% endbutton %}

{% button https://github.com/glfw/glfw/releases/download/{{ glfwversion }}/glfw-{{ glfwversion }}.zip %}
Current Preview Release
<br>
<small>Version {{ glfwversion }}</small>
<br>
<small>Released on {% include time.html date=releasedate %}</small>
{% endbutton %}

<br>

{% button https://github.com/glfw/glfw/releases/download/{{ glfwversion }}/glfw-{{ glfwversion }}.zip %}
Subscribe Feed
{% endbutton %}

{% endcol %}

{% endrow %}

<br/>

---------------------------------

## News

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

*See the [news archive](news.html) for older posts.*
