#include "type_transitions.h"


//
// ---------- PACMAN TRANSITIONS ----------
//

void pacman(sf::RenderWindow& window, vector<vector<Clock>>& clocks, const vector<vector<pair<float, float>>>& targetAngles, int direction){
    if (direction == 1){
        pacman_horizontal(window, clocks, targetAngles);
    }
    if (direction == 2){
        pacman_vertical(window, clocks, targetAngles);
    }
}

void pacman_vertical(sf::RenderWindow& window, vector<vector<Clock>>& clocks, 
            const vector<vector<pair<float, float>>>& targetAngles) {
    float pacmanOpenHour;  // 2:20 -> Open mouth
    float pacmanOpenMinute = 45;

    float pacmanClosedHour = 0;  // 3:15 -> Closed mouth
    float pacmanClosedMinute = 0; 

    for (int row = 0; row < 3; row++) {  
    if (row % 2 == 0) {  // Left to Right
        pacmanOpenHour = 315;
        pacmanOpenMinute = 45;
        pacmanClosedHour = 0; 
        pacmanClosedMinute = 0;
        for (int col = 0; col < 8; col++) {
            Clock& currentClock = clocks[row][col];
            bool isTargetClock = (row == targetRow && col == targetCol);

            // Open Mouth (2:20)
            if (isTargetClock) {
                currentClock.update_with_send(pacmanOpenHour, pacmanOpenMinute, window);
            } else {
                currentClock.update(pacmanOpenHour, pacmanOpenMinute, window);
            }

            // Close Mouth (3:15)
            if (isTargetClock) {
                currentClock.update_with_send(pacmanClosedHour, pacmanClosedMinute, window);
            } else {
                currentClock.update(pacmanClosedHour, pacmanClosedMinute, window);
            }

            std::this_thread::sleep_for(std::chrono::milliseconds((int)(speed)));

            // Set to target angle
            float targetH = targetAngles[col][row].first;
            float targetM = targetAngles[col][row].second;

            if (isTargetClock) {
                currentClock.update_with_send(targetH, targetM, window);
            } else {
                currentClock.update(targetH, targetM, window);
            }
        }
    } else {  // Right to Left
        pacmanOpenHour = 225;
        pacmanOpenMinute = 135;
        pacmanClosedHour = 180; 
        pacmanClosedMinute = 180;
        for (int col = 7; col >= 0; col--) { 
            Clock& currentClock = clocks[row][col];
            bool isTargetClock = (row == targetRow && col == targetCol);

            // Open Mouth (2:20)
            if (isTargetClock) {
                currentClock.update_with_send(pacmanOpenHour, pacmanOpenMinute, window);
            } else {
                currentClock.update(pacmanOpenHour, pacmanOpenMinute, window);
            }

            // Close Mouth (3:15)
            if (isTargetClock) {
                currentClock.update_with_send(pacmanClosedHour, pacmanClosedMinute, window);
            } else {
                currentClock.update(pacmanClosedHour, pacmanClosedMinute, window);
            }

            std::this_thread::sleep_for(std::chrono::milliseconds((int)(speed)));

            // Set to target angle
            float targetH = targetAngles[col][row].first;
            float targetM = targetAngles[col][row].second;

            if (isTargetClock) {
                currentClock.update_with_send(targetH, targetM, window);
            } else {
                currentClock.update(targetH, targetM, window);
            }
        }
    }
    }
}


