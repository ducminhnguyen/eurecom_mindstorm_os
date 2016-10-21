# EURECOM OS Contest Server
# Main website: http://soc.eurecom.fr/OS/projects_fall2016.html

A simple server that enables bluetooth communication between a set EV3 Lego Mindstorm.

It is first intended to be used in the OS course of EURECOM but part or all of it can be freely reused by anyone.

## Setup

### For EV3

Follow the steps from [http://www.ev3dev.org/docs/getting-started/](http://www.ev3dev.org/docs/getting-started/) to download the ev3dev kernel image and flash it on the SD card.

If you do not have blueman, set it up as described: [http://www.ev3dev.org/docs/tutorials/connecting-to-the-internet-via-bluetooth/](http://www.ev3dev.org/docs/tutorials/connecting-to-the-internet-via-bluetooth/).

Turn on the EV3. Unpair the device on the server. Pair again from the server: enter PIN code and reenter it on EV3. Connect by SSH on the EV3. On both the server and the EV3 the command
```
$ hcitool scan
```

should show the connection.

When the server is up and the game has started you can run the program (`client/NXT/client.c` for instance).


### For a fake client

A fake client is provided under `client/Fake` so that you can try exchanging messages with your robot. In order to use it you have
to add or replace a team in the team file that will correspond to the fake client. You have to specify a type of `3` and give the
IP address from which the fake client will connect (`127.0.0.1` if it will be running on the same computer as the server):

```
3 127.0.0.1 Fake
```

Once a game is started, you can run the fake client by giving it the IP address of the server (`127.0.0.1` if it is running on the
same computer as the fake client) and the ID of the team corresponding to the fake client:

```
$ ./client 127.0.0.1 0
```

You can run multiple client from the same IP address but the server will consider that the first to connect is the one with the
smallest ID. So you'll want to be careful about the order in which the clients are started.


### For the server

You can compile the server using the provided Makefile. If you run into an error stating that `bluetooth/bluetooth.h` was not found,
you will need to install the `libbluetooth-dev` package (on Debian-like UNIX).

Run the server with an appropriate team file. Each line of the team file - provided as input to the server - should be formatted as:
```
[TYPE] [ADDR] [NAME]
```
where:
* `[TYPE]` : `1` for NXT, `2` for EV3
* `[ADDR]` : in the form `aa:bb:cc:dd:ee:ff` is the bluetooth address
* `[NAME]` : is the name of the team

You can then run the server as
```
$ ./server teams
```
or
```
$ ./server -o log teams
```
if you wish to log the session.

## Protocol

To communicate between each other, NXT and EV3 must comply with the specified protocol. Invalid messages will be discarded by the
server so robots can always consider that received messages are well formatted (except for `CUSTOM` messages, whose structure is
not pre-determined).

Each message consists of a header and a body. Note that all numbers are unsigned integers whose formats are **little-endian**.

An example trace for a standard exchange is provided in the following sequence diagram:
![Sequence Diagram](robotseq.png)

The protocol may evolve according to needs and proposals.

### Header

The header is 5-bytes long:

```
   0      1      2      3      4
+------+------+------+------+------+
|     ID      | src  | dst  | type |
+------+------+------+------+------+
```

Fields description:
* `ID` is a 2-byte number identifying the message (kind of like a sequence number). It is used when acknowledging messages.
* `src` is a 1-byte number identifying the team who sent the message (it is unique for the whole contest).
* `dst` is a 1-byte number identifying the team who should receive the message (it is unique for the whole contest).
* `type` is a 1-byte code that identify the kind of message that is sent.

### Body

#### ACTION

ACTION messages are used to advertise an intended movement. They are 10-bytes long:
```
    0       1       2       3       4       5       6       7       8       9
+-------+-------+-------+-------+-------+-------+-------+-------+-------+-------+
|      ID       |  src  |  dst  |   0   |     angle     | dist  |     speed     |
+-------+-------+-------+-------+-------+-------+-------+-------+-------+-------+
```

Fields description:
* `angle` is a 2-byte number representing the planned direction (in degree). Values over 360 have undefined meaning. This angle is given in an absolute value, that is, the difference between the intended angle and the angle measured at the begining of the game. As previously said, 2-byte numbers must be sent in little-endian coding. With the little endian coding, an angle of 350 gives `0x5E | 0x01`, or in binary format: `01011110 | 00000001`. 
* `dist` is a 1-byte number representing the planned distance (in cm).
* `speed` is a 2-byte number representing the planned speed (in mm/s).

#### ACK

ACK messages are used to acknowledge the reception of messages. They are 8-byte long:
```
    0       1       2       3       4       5       6       7
+-------+-------+-------+-------+-------+-------+-------+-------+
|      ID       |  src  |  dst  |   1   |    ID ack     | state |
+-------+-------+-------+-------+-------+-------+-------+-------+
```

Fields description:
* `ID ack` is the ID of the message that is acknowledged
* `state` is a status code. `0 -> OK`, `1 -> error`. Other status codes may be used for acknowledging custom messages.

START and STOP messages should not be acknowledged.

#### LEAD

LEAD messages should only be used by the current leader in order to transfer leadership to the next robot. They are 5-byte long:
```
    0       1       2       3       4
+-------+-------+-------+-------+-------+
|      ID       |  src  |  dst  |   2   |
+-------+-------+-------+-------+-------+
```

#### START

START messages can only be used by the server. One is sent to each team when the game starts. If the robot disconnect and reconnect
during the game, another START message will be sent to it right after it connects to the server. They are 9-byte long:
```
    0       1       2       3       4       5       6       7       8
+-------+-------+-------+-------+-------+-------+-------+-------+-------+
|      ID       |  src  |  dst  |   3   | rank  | size  | prev  | next  |
+-------+-------+-------+-------+-------+-------+-------+-------+-------+
```

Fields description:
* `rank` is the rank of the robot in the snake. It can be anything from `0` for the leader to `size-1` for the last robot.
* `size` is the length of the snake, that is, how many robots are participating in this game.
* `prev` is the ID of the previous robot in the snake. If there is no previous robot it will be `0xFF`.
* `next` is the ID of the next robot in the snake. If there is no next robot it will be `0xFF`.

#### STOP

STOP messages are sent by server to every robot when the game ends. They are 5-bytes long:
```
    0       1       2       3       4
+-------+-------+-------+-------+-------+
|      ID       |  src  |  dst  |   4   |
+-------+-------+-------+-------+-------+
```

#### WAIT

WAIT messages can be sent to the robot in front to request for a halt. It is up to the receiving robot to answer or ignore them.
WAIT messages are 6-bytes long.
```
    0       1       2       3       4       5
+-------+-------+-------+-------+-------+-------+
|      ID       |  src  |  dst  |   5   | delay |
+-------+-------+-------+-------+-------+-------+
```

Fields description:
* `delay` is the reqested halt time (in s).

#### CUSTOM

CUSTOM messages may be used to increment the protocol if teams wish to add their own custom messages. CUSTOM messages can not have
a size greater than 58 bytes (header included).
```
    0       1       2       3       4        5      . . .
+-------+-------+-------+-------+-------+-----------------
|      ID       |  src  |  dst  |   6   |   payload . . .
+-------+-------+-------+-------+-------+-----------------
```

#### KICK

KICK messages can only be used by the server. This message is used to advertise that a robot got kicked out of the game. It is sent
to every robot in the game. The message is 8-bytes long:
```
    0       1       2       3       4       5       6       7
+-------+-------+-------+-------+-------+-------+-------+-------+
|      ID       |  src  |  dst  |   7   | rank  | prev  | next  |
+-------+-------+-------+-------+-------+-------+-------+-------+
```

Fields description:
* `rank` is the rank of the robot that was kicked. It is the rank the robot was granted at the begining of the game, and not its current actual rank.
* `prev` is the ID of the previous robot in the snake. If there is no previous robot it will be `0xFF`.
* `next` is the ID of the next robot in the snake. If there is no next robot it will be `0xFF`.

#### CANCEL

CANCEL messages are used to notify that the previous advertised movement has been aborted. They are 6-bytes long:
```
    0       1       2       3       4       5
+-------+-------+-------+-------+-------+-------+
|      ID       |  src  |  dst  |   8   | dist  |
+-------+-------+-------+-------+-------+-------+
```

Fields description:
* `dist` is a 1-byte number representing the distance (in cm) crossed since the aborted movement was advertised. A value of `0xFF` is used if this distance is unknown.
