# CSE 135 – Personal Course Website

This project is a personal course website hosted on a DigitalOcean Ubuntu droplet using Apache. The site includes a homepage for course assignments and a personal member page. The website is deployed from GitHub and automatically updates whenever changes are pushed.
##Memebers of the Team:
- Renee Dhanaraj
- 
 Login for APache for the grader:
grader
ucsdpa1
 

Live site:
- https://reneed.site  
- Personal page: https://reneed.site/members/renee.html

Login to th site is: 
teamuser
user123


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

## GitHub Auto-Deploy Setup

This site is automatically deployed using GitHub and a webhook on the DigitalOcean server. Whenever I push changes to the main branch of my GitHub repository, the website updates on the server without needing to manually pull the code.

The website files are stored in `/var/www/reneed.site` and the directory is connected to GitHub using SSH authentication. An SSH key was generated on the server and added to my GitHub account, allowing secure access to the repository without using a username or password.

To handle deployment, a shell script (`deploy-cse135.sh`) was created on the server. This script pulls the latest changes from the GitHub repository and reloads Apache so the updates go live immediately. A Flask application runs on the server and listens for GitHub webhook events on port 9000. When GitHub sends a POST request after a push, the Flask app triggers the deploy script.

A GitHub webhook was configured in the repository settings to send push events to the server’s `/deploy` endpoint. This setup was verified by pushing changes to GitHub and confirming that the website updated automatically without manual intervention.


## Password-Protected Area

The team site is protected using Apache Basic Authentication over HTTPS. 
ysing the file hello.php

Protected URL:
- https://reneed.site/members/
- Once you login it doens't prompt you to log in again and again. But on incognito it does. 
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





## Server Header Obfuscation

Changing Apache’s `Server` header is not reliably possible using only `mod_headers`
because Apache generates the `Server` header internally after many header filters run.
To fully override the header, ModSecurity was installed and configured.

`libapache2-mod-security2` was enabled and the directive below was added to
`/etc/apache2/mods-enabled/security2.conf`:

`SecServerSignature "CSE135 Server"`

After restarting Apache, the response headers (verified in Chrome DevTools / curl)
show the server header as `Server: CSE135 Server`.


----------------------------------------------------------------------
###HW2: 
Live site: https://reneed.site
Homework 2 – CGI Programs

Under Homework 2 on the main website, there are links to all CGI programs written in multiple languages, including Python, PHP, C, and Perl.

Each language implementation includes:

Hello HTML

Hello JSON

Environment variables

Echo (supports multiple HTTP methods)

State (set/view/clear)

# Server Information:


#Analytics: 
Google Analytics

Google Analytics was integrated into the site to track basic page views and usage statistics. A screenshot of the Google Analytics dashboard is included in the submission as ga-dashboard.png.


LogRocket

LogRocket was used to capture live session replays and client-side interactions.
The LogRocket script was added to the site, and user interactions were recorded by navigating through various pages.

Included in the submission:

logrocket.png — screenshot of the LogRocket dashboard

logrocket-session.mp4


Free Choice Analytics – Plausible

For the free-choice analytics requirement, Plausible Analytics was evaluated and integrated into the website.

Plausible was chosen because it provides privacy-friendly analytics without using cookies, session replay, or user fingerprinting. It offers aggregate metrics such as page views, visitors, and referrers while minimizing data collection.

The Plausible script was added to the site’s HTML, and real-time analytics data appeared after navigating the site. A screenshot of the Plausible dashboard is included as free-choice.png.