void pacman_horizontal(sf::RenderWindow& window, vector<vector<Clock>>& clocks, 
            const vector<vector<pair<float, float>>>& targetAngles) {
    
    float pacmanOpenHour, pacmanOpenMinute;
    float pacmanClosedHour, pacmanClosedMinute;

    // Single Loop for All Columns
    for (int col = 0; col < 8; col++) {  
        if (col % 2 == 0) {  // Even column (Bottom to Top)
            pacmanOpenHour = 225;
            pacmanOpenMinute = 315;
            pacmanClosedHour = 270;
            pacmanClosedMinute = 270;

            for (int row = 2; row >= 0; row--) { // Move Bottom to Top
                Clock& currentClock = clocks[row][col];
                bool isTargetClock = (row == targetRow && col == targetCol);

            // Open Mouth (2:20)
            if (isTargetClock) {
                currentClock.update_with_send(pacmanOpenHour, pacmanOpenMinute, window);
            } else {
                currentClock.update(pacmanOpenHour, pacmanOpenMinute, window);
            }

            // Close Mouth (3:15)
            if (isTargetClock) {
                currentClock.update_with_send(pacmanClosedHour, pacmanClosedMinute, window);
            } else {
                currentClock.update(pacmanClosedHour, pacmanClosedMinute, window);
            }

            std::this_thread::sleep_for(std::chrono::milliseconds((int)(speed)));

            // Set to target angle
            float targetH = targetAngles[col][row].first;
            float targetM = targetAngles[col][row].second;

            if (isTargetClock) {
                currentClock.update_with_send(targetH, targetM, window);
            } else {
                currentClock.update(targetH, targetM, window);
            }  
            }

        } else {  // Odd column (Top to Bottom)
            pacmanOpenHour = 45;
            pacmanOpenMinute = 135;
            pacmanClosedHour = 90;
            pacmanClosedMinute = 90;

            for (int row = 0; row < 3; row++) {  // Move Top to Bottom
                Clock& currentClock = clocks[row][col];
                bool isTargetClock = (row == targetRow && col == targetCol);

            // Open Mouth (2:20)
            if (isTargetClock) {
                currentClock.update_with_send(pacmanOpenHour, pacmanOpenMinute, window);
            } else {
                currentClock.update(pacmanOpenHour, pacmanOpenMinute, window);
            }

            // Close Mouth (3:15)
            if (isTargetClock) {
                currentClock.update_with_send(pacmanClosedHour, pacmanClosedMinute, window);
            } else {
                currentClock.update(pacmanClosedHour, pacmanClosedMinute, window);
            }

            std::this_thread::sleep_for(std::chrono::milliseconds((int)(speed)));

            // Set to target angle
            float targetH = targetAngles[col][row].first;
            float targetM = targetAngles[col][row].second;

            if (isTargetClock) {
                currentClock.update_with_send(targetH, targetM, window);
            } else {
                currentClock.update(targetH, targetM, window);
            }  
            }
        }
    }
}


//
// ---------- WORD / STAR / WAVE TRANSITIONS ----------
//

void wave(sf::RenderWindow& window, vector<vector<Clock>>& clocks, const vector<vector<pair<float, float>>>& currentAngles, const vector<vector<pair<float, float>>>& targetAngles, int direction, int number_waves){
    std::string waves="";
    for (int i=0; i< number_waves; i++){
        waves+="^&^&";
    }
        if (direction==1){
            slideTransition_to_left(window, clocks, currentAngles, getTextAngles(waves), targetAngles);
        }

        if (direction==2){
            slideTransition_to_right(window, clocks, currentAngles, getTextAngles(waves), targetAngles);
        }
        if (direction==3){
            slideTransition_to_top(window, clocks, currentAngles, getTextAngles(waves), targetAngles);
        }
        if (direction==4){
            slideTransition_to_bottom(window, clocks, currentAngles, getTextAngles(waves), targetAngles);
        }
        if (direction==5){
            smoothSpinRevealTextThenTime(window, clocks, currentAngles, getTextAngles(waves), targetAngles);
        }
}

void stars(sf::RenderWindow& window, vector<vector<Clock>>& clocks, const vector<vector<pair<float, float>>>& currentAngles, const vector<vector<pair<float, float>>>& targetAngles, int direction, int number_stars){
    std::string stars="";

    for (int i=0; i< number_stars; i++){
        stars+="<>";
    }
        if (direction==1){
            slideTransition_to_left(window, clocks, currentAngles, getTextAngles(stars), targetAngles);
        }

        if (direction==2){
            slideTransition_to_right(window, clocks, currentAngles, getTextAngles(stars), targetAngles);
        }

        if (direction==3){
            slideTransition_to_top(window, clocks, currentAngles, getTextAngles(stars), targetAngles);
        }

        if (direction==4){
            slideTransition_to_bottom(window, clocks, currentAngles, getTextAngles(stars), targetAngles);
        }
        if (direction==5){
            smoothSpinRevealTextThenTime(window, clocks, currentAngles, getTextAngles(stars), targetAngles);
        }
         
}

