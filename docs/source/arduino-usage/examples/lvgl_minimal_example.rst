LVGL Minimal Examples
=================================

This example shows a basic usage with `LVGL <https://lvgl.io>`_.

In this example, we make a ``lv_helper.cpp`` and ``lv_helper.h`` makes it easy to implement LVGL in Arduino.
And it is a minimal example for you to start with LVGL

Checkout the `LVGL documentation <https://docs.lvgl.io>`_ for more information.

.. note::

    If you haven't download the code:

    .. include:: ../download-code.txt


.. note::

    If you don't have lvgl installed, check this out: :ref:`install_lvgl`.

Open ``termod-s3/examples/lv_example/lv_example.ino`` with Arduino IDE.

Remember to select ``ESP32S3 Dev Module`` and port, then click upload.

`Source code <https://github.com/TAMCTec/termod-s3/tree/main/lv_minimal_example/lv_minimal_example>`_

.. tabs::

    .. tab:: lv_minimal_example.ino

        .. include:: ../../../../examples/lv_minimal_example/lv_minimal_example.ino
            :code: cpp

    .. tab:: lv_helper.cpp

        .. include:: ../../../../examples/lv_minimal_example/lv_helper.cpp
            :code: cpp

    .. tab:: lv_helper.h

        .. include:: ../../../../examples/lv_minimal_example/lv_helper.h
            :code: cpp
