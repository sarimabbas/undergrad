$('#toggle').change(function() {
    
        // get boardName
        var URL = window.location.pathname.split( '/' );
        var boardName = URL[2];
        
        // if popularity (on) is selected
        if ($(this).prop('checked') == true) {
            setTimeout(function(){
                
                window.location.replace('/board/' + boardName + '/popular');
                
            }, 300);
        }
        
        // if recent (off) selected
        else if ($(this).prop('checked') == false) {
            
            setTimeout(function(){
                
                 window.location.replace('/board/' + boardName + '/recent');
                
            }, 300);
        }
});