void words(sf::RenderWindow& window, vector<vector<Clock>>& clocks, const vector<vector<pair<float, float>>>& currentAngles, const vector<vector<pair<float, float>>>& targetAngles, int direction, string word){

    if (direction==1){
        slideTransition_to_left(window, clocks, currentAngles, getTextAngles(word), targetAngles);
    }

    if (direction==2){
        slideTransition_to_right(window, clocks, currentAngles, getTextAngles(word), targetAngles);
    }

    if (direction==3){
        slideTransition_to_top(window, clocks, currentAngles, getTextAngles(word), targetAngles);
    }

    if (direction==4){
        slideTransition_to_bottom(window, clocks, currentAngles, getTextAngles(word), targetAngles);
    }
    if (direction==5){
        while(word.length()<4){
            word+=" ";
        }
            smoothSpinRevealTextThenTime(window, clocks, currentAngles, getTextAngles(word), targetAngles);
    }
}

//
// ---------- SLIDE TRANSITIONS ----------
//

void slideTransition_to_left(sf::RenderWindow& window, vector<vector<Clock>>& clocks, 
                     const vector<vector<pair<float, float>>>& currentAngles,
                     const vector<vector<pair<float, float>>>& wordAngles,
                     const vector<vector<pair<float, float>>>& targetAngles) {

    // Compute total columns required for transition
    size_t timeCols = currentAngles.size();
    size_t wordCols = wordAngles.size();
    size_t targetCols = targetAngles.size();
    size_t totalCols = timeCols + wordCols + targetCols;

    // Create a transition buffer for all animations
    vector<vector<pair<float, float>>> displayAngles(3, 
        vector<pair<float, float>>(totalCols, {-1, -1}));  // Initialize empty clocks

    // Fill displayAngles with Current Hour, Word, and Target Hour
    for (size_t col = 0; col < timeCols; col++) {
        for (size_t row = 0; row < 3; row++) {
            displayAngles[row][col] = currentAngles[col][row];
        }
    }

    // Fill the word after the current hour
    for (size_t col = 0; col < wordCols; col++) {
        for (size_t row = 0; row < 3; row++) {
            displayAngles[row][timeCols + col] = wordAngles[col][row];
        }
    }

    // Fill the target hour after the word
    for (size_t col = 0; col < targetCols; col++) {
        for (size_t row = 0; row < 3; row++) {
            displayAngles[row][timeCols + wordCols + col] = targetAngles[col][row];
        }
    }
    // Animate the Transition Using displayAngles 
    // Initialize lastShownAngles with the first visible frame (columns 0 to 7)
    std::vector<std::vector<std::pair<float, float>>> lastShownAngles(3, std::vector<std::pair<float, float>>(8));
    for (int row = 0; row < 3; ++row) {
        for (int col = 0; col < 8; ++col) {
            lastShownAngles[row][col] = displayAngles[row][col];
        }
    }

    // Loop through each horizontal scroll step (like a sliding window)
    for (size_t step = 1; step <= totalCols - 8; ++step) {
        // Extract the next visible frame from displayAngles
        std::vector<std::vector<std::pair<float, float>>> currentView(3, std::vector<std::pair<float, float>>(8));
        for (int row = 0; row < 3; ++row) {
            for (int col = 0; col < 8; ++col) {
                size_t srcCol = step + col;
                currentView[row][col] = displayAngles[row][srcCol];
            }
        }

        // Compute max angular difference between current and previous state
        float globalMaxDiff = 0.0f;
        for (int row = 0; row < 3; ++row) {
            for (int col = 0; col < 8; ++col) {
                const auto& from = lastShownAngles[row][col];
                const auto& to = currentView[row][col];

                if (to.first != -1 && to.second != -1) {
                    float diffH = angularDistance(from.first, to.first);
                    float diffM = angularDistance(from.second, to.second);
                    globalMaxDiff = std::max(globalMaxDiff, std::max(diffH, diffM));
                }
            }
        }

        int steps = std::max(1, static_cast<int>(globalMaxDiff * MAXSTEP));
        const int delayMs = 5;

        // Interpolate all clocks from lastShownAngles → currentView
        for (int s = 0; s <= steps; ++s) {
            float t = static_cast<float>(s) / steps;
            window.clear(sf::Color::White);

            for (int row = 0; row < 3; ++row) {
                for (int col = 0; col < 8; ++col) {
                    const auto& from = lastShownAngles[row][col];
                    const auto& to = currentView[row][col];

                    if (to.first != -1 && to.second != -1) {
                        float h = interpolateAngle(from.first, to.first, t);
                        float m = interpolateAngle(from.second, to.second, t);
                        clocks[row][col].setInstant(h, m);

                        if (row == targetRow && col == targetCol) {
                            ClockMotion motion;
                            motion.hourAngle = normalize(h);
                            motion.minuteAngle = normalize(m);
                            sendClockMotionToReceptor(motion);
                        }
                        clocks[row][col].draw(window);
                    }
                }
            }

            window.display();
            std::this_thread::sleep_for(std::chrono::milliseconds(delayMs));
        }
            std::this_thread::sleep_for(std::chrono::milliseconds(500));

        // Update lastShownAngles for next step
        lastShownAngles = currentView;
    }
}



