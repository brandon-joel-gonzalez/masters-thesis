# CMake generated Testfile for 
# Source directory: /home/brandon/Desktop/Spring_2021/ROBO597/opencv-master/modules/ml
# Build directory: /home/brandon/Desktop/Spring_2021/ROBO597/build/modules/ml
# 
# This file includes the relevant testing commands required for 
# testing this directory and lists subdirectories to be tested as well.
add_test(opencv_test_ml "/home/brandon/Desktop/Spring_2021/ROBO597/build/bin/opencv_test_ml" "--gtest_output=xml:opencv_test_ml.xml")
set_tests_properties(opencv_test_ml PROPERTIES  LABELS "Main;opencv_ml;Accuracy" WORKING_DIRECTORY "/home/brandon/Desktop/Spring_2021/ROBO597/build/test-reports/accuracy")
