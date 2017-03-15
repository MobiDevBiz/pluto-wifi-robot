$(function () {
    var camera_host_port = window.location.hostname;
    var time_stamp = Math.floor(Date.now() / 1000);
    if (camera_host_port == '194.6.233.46') {
        camera_host_port = '194.6.233.46:8084'
    }

    $('#camera-iframe').attr('src', 'http://' + camera_host_port + '/camera/?t=' + time_stamp);

    $('#set-speed').rangeslider({
        polyfill: false,
        onSlideEnd: function (position, value) {
            $.get('/set_speed/' + value);
        }
    });

    $('#stop').on('click', function () {
        $.get('/stop');
    });
    $('#move-forward').on('click', function () {
        $.get('/move/forward');
    });
    $('#move-backward').on('click', function () {
        $.get('/move/backward');
    });
    $('#move-left').on('click', function () {
        $.get('/move/left');
    });
    $('#move-right').on('click', function () {
        $.get('/move/right');
    });

    $('#camera-reset-pos-ver').on('click', function () {
        $('#camera-vertical').anglepicker("value", 95);
    });
    $('#camera-reset-pos-hor').on('click', function () {
        $('#camera-horizontal').anglepicker("value", 95);
    });

    $('#camera-horizontal').anglepicker({
        change: function (e, ui) {
            $('#camera-horizontal-value').text(ui.value);
            $.get('/camera_hor/' + ui.value);
        },
        value: 95,
        min: 0
    });

    $('#camera-vertical').anglepicker({
        change: function (e, ui) {
            $('#camera-vertical-value').text(ui.value);
            $.get('/camera_ver/' + ui.value);
        },
        value: 95,
        min: 0
    });

    var fired = false;

    // Handle keyboard
    $(document).keydown(function (e) {
        // console.log(e.which);
        // 38 - up; 40 - down; 37 - left; 39 - right; 32 - space;
        if (!fired) {
            switch (e.which) {
                case 37:
                    $('#move-left').click();
                    break;
                case 39:
                    $('#move-right').click();
                    break;
                case 38:
                    $('#move-forward').click();
                    break;
                case 40:
                    $('#move-backward').click();
                    break;
                case 32:
                    $('#stop').click();
                    break;
            }
            fired = true;
        }
    });
    $(document).keyup(function (e) {
        // 38 - up; 40 - down; 37 - left; 39 - right; 32 - space;
        fired = false;
        switch (e.which) {
            case 38:
                $('#stop').click();
                break;
            case 40:
                $('#stop').click();
                break;
        }
    });
});