import os
import re
import json
import arrow
from sql import *
from flask import Flask, jsonify, render_template, request, flash, url_for
from flask_jsglue import JSGlue
from flask_session import Session

from passlib.apps import custom_app_context as pwd_context
from tempfile import gettempdir
from helpers import *

# configure application
app = Flask(__name__)
JSGlue(app)

# ensure responses aren't cached
if app.config["DEBUG"]:
    @app.after_request
    def after_request(response):
        response.headers["Cache-Control"] = "no-cache, no-store, must-revalidate"
        response.headers["Expires"] = 0
        response.headers["Pragma"] = "no-cache"
        return response
        
# configure session to use filesystem (instead of signed cookies)
app.config["SESSION_FILE_DIR"] = gettempdir()
app.config["SESSION_PERMANENT"] = False
app.config["SESSION_TYPE"] = "filesystem"
Session(app)        
        
# configure CS50 Library to use SQLite database
db = SQL(os.environ['DATABASE_URL'])

@app.route("/")
def index():
    """Show homepage"""
    
    return render_template("index.html")
    
@app.route("/register", methods=["GET", "POST"])
def register():
    """Register user."""
    
    # if user reached route via POST (as by submitting a form via POST)
    if request.method == "POST":
        
        # minimise redundancy
        formUsername = request.form.get("username")
        formPassword = request.form.get("password")
        formRepeatPassword = request.form.get("repeatPassword")
        
        # ensure username is entered
        if not formUsername:
            return page_error("You must provide a username.")
            
        # ensure username alphanumeric
        if formUsername.isalnum() == False:
            return page_error("Your username is not alphanumeric. Type in atleast one character/number, with no spaces or symbols.")
            
        # ensure password was submitted
        elif not formPassword:
            return page_error("You must provide a password.")
            
        # ensure password was repeated and that passwords match
        elif not formRepeatPassword or formRepeatPassword != formPassword:
            return page_error("Your passwords do not match.")
            
        # query 'users' table for username
        rows = db.execute("SELECT * FROM users WHERE username = :username", username=formUsername)
        
        # check if username already exists
        if len(rows) == 1:
            return page_error("Your username is taken, please try another.")

        # add credentials to the 'users' table
        db.execute("INSERT INTO users (username, password) VALUES (:username, :hash)", 
        username=formUsername, hash=pwd_context.encrypt(formPassword))
        
        # login automatically after registration
        rows = db.execute("SELECT * FROM users WHERE username = :username", username=formUsername)
        # session for this user only 
        session["user_id"] = rows[0]["user_id"] 
        session["username"] = rows[0]["username"]
        
        # flash a success message
        flash("Thank you for registering, {}. You have been logged in.".format(session["username"]), "info")
        
        # redirect user to home page
        return redirect(url_for("index"))
        
    # else if user reached route via GET (as by clicking a link or via redirect)
    else:
        return render_template("register.html")
        
@app.route("/logout")
def logout():
    """Log user out."""

    # forget any user_id
    session.clear()

    # redirect user to login form
    return redirect(url_for("login"))
    
@app.route("/login", methods=["GET", "POST"])
def login():
    """Log user in."""

    # forget any user_id
    session.clear()

    # if user reached route via POST (as by submitting a form via POST)
    if request.method == "POST":

        # ensure username was submitted
        if not request.form.get("username"):
            return page_error("You must provide a username.")

        # ensure password was submitted
        elif not request.form.get("password"):
            return page_error("You must provide a password.")

        # query database for username
        rows = db.execute("SELECT * FROM users WHERE username = :username", username=request.form.get("username"))

        # ensure username exists and password is correct
        if len(rows) != 1 or not pwd_context.verify(request.form.get("password"), rows[0]["password"]):
            return page_error("invalid username and/or password")

        # remember which user has logged in
        session["user_id"] = rows[0]["user_id"]
        session["username"] = rows[0]["username"]
        
        # flash a success message
        flash("Welcome, {}.".format(session["username"]), "info")

        # redirect user to home page
        return redirect(url_for("index"))

    # else if user reached route via GET (as by clicking a link or via redirect)
    else:
        return render_template("login.html")

