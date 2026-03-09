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
- teamuser
- user123


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
##HW2: 
Live site: https://reneed.site
Homework 2 – CGI Programs

Under Homework 2 on the main website, there are links to all CGI programs written in multiple languages, including Python, PHP, C, and Perl.

Each language implementation includes:

Hello HTML

Hello JSON

Environment variables

Echo (supports multiple HTTP methods)

State (set/view/clear)

## Server Information
- Hosting provider: DigitalOcean
- Server IP address: 134.209.15.14
- Domain: reneed.site
- Web server: Apache
- SSH access: Public key authentication
	- Username: teamuser
	- Password: user123
- SSH key type: ed25519

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

For the free choice analytics requirement, I looked at several analytics tools, including Google Analytics and other privacy focused platforms. Since Google Analytics was already used earlier in the assignment, I chose to evaluate Plausible Analytics as an alternative.
I selected Plausible because it focuses on privacy-friendly analytics and does not rely on cookies, session replay, or user fingerprinting. Instead, it provides simple but useful metrics such as page views, visitors, and referrers while collecting minimal user data.
To evaluate Plausible, I integrated it directly into my website by adding the provided script to the site’s HTML. After navigating and interacting with the site, I was able to see real-time analytics data appear on the Plausible dashboard. Overall, Plausible was easy to set up and provided a clean and lightweight way to understand basic site usage.




##Hw 3:
testing website: https://test.reneed.site/index.html
Base URL: https://reporting.reneed.site
REST Endpoint

The REST API was implemented using PHP on the reporting host.
The endpoint connects to a MySQL database (cse135_hw3) and operates on the events table.

The API supports full CRUD operations for static event records.

Implemented Routes
HTTP Method	Route	Description
GET	/api/static	Retrieve the latest 200 static event records from the database
GET	/api/static/{id}	Retrieve a specific static event record by ID
POST	/api/static	Insert a new static event record into the database
PUT	/api/static/{id}	Update an existing static event record by ID
DELETE	/api/static/{id}	Delete a static event record by ID


Behavior Details

POST expects a JSON body.

PUT and DELETE require an ID in the URL.

If a requested ID does not exist, the API returns:

{"error":"not_found"}

All responses are returned as application/json.

CORS headers are enabled for GET, POST, PUT, DELETE, and OPTIONS.


Database

Database: cse135_hw3

Table: events

The API filters for records where event_type = 'static'.


----------------------
##HW4
In order to log into main domain site, you use 
teamuser
user123

Analytics Backend Scaffold
Live Application
https://reneed.site/hw4/login.php

Grader Login Credentials
Username: grader
Password: ucsdpa1

Overview
This assignment implements the first version of an analytics backend system.
The goal of the assignment is to build a minimal backend that demonstrates:

1. Authentication and protected routes
2. Connection from a datastore to a data table
3. Connection from a datastore to a chart visualization

The application is implemented using PHP and MariaDB and runs on an Apache server hosted on a DigitalOcean Ubuntu droplet.

---

Hosting Environment

Server: DigitalOcean Ubuntu Droplet
Web Server: Apache2
Backend Language: PHP
Database: MariaDB
Domain: https://reneed.site

Database used for analytics collection:
cse135_hw3

Main analytics table:
events

---

Step 1 – MVC-style scaffold with authentication

The analytics backend includes a simple authentication system implemented using PHP sessions.

Pages implemented

/hw4/login.php
Login page for the analytics application.

Users must authenticate before accessing any protected pages.

/hw4/dashboard.php
Protected dashboard page.
Displays the logged-in user and navigation links.

/hw4/reports.php
Protected analytics reporting page.

/hw4/logout.php
Destroys the session and returns the user to the login page.

Authentication implementation

Authentication is implemented using PHP sessions.
The file:

/hw4/inc/auth.php

acts as middleware that checks whether a user session exists before allowing access to protected pages.

If a user attempts to access a protected route without logging in, they are redirected to the login page.

Example protected routes:

/hw4/dashboard.php
/hw4/reports.php

Forceful browsing protection

If a user attempts to directly access a protected page such as:

https://reneed.site/hw4/dashboard.php

https://reneed.site/hw4/reports.php

without authentication, the application automatically redirects them to:

/hw4/login.php

This prevents bypassing the login system by manually entering URLs.

---

Step 2 – Datastore connected to a data table

The reporting page connects to the MariaDB datastore and retrieves analytics data from the events table.

Database

Database name: cse135_hw3
Table name: events

Relevant table fields

id
server_ts
ip
event_type
session_id
page
payload_json
created_at

This demonstrates that the backend is successfully connected to the datastore and able to display collected analytics data in tabular form.

---

Step 3 – Datastore connected to a chart

The reports page also includes a data visualization generated using Chart.js.

Chart library used

Chart.js (loaded via CDN)

API endpoint

/hw4/api/events_by_type.php

This endpoint queries the database and aggregates analytics data by event type.

Frontend chart

The reports page fetches this API endpoint and renders a bar chart that shows the distribution of collected event types.

Data flow

MariaDB events table
→ PHP API endpoint
→ JSON response
→ Chart.js visualization

This demonstrates that the analytics datastore is connected to a chart visualization.

---

Application Structure

hw4/

login.php
dashboard.php
reports.php
logout.php

inc/
auth.php
db.php

api/
events_by_type.php

assets/

---

Summary

This implementation satisfies the three HW4 requirements:

Step 1
Authentication system with login, logout, protected routes, and forceful browsing protection.

Step 2
Connection from the MariaDB datastore to a reporting table displaying collected analytics events.

Step 3
Connection from the datastore to a Chart.js visualization showing aggregated analytics data.

The system forms the initial scaffold for a full analytics backend that can be expanded with additional reporting and visualizations.


