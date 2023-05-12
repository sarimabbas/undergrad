import csv
import urllib.request
import requests
import logging
from flask import redirect, render_template, request, session, url_for
from functools import wraps

# for embedContentHelper
from urllib.parse import urlparse, parse_qs

def page_error(error_text=""):
    """Renders message as a page_error to user."""
    
    return render_template("error.html", error=error_text)

def login_required(f):
    """
    Decorate routes to require login.

    http://flask.pocoo.org/docs/0.11/patterns/viewdecorators/
    """
    @wraps(f)
    def decorated_function(*args, **kwargs):
        if session.get("user_id") is None:
            return redirect(url_for("login", next=request.url))
        return f(*args, **kwargs)
    return decorated_function
    
def embedContentHelper(embedContent):
    """Creates embeddable HTML."""
    
    # for parsing image URLs
    imageFormatList = [".jpg", ".png", ".gif", ".jpeg", ".tiff"]

    # first check if the embedContent URL exists
    request = requests.get(embedContent)
    if request.status_code != 200:
        return "embedBroken"
        
    else:    
        # then parse for YouTube
        if "youtube" and "watch" in embedContent:
            # get videoID from URL
            URL = urlparse(embedContent)
            videoID = parse_qs(URL.query)
            
            # prepare source
            src = "https://www.youtube.com/embed/" + videoID["v"][0]
            
            # create embed HTML
            embedHTML = "<div class=\"embedContentVideo\"><iframe src=\"" + src + "\" frameborder=\"0\" allowfullscreen></iframe></div>"
            
        # then parse for Images
        elif any(extension in embedContent for extension in imageFormatList):
            
            # prepare source
            src = embedContent
            
            # create embed HTML
            embedHTML = "<img src=\"" + src + "\" width=\"100%\" class=\"embedContentImage\">"
        
        # then parse for all other content
        else:
            
            embedHTML = "<a target=\"_blank\" href=\""+ embedContent + "\"><h3>" + embedContent + "</h3></a>"
            
        ### return embedHTML for all cases
        return embedHTML