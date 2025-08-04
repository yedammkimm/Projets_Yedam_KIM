import os
import random
import string
import io
from flask import Flask, render_template, redirect, url_for, request, session, send_file, jsonify
from PIL import Image, ImageDraw, ImageFont
from threading import Thread
import shutil

import ModelLoader

model = ModelLoader.ModelLoader("CheckpointNew.pth")
ai_thread = None

class ThreadAI(Thread):
    
    def __init__(self, group=None, target=None, name=None,
                 args=(), kwargs={}, Verbose=None):
        Thread.__init__(self, group, target, name, args, kwargs)
        self._return = None

    def run(self):
        if self._target is not None:
            self._return = self._target(*self._args,
                                                **self._kwargs)
    def join(self, *args):
        Thread.join(self, *args)
        return self._return



# def f():
#     # ...

#     global ai_thread
#     captcha_path = "caps/baI05.png"
#     char_num = 5
#     ai_thread = ThreadAI(target=model.predictImage, args=(captcha_path, char_num,))
#     ai_thread.start()

#     # ...

# def r():
#     # ...

#     print(ai_thread.join())

#     # ...

app = Flask(__name__)
app.secret_key = "your_secret_key"

def get_font_paths():
    # Base font directory
    base_dir = "/usr/share/fonts/truetype"
    
    # List to store paths of all compatible .ttf fonts
    font_list = []
    brokenfonts = [
    "/usr/share/fonts/truetype/samyak/Samyak-Devanagari.ttf",
    "/usr/share/fonts/truetype/droid/DroidSansFallbackFull.ttf",
    "/usr/share/fonts/truetype/libreoffice/opens___.ttf",
    "/usr/share/fonts/truetype/Gubbi/Gubbi.ttf",
    "/usr/share/fonts/truetype/sinhala/lklug.ttf",
    "/usr/share/fonts/truetype/teluguvijayam/Ponnala.ttf",
    "/usr/share/fonts/truetype/teluguvijayam/LakkiReddy.ttf",
    "/usr/share/fonts/truetype/kacst/KacstDigital.ttf",
    "/usr/share/fonts/truetype/kacst/KacstPen.ttf",
    "/usr/share/fonts/truetype/kacst/KacstTitle.ttf",
    "/usr/share/fonts/truetype/kacst/KacstTitleL.ttf",
    "/usr/share/fonts/truetype/kacst/KacstFarsi.ttf",
    "/usr/share/fonts/truetype/kacst/KacstPoster.ttf",
    "/usr/share/fonts/truetype/kacst/KacstDecorative.ttf",
    "/usr/share/fonts/truetype/kacst/KacstScreen.ttf",
    "/usr/share/fonts/truetype/kacst/KacstOffice.ttf",
    "/usr/share/fonts/truetype/kacst/KacstBook.ttf",
    "/usr/share/fonts/truetype/kacst/KacstArt.ttf",
    "/usr/share/fonts/truetype/kacst/KacstQurn.ttf",
    "/usr/share/fonts/truetype/kacst/KacstLetter.ttf",
    "/usr/share/fonts/truetype/kacst/mry_KacstQurn.ttf",
    "/usr/share/fonts/truetype/kacst/KacstNaskh.ttf",
    "/usr/share/fonts/truetype/malayalam/RaghuMalayalamSans-Regular.ttf",
    "/usr/share/fonts/truetype/teluguvijayam/RaviPrakash.ttf",
    "/usr/share/fonts/truetype/Navilu/Navilu.ttf"]

    # Loop over each directory in the base font directory
    for root, dirs, files in os.walk(base_dir):
        if '-' in root:  # Skip directories with a dash in their name
            continue

        for file in files:
            if file.endswith(".ttf"):
                font_path = os.path.join(root, file)
                # Test loading the font to ensure compatibility
                try:
                    ImageFont.truetype(font_path, 20)
                    if font_path not in brokenfonts:
                        font_list.append(font_path)  # Add only if it loads successfully
                except IOError:
                    pass
    
    return font_list