void slideTransition_to_right(sf::RenderWindow& window, vector<vector<Clock>>& clocks, 
                     const vector<vector<pair<float, float>>>& currentAngles,
                     const vector<vector<pair<float, float>>>& wordAngles,
                     const vector<vector<pair<float, float>>>& targetAngles) {
    
    size_t timeCols = currentAngles.size();
    size_t wordCols = wordAngles.size();
    size_t targetCols = targetAngles.size();
    size_t totalCols = timeCols + wordCols + targetCols;

    // Create a transition buffer for all animations
    vector<vector<pair<float, float>>> displayAngles(3,vector<pair<float, float>>(totalCols, {-1, -1}));  // Initialize empty clocks

    // Fill displayAngles with Current Hour, Word, and Target Hour
    // Fill the target hour at first
    for (size_t col = 0; col < targetCols; col++) {
        for (size_t row = 0; row < 3; row++) {
            displayAngles[row][col] = targetAngles[col][row];
        }
    }
    
    // Fill the word after the target hour
    for (size_t col = 0; col < wordCols; col++) {
        for (size_t row = 0; row < 3; row++) {
            displayAngles[row][timeCols + col] = wordAngles[col][row];
        }
    }

    // Fill the Current hour at last
    for (size_t col = 0; col < timeCols; col++) {
        for (size_t row = 0; row < 3; row++) {
            displayAngles[row][timeCols + wordCols + col] = currentAngles[col][row];
        }
    }

    // Animate the Transition Using displayAngles (right to left scroll)
    std::vector<std::vector<std::pair<float, float>>> lastShownAngles(3, std::vector<std::pair<float, float>>(8));
    for (int row = 0; row < 3; ++row) {
        for (int col = 0; col < 8; ++col) {
            size_t srcCol = totalCols - 8 + col;
            lastShownAngles[row][col] = displayAngles[row][srcCol];
        }
    }

    for (int step = totalCols - 9; step >= 0; --step) {
        // Prepare current 8-column view
        std::vector<std::vector<std::pair<float, float>>> currentView(3, std::vector<std::pair<float, float>>(8));
        for (int row = 0; row < 3; ++row) {
            for (int col = 0; col < 8; ++col) {
                size_t srcCol = step + col;
                currentView[row][col] = displayAngles[row][srcCol];
            }
        }

        // Compute max angular difference for dynamic step count
        float globalMaxDiff = 0.0f;
        for (int row = 0; row < 3; ++row) {
            for (int col = 0; col < 8; ++col) {
                const auto& from = lastShownAngles[row][col];
                const auto& to = currentView[row][col];

                if (to.first != -1 && to.second != -1) {
                    float diffH = angularDistance(from.first, to.first);
                    float diffM = angularDistance(from.second, to.second);
                    globalMaxDiff = std::max(globalMaxDiff, std::max(diffH, diffM));
                }
            }
        }

        int steps = std::max(1, static_cast<int>(globalMaxDiff * MAXSTEP));
        const int delayMs = 5;

        // Interpolate all clocks from lastShownAngles → currentView
        for (int s = 0; s <= steps; ++s) {
            float t = static_cast<float>(s) / steps;
            window.clear(sf::Color::White);

            for (int row = 0; row < 3; ++row) {
                for (int col = 0; col < 8; ++col) {
                    const auto& from = lastShownAngles[row][col];
                    const auto& to = currentView[row][col];

                    if (to.first != -1 && to.second != -1) {
                        float h = interpolateAngle(from.first, to.first, t);
                        float m = interpolateAngle(from.second, to.second, t);
                        clocks[row][col].setInstant(h, m);
                        if (row == targetRow && col == targetCol) {
                            ClockMotion motion;
                            motion.hourAngle = normalize(h);
                            motion.minuteAngle = normalize(m);
                            sendClockMotionToReceptor(motion);
                        }
                        clocks[row][col].draw(window);
                    }
                }
            }

            window.display();
            std::this_thread::sleep_for(std::chrono::milliseconds(delayMs));
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(500));

        // Update for next round
        lastShownAngles = currentView;
    }
}


