# QueueManagement

Purely Educational :)

This is a simple queue management system that handle multiple queues in a single server with real time displays

It may only run on Linux and Mac OS

It can be split into 2 component
- admin
- display

Network Protocol: JSON on TCP

## Admin
For those that manages the queues

#### Protocol
- sign on: {"company":"c1", "queue_size":3, "msg_type":0}
- add queue: {"company":"c1", "msg_type":1, "id":1}
- pop queue: {"company":"c1", "msg_type":2}


## Display
For those waiting for their turn in the queue

#### Protocol
- connect display [persistent connection]: {"company":"c1", "msg_type":3} 