width, height = 150, 40  # Dimensions of the CAPTCHA

#Function to generate random CAPTCHA text
def generate_random_text(captcha_length=5):
    return ''.join(random.choices(string.ascii_letters + string.digits, k=captcha_length))

# Function to generate a CAPTCHA image with multiple fonts and spaced characters
def generate_captcha_image(captcha_text, width=150, height=50, char_spacing=10):
    image = Image.new('RGB', (width, height), (random.randint(0, 50), random.randint(0, 50), random.randint(0, 50)))  # Background color
    draw = ImageDraw.Draw(image)
    
    # Get available font paths
    font_paths = get_font_paths()
    if not font_paths:
        return None
    
    # Calculate the total width of the CAPTCHA text
    total_text_width = 0
    for char in captcha_text:
        font_path = random.choice(font_paths)
        try:
            font = ImageFont.truetype(font_path, 30)  # Select a random font
        except IOError:
            font = ImageFont.load_default()
        
        # Calculate the width of the current character and add it to the total width
        text_width, text_height = draw.textsize(char, font=font)
        total_text_width += text_width
    
    # Add spacing between the characters
    total_text_width += (len(captcha_text) - 1) * char_spacing  # Total space between characters

    # Calculate the starting x-position to center the text with space between characters
    current_x = (width - total_text_width) // 2
    
    # Loop through the CAPTCHA text and draw each character with a random font and spacing
    for i, char in enumerate(captcha_text):
        font_path = random.choice(font_paths)
        try:
            font = ImageFont.truetype(font_path, 30)  # Select a random font
        except IOError:
            font = ImageFont.load_default()
        
        # Calculate the text size for the current character
        text_width, text_height = draw.textsize(char, font=font)
        
        # Calculate y-position to center the text vertically
        y_position = (height - text_height) // 2
        
        # Draw the character with a random color
        char_color = (random.randint(100, 255), random.randint(100, 255), random.randint(100, 255))
        draw.text((current_x, y_position), char, font=font, fill=char_color)
        
        # Update the x-position for the next character, including the spacing
        current_x += text_width + char_spacing
    
    # Add noise: lines and dots
    for _ in range(random.randint(3, 6)):
        line_color = (random.randint(100, 255), random.randint(100, 255), random.randint(100, 255))
        x1, y1 = random.randint(0, width), random.randint(0, height)
        x2, y2 = random.randint(0, width), random.randint(0, height)
        draw.line([(x1, y1), (x2, y2)], fill=line_color, width=random.randint(1, 2))

    for _ in range(random.randint(40, 60)):
        dot_color = (random.randint(100, 255), random.randint(100, 255), random.randint(100, 255))
        x, y = random.randint(0, width), random.randint(0, height)
        draw.point((x, y), fill=dot_color)

    # Return the image in a format suitable for Flask
    img_io = io.BytesIO()
    image.save(img_io, 'PNG')
    img_io.seek(0)
    return img_io

def generate_captcha_image_to_file(captcha_text, file_path):
    img_io = generate_captcha_image(captcha_text)
    with open(file_path, 'wb') as f:
        f.write(img_io.getvalue())

@app.route('/vs-ai/verify-user', methods=['POST'])
def verify_user():
    try:
        # Ensure required session variables exist
        if 'user-captcha-text' not in session or 'score' not in session or 'timer' not in session:
            return jsonify({'error': 'Session data missing or expired'}), 400

        # Get user input
        user_input = request.form.get('captcha_input')
        if not user_input:
            return jsonify({'error': 'Captcha input is missing'}), 400

        # Compare user input to CAPTCHA text
        correct_captcha = session['user-captcha-text']
        score = session['score']
        max_score = session['max_score']
        feedback = "Incorrect! Try again."

        correct_letters_user = sum(1 for i in range(min(len(user_input), len(correct_captcha)))
                                    if user_input[i] == correct_captcha[i])

        if correct_letters_user > 0:
            score += correct_letters_user
            feedback = f"Correct letters: {correct_letters_user}. Your score has increased."
        else:
            feedback = "No correct letters. Try again."
        feedback+=f"the correct answer was: {correct_captcha}"
        max_score += 5
        session['score'] = score
        session['max_score'] = max_score

        # Generate a new CAPTCHA for the user
        session['user-captcha-text'] = generate_random_text()
        new_captcha_url = url_for('captcha') + f'?v={random.randint(0, 100000)}'

        return jsonify({
            'feedback': feedback,
            'score': score,
            'captcha_image': new_captcha_url,
            'max_score': max_score
        })

    except Exception as e:
        app.logger.error(f"Error in /vs-ai/verify-user route: {str(e)}", exc_info=True)
        return jsonify({'error': 'An unexpected error occurred'}), 500