void slideTransition_to_top(sf::RenderWindow& window, vector<vector<Clock>>& clocks, 
                     const vector<vector<pair<float, float>>>& currentAngles,
                     const vector<vector<pair<float, float>>>& wordAngles,
                     const vector<vector<pair<float, float>>>& targetAngles) {
    
    size_t timeCols = currentAngles.size();
    size_t wordCols = wordAngles.size();
    size_t targetCols = targetAngles.size();
    size_t totalRows = 3; 
    
    vector<vector<pair<float, float>>> displayAngles;

    // Fill Current Hour 
    vector<vector<pair<float, float>>> currentBuffer(totalRows);
    for (size_t col = 0; col < timeCols; col++) {
        for (size_t row = 0; row < totalRows; row++) {
            currentBuffer[row].push_back(currentAngles[col][row]);
        }
    }

    displayAngles.insert(displayAngles.end(), currentBuffer.begin(), currentBuffer.end());

    // Fill Words in the middle 
    vector<vector<pair<float, float>>> wordBuffer;
    size_t chunkSize = 4;  // Each chunk is 4 letters
    size_t wordChunks = ((wordCols / 2) + chunkSize - 1) / chunkSize; 
    size_t wordRows = wordChunks *3;

    for (size_t chunk = 0; chunk < wordChunks; chunk++) {
        size_t startCol = chunk * chunkSize*2;
        for (size_t row = 0; row < totalRows; row++) {
            vector<pair<float, float>> rowData;
            for (size_t col = startCol; col < startCol + (chunkSize*2) && col < wordCols; col++) {
                rowData.push_back(wordAngles[col][row]);
            }
            if (wordBuffer.size() < wordRows) {
                wordBuffer.push_back(rowData);
            }
        }
    }

    // Insert into Display Buffer
    displayAngles.insert(displayAngles.end(), wordBuffer.begin(), wordBuffer.end());

    //Fill Target Hour at BOTTOM
    vector<vector<pair<float, float>>> targetBuffer(totalRows);
    for (size_t col = 0; col < targetCols; col++) {
        for (size_t row = 0; row < totalRows; row++) {
            targetBuffer[row].push_back(targetAngles[col][row]);
        }
    }

    displayAngles.insert(displayAngles.end(), targetBuffer.begin(), targetBuffer.end());

    // Animate the Transition
    std::vector<std::vector<std::pair<float, float>>> lastShownAngles = {
        displayAngles[0],
        displayAngles[1],
        displayAngles[2]
    };

    size_t totalSteps = displayAngles.size() - totalRows;
    const int delayMs = 5;

    for (size_t step = 1; step <= totalSteps; ++step) {
        // Build currentView from the next visible 3 rows
        std::vector<std::vector<std::pair<float, float>>> currentView(3);
        for (size_t row = 0; row < totalRows; ++row) {
            if (step + row < displayAngles.size()) {
                currentView[row] = displayAngles[step + row];
            }
        }

        // Compute globalMaxDiff for dynamic step sizing
        float globalMaxDiff = 0.0f;
        for (size_t row = 0; row < 3; ++row) {
            for (size_t col = 0; col < currentView[row].size(); ++col) {
                if (col < lastShownAngles[row].size()) {
                    float diffH = angularDistance(lastShownAngles[row][col].first, currentView[row][col].first);
                    float diffM = angularDistance(lastShownAngles[row][col].second, currentView[row][col].second);
                    globalMaxDiff = std::max(globalMaxDiff, std::max(diffH, diffM));
                }
            }
        }

        int steps = std::max(1, static_cast<int>(globalMaxDiff * MAXSTEP));

        // Interpolate from lastShownAngles → currentView
        for (int s = 0; s <= steps; ++s) {
            float t = static_cast<float>(s) / steps;
            window.clear(sf::Color::White);

            for (size_t row = 0; row < 3; ++row) {
                for (size_t col = 0; col < std::min((size_t)8, currentView[row].size()); ++col) {
                    if (col < lastShownAngles[row].size()) {
                        float fromH = lastShownAngles[row][col].first;
                        float fromM = lastShownAngles[row][col].second;
                        float toH = currentView[row][col].first;
                        float toM = currentView[row][col].second;

                        float h = interpolateAngle(fromH, toH, t);
                        float m = interpolateAngle(fromM, toM, t);

                        clocks[row][col].setInstant(h, m);
                        if (row == targetRow && col == targetCol) {
                            ClockMotion motion;
                            motion.hourAngle = normalize(h);
                            motion.minuteAngle = normalize(m);
                            sendClockMotionToReceptor(motion);
                        }
                        clocks[row][col].draw(window);
                    }
                }
            }

            window.display();
            std::this_thread::sleep_for(std::chrono::milliseconds(delayMs));
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(500));

        // Update lastShownAngles for the next step
        lastShownAngles = currentView;

        // Prevent overshoot (important!)
        if (step == totalSteps) break;
    }
}



