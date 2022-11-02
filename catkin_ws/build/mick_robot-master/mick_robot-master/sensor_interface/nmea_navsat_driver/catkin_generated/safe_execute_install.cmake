execute_process(COMMAND "/home/ks/tutorial_ws/build/mick_robot-master/mick_robot-master/sensor_interface/nmea_navsat_driver/catkin_generated/python_distutils_install.sh" RESULT_VARIABLE res)

if(NOT res EQUAL 0)
  message(FATAL_ERROR "execute_process(/home/ks/tutorial_ws/build/mick_robot-master/mick_robot-master/sensor_interface/nmea_navsat_driver/catkin_generated/python_distutils_install.sh) returned error code ")
endif()
