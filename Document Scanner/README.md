# 📄 Document Scanner(C++)

A real-time **document scanner** built using **C++ and OpenCV** that detects a document from a webcam feed, applies perspective transformation, and displays a top-down scanned view—similar to popular mobile scanning apps.

---

## 🚀 Features

- 📷 Real-time document detection using webcam  
- 🧠 Edge detection & contour analysis  
- 📐 Automatic perspective (bird’s-eye) transformation  
- ✂️ Cropped and clean scanned output  
- 🎯 Detects only 4-corner documents  

---

## 🛠️ Tech Stack

- **Language:** C++  
- **Library:** OpenCV  
- **Concepts Used:**
  - Image preprocessing (Grayscale, Blur, Canny)
  - Morphological operations (Dilation)
  - Contour detection
  - Perspective transform
  - Webcam video capture

---

## 🧩 How It Works

1. Capture video frames from the webcam  
2. Preprocess image:
   - Convert to grayscale  
   - Apply Gaussian blur  
   - Detect edges using Canny  
   - Enhance edges using dilation  
3. Detect contours and select the largest 4-point contour  
4. Reorder the detected points (top-left → bottom-right)  
5. Apply perspective warp to get a scanned view  
6. Crop and display the final scanned document  

---

## ▶️ How to Run

### 1️⃣ Prerequisites

- C++ compiler (g++, clang, MSVC)
- OpenCV installed
- Working webcam

### 2️⃣ Compile

```bash
g++ document_scanner.cpp -o scanner `pkg-config --cflags --libs opencv4`
```

### 3️⃣ Run

```bash
./scanner
```

➡️ Press **ESC** to exit.

---

## 📐 Adjustable Parameters

```cpp
float w = 420, h = 596;
```

Modify these values to change the output document size.

---

## 🧠 Key Learning Outcomes

- Real-time computer vision pipeline
- Contour-based document detection
- Perspective correction using OpenCV
- Webcam-based image processing

---

## 🔮 Future Improvements

- Auto brightness & contrast enhancement  
- Save scanned documents as images or PDFs  
- Support for multiple documents  
- Noise reduction & shadow removal  
