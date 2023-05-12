// the newPost form
$('#newPost').submit(function(event) {
    
    // get boardName
    var URL = window.location.pathname.split( '/' );
    var boardName = URL[2];
    
    // get the form data
    var formData = {
        "postContent" : $('input[id=postContent], textarea').val(),
        "embedContent" : $('input[id=embedContent]').val(),
        "anonymous" : $('input[id=anonymous]:checked').val(),
        "boardName" : boardName
    };
    
    // process the form
    $.ajax({
        type : "POST", // define the type of HTTP verb we want to use (POST for our form)
        url : Flask.url_for("newpost"), // the url where we want to POST
        data : JSON.stringify(formData), // our data object
        contentType : "application/json",
        dataType: "json", // what type of data do we expect back from the server
        encode: true
    })
    
    // using the done promise callback
    .done(function(data) {
        var successMessage = $('.alert-success');
        var errorMessage = $('.alert-danger');
        
        // validation 
        if (data == "boardNameNotExist") {
            if (errorMessage) {
                errorMessage.remove();
            }
            $("#newPost").append('<div class="alert alert-danger">' + 'The board you are posting to does not exist.' + '</div>');
        }
        else if (data == "emptyPostContent") {
            if (errorMessage) {
                errorMessage.remove();
            }
            $("#newPost").append('<div class="alert alert-danger">' + 'Your post content cannot be empty.' + '</div>');
        }
        else if (data == "embedBroken") {
            if (errorMessage) {
                errorMessage.remove();
            }
            $("#newPost").append('<div class="alert alert-danger">' + 'Sorry, there is something wrong with your embed URL.' + '</div>');
        }
        else {
            // show the success message, removing previous ones if they exist
            if (successMessage) {
                errorMessage.remove();
                successMessage.remove();
                $("#newPost").append('<div class="alert alert-success">' + 'Post submitted!' + '</div>');
            }
            // close the form after a time delay
            setTimeout(function(){ $('#postForm').collapse("hide") }, 2000);
            
            // erase form contents for next use after a time delay
            setTimeout(function(){
            $('input[id=postContent], textarea').val('');
            $('input[id=embedContent]').val('');
            $('input[id=anonymous]').val('no');
            $('.alert-success').remove();
            }, 2500);
            
            // get boardName
            setTimeout(function(){
            var URL = window.location.pathname.split( '/' );
            var boardName = URL[2];
            window.location.replace('/board/' + boardName + '/' + URL[3]);
            }, 2500);
        }
    })
    // use the fail promise callback
    .fail(function(data) {
    // for errors we don't know about
    console.log(data);
    });
    // stop the form from submitting the normal way and refreshing the page
    event.preventDefault();
});