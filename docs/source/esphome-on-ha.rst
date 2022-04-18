
ESPHome on Home Assistant
=======================
.. note::
  ESP32 C3 on ESPHome is currently in development, so you need to install the dev version of it.

Install Home Assistant
----------------------

Home Assistant is a greate local IoT Platform which connect different devices and services together. You can install it on your own computer or Raspberry Pi which is recommended. To install Home Assistant, you need to install and setup Home Assistant, go to https://www.home-assistant.io/.

Install ESPHome
----------------
1. After installing Home Assistant, you can install ESPHome on your Home Assistant. Open your home assistant page, which normally http://homeassistant.local:8123/. Go through :guilabel:`Configuration` => :guilabel:`Add-ons, Backups & Supervisor`

.. image:: images/sense-node-c3-software-install-esphome-1.png
  :align: center

2. Click on the right bottom :guilabel:`ADD_ON STORE` button.

.. image:: images/sense-node-c3-software-install-esphome-2.png
  :align: center

3. Click on the top right three dots. Then click on :guilabel:`Reposotories`.

.. image:: images/sense-node-c3-software-install-esphome-3.png
  :align: center

4. Paste in the esphome repo url: ``https://github.com/esphome/hassio`` and click :guilabel:`Add`.

.. image:: images/sense-node-c3-software-install-esphome-4.png
  :align: center

.. note::
  Do not install it from :guilabel:`Home Assistant Community Add-ons` which is the release version.
5. After repository added, go back the add on store, you should see a category called :guilabel:`ESPHome` with 3 options. click on the (dev) versions.

.. image:: images/sense-node-c3-software-install-esphome-5.png
  :align: center

6. Now you are in ESPHome(dev) Add-on page, click on :guilabel:`Install` button to install it.

.. image:: images/sense-node-c3-software-install-esphome-6.png
  :align: center

7. After installation, you can see the :guilabel:`Start` button. Click on it to start the ESPHome. There also a :guilabel:`Show in sidebar` option you can turn on to stick ESPHome on the side bar for easy access.

.. image:: images/sense-node-c3-software-install-esphome-7.png
  :align: center

8. After ESPHome is started, click on :guilabel:`OPEN WEB UI` button to open the web UI. If everything go well, you are now at ESPHome page.

.. note::
  This may failed if you click a little too quick. as it need some time to actually start. if it goes wrong, you can try again. If it still failed, click on the :guilabel:`Log` tab on top to see the error, and maybe search it on the internet or issue on GitHub: https://github.com/esphome/issue. 

Setup Sense Node C3
--------------------
1. Now to add your sense node C3, click on the :guilabel:`NEW DEVICE` button. Type in the name you want it to be, like "living-room-pir" or someting. It will prompt you to input the Wi-Fi SSID and password the first time. This will save in your ESPHome SECRETS settings for all boards. So type in your Wi-Fi SSID and password and click on :guilabel:`NEXT` button.

.. image:: images/sense-node-c3-software-setup-esphome-1.png
  :align: center

2. Then, choose ESP32 and click on :guilabel:`NEXT` button.

.. image:: images/sense-node-c3-software-setup-esphome-2.png
  :align: center

3. It's done! Click :guilabel:`SKIP` as it need more configurations.

.. image:: images/sense-node-c3-software-setup-esphome-3.png
  :align: center

4. Now you can see the device on the main page. Click on :guilabel:`EDIT`.

.. image:: images/sense-node-c3-software-setup-esphome-4.png
  :align: center

5. You can see the YAML file with the basic configuration. Now we are going to set this up with our pre made template. Add the following to the very top, remember to change name and upper_name to your own:

.. code-block:: yaml

  substitutions:
    name: living-rom-pir
    upper_name: Living Room PIR

  packages:
    remote_package: github://tamctec/sense-node-c3/sense-node-c3-template.yaml@main

6. As the template sets everything up for you, remove the rest of the file leaving `ota` and `wifi` and the file will look like this:

.. code-block:: yaml

  substitutions:
    name: living-room-pir
    upper_name: Living Room PIR

  packages:
    remote_package: github://tamctec/sense-node-c3/sense-node-c3-template.yaml@main

  ota:
    password: "34b55d940e9f2b7be567bc8ff20815f5"

  wifi:
    ssid: !secret wifi_ssid
    password: !secret wifi_password

    # Enable fallback hotspot (captive portal) in case wifi connection fails
    ap:
      ssid: "Living-Room-Pir Fallback Hotspot"
      password: "KRwvs978cVbb"

7. Click on :guilabel:`INSTALL` it will save automatically. The first time uploading the firmware, you need to use a USB cable to connect to the board with the Raspberry Pi or a computer your HA is installed on.

.. image:: images/sense-node-c3-software-setup-esphome-5.png
  :align: center

8. And choose the third option, and then choose the coresponding port, which is usually :guilabel:`USB JTAG/serial debug unit`

.. image:: images/sense-node-c3-software-setup-esphome-6.png
  :align: center

.. note::
  If it says **No serial devices found**, try again or check :ref:`faq:no-serial-device-found`.

9. Wait a while for the firmware to be uploaded.

.. image:: images/sense-node-c3-software-setup-esphome-7.png
  :align: center

10. Now click :guilabel:`Configuration` on the left sidebar, and then :guilabel:`Device & Services`

.. image:: images/sense-node-c3-software-setup-esphome-8.png
  :align: center

11. A new device should come up, click :guilabel:`CONFIGURE` to add it to Home Assistant.

.. image:: images/sense-node-c3-software-setup-esphome-9.png
  :align: center

12. Now your device is added! Go to Overview page, add the entities the template made. It should be ``{Your Device Name} Status`` and ``{Your Device Name} Battery Level``. Try to turn the status light on and off to see if it works.

.. image:: images/sense-node-c3-software-setup-esphome-10.png
  :align: center


Add a sensor
--------------------
Now is time to add a sensor to the sense node. ESPHome is capable for a lot of sensors, here we are going to add a PIR sensor as an example. For more information, please refer to the `ESPHome <https://esphome.io/index.html>`__

1. Go to ESPHome(dev) again. If you have add it to the sidebar, just click on it. Or, go through the :guilabel:`Configuration` => :guilabel:`Add-ons, Backups & Supervisor` and click on :guilabel:`ESPHome` => :guilabel:`OPEN WEB UI`.
2. On ESPHome page, click on edit under your device.
3. Now in the yaml editor, add the sensor you want like this:

.. code-block:: yaml

  binary_sensor:
    - platform: gpio
      pin: 7
      name: "Sense Node PIR"
      device_class: motion

4. Click the bottom right :guilabel:`INSTALL` button. Now maybe you can upload the code wirelessly, If it shows your device as :guilabel:`ONLINE`, choose the first option :guilabel:`Wirelessly`. But mine never shows :guilabel:`ONLINE` even it works great on :guilabel:`Overview` page. If so, install it like before.
5. After installation is done, you can now add a battery voltage log and the sensor state to :guilabel:`Overview` page.

.. image:: images/sense-node-c3-software-setup-esphome-11.png
  :align: center
