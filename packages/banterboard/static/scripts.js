// import scripts outside document ready

$.getScript('/static/js/notready.js')
	.done(function() {
	})
	.fail(function() {
    });

$(document).ready(function() {
        
    // enable Bootstrap tool tips
    $('[data-toggle="tooltip"]').tooltip();
    
    // import scripts for document ready
    
    $.getScript('/static/js/posts.js')
	.done(function() {
	})
	.fail(function() {
    });
    
    $.getScript('/static/js/votes.js')
	.done(function() {
	})
	.fail(function() {
    });
    
    $.getScript('/static/js/comments.js')
	.done(function() {
	})
	.fail(function() {
    });
    
    $.getScript('/static/js/boards.js')
	.done(function() {
	})
	.fail(function() {
    });
    
    $.getScript('/static/js/scroll.js')
	.done(function() {
	})
	.fail(function() {
    });
    
    $.getScript('/static/js/flipswitch.js')
	.done(function() {
	})
	.fail(function() {
    });
    
// end document ready    
});


