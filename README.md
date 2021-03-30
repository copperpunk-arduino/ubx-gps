# UbxIns
Implements UbxParser class. Dispatches INS-related messages

The **UbxIns** class inherits from [UbxParser](https://github.com/copperpunk-arduino/ubx-parser). These were separated so that UbxParser could be used for any U-blox UBX message, and the implementation of each message could be left to the derived classes.

Currently NAV-POSLLH, NAV-PVT, and NAV-RELPOSNED messages are supported. If you need to support other messages, please fork this repo and add your own implementation.

The **ParseNavPosLlhMessage** example demonstrates how to use the `read` and `processMessage` functions.

If you have any suggestions for improving this library, there are a few ways to get in touch:<br>

*   Create a new issue
*   Submit a pull request
*   Virtual meeting using our [Open Source Office Hours](https://www.copperpunk.com/service-page/open-source-office-hours)