@app.route('/vs-ai/verify-ai', methods=['POST'])
def verify_ai():
    try:
        # Ensure required session variables exist
        if 'ai-captcha-text' not in session or 'ai_score' not in session:
            return jsonify({'error': 'Session data missing or expired'}), 400
        # AI solves the CAPTCHA
        correct_captcha = session['ai-captcha-text']
        captcha_path = os.path.join(app.static_folder, 'images/ai_captcha', f"{correct_captcha}.png")
        ai_answer = model.predictImage(captcha_path, 5)  # Simulate AI solving

        # Compare AI answer to CAPTCHA text
        ai_score = session['ai_score']
        max_score_ai = session['max_score_ai']

        correct_letters_ai = sum(1 for i in range(min(len(ai_answer), len(correct_captcha)))
                                 if ai_answer[i] == correct_captcha[i])

        if correct_letters_ai > 0:
            ai_score += correct_letters_ai
            ai_feedback = f"AI solved {correct_letters_ai} letters correctly."
        else:
            ai_feedback = "AI did not solve the CAPTCHA."
        ai_feedback+=f"ai answered {ai_answer} and the correct answer was {correct_captcha}"
        max_score_ai += 5
        session['ai_score'] = ai_score
        session['max_score_ai'] = max_score_ai

        # Generate a new CAPTCHA for the AI
        session['ai-captcha-text'] = generate_random_text()
        new_captcha_url = url_for('captcha_ai') + f'?v={random.randint(0, 100000)}'

        return jsonify({
            'ai_feedback': ai_feedback,
            'ai_score': ai_score,
            'captcha_image': new_captcha_url,
            'max_score_ai': max_score_ai
        })

    except Exception as e:
        app.logger.error(f"Error in /vs-ai/verify-ai route: {str(e)}", exc_info=True)
        return jsonify({'error': 'An unexpected error occurred'}), 500

@app.route('/verify', methods=['POST'])
def verify():
    try:
        # Check session data for essential variables
        if 'captcha_text' not in session or 'score' not in session:
            app.logger.error("Session data missing or expired")
            return jsonify({'error': 'Session data missing or expired'}), 400

        user_input = request.form.get('captcha_input')
        if not user_input:
            app.logger.error("Captcha input is missing in the form data")
            return jsonify({'error': 'Captcha input is missing'}), 400

        # Retrieve session variables
        correct_captcha = session['captcha_text']
        score = session['score']
        game_mode = session.get('mode', 'Survival')  # Default to 'Survival'

        feedback = "Incorrect! Try again."  # Default feedback
        game_over = False

        # Common CAPTCHA letter verification
        correct_letters = sum(1 for i in range(min(len(user_input), len(correct_captcha)))
                              if user_input[i] == correct_captcha[i])

        # Handle game modes
        if game_mode == 'Timer Challenge':
            # Timer Challenge Logic
            if correct_letters > 0:
                score += correct_letters
                feedback = f"Correct letters: {correct_letters}. Your score has increased."
            else:
                feedback = "No correct letters. Try again."
            feedback+=f" \n The correct answer was : {correct_captcha}"
            # Update session variables
            session['score'] = score

        elif game_mode == 'Survival':
            # Survival Mode Logic
            remaining_lives = session.get('remaining_lives', 3)
            if correct_letters == 5:
                score += 5
                feedback = f"Correct! Your score has increased by 5."
            else:
                remaining_lives -= 1
                feedback = f"Incorrect! You lost a life. Remaining lives: {remaining_lives}"
            feedback+=f" \n The correct answer was : {correct_captcha}"
            # Check for Game Over
            if remaining_lives <= 0:
                game_over = True
                remaining_lives = 0
                feedback = "Game Over! You have no lives left."
                feedback+=f" \n The correct answer was : {correct_captcha}"

            # Update session variables
            session['score'] = score
            session['remaining_lives'] = remaining_lives

        else:
            app.logger.error(f"Unknown game mode: {game_mode}")
            return jsonify({'error': 'Unknown game mode'}), 400

        # Generate a new CAPTCHA
        session['captcha_text'] = generate_random_text()
        new_captcha_url = url_for('captcha_solo') + f'?v={random.randint(0, 100000)}'

        # Return JSON response
        return jsonify({
            'feedback': feedback,
            'score': score,
            'remaining_lives': session.get('remaining_lives', None),  # Only for Survival mode
            'game_over': game_over,
            'captcha_image': new_captcha_url
        })

    except Exception as e:
        app.logger.error(f"Error in /verify route: {str(e)}", exc_info=True)
        return jsonify({'error': 'An unexpected error occurred'}), 500

