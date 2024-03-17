from quart import Quart, render_template,jsonify
from ble import Connection, asyncio, read_characteristic

app = Quart(__name__)


@app.route('/data')
def echo_socket():
    bluetooth_data=connection.request()    
    return jsonify(bluetooth_data)
        
@app.route('/')
def index():
    return "C'est beau la vie."

@app.route('/dashboard')
async def dashboard():
    return await render_template("index.html")

if __name__ == "__main__":
#############
# App Main
############
    loop = asyncio.get_event_loop()
    connection = Connection(
        loop, read_characteristic
    )
    try:
        asyncio.ensure_future(connection.manager())
        loop.run_until_complete(app.run_task())
    except KeyboardInterrupt:
        print()
        print("User stopped program.")
    finally:
        print("Disconnecting...")
        loop.run_until_complete(connection.cleanup())