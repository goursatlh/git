#!/usr/bin/env python3

from flask import Flask, render_template

app = Flask(__name__)

# Define subpage data
subpages = [
    {"title": "Subpage 1", "content": "Content for Subpage 1"},
    {"title": "Subpage 2", "content": "Content for Subpage 2"},
    {"title": "Subpage 3", "content": "Content for Subpage 3"},
    {"title": "Subpage 4", "content": "Content for Subpage 4"},
    {"title": "Subpage 5", "content": "Content for Subpage 5"},
    {"title": "Subpage 6", "content": "Content for Subpage 6"},
    {"title": "Subpage 7", "content": "Content for Subpage 7"}
]

@app.route('/')
def index():
    return render_template('index3.html', subpages=subpages)

if __name__ == '__main__':
    app.run(debug=True)