@app.route('/vs-ai/ai_challenge', methods=['POST'])
def ai_challenge():
    try:
        if 'captcha_text' not in session or 'score' not in session or 'timer' not in session:
            return jsonify({'error': 'Session data missing or expired'}), 400

        user_input = request.form.get('captcha_input')
        if not user_input:
            return jsonify({'error': 'Captcha input is missing'}), 400
        correct_captcha = session['captcha_text']
        score = session['score']
        ai_score = session['ai_score']
        max_score = session['max_score']
        timer = session['timer']
        game_over = False
        feedback = "Incorrect! Try again."
        ai_feedback = "AI is solving captchas..."

        # AI solving CAPTCHA
        captcha_path = os.path.join(app.static_folder, 'images/captcha', f"{session['captcha_text']}.png")
        ai_answer = model.predictImage(captcha_path, 5)
        ai_answer = ai_thread.join()
        # ai_answer = model.predictImage(captcha_path, 5)


        # Compare user's answer with AI's
        correct_letters_user = sum(1 for i in range(min(len(user_input), len(correct_captcha)))
                                    if user_input[i] == correct_captcha[i])
        correct_letters_ai = sum(1 for i in range(min(len(ai_answer), len(correct_captcha)))
                                 if ai_answer[i] == correct_captcha[i])

        # Update scores based on correctness
        if correct_letters_user > 0:
            score += correct_letters_user
            feedback = f"Correct letters: {correct_letters_user}. Your score has increased."
        else:
            feedback = "No correct letters. Try again."

        if correct_letters_ai > 0:
            ai_score += correct_letters_ai
            ai_feedback = f"AI solved {correct_letters_ai} correctly."
        max_score += 5
        session['score'] = score
        session['ai_score'] = ai_score
        session['max_score'] = max_score
        # Decrease timer
        # session['timer'] = timer - 1

        print(session['timer'])
        if session['timer'] <= 0:
            game_over = True
            feedback += " Time's up! Game over."
            dir="static/images/captcha"
            dir = '/home/midosama/Desktop/CAPTCHA_solving_AI/captcha_racer/static/images/captcha'
            print(4)
            shutil.rmtree(dir, ignore_errors=True)

        feedback += f" The correct answer was: {correct_captcha}"
        ai_feedback+=f"AI answered {ai_answer}"
        # Generate a new CAPTCHA
        session['captcha_text'] = generate_random_text()
        new_captcha_url = url_for('captcha_facing') + f'?v={random.randint(0, 100000)}'

        return jsonify({
            'feedback': feedback,
            'ai_feedback': ai_feedback,
            'score': score,
            'ai_score': ai_score,
            'timer': session['timer'],
            'game_over': game_over,
            'captcha_image': new_captcha_url,
            'max_score' : max_score
        })

    except Exception as e:
        app.logger.error(f"Error in /vs-ai/verify route: {str(e)}", exc_info=True)
        return jsonify({'error': 'An unexpected error occurred'}), 500