@app.route("/newpost", methods=["GET", "POST"])
@login_required
def newpost():
    """Handles AJAX requests from /js/posts.js"""
    
    # if user reached route via POST (as by submitting a form via POST)
    if request.method == "POST":
        
        # formData contains postContent, embedContent, anonymous
        formData = request.get_json()
        
        # get boardId
        rows = db.execute("SELECT * FROM boards WHERE board_name = :board_name", board_name = formData["boardName"])
        if len(rows) == 1:
            boardId = rows[0]["board_id"]
            formData.update({"boardId" : boardId})
        else:    
            return jsonify("boardNameNotExist")    
        
        # add the userId from session
        formData.update({"userId": session["user_id"]})
        
        # parse the embedContent url to usable HTML
        embedHTML = ""
        # attempt embedContentHelper only if valid URL (although HTML5 browsers should also take care of this)
        if ("http://" in formData["embedContent"]) or ("https://" in formData["embedContent"]):
            embedHTML = embedContentHelper(formData["embedContent"])
        
        # if postContent is empty
        if formData["postContent"] == "":
            return jsonify("emptyPostContent")
        ## if embedContent is broken    
        if embedHTML == "embedBroken":
            return jsonify("embedBroken")
        
        # add the data to the 'posts' table in the database
        rows = db.execute("INSERT INTO posts (user_id, post_content, embed_content, anonymous, board_id) VALUES (:user_id, :post_content, :embed_content, :anonymous, :board_id)",
        user_id=formData["userId"], post_content=formData["postContent"], embed_content=embedHTML, anonymous=formData["anonymous"], board_id=formData["boardId"])
        
        # update the post count in the 'boards' table in the database
        rows = db.execute("SELECT post_count FROM boards WHERE board_id = :board_id", board_id = formData["boardId"])
        postCount = rows[0]["post_count"] + 1
        rows = db.execute("UPDATE boards SET post_count = :post_count WHERE board_id = :board_id", post_count = postCount, board_id = formData["boardId"])

        return jsonify(formData)
        
    # else if user reached route via GET (as by clicking a link or via redirect)
    else:
        return redirect(url_for("index"))


@app.route("/newvote", methods=["GET", "POST"])
@login_required
def newvote():
    """Handles AJAX requests from /js/votes.js"""
        
    # if user reached route via POST (as by submitting a form via POST)
    if request.method == "POST":
        
        # check if user logged in
        if not session["user_id"]:
            return jsonify("login to vote")
            
        # voteData contains voteType and postId only
        voteData = request.get_json()
        
        # add the userId from session
        voteData.update({"userId": session["user_id"]})
        
        if voteData["voteType"] == "upvote":
            
            # check for existing vote
            rows = db.execute("SELECT * FROM votes WHERE user_id = :user_id AND post_id = :post_id",
            user_id=voteData["userId"], post_id=voteData["postId"])
            
            # if vote exists
            if len(rows) == 1:
                # prevent repeat upvote
                if rows[0]["vote_type"] == 1:
                    return jsonify("repeatUpvote")
                # update downvote to upvote
                elif rows[0]["vote_type"] == -1:
                    # update in votes table
                    rows = db.execute("UPDATE votes SET vote_type = :vote_type WHERE post_id = :post_id AND user_id = :user_id",
                    vote_type="1", post_id=voteData["postId"], user_id=voteData["userId"])
                    # update count in posts table
                    rows = db.execute("SELECT * FROM posts WHERE post_id = :post_id", post_id=voteData["postId"])
                    count = rows[0]["vote_count"] + 1
                    rows = db.execute("UPDATE posts SET vote_count = :vote_count WHERE post_id = :post_id",
                    vote_count=count, post_id=voteData["postId"])
            # if vote does not exist        
            elif len(rows) == 0:
                # add row to 'votes' table with user_id, post_id, board_id, vote_type = 1
                rows = db.execute("INSERT INTO votes (user_id, post_id, board_id, vote_type) VALUES (:user_id, :post_id, :board_id, :vote_type)",
                user_id=voteData["userId"], post_id=voteData["postId"], board_id="0", vote_type="1")
                # update count in posts table
                rows = db.execute("SELECT * FROM posts WHERE post_id = :post_id", post_id=voteData["postId"])
                count = rows[0]["vote_count"] + 1
                rows = db.execute("UPDATE posts SET vote_count = :vote_count WHERE post_id = :post_id",
                vote_count=count, post_id=voteData["postId"])
            
            return jsonify("upvoted")

        elif voteData["voteType"] == "downvote":
            
            # check for existing vote
            rows = db.execute("SELECT * FROM votes WHERE user_id = :user_id AND post_id = :post_id",
            user_id=voteData["userId"], post_id=voteData["postId"])
            
            # if vote exists
            if len(rows) == 1:
                # prevent repeat downvote
                if rows[0]["vote_type"] == -1:
                    return jsonify("repeatDownvote")
                # update downvote to upvote
                elif rows[0]["vote_type"] == 1:
                    # update in votes table
                    rows = db.execute("UPDATE votes SET vote_type = :vote_type WHERE post_id = :post_id AND user_id = :user_id",
                    vote_type="-1", post_id=voteData["postId"], user_id=voteData["userId"])
                    # update count in posts table
                    rows = db.execute("SELECT * FROM posts WHERE post_id = :post_id", post_id=voteData["postId"])
                    count = rows[0]["vote_count"] - 1
                    rows = db.execute("UPDATE posts SET vote_count = :vote_count WHERE post_id = :post_id",
                    vote_count=count, post_id=voteData["postId"])
            # if vote does not exist        
            elif len(rows) == 0:
                # add row to 'votes' table with user_id, post_id, board_id, vote_type = 1
                rows = db.execute("INSERT INTO votes (user_id, post_id, board_id, vote_type) VALUES (:user_id, :post_id, :board_id, :vote_type)",
                user_id=voteData["userId"], post_id=voteData["postId"], board_id="0", vote_type="-1")
                # update count in posts table
                rows = db.execute("SELECT * FROM posts WHERE post_id = :post_id", post_id=voteData["postId"])
                count = rows[0]["vote_count"] - 1
                rows = db.execute("UPDATE posts SET vote_count = :vote_count WHERE post_id = :post_id",
                vote_count=count, post_id=voteData["postId"])
                
            return jsonify("downvoted")    
        
        # if voteData contains neither upvote nor downvote    
        else:
            return jsonify("unexpected error")

    # else if user reached route via GET (as by clicking a link or via redirect)
    else:
        return redirect(url_for("index"))
   

