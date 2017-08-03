// Initialize Firebase
var config = {
    databaseURL: "https://pdbms-smartfarm.firebaseio.com"
};
firebase.initializeApp(config);
var fb = firebase.database();

$(() => {
    $('#temp').html("Loading..")
    $('#humid').html("Loading..")
    fb.ref('data').on('value', (sn) => {
        var temp = [],
            humid = [],
            label = []
        var d = sn.val()
        var e = 0;
        for (var a in d) {
            for (var i in d[a]) {
                label.push(i+" "+a)
                temp.push(parseFloat(d[a][i].temp))
                humid.push(parseFloat(d[a][i].humid))
            }
        }
        var chart = new Chart(document.getElementById("_chart").getContext('2d'), {
            type: 'line',
            data: {
                labels: label,
                datasets: [{
                    data: temp,
                    label: "Temperature",
                    strokeColor : "#48A4D1",
                    borderColor: "#3e95cd",
                    fill: false
                }, {
                    data: humid,
                    label: "Humidity",
                    borderColor: "#8e5ea2",
                    strokeColor : "#48A4D1",
                    fill: false
                }]
            },
            options: {
                scales: {
                    yAxes: [{
                        display: true,
                        ticks: {
                            min: 0,
                            max: 100
                        }
                    }],
                    xAxes: [{
                       display: true,
                    }]
                },
                showLines: true
            }
        });
        chart.render();
        console.log(label, temp, humid)
        $('#temp').html(parseFloat(temp[temp.length - 1]).toFixed(2) + " &deg;C")
        $('#humid').html(parseFloat(humid[humid.length - 1]).toFixed(2) + " %")
    })
});