@app.route('/solo/survival/<difficulty>')
def survival_game(difficulty):
    if difficulty not in ['easy', 'medium', 'hard']:
        return redirect(url_for('survival_mode'))
    
    session['mode'] = 'Survival'
    session['difficulty'] = difficulty
    session['score'] = 0

    # Définir le nombre total de vies selon la difficulté
    if difficulty == 'easy':
        session['total_lives'] = 5
        session['remaining_lives'] = 5
    elif difficulty == 'medium':
        session['total_lives'] = 3
        session['remaining_lives'] = 3
    else:  # hard
        session['total_lives'] = 1
        session['remaining_lives'] = 1

    session['game_over'] = False
    session['captcha_text'] = generate_random_text()
    
    return render_template('survival_game.html', difficulty=difficulty)

def clear_directory(directory_path):
    """Removes all files and subdirectories in the specified directory."""
    if os.path.exists(directory_path):
        shutil.rmtree(directory_path)  # Remove the directory and all its contents
        os.makedirs(directory_path, exist_ok=True)  # Recreate the empty directory

@app.route('/captcha')
def captcha():
    try:
        # Define the CAPTCHA directory path
        captcha_dir = os.path.join(app.static_folder, 'images/captcha')
        
        # Clear the directory before generating new CAPTCHA files
        clear_directory(captcha_dir)
        
        # Generate CAPTCHA text and store in session
        captcha_text = session.setdefault('user-captcha-text', generate_random_text())
        
        # Define the CAPTCHA file path
        captcha_path = os.path.join(captcha_dir, f'{captcha_text}.png')

        # Generate the CAPTCHA image and save it to disk
        generate_captcha_image_to_file(captcha_text, captcha_path)

        global ai_thread
        char_num = 5
        # ai_thread = ThreadAI(target=model.predictImage, args=(captcha_path, char_num,))
        # ai_thread.start()

        # Return the image to the client
        return send_file(captcha_path, mimetype='image/png')

    except Exception as e:
        app.logger.error(f"Error generating CAPTCHA: {str(e)}")
        return jsonify({'error': 'Failed to generate CAPTCHA'}), 500

@app.route('/captcha_ai')
def captcha_ai():
    try:
        # Define the CAPTCHA directory path for AI
        captcha_dir_ai = os.path.join(app.static_folder, 'images/ai_captcha')

        # Clear the AI CAPTCHA directory before generating a new CAPTCHA file
        clear_directory(captcha_dir_ai)

        # Generate CAPTCHA text and store in session for AI
        captcha_text_ai = session.setdefault('ai-captcha-text', generate_random_text())

        # Define the CAPTCHA file path for AI
        captcha_path_ai = os.path.join(captcha_dir_ai, f'{captcha_text_ai}.png')

        # Generate the CAPTCHA image and save it to disk
        generate_captcha_image_to_file(captcha_text_ai, captcha_path_ai)

        # Start AI solving thread for its CAPTCHA
        char_num = 5
        global ai_thread
        ai_thread = ThreadAI(target=model.predictImage, args=(captcha_path_ai, char_num,))
        ai_thread.daemon=True
        ai_thread.start()

        # Return the AI CAPTCHA image
        return send_file(captcha_path_ai, mimetype='ai_image/png')

    except Exception as e:
        app.logger.error(f"Error generating AI CAPTCHA: {str(e)}")
        return jsonify({'error': 'Failed to generate AI CAPTCHA'}), 500

