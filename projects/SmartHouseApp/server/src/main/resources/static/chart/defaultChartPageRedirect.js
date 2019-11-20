chartDataStr = chartDataStr.replace('"[', "'[");
chartDataStr = chartDataStr.replace(']"', "]'");
chartDataStr = chartDataStr.replace('"{', "'{");
chartDataStr = chartDataStr.replace('}"', "}'");

var chartData = JSON.parse(chartDataStr);
console.log('data parsed');

var redirectUrl = redirectPreffix + currentId;
var startDate;
var endDate;

$(function () {
	$('#dateAfter').datetimepicker({
		locale: 'ru'
	});
});
$(function () {
	$('#dateBefore').datetimepicker({
		locale: 'ru'
	});
});
$("#dateAfter").on("dp.change", function (e) {
    startDate = e.date;
});
$("#dateBefore").on("dp.change", function (e) {
    endDate = e.date;
});

$(function () {
if(afterDate!=undefined && afterDate!=null) {
	$("#dateAfter").data("DateTimePicker").date(moment(afterDate)); 
}
if(beforeDate!=undefined && beforeDate!=null) {
	$("#dateBefore").data("DateTimePicker").date(moment(beforeDate));
}
});
function onDateChange() {
	var startDateUtc = startDate!=undefined?startDate.valueOf():undefined;
	var endDateUtc = endDate!=undefined?endDate.valueOf():undefined;
	var url = redirectUrl;
	if(startDateUtc!=undefined && startDateUtc!=false && startDateUtc!=null) {
		url += "&afterDate=";
		url += startDateUtc;
	}
	if(endDateUtc!=undefined && endDateUtc!=false && endDateUtc!=null) {
		url += "&beforeDate=";
		url += endDateUtc;
	}

	window.location.href = url;
}