# ubx-ins
Implements UbxParser class. Dispatches INS-related messages

The *UbxIns* class inherits from *UbxParser*. These were separated so that UbxParser could be used for any U-blox UBX message, and the implementation of each message could be left to the derived classes.
