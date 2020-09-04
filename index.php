<?php
  
  require_once('backend/login.php');

?>
<!DOCTYPE html>
<html lang="en">
	<head>
		<meta charset="UTF-8" />
		<meta name="viewport" content="width=device-width, initial-scale=1.0" />
		<title>Millimeter | Home</title>
		<link href="https://fonts.googleapis.com/css?family=Open+Sans&display=swap" rel="stylesheet" />
		<link rel="stylesheet" href="./css/style.css" />
		<link rel="shortcut icon" href="img/millimeter favicon.png" type="image/x-icon">
	</head>

	<body>
		<main class="main">
			<div class="image-overlay">
				<div class="main-content">
					<div class="navbar">
						<div class="navbar-container">
							<div class="millimeter-logo">
								<img src="./img/millimeter logo-light.png" alt="Millimeter logo" />
							</div>
							<div class="login-container">
								<button class="primary-btn" onclick="openModal(loginModal)">LOG IN</button>
							</div>
						</div>
					</div>

					<div class="banner">
						<h1>
							Your Smart Metering Solution
						</h1>
						<p>
							Relax! We've solved your metering problems
						</p>
					<button class="primary-btn" onclick="openModal(loginModal)">LOG IN</button>
					</div>
				</div>
			</div>

			<div class="modal" id="loginModal">
				<div class="modal-content">
					<div class="millimeter-logo">
						<img src="./img/millimeter-logo.svg" alt="Millimeter logo" />
					</div>
					<form action="" method="post">
						<div class="input-group">
							<input type="email" required name="email" id="userId" placeholder="Email" />
						</div>
						<div class="input-group">
							<input type="password" required name="password" id="password" placeholder="Password" />
						</div>
						<div>
							<button class="signin-btn" type="submit" name="login">SIGN IN</button>
						</div>
						<small>Having trouble signing in?</small>
						<div class="centralise">
							<div class="seperator"></div>
							<div style="margin: 0px 2px;">or</div>
							<div class="seperator"></div>
						</div>
						<div>
							<button class="close-btn" id="closeBtn">GO BACK</button>
						</div>
					</form>
				</div>
			</div>
		</main>

		<script src="./js/app.js"></script>
	</body>
</html>
