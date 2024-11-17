import cv2 # type: ignore
import pytesseract # type: ignore
import websocket # type: ignore
import json

# Configure WebSocket connection
ws = websocket.WebSocket()
ws.connect("ws://localhost:8080")

# Tesseract OCR setup
pytesseract.pytesseract.tesseract_cmd = r'C:\Program Files\Tesseract-OCR\tesseract.exe'

def detect_vehicle_number(image_path):
    """Detect vehicle number using OCR."""
    image = cv2.imread(image_path)
    gray = cv2.cvtColor(image, cv2.COLOR_BGR2GRAY)
    vehicle_number = pytesseract.image_to_string(gray, config='--psm 8')
    return vehicle_number.strip()

# Simulate video capture
video = cv2.VideoCapture(0)  # 0 for webcam

while True:
    ret, frame = video.read()
    if not ret:
        break

    # Save frame and process it
    cv2.imwrite("frame.jpg", frame)
    detected_number = detect_vehicle_number("frame.jpg")
    print(f"Detected Vehicle Number: {detected_number}")

    # Send vehicle number to WebSocket
    ws.send(json.dumps({"vehicleNumber": detected_number}))

    # Display frame
    cv2.imshow("Vehicle Detection", frame)

    # Exit on 'q'
    if cv2.waitKey(1) & 0xFF == ord('q'):
        break

video.release()
cv2.destroyAllWindows()
