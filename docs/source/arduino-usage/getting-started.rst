Getting Started for Arduino
=============================

Download Arduino IDE
------------------------

.. note:: If you already installed Arduino IDE, skip this step

1. Turn to `Arduino IDE download page`_, download and install Arduino IDE.

.. image:: images/arduino-usage-getting-started-download-arduino-ide.png

Add ESP32 Series
------------------------

.. note:: If you already added ESP32, skip this step

Open Arduino IDE. Click top left **File** menu, select **Preference**.

.. image:: images/arduino-usage-getting-started-open-preference.png

On **Preference** page, click the right-most icon at line **Additional Boards Manager URLS**.

.. image:: images/arduino-usage-getting-started-open-additional-boards-manager-urls.png

On **Additional Boards Manager URLS** page, Past the link 

    https://raw.githubusercontent.com/espressif/arduino-esp32/gh-pages/package_esp32_index.json

and Click **OK**

.. image:: images/arduino-usage-getting-started-open-additional-boards-manager-urls-ok.png

Close **Preference** window, go to **Tools** => **Board** => **Boards Manger..** open Boards Manger Page.

.. image:: images/arduino-usage-getting-started-open-boards-manager.png

On **Boards Manger** page, search for ``ESP32`` and click **Install** button.

.. image:: images/arduino-usage-getting-started-open-boards-manager-install.png


Install FT62X6 Library
------------------------

This is a library for touch screen.

Open Arduino IDE. Click **Sketch** menu, select **clude Library** then **Manage Libraries..**.
    
.. image:: images/arduino-usage-getting-started-open-manage-libraries.png
    
On **Manage Libraries** page, search for ``TAMC_FT62X6`` and click **Install** button.
    
.. image:: images/arduino-usage-getting-started-open-manage-libraries-install-TAMC_FT62X6.png


Install TFT_eSPI Library
------------------------

This is a library for TFT display. There are also other options, but we recommand this one.

**Install**

Open Arduino IDE. Click **Sketch** menu, select **clude Library** then **Manage Libraries..**.
    
.. image:: images/arduino-usage-getting-started-open-manage-libraries.png
    
On **Manage Libraries** page, search for ``TFT_eSPI`` and click **Install** button.
    
.. image:: images/arduino-usage-getting-started-open-manage-libraries-install-TFT_eSPI.png

**Setup for TAMC Termod S3**

After install TFT_eSPI Library, open File Explorer and go to Arduino library folder. Usually is under the following folder.

- For Windows: ``C:\Users\<USER>\Documents\Arduino\libraries``
- For Mac: ``/Users/<USER>/Documents/Arduino/libraries``
- For Linux: ``/home/<USER>/Documents/Arduino/libraries``

Open ``TFT_eSPI`` folder, and open ``User_Setup_Select.h`` file with the editor you like.

Comment out the line: ``#include <User_Setup.h>``, and uncomment the line: ``#include <User_Setups/Setup300_TAMC_Termod_S3.h>``, and the file will look like this:

.. code-block:: cpp
    :linenos:

    ...

    // #include <User_Setup.h>

    ...

    #include <User_Setups/Setup300_TAMC_Termod_S3.h>

    ...

Install LVGL Library (Optional)
-----------------------------------------

`LVGL <https://lvgl.io/>`_ is an amazing GUI library, make it easy to build modern UI.

.. warning::

    Termod S3 uses SPI display, has not enough refresh rate to perfactly support LVGL. There will be some tearing when scrolling.

**Install**

Open Arduino IDE. Click **Sketch** menu, select **clude Library** then **Manage Libraries..**.
    
.. image:: images/arduino-usage-getting-started-open-manage-libraries.png
    
On **Manage Libraries** page, search for ``LVGL`` and click **Install** button.

.. image:: images/arduino-usage-getting-started-open-manage-libraries-install-LVGL.png

**Setup LVGL**

After install LVGL Library, open File Explorer and go to Arduino library folder. Usually is under the following folder.

- For Windows: ``C:\Users\<USER>\Documents\Arduino\libraries``
- For Mac: ``/Users/<USER>/Documents/Arduino/libraries``
- For Linux: ``/home/<USER>/Documents/Arduino/libraries``

Open ``lvgl`` folder, and copy ``lv_conf_template.h`` file to Arduino library folder, alongside lvgl folder, not under lvgl. Like this:

.. image:: images/arduino-usage-getting-started-copy-lv-conf-template.png

Then, rename it to ``lv_conf.h``, open it with your favorate editor, and change first non-comment line ``if 0`` to ``if 1``

.. code-block:: cpp

    /**
     * @file lv_conf.h
     * Configuration file for v8.3.1
     */

    /*
     * Copy this file as `lv_conf.h`
     * 1. simply next to the `lvgl` folder
     * 2. or any other places and
     *    - define `LV_CONF_INCLUDE_SIMPLE`
     *    - add the path as include path
     */

    /* clang-format off */
    #if 1 /*Set it to "1" to enable content*/

    #ifndef LV_CONF_H
    #define LV_CONF_H

    #include <stdint.h>

    /*====================
       COLOR SETTINGS
     *====================*/

    /*Color depth: 1 (1 byte per pixel), 8 (RGB332), 16 (RGB565), 32 (ARGB8888)*/
    #define LV_COLOR_DEPTH 16

    /*Swap the 2 bytes of RGB565 color. Useful if the display has an 8-bit int


Build and upload
---------------------

Now everything is ready to build and upload. Make sure **Board** is set to ``TAMC Termod S3``.

.. image:: images/arduino-usage-getting-started-build-upload-select-board.png


.. _Arduino IDE download page: https://www.arduino.cc/en/software