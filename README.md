# Hand-Detection

* OpenCV 4.3.0
* C++
* Skin color detection

### "getImage" function

Blurred images are retrieved for each frame. Use a 'median filter' to remove noise.

### "skinColorDetection" function

Convert the color space from RGB to YCbCr to detect the skin area.

### "getCenter" function

Find the center of the palm using erode method

### "drawing" function

Draw a circle based on the center of the palm to indicate the area of the hand.

## result
![HandDetection1](https://user-images.githubusercontent.com/77608922/140678878-465e49c0-9069-42af-bbf0-6fbafb9b7f44.gif)
![HandDetection2](https://user-images.githubusercontent.com/77608922/140678884-10bf3320-e7da-41ec-ab75-67254461939b.gif)
![HandDetection3](https://user-images.githubusercontent.com/77608922/140678886-980f9e20-c999-4364-8136-93f29704adcc.gif)
![HandDetection4](https://user-images.githubusercontent.com/77608922/140678888-676859fb-3160-49b9-b8dc-713af37cfd30.gif)
