A Node Editor in QT specifically designed for Rendering Pipelines

After trying a couple of node editors I quickly came to the conclusion that they did not support my use-cases and the easiest way out was to develop my own.
I am sharing this because maybe other people will find this useful.

Screenshot of a the pipeline editor in our demo tool:

Node Editor in QT

[![node_editor_screenshot.png](https://s33.postimg.cc/r57fbwnr3/node_editor_screenshot.png)](https://postimg.cc/image/l49qeu14r/)

Be wary! The example is currently broken but since no-one uses this I'm not going to fix it since it now depends heavily on my external project.
If you want to use this library and really want a working example raise an issue.

Problems currently: a lot of boilerplate needs to be written for a simple node editor to work, there's a lot of unoptimized stuff, and the
rendering part itself needs a lot of love (text rendering outside of nodes)

However, this editor has support for
- dynamic input output ports, there are API calls that allow you to tell the framework that the ports of a specific node have changes. The downside is
that all current connections will be destroyed since it's a pain to manage all that.
- custom widgets embedded into the node

This editor does not have support for
- automatic propagation, it requires you to recursively travel the node graph yourself, you can access the low level graph from the widget API.
This was not one of my use-cases so it hasn't been planned, let me know if you need it, otherwise use this one: 
https://github.com/paceholder/nodeeditor/
