---
title: Luma devlog
---

# Luma devlog

Small games and graphics projects written from scratch in C++ and SDL3,
moving from SDL's 2D renderer to my own software and GPU renderers.
Source: [github.com/Grimleik/luma](https://github.com/Grimleik/luma).

## Posts

{% comment %}
Liquid (Jekyll's template language): lists every file in docs/_posts/,
newest first. relative_url adds the /luma/ prefix the site lives under.
{% endcomment %}
{% for post in site.posts %}
- {{ post.date | date: "%Y-%m-%d" }}: [{{ post.title }}]({{ post.url | relative_url }})
{% else %}
- Nothing yet. The first post comes with 00_window.
{% endfor %}