@app.route("/newcomment", methods=["GET", "POST"])
@login_required
def newcomment():
    """Handles AJAX requests from /js/comments.js"""
    
    # if user reached route via POST (as by submitting a form via POST)
    if request.method == "POST":
        
        # formData contains commentContent, postId
        formData = request.get_json()
        
        # add the userId from session
        formData.update({"userId": session["user_id"]})
        
        # if postContent is empty
        if formData["commentContent"] == "":
            return jsonify("emptyCommentContent")
            
        # get boardId
        rows = db.execute("SELECT board_id FROM boards WHERE board_name = :board_name", board_name = formData["boardName"])
        boardId = rows[0]["board_id"]
        
        # add row to 'comments' table with user_id, post_id, comment_content
        rows = db.execute("INSERT INTO comments (user_id, post_id, comment_content, board_id) VALUES (:user_id, :post_id, :comment_content, :board_id)",
        user_id=formData["userId"], post_id=formData["postId"], comment_content=formData["commentContent"], board_id=boardId)
                
        # update count in posts table
        rows = db.execute("SELECT * FROM posts WHERE post_id = :post_id", post_id=formData["postId"])
        count = rows[0]["comment_count"] + 1
        rows = db.execute("UPDATE posts SET comment_count = :comment_count WHERE post_id = :post_id",
        comment_count=count, post_id=formData["postId"])
        
        # success
        return jsonify("commentAdded")
        
    # else if user reached route via GET (as by clicking a link or via redirect)
    else:
        return redirect(url_for("index"))
        

@app.route('/board')
@login_required
def boardList():
    """Renders a list of a boards of which a user is a member"""
    
    userId = session["user_id"]
    
    # this SQL query checks for membership in a comma-separated list of member_user_id column in table 'boards'
    # it also runs a parallel query on 'users' to find the creator_user_id username
    #rows = db.execute("SELECT boards.*, users.username FROM boards LEFT OUTER JOIN users ON boards.creator_user_id = users.user_id WHERE (',' + RTRIM(member_user_id) + ',') LIKE '%,' + :member_user_id + ',%'",
    #member_user_id = userId)
    
    # this SQL query checks if the user has created any boards
    # it also runs a parallel query on 'users' to find the creator_user_id username
    #rows2 = db.execute("SELECT boards.*, users.username FROM boards LEFT OUTER JOIN users ON boards.creator_user_id = users.user_id WHERE creator_user_id = :creator_user_id", creator_user_id = userId)
    
    # combine both results
    #rows3 = rows + [x for x in rows2 if x not in rows]

    rows = db.execute("SELECT members.*, boards.*, users.username FROM members LEFT JOIN boards on members.board_id = boards.board_id LEFT JOIN users on boards.creator_user_id = users.user_id WHERE member_id = :member_id;", member_id = userId)
    
    app.logger.debug(rows)

    # convert the timestamp of the results of the SQL query
    for row in rows:
        dt = arrow.get(row["timestamp"])
        humanized_dt = dt.humanize()
        row["timestamp"] = humanized_dt

    return render_template("boardlist.html", rows=rows)

