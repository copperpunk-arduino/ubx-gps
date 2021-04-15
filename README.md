# UbxGps
The **UbxGps** class utilizes the [UbxInterpreter](https://github.com/copperpunk-arduino/ubx-interpreter) class. These were separated so that UbxInterpreter could be used for any U-blox UBX message, and the implementation of each message could be left to more specific classes.

This class will process NAV-POSLLH, NAV-PVT, and NAV-RELPOSNED messages. If you need to support other messages, either fork this repo and add your own implementation, or just use this class as a guide. 

The [ParseNavPosLlhMessage](https://github.com/copperpunk-arduino/ubx-ins/tree/main/examples/ParseNavPosLlhMessage) example demonstrates how to use the `checkForMessage`, `processMessage`, and `clearFix` functions. This setup requires a single GPS receiver.

The [DualGpsHeading](https://github.com/copperpunk-arduino/ubx-gps/tree/main/examples/DualGpsHeading) example utilizes the [ArduSimple simpleRTK2B+heading kit](https://www.ardusimple.com/product/simplertk2b-heading-basic-starter-kit-ip67/). You must define the distance between the antennas, and the accuracy required in order to accept the relative heading solution (using the overloaded constructor). For example, if the antennas were 1000mm apart, you might set the accuracy threshold at 100mm, such that if the measured distance were in the range 900mm-1100mm, the solution would be considered valid.<br>
>The relative heading is based on the line drawn from the **BASE** board to the **HEADING** board.

<br><br>
## <u>Contact Us</u>
If you have any suggestions for improving this library, there are a few ways to get in touch:<br>

*   Create a new issue
*   Submit a pull request
*   Virtual meeting using our [Open Source Office Hours](https://www.copperpunk.com/service-page/open-source-office-hours)
