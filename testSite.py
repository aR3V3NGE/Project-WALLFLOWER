from flask import Flask, redirect, url_for, request, render_template, send_from_directory
from find_holds import main_function
import os
import shutil
import threading

app = Flask(__name__)
UPLOAD_FOLDER = 'static/uploads'
app.config['UPLOAD_FOLDER'] = UPLOAD_FOLDER

@app.route('/')
def form():
    return render_template('login.html')

@app.route('/upload', methods=['POST'])
def upload():
    if 'image' in request.files:
        file = request.files['image']
        difficulty = request.form.get('Difficulty')

        if file.filename != '':
            # Ensure the upload folder exists
            os.makedirs(app.config['UPLOAD_FOLDER'], exist_ok=True)

            filepath = os.path.join(app.config['UPLOAD_FOLDER'], file.filename)
            file.save(filepath)
            return redirect(url_for('display_image', filename=file.filename, difficulty=difficulty))
    return 'No file uploaded', 400

@app.route('/display/<filename>')
def display_image(filename):
    difficulty = request.args.get('difficulty')
    uploaded_path = os.path.join(app.config['UPLOAD_FOLDER'], filename)
    main_function(uploaded_path, difficulty)
    return render_template('display_image.html', filename=filename, difficulty=difficulty)

@app.route('/static/uploads/<filename>')
def uploaded_file(filename):
    filepath = os.path.join(app.config['UPLOAD_FOLDER'], filename)
    output_path = os.path.join(app.config['UPLOAD_FOLDER'], 'holds_output.jpg')
    
    # Serve the file
    response = send_from_directory(app.config['UPLOAD_FOLDER'], filename)

    # Schedule delete after 10 seconds
    def delayed_delete(file_to_delete):
        #for f in files_to_delete:
        try:
            os.remove(file_to_delete)
            print(f"Deleted file: {file_to_delete}")
        except Exception as e:
            print(f"Error deleting file {file_to_delete}: {e}")

    #threading.Timer(60.0, delayed_delete, args=[filepath]).start()

    return response

if __name__ == '__main__':
    app.run(host='0.0.0.0', debug=True)