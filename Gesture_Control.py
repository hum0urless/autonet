import cv2
import mediapipe as mp
import numpy as np
import time

# Initialize Hand Tracking
mp_hands = mp.solutions.hands
hands = mp_hands.Hands(min_detection_confidence=0.7, min_tracking_confidence=0.7)
mp_draw = mp.solutions.drawing_utils

# Open Webcam
cap = cv2.VideoCapture(0)
quit_start_time = None  # Track time for quit gesture

while cap.isOpened():
    success, img = cap.read()
    if not success:
        continue

    img = cv2.flip(img, 1)
    img_rgb = cv2.cvtColor(img, cv2.COLOR_BGR2RGB)
    results = hands.process(img_rgb)

    command = "NONE"
    
    if results.multi_hand_landmarks:
        hand_positions = []  # Store wrist positions for both hands

        for hand_landmarks in results.multi_hand_landmarks:
            index_tip = hand_landmarks.landmark[8]
            wrist = hand_landmarks.landmark[0]
            thumb_tip = hand_landmarks.landmark[4]
            thumb_base = hand_landmarks.landmark[2]
            
            # Store wrist positions for quit detection
            hand_positions.append((wrist.x, wrist.y))
            
            # Calculate hand distance (approximation)
            distance = np.linalg.norm([index_tip.x - wrist.x, index_tip.y - wrist.y])
            speed = "FASTER" if distance > 0.3 else "MEDIUM" if distance > 0.2 else "SLOW"
            
            # Gesture: Move Forward
            if index_tip.y < wrist.y:
                command = f"FORWARD ({speed})"
            # Gesture: Turn Right
            elif index_tip.x > wrist.x:
                command = f"TURN_RIGHT ({speed})"
            # Gesture: Turn Left
            elif index_tip.x < wrist.x:
                command = f"TURN_LEFT ({speed})"
            # Gesture: Move Backward (Thumbs Down)
            elif thumb_tip.y > thumb_base.y:
                command = f"BACKWARD ({speed})"
            else:
                command = "STOP"
            
            # Draw hand landmarks
            mp_draw.draw_landmarks(img, hand_landmarks, mp_hands.HAND_CONNECTIONS)
        
        # Gesture: Quit Program (Hands Folded for 3 seconds)
        if len(hand_positions) == 2:  # If both hands are detected
            wrist_distance = np.linalg.norm(np.array(hand_positions[0]) - np.array(hand_positions[1]))
            if wrist_distance < 0.05:  # Hands are very close together
                if quit_start_time is None:
                    quit_start_time = time.time()
                elif time.time() - quit_start_time > 3:
                    print("Quitting program...")
                    break
            else:
                quit_start_time = None
    else:
        quit_start_time = None
    
    # Display the detected command
    cv2.putText(img, f"Gesture: {command}", (10, 50), cv2.FONT_HERSHEY_SIMPLEX, 
                1, (0, 255, 0), 2, cv2.LINE_AA)
    print(command)
    
    cv2.imshow("Gesture Recognition", img)

    # Exit on 'q' key press
    if cv2.waitKey(1) & 0xFF == ord("q"):
        break

cap.release()
cv2.destroyAllWindows()