@app.route('/captcha_facing')
def captcha_facing():
    try:
        # Define the CAPTCHA directory path
        captcha_dir = os.path.join(app.static_folder, 'images/captcha')

        # Clear the directory before generating new CAPTCHA files
        clear_directory(captcha_dir)

        # Generate CAPTCHA text and store in session
        captcha_text = session.setdefault('captcha_text', generate_random_text())

        # Define the CAPTCHA file path
        captcha_path = os.path.join(captcha_dir, f'{captcha_text}.png')

        # Generate the CAPTCHA image and save it to disk
        generate_captcha_image_to_file(captcha_text, captcha_path)

        global ai_thread
        char_num = 5
        ai_thread = ThreadAI(target=model.predictImage, args=(captcha_path, char_num,))
        ai_thread.start()
        # Return the image to the client
        return send_file(captcha_path, mimetype='image/png')

    except Exception as e:
        app.logger.error(f"Error generating CAPTCHA: {str(e)}")
        return jsonify({'error': 'Failed to generate CAPTCHA'}), 500

@app.route('/captcha_solo')
def captcha_solo():
    captcha_text = generate_random_text()  # Générer un texte aléatoire
    session['captcha_text'] = captcha_text  # Stocker dans la session
    img_io = generate_captcha_image(captcha_text)
    return send_file(img_io, mimetype='image/png')
    try:
        captcha_text = generate_random_text()  # Générer un texte aléatoire
        session['captcha_text'] = captcha_text  # Stocker dans la session
        img_io = generate_captcha_image(captcha_text)  # Générer l'image
        if not img_io:
            raise ValueError("Failed to generate CAPTCHA image.")
        app.logger.debug("Captcha image generated successfully.")
        return send_file(img_io, mimetype='image/png')
    except Exception as e:
        app.logger.error(f"Error generating CAPTCHA: {str(e)}")
        return jsonify({'error': 'Failed to generate CAPTCHA'}), 500
# Route to handle CAPTCHA form submission
@app.route('/validate-captcha', methods=['POST'])
def validate_captcha():
    user_input = request.form['captcha']
    if user_input == session.get('captcha_text'):
        return redirect(url_for('home'))  # Redirect to home page on success
    else:
        return render_template('game.html', error="Incorrect CAPTCHA. Please try again.")  # Show error message

# Home Route
@app.route('/')
def home():
    return render_template('home.html')

# Solo Mode Selection Route
@app.route('/solo')
def solo_mode():
    return render_template('solo.html')

# Survival Mode Route
@app.route('/solo/survival')
def survival_mode():
    return render_template('survival.html')

# Timer Challenge Route
@app.route('/solo/timer')
def timer_challenge():
    return render_template('timer.html')

# Timer Challenge Start Route
@app.route('/solo/timer/start')
def timer_game():
    # Initialize timer game session
    session['mode'] = 'Timer Challenge'
    session['timer'] = 60  # 60 seconds countdown
    session['score'] = 0  # Track score
    return render_template('timer_game.html')

# @app.route('/multiplayer')
# def multiplayer_mode():
#     return render_template('multiplayer.html')
# @app.route('/multiplayer/race_the_clock')
# def race_the_clock():
#     return render_template('race_the_clock.html')
@app.route('/vs-ai')
def vs_ai():
    return render_template('versus_AI.html')

@app.route('/vs-ai/1-vs-AI')
def one_vs_ai():
    return render_template('one_vs_ai.html')

@app.route('/vs-ai/race-the-ai')
def race_ai():
        # Initialize timer game session
    session['mode'] = 'AI racing'
    session['timer'] = 120  # 60 seconds countdown
    session['score'] = 0  # Track score
    session['ai_score'] = 0 #track ai score
    session['max_score'] = 0 #the max score
    session['max_score_ai'] = 0 #the max score of the ai
    session['ai-captcha-text'] = generate_random_text()
    session['user-captcha-text'] = generate_random_text()
    return render_template('race_ai.html')


@app.route('/vs-ai/face-the-ai')
def face_ai():
        # Initialize timer game session
    session['mode'] = 'AI facing'
    session['timer'] = 100  # 60 seconds countdown
    session['score'] = 0  # Track score
    session['ai_score'] = 0 #track ai score
    session['max_score'] = 0 #the max score
    session['max_score_ai'] = 0 #the max score of the ai
    return render_template('face_the_ai.html')
if __name__ == '__main__':
    app.run(debug=True)