void slideTransition_to_bottom(sf::RenderWindow& window, vector<vector<Clock>>& clocks, 
                                const vector<vector<pair<float, float>>>& currentAngles,
                                const vector<vector<pair<float, float>>>& wordAngles,
                                const vector<vector<pair<float, float>>>& targetAngles) {
    size_t timeCols = currentAngles.size();
    size_t wordCols = wordAngles.size();
    size_t targetCols = targetAngles.size();
    size_t totalRows = 3;  
    
    vector<vector<pair<float, float>>> displayAngles;

    // Fill Target Hour
    vector<vector<pair<float, float>>> targetBuffer(totalRows);
    for (size_t col = 0; col < targetCols; col++) {
        for (size_t row = 0; row < totalRows; row++) {
            targetBuffer[row].push_back(targetAngles[col][row]);
        }
    }

    displayAngles.insert(displayAngles.end(), targetBuffer.begin(), targetBuffer.end());

    // Fill Words in the middle 
    vector<vector<pair<float, float>>> wordBuffer;
    size_t chunkSize = 4;  // Each chunk is 4 letters
    size_t wordChunks = ((wordCols / 2) + chunkSize - 1) / chunkSize; 
    size_t wordRows = wordChunks * 3;

    for (size_t chunk = 0; chunk < wordChunks; chunk++) {
        size_t startCol = chunk * chunkSize * 2;
        for (size_t row = 0; row < totalRows; row++) {
            vector<pair<float, float>> rowData;
            for (size_t col = startCol; col < startCol + (chunkSize * 2) && col < wordCols; col++) {
                rowData.push_back(wordAngles[col][row]);
            }
            if (wordBuffer.size() < wordRows) {
                wordBuffer.push_back(rowData);
            }
        }
    }

    // Insert into Display Buffer
    displayAngles.insert(displayAngles.end(), wordBuffer.begin(), wordBuffer.end());


    // Fill Current Hour 
    vector<vector<pair<float, float>>> currentBuffer(totalRows);
    for (size_t col = 0; col < timeCols; col++) {
        for (size_t row = 0; row < totalRows; row++) {
            currentBuffer[row].push_back(currentAngles[col][row]);
        }
    }

    displayAngles.insert(displayAngles.end(), currentBuffer.begin(), currentBuffer.end());

    // Animate the Transition
    std::vector<std::vector<std::pair<float, float>>> lastShownAngles = {
        displayAngles[displayAngles.size() - 3],
        displayAngles[displayAngles.size() - 2],
        displayAngles[displayAngles.size() - 1]
    };

    size_t totalSteps = displayAngles.size() - totalRows;
    const int delayMs = 5;

    for (size_t step = totalSteps; step <= totalSteps; --step) {
        // Build currentView from the next visible 3 rows (moving downward)
        std::vector<std::vector<std::pair<float, float>>> currentView(3);
        for (size_t row = 0; row < 3; ++row) {
            if (step + row < displayAngles.size()) {
                currentView[row] = displayAngles[step + row];
            }
        }

        // Compute globalMaxDiff for dynamic step sizing
        float globalMaxDiff = 0.0f;
        for (size_t row = 0; row < 3; ++row) {
            for (size_t col = 0; col < currentView[row].size(); ++col) {
                if (col < lastShownAngles[row].size()) {
                    float diffH = angularDistance(lastShownAngles[row][col].first, currentView[row][col].first);
                    float diffM = angularDistance(lastShownAngles[row][col].second, currentView[row][col].second);
                    globalMaxDiff = std::max(globalMaxDiff, std::max(diffH, diffM));
                }
            }
        }

        int steps = std::max(1, static_cast<int>(globalMaxDiff * MAXSTEP));

        // Interpolate from lastShownAngles → currentView
        for (int s = 0; s <= steps; ++s) {
            float t = static_cast<float>(s) / steps;
            window.clear(sf::Color::White);

            for (size_t row = 0; row < 3; ++row) {
                for (size_t col = 0; col < std::min((size_t)8, currentView[row].size()); ++col) {
                    if (col < lastShownAngles[row].size()) {
                        float fromH = lastShownAngles[row][col].first;
                        float fromM = lastShownAngles[row][col].second;
                        float toH = currentView[row][col].first;
                        float toM = currentView[row][col].second;

                        float h = interpolateAngle(fromH, toH, t);
                        float m = interpolateAngle(fromM, toM, t);

                        clocks[row][col].setInstant(h, m);
                        if (row == targetRow && col == targetCol) {
                            ClockMotion motion;
                            motion.hourAngle = normalize(h);
                            motion.minuteAngle = normalize(m);
                            sendClockMotionToReceptor(motion);
                        }
                        clocks[row][col].draw(window);
                    }
                }
            }

            window.display();
            std::this_thread::sleep_for(std::chrono::milliseconds(delayMs));
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(500));

        // Update lastShownAngles
        lastShownAngles = currentView;

        // Important: Stop when step reaches 0 (avoid underflow)
        if (step == 0) break;
    }

}


