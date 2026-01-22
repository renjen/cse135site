# CSE 135 – Personal Course Website

This project is a personal course website hosted on a DigitalOcean Ubuntu droplet using Apache. The site includes a homepage for course assignments and a personal member page. The website is deployed from GitHub and automatically updates whenever changes are pushed.

Live site:
- https://reneed.site  
- Personal page: https://reneed.site/members/renee.html

---

## Overview
The goal of this assignment was to set up a basic web server, configure domains and virtual hosts, enable HTTPS, and deploy a simple website. The site also supports automatic deployment from GitHub using a webhook.

---

## Hosting and Server Setup
- Hosting provider: DigitalOcean
- Operating system: Ubuntu
- Web server: Apache2
- Main document root: `/var/www/reneed.site`

Virtual hosts were configured for the following domains:
- `reneed.site`
- `collector.reneed.site`
- `reporting.reneed.site`

---

## HTTPS
HTTPS was enabled using Certbot and Let’s Encrypt. After setup, the site is accessible securely using:


- `index.html` is the main homepage
- `members/renee.html` is the personal member page
- `robots.txt` and `favicon.ico` are included as required
- CSS files are stored under `assets/css`

---

## GitHub Deployment
The site is deployed directly from GitHub. Whenever changes are pushed to the `main` branch, the server automatically pulls the latest updates and reloads Apache.

### Repository Setup
The website directory was initialized as a Git repository and connected to GitHub using SSH authentication. An SSH key was generated on the droplet and added to GitHub under **Settings → SSH and GPG Keys**.

---

## Auto-Deploy Setup
Automatic deployment is handled using a GitHub webhook and a small Flask application running on the server.

### Deploy Script
A deploy script is located at:



This script:
1. Pulls the latest changes from the GitHub repository
2. Reloads Apache so the updates go live immediately

The site directory is owned by the user `renee` so Git can update files without permission issues.

---

### Webhook Listener
A Flask application listens for GitHub webhook events on port `9000`.

## Password-Protected Area

The team site is protected using Apache Basic Authentication over HTTPS. 
ysing the file hello.php

Protected URL:
- https://reneed.site/members/

Login credentials:
- Username: teamuser
- Password: user123

- Endpoint: `/deploy`
- Trigger: GitHub push events
- When a POST request is received, the deploy script is executed automatically

The webhook was configured in GitHub under:Repository → Settings → Webhooks



## Text Compression

Text compression was enabled on the Apache server using `mod_deflate`.
After enabling compression, HTML, CSS, and JavaScript files are sent to
the browser in a compressed (gzip) format.

This was verified using Chrome DevTools. When inspecting the network
request for the HTML page, the response headers include:

`Content-Encoding: gzip`

This indicates that the original HTML file was compressed by the server
before being sent over the network, reducing the amount of data
transferred and improving page load performance.

