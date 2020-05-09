const loginModal = document.getElementById('loginModal');
const activityModal = document.getElementById('activity-modal');
const closeBtn = document.getElementById('closeBtn');
const sideNavbarItem = document.querySelectorAll('li.side-navbar-item');
const dashboardSection = document.querySelectorAll('.dashboard-section');
const chart1 = document.getElementById('chart1');
const chart2 = document.getElementById('chart2');
const activityModalCloseBtn = document.getElementById('activity-modal-close');
const sideNavText = document.querySelectorAll('.side-navbar-text');
const sideNavbar = document.getElementById('sideNavbar');
const hamburgerMenu = document.getElementById('hamburgerMenu');
const overlay = document.getElementById('overlay');
const inputGroup = document.getElementsByClassName('input-group')
const addPaymentModal = document.getElementById('addPaymentModal')
const addPaymentModalCloseBtn = document.getElementById('addPaymentModalClose')

const homeNavBtn = document.getElementById('sideNavbarHome');
const profileNavBtn = document.getElementById('sideNavbarProfile');
const reportingNavBtn = document.getElementById('sideNavbarReporting');
const billingNavBtn = document.getElementById('sideNavbarBilling');
const logoutNavBtn = document.getElementById('sideNavbarLogout');

const dashboardHome = document.getElementById('dashboard-home');
const dashboardProfile = document.getElementById('dashboard-profile');
const dashboardReporting = document.getElementById('dashboard-reporting');
const dashboardBilling = document.getElementById('dashboard-billing');

const homeNavImg = './img/home.2.svg';
const profileNavImg = './img/fingerprint.2.svg';
const reportingNavImg = './img/analytics.2.svg';
const billingNavImg = './img/money-bill-wave.2.svg';

const homeNavImgDefault = './img/home-alt.2.svg';
const profileNavImgDefault = './img/fingerprint-alt.2.svg';
const reportingNavImgDefault = './img/analytics-alt.2.svg';
const billingNavImgDefault = './img/money-bill-wave-alt.2.svg';

const sideNavbarItemImg = [homeNavImgDefault, profileNavImgDefault, reportingNavImgDefault, billingNavImgDefault];

function openModal(modalName) {
	modalName.style.display = 'flex';
	modalName.style.animation = 'fade 0.3s ease-in-out';
	setTimeout(reset, 300);
	function reset() {
		modalName.style.opacity = '1';
		modalName.style.animation = 'none';
	}
}

function close(sectionName) {
	sectionName.style.animation = 'fade 0.3s ease-in-out reverse';
	setTimeout(setDisplay, 300);
	function setDisplay() {
		sectionName.style.display = 'none';
		sectionName.style.animation = 'none';
		sectionName.style.opacity = '0';
	}
}

function openDashboardSection(sectionName, sectionNavBtn, navImg) {
	if (sectionName.style.display == 'grid') {
		//do nothing because we dont want the page to flicker
	} else {
		let i = 0;
		sideNavbarItem.forEach((item) => {
			item.classList.remove('side-navbar-active');
			dashboardSection[i].style.display = 'none';
			item.firstChild.src = sideNavbarItemImg[i];
			i++;
		});
		sectionName.style.display = 'grid';
		sectionName.style.animation = 'fade 0.3s ease-in-out';
		sectionNavBtn.classList.add('side-navbar-active');
		sectionNavBtn.firstChild.src = navImg;
		setTimeout(reset, 300);
		function reset() {
			sectionName.style.animation = 'none';
			sectionName.style.opacity = '1';
		}
		if (window.innerWidth <= 768) {
			sideNavText.forEach((item) => {
				item.style.display = 'none';
				sideNavbar.style.width = '70px';
				overlay.style.display = 'none'
				overlay.style.opacity = '0'
			});
		}
		if (window.innerWidth <= 600) {
			sideNavbar.style.width = '70px';
			sideNavbar.style.transform = 'translateX(-300px)';
		}
	}
}

function openMenu() {
	if (sideNavText[0].style.display == 'none' && window.innerWidth <= 768) {
		sideNavText.forEach((item) => {
			item.style.display = 'inline';
			sideNavbar.style.width = '180px';
			overlay.style.display = 'block'
			overlay.style.animation = 'fade 0.3s ease-in-out'
			setTimeout(() => {
				overlay.style.opacity = '1';
				overlay.style.animation = 'none';
			}, 300)
		});
		if (window.innerWidth <= 600) {
			sideNavbar.style.width = '250px';
			sideNavbar.style.transform = 'translateX(0px)';
		}
	} else {
		if (window.innerWidth <= 600) {
			sideNavbar.style.width = '70px';
			sideNavbar.style.transform = 'translateX(-300px)';
		}
		sideNavText.forEach((item) => {
			item.style.display = 'none';
			sideNavbar.style.width = '70px';
			overlay.style.animation = 'fade 0.3s ease-in-out reverse';
			setTimeout(() => {
				overlay.style.display = 'none';
				overlay.style.animation = 'none';
			}, 300);
		});
		
	}
}

if (homeNavBtn) {
	homeNavBtn.addEventListener('click', () => {
		openDashboardSection(dashboardHome, homeNavBtn, homeNavImg);
	});
}

if (profileNavBtn) {
	profileNavBtn.addEventListener('click', () => {
		openDashboardSection(dashboardProfile, profileNavBtn, profileNavImg);
	});
}

if (reportingNavBtn) {
	reportingNavBtn.addEventListener('click', () => {
		openDashboardSection(dashboardReporting, reportingNavBtn, reportingNavImg);
	});
}

if (billingNavBtn) {
	billingNavBtn.addEventListener('click', () => {
		openDashboardSection(dashboardBilling, billingNavBtn, billingNavImg);
	});
}

if (closeBtn) {
	closeBtn.addEventListener('click', function () {
		close(loginModal);
	});
}

if (activityModalCloseBtn) {
	activityModalCloseBtn.addEventListener('click', function () {
		close(activityModal);
	});
}

if (addPaymentModalCloseBtn) {
	addPaymentModalCloseBtn.addEventListener('click', function () {
		close(addPaymentModal);
	});
}