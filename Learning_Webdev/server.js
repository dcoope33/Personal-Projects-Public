const express = require('express');
const sqlite3 = require('sqlite3').verbose();
const path = require('path');
const session = require('express-session'); // Moved up
const bcrypt = require('bcrypt');
const app = express();
const db = new sqlite3.Database('./mike_dan_fitness.db');

// 1. SESSION SETUP (Must be before routes)
app.use(session({
    secret: 'mike_dan_ultra_safe_secret',
    resave: false,
    saveUninitialized: false,
    cookie: { maxAge: 24 * 60 * 60 * 1000 }
}));

// 2. MIDDLEWARE
app.use(express.urlencoded({ extended: true }));

// 3. AUTH CHECK ROUTE 
app.get('/check-auth', (req, res) => {
    if (req.session.userId) {
        res.json({ loggedIn: true, name: req.session.userName, 
            tier: req.session.tier, plan1: req.session.plan1, plan2: req.session.plan2 });
    } else {
        res.json({ loggedIn: false });
    }
});

// 4. SIGN UP ROUTE
app.post('/register', async (req, res) => {
    const { full_name, email, password } = req.body;
    const saltRounds = 10;
    const hashedPassword = await bcrypt.hash(password, saltRounds);
    const sql = `INSERT INTO users (full_name, email, password) VALUES (?, ?, ?)`;
    db.run(sql, [full_name, email, hashedPassword], function(err) {
        if (err) return res.send("Error: This email is already registered.");
        console.log(`New user: ${full_name}`);
        res.redirect('/signin.html'); 
    });
});

// 5. LOGIN ROUTE
app.post('/login', async (req, res) => {
    const { email, password } = req.body;
    const sql = `SELECT * FROM users WHERE email = ?`;
    db.get(sql, [email], async (err, user) => {
        const match = await bcrypt.compare(password, user.password);
        if (err) return res.send("Database error");
        if (!user || !match) {
            return res.send("Invalid email or password");
        }
        req.session.userId = user.id;
        req.session.userName = user.full_name;
        req.session.tier = user.tier;
        req.session.plan1 = user.plan1;
        req.session.plan2 = user.plan2;
        res.redirect('/home.html'); 
    });
});

// LOGOUT
app.get('/logout', (req, res) => {
    req.session.destroy((err) => {
        if (err) {
            console.error("Logout error:", err);
            return res.redirect('/home.html');
        }
        res.clearCookie('connect.sid'); // Clean up the users cookies
        res.redirect('/home.html');    // Send user back to the guest view
    });
});

// add chosen plan to plan1 in database
app.post('/add1', (req, res) => {
    const type = req.body.source;
    const sql = `UPDATE users SET plan1 = ? WHERE id = ?`;
    db.run(sql, [type, req.session.userId], function(err) {
        if (err) {
            console.error(err.message);
            return res.send("Database error occurred.");
        }
    req.session.plan1 = type;
    res.redirect('/home.html'); 
    }); 
});

// add chosen plan2 to database
app.post('/add2', (req, res) => {
    const type = req.body.source;
    const sql = `UPDATE users SET plan2 = ? WHERE id = ?`;
    db.run(sql, [type, req.session.userId], function(err) {
        if (err) {
            console.error(err.message);
            return res.send("Database error occurred.");
        } 
    req.session.plan2 = type;
    res.redirect('/home.html'); 
    }); 
});

// activate chosen tier
app.post('/tier', (req, res) => {
    if (!req.session.userId) return res.redirect('/signin.html');
    console.log(`Activating plan`);
    const tier = req.body;
    console.log(tier.source);
    const sql = `UPDATE users SET tier = ? WHERE id = ?`;
    db.run(sql, [tier.source, req.session.userId], function(err) {
        if (err) {
            console.error(err.message);
            return res.send("Database error occurred.");
        }
    req.session.tier = tier.source;
    res.redirect('/home.html'); 
    });
});

// direct to apropriet plan page
app.post('/view', (req, res) => {
    const type = req.body.source;
    if (req.session.plan1 == type) {
        res.redirect('/myplan1.html');
    } else {
        res.redirect('/myplan2.html');
    }
});

// A "Guard" function to check if a user is logged in
const protect = (req, res, next) => {
    if (req.session.userId) {
        next(); // User is logged in, proceed to the next step
    } else {
        res.redirect('/signin.html'); // Kick user out to sign in
    }
};

// Protect the specific workout pages
app.get(['/myplans.html', '/aquire.html', 'myplan1.html', 'myplan2.html'], protect, (req, res) => {
    res.sendFile(path.join(__dirname, req.path));
});

// 6. STATIC FILES 
app.use(express.static(__dirname));

app.get('/', (req, res) => {
    res.sendFile(path.join(__dirname, 'home.html'));
});

app.listen(3000, () => console.log("Mike and Dan Server live on http://localhost:3000"));