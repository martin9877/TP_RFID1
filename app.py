from flask import Flask, jsonify
import mysql.connector
from flask_cors import CORS

app = Flask(__name__)
CORS(app)

def get_data():
    connection = mysql.connector.connect(
        host='localhost',
        user='root',
        password='12345',
        database='rc522',
        port=3306
    )
    cursor = connection.cursor(dictionary=True)
    cursor.execute("SELECT * FROM registro ORDER BY tiempo DESC LIMIT 100")
    rows = cursor.fetchall()
    cursor.close()
    connection.close()
    return rows

@app.route('/data', methods=['GET'])
def data():
    return jsonify(get_data())

if __name__ == '__main__':
    app.run(debug=True, port=5000)
