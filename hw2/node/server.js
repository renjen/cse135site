const express = require("express");
const cookieParser = require("cookie-parser");

const app = express();
app.set("trust proxy", true);

// Parse both encodings
app.use(express.urlencoded({ extended: false }));
app.use(express.json());
app.use(cookieParser());

// Helpers
function nowStr() {
  return new Date().toString();
}
function clientIp(req) {
  // Express w/ trust proxy -> req.ip is fine
  return req.ip || req.connection?.remoteAddress || "unknown";
}

//  hello-html-node ----------
app.get("/hello-html-node", (req, res) => {
  res.set("Cache-Control", "no-cache");
  res.type("html");

  const date = nowStr();
  const ip = clientIp(req);

  res.send(`<!DOCTYPE html>
<html>
<head><title>Hello Node CGI</title></head>
<body>
  <h1>Hello Renee</h1><hr/>
  <p>This page was generated with the Node.js programming language</p>
  <p>This program was generated at: ${date}</p>
  <p>Your current IP Address is: ${ip}</p>
</body>
</html>`);
});

// ---------- hello-json-node ----------
app.get("/hello-json-node", (req, res) => {
  res.set("Cache-Control", "no-cache");
  res.type("json");

  res.json({
    message: "Hello Renee",
    language: "Node.js",
    generated_at: nowStr(),
    ip: clientIp(req),
  });
});

// ---------- environment-node ----------
app.get("/environment-node", (req, res) => {
  res.set("Cache-Control", "no-cache");
  res.type("text");

  // Similar to CGI env dump: show request + server-ish values
  const envLike = {
    REQUEST_METHOD: req.method,
    REQUEST_URI: req.originalUrl,
    QUERY_STRING: req.url.includes("?") ? req.url.split("?")[1] : "",
    REMOTE_ADDR: clientIp(req),
    HTTP_USER_AGENT: req.get("user-agent") || "",
    HTTP_HOST: req.get("host") || "",
    CONTENT_TYPE: req.get("content-type") || "",
  };

  let out = "";
  for (const [k, v] of Object.entries(envLike)) out += `${k}=${v}\n`;

  // Also include all headers
  out += "\n--- HEADERS ---\n";
  for (const [k, v] of Object.entries(req.headers)) out += `${k}=${v}\n`;

  res.send(out);
});

// ---------- echo-node (GET/POST/PUT/DELETE + urlencoded + JSON) ----------
function echoHandler(req, res) {
  res.set("Cache-Control", "no-cache");
  res.type("html");

  const method = req.method;
  const ct = req.get("content-type") || "";
  const date = nowStr();
  const ip = clientIp(req);
  const agent = req.get("user-agent") || "unknown";

  // GET params -> req.query
  // POST/PUT/DELETE -> req.body (works for urlencoded + json)
  const params = method === "GET" ? req.query : req.body;

  let items = "";
  if (params && typeof params === "object") {
    for (const [k, v] of Object.entries(params)) {
      if (Array.isArray(v)) {
        v.forEach((vv) => (items += `<li>${k} = ${vv}</li>`));
      } else {
        items += `<li>${k} = ${v}</li>`;
      }
    }
  }

  res.send(`<!DOCTYPE html>
<html>
<head><title>Echo Node</title></head>
<body>
  <h1>Echo (Node.js)</h1><hr/>
  <p><b>Method:</b> ${method}</p>
  <p><b>Content-Type:</b> ${ct}</p>
  <p><b>Generated at:</b> ${date}</p>
  <p><b>Your IP:</b> ${ip}</p>
  <p><b>User Agent:</b> ${agent}</p>
  <h2>Parameters</h2>
  <ul>${items}</ul>
</body>
</html>`);
}

app.get("/echo-node", echoHandler);
app.post("/echo-node", echoHandler);
app.put("/echo-node", echoHandler);
app.delete("/echo-node", echoHandler);

// ---------- state-node (cookie-based, server-side memory) ----------
// Simple in-memory store: sessionId -> { name, favorite, saved_at }
const store = new Map();

function getOrMakeSessionId(req, res) {
  let sid = req.cookies.hw2sid;
  if (!sid) {
    sid = Math.random().toString(16).slice(2) + Date.now().toString(16);
    res.cookie("hw2sid", sid, { httpOnly: true, path: "/hw2/node" });
  }
  return sid;
}

app.get("/state-node-set", (req, res) => {
  res.set("Cache-Control", "no-cache");
  res.type("html");

  res.send(`<!DOCTYPE html>
<html><head><title>State Set (Node)</title></head>
<body>
<h1>State Demo (Node.js) - Set</h1><hr/>
<form method="POST" action="/hw2/node/state-node-set">
  <p>Name: <input name="name" /></p>
  <p>Favorite: <input name="favorite" /></p>
  <button type="submit">Save State</button>
</form>
<p><a href="/hw2/node/state-node-view">Go to View State</a></p>
</body></html>`);
});

app.post("/state-node-set", (req, res) => {
  const sid = getOrMakeSessionId(req, res);
  const name = req.body.name || "";
  const favorite = req.body.favorite || "";
  store.set(sid, { name, favorite, saved_at: nowStr() });

  // redirect to view page
  res.redirect("/hw2/node/state-node-view");
});

app.get("/state-node-view", (req, res) => {
  res.set("Cache-Control", "no-cache");
  res.type("html");

  const sid = getOrMakeSessionId(req, res);
  const data = store.get(sid) || { name: "(none)", favorite: "(none)", saved_at: "(none)" };

  res.send(`<!DOCTYPE html>
<html><head><title>State View (Node)</title></head>
<body>
<h1>State Demo (Node.js) - View</h1><hr/>
<p><b>Name:</b> ${data.name}</p>
<p><b>Favorite:</b> ${data.favorite}</p>
<p><b>Saved At:</b> ${data.saved_at}</p>

<form method="POST" action="/hw2/node/state-node-clear">
  <button type="submit">Clear State</button>
</form>

<p><a href="/hw2/node/state-node-set">Back to Set State</a></p>
</body></html>`);
});

app.post("/state-node-clear", (req, res) => {
  const sid = req.cookies.hw2sid;
  if (sid) store.delete(sid);
  res.redirect("/hw2/node/state-node-view");
});

// Listen only on localhost
const PORT = 3010;
app.listen(PORT, "127.0.0.1", () => {
  console.log(`HW2 Node server running on http://127.0.0.1:${PORT}`);
});
