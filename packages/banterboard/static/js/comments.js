// enable Bootstrap comment collapse
$(".show-comments").click(function() {
    $(this).closest("div .post").find(".comment-block").collapse("toggle");
});

// the newComment form
$('.newcomment').submit(function(event) {
    
    // find the post id
    var postId = $(this).closest("div .post").data("post-id");
    
    // get the new comment's info
    var commentContent = $(this).find("#commentContent").val();
    var commentId = '#' + $(this).attr('id');
    var comment_count = $(this).closest(".comment-block").siblings('.card-block').find('.comment-count');
    var commentButtonCounter = comment_count.text();
    
    // get boardName
    var URL = window.location.pathname.split( '/' );
    var boardName = URL[2];
    
    // get the form data
    var formData = {
        "commentContent" : commentContent,
        "postId" : postId,
        "boardName" : boardName
    };
    
    // process the form
    $.ajax({
        type : "POST", // define the type of HTTP verb we want to use (POST for our form)
        url : Flask.url_for("newcomment"), // the url where we want to POST
        data : JSON.stringify(formData), // our data object
        contentType : "application/json",
        dataType: "json", // what type of data do we expect back from the server
        encode: true
    })
    // using the done promise callback
    .done(function(data) {
         
        var successMessage = $('.alert-success');
        var errorMessage = $('.alert-danger');
        
        if (data == "emptyCommentContent") {
            
            if (errorMessage) {
                errorMessage.remove();
            }
            
            // for comment validation
            $(commentId).append('<div class="alert alert-danger">' + 'Your comment cannot be empty. Please try again.' + '</div>');
        }
        else {
            
            // show the success message, removing previous ones if they exist
            if (successMessage) {
                
                errorMessage.remove();
                successMessage.remove();
                
                // 
                $(commentId).append('<div class="alert alert-success">' + 'Comment submitted!' + '</div>');
            }
            
            // prepend comment to comments block
            $.getJSON(Flask.url_for("sessionquery"))
            .done(function(data) {
                
                username = data["username"]; 
                var formCommentBlockId = "#form-comment-block-" + postId;
                var commentBlock = "<div class='card card-block'><p class='card-text'>" + commentContent + "</p><p class='card-text'><small class='text-muted'>0 minutes ago</small><small> by " + username + "</small></p></div>";
                $(commentBlock).insertAfter(formCommentBlockId);
                
                // increment counter
                commentButtonCounter = parseInt(commentButtonCounter, 10) + 1;
                commentButtonCounter = (commentButtonCounter).toString();
                comment_count.html(commentButtonCounter);
            });
            
            // erase form contents for next use after a time delay
            setTimeout(function(){
            $('input[id=commentContent], textarea').val('');
            $('.alert-success').remove();
            }, 2500);
        }
    })
    // use the fail promise callback
    .fail(function(data) {
        console.log(data);
    });
    // stop the form from submitting the normal way and refreshing the page
    event.preventDefault();
});