@app.route('/posts')
@login_required
def postList():
    """Renders a list of a user's posts."""
    
    userId = session["user_id"]
    
    # get posts made by user, ordered by timestamp
    posts = db.execute("SELECT posts.*, users.username FROM posts LEFT OUTER JOIN users ON posts.user_id = users.user_id WHERE posts.user_id = :user_id ORDER BY timestamp DESC", user_id = userId)
    
    # for each post retrieved
    for post in posts:
        # pretty up the timestamp
        dt = arrow.get(post["timestamp"])
        humanized_dt = dt.humanize()
        post["timestamp"] = humanized_dt

        # retrieve comments for each post
        comments = db.execute("SELECT comments.*, users.username FROM comments LEFT OUTER JOIN users ON comments.user_id = users.user_id WHERE post_id = :post_id ORDER BY timestamp DESC", post_id = post["post_id"])
        # pretty up the timestamp
        for comment in comments:
            dt = arrow.get(comment["timestamp"])
            humanized_dt = dt.humanize()
            comment["timestamp"] = humanized_dt
        # append comments to post    
        post.update({"comment_list" : comments})
        
    return render_template("postlist.html", posts=posts)
    
@app.route('/board/<path:boardName>/')    
@app.route('/board/<path:boardName>/<path:sort>')    
def boardRender(boardName, sort="recent"):
    """Implements and renders multiple boards"""
    
    # catch faulty URLs
    if sort != "recent" and sort != "popular":
        return page_error("This URL does not exist.")
    
    # modify SQL query according to sort
    check = ""   
    if sort == "recent":
        order = "timestamp DESC"
    elif sort == "popular":
        check = "checked"
        order = "vote_count DESC"
    
    # check if board exists    
    rows = db.execute("SELECT * FROM boards WHERE board_name = :board_name", board_name = boardName)
    if len(rows) == 1:
        boardId = rows[0]["board_id"]
        public = rows[0]["public"]
        memberList = rows[0]["member_user_id"]
        boardTitle = rows[0]["board_title"]
        
        # if a private board, check for membership in comma-separated memberList above
        rows = db.execute("SELECT * FROM members WHERE member_id = :member_id AND board_id = :board_id", member_id = session["user_id"], board_id = boardId)
        if public == "no" and len(rows) != 1:

            return page_error("Sorry, this board is private.")
        
        # posts with -5 votes get automatically deleted upon boardRender
        db.execute("DELETE FROM posts WHERE vote_count <= -5")
        
        # retrieve posts and comments
        posts = db.execute("SELECT posts.*, users.username FROM posts LEFT OUTER JOIN users ON posts.user_id = users.user_id WHERE board_id = :board_id ORDER BY {}".format(order), board_id = boardId)
        comments = db.execute("SELECT comments.*, users.username FROM comments LEFT OUTER JOIN users ON comments.user_id = users.user_id WHERE board_id = :board_id ORDER BY timestamp DESC", board_id = boardId)
        
        # pretty the timestamp
        for post in posts:
            dt = arrow.get(post["timestamp"])
            humanized_dt = dt.humanize()
            post["timestamp"] = humanized_dt
        
        # pretty the timestamp
        for comment in comments:
            dt = arrow.get(comment["timestamp"])
            humanized_dt = dt.humanize()
            comment["timestamp"] = humanized_dt
        
        # append commentlist to each post
        for post in posts:
            commentList = []
            for comment in comments:
                if post["post_id"] == comment["post_id"]:
                    commentList.append(comment.copy())
            post.update({"comment_list" : commentList})
         
        return render_template("board.html", posts=posts, boardName=boardName, check=check, boardTitle=boardTitle)
    
    else:
        
        return page_error("Sorry, this board does not exist.")
        