void smoothSpinRevealTextThenTime(sf::RenderWindow& window,
                                  std::vector<std::vector<Clock>>& clocks,
                                  const std::vector<std::vector<std::pair<float, float>>>& startAngles,
                                  const std::vector<std::vector<std::pair<float, float>>>& wordAngles,
                                  const std::vector<std::vector<std::pair<float, float>>>& targetAngles) {

    float globalMaxDiff = 0.0f;
    for (int row = 0; row < 3; ++row) {
        for (int col = 0; col < 8; ++col) {
            float diff1 = angularDistance(startAngles[col][row].first, 270.0f);
            float diff2 = angularDistance(startAngles[col][row].second, 270.0f);
            globalMaxDiff = std::max(globalMaxDiff, std::max(diff1, diff2));
        }
    }

    int spinSteps = std::max(1, static_cast<int>(globalMaxDiff * MAXSTEP));
    int moveSteps = std::max(1, static_cast<int>(MAXSTEP * 90));  // reasonable value
    const float totalSpin = 360.0f;

    // Spin while interpolating toward (270°, 270°)
    for (int step = 0; step <= spinSteps; ++step) {
        float t = static_cast<float>(step) / spinSteps;
        float spin = totalSpin * t;

        window.clear(sf::Color::White);
        for (int row = 0; row < 3; ++row) {
            for (int col = 0; col < 8; ++col) {
                float hStart = startAngles[col][row].first;
                float mStart = startAngles[col][row].second;

                float h = interpolateAngle(hStart, 270.0f, t);
                float m = interpolateAngle(mStart, 270.0f, t);

                clocks[row][col].setInstant(fmod(h + spin, 360.f), fmod(m + spin, 360.f));
                clocks[row][col].draw(window);
            }
        }

        window.display();
        std::this_thread::sleep_for(std::chrono::milliseconds(8));
    }

    // Move from (270°, 270°) → wordAngles
    for (int step = 0; step <= moveSteps; ++step) {
        float t = static_cast<float>(step) / moveSteps;

        window.clear(sf::Color::White);
        for (int row = 0; row < 3; ++row) {
            for (int col = 0; col < 8; ++col) {
                float h = interpolateAngle(270.0f, wordAngles[col][row].first, t);
                float m = interpolateAngle(270.0f, wordAngles[col][row].second, t);

                clocks[row][col].setInstant(h, m);
                if (row == targetRow && col == targetCol) {
                    ClockMotion motion;
                    motion.hourAngle = normalize(h);
                    motion.minuteAngle = normalize(m);
                    sendClockMotionToReceptor(motion);
                }
                clocks[row][col].draw(window);
            }
        }
        window.display();
        std::this_thread::sleep_for(std::chrono::milliseconds(8));
    }

    std::this_thread::sleep_for(std::chrono::milliseconds(1000));

    // Spin while interpolating toward (270°, 270°) again
    for (int step = 0; step <= spinSteps; ++step) {
        float t = static_cast<float>(step) / spinSteps;
        float spin = totalSpin * t;

        window.clear(sf::Color::White);
        for (int row = 0; row < 3; ++row) {
            for (int col = 0; col < 8; ++col) {
                float hStart = wordAngles[col][row].first;
                float mStart = wordAngles[col][row].second;

                float h = interpolateAngle(hStart, 270.0f, t);
                float m = interpolateAngle(mStart, 270.0f, t);

                clocks[row][col].setInstant(fmod(h + spin, 360.f), fmod(m + spin, 360.f));
                clocks[row][col].draw(window);
            }
        }

        window.display();
        std::this_thread::sleep_for(std::chrono::milliseconds(8));
    }

    // Move from (270°, 270°) → targetAngles
    for (int step = 0; step <= moveSteps; ++step) {
        float t = static_cast<float>(step) / moveSteps;

        window.clear(sf::Color::White);
        for (int row = 0; row < 3; ++row) {
            for (int col = 0; col < 8; ++col) {
                float h = interpolateAngle(270.0f, targetAngles[col][row].first, t);
                float m = interpolateAngle(270.0f, targetAngles[col][row].second, t);

                clocks[row][col].setInstant(h, m);
                if (row == targetRow && col == targetCol) {
                        ClockMotion motion;
                        motion.hourAngle = normalize(h);
                        motion.minuteAngle = normalize(m);
                        sendClockMotionToReceptor(motion);
                }
                clocks[row][col].draw(window);
            }
        }

        window.display();
        std::this_thread::sleep_for(std::chrono::milliseconds(8));
    }
}
