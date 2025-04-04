from flask import Flask, request
import requests
import base64

app = Flask(__name__)

@app.route('/')
def index():
    return """

  <label for="client_id">client_id:</label><br>
  <input type="text" id="client_id" name="client_id" value=""><br>
  <label for="client_secret">client_secret:</label><br>
  <input type="text" id="client_secret" name="client_secret" value=""><br><br>
  
  <input value="Submit" onclick="getRequest()">

<script>

    function getRequest(){
        
        var cid = document.getElementById('client_id').value
        var csec = document.getElementById('client_secret').value
        
        fetch('http://127.0.0.1:5000/set-variable?client_id=' + cid + '&client_secret=' + csec)
        
        document.location = 'https://accounts.spotify.com/authorize?response_type=code&client_id=' + cid + '&scope=user-read-playback-state%20user-modify-playback-state&redirect_uri=http://127.0.0.1:5000/redirect'
        
    }


</script>

"""

@app.route('/set-variable')
def set_variable():
    
    global client_id
    global client_secret

    client_id = request.args.get('client_id')
    client_secret = request.args.get('client_secret')
    
    
    return ''

@app.route('/redirect')
def redirect():
    code = request.args.get('code')
    
    client_creds = f"{client_id}:{client_secret}"
    client_creds_b64 = base64.b64encode(client_creds.encode()).decode()

    url = 'https://accounts.spotify.com/api/token'
    headers = {
        'Content-Type': 'application/x-www-form-urlencoded',
        'Authorization': f'Basic {client_creds_b64}'
    }
    data = {
        'code': code,
        'grant_type': 'authorization_code',
        'redirect_uri': 'http://127.0.0.1:5000/redirect'
    }

    response = requests.post(url, headers=headers, data=data)

    data = response.json()

    return f"<b>access_token</b> --> {data['access_token']}<br><br><b>refresh_token</b> --> {data['refresh_token']}"

app.run()



