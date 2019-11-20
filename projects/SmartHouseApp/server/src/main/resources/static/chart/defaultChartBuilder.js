var ctx = document.getElementById('chart1').getContext('2d');
		ctx.canvas.width = 1000;
		ctx.canvas.height = 300;

		var color = Chart.helpers.color;
		var cfg = {
			data: {
				datasets: chartData
			},
			options: {
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
						},
						scaleLabel: {
							display: true,
							labelString: chartDataY
						}
					}]
				},
				tooltips: {
					intersect: false,
					mode: 'index',
					callbacks: {
						label: function (tooltipItem, myData) {
							/*var label = myData.datasets[tooltipItem.datasetIndex].label || '';
							if (label) {
								label += ': ';
							}
							label += parseFloat(tooltipItem.value).toFixed(2);
							*/
							var label = parseFloat(tooltipItem.value).toFixed(2);
							label += '  ';
							label += chartDataY;
							return label;
						}
					}
				}
			}
		};

		var chart = new Chart(ctx, cfg);