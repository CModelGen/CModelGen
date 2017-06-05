---
layout: tutorials
permalink: /tutorials/custom-404-page/
title: Custom 404 Page
---

You can easily serve custom 404 error pages with Jekyll to replace the default **Error 404 -- File Not Found** page displayed when one tries to access a broken link on your site.


## On GitHub Pages

Any `404.html` at the **root of your `_site` directory** will be served automatically by GitHub Pages and the local WEBrick development server.

Simply add a `404.md` or `404.html` at the root of your site's source directory and include the YAML Front Matter data to use the theme's base layout.

If you plan to organize your files under subdirectories, the error page should have the following Front Matter Data, set: `permalink: /404.html`. This is to ensure that the compiled `404.html` resides at the root of your processed site, where it'll be picked by the server.

```
---
# example 404.md

layout: default
permalink: /404.html
---

# 404

Page not found! :(
```

## Hosting on Apache Web Servers

Apache Web Servers load a configuration file named [`.htaccess`](http://www.htaccess-guide.com/) that modifies the functionality of these servers.

Simply add the following to your `.htaccess` file.

```
ErrorDocument 404 /404.html
```

With an `.htaccess` file, you have the freedom to place your error page within a subdirectory.

```
ErrorDocument 404 /error_pages/404.html
```

Where the path is relative to your site's domain.

More info on configuring Apache Error Pages can found in [official documentation](https://httpd.apache.org/docs/current/mod/core.html#errordocument). 


## Hosting on Nginx server

The procedure is just as simple as configuring Apache servers, but slightly different.

Add the following to the ngnix configuration file, `nginx.conf`, which is usually located inside `/etc/nginx/` or `/etc/nginx/conf/`:

```
server {
  error_page 404 /404.html;
  location  /404.html {
    internal;
  }
}
```
The `location` directive prevents users from directly browsing the 404.html page.