@app.route("/newboard", methods=["GET", "POST"])
@login_required
def newboard(): 
    """Implements board creation"""
    
    # if user reached route via POST (as by submitting a form via POST)
    if request.method == "POST":
        
        # check if user logged in
        if not session["user_id"]:
            return jsonify("nologin")
        
        # retrieve form data    
        formData = request.get_json()  
        
        # minimise redundancy
        boardTitle = formData["boardTitle"]
        boardName = formData["boardName"]
        public = formData["public"]
        userId = session["user_id"]
        username = session["username"]
        memberList = formData["memberList"]
        # memberList is a comma separated string
        
        # append user_name to list if not in it
        if userId not in memberList:
            memberList.append(userId)
        stringMemberList = ",".join(map(str, memberList))
        
        # check for boardname as alphanumeric (no spaces or symbols)
        if boardName.isalnum() == False:
            return jsonify("boardNameIncorrect")
        
        # check if boardtitle empty
        if boardTitle == "":
            return jsonify("boardTitleEmpty")
        
        # check if boardname unique
        rows = db.execute("SELECT * FROM boards WHERE board_name = :board_name", board_name = boardName)
        if len(rows) != 1:
            # add data to 'boards' table
            rows = db.execute("INSERT INTO boards (board_title, board_name, public, creator_user_id, member_user_id) VALUES (:board_title, :board_name, :public, :creator_user_id, :member_user_id)",
            board_title = boardTitle, board_name = boardName, public = public, creator_user_id = userId)
            # add data to 'members' table
            rows = db.execute("SELECT * FROM boards WHERE board_name = :board_name", board_name = boardName)
            boardId = rows[0]["board_id"]
            memberListArray = stringMemberList.split(",")
            for member in memberListArray:
                rows = db.execute("INSERT INTO members (board_id, member_id) VALUES (:board_id, :member_id)", board_id = boardId, member_id = member)
            return jsonify("success")
        else:
            return jsonify("boardNameExists")
        
    # else if user reached route via GET (as by clicking a link or via redirect)
    else:
        return render_template("newboard.html")   
    
@app.route("/search", methods=["GET", "POST"])
@login_required
def search():
    """Search usernames for typeahead"""
   
    if not request.args.get("q"):
        raise RuntimeError("missing query")
    
    parloc = request.args.get("q") + "%"
    users = db.execute('SELECT username, user_id FROM users WHERE username LIKE :q ', q=parloc)
       
    return jsonify(users)
    
@app.route("/boardsearch", methods=["GET", "POST"])
def boardsearch():
    """Search boards for typeahead"""
   
    if not request.args.get("q"):
        raise RuntimeError("missing query")
    
    parloc = request.args.get("q") + "%"
    boards = db.execute('SELECT board_title, board_name FROM boards WHERE board_name LIKE :q OR board_title LIKE :q', q=parloc)
       
    return jsonify(boards)
    
@app.route('/sessionquery', methods=['GET'])
@login_required
def sessionquery():
    """Returns information about a user's session"""
    
    sessionList = { "userId" : session["user_id"], "username": session["username"] }
    return jsonify(sessionList)    
    
@app.route('/delete', methods=['POST'])
@login_required
def delete():
    """Implements delete functionality at /posts"""
    
    deleteId = request.form.get("delete")
    
    # get boardId
    rows = db.execute("SELECT board_id FROM posts WHERE post_id=:post_id", post_id=deleteId)
    boardId = rows[0]["board_id"]
    
    # delete post
    rows = db.execute("DELETE FROM posts WHERE post_id=:post_id", post_id=deleteId)
    rows = db.execute("DELETE FROM comments WHERE post_id=:post_id", post_id=deleteId)
    
    # update the post count in the 'boards' table in the database
    rows = db.execute("SELECT post_count FROM boards WHERE board_id = :board_id", board_id = boardId)
    postCount = rows[0]["post_count"] - 1
    rows = db.execute("UPDATE boards SET post_count = :post_count WHERE board_id = :board_id", post_count = postCount, board_id = boardId)
    
    # flash a success message
    flash("Your post has been successfully deleted!", "info")
    
    # redirect user to home page
    return redirect(url_for("postList"))
    
if __name__ == '__main__':
    app.run(debug=True)        