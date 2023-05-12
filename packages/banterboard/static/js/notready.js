// search function for add user typeahead's source
function search(query, syncResults, asyncResults)
{
    // get places matching query (asynchronously)
    var parameters = {
        q: query
    };
    $.getJSON(Flask.url_for("search"), parameters)
    .done(function(data, textStatus, jqXHR) {
     
        // call typeahead's callback with search results (i.e., places)
        asyncResults(data);
    })
    .fail(function(jqXHR, textStatus, errorThrown) {
 
        // log error to browser's console
        console.log(errorThrown.toString());
 
        // call typeahead's callback with no results
        asyncResults([]);
    });
}

// search function for Find Board typeahead's source
function boardsearch(query, syncResults, asyncResults)
{
    // get places matching query (asynchronously)
    var parameters = {
        q: query
    };
    $.getJSON(Flask.url_for("boardsearch"), parameters)
    .done(function(data, textStatus, jqXHR) {
     
        // call typeahead's callback with search results (i.e., places)
        asyncResults(data);
    })
    .fail(function(jqXHR, textStatus, errorThrown) {
 
        // log error to browser's console
        console.log(errorThrown.toString());
 
        // call typeahead's callback with no results
        asyncResults([]);
    });
}
