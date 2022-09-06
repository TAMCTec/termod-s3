.. _faq:

FAQ
=======================

Error opening serial port 'COM15'.
--------------------------------------------

Make sure you have choose the coresponding port under **Tools** => **Port**, the port wil have a (ESP32 Dev Module) after it.

.. image:: images/faq-choose-port.png

XXXX is not defined
--------------------------------------------

if something like these is not defined:

- ``BAT_LV``
- ``CHG``
- ``TFT_CS``
- ``TFT_DC``
- ``TFT_RST``
- ``TFT_BCKL``
- ``SD_CS``
- ``SD_CD``
- ``DISPLAY_PORTRAIT``
- ``DISPLAY_LANDSCAPE``
- ``DISPLAY_PORTRAIT_FLIP``
- ``DISPLAY_LANDSCAPE_FLIP``
- ``DISPLAY_WIDTH``
- ``DISPLAY_HEIGHT``
- ``getBatteryVoltage``
- ``getBatteryCapacity``
- ``getChargingState``
- ``setOnChargeStart``
- ``setOnChargeEnd``

Make sure you have select ``TAMC Termod S3`` under **Tools** => **Board**.

.. image:: arduino-usage/images/arduino-usage-getting-started-build-upload-select-board.png

the selected serial port [22712] Failed to execute script 'esptool' due to unhandled exception! does not exist or your board is not connected
--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

Make sure you have choose the coresponding port under **Tools** => **Port**, the port wil have a (ESP32 Dev Module) after it.

If you do have the correct port selected, try forcing te board to flash mode, by holding down th IO0 button, and press and release the reset button, then release the IO0 button. After it's in flash mode, make sure check again the port is selected, as the port number might change.

After a manual reset, the board is not able to restart after upload done. yYou also need to manually reset the board after upload.
