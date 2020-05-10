var ctx = document.getElementById('chart1').getContext('2d');
		ctx.canvas.width = 1000;
		ctx.canvas.height = 300;

		var color = Chart.helpers.color;
		var cfg = {
			type: 'bubble',
			data: {
				datasets: chartData
			},
			options: {
				legend: {
		            display: false
				},
				animation: {
					duration: 0
				},
				scales: {
					xAxes: [{
						type: 'time',
						time: {
							tooltipFormat: 'YYYY-MM-DD  HH:mm:ss',
						},
						distribution: 'linear',
						offset: true,
						ticks: {
							major: {
								enabled: true,
								fontStyle: 'bold'
							},
							source: 'data',
							autoSkip: true,
							autoSkipPadding: 75,
							maxRotation: 0,
							sampleSize: 100
						}
					}],
					yAxes: [{
						gridLines: {
							drawBorder: false
						}
					}]
				}
			}
		};

		var chart = new Chart(ctx, cfg);