[How can I mix LaTeX in with Markdown?](https://stackoverflow.com/questions/2188884/how-can-i-mix-latex-in-with-markdown)

Add the following code to the top of your Markdown files to get MathJax rendering support.
```html
<style TYPE="text/css">
code.has-jax {font: inherit; font-size: 100%; background: inherit; border: inherit;}
</style>
<script type="text/x-mathjax-config">
MathJax.Hub.Config({
    tex2jax: {
        inlineMath: [['$','$'], ['\\(','\\)']],
        skipTags: ['script', 'noscript', 'style', 'textarea', 'pre'] // removed 'code' entry
    }
});
MathJax.Hub.Queue(function() {
    var all = MathJax.Hub.getAllJax(), i;
    for(i = 0; i < all.length; i += 1) {
        all[i].SourceElement().parentNode.className += ' has-jax';
    }
});
</script>
<script type="text/javascript" src="https://cdnjs.cloudflare.com/ajax/libs/mathjax/2.7.4/MathJax.js?config=TeX-AMS_HTML-full"></script>
```
and then `$x^2$` or `$$x^2$$` will render as expected :-)

You can always install a local version of MathJax if you don't want to use the online distribution, but you might need
to host it through a local webserver.

