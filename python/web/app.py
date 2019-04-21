#!/usr/bin/env python3
from  flask  import  Flask
from  flask  import  request
from  flask  import  render_template

app = Flask(__name__)
app.debug = True

@app.route('/<name>', methods=['GET', 'POST'])
def home(name):
    #return '<html><h1>Home World</h1></html>'    # return string
    return render_template('hello.html', name=name)
# *.html should be located in templates directory



if __name__ == '__main__':
    app.run(host='0.0.0.0')
