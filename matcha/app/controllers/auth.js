var pool 		= require('../config/connection.js');
var bcrypt 		= require('bcryptjs');
var salt 		= bcrypt.genSaltSync(10);;
var express 	= require('express');
var router 		= express.Router();
var events		= require('../config/event');
var mailsender	= require('./mail.js');

  /**
   * Receive Signin Form Data
  **/
	router.post('/signin', function(req, res) {
		var mail = req.body.mail, pwd = req.body.pwd;
	 
		// is requested correctly formed
		if (mail == undefined || pwd == undefined) {
	 		res.sendStatus(400); return ;
	 	}
	 	// get connection from the pool
	 	pool.getConnection(function(err, connection) {
			if (err) {
				res.sendStatus(500); return ;
			}
		
			// Make the request to verify that the data are good
			connection.query("SELECT * FROM users WHERE mail = ?", [ mail ],  function(err, rows) {
				// If not found return a 404
				if (err || rows.length == 0) {
					res.sendStatus(404);
					connection.release();
					return ;			
				}
				
				// verify that the hash match
				var state = bcrypt.compareSync( pwd, rows[0].password );
				
				// if match, send a success and save it in session
				if (state) {
					req.session.user = rows[0].id;
					connection.release();
					res.sendStatus(200);
				} 
				// Else just send a 401 error 
				else {
					res.sendStatus(404);
					connection.release();
				}
			});
		});
  });
  
  /**
   * Receive logout request
  **/
  router.get('/signout', function(req, res) {  
	// update his last visit date
	pool.getConnection(function (err, connection) {
		if ( !err ) {
			connection.query("UPDATE users SET last_visit = NOW() WHERE id = ?", [ req.session.user ], function(err, rows) {});
		 }
		req.session.destroy(function(err) {
			// redirect him
			res.redirect('/');
		});
		connection.release();
	});
	
});

  /**
   * Display Signup Form
  **/
  router.get('/signup', function(req, res) {
	res.render('signup', {
		title: "Signup | Matcha",
		connected: req.session.user !== undefined
	});
  });
  
  /**
   * Receive Signup Form Data
  **/
  router.post('/signup', function(req, res) {
	 var mail = req.body.mail, pwd = req.body.pwd, firstname = req.body.firstname, lastname = req.body.lastname;
	 
	 // is request correctly formed
	 if (mail == undefined || pwd == undefined || firstname == undefined || lastname == undefined) {
	 	res.sendStatus(400); return ;
	 }
	 // get connection from the pool
	 pool.getConnection(function(err, connection) {
	   if (err) {
		   res.sendStatus(500); return ;
	   }
	   // Make the request to verify that the mail already exist or not
		connection.query("SELECT * FROM users WHERE mail = ?",  [mail],  function(err, rows) {
		// If found return a conflict
		if (err || rows.length > 0) {
			res.sendStatus(409);
			connection.release();
			return ;
		}
		 
		// Else create the account
		else {
			// Generate an uuid
			var uuid = 'xxxxxxxx-xxxx-4xxx-yxxx-xxxxxxxxxxxx'.replace(/[xy]/g, function(c) {
				var r = Math.random()*16|0, v = c == 'x' ? r : (r&0x3|0x8);
				return v.toString(16);
			});
			// make the request
			connection.query("INSERT INTO users (id, mail, password, firstname, lastname, state) VALUES (?, ?, ?, ?, ?, ?)", 
				[uuid, mail, bcrypt.hashSync(pwd, salt), firstname, lastname, 'REGISTERED'], function (err, rows) {
				// maybe ?
				if (err) {
					console.log(err);
					res.sendStatus(500);
				} 
				// Else its good
				else {
					req.session.user = uuid;
					res.sendStatus(200);
				}
				connection.release();
			});
		  }
		})
	});
});

router.post('/reset', function (req, res) {
	var mail = req.body.mail;
	 
	 // is request correctly formed
	 if (mail == undefined) {
	 	res.sendStatus(400); return ;
	 }
	 // get connection from the pool
	 pool.getConnection(function(err, connection) {
	  	 if (err) {
			   res.sendStatus(500); return ;
	 	  }
	  	 // Make the request to verify that the mail already exist or not
		connection.query("SELECT * FROM users WHERE mail = ?",  [ mail ],  function(err, rows) {
			// If found return a that the mail doesnt exist
			if (rows.length == 0) {
				res.sendStatus(404);
			}
			// else send the mail
			else {
				// generate a random string that will be used to be the new password
				var pwd = Math.random().toString(36).slice(2);
				
				var mailOptions = {
					from: '"Mr.Robot" <noreply@matcha.42>',
					to: rows[0].mail,
					subject: '✔ Reset your password ✔',
					text: "Hello ! Here's your new password : " + pwd
				};
				
				mailsender.sendMail(mailOptions, function( error, info) {
					console.log('Reset password sent to ' + rows[0].id + ' successfuly');
				});
				
				res.sendStatus( 201 );
				
				// update the password
				connection.query("UPDATE users SET password = ? WHERE id = ?", [ bcrypt.hashSync(pwd, salt), rows[0].id ], function (err, rows) {});
			}
			connection.release();
		})
	});
});

router.post('/changepwd', function (req, res) {
	var old = req.body.old, pwd = req.body.pwd, user = req.session.user;
	 
	 // is request correctly formed
	 if (old == undefined || pwd == undefined || user == undefined) {
	 	res.sendStatus(400); return ;
	 }
	 
	 console.log(old + " " + pwd);
	 
	 // get connection from the pool
	 pool.getConnection(function(err, connection) {
	  	 if (err) { res.sendStatus(500); return ; }
	  	 // Make the request to verify that the mail already exist or not
		connection.query("SELECT * FROM users WHERE id = ?",  [ user ],  function(err, rows) {
			// If the password old is the same
			var state = bcrypt.compareSync( old, rows[0].password );
			if (state === true) {
				// update the password
				connection.query("UPDATE users SET password = ? WHERE id = ?", [ bcrypt.hashSync(pwd, salt), user ], function (err, rows) {})
				res.sendStatus( 200 );
			} else
				res.sendStatus( 404 );
		});
		connection.release();
	});
});

module.exports = router;