<?php
	require_once('backend/header-admin.php');
	include('./backend/sensor_values.php');
?>

<script type="text/javascript">

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




//CHART JS
let demo = {
	initDocChart: function () {
		chartColor = "#FFFFFF";

		// General configuration for the charts with Line gradientStroke
		gradientChartOptionsConfiguration = {
			maintainAspectRatio: false,
			legend: {
				display: false
			},
			tooltips: {
				bodySpacing: 4,
				mode: "nearest",
				intersect: 0,
				position: "nearest",
				xPadding: 10,
				yPadding: 10,
				caretPadding: 10
			},
			responsive: true,
			scales: {
				yAxes: [{
					display: 0,
					gridLines: 0,
					ticks: {
						display: false
					},
					gridLines: {
						zeroLineColor: "transparent",
						drawTicks: false,
						display: false,
						drawBorder: false
					}
				}],
				xAxes: [{
					display: 0,
					gridLines: 0,
					ticks: {
						display: false
					},
					gridLines: {
						zeroLineColor: "transparent",
						drawTicks: false,
						display: false,
						drawBorder: false
					}
				}]
			},
			layout: {
				padding: {
					left: 0,
					right: 0,
					top: 15,
					bottom: 15
				}
			}
		};

		ctx = document.getElementById('lineChartExample').getContext("2d");

		gradientStroke = ctx.createLinearGradient(500, 0, 100, 0);
		gradientStroke.addColorStop(0, '#80b6f4');
		gradientStroke.addColorStop(1, chartColor);

		gradientFill = ctx.createLinearGradient(0, 170, 0, 50);
		gradientFill.addColorStop(0, "rgba(128, 182, 244, 0)");
		gradientFill.addColorStop(1, "rgba(249, 99, 59, 0.40)");

		myChart = new Chart(ctx, {
			type: 'line',
			responsive: true,
			data: {
				labels: <?php echo $reading_time;?>,//["Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"],
				datasets: [{
					label: "Active Users",
					borderColor: "#f96332",
					pointBorderColor: "#FFF",
					pointBackgroundColor: "#f96332",
					pointBorderWidth: 2,
					pointHoverRadius: 4,
					pointHoverBorderWidth: 1,
					pointRadius: 4,
					fill: true,
					backgroundColor: gradientFill,
					borderWidth: 2,
					data: <?php echo $voltage;?>//[542, 480, 430, 550, 530, 453, 380, 434, 568, 610, 700, 630]
				}]
			},
			options: gradientChartOptionsConfiguration
		});
	},
}

demo.initDashboardPageCharts = function () {

	gradientChartOptionsConfigurationWithTooltipOrange = {
		maintainAspectRatio: false,
		legend: {
			display: false
		},

		tooltips: {
			backgroundColor: '#f5f5f5',
			titleFontColor: '#333',
			bodyFontColor: '#666',
			bodySpacing: 4,
			xPadding: 12,
			mode: "nearest",
			intersect: 0,
			position: "nearest"
		},
		responsive: true,
		scales: {
			yAxes: [{
				barPercentage: 1.6,
				gridLines: {
					drawBorder: false,
					color: 'rgba(29,29,29,0.0)',
					zeroLineColor: "transparent",
				},
				ticks: {
					suggestedMin: 0, //minimum value for the Y axis of this format of the chart
					suggestedMax: 2000, //maximum value for the Y axis of this format of the chart
					padding: 20,
					fontColor: "#999A9F"
				}
			}],

			xAxes: [{
				barPercentage: 1.6,
				gridLines: {
					drawBorder: false,
					color: 'rgba(220,53,69,0.1)',
					zeroLineColor: "transparent",
				},
				ticks: {
					padding: 20,
					fontColor: "#999A9F"
				}
			}]
		}
	};

	



	//LIGHT INTENSITY CHART
	var ctx = document.getElementById("energyConsumptionChart").getContext("2d");

	var gradientStroke = ctx.createLinearGradient(0, 230, 0, 50);

	gradientStroke.addColorStop(1, 'rgba(255, 81, 25,0.5)');
	gradientStroke.addColorStop(0.2, 'rgba(255, 100, 145,0.0)');
	gradientStroke.addColorStop(0, 'rgba(255, 100, 145,0)'); //purple colors

	var data = {
		labels: <?php echo $reading_time;?>,//['Jan','Feb','Mar','Apr','May','Jun','Jul','Aug','Sep','Oct','Nov', 'Dec'], //LABELS
		datasets: [{
			label: "Data",
			fill: true,
			backgroundColor: gradientStroke,
			borderColor: '#ff8d72',
			borderWidth: 2,
			borderDash: [],
			borderDashOffset: 0.0,
			pointBackgroundColor: '#ff8d72',
			pointBorderColor: 'rgba(255,255,255,0)',
			pointHoverBackgroundColor: '#d048b6',
			pointBorderWidth: 20,
			pointHoverRadius: 4,
			pointHoverBorderWidth: 15,
			pointRadius: 4,
			data: <?php echo $voltage;?>//[500,905,300,1500,400,1203,1400,350,700,1400,900,1500] //CHART DATA
		}]
	};

	var myChart = new Chart(ctx, {
		type: 'line',
		data: data,
		options: gradientChartOptionsConfigurationWithTooltipOrange //CHART FORMAT
	});




	var ctx = document.getElementById("energyConsumptionChart2").getContext("2d");

	var gradientStroke = ctx.createLinearGradient(0, 230, 0, 50);

	gradientStroke.addColorStop(1, 'rgba(255, 81, 25,0.5)');
	gradientStroke.addColorStop(0.2, 'rgba(255, 81, 25,0.3)');
	gradientStroke.addColorStop(0, 'rgba(255, 100, 145,0)'); //purple colors

	var data = {
		labels: <?php echo $reading_time;?>,//['Jan', 'Feb', 'Mar', 'Apr', 'May', 'Jun', 'Jul', 'Aug', 'Sep', 'Oct', 'Nov', 'Dec'], //LABELS
		datasets: [{
			label: "Data",
			fill: true,
			backgroundColor: gradientStroke,
			borderColor: '#ff8d72',
			borderWidth: 2,
			borderDash: [],
			borderDashOffset: 0.0,
			pointBackgroundColor: '#ff8d72',
			pointBorderColor: 'rgba(255,255,255,0)',
			pointHoverBackgroundColor: '#d048b6',
			pointBorderWidth: 20,
			pointHoverRadius: 4,
			pointHoverBorderWidth: 15,
			pointRadius: 4,
			data: <?php echo $voltage;?>//[500, 905, 300, 1500, 400, 1203, 1400, 350, 700, 1400, 900, 1500] //CHART DATA
		}]
	};

	var myChart = new Chart(ctx, {
		type: 'line',
		data: data,
		options: gradientChartOptionsConfigurationWithTooltipOrange //CHART FORMAT
	});

}




demo.initDashboardPageCharts();

</script>